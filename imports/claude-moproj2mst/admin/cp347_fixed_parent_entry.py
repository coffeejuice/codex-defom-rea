"""Fixed watchdog-process entry and terminal parent claim; live entry HARD HOLD.

The parent claim is emitted only after CP333 independently accepted the inert
WORKER transcript, both exact exits, empty jobs and terminal cleanup. It is a
private-channel claim, not top-controller proof. Import and construction are
inert; this module never selects an executable, script or project path.
"""
from __future__ import annotations

from dataclasses import dataclass, field
import hashlib
import hmac
import struct
import weakref

import cp321_native_exports as abi
import cp325_real_bindings as binding
import cp333_watchdog_runner_integration as integration
import cp334_runner_client_pipe as client

NATIVE_ENTRY = "HARD HOLD"
Hold = abi.Hold
MAGIC = 0x37343343  # CP347; distinct from CP339 runner terminal.
VERSION = 1
FLAGS = 0x0000000F  # terminal, WORKER, RUNNER, parent cleanup observed.
FRAME = struct.Struct("<IIQII32s24s")
_BODY = struct.Struct("<IIQII32s")
_USED_ENTRIES = weakref.WeakSet()


class ParentClaimError(ValueError):
    pass


def parse_client_argv(argv):
    """Validate the fixed inherited two-client tail before loading DLLs."""
    if (type(argv) not in (tuple, list) or len(argv) != 4 or
            type(argv[0]) is not str or argv[0] != client.ARGV_TAG or
            any(type(s) is not str or not s.isascii() or not s.isdecimal() or
                not 1 <= len(s) <= 20 or str(int(s)) != s for s in argv[1:])):
        raise Hold("fixed CP347 inherited client argv required")
    read, write, deadline = map(int, argv[1:])
    if (not 0 < read < client.INVALID - 15 or
            not 0 < write < client.INVALID - 15 or read == write or
            not 0 < deadline < 2**64):
        raise Hold("two distinct client handles and channel deadline required")
    return read, write, deadline


class _ClientSink:
    """CP334 overlapped terminal transfer; flush has no Python buffering."""

    def __init__(self, owner):
        self.owner = owner
        self.exports = owner.exports
        self.receipt = owner.receipt
        if self.receipt is None:
            raise Hold("adopted CP334 client receipt required")
        self.read, self.write_handle = self.receipt.client_read, self.receipt.client_write
        self.deadline = owner.deadline
        self._handles, self._operations = owner.handles, owner.operations
        self._custody()

    def _custody(self):
        if (self.owner.exports is not self.exports or
                type(self.owner.deadline) is not int or
                self.owner.deadline != self.deadline or
                self.owner.receipt is not self.receipt or
                type(self.receipt.client_read) is not int or
                type(self.receipt.client_write) is not int or
                (self.receipt.client_read, self.receipt.client_write) !=
                    (self.read, self.write_handle) or
                self.owner.handles is not self._handles or
                self.owner.operations is not self._operations or
                type(self.owner.handles) is not set or
                self.owner.handles != {self.read, self.write_handle} or
                type(self.owner.operations) is not list or
                self.owner.operations != [] or
                self.owner.started is not True or self.owner.held is not False):
            raise Hold("exact terminal client channel custody required")

    def write(self, frame):
        self._custody()
        count = self.owner.transfer(self.write_handle, "write", frame)
        self._custody()
        return count

    def flush(self):
        # CP334.transfer returns only after terminal OVERLAPPED completion.
        self._custody()


def _deadline(value):
    if type(value) is not int or not 0 < value < 2**64:
        raise ParentClaimError("positive uint64 occurrence deadline required")
    return value


def _nonce(value):
    if type(value) is not bytes or len(value) != 32 or not any(value):
        raise ParentClaimError("nonzero exact 32-byte occurrence nonce required")
    return value


@dataclass(frozen=True)
class ParentSuccess:
    deadline: int
    nonce: bytes

    def __post_init__(self):
        _deadline(self.deadline)
        _nonce(self.nonce)


def encode_parent(value):
    if type(value) is not ParentSuccess:
        raise ParentClaimError("ParentSuccess required")
    body = _BODY.pack(MAGIC, VERSION, value.deadline, FLAGS, 0, value.nonce)
    return body + hashlib.sha256(body).digest()[:24]


def decode_parent(raw, *, deadline=None, nonce=None):
    """Validate one exact claim; top-controller process/job checks remain separate."""
    if type(raw) is not bytes or len(raw) != FRAME.size:
        raise ParentClaimError("exact 80-byte parent claim required")
    magic, version, got_deadline, flags, reserved, got_nonce, digest = FRAME.unpack(raw)
    if (magic, version, flags, reserved) != (MAGIC, VERSION, FLAGS, 0):
        raise ParentClaimError("parent claim version, flags or reserved invalid")
    claim = ParentSuccess(got_deadline, got_nonce)
    if (deadline is not None and claim.deadline != _deadline(deadline)) or (
            nonce is not None and claim.nonce != _nonce(nonce)):
        raise ParentClaimError("parent occurrence mismatch")
    if not hmac.compare_digest(digest, hashlib.sha256(raw[:_BODY.size]).digest()[:24]):
        raise ParentClaimError("parent claim checksum mismatch")
    return claim


def from_integration(owner):
    """Accept only CP333's completed exact-process and cleanup observations."""
    if owner is None:
        raise ParentClaimError("CP333 owner required")
    receipt = getattr(owner, "receipt", None)
    frozen = getattr(owner, "_completion_owners", None)
    claim = ParentSuccess(getattr(receipt, "deadline", None),
                          getattr(owner, "nonce", None))
    if (type(frozen) is not tuple or len(frozen) != 9 or
            frozen[0] is not getattr(owner, "exports", None) or
            frozen[5] is not receipt or type(frozen[6]) is not bytes or
            frozen[6] != claim.nonce or
            type(frozen[7]) is not int or frozen[7] != claim.deadline or
            getattr(owner, "_accepted_success", None) is not True or
            getattr(owner, "_verified_success", None) is not True or
            getattr(receipt, "phase", None) != "closed-success" or
            getattr(receipt, "terminal_outcome", None) !=
                "80-byte-occurrence-validated-zero-queued-snapshot" or
            getattr(receipt, "worker_outcome", None) !=
                "exact-worker-observed-and-duplicates-released" or
            getattr(receipt, "runner_outcome", None) !=
                "exact-runner-exit-zero-job-empty-observed" or
            getattr(receipt, "cleanup_outcome", None) !=
                "terminal-parent-owners-released" or
            getattr(receipt, "errors", None) != []):
        raise ParentClaimError("complete CP333 accepted success required")
    return claim


@dataclass
class Receipt:
    phase: str = "new"
    claim_outcome: str = "not-attempted"
    client_cleanup_outcome: str = "not-attempted"
    errors: list = field(default_factory=list)


class ParentEntryFailure(Hold):
    def __init__(self, error, owner):
        self.owner, self.receipt = owner, owner.receipt
        super().__init__(str(error))


class FixedParentEntry:
    """One fixed occurrence; retain owner after any uncertain write or cleanup."""

    def __init__(self, sink=None, *, client_argv=None, stage=None):
        if (sink is None) == (client_argv is None):
            raise TypeError("one binary sink or fixed inherited client argv required")
        if client_argv is not None:
            self._client_fields = parse_client_argv(client_argv)
            self._client_argv = tuple(client_argv)
        else:
            if not callable(getattr(sink, "write", None)) or not callable(
                    getattr(sink, "flush", None)):
                raise TypeError("binary write/flush sink required")
            self._client_fields = self._client_argv = None
        self.sink = self._sink = sink
        self.receipt = Receipt()
        self.exports = self.integration = None
        self._owner = None
        self.client_owner = None
        self._client_owner = None
        self._used = False
        self._claim = None
        self._stage = stage if callable(stage) else None
        self._stage_error = False

    def _mark(self, name):
        """Emit fixed diagnostics; callback faults fail closed before a claim."""
        if self._stage is not None:
            callback = self._stage
            prior_error = self._stage_error
            protected = ("receipt", "_owner", "integration", "exports", "sink",
                         "_sink", "client_owner", "_client_owner", "_used", "_claim",
                         "_stage", "_client_argv", "_client_fields")
            before = {key: getattr(self, key) for key in protected}
            receipt_state = dict(vars(self.receipt))
            if isinstance(receipt_state.get("errors"), list):
                receipt_state["errors"] = list(receipt_state["errors"])
            try:
                callback(name)
            except BaseException:
                self._stage_error = True
            if any(getattr(self, key) is not value for key, value in before.items()):
                self._stage_error = True
                for key, value in before.items():
                    setattr(self, key, value)
            if self.receipt is before["receipt"] and vars(self.receipt) != receipt_state:
                self._stage_error = True
                vars(self.receipt).clear()
                vars(self.receipt).update(receipt_state)
            self._stage_error = bool(prior_error or self._stage_error)

    def _adoption_mark(self, name):
        """Keep the already-qualified CP334 path compact in live traces."""
        if name.startswith("cp334_h") and not (
                "_pipeflags_" in name or name.endswith("_flags_validated")):
            return
        if name in ("cp334_before_final_tick",):
            return
        self._mark(name)

    @classmethod
    def from_client_argv(cls, argv, *, stage=None):
        """Construct inert fixed child entry; no DLL, pipe or process call."""
        return cls(client_argv=argv, stage=stage)

    def run(self):
        if self._used or self in _USED_ENTRIES:
            raise ParentEntryFailure(Hold("fixed parent entry is one-use"), self)
        _USED_ENTRIES.add(self)  # callback mutation of _used cannot authorize replay
        self._used = True
        local_receipt, owner, exports = self.receipt, None, None
        sink = self._sink
        self._mark("entry")
        try:
            if self.sink is not sink:
                raise Hold("original binary claim sink required")
            self._mark("before_load_native_exports")
            self.exports = binding.load_native_exports()
            self._mark("after_load_native_exports")
            exports = self.exports
            if self._client_argv is not None:
                try:
                    self._mark("before_cp334_from_bootstrap_argv")
                    self.client_owner = client.from_bootstrap_argv(
                        exports, self._client_argv, stage=self._adoption_mark)
                    self._mark("after_cp334_adopt")
                except BaseException as error:
                    self.client_owner = getattr(error, "owner", None)
                    self._client_owner = self.client_owner
                    raise
                self._client_owner = self.client_owner
                sink = self.sink = self._sink = _ClientSink(self.client_owner)
            self.integration = integration.WatchdogIntegration(
                self.exports, stage=self._mark)
            owner = self._owner = self.integration
            self._mark("before_cp333_acknowledge")
            self.integration.acknowledge()
            self._mark("after_cp333_acknowledge")
            self._mark("before_cp333_complete")
            result = self.integration.complete()
            self._mark("after_cp333_complete")
            if self._stage_error:
                raise Hold("diagnostic stage callback failed")
            if result is not owner.receipt:
                raise Hold("exact CP333 success receipt required")
            claim = from_integration(owner)
            encoded = self._claim = encode_parent(claim)
            self.receipt.claim_outcome = "unknown: 80-byte write entered"
            count = sink.write(encoded)
            if type(count) is not int or count != FRAME.size:
                raise Hold("exact 80-byte parent claim write required")
            if self.sink is not sink or self._sink is not sink:
                raise Hold("original binary claim sink changed during write")
            self.receipt.claim_outcome = "unknown: flush entered"
            sink.flush()
            if (self.receipt is not local_receipt or self._owner is not owner or
                    self.sink is not sink or self._sink is not sink or
                    (isinstance(sink, _ClientSink) and
                     (self.client_owner is not sink.owner or
                      self._client_owner is not sink.owner)) or
                    self._claim is not encoded or self._used is not True or
                    self.integration is not owner or self.exports is not exports or
                    owner.receipt is not result or from_integration(owner) != claim):
                raise Hold("parent claim or accepted occurrence changed during write")
            self.receipt.claim_outcome = "80-byte-claim-written-and-flushed"
            if self._client_owner is not None:
                self.receipt.client_cleanup_outcome = "unknown: cleanup entered"
                if self._client_owner.cleanup() is not True:
                    raise Hold("terminal inherited client cleanup unresolved")
                if (not isinstance(sink, _ClientSink) or
                        self.client_owner is not sink.owner or
                        self._client_owner is not sink.owner or
                        sink.owner.exports is not exports or
                        type(sink.owner.deadline) is not int or
                        sink.owner.deadline != sink.deadline or
                        sink.owner.handles is not sink._handles or
                        sink.owner.operations is not sink._operations or
                        sink.owner.handles != set() or
                        sink.owner.operations != [] or
                        sink.owner.receipt is not None or
                        sink.owner.started is not True or
                        sink.owner.held is not True):
                    raise Hold("terminal client ownership not released")
                self.receipt.client_cleanup_outcome = "terminal-client-owners-released"
            if (self.receipt is not local_receipt or self._owner is not owner or
                    self.sink is not sink or self._sink is not sink or
                    self._claim is not encoded or self._used is not True or
                    self.integration is not owner or self.exports is not exports or
                    owner.receipt is not result or from_integration(owner) != claim):
                raise Hold("accepted parent occurrence changed after client cleanup")
            self.receipt.phase = "closed-local-success-controller-unverified"
            return self.receipt
        except BaseException as error:
            # A sink callback is an arbitrary Python boundary in injected tests.
            # Restore the exact retained owner and original diagnostics before
            # any cleanup attempt; never follow a replaced public owner.
            self.receipt = local_receipt
            self.sink = self._sink = sink
            if isinstance(sink, _ClientSink):
                self.client_owner = self._client_owner = sink.owner
            self._used = True
            if owner is not None:
                self.integration = self._owner = owner
            if exports is not None:
                self.exports = exports
            self.receipt.errors.append(f"{type(error).__name__}: {error}")
            self.receipt.phase = "hold"
            self.close()
            raise ParentEntryFailure(error, self) from error

    def close(self):
        """Retry owner cleanup only; never replay a claim or CP333 transcript."""
        ready = True
        try:
            if self._owner is not None:
                ready = self._owner.close() is True
        except BaseException as error:
            self.receipt.errors.append(f"cleanup: {type(error).__name__}: {error}")
            ready = False
        try:
            if self._client_owner is not None:
                ready = self._client_owner.cleanup() is True and ready
        except BaseException as error:
            self.receipt.errors.append(f"client cleanup: {type(error).__name__}: {error}")
            ready = False
        return ready


def launch_trial(*_args, **_kwargs):
    raise Hold("CP347 parent live launch HARD HOLD pending top controller review")


if __name__ == "__main__":
    launch_trial()
