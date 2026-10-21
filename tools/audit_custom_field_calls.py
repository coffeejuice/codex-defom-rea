"""Bind saved native dispatcher decoding to PE bytes for CUSVAR/DYNARY."""
import hashlib
import json
import re
import struct
from pathlib import Path


def binding(path):
    return {"path": str(path).replace("\\", "/"),
            "sha256": hashlib.sha256(Path(path).read_bytes()).hexdigest()}


def main():
    target = Path("source/DEF_PRE_64.exe")
    decode = Path("analysis/mesh-field-dispatch-dumpbin.txt")
    data = target.read_bytes()
    pe = struct.unpack_from("<I", data, 0x3c)[0]
    assert data[pe:pe + 4] == b"PE\0\0"
    sections = struct.unpack_from("<H", data, pe + 6)[0]
    optional_size = struct.unpack_from("<H", data, pe + 20)[0]
    optional = pe + 24
    assert struct.unpack_from("<H", data, optional)[0] == 0x20b
    image_base = struct.unpack_from("<Q", data, optional + 24)[0]
    section_table = optional + optional_size
    ranges = []
    for i in range(sections):
        section = section_table + i * 40
        _, rva, size, raw = struct.unpack_from("<IIII", data, section + 8)
        ranges.append((image_base + rva, size, raw))

    def native(address, length):
        for start, size, raw in ranges:
            if start <= address and address + length <= start + size:
                return data[raw + address - start:raw + address - start + length]
        raise ValueError(f"Address {address:#x}+{length} outside file-backed sections")

    instructions = []
    for line in decode.read_text(encoding="utf-8-sig").splitlines():
        match = re.match(r"^\s+([0-9A-F]{16}): ((?:[0-9A-F]{2} )+)\s*(\S+)(?:\s+(.*))?$", line)
        if match:
            instructions.append({"address": int(match[1], 16),
                "bytes": bytes.fromhex(match[2]), "mnemonic": match[3],
                "operands": match[4] or ""})
        elif instructions and re.fullmatch(r"\s+(?:[0-9A-F]{2}\s*)+", line):
            instructions[-1]["bytes"] += bytes.fromhex(line)
    assert len(instructions) == 8030
    for item in instructions:
        assert native(item["address"], len(item["bytes"])) == item["bytes"], hex(item["address"])

    def serialize(item):
        return {**item, "address": hex(item["address"]), "bytes": item["bytes"].hex()}

    cases = []
    for keyword, key, start, end in [("CUSVAR", 198, 0x140397d1e, 0x140397e75),
                                    ("DYNARY", 266, 0x1403970e9, 0x14039776c)]:
        index = key - 144
        jump = 0x140397eac + 4 * index
        entry_bytes = native(jump, 4)
        assert image_base + struct.unpack("<I", entry_bytes)[0] == start
        selected = [i for i in instructions if start <= i["address"] < end]
        assert selected[0]["address"] == start
        for left, right in zip(selected, selected[1:]):
            assert left["address"] + len(left["bytes"]) == right["address"]
        assert selected[-1]["address"] + len(selected[-1]["bytes"]) == end
        for item in selected:
            if item["mnemonic"] == "call" and item["bytes"][0] == 0xe8:
                resolved = item["address"] + 5 + struct.unpack("<i", item["bytes"][1:])[0]
                assert resolved == int(item["operands"], 16)
        cases.append({"keyword": keyword, "id": key, "case_index": index,
            "entry": hex(start), "end_exclusive": hex(end),
            "jump_entry": hex(jump), "jump_entry_bytes": entry_bytes.hex(),
            "instructions": [serialize(i) for i in selected],
            "calls": [serialize(i) for i in selected if i["mnemonic"] == "call"]})
    result = {"status": "both_case_blocks_and_jump_entries_verified_against_PE",
        "source_bindings": [binding(target), binding(decode), binding(__file__),
            binding("analysis/geometry-import-0x14038b8a0.json")],
        "total_instructions_verified": len(instructions), "cases": cases,
        "scope": "Static native-byte and direct-call verification, not runtime observation or semantic equivalence."}
    Path("analysis/custom-field-callsite-audit.json").write_text(json.dumps(result, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"status": result["status"], "instructions": len(instructions),
        "cases": [{"keyword": x["keyword"], "instructions": len(x["instructions"]),
                   "calls": len(x["calls"])} for x in cases]}))


if __name__ == "__main__":
    main()
