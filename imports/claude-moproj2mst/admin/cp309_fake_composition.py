"""CP309 one-use fake-only composition of retained clone and dual-job seams.

All injected APIs are trusted in-memory fakes. The protocol receipts are local
claims, never authenticated worker acceptance or authority to resume a child.
"""
from __future__ import annotations

from dataclasses import dataclass, field

import color_atomic_clone_acquire as clone
import cp303_job_creation as child_creation
import cp307_fixed_clone_preflight as preflight
import cp307_offer_protocol as offer_protocol
import cp307_worker_job as worker_job
import cp308_real_job_protocol as owner_protocol


class _ConservativeOwnerApi:
    """Fake CP308 adapter that defers job close after lost helper outputs."""

    cp308_fake_api = True

    def __init__(self, delegate):
        self.delegate = delegate
        self.hold_jobs = False
        self.job_handles = set()

    def __getattr__(self, name):
        return getattr(self.delegate, name)

    def CloseHandle(self, handle):
        if self.hold_jobs and handle in self.job_handles:
            return False
        return self.delegate.CloseHandle(handle)


@dataclass
class Result:
    outcome: str = "INCOMPLETE_OR_UNCERTAIN"
    preflight: dict | None = None
    cp307_phase: str = "not-started"
    cp308_phase_before_cleanup: str = "not-started"
    cleanup_phase: str = "not-started"
    retained_handles: dict[str, int] = field(default_factory=dict)
    worker_local_close: dict[str, str] = field(default_factory=dict)
    uncertain_worker_local: list[str] = field(default_factory=list)
    worker_local_child_handle_outcome: str = "not-attempted"
    worker_local_child_job_duplicate_outcome: str = "not-attempted"
    cp307_occurrence: tuple[str, str, str] | None = None
    cp308_occurrence: tuple[str, str] | None = None
    occurrence_mapping_scope: str = (
        "CP308 omits CP307 occurrence_id; fake one-use composition only; native replay/isolation HOLD")
    errors: list[str] = field(default_factory=list)
    fake_worker_claim_only: bool = True


class FakeComposition:
    """One CP289 callback and one child attempt; no live execution entry."""

    def __init__(self, *, clone_api, file_api, api, transport, run_id,
                 occurrence_id, child_id, worker_id, supervisor_id, offer_id):
        if (getattr(clone_api, "cp307_fake_clone_api", False) is not True
                or isinstance(clone_api, clone.WindowsCloneFiles)
                or getattr(file_api, "cp307_fake_file_api", False) is not True
                or getattr(api, "cp303_fake_api", False) is not True
                or getattr(api, "cp307_fake_api", False) is not True
                or getattr(api, "cp308_fake_api", False) is not True
                or getattr(transport, "cp308_fake_transport", False) is not True):
            raise RuntimeError("CP309 real worker/child/original/GUI HARD HOLD")
        if not all(isinstance(value, str) and value for value in
                   (run_id, occurrence_id, child_id, worker_id, supervisor_id, offer_id)):
            raise ValueError("complete fake occurrence and binding IDs required")
        worker_job._fake_only(api)
        self.clone_api, self.file_api = clone_api, file_api
        self.api, self.transport = api, transport
        self.owner_api = _ConservativeOwnerApi(api)
        self.run_id, self.occurrence_id, self.child_id = run_id, occurrence_id, child_id
        self.worker_id, self.supervisor_id, self.offer_id = worker_id, supervisor_id, offer_id
        self.result = Result()
        self.used = False
        self.owner = None
        self.worker = None
        self.child_job = None
        self._active_receipt = None
        self._activation = None

    def _callback(self, receipt, token):
        if token is None or token is not self._activation or receipt is not self._active_receipt:
            raise RuntimeError("CP309 retained callback activation missing")
        if self.used:
            raise RuntimeError("CP309 retained callback is one-use")
        self.used = True
        if receipt.get("acquisitionOutcome") != "INCOMPLETE" or receipt.get("allHandlesClosed") is True:
            raise ValueError("CP289 retained callback must remain pending")
        result = self.result
        transferred = False
        worker_local = {}
        duplicate_attempted = duplicate_confirmed = False
        child_create_entered = child_create_returned = False
        try:
            result.preflight = preflight._measure_retained(receipt, file_api=self.file_api, pending=True)
            image = result.preflight["image"]
            root = result.preflight["cwd"]
            self.worker = worker_job.FakeWorkerJob(self.api)
            self.worker.stage_job()
            self.worker.create_suspended_worker()
            self.worker.verify_worker_gate()
            self.child_job = child_creation.stage_supervisor_job_for_fake_test(self.api)
            self.owner = owner_protocol.FakeRealJobProtocol(
                self.owner_api, self.transport,
                occurrence=owner_protocol.Occurrence(self.run_id, self.child_id),
                worker_id=self.worker_id, fixed_clone_image=image,
                fixture_sha256=result.preflight["fixtureSha256"])
            self.worker.transfer_to(self.owner, child_job=self.child_job.handle)
            transferred = True
            # CP303's returned process/thread handles are worker-local fake values.
            # The supervisor sees only CP308's duplicated exact process handle.
            duplicate_attempted = True
            worker_local_child_job = child_creation.duplicate_job_to_worker_for_fake_test(
                self.api, self.child_job, self.owner.worker_process)
            duplicate_confirmed = True
            result.worker_local_child_job_duplicate_outcome = "known fake-local handle value"
            worker_local["child_job_duplicate"] = worker_local_child_job
            child_create_entered = True
            process = child_creation.worker_create_suspended_for_fake_test(
                self.api, child_creation.FixedCloneImage(
                    image, root, result.preflight["fixtureSha256"]), worker_local_child_job)
            child_create_returned = True
            result.worker_local_child_handle_outcome = "known fake-local PROCESS_INFORMATION"
            worker_local["child_process"] = int(process.hProcess) if process.hProcess else None
            worker_local["child_thread"] = int(process.hThread) if process.hThread else None
            if not all((process.hProcess, process.hThread, process.dwProcessId, process.dwThreadId)):
                raise RuntimeError("incomplete fake child PROCESS_INFORMATION")
            # CP307 and CP308 frame classes are intentionally distinct. The CP307
            # in-memory handshake is a claim, while CP308 checks an exact duplicate.
            p307 = offer_protocol.FakeOfferProtocol(
                offer_protocol.Occurrence(self.run_id, self.occurrence_id, self.child_id),
                worker_identity=self.child_id, supervisor_identity=self.supervisor_id)
            frame307 = p307.worker_send_offer(self.offer_id)
            result.cp307_occurrence = (frame307.occurrence.run_id,
                                       frame307.occurrence.occurrence_id,
                                       frame307.occurrence.child_id)
            frame308 = owner_protocol.Offer(
                self.owner.occurrence, frame307.offer_id, int(process.hProcess),
                int(process.dwProcessId), self.api.GetProcessTimes(int(process.hProcess)), image)
            result.cp308_occurrence = (frame308.occurrence.run, frame308.occurrence.child)
            if (frame307.occurrence.run_id != frame308.occurrence.run or
                    frame307.occurrence.child_id != frame308.occurrence.child):
                raise RuntimeError("CP307 and CP308 OFFER occurrence mismatch")
            ack308 = self.owner.accept_offer(frame308)
            ack307 = p307.supervisor_receive_offer(frame307)
            if (ack308.offer_id != ack307.offer.offer_id or
                    ack307.offer.occurrence.run_id != ack308.occurrence.run or
                    ack307.offer.occurrence.child_id != ack308.occurrence.child):
                raise RuntimeError("CP307 and CP308 ACK occurrence/offer mismatch")
            # A test harness may model receipt delivery. Neither receipt proves an
            # external worker accepted an ACK, and no resume path exists here.
            p307.supervisor_receive_receipt(p307.worker_receive_ack(ack307))
            self.owner.receive_worker_receipt(owner_protocol.WorkerReceipt(ack308, self.worker_id))
            result.cp307_phase = p307.phase.name
            result.cp308_phase_before_cleanup = self.owner.report.phase
        except BaseException as error:
            result.errors.append(f"{type(error).__name__}: {error}")
            if duplicate_attempted and not duplicate_confirmed:
                result.worker_local_child_job_duplicate_outcome = "unknown; CP303 helper did not return"
            if child_create_entered and not child_create_returned:
                result.worker_local_child_handle_outcome = "unknown; CP303 helper did not return"
            if isinstance(error, child_creation.CreateOutcomeUnknown):
                partial = error.process_information
                if partial is not None:
                    worker_local["child_process"] = int(partial.hProcess) if partial.hProcess else None
                    worker_local["child_thread"] = int(partial.hThread) if partial.hThread else None
            if (self.owner is not None and transferred and child_create_entered
                    and not child_create_returned and
                    self.owner.report.phase == "worker-gate-checked"):
                self.owner.report.create_outcome = (
                    "unknown: CP303 child helper did not return PROCESS_INFORMATION")
                try:
                    self.owner._hold("CP303 child helper outcome unknown; exact child handle unavailable")
                except owner_protocol.ProtocolHold:
                    pass
        finally:
            # CP308 is the only post-transfer cleanup owner. Before transfer,
            # CP307 owns WORKER handles and CP303 owns the CHILD job.
            owner_adopted_child_job = (self.child_job is not None and self.owner is not None
                                       and self.owner.child_job == self.child_job.handle)
            if self.owner is not None and (
                    (duplicate_attempted and not duplicate_confirmed) or
                    (child_create_entered and not child_create_returned)):
                self.owner_api.hold_jobs = True
                self.owner_api.job_handles = {
                    handle for handle in (self.owner.worker_job, self.owner.child_job)
                    if handle is not None}
            # CP307 can disarm any prefix observed in CP308's public fields
            # even if a failed bind did not reach CP308._retain(). Reconcile
            # those fields before deciding whether CP308 owns cleanup.
            owner_has_handles = (self.owner is not None and any(
                getattr(self.owner, name) is not None for name in (
                    "worker_job", "child_job", "worker_process", "worker_thread",
                    "child_process")))
            if owner_has_handles:
                self.owner._retain()
                try:
                    report = self.owner.cleanup()
                    result.cleanup_phase = report.phase
                    result.retained_handles.update(report.retained)
                except BaseException as error:
                    result.errors.append(f"CP308 cleanup: {type(error).__name__}: {error}")
                    result.retained_handles.update(self.owner.report.retained)
            # Worker-local handle values must never be passed to supervisor
            # CloseHandle. A verified WORKER process exit closes its handle
            # table. Until then, retain these obligations as uncertain.
            worker_exit = (self.owner is not None and
                           self.owner.report.worker_wait == "exited")
            exact_child_exit = (self.owner is not None and
                                self.owner.report.child_wait == "exited")
            for name, handle in worker_local.items():
                if handle is None:
                    continue
                if worker_exit and (name == "child_job_duplicate" or exact_child_exit):
                    result.worker_local_close[name] = "closed by fake verified WORKER exit"
                elif worker_exit:
                    result.worker_local_close[name] = "WORKER exited; exact CHILD exit unverified"
                else:
                    result.worker_local_close[name] = "unknown; WORKER exit unverified"
                    result.retained_handles["worker_local_" + name] = handle
            if duplicate_attempted and not duplicate_confirmed:
                result.worker_local_close["possible_child_job_duplicate"] = (
                    "unknown value; closed by fake verified WORKER exit" if worker_exit else
                    "unknown value and closure; WORKER exit unverified")
                if not worker_exit:
                    result.uncertain_worker_local.append("possible CHILD job duplicate")
            if child_create_entered and not child_create_returned:
                result.worker_local_close["possible_child_process_thread"] = (
                    "unknown values; WORKER exited, exact CHILD exit unverified" if worker_exit else
                    "unknown values and closure; WORKER exit unverified")
                result.uncertain_worker_local.append("possible CHILD process/thread handles and existence")
            if self.worker is not None and self.worker.report.retained_handles:
                report = self.worker.cleanup()
                result.retained_handles.update(report.retained_handles)
                if report.retained_handles:
                    result.errors.append("CP307 worker cleanup uncertain")
            if self.child_job is not None and not owner_adopted_child_job:
                try:
                    if not self.api.CloseHandle(self.child_job.handle):
                        raise RuntimeError("child job close unconfirmed")
                except BaseException as error:
                    result.errors.append(f"CP303 child job close: {type(error).__name__}: {error}")
                    result.retained_handles["child_job"] = self.child_job.handle
        completed = (not result.errors and not result.retained_handles and
                     not result.uncertain_worker_local and
                     result.cp307_phase == "VERIFIED" and
                     result.cp308_phase_before_cleanup == "fake-receipt-claim-matched" and
                     result.cleanup_phase == "cleanup-confirmed")
        return {"completed": completed, "schema": "cp309-fake-composition-v1",
                "fakeWorkerClaimOnly": True,
                "cp309OwnedOperations": "fake acquisition, preflight, job/offer composition and cleanup; no resume, original or GUI calls",
                "errors": list(result.errors), "retainedHandles": dict(result.retained_handles),
                "uncertainWorkerLocal": list(result.uncertain_worker_local),
                "workerLocalChildHandleOutcome": result.worker_local_child_handle_outcome,
                "workerLocalChildJobDuplicateOutcome": result.worker_local_child_job_duplicate_outcome,
                "occurrenceMappingScope": result.occurrence_mapping_scope}

    def acquire_for_fake_test(self, source, parent, entries, *, name):
        if self.used:
            raise RuntimeError("CP309 composition is one-use")
        def activated(receipt):
            if self._activation is not None:
                raise RuntimeError("CP309 callback already active")
            token = object()
            self._activation, self._active_receipt = token, receipt
            try:
                return self._callback(receipt, token)
            finally:
                self._activation = self._active_receipt = None
        receipt = clone._acquire(source, parent, entries, api=self.clone_api,
                                 name=name, production=False, on_retained=activated)
        handoff = receipt.get("retainedHandoff")
        joined = (self.used and isinstance(handoff, dict) and handoff.get("completed") is True
                  and receipt.get("acquisitionOutcome") == "ACQUIRED_FRESH_CLONE_STATIC_ONLY"
                  and receipt.get("allHandlesClosed") is True
                  and receipt.get("handlesOpened") == receipt.get("handlesClosed")
                  and not receipt.get("errors"))
        self.result.outcome = ("FAKE_COMPOSITION_AND_CP289_CLOSE_COMPLETE" if joined
                               else "INCOMPLETE_OR_UNCERTAIN")
        return receipt, self.result


def run_real(*_args, **_kwargs):
    raise RuntimeError("CP309 real worker/child/original/GUI HARD HOLD")
