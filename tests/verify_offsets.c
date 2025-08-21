#include <assert.h>
#include <stdint.h>
#include "../gen/i210_regs.h"

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

    /* Success without runtime */
    return 0;
}
