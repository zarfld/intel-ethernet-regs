## Copilot instructions for intel-ethernet-regs

This repo is a Single Source of Truth (SSOT) for Intel Ethernet controller register maps. Device YAML → Python generator → C headers in `gen/`. Edit YAML, not generated headers.

## Working principles
-- ensure you understand the architecture and patterns before coding
-- Hardware-first policy: No Fake, No Stubs, no Simulations in production paths
-- Optional DEV_SIMULATION feature flag may be used for developer-only fallback paths (must be guarded by AVB_DEV_SIMULATION)
-- no implementation based assumtions, use specification or analysis results (ask if required)
-- no false advertising, prove and ensure correctness
-- always use real hardware access patterns
-- use Intel hardware specifications for register access (SSOT headers + datasheets)
-- code needs to compile before commit, no broken code
-- Always reference the exact Intel datasheet section or spec version when implementing register access.
-- Validate all hardware reads/writes with range checks or masks from the specification (SSOT masks where available).
-- Every function must have a Doxygen comment explaining purpose, parameters, return values, and hardware context.
-- no duplicate or redundant implementations to avoid inconsistencies and confusion; use centralized, reusable functions instead
-- no ad-hoc file copies (e.g., *_fixed, *_new, *_correct); refactor in place step-by-step to avoid breakage
-- Clean submit rules:
   - each commit compiles and passes checks
   - small, single-purpose, reviewable diffs (no WIP noise)
   - no dead or commented-out code; remove unused files
   - run formatter and static analysis before commit
   - update docs/tests and reference the spec/issue in the message
   - use feature flags or compatibility layers when incremental changes risk breakage
-- Avoid unnecessary duplication. Duplication is acceptable when it improves clarity, isolates modules, or is required for performance.
-- Avoid code that is difficult to understand. Prefer clear naming and structure over excessive comments or unnecessary helper variables.
-- Avoid unnecessary complexity. Keep required abstractions for maintainability, testability, or hardware safety
-- Design modules so that changes in one module do not require changes in unrelated modules. Avoid dependencies that cause single changes to break multiple areas.
-- Design components for reuse where practical, but prioritize correctness and domain fit over forced generalization.
-- Prefer incremental modification of existing code over reimplementation; adapt existing functions instead of creating redundant new ones

### Repository map
- `devices/*.yaml` — per‑device register maps (e.g., `i210.yaml`).
- `schema/register_map.schema.json` — JSON Schema describing YAML structure (informational; not enforced by the generator).
- `tools/reggen.py` — header generator.
- `gen/*_regs.h` — generated headers (e.g., `gen/i210_regs.h`).
- `spec/` — vendor PDFs referenced by `spec_section` fields.
- `README.md`, `README-local.md` — overview and usage.

### Generate headers (Windows PowerShell)
- Install deps: `py -3 -m pip install --user pyyaml`
- Generate: `py -3 tools\reggen.py devices\i210.yaml gen`
- Output: `gen/<device>_regs.h` (`device:` uppercased for macros, lowercased for filename).

### YAML expectations (generator-relevant)
- Top level: `device`, `version`, `spec_reference`, optional `pcie_caps`, `blocks[]`.
- Block: `name`, `base` (hex), `registers[]`.
- Register: `name`, `offset` (hex), optional `width` (32/64), `description`, `spec_section`, `access`, `reset`, `count`, `stride`, `fields[]`.
- Field: `name`, `lsb`, `width` (≤64), optional `description`, `spec_section`, `reserved`, `enums` (value→name map).
- Notes: numeric strings may be hex (`0x…`) or ints; `access` is copied to comments only. Schema enumerates `ro|rw|wo|w1c|w0c`, but some YAMLs use `rc/w1c` — the generator passes it through as-is in comments.

### Generated header patterns (from `tools/reggen.py`)
- Guard: `INTEL_<DEVICE>_REGS_H`, plus `#pragma once` and `extern "C"` for C++.
- Registers: `#define <DEVICE>_<REG> 0x<abs>` where `<abs> = base + offset` (e.g., `I210_SYSTIML 0x0B600`).
- Fields: `<DEVICE>_<REG>_<FIELD>_SHIFT` and `_MASK` (64‑bit). If enums are provided, defines `<...>_<ENUMNAME>` constants.
- Inline helpers per register with fields: `<DEVICE>_<REG>_GET(v, mask, shift)` and `_SET(v, mask, shift, val)` using `unsigned long long`.
- Arrays: if `count`/`stride` set, emits `<REG>0`, `<REG>1`, …
- PCIe caps (if present): `<DEVICE>_<CAP>_CAP_ID` and relative capability register offsets/macros; fields get SHIFT/MASK similarly.

### Example (from `devices/i210.yaml`)
- Block `PTP` at `base: 0x0B600`, register `TSYNCTXCTL` with fields `EN` (lsb=4, width=1) and `TYPE` (lsb=5, width=2, enums `ALL|SYNC|DELAY_REQ`).
- Header emits `I210_TSYNCTXCTL`, `I210_TSYNCTXCTL_EN_SHIFT/MASK`, `I210_TSYNCTXCTL_TYPE_SYNC`, and inline GET/SET for `TSYNCTXCTL`.

### Project conventions and tips
- Edit YAML in `devices/` and re‑run the generator; don’t hand‑edit `gen/*_regs.h`.
- Keep `spec_section` strings accurate; they serve as traceability to PDFs in `spec/`.
- Watch for width/offset math and `count`/`stride` combinations — no overlap checks are enforced.
- Macro prefix derives from `device:` uppercased (e.g., `I210`, `I211`).

Questions or mismatches? Point to the relevant lines in `tools/reggen.py`, the YAML, or a generated header and we’ll refine this doc.
