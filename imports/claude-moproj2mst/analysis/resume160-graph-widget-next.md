# CP160: actual SfObjectManager/View owner bridge

## Result

The native finalizer is already bound to the actual owner, manager and View/Settings identities. The remaining admission question is narrower than a missing widget constructor: **which manager population generation exists at the StepData finalizer, and does that generation contain checkable items at the exact top-level coordinates it queries?** CP159's hierarchical object rows cannot simply be treated as those top-level cells.

Correct the CP131 and CP159 callback reports' `QTableWidget` wording. The relevant native object is `GuiModel+0x230`, an `SfObjectManager` derived from **QStandardItemModel**. Fresh import resolution binds finalizer call `d7c4ee` through IAT `141e0b658` to `QStandardItemModel::item(int,int) const`; base constructor call `d3d5a9` through `141e05a90` is `QStandardItemModel(QObject*)`. This is the same manager CP159 populated. An additional QTableWidget occurrence is not the right next requirement.

No source-safe populated-cell lifecycle implementation is admitted by this report. Preserve existing guards. No isolated helper is proposed.

## Immutable evidence

Target: original `evidence/original-binaries/DEF_GUI_MO_64.exe`, freshly verified SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, image base `0x140000000`. This is original DEF_GUI_MO evidence, not a reconstructed executable.

`analysis/resume160-graphwidget-verification.json` binds 24 assembly captures, raw machine-byte spans/hashes, assembly hashes, PE imports and direct-call scan candidates. The producer receipts are `resume160-graphwidget{-activation,-leaf,-mode,-owner,-window}-static.json`. Dumpbin completed those requests with version `14.44.35228.0`; its exact executable/hash is in the verification receipt. An initial bridge request wrote three complete captures and then stopped because `1411f19f0` lacks an unwind record. Its explicit-bound successor succeeded. The verifier covers those three initial files too. No incomplete decoder result is treated as negative code evidence.

| Native span, end exclusive | Raw machine SHA-256 |
|---|---|
| `140d3d590..140d3d66e` | `34b59bab3e6c8c0c0c0abaf5a4482fb187088e23df53f3561f359380da5376eb` |
| `140d78100..140d781df` | `1cf7df2931bdac4c8309748610cb82becdacb9779c38e286302db08c36637254` |
| `140d7c260..140d7c57d` | `3bf98283c7ac33986545da5ae00176706ccb9f7fc95f092bec2318e3df1a9643` |
| `140c82cb0..140c82cee` | `4479f4ff39dc0d62fef80a0c828ab57b90367fc776b8d40c31e7466ae607aeb6` |
| `1411f1a10..1411f1aa8` | `4d3a4165af58a72f4f2a354a0fc65a5b033a8ed3e7f3259dcbb092cd363f5dec` |
| `140d41b10..140d41b26` | `e425515d542f40f86c31e39d6101d2d3a7e87e4b5bdd76247852a04a6c37dbdf` |
| `140d7aae0..140d7ab19` | `60ffd2a02171872affe5ac0f0424f22fd1263a3499b5324138e7c578d1da7ca0` |
| `140c90bd0..140c90efb` | `76c026fea53d2d312c2dfebbc43b14ccec46e90005282d90f19ab0dd548274c6` |
| `140cdfbd0..140cdfef7` | `5938c420c93fc4254d8c53375c8207ecc79950c7f24d5e8bb18e90207a7545dd` |

Unwind bounds establish retained instruction regions, not complete cold/exception-path coverage. Leaf claims stop at decoded terminal RETs, excluding following captured functions. `d7b3f0`'s region includes six jump-table words after RET; they are decoded as table data in the receipt, not executable instructions. The E8 scan supplies candidates, not complete indirect-call coverage; conclusions below use decoded bodies.

## Constructor, connection and generation boundary

The CP159-verified `c90210` accessor lazily stores `d78100(new, owner)` at `owner+0x230` and returns that exact pointer. Fresh `d78100` confirms the constructor calls base `d3d590`, assigns derived vtable `142a3f748`, stores its incoming owner at `manager+0x50`, and calls `d41b10(manager,0)`. Fresh `d41b10` is a plain write to `manager+0x40`: constructor mode really is **0**, not an implicit populated mode.

Base `d3d590` constructs the QStandardItemModel with null QObject parent, creates its selection model at `+0x48`, then registers same-receiver `itemChanged -> slotItemChanged` at `d3d651`. Derived construction later creates the selection model at `+0x58` and registers the derived same-receiver connection at `d781ce`. Both raw connection-type arguments are 3. Their exact slot routes/effects remain those independently recovered in `resume159-graph-callback.md`. Registration precedes any later population on a normally constructed manager. This establishes construction order; event delivery, thread admission and reentrancy do not follow merely from that order.

`d78220` preserves the manager and owner and only selects `d78490` when the manager's existing mode is 1, or `d7a9f0` when it is 2. Consequently the two proven owner-update callers `c80a20` and `c80b00` do not establish initial population of a constructor-mode-0 manager. Their owner+0x10 fields `+0x204` contain 1/2, but those are a different receiver and field from manager+0x40; equating the two mode numbers would be an error.

Initial mode-1 activation has a concrete wrapper: `d7aae0(manager,force)` calls `d78490` if current mode is not 1 or force is nonzero, and returns the same manager. `d78490` itself writes mode 1 at `d79359`, after clear/header setup and before group insertion. Mode-2 wrappers `d7ab20` and `d7ab70` call `d7a9f0` with false/true respectively, under the analogous existing-mode/force check; `d7a9f0` clears the model and sets mode 2 before delegating to `d3f510`. These are distinct population layouts.

One real mode-1 activation occurrence is now decoded in `cdfbd0`: a selected owner from `cdc180` is retained in stack `+0x20`; after global owner publication at `cdfe48` and `ce2520`, predicate `2cb830(143d766b8)` gates `c90210(selectedOwner)` at `cdfe77`, then `d7aae0(manager,true)` at `cdfe84`. The returned manager feeds the next global call at `cdfe9a`. This function has not been proven to occur in the selected fresh Open-to-Save lifecycle before StepData finalization, and its surrounding observer effects are not given invented names.

No explicit numeric generation counter was identified in these bodies. The relevant native identity is the same manager allocation plus its intervening clear/rebuild sequence, mode and owner. An adapter generation must represent that sequence; it cannot be supplied by an unrelated table fixture.

## View and Settings identity is concrete

The StepData dispatcher `c90bd0` calls `c90210(owner)` at `c90ecb` and passes its unmodified return as RCX to `d7c260` at `c90ed3`, after its child loop. In `d7c260`, `c82cb0(manager->owner,&snapshot)` precedes `c7b1a0(manager->owner,true)`. Fresh `c82cb0` copies from **owner+0x18**, the actual View list. The finalizer then iterates that snapshot. The current source's snapshot-before-current-object-refresh order remains supported.

The exact eligibility and Settings chain is now decoded:

1. `1411f19f0(view)` returns `view+0x18`, its bound object pointer. `1411f1b80` returns false when that pointer is null; otherwise `834fa0` returns `object.kind_at_0xc != 7`.
2. `1411f1a10(view)` obtains the guarded owning model through `1411f1980(view) -> c1aef0(view+0x10)`. Null owner returns null.
3. `1411f1ab0(view)` returns the bound object's integer ID at `+8`, or zero for no object. The getter looks up `c832d0(actualViewOwner,&ID)`, whose front gate permits only positive IDs within that owner's Settings list at `+0x20`.
4. If that lookup is null, it obtains the actual View's position through `c83080(actualViewOwner,view)`, then repeats `c832d0` with that position. This is the same identity/fallback rule the current `settingsOf` source models. It is not merely a row ordinal supplied by a fixture.
5. Nonnull Settings admit the next finalizer row. Columns 1..6 are queried on the manager; only an existing checkable item proceeds. `d7b3f0` maps columns 1..6 to Settings selectors 0..5 through its verified six-word jump table. `1411eebd0(settings,selector)` feeds `setCheckState`, after which the eligible row advances.

CP159's `d79750` uses the same `1411f1b80/1411f19f0/1411f1a10` chain on Views supplied by `c82d50(owner,&ordinal)`. Fresh `c82d50` confirms it selects ordinal-1 from owner+0x18, growing through `1411f07d0(newView,owner)` if needed. Thus the state identity bridge is established; the remaining gap is population generation and item coordinate identity, not which Boolean array belongs to a View.

## Smallest missing occurrence and next action

CP159 proves `d78490 -> d3fc70` appends a **single group item beneath the invisible root**, then `d79750` appends each object row **beneath that group**. Conversely `d7c260` calls `QStandardItemModel::item(row,column)` directly, without a group/parent argument. The already-retained Qt item() qualification establishes that this query does not recursively search the group's child rows or create them. Hierarchical child checkboxes therefore do not prove a present item at the finalizer's top-level `(row,1..6)` coordinates. The mode-1 writer alone supplies no such positive witness.

Next obtain the smallest same-manager lifecycle occurrence spanning **activation/clear/rebuild to `c90ed3`**, recording owner, manager allocation, mode, root-vs-child coordinate path and intervening model mutations. A positive admission must show a checkable item returned by the exact top-level item() query, its Settings owner/ID and callback delivery at that generation. A normal-return negative occurrence with all six queried items null is also useful: it would justify the absent-cell projection despite hierarchical population, rather than asserting that a populated manager necessarily reaches the checkbox setter.

For static continuation, start with the concrete mode-1 activation at `cdfbd0:cdfe77..cdfe9a` and determine its enclosing caller/observer admission relative to the selected owner's StepData read. Reuse these captures and CP159; do not retry the completed writer/callback bodies. If this lifecycle instead selects mode 2, the specific delegate to inspect is `d7a9f0 -> d3f510`, before importing any mode-1 row contract. `source/.../engine/db_load.cpp` currently lists the owner-update/notification tail as UI work in its comment at lines 415–417; this source comment is a candidate integration boundary, not proof the native path is unreachable.

Only new analysis script, receipts, immutable-byte captures and this report were written. No C++ source, build, test, original process, GUI session, live REA retry, mutable analysis database or installed program changed. Token measurements are unavailable; no speed or token-savings claim is made.
