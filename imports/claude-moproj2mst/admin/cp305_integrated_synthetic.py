"""CP305 fake-only CP303/CP304 transaction for a supplied fixed CP302 image.

The image contract checks caller fields, not CP289 clone provenance or disk bytes.
CP306 must integrate that provenance before any real child entry is considered.
Callbacks and fake markers are conventions, not a security boundary. Use only
reviewed in-memory fakes; this module never resumes the suspended child.
"""
from __future__ import annotations

from dataclasses import dataclass
import re

import cp303_job_creation as creation
import cp304_job_supervisor as supervisor_protocol


@dataclass(frozen=True)
class FixedTransaction:
    image: creation.FixedCloneImage
    occurrence: str


def _validate_transaction(tx: FixedTransaction) -> tuple[str, str]:
    if not isinstance(tx, FixedTransaction):
        raise ValueError("fixed transaction required")
    exe, cwd = creation._validate_supplied_fixed_image_contract(tx.image)
    if not isinstance(tx.occurrence, str) or re.fullmatch(r"cp305-[0-9a-f]{16}", tx.occurrence) is None:
        raise ValueError("fixed CP305 occurrence token required")
    return exe, cwd


def _handle_value(value):
    value = getattr(value, "value", value)
    return value if type(value) is int and value > 0 else None


class FakeTransaction:
    """One in-memory transaction; all returned fake handles remain accounted."""

    def __init__(self, api, transaction: FixedTransaction):
        self.exe, self.cwd = _validate_transaction(transaction)
        creation._fake_only(api)
        self.api = api
        self.transaction = transaction
        self.supervisor = supervisor_protocol.FakeJobSupervisor(api, transaction.image)
        self.phase = "new"
        self.worker_process_handle = None
        self.worker_thread_handle = None
        self.report = {"status": "not-started", "errors": [], "local_handle_closes": {},
                       "retained_local_handles": {}}

    def _take_process_information(self, pi):
        # Take ownership before checking the complete offer: partial PI output on
        # a failed CreateProcessW must still be accounted if Windows returned it.
        self.worker_process_handle = _handle_value(pi.hProcess)
        self.worker_thread_handle = _handle_value(pi.hThread)

    def _close_local_handles(self, *, wait_ms):
        process_exited = False
        if self.worker_process_handle is not None:
            try:
                result = self.api.WaitForSingleObject(self.worker_process_handle, wait_ms)
                process_exited = result == supervisor_protocol.WAIT_OBJECT_0
                self.report["local_process_wait"] = (
                    "exited" if process_exited else "timeout or failed or unknown")
            except BaseException as error:
                self.report["local_process_wait"] = "failed or unknown"
                self.report["errors"].append(
                    f"local process wait: {type(error).__name__}: {error}")
        for label, attr in (("thread", "worker_thread_handle"),
                            ("process", "worker_process_handle")):
            handle = getattr(self, attr)
            if handle is None:
                continue
            if label == "process" and not process_exited:
                self.report["local_handle_closes"][label] = "deferred; exact handle retained"
                continue
            try:
                closed = self.api.CloseHandle(handle)
            except BaseException as error:
                self.report["errors"].append(f"CloseHandle {label}: {type(error).__name__}: {error}")
                closed = False
            self.report["local_handle_closes"][label] = "succeeded" if closed is True else "failed or unknown"
            if closed is True:
                setattr(self, attr, None)
            else:
                self.report["errors"].append(f"CloseHandle {label} did not confirm closure")
        self.report["retained_local_handles"] = {
            label: handle for label, handle in (("thread", self.worker_thread_handle),
                                                ("process", self.worker_process_handle))
            if handle is not None
        }

    def run(self, *, worker_factory, send_ack, ack_receipt_factory, wait_ms=2000):
        if self.phase != "new":
            raise RuntimeError("transaction can only be run once; cleanup remains retryable")
        if type(wait_ms) is not int or not 0 <= wait_ms <= 5000:
            raise ValueError("bounded wait in [0, 5000] milliseconds required")
        self.phase = "running"
        failed = False
        try:
            self.supervisor.stage_job()
            worker_handle = worker_factory(self.transaction)
            worker_job_handle = self.supervisor.bind_worker(worker_handle)
            self.supervisor.start_worker()
            try:
                pi = creation.worker_create_suspended_for_fake_test(
                    self.api, self.transaction.image, worker_job_handle)
            except creation.CreateOutcomeUnknown as error:
                if error.process_information is not None:
                    self._take_process_information(error.process_information)
                self.supervisor.note_create_interrupted()
                raise
            self._take_process_information(pi)
            pid, tid = int(pi.dwProcessId), int(pi.dwThreadId)
            if (self.worker_process_handle is None or self.worker_thread_handle is None or
                    pid <= 0 or tid <= 0):
                raise ValueError("CreateProcessW returned incomplete PROCESS_INFORMATION")
            created = self.api.GetProcessTimes(self.worker_process_handle)
            if type(created) is not int or created <= 0:
                raise creation.ApiFailure("GetProcessTimes worker-local child", self.api.last_error())
            offer = supervisor_protocol.ChildOffer(
                self.transaction.occurrence, self.worker_process_handle, pid, tid,
                created, self.exe)
            ack = self.supervisor.accept_offer(offer, send_ack)
            receipt = ack_receipt_factory(ack)
            self.supervisor.record_ack_receipt(receipt)
            self.report["status"] = "acknowledged-unverified-receipt"
            return self.report
        except BaseException as error:
            failed = True
            self.report["status"] = "failed-or-uncertain"
            self.report["errors"].append(f"{type(error).__name__}: {error}")
            raise
        finally:
            try:
                self.cleanup(wait_ms=wait_ms)
            except BaseException as error:
                self.report["errors"].append(f"cleanup: {type(error).__name__}: {error}")
                self.report["status"] = "failed-or-uncertain"
                if not failed:
                    raise
            finally:
                self.phase = "finished"

    def cleanup(self, *, wait_ms=2000):
        # CP304 retains exact handles when termination, waits, accounting or close
        # is uncertain. Terminate and wait before considering the worker-local
        # process handle for close, including when no supervisor offer exists.
        result = self.supervisor.cleanup(wait_ms=wait_ms)
        self._close_local_handles(wait_ms=wait_ms)
        self.report["cleanup"] = result
        if (self.report["status"] == "acknowledged-unverified-receipt" and
                (result.retained_handles or result.errors or
                 self.report["retained_local_handles"])):
            self.report["status"] = "acknowledged-cleanup-uncertain"
        return result


def run_integrated_fake_transaction(*_args, **_kwargs):
    raise RuntimeError("CP305 real transaction entry HOLD; CP306 provenance pending")

