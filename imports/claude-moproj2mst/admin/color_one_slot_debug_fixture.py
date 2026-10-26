"""Fixed benign x64 debugger fixture for a single DR0 entry/return handoff.

The only child image is this Python interpreter.  No target path, PID, command,
or arbitrary code address is accepted.  ORIGINAL_RUN_ENABLED is deliberately
false and there is no original-launch branch in this module.

The fixed leaf has no nested entry. With DR0 armed at its dynamic return, a
nested entry at the former entry address would be invisible; this one-slot
surrogate cannot establish original overlap coverage.
"""
import ctypes as C
from ctypes import wintypes as W
import json
import os
from pathlib import Path
import secrets
import subprocess
import sys
import time
import traceback

sys.path.insert(0, str(Path(__file__).resolve().parent))
import child_debug_event_fixture as abi

ORIGINAL_RUN_ENABLED = False
PREFIX = 'COLOR_ONE_SLOT:'
DEBUG_ONLY_THIS_PROCESS = 0x2
CREATE_NO_WINDOW = 0x08000000
DBG_CONTINUE = 0x10002
DBG_EXCEPTION_NOT_HANDLED = 0x80010001
EXCEPTION_SINGLE_STEP = 0x80000004
EXCEPTION_BREAKPOINT = 0x80000003
RF = 0x10000
VIEW = 0x1234567890ABCDEF
MODES = ('normal', 'post-marker-thread', 'duplicate-marker', 'benign-exception', 'timeout')


def _child(token, mode):
    """Two direct calls and ordinary RETs; names describe shape, not app code."""
    alloc = abi.api('VirtualAlloc', abi.P, [abi.P, C.c_size_t, abi.D, abi.D])
    protect = abi.api('VirtualProtect', W.BOOL, [abi.P, C.c_size_t, abi.D, C.POINTER(abi.D)])
    flush = abi.api('FlushInstructionCache', W.BOOL, [abi.P, abi.P, C.c_size_t])
    free = abi.api('VirtualFree', W.BOOL, [abi.P, C.c_size_t, abi.D])
    # call color2d; call color3d; ret; color2d: mov eax,0x2d; ret;
    # six NOPs; color3d: mov eax,0x3d; ret.
    code = bytes.fromhex('e806000000e80d000000c3b82d000000c3' + '90' * 6 + 'b83d000000c3')
    base = alloc(None, 4096, 0x3000, 0x04)
    abi.check(base)
    try:
        C.memmove(base, code, len(code))
        old = abi.D()
        abi.check(protect(base, 4096, 0x20, C.byref(old)))
        abi.check(flush(abi.current(), base, len(code)))
        marker = f'{PREFIX}{token}:{base:x}'.encode('ascii')
        debug_string = abi.api('OutputDebugStringA', None, [W.LPCSTR])
        debug_string(marker)
        if mode == 'duplicate-marker':
            debug_string(marker)
        elif mode == 'post-marker-thread':
            tid = abi.D()
            thread = abi.api('CreateThread', abi.P,
                             [abi.P, C.c_size_t, abi.P, abi.P, abi.D, C.POINTER(abi.D)])(
                                 None, 0, base + 11, None, 0, C.byref(tid))
            abi.check(thread)
            abi.check(abi.close(thread))
        elif mode == 'benign-exception':
            abi.api('RaiseException', None, [abi.D, abi.D, abi.D, abi.P])(
                0xe0424242, 0, 0, None)
        elif mode == 'timeout':
            abi.api('Sleep', None, [abi.D])(750)
        C.CFUNCTYPE(abi.U64, abi.U64)(base)(VIEW)
    finally:
        abi.check(free(base, 0, 0x8000))


def _read_exact(process, address, length):
    buf, got = C.create_string_buffer(length), C.c_size_t()
    abi.check(abi.read(process, address, buf, length, C.byref(got)))
    if got.value != length:
        raise RuntimeError('short child memory read')
    return buf.raw


def _set_dr0(thread, address, saved, *, rf=False):
    raw, ctx = abi.context(thread)
    if ctx.Dr7 & 0x3 and ctx.Dr0 != saved.get('armed', ctx.Dr0):
        raise RuntimeError('DR0 ownership changed')
    ctx.Dr0 = address
    ctx.Dr6 &= ~0xf
    ctx.Dr7 = (ctx.Dr7 & ~0xf0003) | (1 if address else 0)
    if rf:
        ctx.EFlags |= RF
    abi.check(abi.setctx(thread, C.byref(ctx)))
    raw, check = abi.context(thread)
    if (check.Dr0 != address or (check.Dr7 & 0xf0003) != (1 if address else 0)
            or check.Dr6 & 0xe00f):
        raise RuntimeError('DR0/DR6/DR7 readback mismatch')
    if rf and not check.EFlags & RF:
        raise RuntimeError('resume flag readback mismatch')
    saved['armed'] = address
    return dict(dr0=hex(check.Dr0), dr6=hex(check.Dr6), dr7=hex(check.Dr7),
                rf=bool(check.EFlags & RF))


def _restore(thread, saved):
    raw, ctx = abi.context(thread)
    for name in ('Dr0', 'Dr1', 'Dr2', 'Dr3', 'Dr6', 'Dr7'):
        setattr(ctx, name, saved[name])
    ctx.EFlags = (ctx.EFlags & ~RF) | saved['EFlagsRF'] | RF
    abi.check(abi.setctx(thread, C.byref(ctx)))
    raw, after = abi.context(thread)
    if any(getattr(after, n) != saved[n] for n in ('Dr0', 'Dr1', 'Dr2', 'Dr3', 'Dr6', 'Dr7')):
        raise RuntimeError('debug state restore readback mismatch')
    if not after.EFlags & RF:
        raise RuntimeError('restore RF readback mismatch')
    saved['armed'] = None
    return dict(restored=True, rf=bool(after.EFlags & RF))


def run(mode='normal'):
    if os.name != 'nt' or C.sizeof(abi.P) != 8:
        raise RuntimeError('64-bit Windows required')
    if mode not in MODES:
        raise ValueError('only fixed benign modes are supported')
    token = secrets.token_hex(16)
    here = Path(__file__).resolve()
    cmd = subprocess.list2cmdline([sys.executable, '-I', '-S', str(here), '--fixed-child', token, mode])
    si, pi, ev = abi.STARTUPINFO(), abi.PI(), abi.EVENT()
    si.cb = C.sizeof(si)
    result = dict(schema='color-one-slot-v2', mode=mode, originalLaunched=False,
                  originalRunEnabled=ORIGINAL_RUN_ENABLED, guiControlAcquired=False,
                  events=0, continuations=0, hits=[], handles=[], errors=[],
                  exitObserved=False, forcedCleanup=False)
    abi.check(abi.create(sys.executable, C.create_unicode_buffer(cmd), None, None,
                         False, DEBUG_ONLY_THIS_PROCESS | CREATE_NO_WINDOW,
                         None, str(here.parent), C.byref(si), C.byref(pi)))
    result.update(pid=pi.pid, tid=pi.tid)
    live, event_threads, event_process = {}, {}, None
    retained, pending, marked, loader_seen = abi.P(), False, False, False
    base, stage, saved, marked_at = None, 0, {}, None
    started = time.monotonic()

    def own(handle, kind, owner):
        if not handle or handle in live:
            return
        row = dict(generation=len(result['handles']) + 1, handle=hex(handle),
                   kind=kind, owner=owner, closed=False)
        live[handle] = row
        result['handles'].append(row)

    def closed(handle, method):
        row = live.pop(handle, None)
        if row:
            row.update(closed=True, method=method)

    def close_manual(handle):
        if not abi.close(handle):
            raise RuntimeError(f'CloseHandle({hex(handle)}): {C.get_last_error()}')
        closed(handle, 'CloseHandle')

    def resources():
        nonlocal event_process
        if ev.code == 3:
            if ev.tid != pi.tid or event_process is not None:
                raise RuntimeError('create event thread/occurrence mismatch')
            event_process = ev.u.create.process
            own(event_process, 'event-process', 'event')
            own(ev.u.create.thread, 'event-thread', 'event')
            event_threads[ev.tid] = ev.u.create.thread
            if ev.u.create.file:
                own(ev.u.create.file, 'image-file', 'manual')
                close_manual(ev.u.create.file)
        elif ev.code == 2:
            own(ev.u.thread.thread, 'event-thread', 'event')
            event_threads[ev.tid] = ev.u.thread.thread
            result['startupThreads'] = result.get('startupThreads', 0) + 1
            if marked:
                raise RuntimeError('additional thread after marker rejected')
        elif ev.code == 6 and ev.u.load.file:
            own(ev.u.load.file, 'dll-file', 'manual')
            close_manual(ev.u.load.file)

    def continue_event(status):
        nonlocal pending
        ok = bool(abi.cont(ev.pid, ev.tid, status))
        result.setdefault('continuationLedger', []).append(
            dict(code=ev.code, tid=ev.tid, status=hex(status), ok=ok,
                 error=None if ok else C.get_last_error()))
        if not ok:
            raise RuntimeError('ContinueDebugEvent failed')
        result['continuations'] += 1
        pending = False
        if ev.code == 4:
            closed(event_threads.pop(ev.tid, None), 'ContinueDebugEvent:thread-exit')
        elif ev.code == 5:
            closed(event_process, 'ContinueDebugEvent:process-exit')
            for handle in list(event_threads.values()):
                closed(handle, 'ContinueDebugEvent:process-exit')
            event_threads.clear()
            result.update(exitObserved=True, exitCode=hex(ev.u.exitCode))

    own(pi.process, 'caller-process', 'manual')
    own(pi.thread, 'caller-thread', 'manual')
    try:
        abi.check(abi.duplicate(abi.current(), pi.process, abi.current(),
                                C.byref(retained), 0, False, 2))
        own(retained.value, 'retained-process', 'manual')
        abi.check(abi.killonexit(True))
        image, length = C.create_unicode_buffer(32768), abi.D(32768)
        abi.check(abi.queryimage(retained, 0, image, C.byref(length)))
        if Path(image.value).resolve() != Path(sys.executable).resolve():
            raise RuntimeError('child image mismatch')
        result['image'] = image.value
        times = [abi.U64() for _ in range(4)]
        abi.check(abi.gettimes(retained, *(C.byref(t) for t in times)))
        result['creationFiletime'] = str(times[0].value)
        while not result['exitObserved']:
            if result['events'] >= 256 or time.monotonic() - started > 12:
                raise RuntimeError('event/time bound')
            if mode == 'timeout' and marked_at is not None and time.monotonic() - marked_at > 0.25:
                raise RuntimeError('fixed timeout bound')
            if not abi.wait(C.byref(ev), 100):
                if C.get_last_error() == 121:
                    continue
                raise C.WinError(C.get_last_error())
            pending = True
            result['events'] += 1
            if ev.pid != pi.pid:
                raise RuntimeError('foreign child event')
            result['lastEvent'] = ev.code
            resources()
            result['lastPhase'] = 'resources-done'
            status = DBG_CONTINUE
            if ev.code == 8:
                s = ev.u.string
                if not s.unicode and 0 < s.length <= 128:
                    msg = _read_exact(retained, s.address, s.length).rstrip(b'\0').decode('ascii')
                    expected = f'{PREFIX}{token}:'
                    if msg.startswith(expected):
                        if marked or not loader_seen or ev.tid != pi.tid:
                            raise RuntimeError('duplicate marker/reentry signal' if marked else 'marker order/identity mismatch')
                        marked = True
                        marked_at = time.monotonic()
                        base = int(msg[len(expected):], 16)
                        if _read_exact(retained, base, 29) != bytes.fromhex(
                                'e806000000e80d000000c3b82d000000c3' + '90' * 6 + 'b83d000000c3'):
                            raise RuntimeError('fixed child bytes mismatch')
                        raw, ctx = abi.context(pi.thread)
                        saved.update({n: getattr(ctx, n) for n in
                                      ('Dr0', 'Dr1', 'Dr2', 'Dr3', 'Dr6', 'Dr7')})
                        saved['EFlagsRF'] = ctx.EFlags & RF
                        if ctx.Dr7 & 0xff:
                            raise RuntimeError('debug slot already occupied')
                        result['readbacks'] = [_set_dr0(pi.thread, base + 11, saved)]
            elif ev.code == 1:
                ex = ev.u.exception
                raw, ctx = abi.context(pi.thread)
                if (ex.record.code == EXCEPTION_BREAKPOINT and ex.first == 1
                        and not loader_seen and not marked and ev.tid == pi.tid):
                    loader_seen = True
                    result['loaderBreakpoint'] = hex(ex.record.address or 0)
                elif ex.record.code == EXCEPTION_SINGLE_STEP:
                    expected_pc = (base + (11, 5, 23, 10)[stage]) if base is not None and stage < 4 else None
                    if (not marked or ev.tid != pi.tid or ex.first != 1 or
                            ctx.Rip != expected_pc or ex.record.address != expected_pc or
                            ctx.Dr6 & 0xe00f != 1 or ctx.Dr7 & 0xf0003 != 1 or
                            saved.get('armed') != expected_pc):
                        raise RuntimeError('unexpected/retriggered hardware event')
                    record = dict(stage=('2d-entry', '2d-return', '3d-entry', '3d-return')[stage],
                                  tid=ev.tid, rip=hex(ctx.Rip), rsp=hex(ctx.Rsp),
                                  dr6=hex(ctx.Dr6), rfBefore=bool(ctx.EFlags & RF))
                    if stage in (0, 2):
                        if ctx.Rcx != VIEW:
                            raise RuntimeError('entry View mismatch')
                        return_pc = int.from_bytes(_read_exact(retained, ctx.Rsp, 8), 'little')
                        if return_pc != base + (5 if stage == 0 else 10):
                            raise RuntimeError('dynamic return address mismatch')
                        record['returnFromRsp'] = hex(return_pc)
                        record['view'] = hex(ctx.Rcx)
                        record['readback'] = _set_dr0(pi.thread, return_pc, saved, rf=True)
                    else:
                        entry = result['hits'][stage - 1]
                        # RCX survives this fixed MOV EAX/RET leaf. An original
                        # adapter must use its entry-captured View/frame instead.
                        if (ev.tid != entry['tid'] or ctx.Rsp != int(entry['rsp'], 16) + 8 or
                                ctx.Rcx != int(entry['view'], 16) or ctx.Rcx != VIEW):
                            raise RuntimeError('return frame/view mismatch')
                        record['readback'] = (_set_dr0(pi.thread, base + 23, saved, rf=True)
                                              if stage == 1 else _restore(pi.thread, saved))
                    result['hits'].append(record)
                    stage += 1
                else:
                    raise RuntimeError(f'unexpected exception {hex(ex.record.code)}')
            elif ev.code == 5:
                if stage != 4:
                    raise RuntimeError('child exited before both returns')
            continue_event(status)
            result['lastPhase'] = 'continued'
        if abi.waitobj(retained, 2000) != 0:
            raise RuntimeError('retained child not signaled')
        result['ownedProcessSignaled'] = True
    except Exception as exc:
        result.update(error=str(exc), trace=traceback.format_exc(), forcedCleanup=True)
        if retained.value or pi.process:
            ok = bool(abi.terminate(retained.value or pi.process, 0xe231))
            result['termination'] = dict(ok=ok, error=None if ok else C.get_last_error())
        if pending:
            try:
                continue_event(DBG_EXCEPTION_NOT_HANDLED if ev.code == 1 else DBG_CONTINUE)
            except Exception as error:
                result['errors'].append('pending continuation: ' + str(error))
        deadline = time.monotonic() + 3
        while not result['exitObserved'] and time.monotonic() < deadline:
            if not abi.wait(C.byref(ev), 100):
                if C.get_last_error() == 121:
                    continue
                result['errors'].append('cleanup wait: ' + str(C.get_last_error()))
                break
            pending = True
            result['events'] += 1
            try:
                resources()
                continue_event(DBG_EXCEPTION_NOT_HANDLED if ev.code == 1 else DBG_CONTINUE)
            except Exception as error:
                result['errors'].append('cleanup event: ' + str(error))
                break
        result['ownedProcessSignaled'] = abi.waitobj(retained.value or pi.process, 2000) == 0
    finally:
        for handle, row in list(live.items()):
            if row['owner'] == 'manual':
                try:
                    close_manual(handle)
                except Exception as error:
                    result['errors'].append('close: ' + str(error))
        result['logicalHandlesClosed'] = not live and all(r['closed'] for r in result['handles'])
        # CloseHandle success and documented exit-continuation ownership are
        # tracked; invalid-handle probing is intentionally separate/unverified.
        result['osInvalidHandlesVerified'] = False
        result['handlesClosed'] = result['logicalHandlesClosed']
    result['passed'] = bool(not result.get('error') and not result['errors'] and not result['forcedCleanup']
                            and result['exitObserved'] and result.get('exitCode') == '0x0'
                            and result['handlesClosed'] and result.get('ownedProcessSignaled')
                            and result['events'] == result['continuations'] and len(result['hits']) == 4)
    result['negativePassed'] = bool(mode != 'normal' and result['forcedCleanup']
                                    and result['exitObserved'] and result.get('ownedProcessSignaled')
                                    and result['logicalHandlesClosed'] and not result['errors']
                                    and result['events'] == result['continuations'])
    return result


if __name__ == '__main__':
    if (len(sys.argv) == 4 and sys.argv[1] == '--fixed-child' and
            len(sys.argv[2]) == 32 and sys.argv[3] in MODES):
        _child(sys.argv[2], sys.argv[3])
    elif len(sys.argv) == 1:
        outcome = run()
        print(json.dumps(outcome, separators=(',', ':')))
        raise SystemExit(0 if outcome['passed'] else 1)
    else:
        raise SystemExit('Only the fixed benign fixture is supported')
