
#include "node.h"

#include <string.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <iostream>

#define MAX_EPOLL_EVT   10
#define EPOLL_TIMEOUT   300

struct epoll_event event;

static void accept_conn(int sockfd, int epollfd, void *_node) {
    auto node = (Node *)_node;
    struct sockaddr_in clientaddr;

    socklen_t len = sizeof(struct sockaddr);
    int conn_fd = 0;
 
    conn_fd = accept(sockfd, (struct sockaddr*)&clientaddr, &len);
    if (conn_fd <= 0) {
        perror("accept error");
        return;
    }

    event.data.fd = conn_fd;
    event.events = EPOLLIN | EPOLLET;
    if(epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_fd, &event))
        perror("server connection");
    else {
        uint32_t client_ip = ntohl(clientaddr.sin_addr.s_addr);
        uint16_t port = ntohs(clientaddr.sin_port);

        std::cout << "server: connected ip = " << inet_ntoa(clientaddr.sin_addr) << " port: " << port << std::endl;

        // TODO: initialize new node
        Node new_node("temp-node", inet_ntoa(clientaddr.sin_addr), port);
        node->add_node(conn_fd, new_node);
    }
}

static void recv_message(int sockfd) {
    char recv_buf[1024*1024], send_buf[1024*1024];
    memset(recv_buf, 0, sizeof(recv_buf));
    memset(send_buf, 0, sizeof(send_buf));

    recv(sockfd, recv_buf, sizeof(recv_buf), 0);
    fputs(recv_buf, stdout);
    strcpy(send_buf, recv_buf);
    send(sockfd, send_buf, sizeof(send_buf), 0);
 
    return;
}

void *node_listener(void *args) {
    auto node = (Node *)args;

    int epollfd, ret;
    struct epoll_event event, evt_list[MAX_EPOLL_EVT];
    
    epollfd = epoll_create(10);
    event.events = EPOLLIN | EPOLLET | EPOLLHUP | EPOLLRDHUP;
    event.data.fd = node->ss;

    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, node->ss, &event) < 0) {
        perror("register epoll");
        return nullptr;
    }

    while (true) {
        ret = epoll_wait(epollfd, evt_list, MAX_EPOLL_EVT, EPOLL_TIMEOUT);

        if (ret < 0) {
            perror("wait epoll event");
            break;
        }

        for (int i = 0; i < ret; i++) {
            if ((evt_list[i].events & EPOLLERR) 
                || (evt_list[i].events & EPOLLHUP)
                || !(evt_list[i].events & EPOLLIN))
            {
                perror("epoll");
                ::close(evt_list[i].data.fd);
                return nullptr;
            }

            // half connection
            if (evt_list[i].events & EPOLLRDHUP) {
                printf("\nhalf close\n");
                close(evt_list[i].data.fd);
            }

            if (evt_list[i].data.fd == node->ss) {
                accept_conn(node->ss, epollfd, node);
            } else {
                recv_message(evt_list[i].data.fd);
            }
        }
    }
}

void *node_sender(void *args) {

}

Node::Node(const std::string &id, const std::string &ip, const uint16_t port): id(id),ip(ip),port(port) {
    ss = socket(AF_INET, SOCK_STREAM, 0);

    listener_enabled = false;
    sender_enabled = false;
}

#define MAX_LISTEN      128

void Node::listen() {
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(port);
    sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (::bind(ss, (struct sockaddr* ) &sock_addr, sizeof(sock_addr)) == -1)
        return;

    if (::listen(ss, MAX_LISTEN) == -1)
        return;

    if (pthread_create(&listener, nullptr, node_listener, this)) {
        std::cout << "create listener error" << std::endl;
    }

    pthread_setname_np(listener, "listener-0");

    listener_enabled = true;
}

void Node::send(const Node &remote, const std::string &buff) {
    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(remote.port);
    sock_addr.sin_addr.s_addr = inet_addr(remote.ip.c_str());

    int connect_status = ::connect(ss, (struct sockaddr *)&sock_addr, sizeof(sock_addr));
    if (connect_status < 0 && (errno != EISCONN)) {
        perror("client connect");
        return;
    }

    std::cout << "connected" << std::endl;

    sender_enabled = true;

    int data_sent = 0;

    if ((data_sent = ::send(ss, buff.c_str(), buff.length(), 0)) < 0)
        perror("client send");
    else
        std::cout << data_sent << " sent" << std::endl;
}

Node::~Node() {
    if (listener_enabled)
        pthread_join(listener, nullptr);
    if (sender_enabled)
        pthread_join(sender, nullptr);
    // TODO: shutdown socket connections
//    if (ss)
//        close(ss);
}