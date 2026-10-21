"""Read-only, source-bound views of retained JSON evidence (standard library only)."""
import argparse
import hashlib
import json
from pathlib import Path
import sys


def pointer_get(value, pointer):
    if pointer == "":
        return value
    if not pointer.startswith("/"):
        raise ValueError("JSON pointer must be empty or start with /")
    for part in pointer[1:].split("/"):
        # Reject malformed escapes rather than silently selecting a different key.
        if "~" in part.replace("~0", "").replace("~1", ""):
            raise ValueError("Invalid JSON pointer escape")
        key = part.replace("~1", "/").replace("~0", "~")
        if isinstance(value, list):
            if not key.isascii() or not key.isdigit() or (len(key) > 1 and key[0] == "0"):
                raise ValueError("Invalid array index")
            value = value[int(key)]
        else:
            value = value[key]
    return value


def describe(value):
    result = {"type": type(value).__name__}
    if isinstance(value, (dict, list, str)):
        result["length"] = len(value)
    if isinstance(value, dict):
        result["children"] = {k: {"type": type(v).__name__, **(
            {"length": len(v)} if isinstance(v, (dict, list, str)) else {})}
            for k, v in value.items()}
    return result


def extract(path, pointer=None, start=0, count=40, contains=None, expected_sha256=None, record_pointer=""):
    if start < 0 or not 1 <= count <= 200:
        raise ValueError("start must be nonnegative; count must be 1..200")
    path = Path(path).resolve(strict=True)
    raw = path.read_bytes()
    digest = hashlib.sha256(raw).hexdigest()
    if expected_sha256 and digest != expected_sha256.lower():
        raise ValueError("Source SHA-256 mismatch")
    document = pointer_get(json.loads(raw.decode("utf-8-sig")), record_pointer)
    if not isinstance(document, dict):
        raise ValueError("Expected a JSON object evidence document")
    result = {
        "schema_version": 1,
        "source": {"path": str(path), "bytes": len(raw), "sha256": digest,
                   "record_pointer": record_pointer},
        "identity": {k: document[k] for k in (
            "evidence_id", "subject", "provider", "operation", "parameters",
            "confidence", "authority", "predicate_type") if k in document},
        "qualifications": {},
        "notice": "Selected retained data only; not new analysis or proof of complete behavior.",
    }
    for location in ("/limitations", "/relationship_coverage", "/analysis_profile/digest",
                     "/raw_result/limitations", "/raw_result/relationship_coverage",
                     "/normalized_result/limitations", "/normalized_result/facet_availability"):
        try:
            result["qualifications"][location] = pointer_get(document, location)
        except (KeyError, IndexError, TypeError):
            pass
    if pointer is None:
        if contains is not None:
            raise ValueError("--contains requires --pointer")
        result["inventory"] = describe(document)
        if "normalized_result" in document:
            result["normalized_inventory"] = describe(document["normalized_result"])
        return result
    value = pointer_get(document, pointer)
    selection = {"pointer": pointer, "description": describe(value)}
    if isinstance(value, (list, str)):
        items = value.splitlines(keepends=True) if isinstance(value, str) else value
        indexed = list(enumerate(items))
        if contains is not None:
            indexed = [(i, v) for i, v in indexed if contains in (
                v if isinstance(v, str) else json.dumps(v, ensure_ascii=False))]
        page = indexed[start:start + count]
        selection.update({
            "unit": "text_line" if isinstance(value, str) else "array_item",
            "indices_are_zero_based": True, "source_items": len(items),
            "contains_literal_case_sensitive": contains, "matching_items": len(indexed),
            "start": start, "count": count,
            "items": [{"index": i, "value": v} for i, v in page],
            "omitted_items": len(items) - len(page),
            "next_start": start + len(page) if start + len(page) < len(indexed) else None,
        })
    else:
        if contains is not None:
            raise ValueError("--contains supports only text or arrays")
        # Objects remain inventories; callers select an explicit child pointer.
        if not isinstance(value, dict):
            selection["value"] = value
    result["selection"] = selection
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("path")
    parser.add_argument("--pointer")
    parser.add_argument("--record-pointer", default="", help="Select a nested evidence record before applying --pointer")
    parser.add_argument("--start", type=int, default=0)
    parser.add_argument("--count", type=int, default=40)
    parser.add_argument("--contains")
    parser.add_argument("--expect-sha256")
    args = parser.parse_args()
    try:
        result = extract(args.path, args.pointer, args.start, args.count,
                         args.contains, args.expect_sha256, args.record_pointer)
    except (OSError, ValueError, KeyError, IndexError, TypeError) as error:
        print(json.dumps({"error": str(error)}), file=sys.stderr)
        return 2
    sys.stdout.reconfigure(encoding="utf-8")
    print(json.dumps(result, ensure_ascii=False, indent=2))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
