#ifndef TYPES_DEF_H
#define TYPES_DEF_H

#include <string>
#include <iostream>
#include <assert.h>

#define case_assert(expr, info) \
    do { \
    if (!(expr)) { \
        std::cout << static_cast<std::string>(info) << std::endl; \
        assert(expr); \
    } \
} while(0)

typedef enum _CaseStatus {
    Success,
    Failed
} CaseStatus;

typedef struct _CaseResult {
    CaseStatus status;
    std::string exception;
    bool is_success() {
        return status == CaseStatus::Success;
    }
    _CaseResult() {
        status = CaseStatus::Success;
        exception = std::string("");
    }
} CaseResult;

typedef CaseResult(*CaseFunc)();

#endif