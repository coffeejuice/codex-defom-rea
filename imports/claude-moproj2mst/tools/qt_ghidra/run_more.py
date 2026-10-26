"""Additional bounded exports from the already verified private project; no server."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import time

ROOT = Path(__file__).resolve().parents[2]


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--label", required=True)
    parser.add_argument("--script", choices=("QtExport.java",), default="QtExport.java")
    parser.add_argument("addresses", nargs="+")
    args = parser.parse_args()
    assert re.fullmatch("[a-z0-9-]+", args.label)
    assert all(re.fullmatch("67[0-9a-f]{6}", x) for x in args.addresses)
    first = json.loads((ROOT / "analysis/wave3-qt-ghidra-receipt.json").read_text())
    assert first["source_unchanged"]
    output = ROOT / ("analysis/wave3-qt-"+args.label+"-decompile.json")
    receipt_path = ROOT / ("analysis/wave3-qt-"+args.label+"-receipt.json")
    assert not output.exists() and not receipt_path.exists()
    command = first["command"]
    command[command.index("-process")+1] = "QtCore4_64.dll"
    a, b = command.index("-postScript"), command.index("-log")
    command = command[:a+2]+[str(output)]+args.addresses+command[b:]
    command[a+1] = args.script
    work = ROOT / "build/qt-ghidra-wave3"
    command[command.index("-log")+1] = str(work / (args.label+"-ghidra.log"))
    command[command.index("-scriptlog")+1] = str(work / (args.label+"-script.log"))
    start = time.time()
    with (work / (args.label+"-launcher.log")).open("w", encoding="utf-8") as log:
        result = subprocess.run(command, cwd=work, stdout=log, stderr=subprocess.STDOUT, shell=False,
                                creationflags=getattr(subprocess, "CREATE_NO_WINDOW", 0))
    receipt = {"command": command, "exit_code": result.returncode, "elapsed_seconds": round(time.time()-start,3),
               "target_runtime_executed": False, "shared_rea_session": False,
               "private_project_provenance": "analysis/wave3-qt-ghidra-receipt.json",
               "output": str(output), "output_sha256": hashlib.sha256(output.read_bytes()).hexdigest() if output.exists() else None}
    receipt_path.write_text(json.dumps(receipt, indent=2)+"\n")
    print(json.dumps({k: receipt[k] for k in ("exit_code", "elapsed_seconds", "output", "output_sha256")}))
    return result.returncode


if __name__ == "__main__":
    raise SystemExit(main())
