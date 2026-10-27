"""CP283 fixed benign native creation-event DR0 probe. Original remains HOLD.

Only the CP282 hash-pinned fixture can be created. No PID/attach/path command is
accepted. The independent supervisor is mandatory before creation/resumption.
"""
import ctypes as C
import hashlib
import json
import os
from pathlib import Path
import struct
import subprocess
import sys
import time
sys.path.insert(0, str(Path(__file__).resolve().parent))
import color_original_entry_observer as prior
import build_native_color_entry_fixture as build
abi = prior.abi
CONTINUE, FORWARD, STEP, BREAK, RF = prior.CONTINUE, prior.FORWARD, prior.STEP, prior.BREAK, prior.RF
REGS, NTDLL_SHA, LOADER_RVA, LOADER_BYTES = prior.REGS, prior.NTDLL_SHA, prior.LOADER_RVA, prior.LOADER_BYTES
read_exact = prior.read_exact
ORIGINAL_RUN_ENABLED = False
MODES = ('normal', 'reject-image')
MAX_EVENTS, MAX_RECEIPT = 512, 256 * 1024
FIXTURE = Path(__file__).resolve().parent.parent / 'build/cp282-native-startup-fixture/native_color_entry_fixture.exe'
FIXTURE_SHA = '2a07aecccca5562b0dfe02fd212b1aa84d0a69cc58d4ceddd7c1617f970dc962'
ENTRY_RVA, CALL_RVA, RETURN_RVA = 0x1000, 0x11c0, 0x11c5
PREFIX = bytes.fromhex('488b41184889055d5c0700488b05565c')
CALL_BYTES = bytes.fromhex('e83bfeffff')


def run_original():
    raise RuntimeError('HOLD: original launch is not implemented or released')


def fixed_fixture():
    blob = FIXTURE.read_bytes()
    if hashlib.sha256(blob).hexdigest() != FIXTURE_SHA:
        raise RuntimeError('HOLD: CP282 fixed native fixture hash mismatch')
    pe = build.parse_pe(blob)
    if pe['entry_rva'] != ENTRY_RVA or pe['entry_bytes'] != PREFIX.hex():
        raise RuntimeError('HOLD: pinned PE export/prefix mismatch')
    return blob, pe


def hash_image_handle(handle):
    # Hash the event-owned file generation itself, not only its pathname.
    size, position = C.c_longlong(), C.c_longlong()
    abi.check(abi.api('GetFileSizeEx', abi.W.BOOL, [abi.P, abi.P])(handle, C.byref(size)))
    if size.value != 499712:
        raise RuntimeError('event image size mismatch')
    seek = abi.api('SetFilePointerEx', abi.W.BOOL, [abi.P, C.c_longlong, abi.P, abi.D])
    abi.check(seek(handle, 0, C.byref(position), 1))
    abi.check(seek(handle, 0, None, 0))
    try:
        data, read = C.create_string_buffer(size.value), abi.D()
        abi.check(abi.api('ReadFile', abi.W.BOOL, [abi.P, abi.P, abi.D, abi.P, abi.P])(
            handle, data, size.value, C.byref(read), None))
        if read.value != size.value:
            raise RuntimeError('event image short read')
        digest = hashlib.sha256(data.raw).hexdigest()
        if digest != FIXTURE_SHA:
            raise RuntimeError('event image hash mismatch')
        path = C.create_unicode_buffer(32768)
        count = abi.finalpath(handle, path, len(path), 0)
        if not 0 < count < len(path) or str(Path(path.value).resolve()).removeprefix('\\\\?\\').casefold() != str(FIXTURE.resolve()).casefold():
            raise RuntimeError('event image path mismatch')
        # BY_HANDLE_FILE_INFORMATION: volume serial and file index bind identity.
        info = (abi.D * 13)()
        abi.check(abi.api('GetFileInformationByHandle', abi.W.BOOL, [abi.P, abi.P])(handle, C.byref(info)))
        return dict(path=path.value, size=size.value, sha256=digest,
            volumeSerial=info[7], fileIndex=str((info[11] << 32) | info[12]))
    finally:
        abi.check(seek(handle, position.value, None, 0))


def mapped_export(process, base, blob, pe):
    # Reparse the remote export tables under strict fixed-image bounds.
    opt = struct.unpack_from('<I', blob, 0x3c)[0] + 24
    rva, size = struct.unpack_from('<II', blob, opt + 112)
    def read(r, n):
        if not 0 <= r < r + n <= pe['image_size']:
            raise RuntimeError('mapped export range')
        return read_exact(process, base + r, n)
    directory = read(rva, 40)
    nfunc, nname, funcs, names, ords = struct.unpack_from('<IIIII', directory, 20)
    if not 0 < nname <= nfunc <= 8:
        raise RuntimeError('unexpected fixed export count')
    for i in range(nname):
        name_rva = struct.unpack('<I', read(names + 4*i, 4))[0]
        name = read(name_rva, len(b'EntryProbe\0'))
        if name == b'EntryProbe\0':
            ordinal = struct.unpack('<H', read(ords + 2*i, 2))[0]
            if ordinal >= nfunc:
                raise RuntimeError('mapped export ordinal')
            target = struct.unpack('<I', read(funcs + 4*ordinal, 4))[0]
            if rva <= target < rva + size:
                raise RuntimeError('forwarded export rejected')
            return target
    raise RuntimeError('mapped EntryProbe export missing')


def validate_dr7_transition(armed, hit):
    return dict(preservedUnrelatedDr7=((armed ^ hit) & ~0x400) == 0,
        reservedDr7Qualified=(hit == armed or (not armed & 0x400 and hit == (armed | 0x400))))


def validate_entry(snapshot, *, event_pid, pid, event_tid, tid, first, code,
                   exception_address, base, call_bytes, prefix, return_word,
                   view_sample, view_reread):
    return dict(ownedPid=event_pid == pid, primaryTid=event_tid == tid,
        firstChance=first == 1, singleStep=code == STEP,
        exactPc=exception_address == snapshot['Rip'] == base + ENTRY_RVA,
        dr0=snapshot['Dr0'] == base + ENTRY_RVA,
        slot=(snapshot['Dr7'] & 0xf0003) == 1,
        cause=(snapshot['Dr6'] & 0xe00f) == 1,
        exactCall=call_bytes == CALL_BYTES, exactPrefix=prefix == PREFIX,
        callerReturn=return_word == base + RETURN_RVA,
        callTarget=(len(call_bytes) == 5 and call_bytes[:1] == b'\xe8' and
            base + RETURN_RVA + int.from_bytes(call_bytes[1:], 'little', signed=True) == base + ENTRY_RVA),
        viewCanonical=0 < snapshot['Rcx'] < 0x800000000000,
        stackCanonical=0 < snapshot['Rsp'] < 0x800000000000,
        stableObject=len(view_sample) == len(view_reread) == 8 and view_sample == view_reread,
        nonnullObject=len(view_sample) == 8 and 0 < int.from_bytes(view_sample, 'little') < 0x800000000000)


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


def run_harmless(mode='normal', *, _supervision=None):
    if mode not in MODES:
        raise ValueError('fixed harmless mode required')
    if os.name != 'nt' or C.sizeof(abi.P) != 8:
        raise RuntimeError('x64 Windows required')
    here = Path(__file__).resolve()
    if _supervision is None:
        raise RuntimeError('independent fixed supervisor required before native child creation')
    blob, pe = fixed_fixture()
    source_hash = hashlib.sha256(here.read_bytes()).hexdigest()
    result = dict(schema='cp283-native-create-event-entry-v1', mode=mode, originalLaunched=False,
        originalRunEnabled=ORIGINAL_RUN_ENABLED, originalReady=False,
        independentSupervisorEstablished=False, initialEventArmed=False,
        guiControlAcquired=False, sourceBefore=source_hash, events=[], handles=[],
        errors=[], cleanupErrors=[], exceptions=[], entryQualified=False,
        exitObserved=False, cleanupComplete=False, wrongCallerCount=0,
        observerWritesExecutableBytes=False, osInvalidHandlesVerified=False,
        coverage='primary DR0 from creation-event readback until first entry; termination endpoint')
    ledger = HandleLedger()
    pi, si, ev = abi.PI(), abi.STARTUPINFO(), abi.EVENT()
    si.cb = C.sizeof(si)
    command = [str(FIXTURE)]
    abi.check(abi.create(str(FIXTURE), C.create_unicode_buffer(subprocess.list2cmdline(command)),
        None, None, False, 2 | 4 | 0x08000000, None, str(FIXTURE.parent), C.byref(si), C.byref(pi)))
    result.update(pid=pi.pid, primaryTid=pi.tid, command=command, cwd=str(FIXTURE.parent), fixtureSha256=FIXTURE_SHA)
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
        nonlocal event_process, ntdll, base, armed
        if ev.code == 3:
            if ev.tid != pi.tid or event_process is not None:
                raise RuntimeError('initial create-event mismatch')
            event_process = ev.u.create.process
            ledger.own(event_process, 'event-process', 'event')
            ledger.own(ev.u.create.thread, 'event-thread', 'event')
            threads[ev.tid] = ev.u.create.thread
            result['loadedImageBase'] = hex(ev.u.create.base or 0)
            if not ev.u.create.file:
                raise RuntimeError('creation event has no image file handle')
            ledger.own(ev.u.create.file, 'image-file', 'manual')
            try:
                result['createImageFile'] = hash_image_handle(ev.u.create.file)
            finally:
                close_manual(ev.u.create.file)
            base = ev.u.create.base
            if ev.pid != pi.pid or ev.tid != pi.tid or not base or base + pe['image_size'] >= 0x800000000000:
                raise RuntimeError('creation PID/TID/base mismatch')
            mapped_header = read_exact(retained, base, 1024)
            header_diffs = [i for i in range(1024) if mapped_header[i] != blob[i]]
            result['headerDifferences'] = [dict(offset=i, file=blob[i], mapped=mapped_header[i]) for i in header_diffs[:32]]
            imagebase_offset = struct.unpack_from('<I', blob, 0x3c)[0] + 24 + 24
            expected_header = bytearray(blob[:1024])
            struct.pack_into('<Q', expected_header, imagebase_offset, base)
            result['mappedHeaderImageBase'] = hex(struct.unpack_from('<Q', mapped_header, imagebase_offset)[0])
            if mapped_header != expected_header:
                raise RuntimeError('mapped PE header mismatch beyond loaded ImageBase')
            export = mapped_export(retained, base, blob, pe)
            prefix = read_exact(retained, base + ENTRY_RVA, len(PREFIX))
            call = read_exact(retained, base + CALL_RVA, 5)
            result['initialImage'] = dict(base=hex(base), machine=pe['machine'],
                imageSize=pe['image_size'], exportRva=hex(export), prefixHex=prefix.hex(),
                callHex=call.hex(), eventSequence=row['sequence'])
            if export != ENTRY_RVA or prefix != PREFIX or call != CALL_BYTES:
                raise RuntimeError('mapped export/entry/call mismatch')
            row['soleResumeAlreadyIssued'] = result['resumePreviousCount'] == 1
            if mode == 'reject-image':
                raise RuntimeError('fixed injected image gate refusal before arm')
            raw, ctx = abi.context(threads[pi.tid])
            saved = {n: getattr(ctx, n) for n in REGS}
            result['beforeArm'] = saved
            if ctx.Dr7 & 0xff or ctx.Dr6 & 0xe00f:
                raise RuntimeError('occupied or unexplained debug state')
            ctx.Dr0, ctx.Dr6 = base + ENTRY_RVA, ctx.Dr6 & ~1
            ctx.Dr7 = (ctx.Dr7 & ~0xf0003) | 1
            abi.check(abi.setctx(threads[pi.tid], C.byref(ctx)))
            raw_after, after = abi.context(threads[pi.tid])
            snapshot = {n: getattr(after, n) for n in REGS}
            checks = dict(dr0=after.Dr0 == base + ENTRY_RVA,
                dr7Exact=after.Dr7 == ctx.Dr7, dr6Exact=after.Dr6 == ctx.Dr6,
                preserved=all(snapshot[n] == saved[n] for n in REGS
                    if n not in ('Dr0', 'Dr6', 'Dr7')))
            result['armReadback'] = snapshot
            result['armContextHex'] = C.string_at(C.byref(after), C.sizeof(after)).hex()
            result['armChecks'] = checks
            if not all(checks.values()):
                raise RuntimeError('creation-event arm readback failed')
            armed = True
            result['initialEventArmed'] = True
            row['armedBeforeSuccessfulContinuation'] = True
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
        if row.get('continuationAttempted'):
            raise RuntimeError('continuation outcome retained; no retry')
        row['continuationAttempted'] = True
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
        if row['sequence'] == 1 and ev.code != 3:
            raise RuntimeError('first debug event was not CREATE_PROCESS')
        if _supervision:
            _supervision('event-held', dict(row, disposition=hex(disposition)))
        return True

    def terminate():
        if result.get('termination') is None:
            ok = bool(abi.terminate(retained.value or pi.process, 0xe281))
            result['termination'] = dict(ok=ok, error=None if ok else C.get_last_error())
            if not ok:
                raise RuntimeError('owned termination failed')

    try:
        if _supervision:
            _supervision('created-before-explicit-kill-policy', dict(pid=pi.pid, primaryTid=pi.tid))
        abi.check(abi.duplicate(abi.current(), pi.process, abi.current(), C.byref(retained), 0, False, 2))
        ledger.own(retained.value, 'retained-process', 'manual')
        abi.check(abi.killonexit(True))
        result['explicitKillOnExitChecked'] = True
        result['defaultKillOnExitWindow'] = 'CreateProcess to checked explicit call; documented default TRUE'
        image, length = C.create_unicode_buffer(32768), abi.D(32768)
        abi.check(abi.queryimage(retained, 0, image, C.byref(length)))
        if Path(image.value).resolve() != FIXTURE.resolve():
            raise RuntimeError('harmless image mismatch')
        times = [abi.U64() for _ in range(4)]
        abi.check(abi.gettimes(retained, *(C.byref(t) for t in times)))
        result.update(image=image.value, creationFiletime=str(times[0].value))
        if _supervision:
            acknowledgement = _supervision('child-handle-offer', dict(
                handle=pi.process, pid=pi.pid, primaryTid=pi.tid, image=image.value,
                creationFiletime=str(times[0].value), explicitKillOnExitChecked=True))
            if acknowledgement is not True:
                raise RuntimeError('supervisor child identity acknowledgement required')
            result['independentSupervisorEstablished'] = True
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
            elif ev.code == 1:
                ex = ev.u.exception
                raw, ctx = abi.context(threads[ev.tid])
                snapshot = {n: getattr(ctx, n) for n in REGS}
                exception = dict(sequence=row['sequence'], tid=ev.tid, code=hex(ex.record.code),
                    firstChance=bool(ex.first), address=hex(ex.record.address or 0),
                    classification='forwarded-target-exception', context=snapshot)
                result['exceptions'].append(exception)
                if (ex.first == 1 and ex.record.code == BREAK and not loader and armed
                        and ev.tid == pi.tid and ntdll and ex.record.address == ntdll + LOADER_RVA
                        and ctx.Rip == ex.record.address + 1):
                    loader, disposition = True, CONTINUE
                    exception['classification'] = 'qualified-startup-loader-breakpoint'
                elif (armed and ev.tid == pi.tid and ex.first == 1 and ex.record.code == STEP
                        and ex.record.address == ctx.Rip == ctx.Dr0 == base + ENTRY_RVA
                        and ctx.Dr7 & 0xf0003 == 1 and ctx.Dr6 & 0xe00f == 1):
                    disposition = CONTINUE
                    return_word = int.from_bytes(read_exact(retained, ctx.Rsp, 8), 'little')
                    if return_word != base + RETURN_RVA:
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
                        if (after.Dr0 != base + ENTRY_RVA or (after.Dr7 ^ ctx.Dr7) & ~0x400
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
                        call_bytes = read_exact(retained, base + CALL_RVA, 5)
                        prefix = read_exact(retained, base + ENTRY_RVA, len(PREFIX))
                        checks = validate_entry(snapshot, event_pid=ev.pid, pid=pi.pid,
                            event_tid=ev.tid, tid=pi.tid, first=ex.first, code=ex.record.code,
                            exception_address=ex.record.address, base=base,
                            call_bytes=call_bytes, prefix=prefix, return_word=return_word,
                            view_sample=sample, view_reread=reread)
                        checks['initialArmingEstablished'] = result['initialEventArmed']
                        checks.update(validate_dr7_transition(result['armReadback']['Dr7'], ctx.Dr7))
                        checks['preservedDr1To3'] = all(snapshot[n] == result['beforeArm'][n] for n in ('Dr1','Dr2','Dr3'))
                        checks['objectReadable'] = read_exact(retained, int.from_bytes(sample, 'little'), 5) == b'CP282'
                        result['entry'] = dict(context=snapshot, rawContextHex=C.string_at(C.byref(ctx), C.sizeof(ctx)).hex(), returnWord=hex(return_word),
                            callHex=call_bytes.hex(), prefixHex=prefix.hex(), objectHex=sample.hex(),
                            objectRereadHex=reread.hex(), checks=checks, eventSequence=row['sequence'])
                        result['entryQualified'] = all(checks.values())
                        if not result['entryQualified']:
                            raise RuntimeError('entry predicates failed')
                        if _supervision:
                            _supervision('selected-entry-held', dict(row, disposition=hex(disposition)))
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

