#!/usr/bin/env python3
"""
Validate device YAMLs against the JSON Schema and perform structural checks:
- Field overlap within each register (error)
- Duplicate absolute register offsets within a block (error)
- Gaps/holes between field bit ranges within a register (warning only)

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


def check_field_holes(fields: List[Dict[str, Any]], reg_width: int, path: str, warnings: List[str]):
    if not fields:
        return
    # Sort by lsb
    segs = sorted(((int(f["lsb"]), int(f["width"]), f.get("name", "")) for f in fields), key=lambda t: t[0])
    # Detect gaps between consecutive fields (only up to the max-used bit)
    max_used = 0
    gaps = []
    prev_end = None
    for lsb, width, _ in segs:
        end = lsb + width  # exclusive
        if prev_end is not None and lsb > prev_end:
            gaps.append((prev_end, lsb))
        prev_end = max(prev_end or 0, end)
        max_used = max(max_used, end)
    # Only warn on gaps detected; many registers intentionally have reserved holes
    if gaps:
        gap_str = ", ".join([f"[{a}:{b})" for a, b in gaps])
        warnings.append(f"Holes in {path}: uncovered bit ranges {gap_str} (info only)")


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


def validate_device(yaml_path: str, schema: Dict[str, Any]) -> Tuple[bool, List[str], List[str]]:
    with open(yaml_path, "r", encoding="utf-8") as f:
        data = yaml.safe_load(f)

    errors: List[str] = []
    warnings: List[str] = []
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
                reg_width = int(reg.get("width", 32) or 32)
                check_field_holes(fields, reg_width, f"{device}.{blk['name']}.{reg['name']}", warnings)
        # Duplicate absolute offsets within a block
        check_duplicate_offsets(blk, device, errors)

    ok = len(errors) == 0
    return ok, errors, warnings


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
    all_warnings: List[str] = []
    for name in sorted(os.listdir(devices_dir)):
        if not name.lower().endswith(('.yaml', '.yml')):
            continue
        path = os.path.join(devices_dir, name)
        ok, errs, warns = validate_device(path, schema)
        if not ok:
            all_ok = False
            all_errors.extend(errs)
        all_warnings.extend(warns)

    if not all_ok:
        print("Validation FAILED:")
        for e in all_errors:
            print(" -", e)
        return 2
    print("Validation OK")
    if all_warnings:
        print("Warnings:")
        for w in all_warnings:
            print(" -", w)
    return 0


if __name__ == "__main__":
    sys.exit(main())
