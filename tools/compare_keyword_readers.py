"""Compare C++ row readers against inputs/outputs observed inside the original."""
import argparse
import json
from pathlib import Path
import struct
import subprocess

from capture_oracle import digest

ROOT = Path(__file__).resolve().parents[1]


def require(ok, message):
    if not ok:
        raise ValueError(message)


def string_bytes(hex_value):
    return bytes.fromhex(hex_value).split(b"\0", 1)[0]


def compare(path, driver):
    evidence = json.loads(path.read_text(encoding="utf-8"))
    cap = evidence["normalized_result"]
    require(not cap["messages_truncated"] and cap["cleanup_error"] is None, "incomplete capture")
    require(all(m["type"] == "send" for m in cap["messages"]), "instrumentation error")
    events = [m["payload"] for m in cap["messages"]]
    require(events[0]["operation"] == "hooks-installed"
            and events[0]["pid"] == cap["pid"], "hook identity")
    require([e["sequence"] for e in events[1:]] == list(range(1, len(events))), "event gaps")
    require(not any(e["operation"] == "probe-error" for e in events), "probe failure")
    rows = [e for e in events if e["operation"] == "row"]
    commands = []
    for row in rows:
        data = b"".join(bytes.fromhex(line["hex"]) for line in row["lines"] if not line["eof"])
        previous = string_bytes(row.get("previous_name_hex", ""))
        commands.append(f"{row['kind']} {row['skip']} 0 {data.hex() or '-'} {previous.hex() or '-'}")
    process = subprocess.run([str(driver)], input="\n".join(commands) + "\n",
                             text=True, capture_output=True, check=True, timeout=15)
    # ostream can spell a negative double zero as "-0"; retain its sign.
    actual = [json.loads(line, parse_int=lambda s: -0.0 if s == "-0" else int(s))
              for line in process.stdout.splitlines()]
    require(len(actual) == len(rows), "row result count mismatch")
    differences = []
    kinds = {}
    for index, (row, got) in enumerate(zip(rows, actual)):
        kinds[row["kind"]] = kinds.get(row["kind"], 0) + 1
        expected_lines, cursor = [], 0
        for line in row["lines"]:
            data = bytes.fromhex(line["hex"]) if not line["eof"] else b""
            expected_lines.append({"offset": cursor, "cursor": cursor + len(data),
                                   "hex": data.hex(), "status": int(line["eof"])})
            cursor += len(data)
        wanted = {"status": row["status"], "cursor": cursor, "lines": expected_lines,
                  "consumed_hex": "".join(x["hex"] for x in expected_lines)}
        if row["kind"] == "name":
            wanted["name_hex"] = string_bytes(row["name_hex"]).hex()
        else:
            require(len(row["scans"]) <= 1, "unexpected nested numeric scan")
            wanted["conversions"] = row["scans"][0]["conversions"] if row["scans"] else -1
            wanted["values"] = row["values"]
            output_hex = [
                struct.pack("<d" if row["kind"].startswith("coord") and i else "<i",
                            float(v) if row["kind"].startswith("coord") and i else int(v)).hex()
                for i, v in enumerate(got["values"])
            ]
            got["output_hex"] = output_hex
            wanted["output_hex"] = row["output_hex"]
        for field, value in wanted.items():
            if got[field] != value:
                differences.append({"row": index, "file": row["file"], "kind": row["kind"],
                                    "sequence": row["sequence"], "field": field,
                                    "expected": value, "actual": got[field],
                                    "input_lines": row["lines"]})
    return {
        "capture": path.relative_to(ROOT).as_posix(), "sha256": digest(path),
        "evidence_id": evidence["evidence_id"], "pid": cap["pid"],
        "rows": len(rows), "helper_counts": kinds, "differences": differences,
        "verdict": "exact" if not differences else "different",
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--driver", type=Path, default=ROOT / "build/Release/keyword_reader_replay.exe")
    parser.add_argument("--report", type=Path)
    parser.add_argument("--capture", type=Path, action="append")
    args = parser.parse_args()
    driver = args.driver.resolve(strict=True)
    paths = args.capture or sorted((ROOT / "analysis").glob("keyword-rows-runtime-*.json"))
    require(bool(paths), "no captured rows")
    runs = [compare(path.resolve(), driver) for path in paths]
    report = {
        "status": "pass" if all(r["verdict"] == "exact" for r in runs) else "fail",
        "driver_sha256": digest(driver), "runs": runs,
        "comparison": "Native helper inputs from passive fgets capture, return codes, conversion counts, bounded line attempts and exact integer/double output bytes; logical name bytes through first NUL.",
        "limits": [
            "Not a full keyword importer, token-dispatch replay, physical file-offset comparison or whole-object comparison.",
            "Name storage padding and native addresses are not reproduced.",
            "Input is already text-translated; filesystem translation and complete locale/numeric domains remain separate.",
            "The token reader, nonzero blank-skip flags and overlong line splitting require separate original coverage."
        ],
    }
    if args.report:
        args.report.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{report['status'].upper()}: {sum(r['rows'] for r in runs)} original row-helper calls; "
          f"{sum(len(r['differences']) for r in runs)} differences")
    for run in runs:
        for difference in run["differences"]:
            print(json.dumps(difference))
    raise SystemExit(0 if report["status"] == "pass" else 1)


if __name__ == "__main__":
    main()
