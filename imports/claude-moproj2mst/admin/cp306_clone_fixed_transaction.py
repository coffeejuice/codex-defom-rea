"""CP306 retained-clone to CP305 transaction adapter, for in-memory fakes only.

CP289 keeps the creation-bound clone and copied-file handles alive while this
callback runs. CP305 creates the *only* child; CP300's OwnedProcess handoff is
deliberately not called. Neither static clone evidence nor a matching ACK claim
proves loaded-image bytes or a real worker's receipt.
"""
from __future__ import annotations

import hashlib
import json
from pathlib import Path, PureWindowsPath
import re

import color_atomic_clone_acquire as clone
import cp303_job_creation as creation
import cp305_integrated_synthetic as transaction


CP302_SHA256 = "66eadac3f5dd6ee0d0b856a53760dc9270ecfd170808c62dff01b99a583f6a28"
CP302_SOURCE_SHA256 = "31afbe79ea842eb663be7d278ea389dddd70d5c628106371b5ef3e6a55b703b1"
CP302_BUILD_RECEIPT_SHA256 = "53a71fc4383b293c45c8751ead0fe67cb2686372af3a3acdf86e68f697cad4d7"
HERE = Path(__file__).resolve().parent
FIXTURE = HERE / "fixtures/cp302/fixture.exe"
SOURCE = HERE / "fixtures/cp302/exit_only.c"
BUILD_RECEIPT = HERE.parent / "analysis/cp302-fixture-build.json"


def _pinned_fixture():
    """Check the reviewed local CP302 artifact, source and build receipt."""
    if creation.CP302_SHA256 != CP302_SHA256:
        raise ValueError("CP303 CP302 digest pin changed")
    if hashlib.sha256(FIXTURE.read_bytes()).hexdigest() != CP302_SHA256:
        raise ValueError("reviewed CP302 fixture bytes changed")
    if hashlib.sha256(SOURCE.read_bytes()).hexdigest() != CP302_SOURCE_SHA256:
        raise ValueError("reviewed CP302 source bytes changed")
    receipt_bytes = BUILD_RECEIPT.read_bytes()
    if hashlib.sha256(receipt_bytes).hexdigest() != CP302_BUILD_RECEIPT_SHA256:
        raise ValueError("reviewed CP302 build receipt bytes changed")
    receipt = json.loads(receipt_bytes)
    if (receipt.get("source_sha256") != CP302_SOURCE_SHA256 or
            receipt.get("pe", {}).get("sha256") != CP302_SHA256 or
            receipt.get("pe", {}).get("bytes") != FIXTURE.stat().st_size or
            receipt.get("pe", {}).get("machine") != "AMD64" or
            receipt.get("pe", {}).get("subsystem") != "CONSOLE" or
            receipt.get("pe", {}).get("imports") != ["KERNEL32.DLL"]):
        raise ValueError("reviewed CP302 build receipt mismatch")


def _retained_image(receipt):
    """Bind CP289's retained destination identity to CP305's exact image field."""
    if (not isinstance(receipt, dict) or receipt.get("schema") != "cp289-creation-bound-clone-v1" or
            receipt.get("production") is not False or
            receipt.get("sourceStableAcrossAcquisition") is not True or
            receipt.get("manifestsMatchPinnedEntries") is not True):
        raise ValueError("CP289 retained fake clone evidence incomplete")
    root = PureWindowsPath(receipt["cloneRoot"])
    if (root.parent != PureWindowsPath(r"C:\PROBLEM") or
            re.fullmatch(r"m289_[0-9a-f]{16}", root.name) is None or
            len(str(root / "fixture.exe")) >= 100):
        raise ValueError("fresh CP289 clone path required")
    if clone._identity(receipt["createdCloneIdentity"]) != clone._identity(receipt["cloneFinalIdentity"]):
        raise ValueError("creation-bound clone identity changed")
    if receipt["cloneFinalIdentity"].get("finalPath") != "\\\\?\\" + str(root):
        raise ValueError("retained clone final path mismatch")
    command = receipt["commandGate"]
    contract = receipt["launchContract"]
    if (command.get("exactUtf16ArgvCwd") is not True or
            contract.get("cwd") != str(root)):
        raise ValueError("CP289 fake command/cwd gate incomplete")
    copied = [item for item in receipt["copiedFiles"]
              if item.get("path", "").casefold() == "fixture.exe"]
    image = str(root / "fixture.exe")
    if (len(copied) != 1 or copied[0].get("sha256") != CP302_SHA256 or
            copied[0].get("bytes") != FIXTURE.stat().st_size or
            copied[0].get("destinationIdentity", {}).get("finalPath") != "\\\\?\\" + image or
            not copied[0].get("sourceIdentity", {}).get("fileIndex") or
            not copied[0].get("destinationIdentity", {}).get("fileIndex")):
        raise ValueError("retained CP302 copy/handle identity mismatch")
    return creation.FixedCloneImage(image, str(root), CP302_SHA256), copied[0]


class FakeCloneTransactionAdapter:
    """One fake acquisition, one CP305 child. Keep the transaction for retryable cleanup."""

    def __init__(self, *, clone_api, transaction_api, worker_factory, send_ack,
                 ack_receipt_factory, occurrence, wait_ms=2000):
        if (isinstance(clone_api, clone.WindowsCloneFiles) or
                getattr(clone_api, "cp306_fake_clone_api", False) is not True):
            raise RuntimeError("CP306 real clone acquisition HOLD")
        creation._fake_only(transaction_api)
        if getattr(transaction_api, "cp304_fake_api", False) is not True:
            raise RuntimeError("CP306 real supervisor HOLD")
        if not isinstance(occurrence, str) or re.fullmatch(r"cp305-[0-9a-f]{16}", occurrence) is None:
            raise ValueError("fixed CP305 occurrence required")
        if type(wait_ms) is not int or not 0 <= wait_ms <= 5000:
            raise ValueError("bounded wait required")
        self.clone_api = clone_api
        self.transaction_api = transaction_api
        self.worker_factory = worker_factory
        self.send_ack = send_ack
        self.ack_receipt_factory = ack_receipt_factory
        self.occurrence = occurrence
        self.wait_ms = wait_ms
        self.transaction = None
        self.used = False
        self._retained_token = None

    def on_retained(self, receipt):
        raise RuntimeError("CP306 retained callback requires CP289 acquisition activation")

    def _on_retained(self, receipt, token):
        if token is None or token is not self._retained_token:
            raise RuntimeError("CP306 retained callback activation missing")
        if self.used:
            raise RuntimeError("CP306 adapter is one-use")
        self.used = True
        result = {"schema": "cp306-fake-retained-transaction-v1", "completed": False,
                  "completionScope": "retained callback only; CP289 handle-close result follows",
                  "qualification": "fake static clone and exact fake child only; loaded bytes and ACK receipt unverified",
                  "operationScope": "CP306-owned calls only; supplied fake callbacks may have arbitrary side effects",
                  "originalRun": False, "resumed": False, "errors": []}
        try:
            _pinned_fixture()
            image, copied = _retained_image(receipt)
            result.update(cloneIdentity=receipt["cloneFinalIdentity"],
                          clonedImage=image.executable,
                          clonedImageDestinationIdentity=copied["destinationIdentity"],
                          clonedImageSourceIdentity=copied["sourceIdentity"],
                          fixtureSha256=image.sha256)
            tx = transaction.FakeTransaction(
                self.transaction_api, transaction.FixedTransaction(image, self.occurrence))
            self.transaction = tx
            # CP305 may close these during finally, including a partial PI on
            # CreateProcessW failure. Record the original returned values first.
            take_process_information = tx._take_process_information

            def retain_returned_handles(pi):
                take_process_information(pi)
                result["workerLocalReturnedHandles"] = {
                    "process": tx.worker_process_handle,
                    "thread": tx.worker_thread_handle,
                }

            tx._take_process_information = retain_returned_handles
            try:
                tx.run(worker_factory=self.worker_factory, send_ack=self.send_ack,
                       ack_receipt_factory=self.ack_receipt_factory, wait_ms=self.wait_ms)
            finally:
                result["transactionReport"] = tx.report
                offer = tx.supervisor.offer
                ack = tx.supervisor.ack
                result["childOffer"] = vars(offer).copy() if offer is not None else None
                result["childAck"] = vars(ack).copy() if ack is not None else None
                result["workerLocalProcessHandle"] = (offer.worker_handle if offer else
                                                        tx.worker_process_handle)
            cleanup = tx.report.get("cleanup")
            result["completed"] = (
                tx.report["status"] == "acknowledged-unverified-receipt" and
                cleanup is not None and cleanup.active_processes_zero and
                not cleanup.retained_handles and not cleanup.errors and
                not tx.report["retained_local_handles"] and
                tx.supervisor.create_outcome == "verified exact offered child" and
                tx.supervisor.ack_receipt == "matching-unverified-claim")
            if not result["completed"]:
                result["errors"].append("transaction or cleanup certainty incomplete")
        except BaseException as error:
            result["errors"].append(f"{type(error).__name__}: {error}")
        return result

    def acquire_for_fake_test(self, source, parent, entries, *, name):
        if self.used:
            raise RuntimeError("CP306 adapter is one-use")
        _pinned_fixture()
        if (str(PureWindowsPath(str(source))).casefold() != r"c:\problem\s302" or
                str(PureWindowsPath(str(parent))).casefold() != r"c:\problem" or
                len([e for e in entries if e.get("kind") == "file" and
                     e.get("path", "").casefold() == "fixture.exe" and
                     e.get("sha256") == CP302_SHA256 and
                     int(e.get("bytes", -1)) == FIXTURE.stat().st_size]) != 1):
            raise ValueError("exact pinned CP302 source manifest entry required")

        def activated_callback(receipt):
            if self.used or self._retained_token is not None:
                raise RuntimeError("CP306 retained callback already used")
            token = object()
            self._retained_token = token
            try:
                return self._on_retained(receipt, token)
            finally:
                self._retained_token = None

        receipt = clone._acquire(source, parent, entries, api=self.clone_api, name=name,
                                 production=False, on_retained=activated_callback)
        handoff = receipt.get("retainedHandoff")
        joined = (receipt.get("acquisitionOutcome") == "ACQUIRED_FRESH_CLONE_STATIC_ONLY" and
                  receipt.get("allHandlesClosed") is True and
                  isinstance(handoff, dict) and handoff.get("completed") is True and
                  not receipt.get("errors"))
        receipt["cp306JoinedOutcome"] = (
            "FAKE_JOIN_COMPLETE_STATIC_ONLY" if joined else "INCOMPLETE_OR_UNCERTAIN")
        return receipt


def acquire_real(*_args, **_kwargs):
    raise RuntimeError("CP306 real clone and child entry HOLD")
