"""Harmless CP211 fixtures of the actual outer observer, never a native launch.

Run with no arguments, including under Python -I -S. Persist full per-case input
and decision receipts locally; print only the compact outcome and source hashes.
No original/GUI, debugger, process creation or external project access occurs.
"""
import hashlib
import importlib.util
from contextlib import redirect_stdout
import io
import json
from pathlib import Path
import sys
import time

HERE = Path(__file__).resolve()
ROOT = HERE.parent.parent
SPEC = importlib.util.spec_from_file_location('cp211_outer_observer', HERE.with_name('original_outer_loader_debug_probe.py'))
M = importlib.util.module_from_spec(SPEC)
sys.modules[SPEC.name] = M
SPEC.loader.exec_module(M)
BASE = 0x140000000
ZERO = dict.fromkeys(M.DEBUG_FIELDS, 0)
E = 0x100008
RAW = ROOT / 'analysis/resume211-outer-debug-fixture.jsonl'
RECEIPT = ROOT / 'analysis/resume211-outer-debug-fixture.json'


def require(condition, message='Fixture assertion failed'):
    if not condition:
        raise AssertionError(message)


def rejects(call, error=M.Incomplete):
    try:
        call()
    except error:
        return
    raise AssertionError('Expected rejection: ' + error.__name__)


class Fixture:
    def __init__(self, sink, **limits):
        self.o = M.OuterObserver(M.Identity(4242, '134374890722592589', BASE),
                                 M.FrameContract(0x48, 'synthetic frame only; no original byte claim'),
                                 sink, **limits)
        self.seq = 0
        self.tick = 0
        self.make_thread()

    def event(self, tid=7, generation=1, **data):
        self.seq += 1
        self.tick += 1
        return dict(sequence=self.seq, elapsed_ms=self.tick, pid=4242,
                    creation_filetime='134374890722592589', tid=tid,
                    generation=generation, **data)

    def make_thread(self, tid=7, generation=1, baseline=None):
        d = self.o.create_thread(self.event(tid, generation), dict(ZERO) if baseline is None else baseline)
        self.o.arm_ack(tid, generation, d)

    def hit(self, rva, value=0, *, tid=7, generation=1, rsp=None,
            marker=BASE + M.RETURN, path=M.SELECTED_PATH, read_error=None,
            context_change=None, debug_change=None, event_change=None, ack=True):
        t = self.o.threads.get(tid)
        stage = rva not in (M.ENTRY, M.RETURN)
        c = dict(Rip=BASE + rva, Rsp=(E - 0x48 if stage else E + 8 if rva == M.RETURN else E)
                 if rsp is None else rsp, Rax=value, Rcx=0x220000, Rdx=0x330000, EFlags=0x202)
        c.update(context_change or {})
        slot = 0 if rva == M.ENTRY else 1 if rva == M.RETURN else 2
        debug = dict(t.desired if t else self.o.proposal())
        debug['Dr6'] = 1 << slot
        debug.update(debug_change or {})
        e = self.event(tid, generation, code=0x80000004, first_chance=True,
                       exception_address=BASE + rva)
        e.update(event_change or {})
        desired = self.o.hit(e, c, debug, return_marker=marker, path=path, read_error=read_error)
        if ack:
            self.o.arm_ack(tid, generation, desired)
        return desired

    def route(self, values, outer=0, **entry):
        self.hit(M.ENTRY, **entry)
        for rva, value in zip(M.STAGES, values):
            self.hit(rva, value)
        self.hit(M.RETURN, outer)
        return self.o.closed[-1]

    def av(self, first=True, *, tid=7, generation=1, **changes):
        core = dict(module='QtCore4_64.dll', rva=0x32c0, operation=0, address=0x58,
                    rcx=0x58, parameter_count=2, app_returns=[0xa7510, 0xeb1ec, 0xeaa7a],
                    callback_returns=[0x2a725, 0x1cae], native_unwind_qualified=True,
                    module_identity_qualified=True, closed_occurrence=1)
        core.update(changes)
        return self.o.av(self.event(tid, generation, code=0xc0000005, first_chance=first), core)


def cases():
    for i, values in enumerate(([123], [0, 0], [0, 123, 0], [0, 123, 123, 0], [0, 123, 123, 123, 0x100])):
        def route_case(sink, i=i, values=values):
            f = Fixture(sink)
            row = f.route(values)
            require(row['route'] == M.FAILURES[i])
            require(row['outer_result'] == 0 and len(row['values']) == i + 1)
            require(f.o.threads[7].desired['Dr2'] == 0)
            require(f.o.threads[7].desired['Dr3'] == 0)
            return f.o.summary()
        yield 'null-route-' + M.FAILURES[i], route_case

    def success(sink):
        f = Fixture(sink)
        r = f.route([0, 123, 123, 123, 0x101], 0xabcdef)
        require(r['route'] == 'all-five-prerequisites-passed' and r['outer_result'] == 0xabcdef)
        require(r['values'][-1]['al'] == 1)
        return f.o.summary()
    yield 'all-success-nonzero-outer-low-byte', success

    def zero_after_success(sink):
        f = Fixture(sink)
        r = f.route([0, 123, 123, 123, 1])
        require(r['route'] == 'all-five-prerequisites-passed' and r['outer_result'] == 0)
        # No invented five-way failure when an unobserved later route returns zero.
        return f.o.summary()
    yield 'zero-after-five-successes-preserves-unknown-later-route', zero_after_success

    def divergent(sink):
        f = Fixture(sink)
        r = f.route([123], 77)
        require(r['outer_result'] == 77 and not f.o.cp205())
        return f.o.summary()
    yield 'nonzero-outer-after-witnessed-failure', divergent

    for name, kwargs in (
            ('prefix-path', dict(path=M.SELECTED_PATH + '.extra')),
            ('wrong-caller', dict(marker=BASE + 0x1c99)),
            ('null-path-read-value', dict(path=None)),
            ('case-different-exact-path', dict(path=M.SELECTED_PATH.lower()))):
        def unrelated(sink, kwargs=kwargs):
            f = Fixture(sink)
            f.hit(M.ENTRY, **kwargs)
            f.hit(M.RETURN, 0)
            require(not f.o.closed and f.o.entries == 0)
            require(f.o.rejected == {'unrelated-entry': 1, 'unselected-return': 1})
            return f.o.summary()
        yield name, unrelated

    def early_zero(sink):
        f = Fixture(sink)
        f.hit(M.RETURN, 0)
        require(not f.o.closed and f.o.rejected['unselected-return'] == 1)
        f.route([123])
        require(len(f.o.closed) == 1)
        return f.o.summary()
    yield 'early-unselected-zero-not-evidence', early_zero

    for name, action in (
            ('partial-normal-return', lambda f: f.hit(M.RETURN)),
            ('lost-required-stage', lambda f: f.hit(M.STAGES[1], 1)),
            ('recursive-entry', lambda f: f.hit(M.ENTRY)),
            ('overlapping-unrelated-entry', lambda f: f.hit(M.ENTRY, path='other')),
            ('bad-stage-rsp', lambda f: f.hit(M.STAGES[0], rsp=E)),
            ('return-frame-reused', lambda f: f.hit(M.STAGES[0], marker=0)),
            ('selected-read-error-not-zero', lambda f: f.hit(M.STAGES[0], read_error='short-8-byte-read')),
            ('unexpected-stage-value-type', lambda f: f.hit(M.STAGES[0], value=None)),
            ('unexpected-context-flags', lambda f: f.hit(M.STAGES[0], context_change={'EFlags': 2 ** 40})),
            ('wrong-single-step-chance', lambda f: f.hit(M.STAGES[0], event_change={'first_chance': False})),
            ('wrong-exception-address', lambda f: f.hit(M.STAGES[0], event_change={'exception_address': 5})),
            ('foreign-debug-cause-bs', lambda f: f.hit(M.STAGES[0], debug_change={'Dr6': 0x4004})),
            ('multiple-debug-causes', lambda f: f.hit(M.STAGES[0], debug_change={'Dr6': 5})),
            ('foreign-slot3-state', lambda f: f.hit(M.STAGES[0], debug_change={'Dr3': 99})),
            ('malformed-debug-state', lambda f: f.hit(M.STAGES[0], debug_change={'Dr6': '4'})),
            ('changed-thread-generation', lambda f: f.hit(M.STAGES[0], generation=2)),
            ('changed-creation-time', lambda f: f.hit(M.STAGES[0], event_change={'creation_filetime': '134374890722592590'})),
            ('changed-owned-pid', lambda f: f.hit(M.STAGES[0], event_change={'pid': 99})),
            ('sequence-gap', lambda f: f.hit(M.STAGES[0], event_change={'sequence': 99})),
            ('sequence-replay', lambda f: f.hit(M.STAGES[0], event_change={'sequence': 2})),
            ('backward-monotonic-clock', lambda f: f.hit(M.STAGES[0], event_change={'elapsed_ms': 0})),
            ('deadline-reached', lambda f: f.hit(M.STAGES[0], event_change={'elapsed_ms': 30000})),
            ('unknown-breakpoint-pass-through', lambda f: f.o.unknown(f.event(code=0x80000003))),
            ('selected-thread-exit', lambda f: f.o.exit_thread(f.event())),
            ('wrong-readback', lambda f: f.o.arm_ack(7, 1, ZERO))):
        def negative(sink, action=action):
            f = Fixture(sink)
            f.hit(M.ENTRY)
            rejects(lambda: action(f))
            require(f.o.errors and not f.o.closed and not f.o.cp205())
            return f.o.summary()
        yield name, negative

    def unacked(sink):
        f = Fixture(sink)
        f.hit(M.ENTRY, ack=False)
        rejects(lambda: f.hit(M.STAGES[0]))
        return f.o.summary()
    yield 'stage-before-readback-rejected', unacked

    def cross_thread(sink):
        f = Fixture(sink)
        f.make_thread(8)
        f.hit(M.ENTRY)
        rejects(lambda: f.hit(M.STAGES[0], tid=8))
        require(not f.o.closed)
        return f.o.summary()
    yield 'cross-thread-stage-rejected', cross_thread

    def overlap_thread(sink):
        f = Fixture(sink)
        f.make_thread(8)
        f.hit(M.ENTRY)
        rejects(lambda: f.hit(M.ENTRY, tid=8))
        return f.o.summary()
    yield 'cross-thread-overlap-rejected', overlap_thread

    def reuse(sink):
        f = Fixture(sink)
        f.o.exit_thread(f.event())
        f.make_thread(7, 2)
        rejects(lambda: f.hit(M.ENTRY, generation=1))
        return f.o.summary()
    yield 'tid-reuse-rejects-old-generation', reuse

    for field in M.DEBUG_FIELDS:
        def occupied(sink, field=field):
            f = Fixture(sink)
            state = dict(ZERO)
            state[field] = 1
            rejects(lambda: f.make_thread(8, baseline=state))
            return f.o.summary()
        yield 'preexisting-' + field + '-rejected', occupied

    for name, limits, action in (
            ('event-overflow', dict(event_cap=2), lambda f: f.hit(M.ENTRY)),
            ('entry-overflow', dict(entry_cap=1), lambda f: f.hit(M.ENTRY)),
            ('scalar-overflow', dict(scalar_cap=1), lambda f: (f.hit(M.ENTRY), f.hit(M.STAGES[0])))):
        def capped(sink, limits=limits, action=action):
            f = Fixture(sink, **limits)
            rejects(lambda: action(f))
            require(not f.o.cp205())
            return f.o.summary()
        yield name, capped

    def matching_av(sink):
        f = Fixture(sink)
        f.route([123])
        require(f.av() == M.NOT_HANDLED and f.av(False) == M.NOT_HANDLED)
        require(f.o.cp205())
        require(f.o.summary()['status'] == 'HOLD' and not f.o.summary()['native_cleanup_verified'])
        return f.o.summary()
    yield 'cp205-contract-pair-only-never-native-proof', matching_av

    for name, changes in (
            ('divergent-first-av', dict(address=0x99)),
            ('unqualified-stack', dict(native_unwind_qualified=False)),
            ('unqualified-module', dict(module_identity_qualified=False)),
            ('wrong-stack-path', dict(callback_returns=[0x2a725, 0x1cae, 0])),
            ('wrong-closed-association', dict(closed_occurrence=2))):
        def bad_parity(sink, changes=changes):
            f = Fixture(sink)
            f.route([123])
            f.av(**changes)
            f.av(False, **changes)
            require(not f.o.cp205())
            return f.o.summary()
        yield name, bad_parity

    def av_partial(sink):
        f = Fixture(sink)
        f.hit(M.ENTRY)
        f.av()
        require(f.o.errors and not f.o.closed and not f.o.cp205())
        return f.o.summary()
    yield 'abnormal-selected-av-incomplete', av_partial

    def wrong_av_thread(sink):
        f = Fixture(sink)
        f.make_thread(8)
        f.route([123])
        f.av(tid=8)
        f.av(False, tid=8)
        require(not f.o.cp205())
        return f.o.summary()
    yield 'av-on-other-thread-not-associated', wrong_av_thread

    def changed_pair(sink):
        f = Fixture(sink)
        f.route([123])
        f.av()
        f.av(False, rcx=0x99)
        require(f.o.errors and not f.o.cp205())
        return f.o.summary()
    yield 'av-pair-changed', changed_pair

    def missing_first(sink):
        f = Fixture(sink)
        f.route([123])
        rejects(lambda: f.av(False))
        return f.o.summary()
    yield 'av-last-without-first-rejected', missing_first

    def after_av(sink):
        f = Fixture(sink)
        f.route([123])
        f.av()
        rejects(lambda: f.hit(M.ENTRY))
        return f.o.summary()
    yield 'causal-stages-stopped-after-first-av', after_av

    def ambiguous_closed(sink):
        f = Fixture(sink)
        f.route([123])
        f.route([123])
        f.av()
        f.av(False)
        require(not f.o.cp205())
        return f.o.summary()
    yield 'multiple-closed-entries-indeterminate', ambiguous_closed

    def sink_failure(sink):
        def broken(row):
            raise OSError('synthetic receipt-write failure')
        rejects(lambda: Fixture(broken), OSError)
        sink({'kind': 'fixture-only', 'write_failure_propagated': True})
        return {'status': 'PASS-synthetic-write-failure-propagation'}
    yield 'receipt-write-failure-propagates', sink_failure

    def serializer(sink):
        rejects(lambda: M.JsonlSink(io.StringIO())({'bad': object()}), TypeError)
        rejects(lambda: M.JsonlSink(io.StringIO())({'bad': float('nan')}), ValueError)
        return {'status': 'PASS-serialization-failure-propagation'}
    yield 'receipt-serialization-errors-propagate', serializer

    def guarded(sink):
        require(M.ORIGINAL_RUN_ENABLED is False and M.ORIGINAL_READY is False)
        rejects(M.run_original, M.Hold)
        require(M.TARGET_SHA256 == 'fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc')
        require(M.FIXTURE_DIGEST == '55e2a58cf8155995f942f89ac0ebb91454308b521ae76a228edec0ffdc4cedbd')
        require(M.RETURN == 0x1c94)
        return {'status': 'HOLD-original-disabled', 'original_launched': False}
    yield 'literal-original-guard', guarded

    def cli(sink):
        for argv, status in (([], 0), ([M.ORIGINAL_TOKEN], 2),
                             (['--pid', '1'], 2), (['--exe', 'invalid'], 2),
                             (['--attach'], 2), (['--', 'anything'], 2)):
            out = io.StringIO()
            with redirect_stdout(out):
                require(M.main(argv) == status)
            row = json.loads(out.getvalue())
            require(row['status'] == 'HOLD')
            sink({'kind': 'cli-result', 'argv': argv, 'result': row})
        return {'status': 'HOLD-all-CLI-surfaces-closed'}
    yield 'cli-status-token-and-target-rejections', cli

    def bad_identity(sink):
        rejects(lambda: M.Identity(4242, 134374890722592589, BASE), ValueError)
        rejects(lambda: M.Identity(4242, '134374890722592589', 2 ** 64 - 1), ValueError)
        rejects(lambda: M.FrameContract(None, 'unknown'), ValueError)
        rejects(lambda: M.FrameContract(0x48, ''), ValueError)
        return {'status': 'PASS-malformed-identity-frame-contracts'}
    yield 'exact-filetime-and-frame-contract-required', bad_identity

    def reserved_dr7(sink):
        f = Fixture(sink)
        f.hit(M.ENTRY, debug_change={'Dr7': 0x405})
        f.hit(M.STAGES[0], 123, debug_change={'Dr7': 0x415})
        f.hit(M.RETURN, 0, debug_change={'Dr7': 0x405})
        require(f.o.closed[0]['route'] == 'duplicate-identity')
        return f.o.summary()
    yield 'hardware-dr7-fixed-reserved-bit10-only', reserved_dr7

    def foreign_dr7(sink):
        f = Fixture(sink)
        rejects(lambda: f.hit(M.ENTRY, debug_change={'Dr7': 0x205}))
        return f.o.summary()
    yield 'nonreserved-dr7-change-rejected', foreign_dr7

    def skeleton(sink):
        c = M.FixedOriginalTransport.contract()
        require(c['status'] == 'HOLD' and c['target'] == M.TARGET)
        require(c['target_sha256'] == M.TARGET_SHA256 and c['argv'] == list(M.FIXED_ARGV))
        rejects(M.FixedOriginalTransport.launch, M.Hold)
        require(M.ORIGINAL_RUN_ENABLED is False and M.ORIGINAL_READY is False)
        return {'status': 'HOLD-unconditional-original-native-skeleton'}
    yield 'fixed-original-native-skeleton-never-launches', skeleton


def main():
    if len(sys.argv) != 1:
        raise SystemExit('No target, PID, attach or arbitrary command arguments supported')
    started = time.monotonic()
    results = []
    with RAW.open('w', encoding='utf-8', newline='\n') as file:
        durable = M.JsonlSink(file)
        for name, test in cases():
            def sink(row, name=name):
                durable(dict(case=name, **row))
            sink({'kind': 'case-start'})
            try:
                summary = test(sink)
                results.append(dict(name=name, passed=True, summary=summary))
            except Exception as exc:
                results.append(dict(name=name, passed=False, error=repr(exc)))
            sink({'kind': 'case-end', 'result': results[-1]})
    receipt = dict(schema='cp211-outer-debug-fixture-v1',
                   original_launched=False, gui_control_acquired=False, native_child_launched=False,
                   qualification='Synthetic actual-state-machine input tests; no native API or original observation proof',
                   runner_sha256=hashlib.sha256(Path(M.__file__).read_bytes()).hexdigest(),
                   fixture_sha256=hashlib.sha256(HERE.read_bytes()).hexdigest(),
                   raw_sha256=hashlib.sha256(RAW.read_bytes()).hexdigest(),
                   python=sys.version, elapsed_seconds=round(time.monotonic() - started, 6),
                   passed=sum(r['passed'] for r in results), total=len(results), results=results)
    RECEIPT.write_text(json.dumps(receipt, indent=2, allow_nan=False) + '\n', encoding='utf-8')
    print(json.dumps({k: v for k, v in receipt.items() if k != 'results'}, indent=2))
    if receipt['passed'] != receipt['total']:
        print(json.dumps([r for r in results if not r['passed']], indent=2))
        return 1
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
