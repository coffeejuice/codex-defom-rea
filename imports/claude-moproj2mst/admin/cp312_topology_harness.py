"""CP312 fake supervisor rehearsal for the proposed nested-job topology.

No object in this module opens a process or job.  The injected backend and
channel are trusted in-memory fakes; their marker is not a security boundary.
This uses CP311 framed JSON, a dynamic m289 clone path, one CHILD job duplicate,
and an OFFER/ACK/RECEIPT. The prepared worker.c instead uses packed
CP312_INIT/RESULT/ACK, fixed C:\\PROBLEM\\s302\\fixture.exe, both WORKER and
CHILD job duplicates, and no RECEIPT. The two protocols are not wired together.
The live entry remains held until a reviewed exact bridge and cleanup are ready.
"""
from __future__ import annotations

from dataclasses import dataclass, field
import ctypes as C
import hashlib
from pathlib import Path, PureWindowsPath
import re
import secrets

import cp303_job_creation as abi
import cp310_native_job_adapter as observed
import cp311_frame_protocol as framing


HERE = Path(__file__).resolve().parent
FIXTURE = HERE / "fixtures" / "cp302" / "fixture.exe"
WORKER_IMAGE = r"C:\PROBLEM\cp312_topology_worker.exe"
WORKER_CWD = r"C:\PROBLEM"
PREPARED_WORKER_IMAGE = r"C:\PROBLEM\s312\worker.exe"
PREPARED_CHILD_IMAGE = r"C:\PROBLEM\s302\fixture.exe"
CP302_SHA256 = abi.CP302_SHA256
CREATE_FLAGS = (abi.CREATE_SUSPENDED | abi.EXTENDED_STARTUPINFO_PRESENT |
                abi.CREATE_NO_WINDOW)
EXIT_CODE = 0xE312
WAIT_MS = 2000
HARD_HOLD_DEPENDENCIES = (
    "reviewed x64 C:\\PROBLEM\\s312\\worker.exe PE receipt and pinned digest",
    "bridge CP311 JSON/m289/CHILD-only/RECEIPT fake to worker.c packed CP312_INIT/RESULT/ACK, fixed s302, both job handles, no RECEIPT",
    "private bounded stdin/stdout pipe and worker identity binding with explicit inherited pipe HANDLE_LIST",
    "reviewed native creation adapter with one JOB_LIST attribute and no breakaway",
    "ancestor job-chain silent-breakaway assessment before worker resume",
    "interrupted CreateProcessW recovery with independent child-existence evidence",
)


class TopologyHold(RuntimeError):
    pass


class CreationInterrupted(RuntimeError):
    """Fake CreateProcessW interruption with any exposed partial local handles."""

    def __init__(self, *, process: observed.LocalHandle | None = None,
                 thread: observed.LocalHandle | None = None):
        if process is not None and not isinstance(process, observed.LocalHandle):
            raise TypeError("partial process handle must be supervisor-local")
        if thread is not None and not isinstance(thread, observed.LocalHandle):
            raise TypeError("partial thread handle must be supervisor-local")
        self.process, self.thread = process, thread
        super().__init__("CreateProcessW outcome unknown; partial handles retained")


class DuplicationInterrupted(RuntimeError):
    """Fake DuplicateHandle interruption with an exposed local output handle."""

    def __init__(self, handle: observed.LocalHandle | None = None):
        if handle is not None and not isinstance(handle, observed.LocalHandle):
            raise TypeError("partial duplicate must be supervisor-local")
        self.handle = handle
        super().__init__("DuplicateHandle outcome unknown")


@dataclass(frozen=True)
class CreationRequest:
    image: str
    cwd: str
    command_line: str
    job: observed.LocalHandle | observed.RemoteHandle
    flags: int = CREATE_FLAGS
    inherit_handles: bool = False
    attribute_count: int = 1

    def __post_init__(self):
        if (self.flags != CREATE_FLAGS or self.inherit_handles is not False or
                self.attribute_count != 1 or not self.command_line.startswith('"') or
                not self.command_line.endswith('"') or
                self.command_line[1:-1] != self.image):
            raise ValueError("suspended one-JOB_LIST noninheriting creation required")


@dataclass(frozen=True)
class CreatedProcess:
    process: observed.LocalHandle
    thread: observed.LocalHandle
    pid: int
    tid: int

    def __post_init__(self):
        if (not isinstance(self.process, observed.LocalHandle) or
                not isinstance(self.thread, observed.LocalHandle) or
                type(self.pid) is not int or self.pid <= 0 or
                type(self.tid) is not int or self.tid <= 0):
            raise ValueError("complete creation-returned process information required")


@dataclass(frozen=True)
class Identity:
    pid: int
    creation_filetime: int
    image: str


@dataclass
class Report:
    phase: str = "new"
    scope: str = ("FAKE_ONLY CP311 JSON/m289 rehearsal; not worker.c packed "
                  "CP312_INIT/RESULT/ACK or fixed s302 native protocol")
    create_outcome: dict[str, str] = field(default_factory=lambda: {
        "worker": "not-attempted", "child": "not-attempted"})
    membership: dict[str, bool | None] = field(default_factory=dict)
    termination: dict[str, str] = field(default_factory=dict)
    waits: dict[str, str] = field(default_factory=dict)
    active_counts: dict[str, int | None] = field(default_factory=dict)
    closes: dict[str, str] = field(default_factory=dict)
    retained: dict[str, int] = field(default_factory=dict)
    remote_child_job: int | None = None
    errors: list[str] = field(default_factory=list)


def fixed_child_image(image: str, digest: str) -> str:
    """Validate the fixed disposable clone contract and local CP302 source pin."""
    if digest != CP302_SHA256 or not isinstance(image, str):
        raise ValueError("CP302 fixture digest required")
    path = PureWindowsPath(image)
    if (not path.is_absolute() or path.name.casefold() != "fixture.exe" or
            path.parent.parent != PureWindowsPath(r"C:\PROBLEM") or
            re.fullmatch(r"m289_[0-9a-f]{16}", path.parent.name) is None or
            len(str(path)) >= 100):
        raise ValueError("fixed CP302 clone under C:\\PROBLEM required")
    if hashlib.sha256(FIXTURE.read_bytes()).hexdigest() != CP302_SHA256:
        raise ValueError("local CP302 fixture pin mismatch")
    return str(path)


def _one_frame(data: bytes) -> dict:
    frames = framing.decode_frames(data)
    if len(frames) != 1:
        raise TopologyHold("one bounded frame required")
    return frames[0]


def fake_worker_child_request(data: bytes, *, worker_process: observed.LocalHandle,
                              expected_nonce: str) -> CreationRequest:
    """Decode the bounded COMMAND as a worker-side fake creation intent.

    This proves only that the request names CHILD in JOB_LIST with the required
    flags. It does not prove that a native worker used the request or remained
    suspended. A real worker and authenticated channel remain separate work.
    """
    if not isinstance(worker_process, observed.LocalHandle):
        raise TypeError("creation-returned worker handle required")
    obj = _one_frame(data)
    if (set(obj) != {"kind", "nonce", "child_job", "image", "sha256",
                     "flags", "inherit_handles", "attribute_count"} or
            obj["kind"] != "COMMAND" or obj["nonce"] != expected_nonce or
            type(obj["flags"]) is not int or obj["flags"] != CREATE_FLAGS or
            obj["inherit_handles"] is not False or
            type(obj["attribute_count"]) is not int or obj["attribute_count"] != 1):
        raise TopologyHold("invalid worker child creation command")
    image = fixed_child_image(obj["image"], obj["sha256"])
    value = framing.handle_value(obj["child_job"], 64)
    remote = observed.RemoteHandle(value, worker_process.value)
    return CreationRequest(image, str(PureWindowsPath(image).parent),
                           f'"{image}"', remote)


class FakeTopologyHarness:
    """One-use supervisor sequence; backend/channel must be controlled fakes.

    The worker side of the fake channel creates the child after COMMAND. It
    receives a worker-local, noninheritable CHILD job duplicate. A RemoteHandle
    is never queried or closed in the supervisor's local handle table.
    """

    def __init__(self, backend, channel, *, child_image: str,
                 fixture_sha256: str = CP302_SHA256, nonce: str | None = None):
        if (getattr(backend, "cp312_fake_backend", False) is not True or
                getattr(channel, "cp312_fake_channel", False) is not True or
                isinstance(backend, (abi.Kernel32Api, observed.NativeJobApi))):
            raise TopologyHold("CP312 native process/job entry HARD HOLD")
        if C.sizeof(C.c_void_p) != 8:
            raise TopologyHold("CP312 requires native x64 ABI")
        self.image = fixed_child_image(child_image, fixture_sha256)
        self.backend, self.channel = backend, channel
        self.nonce = nonce if nonce is not None else secrets.token_hex(32)
        if not re.fullmatch(r"[0-9a-f]{64}", self.nonce):
            raise ValueError("256-bit lowercase occurrence nonce required")
        self.report = Report()
        self.local: dict[str, observed.LocalHandle] = {}
        self.worker: CreatedProcess | None = None
        self.child: observed.LocalHandle | None = None
        self.remote_job: observed.RemoteHandle | None = None
        self.command_attempted = False
        self._used = False

    def _retain(self):
        self.report.retained = {name: handle.value for name, handle in self.local.items()}

    def _hold(self, reason: str):
        self.report.phase = "hold"
        self.report.errors.append(reason)
        self._retain()
        raise TopologyHold(reason)

    def _job(self, name: str):
        handle = self.backend.create_job()
        if not isinstance(handle, observed.LocalHandle):
            self._hold(f"{name} creation returned no local handle")
        if handle in self.local.values():
            self._hold(f"{name} handle aliases another owned handle")
        self.local[name] = handle
        self._retain()
        self.backend.set_kill_on_close(handle)
        if self.backend.handle_flags(handle) & abi.HANDLE_FLAG_INHERIT:
            self._hold(f"{name} handle is inheritable")
        flags = self.backend.job_limit_flags(handle)
        if flags != abi.JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE:
            self._hold(f"{name} has unexpected job limits {flags:#x}")

    def stage(self):
        if self._used or self.report.phase != "new":
            self._hold("CP312 harness is one-use")
        self._used = True
        self.report.phase = "staging"
        self._job("worker_job")
        self._job("child_job")
        self.report.phase = "jobs-staged"

    def create_worker(self):
        if self.report.phase != "jobs-staged":
            self._hold("jobs must precede worker creation")
        request = CreationRequest(WORKER_IMAGE, WORKER_CWD,
                                  f'"{WORKER_IMAGE}"', self.local["worker_job"])
        self.report.create_outcome["worker"] = "unknown: inside CreateProcessW"
        self.report.phase = "creating-worker"
        try:
            created = self.backend.create_suspended_worker(request)
        except BaseException as error:
            if isinstance(error, CreationInterrupted):
                for name, handle in (("worker_process", error.process),
                                     ("worker_thread", error.thread)):
                    if handle is not None:
                        if handle in self.local.values():
                            self._hold(f"partial {name} aliases an owned handle")
                        self.local[name] = handle
                self._retain()
            self._hold("worker CreateProcessW interrupted or failed; existence uncertain")
        if not isinstance(created, CreatedProcess):
            self._hold("worker creation returned incomplete process information")
        if (created.process == created.thread or
                created.process in self.local.values() or
                created.thread in self.local.values()):
            self._hold("worker creation handles alias another owned handle")
        self.worker = created
        self.local["worker_process"] = created.process
        self.local["worker_thread"] = created.thread
        self._retain()
        self.report.create_outcome["worker"] = "created suspended; assignment unverified"
        for label, job in (("worker_in_worker", self.local["worker_job"]),
                           ("worker_in_child", self.local["child_job"])):
            answer = self.backend.in_job(created.process, job)
            self.report.membership[label] = answer
        if (self.report.membership["worker_in_worker"] is not True or
                self.report.membership["worker_in_child"] is not False):
            self._hold("exact worker job membership mismatch")
        self.report.phase = "worker-gated"

    def offer_child_job(self):
        if self.report.phase != "worker-gated" or self.worker is None:
            self._hold("verified worker required before CHILD job handoff")
        remote = self.backend.duplicate_to_worker(
            self.local["child_job"], self.worker.process, inherit=False)
        if (not isinstance(remote, observed.RemoteHandle) or
                remote.owner_process_handle != self.worker.process.value):
            self._hold("CHILD job duplicate lacks exact worker-table provenance")
        self.remote_job = remote
        self.report.remote_child_job = remote.value
        self.report.phase = "child-job-offered"

    def command_worker(self):
        if self.report.phase != "child-job-offered" or self.remote_job is None:
            self._hold("worker-local CHILD job duplicate required")
        command = {"kind": "COMMAND", "nonce": self.nonce,
                   "child_job": str(self.remote_job.value), "image": self.image,
                   "sha256": CP302_SHA256, "flags": CREATE_FLAGS,
                   "inherit_handles": False, "attribute_count": 1}
        self.command_attempted = True
        self.report.create_outcome["child"] = "unknown: command delivery or CreateProcessW"
        self.report.phase = "command-attempted"
        self.channel.send(framing.encode_frame(command))
        self.report.phase = "awaiting-offer"

    def receive_offer(self):
        if self.report.phase != "awaiting-offer" or self.worker is None:
            self._hold("child offer out of order")
        obj = _one_frame(self.channel.receive())
        if (set(obj) != {"kind", "nonce", "handle", "pid", "creation_filetime", "image"}
                or obj["kind"] != "OFFER" or obj["nonce"] != self.nonce or
                type(obj["pid"]) is not int or obj["pid"] <= 0 or
                type(obj["creation_filetime"]) is not int or
                obj["creation_filetime"] <= 0 or
                not isinstance(obj["image"], str) or
                obj["image"].casefold() != self.image.casefold()):
            self._hold("child offer occurrence or fixed identity mismatch")
        try:
            value = framing.handle_value(obj["handle"], 64)
        except framing.FrameError as error:
            self._hold(f"malformed worker-local child handle: {error}")
        remote = observed.RemoteHandle(value, self.worker.process.value)
        try:
            child = self.backend.duplicate_from_worker(self.worker.process, remote,
                                                        inherit=False)
        except BaseException as error:
            if isinstance(error, DuplicationInterrupted) and error.handle is not None:
                if error.handle in self.local.values():
                    self._hold("partial child process duplicate aliases an owned handle")
                self.child = error.handle
                self.local["child_process"] = error.handle
                self._retain()
            self._hold("child DuplicateHandle outcome unknown")
        if not isinstance(child, observed.LocalHandle):
            self._hold("exact child process duplication unconfirmed")
        if child in self.local.values():
            self._hold("child process handle aliases another owned handle")
        self.child = child
        self.local["child_process"] = child
        self._retain()
        if self.backend.handle_flags(child) & abi.HANDLE_FLAG_INHERIT:
            self._hold("duplicated child process handle is inheritable")
        identity = self.backend.process_identity(child)
        if (not isinstance(identity, Identity) or identity.pid != obj["pid"] or
                identity.creation_filetime != obj["creation_filetime"] or
                identity.image.casefold() != self.image.casefold()):
            self._hold("exact child process identity mismatch")
        for label, job in (("child_in_worker", self.local["worker_job"]),
                           ("child_in_child", self.local["child_job"])):
            self.report.membership[label] = self.backend.in_job(child, job)
        if (self.report.membership["child_in_worker"] is not True or
                self.report.membership["child_in_child"] is not True):
            self._hold("nested child membership not confirmed in both jobs")
        self.report.create_outcome["child"] = (
            "exact child identity and both memberships observed; suspension is a worker claim")
        self.report.phase = "child-verified"
        self.channel.send(framing.encode_frame({"kind": "ACK", "nonce": self.nonce,
                                                "handle": obj["handle"]}))
        receipt = _one_frame(self.channel.receive())
        if receipt != {"kind": "RECEIPT", "nonce": self.nonce,
                       "handle": obj["handle"]}:
            self._hold("worker receipt mismatch or unknown ACK delivery")
        self.report.phase = "fake-topology-observed"

    def cleanup(self, *, wait_ms: int = WAIT_MS) -> Report:
        if type(wait_ms) is not int or not 0 <= wait_ms <= 5000:
            raise ValueError("bounded wait [0, 5000] required")
        previous = self.report.phase
        self.report.phase = "cleanup"
        for name in ("child_job", "worker_job"):
            if name not in self.local:
                continue
            try:
                ok = self.backend.terminate_job(self.local[name], EXIT_CODE)
                self.report.termination[name] = "confirmed" if ok is True else "unknown"
            except BaseException as error:
                self.report.termination[name] = "unknown"
                self.report.errors.append(f"{name} termination unknown: {error}")
        for name in ("child_process", "worker_process"):
            if name not in self.local:
                continue
            try:
                result = self.backend.wait(self.local[name], wait_ms)
                self.report.waits[name] = (
                    "exited" if type(result) is int and result == observed.WAIT_OBJECT_0
                    else "unknown")
            except BaseException as error:
                self.report.waits[name] = "unknown"
                self.report.errors.append(f"{name} exact wait unknown: {error}")
        for name in ("child_job", "worker_job"):
            if name not in self.local:
                continue
            try:
                count = self.backend.active_count(self.local[name])
                self.report.active_counts[name] = count if type(count) is int and count >= 0 else None
            except BaseException as error:
                self.report.active_counts[name] = None
                self.report.errors.append(f"{name} active count unknown: {error}")
        exact_child_required = self.command_attempted
        complete = (all(self.report.termination.get(n) == "confirmed" for n in
                        ("worker_job", "child_job") if n in self.local) and
                    all(self.report.active_counts.get(n) == 0 for n in
                        ("worker_job", "child_job") if n in self.local) and
                    ("worker_process" not in self.local or
                     self.report.waits.get("worker_process") == "exited") and
                    (not exact_child_required or
                     self.report.waits.get("child_process") == "exited") and
                    previous == "fake-topology-observed")
        # A missing OFFER after COMMAND never proves that no child was created.
        # Keep both supervisor job handles on any uncertain outcome.
        for name in ("child_process", "worker_thread", "worker_process",
                     "child_job", "worker_job"):
            handle = self.local.get(name)
            if handle is None:
                continue
            may_close = complete or (name == "child_process" and
                                     self.report.waits.get(name) == "exited") or (
                name in ("worker_thread", "worker_process") and
                self.report.waits.get("worker_process") == "exited")
            if not may_close:
                self.report.closes[name] = "deferred; exact handle retained"
                continue
            try:
                ok = self.backend.close(handle)
            except BaseException as error:
                ok = False
                self.report.errors.append(f"{name} close unknown: {error}")
            self.report.closes[name] = "confirmed" if ok is True else "unknown"
            if ok is True:
                del self.local[name]
        self._retain()
        self.report.phase = "fake-cleanup-confirmed" if complete and not self.local else "cleanup-uncertain"
        return self.report

    def run(self) -> Report:
        """Run one fake occurrence and always attempt bounded cleanup."""
        try:
            self.stage()
            self.create_worker()
            self.offer_child_job()
            self.command_worker()
            self.receive_offer()
        except BaseException as error:
            if not isinstance(error, TopologyHold):
                self.report.errors.append(f"{type(error).__name__}: {error}")
                self.report.phase = "hold"
        finally:
            self.cleanup()
        return self.report


def run_native_topology(*_args, **_kwargs):
    raise TopologyHold("CP312 native topology HARD HOLD: " + "; ".join(HARD_HOLD_DEPENDENCIES))
