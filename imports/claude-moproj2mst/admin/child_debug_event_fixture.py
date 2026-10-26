"""Disposable x64 Windows debugger fixture; cannot accept a target image or PID.

Run with no arguments. The sole launch is this Python interpreter, this file,
and a generated child token. Does not attach, read project data, or use GUI APIs.
"""
import ctypes as C
from ctypes import wintypes as W
import hashlib
import json
import os
from pathlib import Path
import secrets
import subprocess
import sys
import time

if os.name != 'nt' or C.sizeof(C.c_void_p) != 8:
    raise SystemExit('64-bit Windows Python required')
K = C.WinDLL('kernel32', use_last_error=True)
U64 = C.c_ulonglong
P = C.c_void_p
D = W.DWORD

class STARTUPINFO(C.Structure):
    _fields_ = [('cb', D), ('reserved', W.LPWSTR), ('desktop', W.LPWSTR),
                ('title', W.LPWSTR), ('x', D), ('y', D), ('xs', D), ('ys', D),
                ('xc', D), ('yc', D), ('fill', D), ('flags', D), ('show', W.WORD),
                ('cbReserved', W.WORD), ('lpReserved', P), ('stdin', P), ('stdout', P), ('stderr', P)]

class PI(C.Structure):
    _fields_ = [('process', P), ('thread', P), ('pid', D), ('tid', D)]

class EXREC(C.Structure):
    _fields_ = [('code', D), ('flags', D), ('record', P), ('address', P),
                ('count', D), ('info', U64 * 15)]

class EXINFO(C.Structure):
    _fields_ = [('record', EXREC), ('first', D)]

class CREATE(C.Structure):
    _fields_ = [('file', P), ('process', P), ('thread', P), ('base', P),
                ('debugOffset', D), ('debugSize', D), ('tls', P), ('start', P),
                ('image', P), ('unicode', W.WORD)]

class LOAD(C.Structure):
    _fields_ = [('file', P), ('base', P), ('offset', D), ('size', D), ('name', P), ('unicode', W.WORD)]

class THREAD(C.Structure):
    _fields_ = [('thread', P), ('tls', P), ('start', P)]

class STRING(C.Structure):
    _fields_ = [('address', P), ('unicode', W.WORD), ('length', W.WORD)]

class UNION(C.Union):
    _fields_ = [('exception', EXINFO), ('create', CREATE), ('load', LOAD),
                ('string', STRING), ('thread', THREAD), ('exitCode', D), ('raw', C.c_ubyte * 160)]

class EVENT(C.Structure):
    _fields_ = [('code', D), ('pid', D), ('tid', D), ('u', UNION)]

class CONTEXT(C.Structure):
    _fields_ = [(f'home{i}', U64) for i in range(6)] + [
        ('ContextFlags', D), ('MxCsr', D)] + [(f'seg{i}', W.WORD) for i in range(6)] + [
        ('EFlags', D)] + [(n, U64) for n in ('Dr0', 'Dr1', 'Dr2', 'Dr3', 'Dr6', 'Dr7',
        'Rax', 'Rcx', 'Rdx', 'Rbx', 'Rsp', 'Rbp', 'Rsi', 'Rdi', 'R8', 'R9', 'R10',
        'R11', 'R12', 'R13', 'R14', 'R15', 'Rip')] + [('tail', C.c_ubyte * (1232 - 256))]

def api(name, result, args):
    f = getattr(K, name)
    f.restype, f.argtypes = result, args
    return f

create = api('CreateProcessW', W.BOOL, [W.LPCWSTR, W.LPWSTR, P, P, W.BOOL, D, P, W.LPCWSTR, C.POINTER(STARTUPINFO), C.POINTER(PI)])
wait = api('WaitForDebugEvent', W.BOOL, [C.POINTER(EVENT), D])
cont = api('ContinueDebugEvent', W.BOOL, [D, D, D])
getctx = api('GetThreadContext', W.BOOL, [P, P])
setctx = api('SetThreadContext', W.BOOL, [P, P])
close = api('CloseHandle', W.BOOL, [P])
read = api('ReadProcessMemory', W.BOOL, [P, P, P, C.c_size_t, C.POINTER(C.c_size_t)])
terminate = api('TerminateProcess', W.BOOL, [P, D])
waitobj = api('WaitForSingleObject', D, [P, D])
killonexit = api('DebugSetProcessKillOnExit', W.BOOL, [W.BOOL])
duplicate = api('DuplicateHandle', W.BOOL, [P, P, P, C.POINTER(P), D, W.BOOL, D])
current = api('GetCurrentProcess', P, [])
queryimage = api('QueryFullProcessImageNameW', W.BOOL, [P, D, W.LPWSTR, C.POINTER(D)])
gettimes = api('GetProcessTimes', W.BOOL, [P, P, P, P, P])
handleinfo = api('GetHandleInformation', W.BOOL, [P, C.POINTER(D)])
openprocess = api('OpenProcess', P, [D, W.BOOL, D])
module = api('GetModuleHandleW', P, [W.LPCWSTR])
procaddr = api('GetProcAddress', P, [P, W.LPCSTR])
finalpath = api('GetFinalPathNameByHandleW', D, [P, W.LPWSTR, D, D])

def check(ok):
    if not ok:
        raise C.WinError(C.get_last_error())

def context(thread):
    # Windows x64 CONTEXT requires 16-byte alignment.
    raw = C.create_string_buffer(1232 + 15)
    ctx = CONTEXT.from_address((C.addressof(raw) + 15) & ~15)
    ctx.ContextFlags = 0x100013  # AMD64 CONTROL | INTEGER | DEBUG_REGISTERS
    check(getctx(thread, C.byref(ctx)))
    return raw, ctx

def fixture(token, mode):
    api('SetErrorMode', D, [D])(0x0002 | 0x8000)
    alloc = api('VirtualAlloc', P, [P, C.c_size_t, D, D])
    addr = alloc(None, 4096, 0x3000, 0x04)
    check(addr)
    code = bytes.fromhex('48b88877665544332211c3')
    av = bytes.fromhex('48c7c158000000488b01c3')
    C.memmove(addr, code + av, len(code + av))
    old = D()
    check(api('VirtualProtect', W.BOOL, [P, C.c_size_t, D, C.POINTER(D)])(addr, 4096, 0x20, C.byref(old)))
    check(api('FlushInstructionCache', W.BOOL, [P, P, C.c_size_t])(current(), addr, len(code + av)))
    msg = f'CP209:{token}:{addr:x}:{mode}'.encode('ascii')
    api('OutputDebugStringA', None, [W.LPCSTR])(msg)
    value = C.CFUNCTYPE(U64)(addr)()
    if value != 0x1122334455667788:
        os._exit(4)
    if mode == 'av':
        # ctypes catches synchronous native faults on its calling thread. A native
        # fixture thread has no ctypes SEH wrapper and therefore reaches last chance.
        tid = D()
        worker = api('CreateThread', P, [P, C.c_size_t, P, P, D, C.POINTER(D)])(
            None, 0, addr + len(code), None, 0, C.byref(tid))
        check(worker)
        waitobj(worker, 5000)
        close(worker)
        os._exit(5)  # the AV fixture should never return
    check(api('VirtualFree', W.BOOL, [P, C.c_size_t, D])(addr, 0, 0x8000))

def probe_pid(pid, seconds=2):
    attempts, deadline = [], time.monotonic() + seconds
    while True:
        handle = openprocess(0x00100000 | 0x1000, False, pid)
        if not handle:
            error = C.get_last_error()
            attempts.append(dict(openError=error))
            return dict(absent=error == 87, attempts=attempts)
        state = waitobj(handle, 0)
        closed = bool(close(handle))
        attempts.append(dict(wait=state, closed=closed))
        if not closed or time.monotonic() >= deadline or state != 0:
            return dict(absent=False, attempts=attempts)
        time.sleep(0.025)


def run(mode, fault=None, seed=False):
    token, here = secrets.token_hex(16), Path(__file__).resolve()
    si, pi, event = STARTUPINFO(), PI(), EVENT()
    si.cb = C.sizeof(si)
    command = subprocess.list2cmdline([sys.executable, '-I', '-S', str(here), '--fixture', token, mode])
    report = dict(mode=mode, injection=fault, seededDebugState=seed, originalLaunched=False, guiControlAcquired=False,
                  events=0, continues=0, breakpointHits=[], accessViolations=[], exceptions=[],
                  forcedCleanup=False, exitObserved=False, handleLedger=[], continuationLedger=[],
                  counters=dict(loader=0, hardware=0, avFirst=0, avLast=0, unknown=0), cleanupErrors=[])
    check(create(sys.executable, C.create_unicode_buffer(command), None, None, False,
                 0x00000002 | 0x08000000, None, str(here.parent), C.byref(si), C.byref(pi)))
    report.update(pid=pi.pid, primaryTid=pi.tid)
    retained, pending = P(), False
    threads, live, event_threads, event_process = {}, {}, {}, None
    breakaddr, ntdll, saved, armed, marker_seen = None, None, None, False, False
    before_seed = None
    started = time.monotonic()
    # This Windows build's loader uses a private ntdll breakpoint, not the
    # exported DbgBreakPoint. Fail closed on any host-image/site change.
    break_rva = 0x1226c9
    ntdll_sha = '651ac9041af8e6b5ebd331d53163244c0a27d36d1433f0f15cac1f28c5f2e82c'
    loader_bytes = bytes.fromhex('0a807c2440007503cceb004883c438c3cc')

    def inject(point):
        if fault == point:
            report['injectionReached'] = True
            raise RuntimeError('fixture-injected:' + point)

    def own(handle, kind, owner):
        if not handle:
            return None
        if handle in live:
            # A documented alias refers to the same currently live handle generation.
            return live[handle]
        row = dict(generation=len(report['handleLedger']) + 1, value=hex(handle), kind=kind, owner=owner, closed=False)
        report['handleLedger'].append(row)
        live[handle] = row
        return row

    def closed(handle, how):
        row = live.pop(handle, None)
        if row is not None:
            row.update(closed=True, closure=how)
            flags = D()
            row['invalidImmediately'] = not handleinfo(handle, C.byref(flags)) and C.get_last_error() == 6

    def close_checked(handle):
        ok = bool(close(handle))
        if ok:
            closed(handle, 'CloseHandle')
        else:
            report['cleanupErrors'].append(dict(api='CloseHandle', handle=hex(handle), error=C.get_last_error()))
        return ok

    own(pi.process, 'caller-process', 'caller')
    own(pi.thread, 'caller-thread', 'caller')

    def resources():
        nonlocal event_process, ntdll
        # Called immediately for every delivered event, including failure-drain events.
        if event.code == 3:
            event_process = event.u.create.process
            own(event_process, 'event-process', 'event')
            own(event.u.create.thread, 'event-thread', 'event')
            event_threads[event.tid] = event.u.create.thread
            threads[event.tid] = event.u.create.thread
            if event.u.create.file:
                own(event.u.create.file, 'image-file', 'manual')
                if not close_checked(event.u.create.file):
                    raise RuntimeError('image handle close failed')
        elif event.code == 2:
            own(event.u.thread.thread, 'event-thread', 'event')
            event_threads[event.tid] = event.u.thread.thread
            threads[event.tid] = event.u.thread.thread
            report['additionalThreadCount'] = report.get('additionalThreadCount', 0) + 1
            if breakaddr and event.u.thread.start == breakaddr + 1:
                report['expectedAvTid'] = event.tid
        elif event.code == 6 and event.u.load.file:
            own(event.u.load.file, 'dll-file', 'manual')
            path = C.create_unicode_buffer(32768)
            count = finalpath(event.u.load.file, path, len(path), 0)
            if count and count < len(path) and Path(path.value).name.lower() == 'ntdll.dll':
                ntdll = event.u.load.base
                digest = hashlib.sha256(Path(path.value).read_bytes()).hexdigest()
                if digest != ntdll_sha:
                    raise RuntimeError('fixture host ntdll hash changed')
                buf, got = C.create_string_buffer(len(loader_bytes)), C.c_size_t()
                check(read(retained, ntdll + break_rva - 8, buf, len(loader_bytes), C.byref(got)))
                if got.value != len(loader_bytes) or buf.raw != loader_bytes:
                    raise RuntimeError('fixture loader breakpoint bytes changed')
                report['loaderModule'] = dict(path=path.value, sha256=digest, base=hex(ntdll), rva=hex(break_rva), bytes=buf.raw.hex())
                report['loaderBreakpointExpected'] = hex(ntdll + break_rva)
            if not close_checked(event.u.load.file):
                raise RuntimeError('DLL handle close failed')

    def continuation(status, cleanup=False):
        nonlocal pending
        ok = bool(cont(event.pid, event.tid, status))
        report['continuationLedger'].append(dict(code=event.code, tid=event.tid, disposition=hex(status), cleanup=cleanup, ok=ok,
                                                 error=None if ok else C.get_last_error()))
        if not ok:
            raise RuntimeError('ContinueDebugEvent failed')
        pending = False
        report['continues'] += 1
        if event.code == 4:
            handle = event_threads.pop(event.tid, None)
            if handle:
                closed(handle, 'ContinueDebugEvent:thread-exit')
            threads.pop(event.tid, None)
        elif event.code == 5:
            if event_process:
                closed(event_process, 'ContinueDebugEvent:process-exit')
            for handle in list(event_threads.values()):
                closed(handle, 'ContinueDebugEvent:process-exit')
            threads.clear()
            event_threads.clear()
            report.update(exitObserved=True, exitCode=hex(event.u.exitCode))
            if cleanup:
                report['cleanupExitObserved'] = True

    def restore(thread):
        nonlocal armed
        raw, ctx = context(thread)
        for name, value in saved.items():
            if name != 'resumeFlag':
                setattr(ctx, name, value)
        ctx.EFlags = (ctx.EFlags & ~0x10000) | saved['resumeFlag']
        check(setctx(thread, C.byref(ctx)))
        raw2, after = context(thread)
        observed = {n: getattr(after, n) for n in saved if n != 'resumeFlag'}
        observed['resumeFlag'] = after.EFlags & 0x10000
        report['debugStateAfterRestore'] = observed
        report['debugStateRestored'] = observed == saved
        check(report['debugStateRestored'])
        if before_seed is not None:
            for name, value in before_seed.items():
                if name != 'resumeFlag':
                    setattr(after, name, value)
            after.EFlags = (after.EFlags & ~0x10000) | before_seed['resumeFlag']
            check(setctx(thread, C.byref(after)))
            raw3, final = context(thread)
            final_state = {n: getattr(final, n) for n in before_seed if n != 'resumeFlag'}
            final_state['resumeFlag'] = final.EFlags & 0x10000
            report['fixtureSeedRestored'] = final_state == before_seed
            check(report['fixtureSeedRestored'])
        armed = False

    try:
        check(duplicate(current(), pi.process, current(), C.byref(retained), 0, False, 2))
        own(retained.value, 'retained-process', 'caller')
        check(killonexit(True))
        image, size = C.create_unicode_buffer(32768), D(32768)
        check(queryimage(retained, 0, image, C.byref(size)))
        if Path(image.value).resolve() != Path(sys.executable).resolve():
            raise RuntimeError('owned child image mismatch')
        times = [U64() for _ in range(4)]
        check(gettimes(retained, *(C.byref(t) for t in times)))
        report.update(creationFiletime=str(times[0].value), image=image.value)
        inject('before-create-event')
        while not report['exitObserved']:
            if time.monotonic() - started > 15 or report['events'] >= 256:
                raise RuntimeError('bounded event budget exceeded')
            if not wait(C.byref(event), 200):
                if C.get_last_error() == 121:
                    continue
                check(False)
            pending = True
            report['events'] += 1
            if event.pid != pi.pid:
                raise RuntimeError('unexpected process debug event')
            resources()
            status = 0x00010002
            if event.code == 3:
                inject('after-create-event')
            elif event.code == 2:
                inject('after-thread-event')
            elif event.code == 8:
                s = event.u.string
                if not s.unicode and 0 < s.length <= 256:
                    buf, n = C.create_string_buffer(s.length), C.c_size_t()
                    check(read(retained, s.address, buf, s.length, C.byref(n)))
                    if n.value != s.length:
                        raise RuntimeError('short debug-string read')
                    text = buf.raw.rstrip(b'\0').decode('ascii', errors='replace')
                    prefix = f'CP209:{token}:'
                    if text.startswith(prefix):
                        inject('marker-before-context')
                        parts = text[len(prefix):].split(':')
                        if len(parts) != 2 or parts[1] != mode or marker_seen or event.tid != pi.tid or report['counters']['loader'] != 1:
                            raise RuntimeError('fixture marker lifecycle mismatch')
                        marker_seen = True
                        breakaddr = int(parts[0], 16) + 10
                        raw, ctx = context(threads[pi.tid])
                        if seed:
                            before_seed = {n: getattr(ctx, n) for n in ('Dr0', 'Dr1', 'Dr2', 'Dr3', 'Dr6', 'Dr7')}
                            before_seed['resumeFlag'] = ctx.EFlags & 0x10000
                            if ctx.Dr7 & 0xff:
                                raise RuntimeError('cannot seed occupied debug slots')
                            ctx.Dr0, ctx.Dr1, ctx.Dr2 = 0x111122223333, 0x222233334444, 0x333344445555
                            # Preserve a disabled slot's valid write-control field;
                            # otherwise Windows may canonicalize all inactive DRs to 0.
                            ctx.Dr7 = (ctx.Dr7 & ~0xf00000) | 0x100000
                            check(setctx(threads[pi.tid], C.byref(ctx)))
                            raw, ctx = context(threads[pi.tid])
                            report['seedReadback'] = {n: hex(getattr(ctx, n)) for n in ('Dr0', 'Dr1', 'Dr2', 'Dr7')}
                            if (ctx.Dr0, ctx.Dr1, ctx.Dr2) != (0x111122223333, 0x222233334444, 0x333344445555):
                                for name, value in before_seed.items():
                                    if name != 'resumeFlag':
                                        setattr(ctx, name, value)
                                ctx.EFlags = (ctx.EFlags & ~0x10000) | before_seed['resumeFlag']
                                check(setctx(threads[pi.tid], C.byref(ctx)))
                                raw, reset = context(threads[pi.tid])
                                reset_state = {n: getattr(reset, n) for n in before_seed if n != 'resumeFlag'}
                                reset_state['resumeFlag'] = reset.EFlags & 0x10000
                                report['fixtureSeedRestored'] = reset_state == before_seed
                                raise RuntimeError('fixture seed was not retained by Windows')
                            report['debugStateBeforeFixtureSeed'] = before_seed.copy()
                        saved = {n: getattr(ctx, n) for n in ('Dr0', 'Dr1', 'Dr2', 'Dr3', 'Dr6', 'Dr7')}
                        saved['resumeFlag'] = ctx.EFlags & 0x10000
                        report['debugStateBeforeArm'] = saved.copy()
                        if ctx.Dr7 & 3:
                            raise RuntimeError('DR0 already owned')
                        # Leave DR1-3 and all unrelated DR7 control bits unchanged.
                        ctx.Dr0, ctx.Dr6 = breakaddr, ctx.Dr6 & ~1
                        ctx.Dr7 = (ctx.Dr7 & ~0xf0003) | 1
                        inject('context-before-set')
                        check(setctx(threads[pi.tid], C.byref(ctx)))
                        armed = True
                        report['breakpointAddress'] = hex(breakaddr)
                        inject('after-arm')
            elif event.code == 1:
                ex = event.u.exception
                status = 0x80010001
                raw, ctx = context(threads[event.tid])
                snapshot = {n: hex(getattr(ctx, n)) for n in ('Rip', 'Rax', 'Rcx', 'Rdx', 'R11', 'Rsp', 'Dr6', 'Dr7')}
                snapshot.update(firstChance=bool(ex.first), tid=event.tid, code=hex(ex.record.code), exceptionAddress=hex(ex.record.address or 0))
                if (ex.record.code == 0x80000003 and ex.first == 1 and event.tid == pi.tid
                        and not marker_seen and report['counters']['loader'] == 0 and ntdll
                        and ex.record.address == ntdll + break_rva and ctx.Rip == ex.record.address + 1):
                    report['counters']['loader'] += 1
                    snapshot['classification'] = 'qualified-loader-breakpoint'
                    status = 0x00010002
                elif (ex.record.code == 0x80000004 and ex.first == 1 and event.tid == pi.tid
                        and armed and breakaddr and ctx.Rip == breakaddr and ex.record.address == breakaddr
                        and ctx.Dr6 & 0xf == 1 and report['counters']['hardware'] == 0):
                    report['counters']['hardware'] += 1
                    snapshot['classification'] = 'owned-hardware-breakpoint'
                    report['breakpointHits'].append(snapshot.copy())
                    restore(threads[pi.tid])
                    status = 0x00010002
                elif ex.record.code == 0xc0000005:
                    report['counters']['avFirst' if ex.first else 'avLast'] += 1
                    snapshot.update(classification='application-av', parameterCount=ex.record.count,
                                    operation=int(ex.record.info[0]), address=hex(ex.record.info[1]))
                    report['accessViolations'].append(snapshot.copy())
                else:
                    report['counters']['unknown'] += 1
                    snapshot['classification'] = 'unknown-pass-through'
                snapshot['disposition'] = hex(status)
                report['exceptions'].append(snapshot)
            elif event.code == 5:
                inject('before-exit-continuation')
            continuation(status)
        check(waitobj(retained, 2000) == 0)
        report['ownedProcessSignaled'] = True
    except Exception as exc:
        report.update(error=str(exc), forcedCleanup=True)
        if armed and pending and pi.tid in threads:
            try:
                restore(threads[pi.tid])
            except Exception as restore_error:
                report['cleanupErrors'].append(dict(api='restore-debug-state', error=str(restore_error)))
        ok = bool(terminate(retained.value or pi.process, 0xe210))
        report['termination'] = dict(ok=ok, error=None if ok else C.get_last_error())
        if pending:
            try:
                continuation(0x80010001 if event.code == 1 else 0x00010002, True)
            except Exception as continue_error:
                report['cleanupErrors'].append(dict(api='pending-continuation', error=str(continue_error)))
        until = time.monotonic() + 3
        while not report['exitObserved'] and time.monotonic() < until:
            if not wait(C.byref(event), 100):
                if C.get_last_error() != 121:
                    report['cleanupErrors'].append(dict(api='WaitForDebugEvent', error=C.get_last_error()))
                    break
                continue
            pending = True
            report['events'] += 1
            try:
                resources()
                if event.code == 1:
                    report['counters']['unknown'] += 1
                    report['exceptions'].append(dict(code=hex(event.u.exception.record.code), tid=event.tid,
                        firstChance=bool(event.u.exception.first), classification='cleanup-pass-through', disposition='0x80010001'))
                continuation(0x80010001 if event.code == 1 else 0x00010002, True)
            except Exception as drain_error:
                report['cleanupErrors'].append(dict(api='drain', error=str(drain_error)))
                break
        report['ownedProcessSignaled'] = waitobj(retained.value or pi.process, 2000) == 0
    finally:
        # Event exit continuation closes event-owned handles; manual/caller handles
        # are separately closed. Never infer closure solely from a numeric set.
        for handle, row in list(live.items()):
            if row['owner'] != 'event':
                close_checked(handle)
        report['handlesClosed'] = not live and all(row['closed'] and row['invalidImmediately'] for row in report['handleLedger'])
        report['liveHandleGenerations'] = [row['generation'] for row in live.values()]
    avs, counts = report['accessViolations'], report['counters']
    pair_equal = len(avs) == 2 and {k: v for k, v in avs[0].items() if k != 'firstChance'} == {k: v for k, v in avs[1].items() if k != 'firstChance'}
    av_ok = (pair_equal and [a['firstChance'] for a in avs] == [True, False] and report.get('expectedAvTid')
             and all(a['Rcx'] == '0x58' and a['address'] == '0x58' and a['operation'] == 0
                     and a['parameterCount'] == 2 and a['tid'] == report['expectedAvTid']
                     and int(a['Rip'], 16) == breakaddr + 8 for a in avs))
    natural = (not report.get('error') and not report['forcedCleanup'] and report['exitObserved']
               and report['handlesClosed'] and report.get('ownedProcessSignaled') and not report['cleanupErrors']
               and report['events'] == report['continues'] and counts['loader'] == counts['hardware'] == 1
               and counts['unknown'] == 0 and report.get('debugStateRestored')
               and (not seed or report.get('fixtureSeedRestored'))
               and len(report['breakpointHits']) == 1 and report['breakpointHits'][0]['Rax'] == '0x1122334455667788'
               and ((mode == 'normal' and not avs and counts['avFirst'] == counts['avLast'] == 0 and report['exitCode'] == '0x0')
                    or (mode == 'av' and av_ok and counts['avFirst'] == counts['avLast'] == 1 and report['exitCode'] == '0xc0000005')))
    cleanup = (fault and report.get('injectionReached') and report['forcedCleanup'] and report['exitObserved']
               and report['handlesClosed'] and report.get('ownedProcessSignaled') and not report['cleanupErrors']
               and report['events'] == report['continues'] and (not saved or report.get('debugStateRestored') or fault == 'context-before-set'))
    rejected_seed = (seed and report.get('error') == 'fixture seed was not retained by Windows'
                     and report.get('fixtureSeedRestored') and report['forcedCleanup'] and report['exitObserved']
                     and report['handlesClosed'] and report.get('ownedProcessSignaled') and not report['cleanupErrors']
                     and report['events'] == report['continues'])
    report['expectedOutcome'] = 'injected-cleanup' if fault else ('seed-preservation-or-explicit-rejection' if seed else 'natural-fixture')
    report['casePassedBeforeIndependentAbsence'] = bool(cleanup if fault else natural or rejected_seed)
    report['elapsedSeconds'] = round(time.monotonic() - started, 3)
    return report


def suite():
    cases = [run('normal'), run('av'), run('normal', seed=True)]
    for fault in ('before-create-event', 'after-create-event', 'after-thread-event', 'marker-before-context',
                  'context-before-set', 'after-arm', 'before-exit-continuation'):
        cases.append(run('normal', fault))
    return dict(schema='child-debug-event-fixture-v2', python=sys.version,
                scriptSha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest(), results=cases)


if __name__ == '__main__':
    if len(sys.argv) == 4 and sys.argv[1] == '--fixture' and len(sys.argv[2]) == 32 and sys.argv[3] in ('normal', 'av'):
        fixture(sys.argv[2], sys.argv[3])
    elif sys.argv[1:] == ['--debugger-suite']:
        if C.sizeof(EVENT) != 176 or C.sizeof(CONTEXT) != 1232 or CONTEXT.Rip.offset != 248:
            raise SystemExit('Windows ABI layout mismatch')
        print(json.dumps(suite(), indent=2))
    elif len(sys.argv) == 1:
        # The Windows debug object can retain terminated-process references until
        # the debugger exits. Observe absence from this separate non-debugger parent.
        worker = subprocess.run([sys.executable, '-I', '-S', str(Path(__file__).resolve()), '--debugger-suite'],
                                capture_output=True, text=True, timeout=55, creationflags=0x08000000)
        receipt = json.loads(worker.stdout)
        receipt.update(debuggerExited=True, debuggerExitCode=worker.returncode, debuggerStderr=worker.stderr)
        for result in receipt['results']:
            result['independentPidAbsence'] = probe_pid(result['pid'])
            result['pass'] = bool(result['casePassedBeforeIndependentAbsence'] and result['independentPidAbsence']['absent'])
        receipt['pass'] = worker.returncode == 0 and not worker.stderr and all(r['pass'] for r in receipt['results'])
        print(json.dumps(receipt, indent=2))
        raise SystemExit(0 if receipt['pass'] else 1)
    else:
        raise SystemExit('No target/attach arguments supported')
