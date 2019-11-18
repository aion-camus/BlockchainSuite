#include "types_def.h"

extern void avm_run();
extern void generate_keypair();

#define AVM_CASE_ENABLE     true
#define GAS_MINIMUM_ENABLE  false

void run_test() {
#if AVM_CASE_ENABLE
    avm_run();
#endif
#if GAS_MINIMUM_ENABLE
    gas_run();
#endif
}