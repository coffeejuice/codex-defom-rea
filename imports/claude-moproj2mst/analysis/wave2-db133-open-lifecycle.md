# Earlier Open-time Boolean apply

The saved-output recovery does not yet establish the earlier Open-time mutation. Read-only tracing identifies a concrete path that can apply an existing Object editor before selecting the first tree item again, but the first editor's construction on fresh Open still needs a resolved callback edge. No production change follows from this partial trace.

The [machine-readable chain](wave2-db133-open-lifecycle.json) records edges, source hashes and unresolved conditions. [Offline static evidence](wave2-db133-open-static-evidence.json) retains exact original function bytes for the tree click, recursive expansion and empty leave callback; the coordinator worker also exported the [tree-click body](wave2-save-diffs-300-support-140d3b7d0.c) and [expansion body](wave2-save-diffs-300-support-140d3a880.c) from its qualified private static project.

Confirmed sequence:

1. `slotProjectChanged`, `0x14003a3c0`, runs activation, then calls `0x140038950` for an active id. The already-active case returns true without reloading.
2. `0x140038950` calls tree helper `0x140d3b300`. With a model, this sets the current index to root row 0 and **also explicitly calls** `0x140d3b7d0(row0,false)`.
3. `0x140d3b7d0` resolves the tree item and emits the global tree-click signal. The retained `0x140038850 -> 0x140048640` path forwards that item to the current operation class's `vt+0x158`, Boolean's base `0x1405f36d0`.
4. `0x1405f36d0` queries the property-stack widget index. For a non-Process item when an editor already exists, it emits `signalUpdateDataInWidgetArea` **before** leave and factory replacement. The current-widget update dispatcher `0x140dae340` calls editor `vt+0x1c0`, Boolean's `0x1411e2870` apply.

This gives a precise candidate explanation if the initial tree refresh already constructed the Object editor: the explicit first-row selection afterward applies it and creates the cutter before rebuilding the editor. The existing selection-snapshot implementation then matches the new cutter rather than creating another at Save.

Two tempting explanations were ruled out. `SfObjectSetupBool::vt+0x1d0` is `0x140131160`, exactly `mov [rsp+8],rcx; ret`; leave/replacement alone cannot create a cutter. Also, `0x140d3a880` only recursively restores expansion flags, and `0x140d40d50` only appends a separator item. Neither is an initial selection/apply call.

Direct PE resolution also rules out the model-attach candidate. The actual Object-tree constructor `0x1400c4890` installs vtable `0x14215e7b8`; its slot `+0x138` resolves to `0x140dd3cf0`, a 16-byte leaf that spills its three arguments and returns. The earlier flattened-table match to `SfDragDropModel::0x140d49080` was a different class and is not used by this tree.

The remaining edge is whether the first `setCurrentIndex` Qt callbacks or another refresh construct the Object editor before the explicit `0x140d3b300 -> 0x140d3b7d0` click. Initial signal wiring, callback order and click count remain unproven. Historical `opened.png` supplies an observed state, but cannot establish that sequence. This bounded static trace is complete without justifying another production change.

Only retained source, offline original PE disassembly and historical image files were inspected. No GUI control, original application, original-function execution or build was performed by this worker.
