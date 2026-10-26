"""Capture the bounded native editor300 Extra3D backend checkpoint."""
from pathlib import Path
import hashlib
import json
import re

ROOT = Path(__file__).resolve().parents[1]


def record(path):
    data = (ROOT / path).read_bytes()
    return {"path": path, "bytes": len(data), "sha256": hashlib.sha256(data).hexdigest()}


sources = [
    "source/apps/moproj2mst/include/m2m/engine/simcontrol300_io.hpp",
    "source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp",
    "source/apps/moproj2mst/tests/mdt/test_simcontrol300_native_io.cpp",
]
evidence = [
    "analysis/wave2-save-diffs-300-native-reader-contract.json",
    "analysis/wave2-save-diffs-qt-textstream-real-tables.json",
    "analysis/wave2-save-diffs-qt-textstream-getreal-complete.asm",
    "analysis/wave2-save-diffs-qt-textstream-getinteger-complete.asm",
    "analysis/wave2-save-diffs-qt-textstream-read-int-complete.asm",
    "analysis/wave2-save-diffs-qt-textstream-read-double-complete.asm",
    "analysis/wave2-save-diffs-qt-textstream-default-exports.txt",
    "analysis/wave2-save-diffs-qt-textstream-device-ctor.asm",
    "analysis/wave2-save-diffs-qt-textstream-private-ctor.asm",
    "analysis/wave2-save-diffs-qt-textstream-private-reset.asm",
    "analysis/wave2-doe-read-handle-contract.json",
    "analysis/wave2-save-diffs-300-extra3d-increment.json",
    "analysis/wave3-distortion-cogging-validation.json",
]
cases = re.findall(r"TEST_CASE\(([^)]+)\)", (ROOT / sources[-1]).read_text())
receipt = {
    "status": "native_extra3d_bounded_backend_implemented_frozen_for_root_build",
    "sources": [record(path) for path in sources],
    "evidence": [record(path) for path in evidence],
    "original_exe_sha256": "fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc",
    "original_qtcore_sha256": "bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160",
    "contract": {
        "directory": "Explicit absolute per-call directory replaces original process working directory; no ambient original installation access or directory creation.",
        "read": "Zero outputs; exists before native ReadOnly open. Reuse verified StudyIo GENERIC_READ/share READ|WRITE/no DELETE/OPEN_EXISTING/normal/noninheritable handle. Successful count0 open does not read or decode bytes.",
        "real": "KnownC ASCII, original 13x10 getReal transition table and delimiter pushback, 124-character bound plus trailing-digit consumption; existing qt::toDouble conversion. Failure yields zero and later extractions continue despite sticky stream status.",
        "integer": "Explicit first profile: signed decimal int32, no ambiguous leading zeros, whitespace or end delimiter. Automatic radix selection, wrapping overflow and other original integer branches refuse; they are not approximated as zero.",
        "defaults": "Qt device constructor -> private constructor creates C QLocale; private reset initializes integerBase0. Base0 is original evidence; restricted decimal handling is an adapter profile guard.",
        "springs": "Absent, empty or nonpositive count produces no spring items. Positive count explicitly refuses before selected-editor behavior can be approximated.",
        "write": "Existing FileDevice plus recovered Extra3DIo Stream helper, WriteOnly without Text, literal tabs and CRLF preserved. Open success remains the original helper result; later write failures do not abort later blocks.",
        "delete": "CRT wide fopen(rt), fclose, remove through existing path seam; read-open failure returns true. Only failed NBC removal invokes supplied warning callback.",
    },
    "tests": {
        "status": "not_run_worker_root_owns_build_and_execution",
        "new_cases": cases,
        "new_cases_count": len(cases),
        "prior_validation": "analysis/wave3-distortion-cogging-validation.json: root reports full110 CTests and40 editor helper cases passing, no fixture/diagnostic changes",
        "expected_editor_helper_cases": 40 + len(cases),
        "coverage": "Native empty and absent files, count0 binary content, real prefix/pushback/failure continuation, signed zero, explicit integer/decoder/spring guards, remembered read-open denial, write sharing failure continuation, NBC delete failure warning and later-file continuation, no missing-parent creation, absolute-directory requirement.",
    },
    "checks": "git diff --check and owned-file trailing-whitespace inspection passed; no build/test execution by worker.",
    "qualifications": [
        "No original DLL loading, original executable runtime, app control or fixture mutation.",
        "No general Unicode/BOM or non-C locale decoder equivalence; unsupported bytes explicitly refuse when extraction is requested.",
        "Existing StudyIo readAll packet sizing and dynamic-file/read-fault timing remain qualified; native open flags alone are not total QFile equivalence.",
        "Existing long-path/Qt path seams and numeric conversion seams retain their inherited qualifications.",
        "Positive spring selected-editor state, populated recovery lists, whole initial-editor composition and external capability/remesh lifecycle remain queued.",
        "Public mode300 Save remains explicitly refused; this checkpoint is not public integration or original lifecycle acceptance.",
    ],
    "successor": "No new implementation scope before parent integration. Retained original integer source permits bounded automatic-base recovery afterward; public composition still requires remaining lifecycle/capability bindings.",
}
target = ROOT / "analysis/wave2-save-diffs-300-native-io-increment.json"
target.write_text(json.dumps(receipt, indent=2) + "\n", encoding="utf-8")
print(json.dumps({"receipt": str(target.relative_to(ROOT)), "sha256": hashlib.sha256(target.read_bytes()).hexdigest(), "new_cases": len(cases)}))
