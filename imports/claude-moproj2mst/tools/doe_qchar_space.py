"""Emulate the original Qt QChar::isSpace table lookup; no DLL execution."""
import hashlib
import json
from pathlib import Path
from doe_static_pe import PE, ROOT


def main():
    pe = PE(ROOT / "evidence/original-binaries/QtCore4_64.dll")
    index = pe.off(pe.base + 0x1f4050)
    props = pe.off(pe.base + 0x1ff7b0)
    spaces = []
    for code in range(65536):
        slot = pe.u16(index + 2*(code >> 5)) + (code & 31)
        record = pe.u16(index + 2*slot)
        category = pe.data[props + 20*record] & 31
        if 9 <= code <= 13 or (1 << category) & 0x380:
            spaces.append(hex(code))
    result = {"target_sha256": hashlib.sha256(pe.data).hexdigest(), "function": "0x67069c80",
              "evidence": "analysis/wave2-doe-qchar-space.asm",
              "method": "Emulate DUMPBIN instructions: code units9..13 or category-bitmask0x380 via two-level Qt property index; no DLL execution",
              "qualification": "QChar predicate only; does not establish how QRegExp routes the \\s character class",
              "bmp_space_code_units": spaces}
    output = ROOT / "analysis/wave2-doe-qchar-space-table.json"
    assert not output.exists()
    output.write_text(json.dumps(result, indent=2)+"\n", encoding="utf-8")
    print(json.dumps(result))


if __name__ == "__main__":
    main()
