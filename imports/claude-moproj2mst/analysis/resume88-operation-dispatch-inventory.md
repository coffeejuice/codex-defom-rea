# CP88: Operation/property dispatch inventory and next safe boundary

## Conclusion

The current retained evidence does not identify a remaining Open/Save/Close operation-class branch where a production source change can safely advance **public lifecycle behavior** without first closing another source-owned state/receiver dependency. The active Save class handlers with closed contracts are already implemented. The remaining high-impact reachable guards—DoeOutput and Report—are deliberate boundaries around a missing runtime-owned model/document path. Removing either guard or pretending its callback succeeded would invent behavior.

The best next evidence-backed operation branch to keep queued is **DoeOutput active Save**. Its exact native call and output order are known, and candidate source reaches its explicit guard, but current source change is not independent of the missing refresh receiver/DBLIST owner. Keep the guard until that source ownership boundary is closed.

## Inventory

| Branch | Native source contract | Candidate state | Why it is or is not a safe independent source increment |
|---|---|---|---|
| Base active-operation Save / class `vt+0x68`, property `vt+0x60` | `0x140052ec0` calls the class Save slot; `0x1400543a0` performs property-to-engine dispatch. | Base order is integrated; `ShapeOptimClass::propsToEngine` handles its qualified `vt+0x60` fields and tests both widget-update sides. CopyMirror and Convert2Dto3D also have class overrides. | No remaining omission identified by current source/reports; do not reopen earlier ShapeOptim audit.
| Step-4 property `vt+0xb8` writers | Native `0x1400982e0` traverses operation-manager rows and invokes property writers before MST generation. Boolean `0x1411e09a0 -> 0x1411e1820` is called here and again during its MST block. | Cogging, Boolean, L6 and Express handlers are present; Boolean's earlier BWZ/INI attempt was integrated separately from the MST call. | No source-closed missing family identified by the current inventory. Existing results and writer failure-order tests bound these paths; avoid another generic dispatcher change.
| DoeOutput active Save (`SfOprDoeOutput vt+0x68`) | Original `0x140e2e730` uses the viewer model to write a sample object, then the DoeOutput document (`SubProject+0x30`). After a normal Type2 refresh return, native Save writes `Control/DEF_DOE.PAR` at `0x1400d75a0`, then mode writers `0x1400d7790` in order. | `save_current.cpp:757-760` explicitly refuses before those effects. The existing XML writer reaches `requireRefreshDelivery()` and remains guarded. | **Not independently implementable.** The pre-writer refresh event requires the actual sender/receiver return; the DBLIST entry gate reads process-global `0x143d766b8+0x1c`, whose owner/reset is not bound to the candidate session. Installing the known post-return `.PAR`/sample order before normal callback return would invent reachability.
| Report active Save (`SfOprReport vt+0x68`) | Original `0x1412adf90` invokes document-save `vt+0x80` (`0x1412aeb30`, `.DS`) then `0x1412af0c0` for viewer-model work. | `save_current.cpp:761-764` refuses. Candidate Report document slot models only declared null/non-null observations; its normal Close producer/destructor binding is not integrated. | **Not independently implementable.** A pure path/slot helper cannot supply the actual Report document, current viewer models, or their lifetime/order. Keep the active-Save guard.
| Distortion contact-pair default override (`vt+0x198`) | `0x14005a8a0` rebuilds system contacts; L6 specification identifies Distortion as owning its own `vt+0x198`. | The default-pair path remains an explicit refusal in `rebuildSystemContacts`. | Native override body/field-to-pair semantics are absent from retained evidence. Substituting the base contact-pair algorithm would be unsupported; first recover target/body statically.
| Pre-operation classes (`SfOprPre` / `SfOprPre2D`) | Native `vt+0x68` is empty. | Candidate returns without writes. | Already represented; no source change needed.

## Recommended next contract: DoeOutput, guard retained

The exact candidate integration checkpoint is the first post-refresh `.PAR` output at `source/apps/moproj2mst/src/moproj/write.cpp:143-161`, followed by `0x1400d7790` mode 0/1/2 writers. The current code writes its earlier `.moproj` prefix, marks the refresh boundary, then refuses when `DoeTableRefresh` delivery is unqualified. `table_owner_state.cpp::requireRefreshDelivery` is the source guard; it must not be replaced with a fabricated normal callback return.

The next independently useful work is evidence/ownership closure, not removal of `save_current.cpp`'s DoeOutput guard:

1. Recover/validate the actual `DoeTableRefresh` sender, receiver, event/callback and normal return path on the selected Open/Save route.
2. Separately bind the actual process-global DBLIST admission receiver tested at `0x140651eba..0x140651ed1`; the known `+0x1c=1` helper does not yet have a direct caller, and a zero initializer found for another base is a different global.
3. After those inputs are bound, use the existing `m2m.doe` writer tests to assert the mode order, first `.PAR` output, writer-false continuation, and native project-prefix partial effects. Until then, the precise candidate assertion is refusal before `.PAR`/sample writer entry.

The Report route is a separate, higher lifetime burden: accepted Close must be tied to the actual same Project/SubProject/QPointer/document/MainWindow identity, then its destructor/parent ownership and viewer-model route must be closed before the Save guard can move.

## Evidence

- `analysis/resume74-operation-dispatch-next.md`: reconciled class/property Save-family inventory; explains why DoeOutput/Report are not generic handler omissions.
- `analysis/resume87-doe-output-next-evidence.md`: DoeOutput post-refresh outputs and independent DBLIST global owner blocker.
- `analysis/resume87-report-ds-owner.md`: Report document QPointer construction/release and active Save owner boundary.
- `source/apps/moproj2mst/src/engine/save_current.cpp:151-216,745-791`: current property/class dispatch and DoeOutput/Report/Pre branches.
- `source/apps/moproj2mst/src/core/convert.cpp:357-385`: candidate step-4 property loop.
- `source/docs/re/moproj2mst/mdt/spec.md` §1 and `source/docs/re/moproj2mst/mdt/L6/spec.md` §7: native Save/property order and L6 class-slot map.
- `analysis/resume47-operation-override.md`: Distortion `vt+0x198` target-body evidence gap.

This inventory is read-only. No source, build, test, GUI, or mutable REA operation was performed.
