"""Bounded address discovery in retained JSON; never accesses REA or target processes."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import sys


def sha256(raw):
    return hashlib.sha256(raw).hexdigest()


def encode(value):
    return (json.dumps(value, ensure_ascii=False, separators=(",", ":")) + "\n").encode("utf-8")


def address(value):
    if not isinstance(value, str) or not re.fullmatch(r"0[xX][0-9a-fA-F]+", value):
        raise ValueError("Procedure must be a hexadecimal address with 0x prefix")
    return hex(int(value, 16))


def records(document):
    if not isinstance(document, dict):
        raise ValueError("Unsupported evidence root")
    if isinstance(document.get("evidence_bundle"), dict) and isinstance(document["evidence_bundle"].get("records"), list):
        return [(f"/evidence_bundle/records/{i}", r) for i, r in enumerate(document["evidence_bundle"]["records"])]
    if isinstance(document.get("records"), list):
        return [(f"/records/{i}", r) for i, r in enumerate(document["records"])]
    if "evidence_id" in document:
        return [("", document)]
    raise ValueError("Unsupported evidence container; provide explicit supported sources")


def body_pointers(record):
    result = []
    for key in ("normalized_result", "raw_result"):
        value = record.get(key)
        if key == "normalized_result" and isinstance(value, str) and record.get("operation") == "procedure_pseudo_code":
            result.append("/normalized_result")
        if isinstance(value, dict):
            if isinstance(value.get("pseudocode"), str):
                result.append(f"/{key}/pseudocode")
            nested = value.get("value")
            if isinstance(nested, str) and record.get("operation") == "procedure_pseudo_code":
                result.append(f"/{key}/value")
            if isinstance(nested, dict) and isinstance(nested.get("pseudocode"), str):
                result.append(f"/{key}/value/pseudocode")
    return result


def build(paths):
    sources, entries = [], []
    resolved = sorted({str(Path(p).resolve(strict=True)) for p in paths})
    if not resolved:
        raise ValueError("At least one source required")
    for path in resolved:
        raw = Path(path).read_bytes()
        rows = records(json.loads(raw.decode("utf-8-sig")))
        source = {"path": path, "bytes": len(raw), "sha256": sha256(raw),
                  "records_seen": len(rows), "records_indexed": 0, "records_skipped": 0}
        for pointer, row in rows:
            try:
                proc = address(row["parameters"]["procedure"])
                target = row["subject"]["digest"]["sha256"].lower()
                evidence_id, operation = row["evidence_id"], row["operation"]
                if not re.fullmatch(r"[0-9a-f]{64}", target) or not evidence_id or not operation:
                    raise ValueError("Incomplete identity")
            except (KeyError, TypeError, ValueError, AttributeError):
                source["records_skipped"] += 1
                continue
            entries.append({"source_path": path, "source_sha256": source["sha256"],
                            "record_pointer": pointer, "evidence_id": evidence_id,
                            "target_sha256": target, "operation": operation,
                            "procedure": proc, "body_pointers": body_pointers(row)})
            source["records_indexed"] += 1
        sources.append(source)
    return {"schema_version": 1, "scope_policy": "Only explicitly listed files, including ignored files; no recursive or global absence claim",
            "sources": sources, "entries": entries}


def query(index_path, expected_index_sha256, procedure, target_sha256, operation=None):
    raw_index = Path(index_path).read_bytes()
    if sha256(raw_index) != expected_index_sha256.lower():
        raise ValueError("Index SHA-256 mismatch; use the build receipt or rebuild")
    index = json.loads(raw_index)
    if index["schema_version"] != 1:
        raise ValueError("Unsupported index version")
    proc = address(procedure)
    target = target_sha256.lower()
    if not re.fullmatch(r"[0-9a-f]{64}", target):
        raise ValueError("Target SHA-256 must contain 64 hexadecimal digits")
    # Hash ALL sources, including for misses. Size/mtime alone can miss edits.
    # Streaming avoids reparsing and retaining the 150MB bundle for each query.
    for source in index["sources"]:
        with Path(source["path"]).open("rb") as stream:
            digest = hashlib.file_digest(stream, "sha256").hexdigest()
        if digest != source["sha256"]:
            raise ValueError(f"Stale source: {source['path']}; rebuild index")
    matches = [entry for entry in index["entries"] if entry["procedure"] == proc
               and entry["target_sha256"] == target
               and (operation is None or entry["operation"] == operation)]
    return {"status": "matches" if matches else "not_found_in_indexed_scope",
            "procedure": proc, "target_sha256": target, "operation": operation,
            "scope": index["sources"], "matches": matches,
            "notice": "Metadata discovery only. Skipped records and files outside scope remain unknown. Read source-bound bodies and qualifications before drawing conclusions; files may change after validation."}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    commands = parser.add_subparsers(dest="command", required=True)
    make = commands.add_parser("build")
    make.add_argument("sources", nargs="+")
    make.add_argument("--output", required=True, help="Fresh index path; existing files/links are rejected")
    find = commands.add_parser("query")
    find.add_argument("index")
    find.add_argument("--expect-index-sha256", required=True)
    find.add_argument("--procedure", required=True)
    find.add_argument("--target-sha256", required=True)
    find.add_argument("--operation")
    args = parser.parse_args()
    try:
        if args.command == "build":
            # Do not resolve the final component: even a dangling link counts
            # as an existing output. Path equality cannot detect hard links.
            output = Path(args.output).absolute()
            if os.path.lexists(output):
                raise ValueError("Index output already exists; choose a fresh path")
            index = build(args.sources)
            raw = encode(index)
            # Exclusive creation is the authoritative check: an output created
            # after the preflight must fail, never truncate its underlying file.
            with output.open("xb") as stream:
                stream.write(raw)
            result = {"index": str(output), "index_sha256": sha256(raw), "index_bytes": len(raw),
                      "sources": index["sources"], "entries": len(index["entries"])}
        else:
            result = query(args.index, args.expect_index_sha256, args.procedure, args.target_sha256, args.operation)
        sys.stdout.buffer.write(encode(result))
        return 0
    except (OSError, ValueError, KeyError, TypeError) as error:
        print(json.dumps({"error": str(error)}), file=sys.stderr)
        return 2


if __name__ == "__main__":
    raise SystemExit(main())
