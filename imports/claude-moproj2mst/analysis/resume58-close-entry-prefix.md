# Checkpoint58: actual-owner Close entry prefix

The isolated helper now recognizes the source-qualified unbusy entry and the actual Mo child whose native Project backlink is null. It stops before the whole native busy-transition call. This is a guarded continuation result, not accepted Close, cancellation, or native lifecycle integration.

Only the three new helper/test files and this receipt belong to this increment. Public Close, ConversionSession, shared model interfaces, CMake, plans, destructors and OpClass.current were not edited. No build, GUI, installed application, or mutable REA operation was run by this worker. Root owns combined validation.

## Source qualification and ordered contract

The original reference is DEF_GUI_MO_64 v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The retained-code interpretation is qualified in `analysis/resume57-close-staged-owner.md`, especially its Close-prefix discussion. The new helper preserves these boundaries:

1. Public native0299a0 supplies show=true and calls2ca920 at0299cb. In2ca920, global+9 is tested first at2ca937. Nonzero bypasses the main-window lookup. The helper preserves any nonzero byte as busy, without normalizing or writing it.
2. When+9 is zero,2ca840 obtains the actual global main-window pointer through2c8090. Null returns false; otherwise it dispatches virtual+300. For the recovered main vtable1420fb108, that slot is01daf0, whose condition includes QStackedWidget::currentIndex(main+480)==2 OR0327c0. Neither fresh-session existence nor MainView/engineMutex/MoInputMode establishes this return value.
3. Busy with show=true calls2caf10 at2ca982 before returning its latched busy result. That warning reaches QDialog::exec and has checkbox/counter/window effects. The helper therefore returns `BusyWarningEffectsUnavailable`, never an immediate terminal false/cancel.
4. Qualified unbusy proceeds to the actual ModelManager owning population. Native c9b8c0/c7a100 scans children through virtual+68. The helper iterates all actual children, including unselected ones; it does not substitute the current model. Unknown/non-Mo virtual behavior remains guarded.
5. Mo044110 tests model+19c0 at044132. Null immediately allows at04413c, reaching the epilogue without process-type/modal/Save work. The helper derives this fact from actual `DbHandle::moProject` and reuses only `close_dialog::resolve`'s immediate null-Project leaf. Its default process-type field is never used to justify allowing a nonnull Project.
6. Nonnull Project stops before044143's application process-type/conditional question route. Normal successful Open selects application process5; adapter MoInputMode/cogging process type cannot replace that owner or qualify a modal schedule.
7. After all children allow,029a54 calls2ca9f0(true). This function writes global+9 at2caa12, calls2ca920(show=false), and invokes2cb530/2cb570 plus further notification/UI work. The helper stops at its entry, before every part of that call. It does not publish a partial busy assignment or continue into02aae0/manager clear.

Previously inspected native function-body SHA-256 receipts, end-exclusive unwind bounds:2ca920..2ca990 `9918437dde0217d0f47173d73a00dd1478e99db1c3ea0a2fa43c3f42e9047434`;2ca840..2ca883 `dd05aeaa7d2cb198061a8671b030ab8878b39960946eac20ba6208446bef942a`;01daf0..01db3c `9fde859043f5266fd0d9107338d3d28a1dd5634080b8211f4ce2919e489249f4`;2caf10..2cb506 `a4b9aca0320726cf22f4880eb2c440977d45e8dc33d4a1714484e14f7a8b677c`;044110..044a81 `bcf5727ecceb436cc0c9902c90b9ff7638c74058036deeeca569470eabb405a2`. These qualify static control flow, not observations of a current runtime attempt.

## Actual owners and limitations

`attemptCloseEntryPrefix(const ConversionSession&, const CloseEntryBusyObservations&)` obtains the actual manager from the live session's `modelManagerOwner()`. It verifies both manager->ownerSession() and the observations' sessionOwner. The caller cannot substitute a separate manager carrying a plausible owner pointer. Busy observations require the exact normal-Close occurrence plus nonblank qualification. Qualified null global window skips the virtual query, and nonzero global+9 skips both window facts, matching the native short circuit.

These qualification strings label supplied observations; they do not authenticate them. There is no production owner for native global+9/main-window+300 state added here. The caller must serialize the attempt and supply facts valid for that same attempt. This helper has no callbacks or suspension and does not establish thread safety or protect against external unsynchronized manager mutation.

Session construction already allocates adapter Project storage; that is distinct from native Mo model+19c0. `prepareProjectModel` creates/registers the real Mo DbHandle with moProject still null. Only `commitProjectModelConstruction` publishes its Project backlink. The focused test deliberately uses the prepared-but-uncommitted state and proves that a live adapter Project alone does not select the nonnull branch. A committed Project remains guarded. An empty actual manager reaches the same next busy-transition guard with zero allowed Mo children.

No helper result mutates busy observations, manager selection/population/connections, pending timers, Project storage, or the native Project backlink. There are no modal deliveries, Save calls, deletes, destructor calls, or native-current expiry claims.

## Focused validation and freeze

Target: `m2m_mdt_close_entry_prefix`. CTest: `m2m.mdt.close_entry_prefix`.

Root's focused command after its source-stable configure/build is:

```text
ctest --test-dir <combined-build-directory> -C Release -R ^m2m\.mdt\.close_entry_prefix$ --output-on-failure
```

Eight Testkit cases cover actual uncommitted Mo ownership and unchanged state; nonzero-byte short circuit; missing/unqualified/wrong-occurrence busy facts; null global window; true main-window busy; committed Project refusal; unknown child virtual refusal; and owner binding/whitespace qualification. Testkit include, entry point, supported CHECK/REQUIRE macros and absence of C assert/NDEBUG-dependent checks were checked statically. Whitespace checks passed. Tests have not been compiled or executed by this worker.

Frozen SHA-256:

| Assigned file under source/apps/moproj2mst | SHA-256 |
| --- | --- |
| include/m2m/engine/close_entry_prefix.hpp | be4321c43f31bc16347ce6578ffbdfb1890b3738b778797dd29bbb4108a4512f |
| src/engine/close_entry_prefix.cpp | 3af630e8a3ac1729d939342de5344efdbbe781cf4a3ba3358f43d222124ebc3e |
| tests/mdt/test_close_entry_prefix.cpp | 49a8af2cb98e6307f3a4ea4a9ab0a138d0e456d6ce4d2a72c341f75ee7a756ce |

Remaining prerequisites are actual busy/window owner observations, the busy warning modal effects, nonnull-Project can-close process/modal/Save order, the complete2ca9f0 transition and subsequent null-binding/manager-clear/destructor effects. The OPI Props/cache/Qt lifetime prerequisites described in resume57 remain unchanged. Public Close remains guarded.
