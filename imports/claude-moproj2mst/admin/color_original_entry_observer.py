"""CP281 source-stable harmless entry-observer increment; ORIGINAL IS HOLD.

Only this interpreter and this file's fixed child can be created. The disabled
original binding intentionally has no launch implementation. No attach API,
target command/address option, target code write or GUI API exists here.
The child alone allocates its immutable native surrogate before announcing it.
"""
import ctypes as C
from ctypes import wintypes as W
import hashlib
import json
import os
from pathlib import Path
import secrets
import struct
import subprocess
import sys
import time

sys.path.insert(0, str(Path(__file__).resolve().parent))
import child_debug_event_fixture as abi

ORIGINAL_RUN_ENABLED = False
ORIGINAL_BINDING = dict(image='DEF_GUI_MO_64.exe', installation='v11.0.2',
    sha256='fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc',
    callRva=0x11f2482, entryRva=0x11f26e0, returnRva=0x11f2487,
    callHex='e859020000', prefixHex='48894c2408534881ecc0000000488b05')
PREFIX = bytes.fromhex(ORIGINAL_BINDING['prefixHex'])
MARKER = 'CP281_HARMLESS:'
MODES = ('normal', 'handled', 'target-int3', 'wrong-caller', 'worker-only',
         'extra-thread', 'natural-exit', 'timeout', 'unhandled')
CONTINUE, FORWARD = 0x10002, 0x80010001
STEP, BREAK = 0x80000004, 0x80000003
RF = 0x10000
MAX_EVENTS = 512
MAX_RECEIPT = 256 * 1024
NTDLL_SHA = '651ac9041af8e6b5ebd331d53163244c0a27d36d1433f0f15cac1f28c5f2e82c'
LOADER_RVA = 0x1226c9
LOADER_BYTES = bytes.fromhex('0a807c2440007503cceb004883c438c3cc')
REGS = ('Rip', 'Rsp', 'Rcx', 'Rdx', 'R8', 'R9', 'EFlags',
        'Dr0', 'Dr1', 'Dr2', 'Dr3', 'Dr6', 'Dr7')


def run_original():
    # Literal refusal, even if someone changes the informational constant.
    raise RuntimeError('HOLD: original launch is not implemented or released')


def native_code():
    """Fixed x64 wrappers own shadow space; helper preserves nonvolatile RBX."""
    code = bytearray(b'\x90' * 128)
    for offset in (0, 16):
        code[offset:offset + 14] = (bytes.fromhex('4883ec28e8') +
            struct.pack('<i', 64 - (offset + 9)) + bytes.fromhex('4883c428c3'))
    # Exact original 16-byte identity prefix, completed RIP-relative load to
    # readable RX memory; remainder is a harmless synthetic epilogue.
    leaf = PREFIX + b'\0' * 4 + bytes.fromhex('31c04881c4c00000005bc3')
    code[64:64 + len(leaf)] = leaf
    code[112:114] = b'\x0f\x0b'  # unhandled UD2 on a native worker
    return bytes(code)


def _child(token, mode):
    abi.api('SetErrorMode', abi.D, [abi.D])(0x8002)
    alloc = abi.api('VirtualAlloc', abi.P, [abi.P, C.c_size_t, abi.D, abi.D])
    base = alloc(None, 4096, 0x3000, 4)
    abi.check(base)
    object_storage, view = C.c_uint64(0x281), (C.c_uint64 * 4)()
    view[3] = C.addressof(object_storage)
    code = native_code()
    C.memmove(base, code, len(code))
    old = abi.D()
    abi.check(abi.api('VirtualProtect', W.BOOL,
        [abi.P, C.c_size_t, abi.D, C.POINTER(abi.D)])(base, 4096, 0x20, C.byref(old)))
    abi.check(abi.api('FlushInstructionCache', W.BOOL,
        [abi.P, abi.P, C.c_size_t])(abi.current(), base, len(code)))
    callback_type = C.WINFUNCTYPE(C.c_long, abi.P)

    def handler(pointers):
        record = C.cast(pointers, C.POINTER(abi.P))[0]
        exception_code = C.cast(record, C.POINTER(abi.D))[0]
        if exception_code == BREAK:
            # This is the child's own VEH, after debugger NOT_HANDLED. Windows
            # presents this INT3 at its fault PC; explicitly skip that byte.
            context_pointer = C.cast(pointers, C.POINTER(abi.P))[1]
            context = abi.CONTEXT.from_address(context_pointer)
            fault = abi.EXREC.from_address(record).address
            if context.Rip == fault:
                context.Rip += 1
        return -1 if exception_code in (0xe0424281, BREAK) else 0

    callback = callback_type(handler)  # retain callback through all native calls
    veh = abi.api('AddVectoredExceptionHandler', abi.P, [abi.D, callback_type])(1, callback)
    abi.check(veh)
    debug = abi.api('OutputDebugStringA', None, [W.LPCSTR])
    debug(f'{MARKER}{token}:{base:x}:{C.addressof(view):x}'.encode('ascii'))
    if mode == 'handled':
        abi.api('RaiseException', None, [abi.D, abi.D, abi.D, abi.P])(0xe0424281, 0, 0, None)
    if mode == 'target-int3':
        abi.api('DebugBreak', None, [])()
    if mode == 'natural-exit':
        return
    if mode == 'timeout':
        abi.api('Sleep', None, [abi.D])(2000)
        return
    call = C.CFUNCTYPE(abi.U64, abi.U64)
    if mode == 'wrong-caller':
        call(base + 16)(C.addressof(view))
    if mode in ('worker-only', 'extra-thread', 'unhandled'):
        tid = abi.D()
        address = base + (112 if mode == 'unhandled' else 0)
        thread = abi.api('CreateThread', abi.P,
            [abi.P, C.c_size_t, abi.P, abi.P, abi.D, C.POINTER(abi.D)])(
                None, 0, address, C.addressof(view), 0, C.byref(tid))
        abi.check(thread)
        abi.check(abi.waitobj(thread, 4000) == 0)
        abi.check(abi.close(thread))
        if mode in ('worker-only', 'unhandled'):
            return
    call(base)(C.addressof(view))


def read_exact(process, address, length):
    buf, got = C.create_string_buffer(length), C.c_size_t()
    abi.check(abi.read(process, address, buf, length, C.byref(got)))
    if got.value != length:
        raise RuntimeError('short memory read')
    return buf.raw


def validate_entry(snapshot, *, event_pid, pid, event_tid, tid, first, code,
                   exception_address, base, call_bytes, prefix, return_word,
                   view_sample, view_reread):
    """Pure entry predicate shared by the harmless loop and adversarial tests."""
    expected_call = native_code()[4:9]
    checks = dict(ownedPid=event_pid == pid, primaryTid=event_tid == tid,
        firstChance=first == 1, singleStep=code == STEP,
        address=exception_address == snapshot['Rip'] == base + 64,
        dr0=snapshot['Dr0'] == base + 64,
        slot=(snapshot['Dr7'] & 0xf0003) == 1,
        cause=(snapshot['Dr6'] & 0xe00f) == 1,
        exactCall=call_bytes == expected_call,
        exactPrefix=prefix == PREFIX,
        callerReturn=return_word == base + 9,
        viewCanonical=0 < snapshot['Rcx'] < 0x800000000000,
        stackCanonical=0 < snapshot['Rsp'] < 0x800000000000,
        stableObject=len(view_sample) == len(view_reread) == 8 and view_sample == view_reread)
    checks['callTarget'] = (len(call_bytes) == 5 and call_bytes[:1] == b'\xe8'
        and base + 9 + int.from_bytes(call_bytes[1:], 'little', signed=True) == base + 64)
    checks['nonnullObject'] = len(view_sample) == 8 and int.from_bytes(view_sample, 'little') != 0
    return checks


class HandleLedger:
    """Generation identifiers prevent stale closures after numeric reuse."""
    def __init__(self):
        self.rows, self.live = [], {}

    def own(self, handle, kind, owner):
        if not handle:
            return None
        if handle in self.live:
            row = self.live[handle]
            row.setdefault('aliases', []).append(kind)
            return row['generation']
        row = dict(generation=len(self.rows) + 1, value=hex(handle), kind=kind,
                   owner=owner, closed=False)
        self.rows.append(row)
        self.live[handle] = row
        return row['generation']

    def closed(self, handle, generation, method):
        row = self.live.get(handle)
        if row is None or row['generation'] != generation:
            raise RuntimeError('stale handle generation')
        row.update(closed=True, closure=method)
        del self.live[handle]


def run_harmless(mode='normal'):
    if mode not in MODES:
        raise ValueError('fixed harmless mode required')
    if os.name != 'nt' or C.sizeof(abi.P) != 8:
        raise RuntimeError('x64 Windows required')
    here, token = Path(__file__).resolve(), secrets.token_hex(16)
    source_hash = hashlib.sha256(here.read_bytes()).hexdigest()
    result = dict(schema='cp281-harmless-entry-v1', mode=mode, originalLaunched=False,
        originalRunEnabled=ORIGINAL_RUN_ENABLED, originalReady=False,
        independentSupervisorEstablished=False, initialEventArmed=False,
        guiControlAcquired=False, sourceBefore=source_hash, events=[], handles=[],
        errors=[], cleanupErrors=[], exceptions=[], entryQualified=False,
        exitObserved=False, cleanupComplete=False, wrongCallerCount=0,
        observerWritesExecutableBytes=False, osInvalidHandlesVerified=False,
        coverage='primary thread after marker readback until first selected entry; termination endpoint')
    ledger = HandleLedger()
    pi, si, ev = abi.PI(), abi.STARTUPINFO(), abi.EVENT()
    si.cb = C.sizeof(si)
    command = [sys.executable, '-I', '-S', str(here), '--fixed-child', token, mode]
    abi.check(abi.create(sys.executable, C.create_unicode_buffer(subprocess.list2cmdline(command)),
        None, None, False, 2 | 4 | 0x08000000, None, str(here.parent), C.byref(si), C.byref(pi)))
    result.update(pid=pi.pid, primaryTid=pi.tid, command=command, cwd=str(here.parent))
    ledger.own(pi.process, 'caller-process', 'manual')
    ledger.own(pi.thread, 'caller-thread', 'manual')
    retained, threads, event_process = abi.P(), {}, None
    pending, row, disposition = False, None, CONTINUE
    base, view, armed, loader, ntdll = None, None, False, False, None
    started, observation_started = time.monotonic(), None

    def close_manual(handle):
        generation = ledger.live[handle]['generation']
        abi.check(abi.close(handle))
        ledger.closed(handle, generation, 'CloseHandle')

    def resources():
        nonlocal event_process, ntdll
        if ev.code == 3:
            if ev.tid != pi.tid or event_process is not None:
                raise RuntimeError('initial create-event mismatch')
            event_process = ev.u.create.process
            ledger.own(event_process, 'event-process', 'event')
            ledger.own(ev.u.create.thread, 'event-thread', 'event')
            threads[ev.tid] = ev.u.create.thread
            result['loadedImageBase'] = hex(ev.u.create.base or 0)
            if ev.u.create.file:
                ledger.own(ev.u.create.file, 'image-file', 'manual')
                close_manual(ev.u.create.file)
        elif ev.code == 2:
            ledger.own(ev.u.thread.thread, 'event-thread', 'event')
            threads[ev.tid] = ev.u.thread.thread
        elif ev.code == 6 and ev.u.load.file:
            handle = ev.u.load.file
            ledger.own(handle, 'dll-file', 'manual')
            try:
                path = C.create_unicode_buffer(32768)
                count = abi.finalpath(handle, path, len(path), 0)
                if 0 < count < len(path) and Path(path.value).name.lower() == 'ntdll.dll':
                    digest = hashlib.sha256(Path(path.value).read_bytes()).hexdigest()
                    if digest != NTDLL_SHA:
                        raise RuntimeError('pinned harmless host ntdll changed')
                    ntdll = ev.u.load.base
                    actual = read_exact(retained, ntdll + LOADER_RVA - 8, len(LOADER_BYTES))
                    if actual != LOADER_BYTES:
                        raise RuntimeError('loader signature changed')
                    result['loaderModule'] = dict(path=path.value, sha256=digest,
                        base=hex(ntdll), rva=hex(LOADER_RVA), bytes=actual.hex())
            finally:
                close_manual(handle)

    def continuation():
        nonlocal pending
        ok = bool(abi.cont(ev.pid, ev.tid, disposition))
        row.update(disposition=hex(disposition), continued=ok,
                   continuationError=None if ok else C.get_last_error())
        if not ok:
            raise RuntimeError('ContinueDebugEvent failed')
        pending = False
        if ev.code == 4:
            handle = threads.pop(ev.tid, None)
            if handle in ledger.live:
                ledger.closed(handle, ledger.live[handle]['generation'], 'ContinueDebugEvent:thread-exit')
        if ev.code == 5:
            for handle in set([event_process, *threads.values()]):
                if handle in ledger.live:
                    ledger.closed(handle, ledger.live[handle]['generation'], 'ContinueDebugEvent:process-exit')
            threads.clear()
            result.update(exitObserved=True, exitCode=hex(ev.u.exitCode))

    def next_event():
        nonlocal pending, row, disposition
        if not abi.wait(C.byref(ev), 100):
            if C.get_last_error() == 121:
                return False
            raise C.WinError(C.get_last_error())
        pending, disposition = True, FORWARD if ev.code == 1 else CONTINUE
        row = dict(sequence=len(result['events']) + 1, code=ev.code, pid=ev.pid,
                   tid=ev.tid, continued=False)
        result['events'].append(row)
        if ev.pid != pi.pid:
            raise RuntimeError('foreign event')
        resources()
        return True

    def terminate():
        if result.get('termination') is None:
            ok = bool(abi.terminate(retained.value or pi.process, 0xe281))
            result['termination'] = dict(ok=ok, error=None if ok else C.get_last_error())
            if not ok:
                raise RuntimeError('owned termination failed')

    try:
        abi.check(abi.duplicate(abi.current(), pi.process, abi.current(), C.byref(retained), 0, False, 2))
        ledger.own(retained.value, 'retained-process', 'manual')
        abi.check(abi.killonexit(True))
        image, length = C.create_unicode_buffer(32768), abi.D(32768)
        abi.check(abi.queryimage(retained, 0, image, C.byref(length)))
        if Path(image.value).resolve() != Path(sys.executable).resolve():
            raise RuntimeError('harmless image mismatch')
        times = [abi.U64() for _ in range(4)]
        abi.check(abi.gettimes(retained, *(C.byref(t) for t in times)))
        result.update(image=image.value, creationFiletime=str(times[0].value))
        observation_started = time.monotonic()
        previous = abi.api('ResumeThread', abi.D, [abi.P])(pi.thread)
        result.update(resumePreviousCount=previous,
            startupSequence='CreateProcess-suspended -> ResumeThread -> first WaitForDebugEvent',
            designCreateEventBeforeResumeVerified=False)
        if previous != 1:
            raise RuntimeError('unexpected startup suspension count')
        while not result['exitObserved']:
            if len(result['events']) >= MAX_EVENTS or time.monotonic() - started > 12:
                raise RuntimeError('event/time bound')
            if len(json.dumps(result)) > MAX_RECEIPT:
                raise RuntimeError('receipt bound')
            if mode == 'timeout' and base and time.monotonic() - observation_started > .4:
                raise RuntimeError('fixed harmless observation timeout')
            if not next_event():
                continue
            if ev.code == 3:
                row['soleResumeAlreadyIssued'] = True
            elif ev.code == 8:
                s = ev.u.string
                if not s.unicode and 0 < s.length <= 192:
                    text = read_exact(retained, s.address, s.length).rstrip(b'\0').decode('ascii', errors='replace')
                    expected = f'{MARKER}{token}:'
                    if text.startswith(expected):
                        if base or not loader or ev.tid != pi.tid:
                            raise RuntimeError('marker lifecycle mismatch')
                        parts = text[len(expected):].split(':')
                        if len(parts) != 2:
                            raise RuntimeError('marker format')
                        base, view = (int(p, 16) for p in parts)
                        if read_exact(retained, base, len(native_code())) != native_code():
                            raise RuntimeError('fixed native bytes mismatch')
                        raw, ctx = abi.context(threads[pi.tid])
                        result['beforeArm'] = {n: getattr(ctx, n) for n in REGS}
                        if ctx.Dr7 & 0xff or ctx.Dr6 & 0xe00f:
                            raise RuntimeError('occupied or unexplained debug state')
                        ctx.Dr0, ctx.Dr6 = base + 64, ctx.Dr6 & ~1
                        ctx.Dr7 = (ctx.Dr7 & ~0xf0003) | 1
                        abi.check(abi.setctx(threads[pi.tid], C.byref(ctx)))
                        raw_after, after = abi.context(threads[pi.tid])
                        if after.Dr0 != base + 64 or after.Dr7 != ctx.Dr7 or after.Dr6 & 0xe00f:
                            raise RuntimeError('arm readback failed')
                        result['armReadback'] = {n: getattr(after, n) for n in REGS}
                        armed = True
            elif ev.code == 1:
                ex = ev.u.exception
                raw, ctx = abi.context(threads[ev.tid])
                snapshot = {n: getattr(ctx, n) for n in REGS}
                exception = dict(sequence=row['sequence'], tid=ev.tid, code=hex(ex.record.code),
                    firstChance=bool(ex.first), address=hex(ex.record.address or 0),
                    classification='forwarded-target-exception', context=snapshot)
                result['exceptions'].append(exception)
                if (ex.first == 1 and ex.record.code == BREAK and not loader and not base
                        and ev.tid == pi.tid and ntdll and ex.record.address == ntdll + LOADER_RVA
                        and ctx.Rip == ex.record.address + 1):
                    loader, disposition = True, CONTINUE
                    exception['classification'] = 'qualified-startup-loader-breakpoint'
                elif (armed and ev.tid == pi.tid and ex.first == 1 and ex.record.code == STEP
                        and ex.record.address == ctx.Rip == ctx.Dr0 == base + 64
                        and ctx.Dr7 & 0xf0003 == 1 and ctx.Dr6 & 0xe00f == 1):
                    disposition = CONTINUE
                    return_word = int.from_bytes(read_exact(retained, ctx.Rsp, 8), 'little')
                    if return_word != base + 9:
                        exception['classification'] = 'owned-unselected-caller'
                        result['wrongCallerCount'] += 1
                        if result['wrongCallerCount'] > 4:
                            raise RuntimeError('wrong-caller bound')
                        ctx.Dr6 &= ~1
                        ctx.EFlags |= RF
                        abi.check(abi.setctx(threads[pi.tid], C.byref(ctx)))
                        raw_after, after = abi.context(threads[pi.tid])
                        exception['readback'] = {n: getattr(after, n) for n in REGS}
                        exception['expectedRf'] = True
                        # DR7 bit 10 is architectural reserved-one; this host's
                        # Set/Get pair normalizes it. Retain both raw values.
                        if (after.Dr0 != base + 64 or (after.Dr7 ^ ctx.Dr7) & ~0x400
                                or after.Dr6 & 0xe00f or not after.EFlags & RF):
                            raise RuntimeError('RF readback failed')
                        exception['rfSuppressionInterval'] = 'next instruction; no absence claim'
                    else:
                        exception['classification'] = 'owned-selected-entry'
                        if 'entry' in result:
                            raise RuntimeError('selected entry already recorded')
                        # A failed read leaves the raw exception/context retained.
                        sample = read_exact(retained, ctx.Rcx + 0x18, 8)
                        reread = read_exact(retained, ctx.Rcx + 0x18, 8)
                        call_bytes = read_exact(retained, base + 4, 5)
                        prefix = read_exact(retained, base + 64, len(PREFIX))
                        checks = validate_entry(snapshot, event_pid=ev.pid, pid=pi.pid,
                            event_tid=ev.tid, tid=pi.tid, first=ex.first, code=ex.record.code,
                            exception_address=ex.record.address, base=base,
                            call_bytes=call_bytes, prefix=prefix, return_word=return_word,
                            view_sample=sample, view_reread=reread)
                        checks['markerViewIdentity'] = ctx.Rcx == view
                        result['entry'] = dict(context=snapshot, returnWord=hex(return_word),
                            callHex=call_bytes.hex(), prefixHex=prefix.hex(), objectHex=sample.hex(),
                            objectRereadHex=reread.hex(), checks=checks, eventSequence=row['sequence'])
                        result['entryQualified'] = all(checks.values())
                        if not result['entryQualified']:
                            raise RuntimeError('entry predicates failed')
                        terminate()  # helper's first instruction never executes in this mode
                exception['disposition'] = hex(disposition)
                if not ex.first:
                    result['unhandledSecondChance'] = True
            continuation()
    except Exception as error:
        result['errors'].append(str(error))
        try:
            if not result['exitObserved']:
                terminate()
        except Exception as cleanup_error:
            result['cleanupErrors'].append(str(cleanup_error))
        if pending:
            try:
                continuation()  # preserve already-selected disposition, no silent reclassification
            except Exception as cleanup_error:
                result['cleanupErrors'].append(str(cleanup_error))
        deadline = time.monotonic() + 3
        cleanup_events = 0
        while (not pending and not result['exitObserved'] and time.monotonic() < deadline
               and cleanup_events < 64):
            try:
                if next_event():
                    cleanup_events += 1
                    row['cleanup'] = True
                    continuation()
            except Exception as cleanup_error:
                result['cleanupErrors'].append(str(cleanup_error))
                break
        if not result['exitObserved']:
            result['cleanupErrors'].append('bounded cleanup did not reach EXIT_PROCESS')
    finally:
        result['pendingEvent'] = dict(row) if pending else None
        result['ownedProcessSignaled'] = abi.waitobj(retained.value or pi.process, 2000) == 0
        for handle, item in list(ledger.live.items()):
            if item['owner'] == 'manual':
                try:
                    close_manual(handle)
                except Exception as error:
                    result['cleanupErrors'].append(str(error))
        result['handles'] = ledger.rows
        result['logicalHandlesClosed'] = not ledger.live
        # probe_pid is deliberately not used: creation-aware independent absence
        # verification has not been implemented in this first increment.
        result['independentAbsenceVerified'] = False
        result['cleanupComplete'] = (result['exitObserved'] and result['ownedProcessSignaled']
            and not pending and not ledger.live and not result['cleanupErrors'])
        result['sourceAfter'] = hashlib.sha256(here.read_bytes()).hexdigest()
        result['sourceStable'] = result['sourceBefore'] == result['sourceAfter']
        result['elapsedSeconds'] = round(time.monotonic() - started, 4)
        result['harmlessPassed'] = (result['entryQualified'] and result['cleanupComplete']
            and result['sourceStable'] and not result['errors'])
    return result


if __name__ == '__main__':
    if len(sys.argv) == 4 and sys.argv[1] == '--fixed-child' and sys.argv[3] in MODES:
        _child(sys.argv[2], sys.argv[3])
    elif len(sys.argv) == 3 and sys.argv[1] == '--harmless' and sys.argv[2] in MODES:
        print(json.dumps(run_harmless(sys.argv[2]), sort_keys=True))
    else:
        raise SystemExit('only --harmless FIXED_MODE is supported; original remains HOLD')
