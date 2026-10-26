"""CP211 outer-loader hardware observer decision core. Original execution HOLD.

The original factory is unconditionally HOLD. A separate fixed harmless native
fixture transport exercises this decision core; it accepts no original target,
arbitrary executable or PID. That transport does not supply the original's
unverified QString/frame/unwind contracts or original release readiness.
"""
from dataclasses import asdict, dataclass, field
from copy import deepcopy
import hashlib
import json
from pathlib import Path
import sys

ORIGINAL_RUN_ENABLED = False
ORIGINAL_READY = False
ORIGINAL_TOKEN = '--run-owned-original-outer-loader-once'
TARGET = r'C:\Program Files\SFTC\DEFORM\v11.0.2\DEF_GUI_MO_64.exe'
TARGET_SHA256 = 'fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc'
FIXTURE_CWD = r'C:\PROBLEM\p202a'
SELECTED_PATH = FIXTURE_CWD + r'\Gear_blank.moproj'
FIXTURE_DIGEST = '55e2a58cf8155995f942f89ac0ebb91454308b521ae76a228edec0ffdc4cedbd'
SOURCE_FIXTURE = 'evidence/gui_mo/oracle_b/98_B_DS3F_before'
FIXED_ARGV = (TARGET, '-m', 'open', '-f', SELECTED_PATH)
ENTRY, RETURN = 0x1ca30, 0x1c94
STAGES = (0x1cf11, 0x1d08a, 0x1d0bd, 0x1d0d8, 0x1d0fb)
FAILURES = ('duplicate-identity', 'factory-return-null', 'cast-return-null',
            'project-accessor-null', 'acceptance-false')
DEBUG_FIELDS = ('Dr0', 'Dr1', 'Dr2', 'Dr3', 'Dr6', 'Dr7')
CONTINUE, NOT_HANDLED = 0x10002, 0x80010001
HOLD_REASONS = (
    'Original native transport remains an unconditionally disabled fixed-target skeleton',
    'Exact pinned PE and loaded instruction spans/body RSP relation not independently verified',
    'Bounded target QString decoder and guarded ReadProcessMemory transport not implemented',
    'Original-context restart and per-thread arm policy remain independently unreviewed',
    'Qualified native x64 unwind/module binding and closed-occurrence association not implemented',
    'Harmless native fixture coverage does not establish original observation neutrality',
    'Arbitrary real Win32 failure/partial mutation and abrupt debugger-death cleanup remain unproved',
    'Independent release review of final original-capable transport remains required',
)


class Hold(RuntimeError):
    pass


class Incomplete(RuntimeError):
    pass


def u64(value):
    return type(value) is int and 0 <= value < 2 ** 64


@dataclass(frozen=True)
class Identity:
    pid: int
    creation_filetime: str
    image_base: int

    def __post_init__(self):
        if (type(self.pid) is not int or self.pid <= 0 or not u64(self.image_base)
                or self.image_base + max(ENTRY, RETURN, *STAGES) >= 2 ** 64
                or not isinstance(self.creation_filetime, str)
                or not self.creation_filetime.isascii() or not self.creation_filetime.isdigit()
                or not 0 < int(self.creation_filetime) < 2 ** 64):
            raise ValueError('Malformed owned creation identity; FILETIME must be exact text')


@dataclass(frozen=True)
class FrameContract:
    """Transport-supplied exact-prologue contract, synthetic only until review.

    body_delta is E minus body RSP, not a decompiler local offset. The original
    runner has NO default for it. The fixture explicitly supplies its own 0x48.
    """
    body_delta: int
    qualification: str

    def __post_init__(self):
        if (type(self.body_delta) is not int or not 0 <= self.body_delta <= 0x10000
                or self.body_delta % 8 or not self.qualification):
            raise ValueError('Missing verified body frame contract')


@dataclass
class Occurrence:
    number: int
    tid: int
    generation: int
    entry_rsp: int
    main: int
    path_address: int
    path: str
    stage: int = 0
    route: str | None = None
    values: list = field(default_factory=list)
    outer_result: int | None = None


@dataclass
class Thread:
    generation: int
    baseline: dict
    desired: dict
    armed: bool = False
    occurrence: Occurrence | None = None


class OuterObserver:
    """Fail-closed decision core; all input comes from a stopped-event adapter.

    No returned debug-register proposal is evidence it reached hardware. arm_ack
    and change_ack require a fresh readback before any later event is accepted.
    Native transport must preserve all ordinary registers/flags, use execution
    DR7 encodings, clear owned DR6 bits, set RF for restart, and restore owned
    state while stopped. This class NEVER claims OS cleanup or capture readiness.
    """
    def __init__(self, identity, frame, sink, *, event_cap=4096, entry_cap=16,
                 scalar_cap=128, deadline_seconds=30):
        self.identity, self.frame, self.sink = identity, frame, sink
        self.event_cap, self.entry_cap, self.scalar_cap = event_cap, entry_cap, scalar_cap
        self.deadline_seconds = deadline_seconds
        self.threads, self.generations, self.closed = {}, {}, []
        self.seq = self.entries = self.scalars = 0
        self.last_ms = 0
        self.errors, self.avs = [], []
        self.stopped = False
        self.rejected = {'unrelated-entry': 0, 'unselected-return': 0}

    def emit(self, kind, **data):
        # Sink failures propagate to the future native transport's failure drain.
        self.sink({'kind': kind, 'sequence': self.seq, **data})

    def fail(self, reason):
        self.errors.append(reason)
        self.stopped = True
        self.emit('incomplete', reason=reason)
        raise Incomplete(reason)

    def envelope(self, e):
        """One call per delivered debug event, including create/exit/unknown.

        event sequence is transport-wide and must never be reset at selection.
        elapsed_ms is monotonic time from owned creation, not wall-clock text.
        """
        if self.errors:
            raise Incomplete('Observer already incomplete; transport must drain')
        if (not isinstance(e, dict) or type(e.get('sequence')) is not int
                or e.get('sequence') != self.seq + 1):
            self.fail('Lost, duplicated or out-of-order event')
        self.seq += 1
        elapsed = e.get('elapsed_ms')
        if (type(elapsed) is not int or elapsed < self.last_ms
                or elapsed >= self.deadline_seconds * 1000 or self.seq >= self.event_cap):
            self.fail('Deadline/event cap or malformed monotonic time')
        self.last_ms = elapsed
        if (e.get('pid') != self.identity.pid
                or e.get('creation_filetime') != self.identity.creation_filetime):
            self.fail('Owned process creation mismatch')
        self.emit('raw-event', event=e)

    def proposal(self, stage=None):
        b = self.identity.image_base
        return dict(Dr0=b + ENTRY, Dr1=b + RETURN,
                    Dr2=0 if stage is None else b + STAGES[stage], Dr3=0,
                    Dr6=0, Dr7=0x5 if stage is None else 0x15)

    def thread(self, e):
        t = self.threads.get(e.get('tid'))
        if t is None or e.get('generation') != t.generation:
            self.fail('Unknown, exited or reused thread generation')
        return t

    def ack(self, t, readback):
        if (not isinstance(readback, dict) or readback != t.desired
                or any(not u64(v) for v in readback.values())):
            self.fail('Debug-register readback mismatch')
        t.armed = True
        self.emit('verified-debug-state', readback=readback)

    def create_thread(self, e, baseline):
        self.envelope(e)
        tid = e.get('tid')
        if type(tid) is not int or tid <= 0 or tid in self.threads:
            self.fail('Duplicate/malformed thread creation')
        expected = self.generations.get(tid, 0) + 1
        if e.get('generation') != expected:
            self.fail('Nonmonotonic thread generation')
        # CP210 only proved zero baseline. Reject every unexplained nonzero DR.
        if not isinstance(baseline, dict) or set(baseline) != set(DEBUG_FIELDS) or any(
                type(v) is not int or v != 0 for v in baseline.values()):
            self.fail('Unexplained preexisting debug state')
        self.generations[tid] = expected
        t = Thread(expected, dict(baseline), self.proposal())
        self.threads[tid] = t
        self.emit('arm-request', tid=tid, generation=expected, before=baseline, desired=t.desired)
        return dict(t.desired)

    def arm_ack(self, tid, generation, readback):
        t = self.thread(dict(tid=tid, generation=generation))
        self.ack(t, readback)

    def exit_thread(self, e):
        self.envelope(e)
        t = self.thread(e)
        if t.occurrence:
            self.fail('Thread exited with selected occurrence active')
        del self.threads[e['tid']]
        self.emit('thread-exit-observed', tid=e['tid'], generation=t.generation)

    def advance(self, t, stage=None):
        t.desired, t.armed = self.proposal(stage), False
        self.emit('stage-request', desired=t.desired)
        return dict(t.desired)

    def context(self, c):
        names = ('Rip', 'Rsp', 'Rax', 'Rcx', 'Rdx', 'EFlags')
        if not isinstance(c, dict) or any(not u64(c.get(n)) for n in names):
            self.fail('Malformed x64 context')
        if c['Rsp'] % 8 or c['EFlags'] > 0xffffffff:
            self.fail('Malformed stack/flags context')

    def hit(self, e, c, debug, *, return_marker=None, path=None, read_error=None):
        """Owned single-step only; read_error is not a null pointer/value.

        return_marker is a bounded successful 8-byte read at the selected E.
        path must be an exact, bounded, successfully decoded QString, not a
        prefix or replacement-decoded string. Decoder itself is a native gate.
        """
        self.envelope(e)
        t = self.thread(e)
        self.context(c)
        if self.stopped or not t.armed:
            self.fail('Hardware event after stop or before verified arm')
        if (e.get('code') != 0x80000004 or e.get('first_chance') is not True
                or e.get('exception_address') != c['Rip']):
            self.fail('Unexpected hardware exception identity/chance')
        if (not isinstance(debug, dict) or set(debug) != set(DEBUG_FIELDS)
                or any(not u64(v) for v in debug.values())):
            self.fail('Malformed debug context')
        # DR6.BS/BD/BT are foreign causes too; reserved high bits are retained raw.
        slot = {self.identity.image_base + ENTRY: 0,
                self.identity.image_base + RETURN: 1}
        o = t.occurrence
        if o and o.route is None:
            slot[self.identity.image_base + STAGES[o.stage]] = 2
        index = slot.get(c['Rip'])
        if (index is None or debug['Dr6'] & 0xe00f != 1 << index
                or any((debug[n] & ~0x400 if n == 'Dr7' else debug[n]) !=
                       (t.desired[n] & ~0x400 if n == 'Dr7' else t.desired[n])
                       for n in DEBUG_FIELDS if n != 'Dr6')):
            self.fail('Foreign/multiple debug cause, wrong slot or changed debug ownership')
        if read_error is not None:
            self.fail('Guarded read failed: ' + str(read_error))
        if index == 0:
            if any(x.occurrence for x in self.threads.values()):
                self.fail('Overlapping/recursive outer entry cannot be attributed')
            if return_marker != self.identity.image_base + RETURN or path != SELECTED_PATH:
                self.rejected['unrelated-entry'] += 1
                self.emit('unrelated-entry', caller=return_marker, path=path)
                return self.advance(t)
            if c['Rcx'] == 0 or c['Rdx'] == 0 or c['Rsp'] < self.frame.body_delta:
                self.fail('Invalid selected entry pointers/frame')
            self.entries += 1
            if self.entries >= self.entry_cap:
                self.fail('Selected-entry cap reached')
            t.occurrence = Occurrence(self.entries, e['tid'], t.generation,
                                      c['Rsp'], c['Rcx'], c['Rdx'], path)
            self.emit('selected-entry', occurrence=asdict(t.occurrence), context=c)
            return self.advance(t, 0)
        if o is None:
            if index == 1:
                self.rejected['unselected-return'] += 1
                self.emit('unselected-return', context=c)
                return self.advance(t)
            self.fail('Stage without selected occurrence')
        if return_marker != self.identity.image_base + RETURN:
            self.fail('Selected entry return marker lost/reused')
        if index == 1:
            if c['Rsp'] != o.entry_rsp + 8:
                self.fail('Selected continuation frame mismatch')
            if o.route is None:
                self.fail('Selected normal return missing required stage')
            o.outer_result = c['Rax']
            self.closed.append(asdict(o))
            self.emit('outer-return', occurrence=asdict(o), context=c,
                      classification=('divergent-nonzero-outer' if c['Rax'] else
                                      'zero-after-' + o.route))
            t.occurrence = None
            return self.advance(t)
        if c['Rsp'] != o.entry_rsp - self.frame.body_delta:
            self.fail('Stage body frame mismatch')
        self.scalars += 1
        if self.scalars >= self.scalar_cap:
            self.fail('Scalar cap reached')
        value = c['Rax']
        o.values.append(dict(rva=STAGES[o.stage], rax=value, al=value & 255))
        self.emit('scalar', occurrence=o.number, stage=o.stage, context=c)
        failed = value != 0 if o.stage == 0 else (value & 255 == 0 if o.stage == 4 else value == 0)
        if failed:
            o.route = FAILURES[o.stage]
        elif o.stage == 4:
            o.route = 'all-five-prerequisites-passed'
        else:
            o.stage += 1
        return self.advance(t, o.stage if o.route is None else None)

    def unknown(self, e):
        self.envelope(e)
        # A future transport must pass it through even though evidence is invalid.
        self.emit('exception-disposition', disposition=NOT_HANDLED)
        self.fail('Unknown breakpoint/single-step/exception; pass through and drain')

    def av(self, e, core):
        self.envelope(e)
        self.thread(e)
        # Persist compact core before any proposed enrichment.
        self.emit('av-core', core=core, disposition=NOT_HANDLED,
                  closed_occurrences=[o['number'] for o in self.closed])
        if (e.get('code') != 0xc0000005 or type(e.get('first_chance')) is not bool
                or not isinstance(core, dict)):
            self.fail('Malformed application AV')
        chance = e['first_chance']
        if (not self.avs and not chance) or len(self.avs) >= 2 or (self.avs and chance):
            self.fail('Lost/out-of-order AV chances')
        if any(t.occurrence for t in self.threads.values()):
            self.errors.append('AV interrupted selected outer frame; no complete route')
        self.stopped = True
        self.avs.append(dict(tid=e['tid'], generation=e['generation'],
                             first_chance=chance, core=deepcopy(core)))
        if len(self.avs) == 2 and any(self.avs[0][n] != self.avs[1][n]
                                       for n in ('tid', 'generation', 'core')):
            self.errors.append('AV chance pair changed')
        return NOT_HANDLED

    def cp205(self):
        if len(self.avs) != 2 or self.errors:
            return False
        c = self.avs[0]['core']
        # Transport must independently verify image hash/base, native unwind and
        # occurrence association. Boolean supplied by a mock is not native proof.
        expected = dict(module='QtCore4_64.dll', rva=0x32c0, operation=0,
                        address=0x58, rcx=0x58, parameter_count=2,
                        app_returns=[0xa7510, 0xeb1ec, 0xeaa7a],
                        callback_returns=[0x2a725, 0x1cae])
        eligible = [o for o in self.closed if o['outer_result'] == 0]
        return (all(c.get(k) == v for k, v in expected.items())
                and c.get('native_unwind_qualified') is True
                and c.get('module_identity_qualified') is True
                and len(eligible) == 1 and len(self.closed) == 1
                and eligible[0]['tid'] == self.avs[0]['tid']
                and eligible[0]['generation'] == self.avs[0]['generation']
                and c.get('closed_occurrence') == eligible[0]['number'])

    def summary(self):
        return dict(status='HOLD', original_ready=False, original_launched=False,
                    gui_control_acquired=False, native_cleanup_verified=False,
                    closed=self.closed, errors=self.errors, counters=self.rejected,
                    events=self.seq, selected_entries=self.entries, scalars=self.scalars,
                    cp205_input_contract_matched=self.cp205(),
                    hold_reasons=list(HOLD_REASONS))


class JsonlSink:
    """Incremental receipt sink. Never catches serialization/write failures."""
    def __init__(self, file):
        self.file = file

    def __call__(self, row):
        self.file.write(json.dumps(row, sort_keys=True, allow_nan=False) + '\n')
        self.file.flush()


# The only native launch supported below is the fixed harmless fixture. This
# base is a private fixture allocation, never an original image RVA binding.
NATIVE_FIXTURE_BASE = 0x1a0000000
NATIVE_FIXTURE_MODES = ('normal', 'duplicate', 'av')
NATIVE_FIXTURE_FAULTS = (
    None, 'before-create-event', 'after-create-event', 'before-context-set',
    'after-context-set', 'after-arm', 'after-thread-event',
    'before-hardware-continuation', 'after-stage-hit', 'before-exit-continuation',
    'native-invalid-handle',
)
CP210_ABI_SHA256 = '0aea50992e9e6749f9d5f4db7092710d61ca159e1622f7fd65c275e273ed31ee'


class FixedOriginalTransport:
    """Reviewable original boundary; no process creation is implemented here.

    A future adapter must independently populate original byte/frame/QString and
    native unwind contracts. Merely enabling either literal flag is insufficient.
    """
    @staticmethod
    def contract():
        return dict(status='HOLD', target=TARGET, target_sha256=TARGET_SHA256,
                    installation_version='11.0.2', version_resource=None,
                    cwd=FIXTURE_CWD, argv=list(FIXED_ARGV), fixture_digest=FIXTURE_DIGEST,
                    debug_flags=['DEBUG_ONLY_THIS_PROCESS', 'CREATE_NO_WINDOW'],
                    process_identity=['exact retained creation handle', 'PID', 'FILETIME text',
                                      'queried image path/hash', 'loaded main base/bytes'],
                    handles=['caller process/thread', 'retained duplicate process',
                             'event process/thread generations', 'image/DLL file generations'],
                    cleanup=['checked pending-event continuation/drain', 'owned DR restore/readback',
                             'exact-child termination only if needed', 'continued natural exit',
                             'exact creation handle signaled', 'independent post-debugger absence'],
                    hold_reasons=list(HOLD_REASONS))

    @staticmethod
    def launch():
        # Unconditional even if flags are accidentally edited; no fixture fallback.
        raise Hold('Original native transport is a skeleton: ' + '; '.join(HOLD_REASONS))


def load_cp210_abi():
    """Lazy, pinned local reuse; does not execute the CP210 fixture suite."""
    import importlib.util
    path = Path(__file__).with_name('child_debug_event_fixture.py')
    if hashlib.sha256(path.read_bytes()).hexdigest() != CP210_ABI_SHA256:
        raise Hold('CP210 ctypes/ABI dependency identity changed')
    spec = importlib.util.spec_from_file_location('cp211_pinned_native_abi', path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    if (module.C.sizeof(module.EVENT) != 176 or module.C.sizeof(module.CONTEXT) != 1232
            or module.CONTEXT.Rip.offset != 248):
        raise Hold('Native x64 ABI mismatch')
    return module


def native_fixture_image(mode):
    """Fixed harmless x64 code/data spans, consumed by child and verifier.

    These labels model original RVAs only to exercise the real observer. They
    establish no original instruction bytes, prologue or call-site semantics.
    """
    import struct
    if mode not in NATIVE_FIXTURE_MODES:
        raise ValueError('Unknown fixed native fixture mode')
    base = NATIVE_FIXTURE_BASE
    def rel(origin, target):
        return b'\xe9' + struct.pack('<i', target - origin - 5)
    wrapper = (bytes.fromhex('4883ec2848b9') + struct.pack('<Q', 0x11223344)
               + bytes.fromhex('48ba') + struct.pack('<Q', base + 0x24000) + b'\x90\x90')
    if len(wrapper) != 26:
        raise RuntimeError('Fixture wrapper layout mismatch')
    wrapper += b'\xe8' + struct.pack('<i', ENTRY - RETURN) + bytes.fromhex('4883c428c3')
    spans = {0x1c75: wrapper,
             ENTRY: bytes.fromhex('4883ec4831c0') + rel(ENTRY + 6, STAGES[0]),
             0x22000: bytes.fromhex('48c7c158000000488b01c3'),
             0x24000: (SELECTED_PATH + '\0').encode('utf-16-le')}
    if mode == 'duplicate':
        spans[ENTRY] = bytes.fromhex('4883ec48b801000000') + rel(ENTRY + 9, STAGES[0])
        spans[STAGES[0]] = bytes.fromhex('9031c04883c448c3')
    else:
        for i, (rva, value) in enumerate(zip(STAGES, (0x1111, 0x2222, 0x3333, 0x101, 0x4444))):
            code = b'\x90\xb8' + struct.pack('<I', value)
            code += rel(rva + len(code), STAGES[i + 1]) if i < 4 else bytes.fromhex('4883c448c3')
            spans[rva] = code
    return spans


def run_harmless_native_fixture(mode='normal', fault=None, sink=None):
    """Own exactly one fresh fixed interpreter/fixture child; no target argument.

    Reuses CP210's API declarations only. All lifecycle, staged context logic and
    receipts below are new and remain subject to their own fixture review.
    """
    import os
    import secrets
    import subprocess
    import time
    if mode not in NATIVE_FIXTURE_MODES or fault not in NATIVE_FIXTURE_FAULTS:
        raise ValueError('Only fixed harmless native fixture cases are supported')
    if os.name != 'nt':
        raise Hold('Harmless native fixture requires Windows x64')
    A = load_cp210_abi()
    C, D, P = A.C, A.D, A.P
    script = Path(__file__).with_name('test_outer_loader_debug_native_fixture.py').resolve(strict=True)
    interpreter = Path(sys.executable).resolve(strict=True)
    token = secrets.token_hex(16)
    argv = [str(interpreter), '-I', '-S', str(script), '--native-child', token, mode]
    command = subprocess.list2cmdline(argv)
    report = dict(schema='cp211-harmless-native-case-v1', mode=mode, fault=fault,
                  original_launched=False, gui_control_acquired=False,
                  target=dict(path=str(interpreter), sha256=hashlib.sha256(interpreter.read_bytes()).hexdigest(),
                              argv=argv, cwd=str(script.parent),
                              fixture_script_sha256=hashlib.sha256(script.read_bytes()).hexdigest()),
                  source_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
                  cp210_abi_sha256=CP210_ABI_SHA256,
                  events=0, continues=0, trace_rows=0, handles=[], continuation_ledger=[],
                  context_ledger=[], exceptions=[], cleanup_errors=[], receipt_errors=[],
                  forced_cleanup=False, exit_observed=False, marker_verified=False,
                  counters=dict(loader=0, hardware=0, av_first=0, av_last=0, unknown=0))
    started = time.monotonic()
    pi, si, event = A.PI(), A.STARTUPINFO(), A.EVENT()
    si.cb = C.sizeof(si)
    retained, pending, observer = P(), False, None
    live, threads, contexts = {}, {}, {}
    generations, event_threads = {}, {}
    event_process, ntdll = None, None
    restored = False
    raw_complete = True

    def emit(kind, **data):
        nonlocal sink, raw_complete
        row = dict(kind=kind, transport_event=report['events'], **data)
        report['trace_rows'] += 1
        if sink:
            try:
                sink(row)
            except Exception as exc:
                report['receipt_errors'].append(repr(exc))
                raw_complete, sink = False, None
                raise

    def check(ok):
        if not ok:
            raise C.WinError(C.get_last_error())

    def inject(point):
        if fault == point:
            report['injection_reached'] = True
            raise RuntimeError('fixture-injected:' + point)

    def own(handle, kind, owner):
        if not handle:
            return
        if handle in live:
            live[handle].setdefault('aliases', []).append(dict(kind=kind, owner=owner))
            return
        row = dict(generation=len(report['handles']) + 1, value=hex(handle),
                   kind=kind, owner=owner, closed=False)
        live[handle] = row
        report['handles'].append(row)
        # Acquisition registration cannot call a fallible sink before sibling
        # handles returned by the same Win32 operation have also been recorded.

    def closed(handle, how):
        row = live.pop(handle, None)
        if row:
            flags = D()
            row.update(closed=True, closure=how,
                       invalid_immediately=not A.handleinfo(handle, C.byref(flags)) and C.get_last_error() == 6)
            emit('handle-closed', handle=dict(row))

    def close_checked(handle):
        ok = bool(A.close(handle))
        if ok:
            closed(handle, 'CloseHandle')
        else:
            report['cleanup_errors'].append(dict(api='CloseHandle', handle=hex(handle), error=C.get_last_error()))
        return ok

    def read_exact(address, size):
        if not u64(address) or type(size) is not int or not 0 < size <= 4096:
            raise RuntimeError('Invalid bounded fixture read')
        buffer, count = C.create_string_buffer(size), C.c_size_t()
        check(A.read(retained.value or pi.process, address, buffer, size, C.byref(count)))
        if count.value != size:
            raise RuntimeError('Short bounded fixture read')
        return buffer.raw

    def ctx_parts(ctx):
        ordinary = {name: getattr(ctx, name) for name in (
            'Rax', 'Rcx', 'Rdx', 'Rbx', 'Rsp', 'Rbp', 'Rsi', 'Rdi', 'R8', 'R9', 'R10',
            'R11', 'R12', 'R13', 'R14', 'R15', 'Rip', 'EFlags')}
        return ordinary, {name: getattr(ctx, name) for name in DEBUG_FIELDS}

    def set_debug(tid, desired, restart=False, cleanup=False):
        nonlocal restored
        handle = threads[tid]
        raw, ctx = A.context(handle)
        before, debug_before = ctx_parts(ctx)
        baseline = contexts.setdefault(tid, dict(generation=generations[tid], baseline=debug_before,
                                                baseline_rf=ctx.EFlags & 0x10000, mutated=False))
        if not cleanup:
            inject('before-context-set')
            if fault == 'native-invalid-handle':
                report['injection_reached'] = True
                # Real kernel API failure on an invalid handle; this does not
                # model partial writes to a valid original thread context.
                check(A.setctx(None, C.byref(ctx)))
        for name, value in desired.items():
            setattr(ctx, name, value)
        if cleanup:
            ctx.EFlags = (ctx.EFlags & ~0x10000) | baseline['baseline_rf']
        elif restart:
            ctx.EFlags |= 0x10000
        baseline['mutated'] = True  # before the call: partial failure requires cleanup
        check(A.setctx(handle, C.byref(ctx)))
        if not cleanup:
            inject('after-context-set')
        raw2, after = A.context(handle)
        after_ordinary, after_debug = ctx_parts(after)
        expected_flags = ctx.EFlags
        stable = all(after_ordinary[k] == v for k, v in before.items() if k != 'EFlags')
        # Windows normalizes reserved EFLAGS bit 1 in the held fixture context.
        # Preserve every architected status/control bit, allowing only this
        # observed reserved-bit difference plus the explicitly requested RF.
        stable = stable and (after_ordinary['EFlags'] & ~2) == (expected_flags & ~2)
        row = dict(tid=tid, generation=generations[tid], before=debug_before,
                   desired=desired, readback=after_debug, ordinary_before=before,
                   ordinary_after=after_ordinary, ordinary_stable=stable,
                   restart=restart, cleanup=cleanup)
        report['context_ledger'].append(row)
        emit('context-write-readback', **row)
        if after_debug != desired or not stable:
            raise RuntimeError('Context readback or ordinary-state preservation mismatch')
        if cleanup:
            baseline['mutated'] = False
        else:
            observer.arm_ack(tid, generations[tid], after_debug)
            inject('after-arm')

    def restore_all():
        nonlocal restored
        if not pending:
            raise RuntimeError('Refusing context restoration without a held debug event')
        for tid, saved in list(contexts.items()):
            if saved['mutated'] and tid in threads:
                set_debug(tid, saved['baseline'], cleanup=True)
        restored = not any(c['mutated'] for c in contexts.values())
        report['debug_state_restored'] = restored

    def resources():
        nonlocal event_process, ntdll
        if event.code == 3:
            event_process = event.u.create.process
            own(event_process, 'event-process', 'event')
            own(event.u.create.thread, 'event-thread', 'event')
            threads[event.tid] = event.u.create.thread
            event_threads[event.tid] = event.u.create.thread
            generations[event.tid] = generations.get(event.tid, 0) + 1
            report['loaded_main_base'] = hex(event.u.create.base)
            if event.u.create.file:
                own(event.u.create.file, 'image-file', 'manual')
            emit('event-resources-registered', handle_generations=[r['generation'] for r in live.values()])
            if event.u.create.file:
                if not close_checked(event.u.create.file):
                    raise RuntimeError('Image file close failed')
        elif event.code == 2:
            own(event.u.thread.thread, 'event-thread', 'event')
            threads[event.tid] = event.u.thread.thread
            event_threads[event.tid] = event.u.thread.thread
            generations[event.tid] = generations.get(event.tid, 0) + 1
            emit('thread-generation', tid=event.tid, generation=generations[event.tid],
                 start=hex(event.u.thread.start or 0))
            if event.u.thread.start == NATIVE_FIXTURE_BASE + 0x22000:
                report['expected_av_tid'] = event.tid
        elif event.code == 6 and event.u.load.file:
            handle = event.u.load.file
            own(handle, 'dll-file', 'manual')
            path = C.create_unicode_buffer(32768)
            count = A.finalpath(handle, path, len(path), 0)
            if not count or count >= len(path):
                raise RuntimeError('Loaded DLL path unavailable')
            emit('loaded-module', path=path.value, base=hex(event.u.load.base))
            if Path(path.value).name.lower() == 'ntdll.dll':
                ntdll = event.u.load.base
                digest = hashlib.sha256(Path(path.value).read_bytes()).hexdigest()
                expected = bytes.fromhex('0a807c2440007503cceb004883c438c3cc')
                if digest != '651ac9041af8e6b5ebd331d53163244c0a27d36d1433f0f15cac1f28c5f2e82c':
                    raise RuntimeError('Pinned harmless host ntdll changed')
                if read_exact(ntdll + 0x1226c9 - 8, len(expected)) != expected:
                    raise RuntimeError('Qualified loader breakpoint bytes changed')
                report['loader_module'] = dict(path=path.value, base=hex(ntdll), sha256=digest,
                                               rva=hex(0x1226c9), bytes=expected.hex())
            if not close_checked(handle):
                raise RuntimeError('DLL file close failed')

    def event_envelope():
        return dict(sequence=report['events'], elapsed_ms=int((time.monotonic() - started) * 1000),
                    pid=event.pid, creation_filetime=report['creation_filetime'], tid=event.tid,
                    generation=generations.get(event.tid), event_code=event.code)

    def continuation(status, cleanup=False):
        nonlocal pending
        ok = bool(A.cont(event.pid, event.tid, status))
        row = dict(sequence=report['events'], event_code=event.code, tid=event.tid,
                   disposition=hex(status), cleanup=cleanup, ok=ok,
                   error=None if ok else C.get_last_error())
        report['continuation_ledger'].append(row)
        # Mark pending before writing the receipt: an I/O error must not cause a
        # second continuation of an already released Windows debug event.
        if ok:
            pending = False
            report['continues'] += 1
            if event.code == 4:
                handle = event_threads.pop(event.tid, None)
                if handle:
                    closed(handle, 'ContinueDebugEvent:thread-exit')
                threads.pop(event.tid, None)
                contexts.pop(event.tid, None)
            elif event.code == 5:
                report.update(exit_observed=True, exit_code=hex(event.u.exitCode))
                if event_process:
                    closed(event_process, 'ContinueDebugEvent:process-exit')
                for handle in list(event_threads.values()):
                    closed(handle, 'ContinueDebugEvent:process-exit')
                event_threads.clear()
                threads.clear()
        emit('continue-debug-event', **row)
        if not ok:
            raise RuntimeError('ContinueDebugEvent failed')

    def process_event():
        e = event_envelope()
        status = CONTINUE
        if event.code in (2, 3):
            raw, ctx = A.context(threads[event.tid])
            _, baseline = ctx_parts(ctx)
            desired = observer.create_thread(e, baseline)
            set_debug(event.tid, desired)
            inject('after-create-event' if event.code == 3 else 'after-thread-event')
        elif event.code == 4:
            observer.exit_thread(e)
        elif event.code == 1:
            ex = event.u.exception
            raw, ctx = A.context(threads[event.tid])
            ordinary, debug = ctx_parts(ctx)
            e.update(code=int(ex.record.code), first_chance=bool(ex.first),
                     exception_address=int(ex.record.address or 0))
            snapshot = dict(event=e, context=ordinary, debug=debug, parameter_count=int(ex.record.count),
                            exception_information=[int(ex.record.info[i]) for i in range(min(ex.record.count, 15))])
            status = NOT_HANDLED
            if (ex.record.code == 0x80000003 and ex.first == 1 and event.tid == pi.tid
                    and not report['marker_verified'] and report['counters']['loader'] == 0 and ntdll
                    and ex.record.address == ntdll + 0x1226c9 and ctx.Rip == ex.record.address + 1):
                observer.envelope(e)
                report['counters']['loader'] += 1
                snapshot['classification'] = 'qualified-loader-breakpoint'
                status = CONTINUE
            elif ex.record.code == 0x80000004:
                if not report['marker_verified']:
                    raise RuntimeError('Hardware hit before fixture byte qualification')
                o = observer.threads[event.tid].occurrence
                marker_address = ctx.Rsp if ctx.Rip == NATIVE_FIXTURE_BASE + ENTRY else o.entry_rsp if o else None
                return_marker = int.from_bytes(read_exact(marker_address, 8), 'little') if marker_address else None
                path = None
                if ctx.Rip == NATIVE_FIXTURE_BASE + ENTRY:
                    expected_path = (SELECTED_PATH + '\0').encode('utf-16-le')
                    if ctx.Rdx != NATIVE_FIXTURE_BASE + 0x24000:
                        raise RuntimeError('Fixture path pointer changed')
                    path = read_exact(ctx.Rdx, len(expected_path)).decode('utf-16-le')
                    if not path.endswith('\0'):
                        raise RuntimeError('Unterminated fixture path')
                    path = path[:-1]
                desired = observer.hit(e, ordinary, debug, return_marker=return_marker, path=path)
                set_debug(event.tid, desired, restart=True)
                report['counters']['hardware'] += 1
                snapshot['classification'] = 'owned-staged-hardware'
                status = CONTINUE
                if ctx.Rip - NATIVE_FIXTURE_BASE in STAGES:
                    inject('after-stage-hit')
                inject('before-hardware-continuation')
            elif ex.record.code == 0xc0000005:
                report['counters']['av_first' if ex.first else 'av_last'] += 1
                core = dict(module='harmless-private-executable-fixture', rip=int(ctx.Rip),
                            rva=int(ctx.Rip) - NATIVE_FIXTURE_BASE,
                            operation=int(ex.record.info[0]), address=int(ex.record.info[1]), rcx=int(ctx.Rcx),
                            parameter_count=int(ex.record.count), native_unwind_qualified=False,
                            module_identity_qualified=False, app_returns=[], callback_returns=[])
                status = observer.av(e, core)
                if ex.first:
                    restore_all()
                snapshot.update(classification='harmless-native-av', av_core=core)
            else:
                report['counters']['unknown'] += 1
                observer.unknown(e)
            snapshot['disposition'] = hex(status)
            report['exceptions'].append(snapshot)
            emit('native-exception', **snapshot)
        else:
            observer.envelope(e)
            if event.code == 8:
                s = event.u.string
                if s.unicode or not 0 < s.length <= 256:
                    raise RuntimeError('Unexpected fixture debug string encoding/length')
                text = read_exact(s.address, s.length).rstrip(b'\0').decode('ascii')
                if text == 'CP211:' + token + ':' + mode:
                    if report['marker_verified'] or report['counters']['loader'] != 1 or event.tid != pi.tid:
                        raise RuntimeError('Fixture marker lifecycle mismatch')
                    spans = native_fixture_image(mode)
                    for rva, expected in spans.items():
                        actual = read_exact(NATIVE_FIXTURE_BASE + rva, len(expected))
                        if actual != expected:
                            raise RuntimeError('Fixed fixture instruction/data span changed')
                        emit('verified-native-fixture-span', rva=hex(rva), length=len(expected),
                             sha256=hashlib.sha256(actual).hexdigest())
                    report['marker_verified'] = True
                elif text == 'CP211-DONE:' + token and mode != 'av':
                    if not report['marker_verified'] or not observer.closed:
                        raise RuntimeError('Premature fixture completion marker')
                    restore_all()
                else:
                    raise RuntimeError('Unqualified fixture debug marker')
            if event.code == 5:
                inject('before-exit-continuation')
        continuation(status)

    try:
        emit('fixed-launch-contract', contract=report['target'])
        # This is the sole CreateProcess call in this module. Its image is the
        # current interpreter, and its only script is the fixed native fixture.
        check(A.create(str(interpreter), C.create_unicode_buffer(command), None, None, False,
                       0x00000002 | 0x08000000, None, str(script.parent), C.byref(si), C.byref(pi)))
        report.update(pid=int(pi.pid), primary_tid=int(pi.tid))
        own(pi.process, 'caller-process', 'caller')
        own(pi.thread, 'caller-thread', 'caller')
        emit('caller-resources-registered', handle_generations=[r['generation'] for r in live.values()])
        check(A.duplicate(A.current(), pi.process, A.current(), C.byref(retained), 0, False, 2))
        own(retained.value, 'retained-creation-process', 'caller')
        check(A.killonexit(True))
        report['kill_on_debugger_exit'] = True
        image, size = C.create_unicode_buffer(32768), D(32768)
        check(A.queryimage(retained, 0, image, C.byref(size)))
        times = [A.U64() for _ in range(4)]
        check(A.gettimes(retained, *(C.byref(t) for t in times)))
        if Path(image.value).resolve() != interpreter:
            raise RuntimeError('Exact created fixture image path mismatch')
        if hashlib.sha256(Path(image.value).read_bytes()).hexdigest() != report['target']['sha256']:
            raise RuntimeError('Exact created fixture image hash mismatch')
        report['creation_filetime'] = str(times[0].value)
        observer = OuterObserver(Identity(int(pi.pid), str(times[0].value), NATIVE_FIXTURE_BASE),
                                 FrameContract(0x48, 'fixed native fixture exact sub rsp,48 only'),
                                 lambda row: emit('observer-decision', record=row))
        emit('owned-creation-verified', pid=int(pi.pid), primary_tid=int(pi.tid),
             creation_filetime=str(times[0].value), image=image.value,
             synthetic_code_base=hex(NATIVE_FIXTURE_BASE))
        inject('before-create-event')
        while not report['exit_observed']:
            if time.monotonic() - started >= 30 or report['events'] >= 4096:
                raise RuntimeError('Bounded native observation deadline/event cap')
            if not A.wait(C.byref(event), 100):
                if C.get_last_error() == 121:
                    continue
                check(False)
            pending = True
            report['events'] += 1
            if event.pid != pi.pid:
                raise RuntimeError('Unexpected process debug event')
            resources()
            emit('delivered-debug-event', code=int(event.code), pid=int(event.pid), tid=int(event.tid))
            process_event()
        check(A.waitobj(retained, 2000) == 0)
        report['exact_process_signaled'] = True
    except Exception as exc:
        report.update(error=repr(exc), forced_cleanup=True)
        if pi.process:
            if pending:
                try:
                    restore_all()
                except Exception as restore_error:
                    report['cleanup_errors'].append(dict(api='restore-before-terminate', error=repr(restore_error)))
            ok = bool(A.terminate(retained.value or pi.process, 0xe211))
            report['termination'] = dict(ok=ok, error=None if ok else C.get_last_error())
            if pending:
                try:
                    continuation(NOT_HANDLED if event.code == 1 else CONTINUE, cleanup=True)
                except Exception as continue_error:
                    report['cleanup_errors'].append(dict(api='pending-continuation', error=repr(continue_error)))
            until = time.monotonic() + 5
            while not report['exit_observed'] and time.monotonic() < until:
                if not A.wait(C.byref(event), 100):
                    if C.get_last_error() != 121:
                        report['cleanup_errors'].append(dict(api='WaitForDebugEvent', error=C.get_last_error()))
                        break
                    continue
                pending = True
                report['events'] += 1
                try:
                    if event.pid != pi.pid:
                        raise RuntimeError('Unexpected process in failure drain')
                    resources()
                    emit('drained-debug-event', code=int(event.code), pid=int(event.pid), tid=int(event.tid))
                    continuation(NOT_HANDLED if event.code == 1 else CONTINUE, cleanup=True)
                except Exception as drain_error:
                    report['cleanup_errors'].append(dict(api='failure-drain', error=repr(drain_error)))
                    break
            report['exact_process_signaled'] = A.waitobj(retained.value or pi.process, 2000) == 0
    finally:
        for handle, row in list(live.items()):
            if row['owner'] != 'event':
                try:
                    close_checked(handle)
                except Exception as close_error:
                    report['cleanup_errors'].append(dict(api='close-receipt', error=repr(close_error)))
        report['live_handle_generations'] = [r['generation'] for r in live.values()]
        report['handles_closed'] = bool(not live and all(r['closed'] and r.get('invalid_immediately') for r in report['handles']))
        report['pending_event'] = pending
        report['raw_receipt_complete'] = raw_complete
        report['observer'] = observer.summary() if observer else None
    counts = report['counters']
    clean = (report['exit_observed'] and report.get('exact_process_signaled') and report['handles_closed']
             and not pending and not report['cleanup_errors'] and report['events'] == report['continues'])
    closed_rows = observer.closed if observer else []
    expected_hw = 3 if mode == 'duplicate' else 7
    route_ok = (len(closed_rows) == 1 and closed_rows[0]['route'] ==
                ('duplicate-identity' if mode == 'duplicate' else 'all-five-prerequisites-passed')
                and closed_rows[0]['outer_result'] == (0 if mode == 'duplicate' else 0x4444))
    av_rows = [r for r in report['exceptions'] if r.get('classification') == 'harmless-native-av']
    av_ok = (len(av_rows) == 2 and [r['event']['first_chance'] for r in av_rows] == [True, False]
             and av_rows[0]['av_core'] == av_rows[1]['av_core']
             and all(r['event']['tid'] == report.get('expected_av_tid') and r['av_core']['address'] == 0x58
                     and r['av_core']['rcx'] == 0x58 and r['av_core']['operation'] == 0
                     and r['av_core']['rip'] == NATIVE_FIXTURE_BASE + 0x22007
                     and r['disposition'] == hex(NOT_HANDLED) for r in av_rows))
    natural = (clean and not report['forced_cleanup'] and raw_complete and not report['receipt_errors']
               and restored and report['marker_verified'] and counts['loader'] == 1
               and counts['hardware'] == expected_hw and counts['unknown'] == 0 and route_ok
               and not observer.errors and report['exit_code'] == ('0xc0000005' if mode == 'av' else '0x0')
               and ((mode == 'av' and av_ok) or (mode != 'av' and not av_rows)))
    injected = (clean and fault is not None and report.get('injection_reached') and report['forced_cleanup']
                and not any(c['mutated'] for c in contexts.values()))
    report.update(native_capture_complete=bool(natural), expected_case_passed_before_absence=bool(injected if fault else natural),
                  original_ready=False, elapsed_seconds=round(time.monotonic() - started, 6))
    return report


def run_original():
    if not ORIGINAL_RUN_ENABLED:
        raise Hold('Original execution disabled by literal guard; readiness HOLD')
    if not ORIGINAL_READY:
        raise Hold('Original readiness HOLD: ' + '; '.join(HOLD_REASONS))
    # Intentionally no native launch path exists in this safe tranche.
    raise Hold('Native owned-original transport is not implemented')


def main(argv=None):
    argv = sys.argv[1:] if argv is None else argv
    if argv == [ORIGINAL_TOKEN]:
        try:
            run_original()
        except Hold as exc:
            print(json.dumps(dict(status='HOLD', reason=str(exc), original_launched=False)))
            return 2
    if argv:
        print(json.dumps(dict(status='HOLD', reason='No target, PID, attach or arbitrary command interface')))
        return 2
    print(json.dumps(dict(status='HOLD', original_run_enabled=ORIGINAL_RUN_ENABLED,
                          original_ready=ORIGINAL_READY, target=TARGET,
                          target_sha256=TARGET_SHA256, fixture_digest=FIXTURE_DIGEST,
                          source_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
                          hold_reasons=list(HOLD_REASONS)), indent=2))
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
