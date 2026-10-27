"""Fixed contained CP331 child wiring; public execution remains HARD HOLD.

Import and construction are inert. Only explicit run() loads CP325 exports,
after strict argument and seam validation. This is not trial authorization.
"""
from __future__ import annotations

from dataclasses import dataclass, field
import inspect

import cp321_native_exports as abi
import cp325_real_bindings as binding
import cp330_native_inert_runner as runner
import cp333_watchdog_runner_integration as integration
import cp334_runner_client_pipe as client
import cp339_terminal_protocol as terminal
import cp420_runner_progress as progress

NATIVE_ENTRY = "HARD HOLD"
Hold = abi.Hold
ARGV_TAG = "--cp334-client"


def parse_argv(argv):
    """Accept only the fixed argv tail, never an image, script, or option."""
    if (type(argv) not in (list, tuple) or len(argv) != 4 or
            type(argv[0]) is not str or argv[0] != ARGV_TAG):
        raise Hold("fixed CP335 bootstrap argv required")
    for value in argv[1:]:
        if (type(value) is not str or not 1 <= len(value) <= 20 or
                not value.isascii() or not value.isdecimal() or
                str(int(value)) != value):
            raise Hold("canonical unsigned decimal bootstrap fields required")
    read, write, deadline = map(int, argv[1:])
    if (not 0 < read < 2**64 - 16 or not 0 < write < 2**64 - 16 or
            read == write or not 0 < deadline < 2**64):
        raise Hold("two distinct real handles and absolute deadline required")
    return read, write, deadline


@dataclass
class Receipt:
    phase: str = "new"
    deadline: int | None = None
    terminal_outcome: str = "not-attempted"
    terminal_memory_stage: str = "not-attempted"
    errors: list = field(default_factory=list)


class EntryFailure(Hold):
    def __init__(self, error, owner):
        self.owner, self.receipt = owner, owner.receipt
        super().__init__(str(error))


class FixedRunnerEntry:
    """One occurrence, one deadline, no retry or replay of protocol actions.

    Retain this owner after EntryFailure; close() retries cleanup only. The
    inherited channel has exactly two endpoints and is never a CP328 server.
    """
    def __init__(self, argv):
        self.argv = tuple(argv) if type(argv) in (list, tuple) else argv
        self.receipt = Receipt()
        self.exports = self.pipe_owner = self.handoff = self.runner = None
        self.nonce = None
        self._used = False

    def run(self):
        if self._used:
            raise EntryFailure(Hold("fixed runner entry is one-use"), self)
        self._used = True
        try:
            _, _, deadline = parse_argv(self.argv)
            self.receipt.deadline = deadline
            # Missing hooks must fail before DLL loading or handle adoption.
            parameters = inspect.signature(runner.NativeInertRunner).parameters
            if any(name not in parameters for name in ("deadline", "nonce", "before_resume")):
                raise Hold("CP330 external deadline/nonce/before_resume hooks required")
            self.exports = binding.load_native_exports()
            try:
                self.pipe_owner = client.from_bootstrap_argv(self.exports, self.argv)
            except BaseException as error:
                self.pipe_owner = getattr(error, "owner", None)
                raise
            self.handoff = integration.RunnerHandoff(self.exports,
                pipe_owner=self.pipe_owner, deadline=deadline)
            echoed_deadline, self.nonce = self.handoff.receive_bootstrap()
            if (type(echoed_deadline) is not int or echoed_deadline != deadline or
                    type(self.nonce) is not bytes or len(self.nonce) != 32):
                raise Hold("exact BOOTSTRAP occurrence required")
            try:
                progress.write_progress(self.nonce, deadline, "bootstrap")
            except BaseException:
                pass
            marker_nonce = self.nonce
            original_before_resume = self.handoff.before_resume

            def before_resume(worker, nonce):
                offer = original_before_resume(worker, nonce)
                try:
                    progress.write_progress(marker_nonce, deadline, "parent_ack")
                except BaseException:
                    pass
                return offer

            self.runner = runner.NativeInertRunner(self.exports, deadline=deadline,
                nonce=self.nonce, before_resume=before_resume)
            result = self.runner.run()
            if (result is not self.runner.receipt or result.phase != "closed-success" or
                    result.deadline != deadline or self.handoff.phase != "authorized" or
                    type(self.nonce) is not bytes or len(self.nonce) != 32 or
                    self.handoff.nonce != self.nonce):
                raise Hold("authorized exact runner completion required")
            worker = self.runner.worker
            if worker is None:
                raise Hold("exact WORKER receipt required for terminal claim")
            claim = terminal.from_runner(result, worker.receipt,
                                         deadline=deadline, nonce=self.nonce)
            frame = terminal.encode_terminal(claim)
            if type(frame) is not bytes or len(frame) != terminal.FRAME.size:
                raise Hold("exact terminal claim frame required")
            channel = self.pipe_owner.receipt
            if channel is None:
                raise Hold("adopted client channel required for terminal claim")
            write_handle = channel.client_write
            self.receipt.terminal_outcome = "unknown: terminal write entered"
            self.receipt.terminal_memory_stage = "entered"
            count = self.pipe_owner.transfer(write_handle, "write", frame)
            if (type(count) is not int or count != len(frame) or
                    self.pipe_owner.receipt is not channel or
                    channel.client_write != write_handle or
                    self.nonce != claim.nonce or self.handoff.nonce != claim.nonce or
                    self.handoff.phase != "authorized"):
                raise Hold("exact terminal claim delivery and custody required")
            self.receipt.terminal_memory_stage = "returned"
            self.receipt.terminal_outcome = "80-byte-claim-written-parent-unconfirmed"
            if not self.close():
                raise Hold("fixed runner terminal cleanup unresolved")
            self.receipt.phase = "closed-success"
            return self.receipt
        except BaseException as error:
            self.receipt.errors.append(f"{type(error).__name__}: {error}")
            self.receipt.phase = "hold"
            self.close()
            # Failure-only evidence is written after cleanup. Its absence is
            # inconclusive when the outer watchdog ends the runner first.
            try:
                if self.runner is not None and type(self.nonce) is bytes and len(self.nonce) == 32:
                    rr = self.runner.receipt
                    wr = getattr(getattr(self.runner, "worker", None), "receipt", None)
                    snapshot = {
                        "runner": {
                            "ack_memory_stage": getattr(rr, "ack_memory_stage", None),
                            "ack_remaining": getattr(rr, "ack_remaining", None),
                            "post_ack_budget": getattr(rr, "post_ack_budget", None),
                            "creator_completion": getattr(rr, "creator_completion", None),
                            "terminal_memory_stage": self.receipt.terminal_memory_stage,
                        },
                        "worker": {
                            "wait_outcome": getattr(wr, "completion_wait_outcome", None),
                            "wait_remaining": getattr(wr, "completion_wait_remaining", None),
                            "wait_status": getattr(wr, "completion_wait_status", None),
                            "post_wait_budget": getattr(wr, "completion_post_wait_budget", None),
                            "post_wait_remaining": getattr(wr, "completion_post_wait_remaining", None),
                        },
                    }
                    progress.write_failure_completion(self.nonce, self.receipt.deadline, snapshot)
            except BaseException:
                pass
            raise EntryFailure(error, self) from error

    def close(self):
        """One nonblocking cleanup pass; never replay BOOTSTRAP, OFFER or ACK."""
        ready = True
        if self.runner is not None:
            try:
                self.runner.close()
                worker = self.runner.worker
                ready = (self.runner.receipt.phase == "closed-success" or
                         (worker is not None and worker.receipt.phase == "closed"))
            except BaseException as error:
                self.receipt.errors.append(f"cleanup: {type(error).__name__}: {error}")
                ready = False
        if self.pipe_owner is not None:
            try:
                ready = self.pipe_owner.cleanup() is True and ready
            except BaseException as error:
                self.receipt.errors.append(f"cleanup: {type(error).__name__}: {error}")
                ready = False
        return ready


def launch_trial(*_args, **_kwargs):
    raise Hold("CP335 public fixed entry HARD HOLD pending hooks and independent review")


if __name__ == "__main__":
    launch_trial()
