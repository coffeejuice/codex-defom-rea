# Checkpoint 47: complete-project connector fixture

## Recommended input

Use **51_MO2_Operation_Cycles2 (SI)**, copied into two independent disposable project directories before either run. It is a complete `.moproj` plus saved `.MST` corpus case in the original project corpus. The repository's Oracle-B runner documentation already selects this exact project as its 2D reference input, and the lifecycle source notes identify `MO2_Operation_Cycles2` as the corpus example with a `SfOprCycle` item whose `CycleCount="9"` and one member. The source analysis also reports 9 cycle output blocks for that count.

The canonical retained corpus is documented as `deform-rea-work/gui_mo/s1/corpus_b/` (see `source/docs/re/moproj2mst/runner/oracle_b/README.md`). This checkout does not contain the case payload itself, so resolve the exact case-directory and project filename from that corpus manifest before running; do not substitute the similarly named 2013 BEND_2D case. Preserve the corpus original and use fresh copies for original and candidate runs.

Before using it specifically as a combined-connector integration case, inspect the copied `.moproj`'s `DiagramItem` order, class names, `Connections/ItemIndex`, `CellPos`, and Cycle group bounds. Confirm the Cycle item lies on the chain and has a reachable incoming and outgoing edge, ideally one with dimensional compatibility that would fail if Cycle defaulted to the wrong single type. The retained summary proves the project contains a Cycle and exercises cycle serialization; it does not by itself enumerate its connector neighbors or prove a particular 2D/3D bridge edge. If those preconditions fail, keep this as a complete Cycle serialization fixture and use a separate constructed disposable project for connector adjacency, clearly labeling that constructed input as synthetic.

## Paired run and checkpoints

Run each copy from a fresh original-equivalent application state, with the same project-relative filenames, dependencies, default dialog answers, locale, and working-directory conditions. Capture the original run first, then the candidate run against an untouched copy. Save and Close both; preserve the initial trees and post-run trees separately.

Compare at these checkpoints:

1. **Open result:** accepted/refused status and error/dialog result; project title/path; sub-project count/type/id/active state; loaded state; operation registry identities and dimension flags; diagram item count/order/class/id/name; Cycle `ItemData CycleCount`; cell/group geometry; graph `next`/`prev` links; active item; and any defaulted properties/state.
2. **Save result and sequencing:** API/adapter status, error code/message and continuation point; which operation writer ran and in what order; Cycle iteration context and number of emitted blocks; candidate/current item state; whether failed writers leave earlier outputs in place.
3. **Exact filesystem delta:** recursively compare relative paths, file types, sizes, hashes and bytes before/after. Include `.moproj`, `.MST`, per-operation task files, `Cycle.CWZ`/KEY/MDT or other side files where present, directories created or removed, and temporary files. Retain raw byte differences beside any source-justified normalization.
4. **Close/reset:** close result, final in-memory project/sub-project/operation ownership and graph state, destructor/notification effects, remaining output files, and whether a second fresh conversion of the paired copy is affected by stale globals or session state. Keep the repeat as a separate observation within the allowed one-project boundary, not a cross-project parity claim.

## Evidence limits

The 16-case native probe at `0x140074f50` is not this acceptance. It executes the original admission function with mocked type and owner-index callbacks; it does not open a project, resolve original class flags, save files, or close a session. Likewise, `m2m.p03.connector_contract` is a parser-level regression. The proposed corpus comparison is the needed integration evidence, but one case only qualifies the paths it actually reaches. It does not establish all operation-family, DOE study, or project-lifecycle parity.

## Sources

- `source/docs/re/moproj2mst/runner/oracle_b/README.md` (case 51 is the 2D SI reference input)
- `source/docs/re/moproj2mst/s1-04/spec.md` (Cycle writer behavior, one-member count-9 sample, validation observations)
- `source/docs/re/moproj2mst/s1-01/spec.md` (corpus and operation behavior; BEND_2D DoeOutput is not a DiagramItem and is not written)
- `analysis/connector-admission-original.json` (16 synthetic-interface probe qualifications)
- `analysis/connector-type-contract.md` (bounded parser contract and static type evidence)

Read-only fixture identification and planning only. No corpus file was copied or modified; no build, original app, GUI control, or REA database was used.
