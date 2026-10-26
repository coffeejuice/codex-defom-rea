# CP201 owned suspended launcher implementation

Prepared infrastructure for the alternative owned-launch lane in
`resume201-passive-oracle-probe-plan.md`. No original executable, reconstructed
executable, GUI, REA session or installed file was used or changed. No application
Open, Save or accepted Close observation was made.

## Files and contract

- `admin/owned_suspended_process.py`: standalone Windows Python/ctypes launcher.
- `admin/tests/test_owned_suspended_process.py`: eight Windows integration tests.

Required CLI fields are `--exe ABSOLUTE_PATH --cwd ABSOLUTE_DIRECTORY --sha256
EXPECTED_DIGEST --max-seconds SECONDS`, followed by `--` and child arguments.
The deadline must be greater than zero and at most 20 seconds, measured from
before creation; it is not reset by commands or resume. The helper verifies the
file digest before creation, passes an explicit application name, correctly
quoted mutable command line and explicit cwd to CreateProcessW, disables handle
inheritance and uses CREATE_SUSPENDED | CREATE_NO_WINDOW. CREATE_NO_WINDOW avoids
a console window for console children; it does not suppress windows belonging to
a GUI application. Running an original remains a separate execution decision.

The first JSONL receipt reports `created`, the suspended state, and an identity
containing PID, primary TID, creation FILETIME (integer Windows 100 ns ticks),
canonical resolved image path, file SHA-256, exact argv, cwd, host name and launcher
PID. QueryFullProcessImageNameW and GetProcessTimes operate on the retained
creation-time process handle. The image must match the expected canonical path.
The process and primary thread handles remain held until cleanup. No PID reopening,
process discovery, existing-process attachment or image-name termination exists.

Stdin accepts exactly one object per line:

```json
{"command":"status"}
{"command":"resume"}
{"command":"terminate"}
```

Each object must contain exactly the `command` field; each line is limited to
4096 characters and must end in a newline. `status` reports wait result and exit
code; exit is established by a signaled held handle, not by comparing exit code
with STILL_ACTIVE. `resume` calls ResumeThread on the retained primary thread
once and requires previous suspend count 1. A duplicate resume is an error that
ends the run and invokes cleanup. `terminate` ends the command loop. EOF,
malformed/unsupported commands, exceptions and deadline also end the run.

Cleanup calls TerminateProcess on the exact held handle if still live, waits up
to 5000 ms on that same handle and retrieves GetExitCodeProcess. A natural-exit
race is accepted only after the same handle proves exit. Handles are closed on
all normal/error unwinds. Successful cleanup produces `exit_verified=true`,
`wait_result=0`, exit code and `handles_closed=true`; any unverified cleanup is an
error and nonzero launcher exit, never an absence claim. Cleanup termination code
is 0xE201. A separate watchdog thread can terminate the target at the deadline
even if JSON output is blocked; lifecycle methods serialize access to held
handles. The main JSON stream can still be delayed by an unresponsive consumer.

Termination provenance is cumulative across watchdog and final cleanup calls.
`termination_requested` means at least one actual TerminateProcess invocation
was attempted during this ownership lifetime. `termination_attempts` retains
each invocation's source, requested exit code, API success/error, wait result and
verified exit result. A final close of an already exited process preserves the
earlier watchdog record rather than describing that exit as natural. API success
and independently verified exit remain separate fields, including natural-exit
races. A naturally exited child has false `termination_requested` and an empty
attempt list.

## Verification

Executed on this Windows host with Python 3.12, using only `sys.executable`
children with `-I -S -c`, CREATE_NO_WINDOW, and unique temporary directories under
converter `scratch/resume201-owned-*`. Temporary test directories were removed.
The children only sleep, return a fixed code, or write a cwd marker in their own
scratch directory. They launch no additional processes.

```text
python -m unittest discover -s admin/tests -p test_owned_suspended_process.py -v
Ran 8 tests in 2.495s
OK
```

Tests establish suspended-before-code execution, explicit effective cwd, a single
successful resume with suspend count 1, exact-handle termination, EOF cleanup
while suspended, malformed command cleanup, deadline cleanup while stdin remains
open, natural exit code 7, hash mismatch refusal before creation, and duplicate
resume cleanup. All successful cleanup receipts required a signaled process
handle and closed handles. No image-name kill was used.

The focused watchdog regression delays only the controller main thread after
the created receipt for 1 second, with a 0.5-second deadline. The real watchdog
terminates the still-suspended harmless child while that main thread is delayed.
An independent test handle first checks the recorded PID's creation FILETIME,
then verifies signaled exit and code 0xE201. The final JSON receipt must retain
exactly one successful `watchdog_deadline` attempt and
`termination_requested=true`. After all handles close, OpenProcess on that PID
must fail with ERROR_INVALID_PARAMETER (87); PID reuse or an openable object
fails the test and never triggers a kill. This local test established owned
instance exit and post-cleanup PID absence. It does not establish visibility or
absence on a REA device.

## Qualification and remaining gates

This demonstrates a local Windows process ownership/cleanup lane only. It does
not demonstrate that REA's selected Frida device shares this host or sees this
PID, nor independent same-device process enumeration absence, Frida attachment,
module identity, hook readiness, retained-file identity or any original behavior.
The plan's remaining host reconciliation, independently verified process absence,
passive hook readiness and run authorization gates remain in force.

The executable file digest is a precreation disk check, not a loaded-memory
digest or a race-free file-lock guarantee. cwd is the explicit creation request;
the harmless child test establishes its effective cwd for that test only.
The helper inherits the launcher environment, never elevates, never controls
GUI input and never manages descendants. It is suitable only for the specified
single-owned-process lane; targets that require descendant containment or hidden
GUI behavior are unsupported. Abrupt launcher termination, host failure or
unrecoverable kernel/API failure cannot promise cleanup. EOF/error guarantees
refer to Python-controlled unwinds with functioning Win32 APIs, not a kill of
the launcher itself. The helper never calls ExitProcess inside the target.

No app-control session was acquired. No app-control release is claimed.
