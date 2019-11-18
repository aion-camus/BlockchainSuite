
#include "handler.h"

/*
let mut req = ChannelBuffer::new();
        req.head.ver = Version::V0.value();
        req.head.ctrl = Control::NET.value();
        req.head.action = NetAction::HANDSHAKEREQ.value();

        req.body.clear();
        req.body.put_slice(&local_node.node_id);

        let mut net_id = [0; 4];
        BigEndian::write_u32(&mut net_id, local_node.net_id);

        req.body.put_slice(&net_id);
        req.body.put_slice(&local_node.ip_addr.ip);
        let mut port = [0; 4];
        BigEndian::write_u32(&mut port, local_node.ip_addr.port);
        req.body.put_slice(&port);
        let mut revision = short_version();
        revision.insert_str(0, REVISION_PREFIX);
        req.body.push(revision.len() as u8);
        req.body.put_slice(revision.as_bytes());
        req.body.push((VERSION.len() / 2) as u8);
        req.body.put_slice(VERSION.as_bytes());

        req.head.len = req.body.len() as u32;

        // handshake req
        trace!(target: "net", "Net handshake req sent...");
        P2pMgr::send(node.node_hash, req.clone());
*/

bool Handler::send(const Node &node, const ChannelBuffer &cb) {
    switch (cb.head.version) {
        case 0:

        break;
        case Version::V1: {
            ChannelBuffer req;
            req.head.version = Version::V0;
            req.head.ctrl = Control::NET;
            req.head.action = Action::HANDSHAKEREQ;
            req.body.node_id = "example";
            req.body.ip = 1234;         // local node ip
            req.body.port = 1234;       // local node port
            req.body.revision = "v1";   // rivision string
            req.body.version = 1;       // request version
            req.head.len = 12;          // body length
        }
        break;
        default:
        break;
    }
}