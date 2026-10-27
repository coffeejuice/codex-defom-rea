# CP281 harmless entry-observer implementation: original HOLD

This first increment establishes a primary-thread positive helper-entry witness in a fixed harmless Python/native child.
It does not release original execution. `ORIGINAL_RUN_ENABLED=False`; `run_original()` is a literal unconditional
exception with no process-creation branch, even if the informational constant were changed. The only process creation
accepts this interpreter, this file, and fixed child modes. There is no attach, executable/path/address argument,
GUI-control, Frida or REA access. No original application, test project, clone or converter executable was launched or
changed.

The disabled binding records original installation v11.0.2 `DEF_GUI_MO_64.exe`, SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, and the CP281 call/entry/continuation RVAs. Those
fields are a static proposed binding, not a verified runtime launch contract. This report makes no claim about a
reconstructed application.

## Implemented boundary

New files are `admin/color_original_entry_observer.py` and `admin/test_color_original_entry_observer.py`. Existing CP278
and `child_debug_event_fixture.py` remain unchanged; only the latter's Win32 layouts, aligned CONTEXT storage and API
declarations are imported. The pure predicate and handle-generation ledger have focused adversarial tests. The actual
event loop, native child and cleanup run in bounded test subprocesses.

The native surrogate has two statically fixed direct CALL wrappers with their own Windows x64 shadow space. The selected
call at allocation+4 is exactly `e8 37 00 00 00`, independently decoded to allocation+64, with continuation
allocation+9. The helper's first 16 bytes equal the original helper prefix `48 89 4c 24 08 53 48 81 ec c0 00 00 00 48 8b
05`; its final instruction is completed with a harmless RIP-relative load and synthetic epilogue. This is an allocated
native surrogate, not a mapped original PE or a copied original helper body.

At the selected DR0 exception, the loop requires the owned PID, primary TID, first-chance SINGLE_STEP, matching
exception address/RIP/armed DR0, execution/local-enable/length DR7 fields, and B0 without another B0..B3/BS/BD/BT cause.
It rereads exact CALL/prefix bytes and `[RSP]`, records RCX/RDX/R8/R9/flags/debug registers, reads and rereads eight
bytes at dynamic RCX+0x18, and requires a stable nonzero object word. The child's dynamically announced View identity is
only a harmless-fixture constraint; it is not the CP278 synthetic constant or an original ABI predicate. Unreadable
state raises explicitly while retaining raw exception/context. The first selected entry is immutable and immediately
ends in owned termination while held, then correct continuation and EXIT_PROCESS drain. No helper result, normal RET,
palette effect or completed Open is claimed.

The startup breakpoint is handled only after exact same-host ntdll file SHA-256, mapped signature at module+RVA,
first-chance address, primary TID, pre-marker phase and RIP=INT3+1 checks. All other target exceptions are forwarded
`DBG_EXCEPTION_NOT_HANDLED`. A child-owned VEH handles the custom benign exception and a separate child INT3; its
explicit RIP skip belongs to the harmless target handler, never to observer context manipulation. An unhandled
native-worker UD2 reaches both first and second chance and both are forwarded. Additional threads are recorded; only
primary DR0 is armed.

Wrong-caller entry is recorded, B0 cleared and RF set/read back before continuation. The next primary selected entry is
observed. This is an explicit one-instruction RF suppression interval and gives no absence/overlap claim. DR7 bit 10
normalized from event value 1025 to readback 1 on this host; the check preserves all other bits and records raw
before/after values. An initial overly strict all-bit equality rejected the first wrong-caller trial; the bounded
cleanup succeeded. This normalization is not a full cross-host debug-state neutrality proof.

Each event has a sequenced row before resource processing and an explicit continuation result/disposition. Handles have
generations and alias records; manual image/DLL/PROCESS_INFORMATION/retained handles close once, while event handles
close logically after documented successful exit continuations. Ledger completion is explicitly separate from
OS-invalid-handle proof. Cleanup terminates by retained owned handle, drains at most 64 additional events for three
seconds, checks the process signal with a two-second bound, then closes manual handles. The observation loop uses 100ms
polls, a 12-second bound, 512-event guard and a 256KiB JSON-size guard; a timeout fixture terminates at 0.4 seconds
after startup. The size guard is a stop threshold, not proof the final receipt is below that threshold: final
pending/cleanup/handle metadata is retained afterward. High-volume and overflow stress remain unqualified.

## Startup ordering correction and retained failure

The first implementation used CP281's proposed `CreateProcess(DEBUG_ONLY_THIS_PROCESS | CREATE_SUSPENDED) -> wait
CREATE_PROCESS -> ResumeThread` order. On this host no debug event was delivered during the complete 12-second wait.
Only after the owned termination request did CREATE_PROCESS, one ntdll LOAD_DLL and EXIT_PROCESS arrive in cleanup. The
child was signaled and all three events continued; no witness or user-mode fixture marker occurred. This falsifies the
assumed delivery order for this concrete trial; it does not establish a universal Win32 ordering rule.

Raw startup-failure fields from the tool receipt (initial implementation source hash retained; this failed source was
superseded, not frozen as a candidate):

Raw receipt: [resume281-color-entry-observer-startup-failure.json](resume281-color-entry-observer-startup-failure.json).

SHA-256: `b21814a6d6f3749c446ffa758ca808605280bd3d29e9812a79247b676654a179`. Payload bytes: 903.

Selected fields: `{"cleanupComplete":true,"elapsedSeconds":12.063,"entryQualified":false,"errors":["event/time
bound"],"eventCount":3,"exitCode":"0xe281","exitObserved":true,"ownedProcessSignaled":true,"pendingEvent":null}`.

The corrected harmless implementation issues exactly one ResumeThread, verifies previous suspend count 1, then waits for
CREATE_PROCESS. DR0 is armed later at the fixed native marker while its debug event is stopped. Receipts state
`initialEventArmed=false` and `designCreateEventBeforeResumeVerified=false`. This passes harmless primary-entry tests
but does not prove the proposed original initial-create-event comparator sequence. Independent watchdog ownership is not
established before this harmless resume, so this sequence must not be promoted into original execution.

Host/tool identity: Windows version API reports major 10, minor 0, build 26300, platform 2; Python 3.12.10
(`tags/v3.12.10:0cc8128`, Apr 8 2025 12:21:36), MSC v.1943 64-bit AMD64, interpreter `C:\Program
Files\Python312\python.exe`. ntdll SHA-256 is `651ac9041af8e6b5ebd331d53163244c0a27d36d1433f0f15cac1f28c5f2e82c`, loader
RVA `0x1226c9`, mapped signature from RVA-8 `0a807c2440007503cceb004883c438c3cc`. There was no mutable REA/tool session
and no GUI acquisition.

## Validation and remaining gates

Source-stable command: `python -m unittest discover -s admin -p test_color_original_entry_observer.py -q`; 12 tests,
2.399s, exit 0, OK. Tests cover original literal refusal and CLI refusal; positive entry; handled custom first-chance
forwarding; target-generated INT3 forwarding; wrong-caller RF progress; worker-only no primary witness; extra-thread
positive; natural pre-entry exit; timeout cleanup; first/second-chance UD2 forwarding; byte/frame/base/read/cause
predicate negatives; and stale numeric-handle-generation rejection. Pure negative sample tests do not replace OS
short-read or injected SetThreadContext failures. The child tests use `subprocess.run(timeout=22)` around the debugger
worker; that test harness is not the required independently acknowledged supervisor duplicated-handle protocol.

Original remains HOLD for: independent creation-bound supervisor handle handshake and nonresettable owner watchdog;
debugger crash/hang and handshake-failure tests; initial-event arming/readback before user code; mapped
PE/header/section/image-file identity gates and runtime dependency identity binding; fresh
reservation/manifest/path/command and source/clone before/after receipts; high-volume event/receipt cap tests; injected
memory/context/continuation failures and pending-event recovery; seeded occupied OS DR tests; OS-invalid-handle/reuse
validation beyond the pure ledger; creation-aware independent absence checks; and independent policy/neutrality review.
The code records the relevant readiness fields false. No ready-for-original inference follows from 12 passing tests. No
full CP214 chronology, all-thread coverage, uniqueness, nested overlap absence, normal return pair, color effects, Save
or Close claim is added.

The following receipt links preserve exact raw JSON payloads for independent review, including event/continuation and
handle/cleanup rows. Their normal and timeout source hashes match the source manifest below; no child was rerun for this
presentation change.

### Raw final-source normal receipt

Raw receipt: [resume281-color-entry-observer-normal.json](resume281-color-entry-observer-normal.json).

SHA-256: `6a9eae69c68af547ad59e18b5605e42f3e35c1aa78f35c829bc63f3f96ace516`. Payload bytes: 13836.

Selected fields:
`{"cleanupComplete":true,"elapsedSeconds":0.109,"entryQualified":true,"errors":[],"eventCount":41,"exitCode":"0xe281","exitObserved":true,"ownedProcessSignaled":true,"pendingEvent":null}`.

### Raw final-source timeout receipt

Raw receipt: [resume281-color-entry-observer-timeout.json](resume281-color-entry-observer-timeout.json).

SHA-256: `2571b774cb8761956bfe22efc2d80c85278c28c046fc01d4109b86f8fd793ad2`. Payload bytes: 12612.

Selected fields: `{"cleanupComplete":true,"elapsedSeconds":0.422,"entryQualified":false,"errors":["fixed harmless
observation
timeout"],"eventCount":40,"exitCode":"0xe281","exitObserved":true,"ownedProcessSignaled":true,"pendingEvent":null}`.

### Source manifest

```json
{
  "admin/child_debug_event_fixture.py": "0aea50992e9e6749f9d5f4db7092710d61ca159e1622f7fd65c275e273ed31ee",
  "admin/color_one_slot_debug_fixture.py": "7c5c1d78bc0be940cf68c73a6bebf2577079b5ad4ac9274bbc4e037bbf0f1119",
  "admin/color_original_entry_observer.py": "57df26f7409eb7c5f2b41cc37531bc8467126bdbf5a177d97135302694bda3d8",
  "admin/test_color_original_entry_observer.py": "55bf7c872bef8828b774075441b31fb9a076b036ef7cb909b1d0f6cbd709e9c4"
}
```
