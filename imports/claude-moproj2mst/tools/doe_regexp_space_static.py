"""Retain and evaluate shipped Qt regex whitespace evidence without loading it."""
import hashlib
import json
import re
import subprocess
from pathlib import Path
from doe_static_pe import PE, ROOT

DUMPBIN = Path("C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/VC/Tools/MSVC/14.44.35207/bin/Hostx64/x64/dumpbin.exe")
TARGET = ROOT / "evidence/original-binaries/QtCore4_64.dll"
# Decode from function entry where practical, then retain only relevant instructions.
SELECTIONS = {
    "class-init": (0x6705ada0, 0x6705ae76, None),
    "token-reset": (0x67063b30, 0x670640b2, [(0x67063b30, 0x67063c15), (0x67063ed0, 0x67063f20)]),
    "escape-space": (0x670629d0, 0x67063b24, [(0x67062b0e, 0x67062b2f), (0x67062ceb, 0x67062d1a)]),
    "add-categories": (0x6705af10, 0x6705af91, None),
    "add-range": (0x67062420, 0x670626b2, None),
    "membership": (0x67059cc0, 0x67059d64, None),
    "category": (0x67069fb0, 0x67069fe9, None),
    "matcher-calls": (0x670644e0, 0x670654e1, [(0x670647e0, 0x670648d8)]),
    "search-calls": (0x67066840, 0x67066a30, None),
    "index-in": (0x670686a0, 0x67068764, None),
}


def sha(data):
    return hashlib.sha256(data).hexdigest()


def main():
    receipt_path = ROOT / "analysis/wave2-doe-regexp-space-contract.json"
    assert not receipt_path.exists(), "preserve existing evidence"
    pe = PE(TARGET)
    assert sha(pe.data) == "bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160"
    captures = []
    decoded = {}
    for label, (begin, end, selections) in SELECTIONS.items():
        command = [str(DUMPBIN), "/disasm:bytes", f"/range:0x{begin:x},0x{end:x}", str(TARGET)]
        raw = subprocess.run(command, capture_output=True, check=True).stdout.decode()
        decoded[label] = raw
        if selections:
            kept = [f"; DUMPBIN decoded from {begin:#x} through {end:#x}; selected intervals {selections!r}\n"]
            include = False
            for line in raw.splitlines(keepends=True):
                match = re.match(r"\s+([0-9A-F]{16}):", line)
                if match:
                    address = int(match[1], 16)
                    include = any(a <= address <= b for a, b in selections)
                if include:
                    kept.append(line)
            raw = "".join(kept)
        output = ROOT / f"analysis/wave2-doe-regexp-{label}.asm"
        assert not output.exists()
        output.write_text(raw, encoding="utf-8")
        captures.append({"output": str(output.relative_to(ROOT)), "sha256": sha(output.read_bytes()),
                         "decode_begin": hex(begin), "decode_end_inclusive": hex(end),
                         "selected_intervals": [[hex(a), hex(b)] for a, b in selections] if selections else None,
                         "decoded_bytes_sha256": sha(pe.data[pe.off(begin):pe.off(end)+1]), "command": command})
    # Confirm real decoded call instructions, not just byte-pattern references.
    calls = [("index-in", 0x67068740, 0x67066840),
             ("search-calls", 0x6706691a, 0x670644e0),
             ("search-calls", 0x67066937, 0x670644e0),
             ("matcher-calls", 0x67064808, 0x67059cc0),
             ("membership", 0x67059d03, 0x67069fb0)]
    for label, site, target in calls:
        assert re.search(rf"{site:016X}:.*call\s+{target:016X}", decoded[label])
    selector = pe.data[pe.off(0x67063adc) + ord('s') - 0x30]
    case = pe.base + pe.u32(pe.off(0x67063a98) + selector*4)
    assert selector == 13 and case == 0x67062ceb
    index = pe.off(0x671f4050)
    props = pe.off(0x671ff7b0)
    def classify(code):
        first = pe.u16(index + 2*(code >> 5))
        slot = first + (code & 31)
        record = pe.u16(index + 2*slot)
        byte = pe.data[props + 20*record]
        return {"code_unit": f"U+{code:04X}", "first_index": first, "slot": slot,
                "record": record, "category_byte": byte, "bt_category": byte & 31,
                "category_match": bool(0x380 & (1 << (byte & 31))),
                "range_match": 9 <= code <= 13,
                "matches_s": bool(0x380 & (1 << (byte & 31))) or 9 <= code <= 13}
    spaces = [hex(c) for c in range(65536) if classify(c)["matches_s"]]
    previous = json.loads((ROOT / "analysis/wave2-doe-qchar-space-table.json").read_text())
    assert spaces == previous["bmp_space_code_units"]
    source_files = ["source/apps/moproj2mst/include/m2m/doe_study.hpp",
                    "source/apps/moproj2mst/src/doe/doe_study.cpp",
                    "source/apps/moproj2mst/tests/mdt/test_doe_study.cpp"]
    receipt = {"schema": "wave2-doe-regexp-space-v1", "target": str(TARGET.relative_to(ROOT)),
               "target_sha256": sha(pe.data), "image_base": hex(pe.base),
               "method": "Offline DUMPBIN plus PE table evaluation; no DLL load, original execution, UI, build, or REA session",
               "tool": str(DUMPBIN), "tool_sha256": sha(DUMPBIN.read_bytes()),
               "reproducer": str(Path(__file__).relative_to(ROOT)),
               "reproducer_sha256": sha(Path(__file__).read_bytes()),
               "escape_dispatch": {"subtract": "0x30", "character": "s", "selector_table": "0x67063adc",
                                   "selector": selector, "rva_table": "0x67063a98", "case": hex(case),
                                   "preceding_short_escape_characters": pe.data[pe.off(0x671f38d8):pe.off(0x671f38d8)+7].hex(),
                                   "preceding_short_escape_values": pe.data[pe.off(0x671f38d0):pe.off(0x671f38d0)+7].hex(),
                                   "note": "s is absent from preceding short escapes afnrtv; lexer token reset maps backslash via 0x67064034/0x67063ff4 to 0x67063ef9, calling escape parser at 0x67063efc."},
               "class": {"category_mask": "0x380", "inclusive_ranges": [[9,13]], "negated": False,
                         "reset_mask": "0x67063b6a", "reset_negation": "0x67063bac",
                         "category_add": "0x6705af10", "range_add": "0x67062420",
                         "membership": "0x67059cc0", "category": "0x67069fb0",
                         "cache": "Constructor fills 64 buckets INT_MAX; category add zeroes all buckets; range add invalidates affected buckets with zero. No positive-match cache shortcut."},
               "call_edges": [{"site": hex(s), "target": hex(t)} for _,s,t in calls],
               "boundary_categories": [classify(c) for c in [8,9,13,14,32,0x84,0x85,0x86,0xa0,0x180e,0x200a,0x200b,0x200c,0x2028,0x2029,0x202a]],
               "bmp_space_code_units": spaces, "captures": captures,
               "prior_parser_evidence": ["analysis/wave2-doe-qt-static-receipt.json", "analysis/wave2-doe-doe-sample-parser.asm", "analysis/wave2-doe-qstring-regexp-split.asm", "analysis/wave2-doe-lifecycle.md"],
               "source_sha256": {s: sha((ROOT/s).read_bytes()) for s in source_files},
               "qualification": ["Proves shipped Qt regex lowercase s membership for BMP code units; not a claim of full regex engine equivalence.", "Earlier QChar-only U+0085 routing uncertainty is closed by this receipt; earlier immutable evidence is unchanged.", "QString::toDouble first tries default QLocale then C locale; candidate remains C-only. Whitespace recovery does not settle locale-sensitive numeric conversion.", "No runtime tests or builds performed by this worker; coordinator owns focused and full validation."]}
    receipt_path.write_text(json.dumps(receipt, indent=2)+"\n", encoding="utf-8")
    print(json.dumps({"receipt": str(receipt_path), "sha256": sha(receipt_path.read_bytes()),
                      "captures": len(captures), "nel": classify(0x85)}))


if __name__ == "__main__":
    main()
