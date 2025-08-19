# Intel Ethernet Regs (local usage)

## Generate headers

Prerequisites:

- Python 3.x (py launcher works on Windows)
- PyYAML: pip install pyyaml

Steps:

1. Install PyYAML
2. Generate headers

```powershell
py -3 -m pip install --user pyyaml
py -3 tools\reggen.py devices\i210.yaml gen
```

Output will be written to `gen/i210_regs.h`.

## Datasheet references

- Prefer Intel PDFs as the canonical source. For I210, use:
	- `external/intel_avb/spec/335761 - I210_CS_CL_Datasheet_v_1_8.pdf`
- For easier reading in the editor, you can keep a Markdown-converted copy alongside the PDF (no content changes), e.g. `external/intel_avb/spec/335761 - I210_CS_CL_Datasheet_v_1_8.md`.
