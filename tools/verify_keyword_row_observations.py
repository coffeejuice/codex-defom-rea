"""Verify original keyword-row evidence independently of the replacement reader."""
from collections import Counter
from copy import deepcopy
import json
from pathlib import Path
import re
import struct

from capture_oracle import digest
from verify_boundary_sort_observations import (
    EXE_SHA, ROOT, binding, check, read, relative, verify_files,
)

SPECS = [
    ("training", 23484, 46, 29, [0, 2, 0, 2]),
    ("discovery", 8240, 18, 14, [0]),
    ("held-out", 31700, 41, 23, [0, 2, 0, 2]),
]
WIDTHS = {"int1": 1, "int2": 2, "int5": 5, "int9": 9, "coord2": 3, "coord3": 4}
KEYWORDS = {b"OBJNAM": 144, b"OBJTYP": 145, b"RZ": 159, b"ELMCON": 173}


def unhex(value):
    check(isinstance(value, str) and len(value) % 2 == 0
          and re.fullmatch("[0-9a-f]*", value) is not None, "invalid hex")
    return bytes.fromhex(value)


def verify_row(row, data, cursor):
    kind = row["kind"]
    check(kind in {*WIDTHS, "name"} and row["skip"] == 0, "row kind/skip changed")
    lines = row["lines"]
    check(len(lines) == (2 if kind == "name" else 1), "line attempt count changed")
    for line in lines:
        check(line["limit"] == 1024 and type(line["eof"]) is bool, "fgets limit/type changed")
        if line["eof"]:
            check(cursor == len(data) and line["hex"] is None, "false EOF")
        else:
            raw = unhex(line["hex"])
            end = data.find(b"\n", cursor)
            end = len(data) if end < 0 else end + 1
            end = min(end, cursor + 1023)
            check(raw and raw == data[cursor:end] and b"\0" not in raw,
                  "fgets bytes differ from fixture at cursor")
            cursor = end
    if kind == "name":
        check(row["scans"] == [], "name unexpectedly scanned")
        previous, output = unhex(row["previous_name_hex"]), unhex(row["name_hex"])
        check(len(previous) == len(output) == 81 and b"\0" in previous and b"\0" in output,
              "name buffer capture incomplete")
        check(row["status"] == int(lines[-1]["eof"]), "name EOF status mismatch")
        if row["status"]:
            check(output == previous, "failed name read changed bytes")
    else:
        count = WIDTHS[kind]
        check(len(row["values"]) == len(row["output_hex"]) == count, "output count mismatch")
        scans = row["scans"]
        check(len(scans) == int(not lines[-1]["eof"]), "scan/EOF mismatch")
        conversions = -1
        if scans:
            scan = scans[0]
            fmt = "%i" + (" %Flf" * (count - 1) if kind.startswith("coord")
                          else " %i" * (count - 1))
            check(scan["format"] == fmt, "scanf format mismatch")
            check(unhex(scan["input_hex"]) == unhex(lines[-1]["hex"]).replace(b",", b" "),
                  "scanner input does not match comma-normalized fgets bytes")
            conversions = scan["conversions"]
            check(type(conversions) is int and -1 <= conversions <= count,
                  "conversion count outside output width")
        check(row["status"] == int(conversions == -1), "numeric status/conversions mismatch")
        for index, (value, output) in enumerate(zip(row["values"], row["output_hex"])):
            raw = unhex(output)
            floating = kind.startswith("coord") and index > 0
            check(len(raw) == (8 if floating else 4), "output byte width mismatch")
            decoded = struct.unpack("<d" if floating else "<i", raw)[0]
            # Frida JSON serializes negative zero as 0; raw bytes retain the sign.
            check(decoded == value, "value/output bytes disagree")
            if index >= max(0, conversions):
                check(raw == bytes(len(raw)), "unconverted output not initialized to zero")
    return cursor


def verify_events(evidence, run, spec, inputs):
    split, pid, count, rowcount, statuses = spec
    cap, params = evidence["normalized_result"], evidence["parameters"]
    check(cap["pid"] == params["pid"] == run["pid"] == pid, "PID mismatch")
    check(cap["target"] == f"DEF_PRE_64.exe (pid {pid})", "target mismatch")
    check(cap["session_id"] == params["session_id"], "session mismatch")
    check(cap["device_id"] == params["device_id"] == "local"
          and cap["mode"] == params["mode"] == "attach"
          and cap["state"] == "detached" and cap["source_kind"] == "file",
          "capture lifecycle mismatch")
    check(cap["messages_truncated"] is False and cap["cleanup_error"] is None,
          "capture incomplete")
    scripts, messages = cap["scripts"], cap["messages"]
    check(len(scripts) == 1 and all(m["type"] == "send"
          and m["script_id"] == scripts[0]["script_id"] for m in messages),
          "script mismatch or error")
    first = messages[0]["payload"]
    check(first["operation"] == "hooks-installed" and first["pid"] == pid
          and int(first["base"], 16) > 0, "hook identity mismatch")
    events = [m["payload"] for m in messages[1:]]
    check(len(events) == count and [e["sequence"] for e in events] == list(range(1, count + 1)),
          "event sequence incomplete")
    active, imports, rows, cursor, data = None, [], [], 0, b""
    for event in events:
        op, filename = event["operation"], event["file"]
        if op == "import-before":
            check(active is None and len(imports) < len(run["ordered_imports"])
                  and filename == run["ordered_imports"][len(imports)], "import order mismatch")
            active, cursor, data = filename, 0, inputs[filename].replace(b"\r\n", b"\n")
            check(b"\0" not in data and b"\x1a" not in data, "uncovered text translation")
        else:
            check(active is not None and filename == active, "event outside active import")
            if op == "keyword":
                token = unhex(event["token_hex"])
                check(token in KEYWORDS and event["id"] == KEYWORDS[token], "keyword ID mismatch")
                # These fixtures use object selector 1. Its native reader is not hooked.
                prefix = re.match(rb"(?:\s+|\*[^\n]*(?:\n|$))*", data[cursor:])[0]
                cursor += len(prefix)
                match = re.match(re.escape(token) + rb"[ \t]+1(?=[ \t\n]|$)", data[cursor:])
                check(match is not None, "keyword/object selector fixture order mismatch")
                cursor += len(match[0])
            elif op == "row":
                cursor = verify_row(event, data, cursor)
                rows.append(event)
            elif op == "import-after":
                check(event["status"] == statuses[len(imports)], "import status changed")
                check(not data[cursor:].strip(), "unconsumed fixture content")
                imports.append({"file": active, "status": event["status"],
                                "text_bytes_accounted": cursor})
                active = None
            else:
                raise ValueError("unexpected event or probe error")
    check(active is None and len(imports) == len(statuses) and len(rows) == rowcount,
          "missing import or row")
    return imports, rows


def verify_probe(cap, run, split):
    template = ROOT / "tools/observe_keyword_rows.js"
    script = ROOT / f"analysis/oracle/keyword-rows-{split}.js"
    gate = ROOT / f"analysis/oracle/keyword-rows-{split}.ready"
    check(Path(cap["source_path"]) == script and digest(script) == cap["source_sha256"],
          "probe path/hash mismatch")
    check(Path(run["ready_gate"]) == gate, "gate mismatch")
    source = template.read_text(encoding="utf-8")
    check(source.count("__READY_GATE__") == 2, "template placeholder count")
    check(script.read_text(encoding="utf-8") == source.replace("__READY_GATE__", gate.as_posix()),
          "probe differs from passive template")
    return binding(script)


def verify_run(spec, evidence=None, run=None, cases=None):
    split = spec[0]
    capture = ROOT / f"analysis/keyword-rows-runtime-{split}.json"
    directory = ROOT / f"analysis/oracle/keyword-rows-{split}"
    manifest = directory / "run.json"
    evidence, run = evidence or read(capture), run or read(manifest)
    check(run["status"] == "completed" and run["exit_code"] == 0, "original did not complete")
    check(Path(run["cwd"]) == directory, "run directory mismatch")
    check(Path(run["executable"]) == ROOT / "source/DEF_PRE_64.exe"
          and run["executable_sha256"] == EXE_SHA
          and digest(Path(run["executable"])) == EXE_SHA, "original identity mismatch")
    verify_files(run, directory)
    check((directory / "stdout.bin").stat().st_size < run["max_output_bytes"],
          "transcript exceeded bound")
    probe = verify_probe(evidence["normalized_result"], run, split)
    casepath = ROOT / ("analysis/keyword-row-discovery-cases.json" if split == "discovery"
                       else "analysis/keyword-row-evaluation-cases.json")
    cases = cases or read(casepath)
    expected = {c["path"]: c["sha256"] for c in cases["cases"]
                if f"/{split.replace('-', '_')}/" in c["path"]}
    check(len(expected) == len(run["inputs"]), "fixture case count mismatch")
    check([i["name"] for i in run["inputs"]] == run["ordered_imports"],
          "input/import order mismatch")
    inputs, fixtures = {}, []
    for item in run["inputs"]:
        source, copy = Path(item["source"]), Path(item["copy"])
        check(relative(source) in expected and copy == directory / item["name"]
              and source.name == item["name"], "fixture path mismatch")
        sha = expected[relative(source)]
        check(digest(source) == digest(copy) == item["sha256_before"] == item["sha256_after"]
              == item["source_sha256_after"] == sha, "fixture hashes mismatch")
        inputs[item["name"]] = copy.read_bytes()
        fixtures.append({"source": relative(source), "copy": relative(copy), "sha256": sha})
    imports, rows = verify_events(evidence, run, spec, inputs)
    normalized = []
    for row in rows:
        clean = deepcopy(row)
        if row["kind"] == "name":
            # Uninitialized padding can contain unrelated memory; raw evidence remains bound.
            for field in ("previous_name_hex", "name_hex"):
                clean[field] = unhex(row[field]).split(b"\0", 1)[0].hex()
        normalized.append(clean)
    return {"split": split, "capture": binding(capture), "manifest": binding(manifest),
            "fixture_manifest": binding(casepath), "probe": probe,
            "evidence_id": evidence["evidence_id"], "pid": spec[1],
            "events": spec[2], "rows": len(rows), "imports": imports, "fixtures": fixtures,
            "helper_counts": dict(Counter(r["kind"] for r in rows)), "observations": normalized}


def tamper_checks():
    spec = SPECS[0]
    evidence = read(ROOT / "analysis/keyword-rows-runtime-training.json")
    run = read(ROOT / "analysis/oracle/keyword-rows-training/run.json")
    inputs = {i["name"]: Path(i["copy"]).read_bytes() for i in run["inputs"]}
    tests = []

    def attempt(label, mutate):
        altered = deepcopy(evidence)
        mutate(altered)
        try:
            verify_events(altered, run, spec, inputs)
        except (ValueError, KeyError, IndexError):
            tests.append({"mutation": label, "rejected": True})
        else:
            raise ValueError(f"tamper not rejected: {label}")

    def payload(e, index):
        return e["normalized_result"]["messages"][index]["payload"]

    attempt("wrong PID", lambda e: e["normalized_result"].update(pid=1))
    attempt("truncated capture", lambda e: e["normalized_result"].update(messages_truncated=True))
    attempt("missing event", lambda e: e["normalized_result"]["messages"].pop(8))
    attempt("wrong event sequence", lambda e: payload(e, 8).update(sequence=99))
    attempt("wrong file", lambda e: payload(e, 8).update(file="different.KEY"))
    attempt("wrong keyword ID", lambda e: payload(e, 2).update(id=0))
    attempt("altered raw line", lambda e: payload(e, 8)["lines"][0].update(hex="300a"))
    attempt("false EOF", lambda e: payload(e, 8)["lines"][0].update(eof=True, hex=None))
    attempt("wrong scanf format", lambda e: payload(e, 8)["scans"][0].update(format="%d"))
    attempt("wrong scan bytes", lambda e: payload(e, 8)["scans"][0].update(input_hex="300a"))
    attempt("impossible conversions", lambda e: payload(e, 8)["scans"][0].update(conversions=7))
    attempt("output bytes disagree", lambda e: payload(e, 8)["output_hex"].__setitem__(1, "0000000000000000"))
    attempt("numeric status mismatch", lambda e: payload(e, 8).update(status=1))
    attempt("short name buffer", lambda e: payload(e, 3).update(name_hex="00"))
    attempt("wrong import status", lambda e: payload(e, 16).update(status=2))
    badrun = deepcopy(run)
    badrun["executable_sha256"] = "0" * 64
    try:
        verify_run(spec, run=badrun)
    except ValueError:
        tests.append({"mutation": "original hash binding", "rejected": True})
    else:
        raise ValueError("original hash tamper not rejected")
    badrun = deepcopy(run)
    badrun["inputs"][0]["sha256_after"] = "0" * 64
    try:
        verify_run(spec, run=badrun)
    except ValueError:
        tests.append({"mutation": "fixture copy hash", "rejected": True})
    else:
        raise ValueError("fixture hash tamper not rejected")
    for label, mutate in [
        ("probe source hash", lambda e, r: e["normalized_result"].update(source_sha256="0" * 64)),
        ("output transcript hash", lambda e, r: next(
            f for f in r["files"] if f["name"] == "stdout.bin").update(sha256="0" * 64)),
        ("wrong ready gate", lambda e, r: r.update(ready_gate=str(ROOT / "wrong.ready"))),
    ]:
        altered, badrun = deepcopy(evidence), deepcopy(run)
        mutate(altered, badrun)
        try:
            verify_run(spec, evidence=altered, run=badrun)
        except ValueError:
            tests.append({"mutation": label, "rejected": True})
        else:
            raise ValueError(f"provenance tamper not rejected: {label}")
    return tests


def main():
    runs = [verify_run(spec) for spec in SPECS]
    frozen_path = ROOT / "analysis/keyword-reader-candidate-freeze.json"
    frozen = read(frozen_path)
    report = {
        "status": "pass", "verifier": binding(Path(__file__)),
        "scope": "Original passive row-helper capture provenance and internal consistency; no replacement execution or replacement-generated expected values.",
        "template": binding(ROOT / "tools/observe_keyword_rows.js"),
        "runs": runs, "total_imports": sum(len(r["imports"]) for r in runs),
        "total_rows": sum(r["rows"] for r in runs),
        "candidate_freeze": {"manifest": binding(frozen_path), "declared_stage": frozen["stage"],
                             "files": [dict(f, current_matches=(ROOT / f["path"]).is_file()
                                       and digest(ROOT / f["path"]) == f["sha256"])
                                       for f in frozen["files"]]},
        "tampering_checks": tamper_checks(),
        "comparator_review": {
            "source": binding(ROOT / "tools/compare_keyword_readers.py"),
            "conclusion": "No material mismatch found within its declared per-helper replay scope.",
            "boundaries": [
                "Concatenates captured fgets chunks into isolated helper input, so cannot establish physical file position, token dispatch, object selector parsing or inter-helper state.",
                "Checks return codes, conversion counts, line attempts/cursor and exact integer/double output bytes; preserves negative zero when decoding replay JSON.",
                "Names are compared only through first NUL. Full 81-byte padding and prior uninitialized bytes are deliberately outside comparison.",
                "Does not compare import-after status or application model; this verifier preserves observed import statuses independently.",
            ],
        },
        "limitations": [
            "66 helper calls in nine declared fixture copies; not complete importer equivalence or a general numerical grammar proof.",
            "Only skip=0 and lines shorter than the 1024-byte fgets limit occur; locale, nonfinite values, overflow and long-line splitting remain untested.",
            "File accounting checks these ASCII fixtures with CRLF text translation; object-selector 1 and comment skipping are unhooked gaps checked against fixture syntax, not recovered token-reader coverage.",
            "Conversion counts are native observations checked for bounds/status/output consistency, not recomputed by a replacement or independent scanf implementation.",
            "Negative zero sign is retained in raw output_hex; Frida's JSON numeric value alone loses its sign.",
            "Name buffers are validated at 81 bytes in raw captures; report stores logical bytes through first NUL to omit uninitialized padding.",
            "Hash binding detects changed retained artifacts, not malicious coordinated rewriting of all records. Candidate freeze stage is a declared chronology, not a trusted timestamp.",
            "Candidate current hash matches are informational and do not gate original-capture provenance.",
        ],
    }
    output = ROOT / "analysis/keyword-row-observations.json"
    output.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"PASS: {report['total_imports']} imports, {report['total_rows']} row calls, "
          f"{sum(r['events'] for r in runs)} events, {len(report['tampering_checks'])} tampering checks")


if __name__ == "__main__":
    main()
