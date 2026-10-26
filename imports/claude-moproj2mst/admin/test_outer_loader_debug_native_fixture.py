"""Fixed harmless native integration for CP211. No original/GUI target support.

No arguments runs a bounded suite in separate owning debugger subprocesses.
--native-child and --debugger-case are internal fixed fixture roles only.
"""
import hashlib
import importlib.util
import json
import secrets
from pathlib import Path
import subprocess
import sys
import time

HERE = Path(__file__).resolve()
ROOT = HERE.parent.parent
SPEC = importlib.util.spec_from_file_location('cp211_native_observer', HERE.with_name('original_outer_loader_debug_probe.py'))
M = importlib.util.module_from_spec(SPEC)
sys.modules[SPEC.name] = M
SPEC.loader.exec_module(M)
OUT = ROOT / 'analysis/resume211-outer-debug-native-fixture.json'


def child(token, mode):
    A = M.load_cp210_abi()
    C, P, D = A.C, A.P, A.D
    A.api('SetErrorMode', D, [D])(0x0002 | 0x8000)
    alloc = A.api('VirtualAlloc', P, [P, C.c_size_t, D, D])
    base = alloc(M.NATIVE_FIXTURE_BASE, 0x30000, 0x3000, 0x04)
    A.check(base == M.NATIVE_FIXTURE_BASE)
    spans = M.native_fixture_image(mode)
    for rva, data in spans.items():
        C.memmove(base + rva, data, len(data))
    old = D()
    A.check(A.api('VirtualProtect', A.W.BOOL, [P, C.c_size_t, D, C.POINTER(D)])(
        base, 0x30000, 0x20, C.byref(old)))
    A.check(A.api('FlushInstructionCache', A.W.BOOL, [P, P, C.c_size_t])(A.current(), base, 0x30000))
    out = A.api('OutputDebugStringA', None, [A.W.LPCSTR])
    out(('CP211:' + token + ':' + mode).encode('ascii'))
    value = C.CFUNCTYPE(A.U64)(base + 0x1c75)()
    if value != (0 if mode == 'duplicate' else 0x4444):
        raise SystemExit('Native fixture ordinary return value changed')
    if mode == 'av':
        tid = D()
        handle = A.api('CreateThread', P, [P, C.c_size_t, P, P, D, C.POINTER(D)])(
            None, 0, base + 0x22000, None, 0, C.byref(tid))
        A.check(handle)
        # No ctypes SEH wrapper on this native thread: preserve both AV chances.
        A.waitobj(handle, 5000)
        A.close(handle)
        raise SystemExit('Expected natural AV did not terminate native fixture')
    out(('CP211-DONE:' + token).encode('ascii'))
    A.check(A.api('VirtualFree', A.W.BOOL, [P, C.c_size_t, D])(base, 0, 0x8000))


def debugger_case(mode, fault):
    raw = ROOT / ('analysis/resume211-outer-debug-native-' + mode + '-' + (fault or 'natural')
                  + '-' + secrets.token_hex(4) + '.jsonl')
    with raw.open('w', encoding='utf-8', newline='\n') as file:
        report = M.run_harmless_native_fixture(mode, fault, M.JsonlSink(file))
    report.update(raw_path=str(raw), raw_sha256=hashlib.sha256(raw.read_bytes()).hexdigest(),
                  native_fixture_sha256=hashlib.sha256(HERE.read_bytes()).hexdigest())
    print(json.dumps(report, allow_nan=False))


def suite():
    A = M.load_cp210_abi()
    sources = ROOT / 'analysis/resume211-outer-debug-native-sources'
    sources.mkdir(exist_ok=True)
    source_snapshots = []
    for path in (Path(M.__file__), HERE, HERE.with_name('child_debug_event_fixture.py')):
        content = path.read_bytes()
        sha = hashlib.sha256(content).hexdigest()
        snapshot = sources / (sha + '-' + path.name)
        if not snapshot.exists():
            snapshot.write_bytes(content)
        if hashlib.sha256(snapshot.read_bytes()).hexdigest() != sha:
            raise RuntimeError('Retained native fixture source snapshot changed')
        source_snapshots.append(dict(path=str(snapshot), sha256=sha))
    cases = [('normal', None), ('duplicate', None), ('av', None)]
    cases.extend(('normal', fault) for fault in M.NATIVE_FIXTURE_FAULTS if fault is not None)
    results = []
    started = time.monotonic()
    for mode, fault in cases:
        worker = subprocess.run([sys.executable, '-I', '-S', str(HERE), '--debugger-case', mode, fault or 'none'],
                                capture_output=True, text=True, timeout=45, creationflags=0x08000000)
        try:
            report = json.loads(worker.stdout)
        except Exception as exc:
            results.append(dict(mode=mode, fault=fault, passed=False, debugger_exit=worker.returncode,
                                debugger_stderr=worker.stderr, error=repr(exc), bounded_stdout=worker.stdout[:2000]))
            break
        report.update(debugger_exit=worker.returncode, debugger_stderr=worker.stderr,
                      independent_pid_absence=A.probe_pid(report['pid']))
        report['passed'] = bool(worker.returncode == 0 and not worker.stderr
                                and report['expected_case_passed_before_absence']
                                and report['independent_pid_absence']['absent'])
        results.append(report)
        # A failed fixture releases no original gate. Preserve its full receipt
        # and stop this debugging iteration before spending on later cases.
        if not report['passed']:
            break
    receipt = dict(schema='cp211-outer-debug-native-fixture-v1', original_launched=False,
                   gui_control_acquired=False, original_ready=False,
                   qualification='Harmless fixed native child/actual observer integration only; original HOLD',
                   observer_sha256=hashlib.sha256(Path(M.__file__).read_bytes()).hexdigest(),
                   fixture_sha256=hashlib.sha256(HERE.read_bytes()).hexdigest(),
                   cp210_abi_sha256=M.CP210_ABI_SHA256, python=sys.version,
                   source_snapshots=source_snapshots,
                   planned_cases=len(cases), completed_cases=len(results), passed=sum(r['passed'] for r in results),
                   elapsed_seconds=round(time.monotonic() - started, 6), results=results)
    receipt['suite_passed'] = len(results) == len(cases) and all(r['passed'] for r in results)
    OUT.write_text(json.dumps(receipt, indent=2, allow_nan=False) + '\n', encoding='utf-8')
    print(json.dumps({k: v for k, v in receipt.items() if k != 'results'}, indent=2))
    for r in results:
        print(json.dumps({k: r.get(k) for k in ('mode', 'fault', 'passed', 'error', 'events', 'continues',
                                                 'debug_state_restored', 'handles_closed', 'exit_code', 'cleanup_errors')}))
    return 0 if receipt['suite_passed'] else 1


def main():
    args = sys.argv[1:]
    if not args:
        return suite()
    if (len(args) == 3 and args[0] == '--native-child' and len(args[1]) == 32
            and all(c in '0123456789abcdef' for c in args[1]) and args[2] in M.NATIVE_FIXTURE_MODES):
        child(args[1], args[2])
        return 0
    if len(args) == 3 and args[0] == '--debugger-case' and args[1] in M.NATIVE_FIXTURE_MODES:
        fault = None if args[2] == 'none' else args[2]
        if fault in M.NATIVE_FIXTURE_FAULTS:
            debugger_case(args[1], fault)
            return 0
    raise SystemExit('No target, PID, attach or arbitrary command arguments supported')


if __name__ == '__main__':
    raise SystemExit(main())
