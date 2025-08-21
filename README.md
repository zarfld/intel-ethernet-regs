# Intel Ethernet Registers (SSOT) [![Validate and prevent header drift](https://github.com/zarfld/intel-ethernet-regs/actions/workflows/validate-and-no-drift.yml/badge.svg)](https://github.com/zarfld/intel-ethernet-regs/actions/workflows/validate-and-no-drift.yml)

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

## How to consume (as submodule)

1. Add as a submodule under `third_party/`:

	```sh
	git submodule add https://github.com/zarfld/intel-ethernet-regs.git third_party/intel-ethernet-regs
	git submodule update --init --recursive
	```

2. Include generated headers from `gen/` in your build:
	- Treat `gen/<device>_regs.h` as read-only artifacts; regenerate on update.

3. Optional: vendor a specific commit to lock register definitions.

4. When updating, run the validator and generator:

	```powershell
	py -3 tools\validate_yaml.py schema\register_map.schema.json devices
	py -3 tools\reggen.py devices\i210.yaml gen
	```
