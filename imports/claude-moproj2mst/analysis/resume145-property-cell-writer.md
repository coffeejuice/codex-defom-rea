# CP145: property-cell writer and derived-slot static boundary

## Outcome

No new retained static edge closes either the property-cell writer or the derived `SfObjectManager::slotItemChanged(QStandardItem*)` body. The public update path is now qualified down to a manager virtual call, but its concrete body/data flow is still the missing link. Keep the populated-cell refusal; no source-ready cell projection follows from the evidence collected here.

## Bounded review

The original executable remains `evidence/gui_mo/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` (identity inherited from CP142–144). I reviewed `analysis/resume144-property-cells.md`, `analysis/resume143-property-cell-writer.md`, `analysis/resume142-property-cells-next.md`, the retained `evidence/gui_mo/s1/vtables.tsv`, and searches confined to retained `analysis` plus `evidence/gui_mo/s1/mdt` source/decompilation/capture files.

The class vtable confirms the relevant direct targets: `SfObjectManager` slot `+0x138 = 0x140d7a760` and slot `+0x140 = 0x140d78220` (`evidence/gui_mo/s1/vtables.tsv`, class rows). The existing caller proof remains `0x140c80a20` / `0x140c80b00` in `evidence/gui_mo/s1/mdt/DB/d3.c`: both retrieve `0x140c90210` and invoke the latter slot on that returned manager. CP144's bounded byte findings further identify calls from `0x140d78220` into `0x140d78490` and `0x140d7a9f0`; these are candidate helpers on the update path, but no retained body capture ties them to `QStandardItem` allocation, `setCheckable`, insertion into the `c90210` model, or item roles/check state.

The retained evidence also continues to show base and derived `itemChanged(QStandardItem*) -> slotItemChanged(QStandardItem*)` registrations in the constructors and an empty root model at construction. The base method body at `0x140d3d8c0` is known, but the search found no derived meta-call/body capture associating the class string with a target VA. The string occurrence in `evidence/gui_mo/strings.txt` is an image file offset, not a code address. Nearby QStandardItem operations or generic Qt model virtuals cannot substitute for same-owner receiver binding.

## Exact missing dependency

The next bounded query must recover either:

1. the body/data flow through `0x140d78220`, `0x140d78490`, and `0x140d7a9f0` far enough to identify the QStandardItem constructor and the exact model insertion/checkable/role operations, preserving proof that the receiver is the lazy `GuiModel+0x230` manager; or
2. an actual caller that populates that manager if these helpers only delegate further.

Separately, bind the derived `slotItemChanged` method's meta-object dispatch entry and body, including state/notifier effects and relation to the base slot. Until both owner identity and callback effects are recovered, a synthetic checkable item test would validate invented setup and cannot establish native behavior.

No mutable REA session, source edit, build, test, GUI, or original execution was used. No new PE capture was created; this report adds a bounded retained-file review, not new machine-code evidence.
