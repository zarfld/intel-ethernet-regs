#include <assert.h>
#include <stdint.h>
#include "../gen/i210_regs.h"
#include "../gen/i226_regs.h"

/* Compile-time checks for golden offsets; adjust as needed per device. */

/* C23 static_assert; fallback to assert for older standards when compiled */
#ifndef static_assert
#define static_assert _Static_assert
#endif

int main(void) {
    /* I210 PTP SYSTIM base (0x0B600) */
    static_assert(I210_SYSTIML == 0x0B600, "I210_SYSTIML offset mismatch");
    static_assert(I210_SYSTIMH == 0x0B604, "I210_SYSTIMH offset mismatch");

    /* A couple of extended interrupts */
    static_assert(I210_EICR == 0x01580, "I210_EICR offset mismatch");
    static_assert(I210_EIMS == 0x01524, "I210_EIMS offset mismatch");

    /* I226 Transmit Control and its enable bit; expected values from the public
       Linux igc driver (IGC_TCTL 0x00400, IGC_TCTL_EN 0x00000002), not from the YAML */
    static_assert(I226_TCTL == 0x00400, "I226_TCTL offset mismatch");
    static_assert(I226_TCTL_EN_SHIFT == 1, "I226_TCTL_EN shift mismatch");
    static_assert(I226_TCTL_EN_MASK == 0x2ULL, "I226_TCTL_EN mask mismatch");

    /* I226 Scheduling Base Time High and its seconds field. The offset is from the public
       Linux igc driver (IGC_BASET_H 0x3318); shift 0 and mask 0xFFFFFFFF are the declared
       I225-derived encoding (I225 manual bits 31:0, lsb 0 consistent with igc's unshifted
       write), not from the YAML. They do not verify I226 hardware width */
    static_assert(I226_BASET_H == 0x03318, "I226_BASET_H offset mismatch");
    static_assert(I226_BASET_H_BASE_TIME_H_SHIFT == 0, "I226_BASET_H_BASE_TIME_H shift mismatch");
    static_assert(I226_BASET_H_BASE_TIME_H_MASK == 0xFFFFFFFFULL, "I226_BASET_H_BASE_TIME_H mask mismatch");

    /* Success without runtime */
    return 0;
}
