# CP76: native process-type refresh versus DBLIST admission

## Finding

The existing `ApplicationStateProjection` models only the native `+0x0c`
process-type result. It does not provide the earlier DBLIST `+0x1c` admission
decision, and there is no candidate application lifecycle owner or real native
Open/Close callsite wired to it. It is therefore a useful isolated projection,
not an integration point that can admit public DBLIST handling.

The earliest source-qualified candidate boundary is not DOE parsing. Native
DBLIST dispatch first tests `0x143d766b8+0x1c` through
`FUN_1402cbec0(0x143d766b8)` before reaching DBLIST's manager/path processing.
Only after DBNAME iteration and its earlier effects does the handler consult
process type `+0x0c` for warning behavior. A process-type observation of 5
suppresses that warning only; it neither changes the prior admission test nor
rolls back manager effects. The two DWORDs must remain independent.

## Native refresh and first real owner/callsite

The global object at `0x143d766b8` is application/process scope. Its first
qword is the native main-window pointer; the DBLIST handler then obtains the
native `ModelManager` from `mainwindow+0x50`. The process-type DWORD is at
global `+0x0c`; DBLIST admission is at `+0x1c`.

The closed process-type update is `FUN_140013d40` mode 0. It stores 5 when
there is no current Project, stores 5 when the current Project's native-open
success byte at `Project+0x55` is true, and stores 1 when a Project is present
and that byte is false. The first retained normal Open registration callsite
is `FUN_14002a0e0+0x3b` (`0x14002a11b`), which calls
`FUN_140013d40(main,0,0)` after the loaded Project success state has been set.
The retained normal Close route also reaches `FUN_14002a0e0` after manager
clear and refreshes process type; it does not establish a reset to zero.
These are native callsites, not callsites in the converter candidate.

The exact candidate-side owner needed before integration is an application
object whose lifetime spans successive Project/Session opens and closes and
which owns (or is explicitly bound to) the current Project and native-open
success observation. It must receive the qualified mode-0 refresh at the
equivalent post-Open registration and Close-refresh occurrences. No such
application lifecycle owner is present in the inspected converter path: its
`ConversionSession`/`ModelManager` are conversion-scoped, and
`ApplicationStateProjection` is currently just a separately instantiated
value object used by its focused test. Do not attach it to a `ConversionSession`
or infer its inputs from `Session::actualSession`.

## DBLIST admission remains unresolved

The `+0x1c` gate is evaluated before the handler's first file/manager effects.
The retained helper `FUN_1402cbf30` writes 1 to `+0x1c`, but there is no
qualified caller/receiver or reset lifecycle in retained evidence. Thus even
a correctly integrated process-type refresh cannot safely supply the guard.
The current projection intentionally has no `+0x1c` mutator; its value remains
unavailable. Keep public B127 DBLIST dispatch guarded until the setter's
application owner, caller and all relevant transitions are recovered and
represented.

## Candidate boundaries observed

* `source/apps/moproj2mst/src/doe/application_state_projection.{hpp,cpp}`
  exposes `observeMode0ProcessTypeRefresh(...)`, setting only `processType_`;
  `dbListAdmission_` remains unavailable with no setter API.
* `source/apps/moproj2mst/src/moproj/write.cpp` reaches the Type2 prefix and
  then `DoeTableRefresh` delivery. Delivery is explicitly guarded; this is a
  Save callback boundary, not the original application's process-state owner.
* `source/apps/moproj2mst/src/doe/session_records.cpp` still refuses DBLIST and
  ReportList at the public import boundary. The existing pure DBLIST reader
  and manager prefix do not supply native global state or native manager
  ownership.

No smallest safe public integration branch is currently closed. The next
admissible source task is to introduce/identify the real application-lifetime
owner and bind only the occurrence-qualified `+0x0c` refresh, keeping `+0x1c`
unknown. A separate evidence task must resolve `FUN_1402cbf30` and all
admission transitions before any public DBLIST branch can be enabled.

## Evidence and limits

This report uses the retained static/source evidence only. Controlling original
PE SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
Key receipts: `analysis/resume66-dblist-global-lifecycle.md` (SHA-256
`e57b855a498e2cdc8d760663a829e7fbe849f73b925a2c4e64440d122bbbf90e`; see
file hash for exact unspaced digest),
`analysis/resume75-doe-dblist-global-owner.md` (SHA-256
`35ba1f127aff7a083b94a7a11b506e10e177e24bd9bc0dce911f1f4243e4d5e4`),
`analysis/wave3-closure-next-140013d40.asm` (SHA-256
`d581ce0d292e03e3433cdcf8bd2e477bf75b7cdf0d93e1538803f090c7139d10`),
and `analysis/wave3-boolean-external-callback-14002a0e0.asm` (SHA-256
`e3e74166115892e04fe23160955278cf3101abacb6d818cd41746bca94c43f63`).

This was bounded offline evidence review. No source, build, test, GUI,
application, or mutable REA state was changed or run. No public DBLIST path is
enabled by this report.
