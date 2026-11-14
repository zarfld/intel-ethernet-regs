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

    /* I226 TSN schedule-register baselines (#8). Expected values are the read-back recorded in the
       IntelAvbFilter #328 recovery record of 2026-11-05 (I226-LM 0x125B REV_04, Windows miniport, log SHA256
       c9f1d2f774d0efb3), not the YAML. These asserts check that the generated constants match that declared
       evidence; they do not prove I226 reset behaviour, a clean adapter, restoration or transmit behaviour */
    static_assert(I226_TQAVCTRL_BASELINE == 0x0U, "I226_TQAVCTRL baseline mismatch");
    static_assert(I226_BASET_L_BASELINE == 0x0U, "I226_BASET_L baseline mismatch");
    static_assert(I226_BASET_H_BASELINE == 0x0U, "I226_BASET_H baseline mismatch");
    static_assert(I226_QBVCYCLET_BASELINE == 0x3B9ACA00U, "I226_QBVCYCLET baseline mismatch");
    static_assert(I226_QBVCYCLET_S_BASELINE == 0x0U, "I226_QBVCYCLET_S baseline mismatch");
    static_assert(I226_STQT_BASELINE == 0x0U, "I226_STQT baseline mismatch");
    static_assert(I226_ENDQT_BASELINE == 0x0U, "I226_ENDQT baseline mismatch");
    static_assert(I226_TXQCTL_BASELINE == 0x0U, "I226_TXQCTL baseline mismatch");

    /* The second ENDQT value IntelAvbFilter DES-C-TAS-010 section 3.6 accepts for the windows baseline: 1 s, the
       I225 manual Rev 1.3 initial value and the value Linux igc programs (NSEC_PER_SEC). Not observed on this I226
       and not an established I226 reset value */
    static_assert(I226_ENDQT_BASELINE_ALT == 0x3B9ACA00U, "I226_ENDQT alternative baseline mismatch");

    /* DTXMXPKTSZ.MAX_TPKT_SIZE scale, bytes per unit, from Linux igc: IGC_DTXMXPKTSZ_TSN 0x19 = 1600 bytes and
       IGC_DTXMXPKTSZ_DEFAULT 0x98 = 9728 bytes. Decoding only; not measured on I226 */
    static_assert(I226_DTXMXPKTSZ_MAX_TPKT_SIZE_SCALE == 64U, "I226_DTXMXPKTSZ_MAX_TPKT_SIZE scale mismatch");

    /* Success without runtime */
    return 0;
}
