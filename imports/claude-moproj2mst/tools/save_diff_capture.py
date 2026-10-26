"""Run only frozen candidate full Save on disposable RS55; retain all outputs."""
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

ROOT = Path(__file__).resolve().parents[1]


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def manifest(root):
    return {p.relative_to(root).as_posix(): dict(bytes=p.stat().st_size, sha256=sha(p))
            for p in sorted(root.rglob("*")) if p.is_file()}


def main():
    receipt = ROOT / "analysis/wave1-connector-final-v2-build.json"
    build = json.loads(receipt.read_text())
    exe = ROOT / "build/candidate/bin/moproj2mst.exe"
    expected = next(a for a in build["artifacts"] if Path(a["path"]).resolve() == exe.resolve())
    if sha(exe) != expected["sha256"]:
        raise SystemExit("Candidate no longer matches the declared full-build receipt")
    source = ROOT / "evidence/gui_mo/oracle_b/RS_55_Multiple_Operations_Example1_before"
    before = manifest(source)
    run = ROOT / "build/wave2-save-diffs" / ("rs55-" + str(time.time_ns()))
    run.mkdir(parents=True, exist_ok=False)
    frozen = run / "frozen-bin"
    frozen.mkdir()
    frozen_exe = frozen / exe.name
    shutil.copy2(exe, frozen_exe)
    assert sha(frozen_exe) == expected["sha256"]
    project = run / "project"
    shutil.copytree(source, project)
    assert manifest(project) == before
    temp = run / "temp"
    temp.mkdir()
    env = dict(os.environ, TEMP=str(temp), TMP=str(temp), TMPDIR=str(temp))
    command = [str(frozen_exe), str(project / "Multiple_Operations_Example1.moproj"), "--write-save"]
    started = time.monotonic()
    with (run / "candidate.log").open("w", encoding="utf-8") as log:
        result = subprocess.run(command, cwd=run, env=env, stdout=log, stderr=subprocess.STDOUT)
    elapsed = round(time.monotonic() - started, 3)
    after_source = manifest(source)
    out = dict(run=str(run), command=command, exit_code=result.returncode,
               elapsed_seconds=elapsed, candidate=expected, build_receipt=str(receipt),
               frozen_executable=str(frozen_exe), fixture_unchanged=before == after_source,
               original_process_executed=False, project=str(project),
               scope="RS55 copied project full Save, default CLI answers; all outputs retained",
               fixture_before=before, candidate_after=manifest(project))
    path = ROOT / "analysis/wave2-save-diffs-rs55-capture.json"
    path.write_text(json.dumps(out, indent=2), encoding="utf-8")
    print(json.dumps({k: v for k, v in out.items() if k not in ("fixture_before", "candidate_after")}, indent=2))
    if result.returncode or before != after_source:
        raise SystemExit(1)


if __name__ == "__main__":
    main()
