"""Replay captured original fscanf token/selector calls against KeywordReader."""
import argparse
import json
from pathlib import Path
import subprocess

from capture_oracle import digest

ROOT = Path(__file__).resolve().parents[1]


def require(condition, message):
    if not condition:
        raise ValueError(message)


def compare(capture, fixtures, driver):
    evidence = json.loads(capture.read_text(encoding="utf-8"))
    record = evidence["normalized_result"]
    require(not record["messages_truncated"] and not record["cleanup_error"], "incomplete capture")
    require(all(m["type"] == "send" for m in record["messages"]), "instrumentation error")
    events = [m["payload"] for m in record["messages"]]
    require(not any(e["operation"] == "probe-error" for e in events), "probe failure")
    scans = [e for e in events if e["operation"] == "stream-scan"]
    commands, expected = [], []
    eof_files = set()
    inputs = {}
    for scan in scans:
        path = fixtures / scan["file"]
        require(path.name == scan["file"], "unexpected fixture path")
        data = path.read_bytes()
        # These fixtures deliberately avoid text-mode ambiguity.
        require(b"\r" not in data and b"\x1a" not in data and len(data) < 4096,
                "fixture requires unimplemented physical/text offset mapping")
        inputs[path.name] = digest(path)
        before, after = scan["before"], scan["after"]
        for state in [before, after]:
            if state["buffer_hex"]:
                require(bytes.fromhex(state["buffer_hex"]) == data, "stream buffer/input mismatch")
        # A preceding fgets (not hooked by this probe) may already have
        # performed the failed refill before the next observed fscanf.
        prior_eof = before["buffer_hex"] == "" and before["count"] <= 0
        offset = len(data) if scan["file"] in eof_files or prior_eof else before["cursor"]
        # A failed EOF refill resets the CRT's buffer pointer. The resulting
        # zero is not a logical file offset; bind EOF to the complete fixture.
        eof_reset = after["cursor"] == 0 and after["count"] <= 0 and after["buffer_hex"] == ""
        cursor = len(data) if eof_reset else after["cursor"]
        if eof_reset:
            eof_files.add(scan["file"])
        kind = "token" if scan["format"] == "%39s" else "selector"
        require(scan["format"] in ("%39s", "%i"), "unsupported captured format")
        commands.append(f"{kind} 0 {offset} {data.hex()} -")
        wanted = {"status": int(scan["conversions"] == -1), "cursor": cursor, "lines": []}
        if kind == "token":
            wanted["token_hex"] = (scan["value"] or "").encode("ascii").hex()
        else:
            wanted["conversions"] = scan["conversions"]
            wanted["values"] = [scan["value"]]
        expected.append(wanted)
    process = subprocess.run([str(driver)], input="\n".join(commands) + "\n",
                             capture_output=True, text=True, check=True, timeout=15)
    actual = [json.loads(line) for line in process.stdout.splitlines()]
    require(len(actual) == len(expected), "replay count mismatch")
    differences = []
    for scan, wanted, got in zip(scans, expected, actual):
        for key, value in wanted.items():
            if got[key] != value:
                differences.append({"file": scan["file"], "sequence": scan["sequence"],
                                    "field": key, "expected": value, "actual": got[key]})
    return {"capture": capture.relative_to(ROOT).as_posix(), "capture_sha256": digest(capture),
            "evidence_id": evidence["evidence_id"], "inputs": inputs, "scans": len(scans),
            "token_calls": sum(e["format"] == "%39s" for e in scans),
            "selector_calls": sum(e["format"] == "%i" for e in scans),
            "differences": differences}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--driver", type=Path, default=ROOT / "build/Release/keyword_reader_replay.exe")
    parser.add_argument("--capture", type=Path, default=ROOT / "analysis/keyword-import-runtime-training.json")
    parser.add_argument("--fixtures", type=Path, default=ROOT / "tests/fixtures/keyword_import/training")
    parser.add_argument("--report", type=Path)
    args = parser.parse_args()
    run = compare(args.capture.resolve(), args.fixtures.resolve(), args.driver.resolve())
    report = {"status": "fail" if run["differences"] else "pass",
              "driver_sha256": digest(args.driver), "run": run,
              "limits": ["Bounded ASCII fixtures smaller than the observed single CRT buffer.",
                         "Logical EOF derives from complete fixture identity when CRT buffer resets.",
                         "No claim for overflow, locale changes, embedded NUL or file text translation."]}
    if args.report:
        args.report.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{report['status'].upper()}: {run['scans']} stream calls; {len(run['differences'])} differences")
    for difference in run["differences"]:
        print(json.dumps(difference))
    raise SystemExit(int(bool(run["differences"])))


if __name__ == "__main__":
    main()
