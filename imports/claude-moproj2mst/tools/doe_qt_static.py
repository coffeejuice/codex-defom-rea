"""Retain bounded DUMPBIN evidence for DOE's Qt text seams; no DLL execution."""
import hashlib
import json
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[1]
DUMPBIN = Path("C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/VC/Tools/MSVC/14.44.35207/bin/Hostx64/x64/dumpbin.exe")
TARGET = ROOT / "evidence/original-binaries/QtCore4_64.dll"
RANGES = {
    "qchar-space": (0x67069c80, 0x67069d0f),
    "qstring-double": (0x6706c340, 0x6706c3cf),
    "qlocale-double": (0x6704c340, 0x6704c49f),
    "qstring-regexp-split": (0x67079640, 0x670798df),
    "doe-sample-parser": (0x1409157c0, 0x140915bd0),
}


def main():
    receipt_path = ROOT / "analysis/wave2-doe-qt-static-receipt.json"
    assert not receipt_path.exists(), "preserve the existing capture"
    exported = subprocess.run([str(DUMPBIN), "/exports", str(TARGET)], capture_output=True, check=True)
    wanted = (b"toDouble@QString", b"split@QString", b"isSpace@QChar", b"readLine@QTextStream",
              b"toInt@QString", b"0004D4F0", b"00049340", b"0004C340", b"0004D5A0")
    selected = b"\n".join(line for line in exported.stdout.splitlines() if any(x in line for x in wanted))+b"\n"
    exports_path = ROOT / "analysis/wave2-doe-qt-selected-exports.txt"
    assert not exports_path.exists()
    exports_path.write_bytes(selected)
    captures = []
    for label, (begin, end) in RANGES.items():
        target = ROOT / "evidence/original-binaries/DEF_GUI_MO_64.exe" if label.startswith("doe-") else TARGET
        output = ROOT / ("analysis/wave2-doe-"+label+".asm")
        assert not output.exists()
        command = [str(DUMPBIN), "/disasm:bytes", f"/range:0x{begin:x},0x{end:x}", str(target)]
        result = subprocess.run(command, capture_output=True, check=True)
        output.write_bytes(result.stdout)
        captures.append({"command": command, "output": str(output.relative_to(ROOT)),
                         "sha256": hashlib.sha256(result.stdout).hexdigest(), "exit_code": result.returncode})
    receipt = {"schema": "wave2-doe-qt-static-v1", "target": str(TARGET.relative_to(ROOT)),
               "target_sha256": hashlib.sha256(TARGET.read_bytes()).hexdigest(),
               "tool": str(DUMPBIN), "tool_sha256": hashlib.sha256(DUMPBIN.read_bytes()).hexdigest(),
               "selected_exports": str(exports_path.relative_to(ROOT)),
               "selected_exports_sha256": hashlib.sha256(selected).hexdigest(), "captures": captures,
               "qualification": "Static DUMPBIN only. Bounded ranges may include alignment/adjacent code or stop before a complete callee; export address identity is not semantic equivalence. No original or candidate runtime, no DLL load, no REA session."}
    receipt_path.write_text(json.dumps(receipt, indent=2)+"\n", encoding="utf-8")
    print(json.dumps({"receipt": str(receipt_path), "qt_sha256": receipt["target_sha256"], "captures": len(captures)}))


if __name__ == "__main__":
    main()
