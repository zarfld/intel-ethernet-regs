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

## Provenance lines in generated headers (current generator)

Each header in `gen/` carries a line such as `/* Source YAML: i226.yaml @ 09d4c1c; Repo: 09d4c1c */`. `tools/reggen.py` takes both values from Git history: the last commit that touched the YAML file and the last commit that touched `devices/`. Both CI workflows regenerate every header and fail when the result differs from the committed `gen/` files (#2).

These are properties of the current generator, not a merge policy:

- A header regenerated before its YAML change is committed names the previous commit and is stale once the change is committed. Because `Repo:` follows every change under `devices/`, one YAML change makes the line of every header stale.
- Stamps stay valid when all headers are regenerated in a later commit that changes only `gen/`: commit the YAML change, then run `for f in devices/*.yaml; do python tools/reggen.py "$f" gen; done` and commit `gen/` alone.
- A merge method that rewrites the commits of a YAML change (squash, rebase) gives them new hashes on `main`, so stamps generated on the branch no longer match regeneration on `main`. This follows from `tools/reggen.py` and is not tested; the maintainer chooses the merge method per pull request.

Making generation independent of commit history and merge method is future work for a separate issue.
