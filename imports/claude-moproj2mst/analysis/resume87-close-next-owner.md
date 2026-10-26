# CP87: actual notifier prefix before Close child cleanup

The smallest source-closed cleanup step independent of the Report destructor/root is the **TreeEnableSelection dispatcher's suppression/flag90 prefix on the actual Session-owned notifier**. Read its current suppression byte; suppressed returns without effects, while unsuppressed stores flag90=false and stops before constructing the signal-key QString. This changes existing original-corresponding state rather than introducing another Close-state copy. It does not complete the busy setter or accepted Close, and it needs no Report document, QObject destructor or GUI execution.

Only this report was written. No source/build/tests/GUI/original execution/mutable REA state was changed. Evidence: current converter source, retained CP59/60 qualifications, and bounded offline disassembly/import/string parsing of immutable `evidence/original-binaries/DEF_GUI_MO_64.exe`, v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. No reconstructed executable/deployment was inspected or changed.

## Exact owner and ordered native edge

The native notifier receiver is singleton **143d76700**, passed as rcx at2cac8d to2cf3c0 at2cac94. It is distinct from busy-field storage reached through143d766b8 and remesh storage143d91238. Current `ConversionSession::notificationState_` is the shared candidate owner of this notifier projection; `sessionNotificationStateOwner()` returns that same owner. It already owns `suppressed`, `flag90`, per-key registration projections, empty-key observations and attempts. `StudyIo::bindSessionNotificationState` and existing main/view-manager construction share it. Do not construct a second notifier or copy its Boolean fields into the new helper.

For the pending false-Boolean request, native2ca9f0's source order is:

```text
global busy+9=1, fields+A/+B=0/0 (accepted true prefix)
2caa63: source Boolean argument false
2caa82:025080 constructs its typed argument; more QGenericArgument setup follows
2cac86: key1422d3078 = "2signalTreeEnableSelection()"
2cac8d: receiver143d76700
2cac94: call2cf3c0
only after setter return:029a61 ->02aae0 null Tree/Property/Scene refresh
only after refresh:029a76 ->c78300 model-manager child cleanup
```

The same false-Boolean notification also occurs in the later false setter's still-busy branch; that is a different invocation occurrence. Start with the accepted true-setter occurrence and do not merge their progress or schedules. The true-Boolean key at1422d30a0 has the same text, but a different argument/source occurrence.

Inside2cf3c0, the bounded body establishes:

| Site | Actual effect / next boundary |
| --- | --- |
| 2cf3f1 | Read receiver+141 suppression. |
| 2cf3fa ->2cf72e | Nonzero: direct epilogue/return2cf737. No flag90 store, key construction, map lookup, snapshot or receiver effect. |
| 2cf408 | Unsuppressed: write receiver+90=false. This precedes every following provider. |
| 2cf41f | Construct local QString from supplied key through IAT141e04480. This is the **first unimplemented provider after the flag store**, earlier than map lookup. |
| 2cf454 | Only after key construction:2d03f0 map lookup/default insertion. |
| 2cf461 |2ef070 local registration-list snapshot; later traversal/direct callbacks follow. |

The next guard must therefore be **before2cf41f**, not merely before2cf454. Keeping the flag store when this provider refuses preserves native partial progress. A preflight that rejects missing population before clearing flag90 is too early; treating the missing population as empty is wrong.

Exact import mapping confirms QtCore OriginalFirstThunk RVA350c768, FirstThunk RVA1e03b60, zero-based index292 ->IAT141e04480 ->import-name RVA35155a2 ->`??0QString@@QEAA@PEBD@Z` (`QString(char const*)`). Index193 ->141e04168 ->`??0QGenericArgument@@QEAA@PEBDPEBX@Z`. This follows CP83's section-aware indexed mapping; no symbol is inferred only from the callsite shape.

## Current source seam and proposed focused implementation

Existing `close_busy_prefix.cpp::attemptCloseBusyTruePrefix` stops at `BeforeTreeEnableSelectionDispatch`, specifically before caller argument production at2caa63. It has completed only three stores in an isolated declared global-state projection. It must not silently jump across025080/QGenericArgument setup to claim native dispatcher entry. Its actual Session/manager identity check can be reused to validate the already-reached candidate busy prefix; checking the stage must come **before** calling it, so an unstarted busy prefix is not advanced as a side effect.

The clean first increment is a separate direct-callee-entry prefix, with the caller argument-production dependency explicit. Proposed files under `source/apps/moproj2mst/`:

- New `include/m2m/engine/close_tree_notification_prefix.hpp`.
- New `src/engine/close_tree_notification_prefix.cpp`.
- New `tests/mdt/test_close_tree_notification_prefix.cpp`.
- No Session/header/member, DOE notifier API, CMake, public Close or Report edit is required for this first helper. Reserve any later shared busy-helper integration separately.

Suggested API: `CloseTreeNotificationPrefixProgress(session, alreadyReachedBusyPrefix, exactDispatcherOccurrence, qualification)` and `attemptCloseTreeNotificationPrefix(session, progress)`. Construction binds the actual manager/notifier ownership/control blocks and opaque diagnostic identities, but does not read suppression or write flag90. A nonblank label describes supplied occurrence evidence; it does not authenticate original execution or certify completed argument construction. Result flags must keep `callerArgumentProductionCompleted`, `busySetterComplete`, `modelCleanupComplete` and `completeForPublicClose` false.

Ordered helper contract:

1. Require the earlier true busy prefix already at its pending dispatch stage; validate its same actual Session/manager without permitting implicit entry/stores. Require the exact2cac94 ->2cf3c0(false) occurrence. Explicitly classify this as direct callee-entry projection; the actual caller continuation remains guarded before argument construction.
2. Get `session.sessionNotificationStateOwner()` internally, require the same canonical shared owner on every attempt, and retain only weak metadata outside execution. Never accept a detached notifier reference or caller-supplied suppression/flag90 pair.
3. On first attempt, read the actual current `suppressed` once. If true, latch `SuppressedCalleeReturn` with no mutation. This closes only the source callee's suppressed branch; it does not synthesize a native caller return/cursor.
4. If false, store this actual owner's `flag90=false`, latch `BeforeKeyQStringConstruction`, and return a typed unavailable result at2cf41f. Preserve the consumed false branch/store even if later metadata changes. Do not create/insert the Tree key, traverse registrations, record a fake callback or use `observeEmptySessionNotification`.
5. Retry reports the latched phase without rereading suppression or replaying the flag store. Reentrant/later external changes are not rolled back. A foreign/replaced/dead Session/notifier/manager refuses before effects; escaped diagnostics expose opaque identity only.

Current `removeGraphItems` in close_effects.cpp already demonstrates actual shared suppression-before-flag-reset ordering. Its key-specific population is not a Tree population. `observeEmptySessionNotification` expressly excludes TreeEnableSelection. Positive native constructor registrationd39b9c for slotEnableSelection prevents a fabricated empty default here. The new prefix needs no new population field because it stops before key construction and every map effect.

## Focused tests and uncertainty

Proposed target `m2m_mdt_close_tree_notification_prefix`, using real `ConversionSession`, its actual notifier and existing busy helper:

- Constructor/binding performs no store; unstarted busy prefix or wrong occurrence refuses before both busy and notifier changes.
- Suppressed=true with flag90=true: retain flag90, all registration maps/emptyKeys/attempts, Project/manager selection and pending timers; latch the callee skip. Do not report setter/public completion.
- Suppressed=false with flag90=true: mutate the **same notifier owner visible through Session/StudyIo** to false, then stop before QString construction with every map/recipient/timer/Project field unchanged.
- Missing Tree population does not prevent the preceding flag write, and no empty-key entry is created. Populate an unrelated known key to prove it is preserved.
- After either consumed branch, change suppression/flag90 externally and retry: retain the original stage without repeating the store or switching branches. This checks partial-progress semantics rather than mirroring private implementation.
- Foreign Session/notifier/manager, actual owner destruction/address reuse and an escaped progress object cannot authorize a different allocation. Diagnostics retain no dereferenceable historical pointer.
- Existing true busy bytes/counter stay unchanged by this helper. No Project storage release, Report/OPI expiry, receiver delivery, modal activity or public Close success occurs.

Use Release-active Testkit assertions and its TESTKIT_MAIN entry; root should group the new target with existing close_busy_prefix, close_busy_false_prefix and doe_session_notifications tests. No test was created or run in this evidence lane.

The unavailable caller argument-production edge is intentional: this helper cannot yet be called automatically from the public Close or existing busy-prefix continuation. Later integration needs the actual source-ordered argument setup, string/allocation provider, map identity/current coverage and direct callback effects. The accepted can-close/modal/busy/refresh root remains guarded. Nevertheless the proposed helper executes a real original-corresponding cleanup store on an existing actual Session owner and isolates an earlier, smaller dependency than Report deletion or nonempty model-manager teardown.

## Receipt

DUMPBIN14.44.35228.0 from MSVC14.44.35207 provided bounded source views; PE section-aware parsing supplied imports and key strings. Exact end-exclusive code-byte hashes from the immutable original:

| Region | SHA-256 |
| --- | --- |
| 1402cf3c0..1402cf40f (suppression branch and flag store) | `62cab9e98f61c018acb827dbf499bbd4ab578b3de9242e076685e69800fde92f` |
| 1402cf72e..1402cf738 (suppressed epilogue/return) | `fc7759766e2f5a8dc2c24597c5f339ffc082442a52fbb372cfeacb44de9bd511` |

Retained qualifications: `resume59-close-next.md`, `resume60-close-tail-next.md`, `wave3-normal-close-diagram-lifetime-contract.json` and current `close_busy_prefix.hpp/.cpp`, `doe_session_notifications.hpp`, `session_notifications.cpp`, `close_effects.cpp`, `conversion_session.hpp/.cpp`. This report adds no new raw capture or mutable owner and releases the read-only lane.
