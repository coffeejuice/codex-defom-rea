The recovered Qt metadata separates two callbacks that were previously conflated. `slotEnableSelection(bool)` is local method6 and dispatches to0x140d3c0c0, which only sets selection mode1 or0. The busy setter emits this Boolean callback. It does not directly construct or apply the Boolean editor.

`slotMoInputModeChanged()` is local method1 and dispatches to0x140d3c070. That function obtains the current tree index and invokes0x140d3b7d0(index,true), which can reach the editor. The method identities come from the fixed PE metadata table0x142a6da40 and strings0x142a6dbb0, anchored to the retained meta-call dispatcher.

The custom notification registration strips both the leading Qt method marker and the parenthesis suffix. Therefore the constructor text `slotEnableSelection()` does not defeat a later Boolean argument: the notifier stores the bare method name and Qt resolves `slotEnableSelection(bool)`. The earlier tentative method1 mapping is superseded.

The fresh Open wrapper0x1400271b0 calls busytrue, project Open0x14001ca30, post-Open0x14002a0e0, recent-path0x14001dc00 and busyfalse. Complete-body refinement: those direct notifications occur only on the null-project close/reset branch. Successful Open instead delegates0x14002a9b0, which activates subproject0 then refreshes views through0x140038380. Early factory/apply timing remains open on that activation/view-binding chain. The0x14001c2f0 fallback is an import/archive dialog rather than normal .moproj parsing. No production change is justified by this finding alone.

Exact bytes, metadata signatures, captures, addresses, provenance and hashes are retained in [the machine-readable receipt](wave3-boolean-external-callback-contract.json). All work was offline; no original process or UI was controlled.
