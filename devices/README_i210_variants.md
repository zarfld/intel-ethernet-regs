# I210 Register Map Variants

We maintain two I210 register maps sourced from Intel documentation. Both share the same base addresses for core blocks like PTP (0x0B600), MAC control, MDIC, and interrupts. Differences are subtle (naming/coverage) and tracked with `spec_reference`.

- `i210_cscl_v1_8.yaml` — Intel I210 CS/CL v1.8 (335761)
- `i210_v3_7.yaml` — Intel I210 Datasheet v3.7 (333016)

Generation outputs will be distinct headers:

- `gen/i210_cscl_regs.h`
- `gen/i210_v37_regs.h`

Selection options:

- Build-time: define one include in `precomp.h` or select via a small per-device switch in the generator.
- Runtime: both maps share key PTP offsets; timestamp code remains compatible.

Always cite the matching spec section when adding or modifying registers.
