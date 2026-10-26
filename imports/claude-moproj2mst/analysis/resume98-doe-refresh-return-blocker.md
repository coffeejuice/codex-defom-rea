# CP98: Type2 Save refresh-return and owner boundary

## Exact missing witness

The native path's next sample call is conditional on **normal return** from
the no-argument `DoeTableRefresh` dispatch. `FUN_1400d3200` emits the signal at
`0x1400d369d` after the InputTable writer/close/error path, regardless of the
writer Boolean. The actual global dispatcher `0x1402cf3c0` consults its
suppression/registration state and, for a live registered target, invokes
`SfMoDoeTable::slotUpdateUI()` by DirectConnection. Its `invokeMethod` result
is ignored, but a synchronously entered slot must itself return for the Save
caller to continue.

The receiver is a main-window-owned/cache-backed table, not the current DOE
Study: lazy getter `0x1400196d0` uses main-window `+0x458`, constructs the
`0xb8`-byte `SfMoDoeTable`, and publishes it only after constructor return.
The table is installed through dock `+0x460`; constructor creates child
QTimer at `table+0xb0`, completes UI/setup and initial refresh, then registers
at `0x1400ebc5c` under key `2signalDoeTableRefresh()` for
`slotUpdateUI()`. On actual delivery the slot first calls `QTimer::stop()` at
`0x1400f4e25`, then calls the table's virtual `+0x1b8` refresh at
`0x1400f4e38`. That refresh begins with `table+0x98=1`, uses live UI/current
Project context and normally clears `+0x98` at `0x1400f4ded`. Static
registration does not prove a live receiver, current timer/thread state,
complete UI ownership, or normal completion of those calls in a Save run.

Same-instance binding adds a second required observation. Before XML, native
`0x140099040` has already selected the active SUB, called getter
`0x1400b5a90` (exactly `return [SUB+0x20]`), and cached that manager at
`[rsp+0x90]`. After normal signal return it passes that same cached manager to
current-PAR `0x1400d75a0` at `0x1400997b2` and mode 0/1/2 writers at
`0x140099a77`, `0x140099d7c`, `0x14009a081`. The table refresh separately
reads the then-current Project/active SUB/manager. A candidate continuation
must therefore retain the original selected SUB/Study/I/O identities through
the callback and resume against them, not resolve a possibly changed active
SUB again.

## Candidate boundary and safe increment search

Current `write.cpp` records the completed InputTable boundary, obtains the
Session's `DoeTableOwnerState`, then unconditionally stops at
`requireRefreshDelivery()`. That function checks Session scope and
main-window `+0x458` knowledge, then refuses timer/UI receiver delivery. The
candidate state object is lifecycle evidence only: it does not own a real
QObject, QTimer, live widget payload or complete global messenger population.
`Type2SaveNodePrefixResult::studyOwner` is a `weak_ptr` explicitly described
as a diagnostic lease. The XML-prefix routine validates a strong Study/I/O/
ModelManager identity while it runs, but the public path has no normal-return
continuation after the guard. Existing actual-owner tests verify XML/partial
effects and the refusal; they do not test sample calls after refresh.

No safe production increment can resume the public sample sequence
independently of this witness. Releasing the guard based on static registration,
empty/suppressed registry assumptions, the Session's projection, or a
synthetic “callback returned” value would skip the table/timer/UI phase or
invent its result. Adding another detached writer test or an owner receipt
would not make the branch reachable. Keep current writer and sample outputs
guarded.

## Concrete next action

At the existing XML boundary, obtain one source-owned receipt for an actual
Save occurrence that binds: (1) the selected SUB and its cached `+0x20`
manager/Study identity before signal dispatch; (2) the actual main-window
`+0x458` table generation and registration for the no-argument key; (3) whether
the dispatcher suppresses, finds no live receiver, or directly enters that
same receiver; (4) timer-stop and refresh-slot normal return; and (5) the
post-callback cached-manager identity used by first current-PAR output. The
receipt must preserve XML false-result delivery and partial effects. If a
static/source witness cannot bind runtime population and return, this is an
original application runtime capture dependency. No GUI or original execution
was performed in this read-only lane.

## Evidence

Original GUI PE SHA-256:
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

* `analysis/resume65-doe-refresh-owner.md`, SHA-256
  `34d23e77aee1693b151479f089fb2baf7c1a1484004cb5f773036ad74bd53855` —
  receiver constructor, registration, direct slot, timer stop and refresh
  order.
* `analysis/resume66-doe-refresh-ui.md`, SHA-256
  `0975503f6ce1b52a47e072ae052725b236328127cd5f6ce01ed2a4ec42ad17b3` —
  UI refresh state and limits.
* `analysis/resume67-doe-table-owner-lifecycle.md`, SHA-256
  `cfb8ee57da5cb32e9081fa3f477179f3f8abf486987f2d6ae6f3cd5ad6bb4c76`, and
  `analysis/resume68-doe-table-owner-state.md`, SHA-256
  `5cb50b1f6d30e3d92e5c60a51729044c9a90a94acba3a07c6e45a5c753951d9d` —
  candidate lifecycle projection and persistent refusal boundary.
* `analysis/resume90-doe-save-refresh-binding.md`, SHA-256
  `2d5e78758f7ed5928423b462d3f44c41713ee1e2dcaa2b6e10c22c0426b36f42` —
  cached native manager and sample return continuation.

Candidate source reviewed: `source/apps/moproj2mst/src/moproj/write.cpp`,
`source/apps/moproj2mst/src/doe/table_owner_state.cpp`,
`source/apps/moproj2mst/src/doe/type2_save_node_prefix.cpp/.hpp`, and
`source/apps/moproj2mst/src/doe/type2_xml_prefix.cpp`. Read-only retained
source/capture review only; no source, build, test, GUI, original execution,
or mutable REA database was changed or used.
