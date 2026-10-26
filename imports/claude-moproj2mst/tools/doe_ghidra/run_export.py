"""Copy imported project; run standalone read-only decompiler. Never executes target."""
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / "evidence/gui_mo/gproj_p17"
WORK = ROOT / "build/doe-ghidra-wave2"
OUTPUT = ROOT / "analysis/wave2-doe-wrapper-decompile.json"
ADDRESSES = ["1400d75a0", "1400d7790", "1400d7820", "1400d7a80", "140915290", "140915360",
             "1409156e0", "140915cd0", "14064b890", "14064b950", "14064a830", "140645030",
             "140643100", "140e2fcd0"]


def hashes(root):
    return {p.relative_to(root).as_posix(): hashlib.sha256(p.read_bytes()).hexdigest()
            for p in sorted(root.rglob("*")) if p.is_file()}


def main():
    WORK.mkdir(parents=True, exist_ok=True)
    project = WORK / "project-copy"
    if project.exists() or OUTPUT.exists():
        raise FileExistsError("Use a fresh explicitly named project/output for another export")
    before = hashes(SOURCE)
    shutil.copytree(SOURCE, project, copy_function=shutil.copy2)
    if before != hashes(SOURCE) or before != hashes(project):
        raise RuntimeError("project snapshot changed or copy differs")
    runtime = WORK / "runtime"
    for name in ("user", "tmp", "settings", "cache"):
        (runtime / name).mkdir(parents=True)
    receipt = {"source": str(SOURCE), "project_copy": str(project), "source_file_hashes": before,
               "status": "copied-verified-not-opened", "target_runtime_executed": False}
    receipt_path = ROOT / "analysis/wave2-doe-ghidra-receipt.json"
    receipt_path.write_text(json.dumps(receipt, indent=2)+"\n")
    command = ["C:/Tools/jdk-21/bin/java.exe", "-Xmx2G", "-XX:ParallelGCThreads=2", "-XX:CICompilerCount=2",
               "-Djava.system.class.loader=ghidra.GhidraClassLoader", "-Dfile.encoding=UTF8",
               "-Duser.country=US", "-Duser.language=en", "-Dsun.java2d.d3d=false",
               f"-Duser.home={runtime / 'user'}", f"-Djava.io.tmpdir={runtime / 'tmp'}",
               f"-Dapplication.settingsdir={runtime / 'settings'}", f"-Dapplication.cachedir={runtime / 'cache'}",
               "-cp", "C:/Tools/ghidra_12.1.4_PUBLIC/Ghidra/Framework/Utility/lib/Utility.jar",
               "ghidra.Ghidra", "ghidra.app.util.headless.AnalyzeHeadless", str(project), "guimo",
               "-process", "DEF_GUI_MO_64.exe", "-readOnly", "-noanalysis", "-scriptPath", str(ROOT / "tools/doe_ghidra"),
               "-postScript", "DoeExport.java", str(OUTPUT), *ADDRESSES,
               "-log", str(WORK / "ghidra.log"), "-scriptlog", str(WORK / "script.log")]
    started = time.time()
    with (WORK / "launcher.log").open("w", encoding="utf-8") as log:
        run = subprocess.run(command, cwd=WORK, stdout=log, stderr=subprocess.STDOUT, shell=False,
                             creationflags=getattr(subprocess, "CREATE_NO_WINDOW", 0))
    receipt.update(command=command, exit_code=run.returncode, elapsed_seconds=round(time.time()-started, 3),
                   source_unchanged=before == hashes(SOURCE), status="exported" if OUTPUT.exists() else "export-missing")
    if OUTPUT.exists():
        receipt["output_sha256"] = hashlib.sha256(OUTPUT.read_bytes()).hexdigest()
    receipt_path.write_text(json.dumps(receipt, indent=2)+"\n")
    print(json.dumps({k: receipt.get(k) for k in ("status", "exit_code", "elapsed_seconds", "source_unchanged", "output_sha256")}))
    return run.returncode


if __name__ == "__main__":
    raise SystemExit(main())
