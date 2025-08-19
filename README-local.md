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
