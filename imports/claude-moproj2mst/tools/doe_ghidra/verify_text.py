"""Independently bind private Ghidra executable memory to original PE file bytes."""
import hashlib
import json
from pathlib import Path
import struct
import subprocess
import sys
import time

sys.dont_write_bytecode = True
sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from doe_static_pe import PE, ROOT


def main():
    pe = PE()
    sections, arguments = [], []
    for s in pe.sections:
        if s["executable"]:
            # Section VirtualSize is authoritative for initialized memory; exclude raw alignment tail.
            table = pe.optional+pe.u16(pe.optional-4)
            matches = [table+40*i for i in range(len(pe.sections)) if pe.u32(table+40*i+12) == s["rva"]]
            assert len(matches) == 1
            length = min(pe.u32(matches[0]+8), s["size"])
            start = pe.base+s["rva"]
            sections.append({"name": s["name"], "start": f"{start:x}", "length": length,
                             "file_offset": s["offset"],
                             "sha256": hashlib.sha256(pe.data[s["offset"]:s["offset"]+length]).hexdigest()})
            arguments.extend([f"{start:x}", str(length)])
    first = json.loads((ROOT / "analysis/wave2-doe-ghidra-receipt.json").read_text())
    command = first["command"]
    a, b = command.index("-postScript"), command.index("-log")
    output = ROOT / "analysis/wave2-doe-ghidra-text-memory.json"
    receipt_path = ROOT / "analysis/wave2-doe-ghidra-text-verification.json"
    assert not output.exists() and not receipt_path.exists()
    command = command[:a+1]+["DoeTextIdentity.java", str(output)]+arguments+command[b:]
    work = ROOT / "build/doe-ghidra-wave2"
    command[command.index("-log")+1] = str(work / "text-identity-ghidra.log")
    command[command.index("-scriptlog")+1] = str(work / "text-identity-script.log")
    start = time.time()
    with (work / "text-identity-launcher.log").open("w", encoding="utf-8") as log:
        run = subprocess.run(command, cwd=work, stdout=log, stderr=subprocess.STDOUT, shell=False,
                             creationflags=getattr(subprocess, "CREATE_NO_WINDOW", 0))
    result = json.loads(output.read_text())
    actual = {r["start"]: r for r in result["ranges"]}
    for s in sections:
        s["match"] = actual[s["start"]]["length"] == s["length"] and actual[s["start"]]["sha256"] == s["sha256"]
    receipt = {"command": command, "exit_code": run.returncode, "elapsed_seconds": round(time.time()-start,3),
               "target_sha256": hashlib.sha256(pe.data).hexdigest(), "sections": sections,
               "all_match": bool(sections) and all(s["match"] for s in sections),
               "qualification": "Whole initialized executable-section bytes compared; does not establish decompiler types/semantics. No target runtime/shared REA connection."}
    receipt_path.write_text(json.dumps(receipt, indent=2)+"\n")
    print(json.dumps({"receipt": str(receipt_path), "sections": sections, "all_match": receipt["all_match"]}))


if __name__ == "__main__":
    main()
