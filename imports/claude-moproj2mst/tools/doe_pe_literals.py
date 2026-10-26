"""Bounded literal-byte capture from the immutable original GUI PE."""
import argparse
import hashlib
import json
from pathlib import Path
from doe_static_pe import PE, ROOT, BINARY


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--label", required=True)
    parser.add_argument("addresses", nargs="+")
    args = parser.parse_args()
    assert args.label.replace("-", "").isalnum()
    output = ROOT / ("analysis/wave2-doe-"+args.label+"-literals.json")
    assert not output.exists()
    pe = PE()
    values = []
    for text in args.addresses:
        address = int(text, 16)
        offset = pe.off(address)
        end = pe.data.find(b"\0", offset, offset+256)
        assert end >= offset
        raw = pe.data[offset:end]
        values.append({"va": hex(address), "file_offset": offset,
                       "ascii": raw.decode("ascii"), "raw_hex": raw.hex()})
    result = {"target": str(BINARY.relative_to(ROOT)), "sha256": hashlib.sha256(pe.data).hexdigest(),
              "qualification": "Raw NUL-terminated ASCII bytes, bounded to256; interpret only with original instruction/decompile evidence",
              "literals": values}
    output.write_text(json.dumps(result, indent=2)+"\n", encoding="utf-8")
    print(json.dumps({"path": str(output), "sha256": hashlib.sha256(output.read_bytes()).hexdigest(), "literals": values}))


if __name__ == "__main__":
    main()
