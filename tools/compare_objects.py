"""Compare current object summaries with passive original importer observations."""
from __future__ import annotations

import json
from pathlib import Path
import subprocess

from compare_runtime import sha256


def check(condition: bool, message: str) -> None:
    if not condition:
        raise RuntimeError(message)


def main() -> int:
    root = Path(__file__).resolve().parents[1]
    executable = root / "build/Release/deform_objects.exe"
    report = {"executable_sha256": sha256(executable), "cases": []}
    for label, step in [("small", 25), ("large", 690)]:
        evidence_path = root / "analysis" / f"import-objects-{label}.json"
        evidence = json.loads(evidence_path.read_text())
        manifest_path = root / "analysis/oracle/interactive" / f"import-objects-{label}/run.json"
        capture = json.loads(manifest_path.read_text())
        observed = evidence["normalized_result"]
        check(capture["status"] == "completed" and capture["exit_code"] == 0, "Reference failed")
        check(capture["pid"] == observed["pid"], "Reference process identity differs")
        check(sha256(Path(capture["executable"])) == capture["executable_sha256"],
              "Reference executable changed")
        check(sha256(root / "tools/observe_objects.js") == observed["source_sha256"],
              "Probe source changed")
        fixture = Path(capture["input_copy"])
        check(sha256(fixture) == capture["input_copy_sha256_before"] ==
              capture["input_copy_sha256_after"], "Input fixture changed")
        check(not observed["messages_truncated"] and observed["cleanup_error"] is None,
              "Observation incomplete")
        messages = observed["messages"]
        check(all(m.get("type") != "error" for m in messages), "Probe script error")
        events = [m["payload"] for m in messages if m.get("type") == "send"]
        check(all(e.get("operation") != "probe-error" for e in events), "Probe read error")
        imports = [e for e in events if e.get("function") == "import-step"
                   and e["operation"] == "leave"]
        check(len(imports) == 1 and imports[0]["status"] == 0, "Import did not succeed")
        tables = next(e for e in events if e.get("function") == "read-objects"
                      and e["operation"] == "enter")
        inputs = {e["id"]: e for e in events if e["operation"] == "object-input"}
        outputs = [e for e in events if e["operation"] == "object-output"]
        ia, ra = tables["integer_address_table"], tables["real_address_table"]
        expected = []
        for obj in outputs:
            slot = obj["id"] - 1
            d = inputs[obj["id"]]["integer_descriptor"]
            name_bytes = bytes(obj["name_bytes"]).split(b"\0", 1)[0]
            expected.append({
                "id": obj["id"], "active": d[0] != 0, "has_mesh": obj["mesh_flag"] == 1,
                "raw_type": d[1], "internal_type": obj["internal_type"],
                "node_count": obj["node_count"], "element_count": obj["element_count"],
                "name": name_bytes.decode("latin1"),
                "integer_descriptor_index": ia[3] - 1 + ia[73] * slot,
                "real_descriptor_index": ra[5] - 1 + ra[145] * slot,
            })
        check(len(expected) == tables["object_count"], "Reference objects missing")
        result_path = root / "analysis" / f"object-summary-{label}.json"
        run = subprocess.run(
            [str(executable), str(fixture), str(step), str(result_path)],
            capture_output=True, timeout=60,
            creationflags=getattr(subprocess, "CREATE_NO_WINDOW", 0),
        )
        check(run.returncode == 0, run.stderr.decode(errors="replace"))
        actual = json.loads(result_path.read_text())
        matches = actual["objects"] == expected and actual["step"] == step
        report["cases"].append({
            "step": step, "evidence_id": evidence["evidence_id"], "pid": capture["pid"],
            "capture_manifest": str(manifest_path),
            "reference_executable_sha256": capture["executable_sha256"],
            "fixture_sha256": capture["input_copy_sha256_before"],
            "original": expected, "cpp": actual["objects"], "all_match": matches,
        })
        print(f"step {step}: {len(expected)} objects; {'MATCH' if matches else 'MISMATCH'}")
    report["all_match"] = all(c["all_match"] for c in report["cases"])
    (root / "analysis/object-comparison.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8"
    )
    return 0 if report["all_match"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
