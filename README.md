# Intel Ethernet Registers (SSOT)

Centralized, machine-readable register definitions for Intel Ethernet controllers.

Goals:
- Single source of truth for register maps across drivers/tools
- Generate C headers for kernel/user code
- Traceability: each register references the Intel datasheet section

Layout:
- schema/register_map.schema.json – JSON schema for YAML devices
- devices/<device>.yaml – per-device register maps
- tools/reggen.py – simple code generator to emit headers
- gen/headers – generated C headers

Status: seed with I210 PTP/SYSTIM registers; extend incrementally.
