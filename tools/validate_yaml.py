#!/usr/bin/env python3
"""
Validate device YAMLs against the JSON Schema and perform basic structural checks:
- Field overlap within each register
- Duplicate absolute register offsets within a block

Usage (PowerShell):
  py -3 tools\validate_yaml.py schema\register_map.schema.json devices
"""
import sys, os, json
from typing import Dict, Any, List, Tuple

try:
    import yaml  # PyYAML
except Exception:
    print("This tool requires PyYAML (pip install pyyaml)")
    sys.exit(2)

try:
    import jsonschema
except Exception:
    print("This tool requires jsonschema (pip install jsonschema)")
    sys.exit(2)


def parse_num(v: Any) -> int:
    if isinstance(v, int):
        return v
    if isinstance(v, str):
        v = v.strip()
        if v.lower().startswith("0x"):
            return int(v, 16)
        return int(v)
    raise TypeError(f"Unsupported numeric type: {type(v)}")


def check_field_overlaps(fields: List[Dict[str, Any]], path: str, errors: List[str]):
    used_bits = 0
    for f in fields:
        lsb = int(f["lsb"])
        width = int(f["width"])
        if width >= 64:
            field_mask = (1 << 64) - 1
        else:
            field_mask = (1 << width) - 1
        mask = field_mask << lsb
        if used_bits & mask:
            errors.append(f"Overlap in {path}: field {f['name']} (lsb={lsb}, width={width}) overlaps previous fields")
        used_bits |= mask


def check_duplicate_offsets(block: Dict[str, Any], device: str, errors: List[str]):
    base = parse_num(block["base"]) if "base" in block else 0
    seen = {}
    for reg in block.get("registers", []):
        off = parse_num(reg["offset"]) if "offset" in reg else 0
        count = int(reg.get("count", 0) or 0)
        stride = reg.get("stride")
        if count and stride is not None:
            s = parse_num(stride)
            for i in range(count):
                abs_off = base + off + i * s
                key = abs_off
                name_i = f"{reg['name']}{i}"
                if key in seen:
                    errors.append(f"Duplicate absolute offset 0x{abs_off:X} in block {block['name']} ({device}): {name_i} vs {seen[key]}")
                else:
                    seen[key] = name_i
        else:
            abs_off = base + off
            key = abs_off
            if key in seen:
                errors.append(f"Duplicate absolute offset 0x{abs_off:X} in block {block['name']} ({device}): {reg['name']} vs {seen[key]}")
            else:
                seen[key] = reg['name']


def validate_device(yaml_path: str, schema: Dict[str, Any]) -> Tuple[bool, List[str]]:
    with open(yaml_path, "r", encoding="utf-8") as f:
        data = yaml.safe_load(f)

    errors: List[str] = []
    try:
        jsonschema.validate(instance=data, schema=schema)
    except jsonschema.ValidationError as e:
        errors.append(f"Schema validation error in {yaml_path}: {e.message}")

    device = data.get("device", os.path.basename(yaml_path))
    # Overlaps per register
    for blk in data.get("blocks", []):
        for reg in blk.get("registers", []):
            fields = reg.get("fields", []) or []
            if fields:
                check_field_overlaps(fields, f"{device}.{blk['name']}.{reg['name']}", errors)
        # Duplicate absolute offsets within a block
        check_duplicate_offsets(blk, device, errors)

    ok = len(errors) == 0
    return ok, errors


def main():
    if len(sys.argv) < 3:
        print("Usage: validate_yaml.py <schema.json> <devices_dir>")
        return 1
    schema_path = sys.argv[1]
    devices_dir = sys.argv[2]
    with open(schema_path, "r", encoding="utf-8") as f:
        schema = json.load(f)

    all_ok = True
    all_errors: List[str] = []
    for name in sorted(os.listdir(devices_dir)):
        if not name.lower().endswith(('.yaml', '.yml')):
            continue
        path = os.path.join(devices_dir, name)
        ok, errs = validate_device(path, schema)
        if not ok:
            all_ok = False
            all_errors.extend(errs)

    if not all_ok:
        print("Validation FAILED:")
        for e in all_errors:
            print(" -", e)
        return 2
    print("Validation OK")
    return 0


if __name__ == "__main__":
    sys.exit(main())
