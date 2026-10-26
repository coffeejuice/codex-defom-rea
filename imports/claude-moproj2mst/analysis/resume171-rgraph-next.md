# RGRAPH CP171: next cut after CP164

## Finding

No isolated instruction/state cut is currently safe to integrate for populated graph cells. The narrow blocker remains the missing same-manager-generation occurrence joining mode activation (or the concrete mode-2 delegate) through model clear/rebuild to the StepData finalizer's top-level item lookups. Existing evidence identifies each side of this join, but not one accepted lifecycle occurrence spanning them. Keep the populated-cell public guard.

## Evidence qualification

- `analysis/resume164-graph-next.md` is the immediate prior boundary. Its CP159 callback and writer findings are source-qualified, but the proposed static caller trace did not establish that activation precedes finalization for the same selected owner and manager.
- CP159's `analysis/resume159-graph-callback.md` binds the derived `SfObjectManager` callback receiver, role reads/writes, selector normalization and linked-owner/sibling updates. It does not establish dispatch timing within a populated graph lifecycle.
- CP160's `analysis/resume160-graph-widget-next.md` binds the `GuiModel+0x230` manager, actual View/Settings chain, mode-1 group/object insertion, and finalizer queries. The writer creates group items under the invisible root and object items under groups; finalization calls `QStandardItemModel::item(row,column)` on the manager directly. The evidence therefore does not equate hierarchical child rows with queried top-level cells.
- The CP164 takeover entry explicitly records that graph writer/callback/Widget fragments still lack a same-manager-generation activation-to-finalizer occurrence. This is the accepted checkpoint status, not evidence that a search proved no such occurrence exists.
- Source subject for CP159/160 is the original `evidence/original-binaries/DEF_GUI_MO_64.exe`, v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. These are original binary observations, not reconstructed executable behavior. Bounded retained machine-code ranges support the stated local control/data flow; they do not establish full cold/exception coverage or dynamic callback order.

## Next evidence and acceptance test

Continue from the already decoded activation site `0x140cdfbd0:0x140cdfe77..0x140cdfe9a`; recover its concrete enclosing caller/observer path and determine whether the selected owner and manager reach `0x140c90bd0 -> 0x140d7c260` finalization in one normal lifecycle. Follow `0x140d7a9f0 -> 0x140d3f510` only if the concrete path selects mode 2. Do not repeat the completed writer or callback body decodes.

The witness passes only if it preserves selected owner identity, exact manager allocation identity, mode, clear/rebuild sequence, root/child coordinates, and normal-return finalizer occurrence. At each queried top-level `(row, 1..6)` coordinate, record whether `item(row,column)` returns null or a checkable item. If nonnull, bind its View/Settings row and callback delivery/order; if all are null, record the complete six-column null outcome for the relevant row(s). A partial trace, disconnected caller edge, isolated constructor, fixture-only result, or absence of a static xref is not a pass. Only after this test yields a concrete contract should a narrowly scoped adapter/test cut be considered.

No source, build, test, GUI, executable, or mutable REA/shared state was changed or used. This note is read-only analysis of retained evidence.
