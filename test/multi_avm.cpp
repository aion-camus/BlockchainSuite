#include "types_def.h"

#include <iostream>

static CaseResult v1_can_not_get_tx_hash();
static CaseResult v2_should_retransform_avm_code();

#define AVM_CASE_NUM    2

CaseFunc avm_cases[AVM_CASE_NUM] = {
        v1_can_not_get_tx_hash,
        v2_should_retransform_avm_code
};

// multiple avm version
static CaseResult v1_can_not_get_tx_hash() {
    CaseResult res;

    return res;
}

static CaseResult v2_should_retransform_avm_code() {
    CaseResult res;

//    res.status = CaseStatus::Failed;

    return res;
}

void avm_run() {
    for (int i = 0; i < AVM_CASE_NUM; i++) {
        case_assert(avm_cases[i]().is_success(), "avm failed");
    }
}