# CP75: actual constructor lineage and the reached initial-null edge

Retained original evidence establishes a historical main-window constructor return on the same owner later selected for native CLI Open. Together with the unconditional constructor/initializer bodies, this establishes that its `1400184fa` null store was reached before that return. It does **not** establish the field's value at return, an exact globally first store occurrence, or a reached constructor cursor in today's candidate Session. The Boolean editor has a separate heap allocation and ownership chain; the retained editor probe selected a different editor class.

This lane wrote only this report and unique bounded evidence below. No source/build/tests/GUI/native execution or mutable REA. Original EXE SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

## Main-window owner is an application-frame object

Retained `wave3-editor-owned-open-entry-140001410.asm` shows native application code loading `rcx=&[rsp+40]` at `140001b34` and calling `140010490` at `140001b39`. Thus this selected main window is constructed in the caller's frame, not allocated by the Boolean factory and not identical to SfMoMainView, Project, diagram receiver, area, or impl. A reconstruction's lifetime token can represent this canonical main-window object without mimicking its stack address, but cannot invent a heap allocation/free for it or transplant a captured address to another run.

Constructor10490 preserves its `this`, calls base construction, installs primary vtable `1420fb108`, calls initializer106d0 at1058e and setup111c0 at10598, then returns. The retained complete selected ctor and initializer bodies have no conditional branch bypassing the initializer call or `106d0:1080c ->184f0`; exceptions/failure to return remain distinct. Leaf184f0 stores qword0 at `[this+3c8]` at184fa and returns18505. Its callback-free field write is qualified by CP71.

## Historical reached-store inference is now tied to a retained owner

`build/original-editor-probe-004/capture-evidence.json` retains these observations:

| Message index (zero based) | Observation ordinal | TID | Stage and owner |
| --- | --- | --- | --- |
|53|1|30444|`mainwindow_return`, owner `0xafdce0`|
|54|2|30444|`native_open_enter`, same owner `0xafdce0`, selected `C:\PROBLEM\m2m42\DB_deconstruction_02.moproj`|
|58|6|30444|`post_open_enter`, Project `0x3797390`|
|59|7|30444|`editor_constructor_enter`, editor `0x383e780`, Operation `0x37bdbe0`|

The retained script hooks10490, selects the first encountered owner, and records its onLeave; selected Open additionally requires that same owner, mainReady and native return address RVA1c94. The root receipt identifies PID27216, module base `0x7ff7c94b0000`, verifies the original executable/DLL hashes, records zero observer errors, and verifies owned-process absence after capture. This is inspection of historical evidence, not new process/app control.

For that selected normal constructor return, source order therefore supports **at least one earlier initializer store for that owner**. The store itself and its field value were not hooked/read. The script has no184fa/184f0/+3c8 hook or field observation. Intervening setup already invokes the area's lazy getter and can create/commit a nonnull area. Constructor return cannot serve as a known-null current-state witness.

This does not establish a globally first occurrence: earlier/reentrant helper executions and full invocation population are not enumerated by the bounded callback list or E8 scan. It identifies the selected outer constructor's own initializer edge. “First recorded constructor return” also differs from “first instruction executed in fresh application state.” The root receipt explicitly qualifies native CLI startup Open before window show/event-loop and current user settings; it is not interactive File/Open timing or pristine-settings evidence.

## Boolean allocation and later pointer adoption are separate

New immutable-PE capture `resume75-boolean-factory-1411e0510.asm` positively decodes the Boolean factory branch:

- Key comparison at1411e0630..0654 selects its Object branch (the exact key is source-qualified by `wave2-db133-boolean-contract.json`).
- Allocator14146eabc is called with size0x50 at1411e0656..065b. The actual result is saved at0660 and null-tested at0665.
- Success supplies that allocation as ctor `this` at0672 and the actual Operation argument at066d; call1411e0677 invokes Boolean ctor1411e2480. The returned pointer is retained at067c and copied into the factory result at069a..06a4.
- Null local result reaches the separate base factory fallback1405ff0b0 at1411e06c1. Do not model allocator failure as a successful empty Boolean editor or claim the whole factory necessarily returns null.

CP62 qualifies Boolean ctor's parentless base130740, primary vtable142cd9b18, UI allocation/setup and Fill1411e25b0. Operation/editor guard fields do not make Operation the QWidget parent or field+3c8 owner. CP70 qualifies outer factory return at1405f4756/4759, typed QWidget* emission at5f499f, actual area's one-argument clone, impl replacement, parent/layout/show/Enter, and current guard commit only at140dae213. Those callbacks still need actual owner/lifetime and execution admission. A factory-local returned pointer is not yet a current-widget commit.

The observed editor in probe004 is selected by a hook at RVAeb26f0 and later reaches its ed9830 setup, **not** ctor11e2480. Its parentless QWidget observation cannot be reclassified as an observed Boolean constructor or Boolean Apply. No retained capture examined here directly binds the first Boolean allocation, first area/current adoption or first timer-triggered Apply to fresh Open.

## CP65/68 reconciliation and smallest implementable edge

Current inspected Session still has no WidgetArea owner/cursor binding. CP68's typed window owner records initially unknown `area3c8`; its initial-observation API cannot replace known null with nonnull. CP65's lazy parent separately owns its mutable optional field and commit count. Shared Session lifetime or matching raw identities alone cannot reconcile these two cells.

The safe integration sequence remains:

1. Session retains one canonical main-window identity/projection with current fields unknown. Optionally retain a separate **historical initialization-reached** receipt for an explicitly selected original capture; do not expose that receipt as current area state or native liveness.
2. A candidate constructor cursor, or a bounded original observation cursor at184fa, admits exactly that owner's selected source occurrence and writes one canonical live field to known null. It needs no Qt receiver delivery inside the leaf. It must reject foreign/released generations, arbitrary qualification strings, replayed occurrences and overwriting later state.
3. Use the same field as CP65's lookup/commit storage, or add a source-specific parent-field transition at184cb after the actual area's constructor return. Keep CP68's immutable initial-observation semantics for initial observations. Before the constructor continuation is supplied, stop at the area/impl/Qt registration guard and retain null/unknown according to the actual reached cursor.

Historical ctor-return evidence closes the question “did this selected original main window traverse the initializer?” It does not execute missing candidate base/helpers, so it cannot justify jumping today's public Session cursor to that point. The smallest **effect** is still184fa's single null store; the missing implementation dependency is its actual Session/main-window constructor occurrence bridge. The smallest **immediately safe ownership increment** remains an unknown Session-held main-window projection. No fresh-Open Apply, timer readiness, stack selection, receiver registration or public PostPending effect can be inferred from either increment.

## Unique evidence and limits

- `resume75-boolean-constructor-carriers.json`: executable-section E8 carrier search returns selected direct sites1b39->10490,1080c->184f0,11e0677->11e2480. Positive disassembly qualifies these hits; the scan is not an exhaustive callgraph, write set or occurrence count.
- `resume75-boolean-factory-static.json` and its1411e0510 asm: new bounded unwind-region decode of factory allocation/constructor/fallback. No target binary was loaded/executed.
- `resume75-boolean-owner-retained-receipt.json`: compact extraction of five original observations, original PID, and SHA-256 bindings for raw capture, script, root receipt and relevant retained code bodies. The source-order inference is explicitly separate from directly observed field state.
- CP71/73/70 reports and original factory contract retain the constructor/null-store, ownership and Qt continuation qualifications. No source files were changed or tested in this lane.
