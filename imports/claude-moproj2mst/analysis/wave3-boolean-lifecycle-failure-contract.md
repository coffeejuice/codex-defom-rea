# Preserve writer gates and failure continuation

The next correction should preserve the original MST-open gate and each writer's continuation behavior. The current deferred atomic runner can produce the correct successful files while changing which files survive a failed Save. The [static receipt](wave3-boolean-lifecycle-failure-static.json) binds this finding to the original executable, eight bounded function disassemblies, retained source qualifications and current candidate hashes.

## Earlier Open remains a separate dependency

The original `SfTreeView` constructor `0x140d39aa0` installs vtable `0x142a0b158` and directly connects only `expanded` to `slotItemExpanded`, `collapsed` to `slotItemCollapsed`, and `clicked` to `slotItemClicked`. Its `setSelectionModel`, `selectionChanged` and `currentChanged` virtual entries resolve through six-byte import stubs to QTreeView base methods. The Object-tree base model constructor `0x140d3d590` allocates an ordinary QItemSelectionModel and connects the model's `itemChanged(QStandardItem*)` to its own `slotItemChanged`. It installs no current-selection callback.

The latter body is retained in `wave2-doe-lazy-empty-finalizer-tree-decompile.json`, acquired by the private static owner and independently disassembled here. The matching code-identity qualification is linked in the receipt. Shipped QtGui QTreeView::currentChanged `0x654dfa70` is also retained, but its transitive base/virtual/event effects are not claimed closed. These facts exclude a constructor-installed custom selection callback as an established explanation; they do not prove absence of external connections or first-editor creation elsewhere.

No early-Open creation was added. The confirmed existing-editor apply path and unknown first-editor event remain as recorded in [the earlier lifecycle report](wave2-db133-open-lifecycle.md). Historical screenshots alone do not establish signal count, order, or a fresh default-only Open contract.

## Proven write contracts

| Site | Original behavior | Current difference |
|---|---|---|
| `0x1401313c0` | Opens the destination MST with QFile mode 2 before header/body/operation dispatch. Failed open returns false and never runs those generators. | `writeMainMst` runs before the final physical MST open. Boolean side files can change even if that open will fail. |
| `0x1411e1820` | Attempts Boolean.BWZ (`0x1411e1970`), then the control INI (`0x1411e0e40`) regardless of the BWZ result; returns the BWZ result. | `applyEffects` throws on the first physical write failure, preventing the next attempt. |
| `0x1411df6d0` | Ignores the side-file helper result and continues with EXECMD and later MST text. | A failed side-file write can prevent writing any MST output. |
| `0x1411e1970`, `0x1411e0e40` | Construct QFile on the final name, open mode 2, return false if open fails, create no directory. Their successful-open paths close and return true without explicitly checking QTextStream status. | Shared runner creates parents, writes a temporary file, replaces the target and raises write errors. |
| `0x1400980d0` | Runs current-operation Save, project writer, operation property writers, then MST. It does not skip later stages solely because the project writer returned false. | Shared exceptions stop later stages. Physical Cogging.MDT output is deferred until after generic step-5 files. |
| `0x1400a20d0` | Saves the result from `0x1401313c0`, then still executes permitted study calls `0x1400ba650`/`0x1400ba800`; returns the saved MST result. | A final MST write exception exits the adapter before any equivalent later native stage could run. |

The first three findings are a focused Boolean failure dependency, not an instruction to redesign every writer at once. `.moproj`, Cogging.MDT, DOE and each other operation writer have different return and status contracts. The earlier [Save audit](save-semantics.md) remains the source qualification for project/Cogging details.

## Small correction boundary for the coordinator

Add an internal native Save coordinator seam without changing the `m2m_*` ABI. Keep the recording path usable for byte tests. For native full Save, apply earlier stages at their original boundary, acquire the final MST destination at step-5 entry, and dispatch MST generation only if that acquisition succeeds. Do not open it before the earlier property writers, since that would change failure order again.

While generation runs, route each Boolean attempt through a direct-destination, no-parent-creation writer at the existing Boolean call site. Return success/failure to the Boolean helper; always attempt the second file, and keep emitting MST text after either failure. An in-memory stream may still collect text for adapter diagnostics, but deferring all physical attempts until after complete generation cannot reproduce native state-dependent failures.

The first implementation should establish the failed-open contract and ordered attempts. Exact Qt buffering, endl flushes, short-write residue and close-time error behavior need a subsequent writer-level dependency. A generic throw-on-error or continue-on-error setting for every writer would erase recovered differences. The original GUI Save return must also be kept distinct from the adapter's existing error-code mapping until that mapping is explicitly chosen and documented.

## Focused failure fixtures

Use a recording injected native-I/O seam to assert order and branch decisions; then use candidate-only disposable filesystem fixtures for actual failed opens. No new original execution is needed to establish these already source-proven branches.

| Fixture | Expected original attempts/effects |
|---|---|
| MST path occupied by a directory | Earlier Save stages still run; MST open fails; no MST generator, Boolean.BWZ or control-INI attempt. |
| Valid MST, Boolean.BWZ path occupied by a directory | MST opens first; BWZ open fails; INI is still attempted; subsequent MST generation continues. |
| Valid MST and BWZ, INI path occupied by a directory | BWZ is written; INI open fails; subsequent MST generation continues. |
| Both Boolean destinations unavailable | Both attempts occur in BWZ→INI order; no implicit task-directory creation; MST generation continues. |
| Project target unavailable, writable task/MST paths | Project failure does not itself suppress the property pass or MST/Boolean attempts. Preserve separate status qualification. |
| MST unavailable with an eligible DOE study | MST generation and its Boolean attempts are skipped; the source-permitted study follow-up calls still run after the failed MST stage. |
| Two property/side-effect stages target an observable shared path | Assert original call order, including Cogging property writes before step-5 side files; do not group all writes after all directory operations. |

A later partial-write fixture must assert the original stream's actual byte residue and following calls, after its buffering/flush contract is recovered. Successful-byte equality alone is insufficient for that claim.

This checkpoint contains analysis and reproducible static tooling only. No production source was edited, no build was run, and no original process or UI was controlled.
