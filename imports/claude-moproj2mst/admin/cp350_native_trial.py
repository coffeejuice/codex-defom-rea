"""One fixed, explicitly invoked CP316 inert WORKER trial receipt owner.

Import and public CLI are inert. run_once() is private trial machinery, not a
general launcher: CP349 alone selects the fixed child tree. A returned TrialRun
retains the top owner if cleanup is uncertain. The ten-second controller timer
is practical containment, not a hard real-time or crash-atomic guarantee.
"""
from __future__ import annotations

from dataclasses import dataclass
import hashlib
import json
import os
from pathlib import Path
import re
import sys

NATIVE_ENTRY = "HARD HOLD"
ADMIN_DIR = Path(__file__).resolve().parent
ANALYSIS_DIR = ADMIN_DIR.parent / "analysis"
WORKER_IMAGE = Path(r"C:\PROBLEM\s316\worker.exe")
WORKER_BYTES = 104960
WORKER_SHA256 = "19886a0821d47611cc82dc44476ae222328e9c169b8e8ba9183cf92c3b655985"
WORKER_SOURCE = ADMIN_DIR / "fixtures" / "cp316" / "worker.c"
WORKER_SOURCE_SHA256 = "e657f3f4bcc69732238a50094a7766c91f801c3b05a38db88cd2795a63f48703"
WORKER_BUILD_RECEIPT = ANALYSIS_DIR / "cp316-worker-fixture-build.json"
WORKER_BUILD_SHA256 = "7dbf6936c3261e5faa83b1d29e3fd69e54963ab312670c8b1d9a058d6728e309"
PYTHON_IMAGE = Path(sys.executable).resolve()
REPORT_NAME = re.compile(r"cp350-inert-trial-[0-9a-f]{32}\.json\Z")
_used = False


class Hold(RuntimeError):
    pass


class TrialFailure(Hold):
    def __init__(self, message, trial):
        self.trial, self.owner, self.report = trial, trial.owner, trial.report
        super().__init__(message)


@dataclass
class TrialRun:
    owner: object | None
    report: dict
    report_path: Path

    @property
    def accepted(self):
        return self.report.get("accepted") is True


def _digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def _report_path(value):
    if type(value) is not str and not isinstance(value, Path):
        raise Hold("fixed analysis report path required")
    path = Path(value)
    if (not path.is_absolute() or
            not REPORT_NAME.fullmatch(path.name) or
            path.resolve(strict=False).parent != ANALYSIS_DIR.resolve() or
            path.exists()):
        raise Hold("new cp350 report name inside the converter analysis directory required")
    return path


def _source_manifest():
    """Current bytes only; caller must quiesce the tree during the trial."""
    files = sorted(ADMIN_DIR.glob("cp3*.py"), key=lambda p: p.name)
    if not files or not any(p.name == "cp349_top_controller.py" for p in files):
        raise Hold("fixed CP3xx source set missing")
    cache = ADMIN_DIR / "__pycache__"
    pyc = sorted(cache.glob("cp3*.pyc"), key=lambda p: p.name) if cache.exists() else []
    return {
        "source": {p.name: _digest(p) for p in files},
        "pyc": {p.name: _digest(p) for p in pyc},
    }


def _preflight():
    manifest = _source_manifest()
    if manifest["pyc"]:
        raise Hold("existing CP3xx bytecode cache prevents source-only trial binding")
    if not WORKER_IMAGE.is_file() or WORKER_IMAGE.stat().st_size != WORKER_BYTES:
        raise Hold("fixed CP316 WORKER image missing or wrong size")
    worker_hash = _digest(WORKER_IMAGE)
    if worker_hash != WORKER_SHA256:
        raise Hold("fixed CP316 WORKER digest mismatch")
    if (not WORKER_SOURCE.is_file() or
            _digest(WORKER_SOURCE) != WORKER_SOURCE_SHA256 or
            not WORKER_BUILD_RECEIPT.is_file() or
            _digest(WORKER_BUILD_RECEIPT) != WORKER_BUILD_SHA256):
        raise Hold("fixed CP316 source or build receipt identity mismatch")
    build = json.loads(WORKER_BUILD_RECEIPT.read_text(encoding="utf-8"))
    if (build.get("source_sha256") != WORKER_SOURCE_SHA256 or
            build.get("pe", {}).get("sha256") != WORKER_SHA256 or
            build.get("pe", {}).get("bytes") != WORKER_BYTES):
        raise Hold("CP316 source, build and executable identities disagree")
    if not PYTHON_IMAGE.is_file():
        raise Hold("fixed Python interpreter missing")
    return {
        "admin_modules": manifest,
        "worker": {"path": str(WORKER_IMAGE), "bytes": WORKER_BYTES,
                   "sha256": worker_hash},
        "worker_source": {"path": str(WORKER_SOURCE),
                          "sha256": WORKER_SOURCE_SHA256},
        "worker_build_receipt": {"path": str(WORKER_BUILD_RECEIPT),
                                 "sha256": WORKER_BUILD_SHA256},
        "python": {"path": str(PYTHON_IMAGE), "sha256": _digest(PYTHON_IMAGE)},
    }


def _load_exports():
    import cp325_real_bindings as binding
    return binding.load_native_exports()


def _make_controller(exports):
    import cp349_top_controller as top
    return top.TopController(exports)


def _receipt(owner):
    if owner is None:
        return None
    r = owner.receipt
    claim = getattr(owner, "claim", None)
    child_diagnostic = getattr(owner, "child_diagnostic_result", None)
    child_trace = getattr(owner, "child_trace_result", None)
    return {
        "phase": getattr(r, "phase", None),
        "deadline": getattr(r, "deadline", None),
        "pid": getattr(r, "pid", None),
        "tid": getattr(r, "tid", None),
        "create_outcome": getattr(r, "create_outcome", None),
        "resume_outcome": getattr(r, "resume_outcome", None),
        "runner_waited": getattr(r, "runner_waited", None),
        "job_empty": getattr(r, "job_empty", None),
        "job_terminated": getattr(r, "job_terminated", None),
        "process_terminated": getattr(r, "process_terminated", None),
        "errors": [str(e)[:400] for e in getattr(r, "errors", [])[:12]],
        "accepted_owner": getattr(owner, "_accepted", None),
        "timer_fired": getattr(owner, "_timer_fired", None),
        "owned_handle_labels": sorted(str(k) for k in getattr(owner, "_owned_handles", {})),
        "pipe_handle_count": len(getattr(getattr(owner, "pipe_owner", None), "handles", ())),
        "claim_deadline": getattr(claim, "deadline", None),
        "claim_nonce_hex": getattr(claim, "nonce", b"").hex() if claim is not None else None,
        "child_diagnostic": child_diagnostic,
        "child_trace": child_trace,
    }


def _write_report(path, report):
    # Exclusive creation retains partial writes if interrupted; never overwrite
    # or silently convert a missing/partial report to a successful observation.
    with path.open("x", encoding="utf-8", newline="\n") as stream:
        json.dump(report, stream, sort_keys=True, separators=(",", ":"))
        stream.write("\n")
        stream.flush()
        os.fsync(stream.fileno())


def run_once(report_path):
    """Run one fixed trial and return the owner even when cleanup is unresolved.

    No automatic second run is possible in this process. Only owner.close() is
    retryable. The caller must retain TrialRun until the exact cleanup receipt
    is inspected; failure is reported as data, not accepted by process exit.
    """
    global _used
    path = _report_path(report_path)
    if _used:
        raise Hold("CP350 trial is one-use in this process")
    _used = True
    owner = None
    report = {"schema": "cp350-inert-trial-v1", "accepted": False,
              "phase": "preflight-entered", "error": None,
              "cleanup": "not-attempted", "before": None, "after": None,
              "owner": None}
    try:
        report["before"] = _preflight()
        exports = _load_exports()
        owner = _make_controller(exports)
        result = owner.run()
        if (result is not owner.receipt or result.phase != "closed-success" or
                getattr(owner, "_accepted", None) is not True):
            raise Hold("exact CP349 closed-success owner required")
        report["phase"] = "top-success-source-recheck-pending"
    except BaseException as error:
        report["phase"] = "hold"
        report["error"] = f"{type(error).__name__}: {str(error)[:500]}"
        if owner is not None:
            try:
                closed = owner.close()
                report["cleanup"] = getattr(closed, "phase", "unknown")
            except BaseException as cleanup_error:
                report["cleanup"] = ("unknown: " + type(cleanup_error).__name__ +
                                     ": " + str(cleanup_error)[:300])
    finally:
        if report["before"] is not None:
            try:
                report["after"] = _preflight()
            except BaseException as error:
                report["after"] = {"error": f"{type(error).__name__}: {str(error)[:500]}"}
        report["owner"] = _receipt(owner)
    if report["phase"] == "top-success-source-recheck-pending":
        if report["after"] == report["before"]:
            report["phase"] = "closed-success"
            report["accepted"] = True
        else:
            report["phase"] = "hold"
            report["error"] = "source, interpreter or WORKER identity changed during trial"
    trial = TrialRun(owner, report, path)
    try:
        _write_report(path, report)
    except BaseException as error:
        report["accepted"] = False
        report["phase"] = "hold"
        report["error"] = "trial report write uncertain: " + str(error)[:500]
        raise TrialFailure("trial report write uncertain: " + str(error), trial) from error
    return trial


def launch_trial(*_args, **_kwargs):
    raise Hold("CP350 public native trial HARD HOLD pending frozen-source review")


if __name__ == "__main__":
    launch_trial()
