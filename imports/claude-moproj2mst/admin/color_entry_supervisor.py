"""CP282 fixed harmless debugger-worker supervisor. Original execution is HOLD.

No executable/PID/attach arguments are accepted. Process handles come only from
our own worker creation and DuplicateHandle from that worker's child offer.
"""
import ctypes as C
import hashlib
import json
from pathlib import Path
import queue
import subprocess
import sys
import threading
import time

sys.path.insert(0, str(Path(__file__).resolve().parent))
import color_original_entry_observer as observer
abi = observer.abi
MODES = ('normal', 'held-entry', 'held-exit', 'no-ack', 'pre-kill-hang')
OWNER_SECONDS = 3.0
COOPERATIVE_SECONDS = .35
WAIT_MS = 2000
MAX_IPC = 384 * 1024


def identity(handle):
    image, length = C.create_unicode_buffer(32768), abi.D(32768)
    abi.check(abi.queryimage(handle, 0, image, C.byref(length)))
    times = [abi.U64() for _ in range(4)]
    abi.check(abi.gettimes(handle, *(C.byref(t) for t in times)))
    pid = abi.api('GetProcessId', abi.D, [abi.P])(handle)
    abi.check(pid)
    return dict(pid=pid, creationFiletime=str(times[0].value), image=image.value)


def send(stream, payload):
    stream.write(json.dumps(payload, sort_keys=True) + '\n')
    stream.flush()


def reader(stream, messages):
    total = 0
    try:
        while True:
            line = stream.readline(MAX_IPC + 1)
            if not line:
                break
            total += len(line)
            if total > MAX_IPC or not line.endswith('\n'):
                messages.put(dict(kind='ipc-error', error='IPC byte bound'))
                break
            messages.put(json.loads(line))
    except Exception as error:
        messages.put(dict(kind='ipc-error', error=str(error)))
    finally:
        messages.put(dict(kind='ipc-eof'))


def _worker(mode, backend="python-marker"):
    target = observer
    if backend == "native-create-event":
        import color_create_event_probe as target
    # Parent owns the worker handle/identity and immutable deadline before gate.
    gate = json.loads(sys.stdin.readline(1024))
    if gate != dict(kind='start-fixed-worker'):
        raise RuntimeError('fixed supervisor start gate required')
    commands = queue.Queue()
    threading.Thread(target=reader, args=(sys.stdin, commands), daemon=True).start()

    def hang(point, data):
        send(sys.stdout, dict(kind='injected-held-hang', point=point, data=data))
        while True:
            time.sleep(.1)  # deliberate unresponsive worker; supervisor must kill handle

    def hook(point, data):
        send(sys.stdout, dict(kind=point, data=data))
        if mode == 'pre-kill-hang' and point == 'created-before-explicit-kill-policy':
            hang(point, data)
        if point == 'child-handle-offer':
            try:
                acknowledgement = commands.get(timeout=1.5)
            except queue.Empty:
                raise RuntimeError('bounded child identity acknowledgement timeout')
            expected = {k: data[k] for k in ('pid', 'creationFiletime', 'image')}
            if acknowledgement != dict(kind='child-identity-ack', identity=expected):
                raise RuntimeError('child identity acknowledgement mismatch')
            return True
        if ((mode == 'held-entry' and point == 'selected-entry-held') or
                (mode == 'held-exit' and point == 'event-held' and data['code'] == 5)):
            hang(point, data)
        try:
            command = commands.get_nowait()
        except queue.Empty:
            return None
        if command.get('kind') == 'cleanup':
            raise RuntimeError('supervisor requested cooperative cleanup')
        raise RuntimeError('unexpected supervisor command')

    result = target.run_harmless(mode if mode in ('reject-image', 'reject-create-event') else 'normal', _supervision=hook)
    send(sys.stdout, dict(kind='worker-result', data=result))


def run_supervised(mode='normal', *, backend='python-marker'):
    if backend not in ('python-marker', 'native-create-event'):
        raise ValueError('fixed harmless backend required')
    allowed = MODES if backend == 'python-marker' else (*MODES, 'reject-image', 'reject-create-event')
    if mode not in allowed:
        raise ValueError('fixed harmless supervisor mode required')
    started = time.monotonic()
    deadline = started + OWNER_SECONDS  # never extended by traffic or cleanup
    here = Path(__file__).resolve()
    sources = [here, Path(observer.__file__).resolve()]
    expected_child = Path(sys.executable).resolve()
    if backend == 'native-create-event':
        import color_create_event_probe as native
        native.fixed_fixture()
        expected_child = native.FIXTURE.resolve()
        sources.extend([Path(native.__file__).resolve(), Path(native.build.__file__).resolve(), Path(abi.__file__).resolve()])
    source_before = {p.name: hashlib.sha256(p.read_bytes()).hexdigest()
                     for p in sources}
    result = dict(schema='cp282-fixed-harmless-supervisor-v1', mode=mode, backend=backend,
        originalLaunched=False, originalReady=False, guiControlAcquired=False,
        deadlineStartedBeforeWorkerCreation=True, ownerDeadlineSeconds=OWNER_SECONDS,
        cooperativeSeconds=COOPERATIVE_SECONDS, workerIdentity=None,
        childIdentity=None, childHandleAcknowledged=False, errors=[], messages=[],
        workerResult=None, workerTermination=None, childTermination=None,
        pendingEvent=None, eventTail='unknown', workerCleanupComplete=False,
        independentChildExitVerified=False, osInvalidHandlesVerified=False)
    worker = child = None
    messages, eof = queue.Queue(), False
    fallback_at = None
    last_event, injection = None, None
    try:
        command = [sys.executable, '-I', '-S', str(here), '--fixed-worker', mode, backend]
        worker = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE,
            stderr=subprocess.DEVNULL, text=True, bufsize=1, creationflags=0x08000000)
        worker_handle = int(worker._handle)  # creation-owned Popen handle; no PID reopen
        result['workerIdentity'] = identity(worker_handle)
        result['workerIdentity']['creationHandleOwnedBeforeSetup'] = True
        threading.Thread(target=reader, args=(worker.stdout, messages), daemon=True).start()
        if time.monotonic() >= deadline:
            raise RuntimeError('owner deadline expired before worker start gate')
        send(worker.stdin, dict(kind='start-fixed-worker'))
        while True:
            now = time.monotonic()
            if now >= deadline and fallback_at is None:
                result['deadlineFired'] = True
                try:
                    send(worker.stdin, dict(kind='cleanup'))
                    result['cooperativeCleanupRequested'] = True
                except (OSError, ValueError):
                    result['cooperativeCleanupRequested'] = False
                if child:
                    ok = bool(abi.terminate(child.value, 0xe282))
                    result['childTermination'] = dict(ok=ok, error=None if ok else C.get_last_error())
                    result['childWaitBeforeWorkerFallback'] = abi.waitobj(child, 0)
                fallback_at = now + COOPERATIVE_SECONDS
            if fallback_at is not None and now >= fallback_at:
                if abi.waitobj(worker_handle, 0) != 0:
                    ok = bool(abi.terminate(worker_handle, 0xe282))
                    result['workerTermination'] = dict(ok=ok, error=None if ok else C.get_last_error())
                break
            try:
                message = messages.get(timeout=.025)
            except queue.Empty:
                if eof and abi.waitobj(worker_handle, 0) == 0:
                    break
                continue
            kind = message['kind']
            if kind == 'worker-result':
                result['workerResult'] = message['data']
            elif kind == 'ipc-eof':
                eof = True
            elif kind == 'child-handle-offer':
                data = message['data']
                child = abi.P()
                abi.check(abi.duplicate(worker_handle, data['handle'], abi.current(),
                    C.byref(child), 0, False, 2))
                bound = identity(child)
                expected = {k: data[k] for k in ('pid', 'creationFiletime', 'image')}
                if bound != expected or Path(bound['image']).resolve() != expected_child:
                    raise RuntimeError('duplicated child identity mismatch')
                if not data['explicitKillOnExitChecked']:
                    raise RuntimeError('explicit kill policy not checked')
                result['childIdentity'] = bound
                if mode != 'no-ack':
                    send(worker.stdin, dict(kind='child-identity-ack', identity=bound))
                    result['childHandleAcknowledged'] = True
            elif kind in ('event-held', 'selected-entry-held'):
                last_event = message['data']
            elif kind == 'injected-held-hang':
                injection = message
            elif kind == 'ipc-error':
                raise RuntimeError(message['error'])
            if kind not in ('worker-result', 'ipc-eof'):
                result['messages'].append(message)
    except Exception as error:
        result['errors'].append(str(error))
        if child:
            ok = bool(abi.terminate(child, 0xe282))
            result['childTermination'] = dict(ok=ok, error=None if ok else C.get_last_error())
        if worker and abi.waitobj(int(worker._handle), 0) != 0:
            ok = bool(abi.terminate(int(worker._handle), 0xe282))
            result['workerTermination'] = dict(ok=ok, error=None if ok else C.get_last_error())
    finally:
        if worker:
            result['workerWait'] = abi.waitobj(int(worker._handle), WAIT_MS)
            result['workerExitVerified'] = result['workerWait'] == 0
            if result['workerExitVerified']:
                worker.wait(timeout=.1)
                result['workerExitCode'] = worker.returncode
            else:
                result['errors'].append('worker exit wait failed or timed out')
            if child:
                result['childWait'] = abi.waitobj(child, WAIT_MS)
                result['independentChildExitVerified'] = result['childWait'] == 0
                code = abi.D()
                queried = bool(abi.api('GetExitCodeProcess', abi.W.BOOL,
                    [abi.P, C.POINTER(abi.D)])(child, C.byref(code)))
                result['childExitCodeQuery'] = dict(ok=queried, error=None if queried else C.get_last_error())
                result['childExitCode'] = hex(code.value) if queried else None
                if not result['independentChildExitVerified']:
                    result['errors'].append('child exit wait failed or timed out')
                result['childHandleClosed'] = bool(abi.close(child))
                if not result['childHandleClosed']:
                    result['errors'].append('supervisor child CloseHandle failed')
            else:
                result['childExitStatus'] = 'unknown: no duplicated creation-bound child handle'
            for stream in (worker.stdin, worker.stdout):
                try:
                    stream.close()
                except OSError:
                    pass
            # Popen owns and closes this generation exactly once.
            worker._handle.Close()
            result['workerHandleClosed'] = True
        completed = result['workerResult']
        if completed:
            result['pendingEvent'] = completed['pendingEvent']
            result['eventTail'] = 'complete' if completed['exitObserved'] else 'incomplete'
            result['workerCleanupComplete'] = completed['cleanupComplete']
        elif result['workerTermination']:
            result['pendingEvent'] = dict(last_event) if last_event else None
            if result['pendingEvent']:
                result['pendingEvent']['continuationStatus'] = (
                    'abandoned by debugger-worker termination' if injection else
                    'unknown: no successful continuation receipt')
            result['eventTail'] = 'unknown after debugger-worker termination'
        result['sourceBefore'] = source_before
        result['sourceAfter'] = {p.name: hashlib.sha256(p.read_bytes()).hexdigest()
                                for p in sources}
        result['sourceStable'] = result['sourceBefore'] == result['sourceAfter']
        result['elapsedSeconds'] = round(time.monotonic() - started, 4)
    return result


if __name__ == '__main__':
    if (len(sys.argv) == 4 and sys.argv[1] == '--fixed-worker'
            and sys.argv[3] in ('python-marker', 'native-create-event')
            and sys.argv[2] in (*MODES, 'reject-image', 'reject-create-event')
            and (sys.argv[2] not in ('reject-image', 'reject-create-event') or sys.argv[3] == 'native-create-event')):
        _worker(sys.argv[2], sys.argv[3])
    elif len(sys.argv) == 3 and sys.argv[1] == '--harmless-supervised' and sys.argv[2] in MODES:
        print(json.dumps(run_supervised(sys.argv[2]), sort_keys=True))
    else:
        raise SystemExit('only fixed harmless supervision supported; original remains HOLD')
