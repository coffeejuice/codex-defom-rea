# CP220 operation-ID and diagram-reference comparison design

## Decision

Recommend one **disposable, paired-reference negative-ID witness** only if the immediate question is whether CP219's negative-ID result was confounded by stale `DiagramItem` references. In a fresh variant, change both operation IDs and their matching `DiagramItem Operation_Id` values from `1,2` to `-1,-2`. This preserves the ID relationship represented in the project while keeping both parsed operation IDs below the documented `id >= 1` directory gate.

This is a more discriminating next input than repeating CP219's operation-only mutation: the CP219 negative variant retained diagram references `1,2`, so it did not preserve referential consistency. It is still a **multi-field perturbation**, not a pure test of the operation-ID branch. The XML parser reads diagram IDs independently, and a negative diagram ID can itself alter diagram construction or downstream Open. Therefore only run this if the purpose is to test the stale-reference confound; do not present it as a clean causal intervention on the mkdir branch. If the required claim is branch causality, the better next witness is a same-occurrence original trace of operation ID at the `id >= 1` decision / `0x14008c630` caller, with Open-stage qualification, rather than another XML mutation.

## Evidence that motivates the comparison

CP219's no-hook B98 control added the empty `Lubricants` directory. Its paired negative-ID variant added no `Lubricants`, but added root-level `ObjProperties00001.KEY`, `ObjProperties00002.KEY`, and `ObjProperties00003.KEY`. The output deltas were disjoint. Thus the directory contrast is suggestive but not a matched downstream effect profile: the variant evidently took a different property-file path, or otherwise changed later processing. The receipts establish those file additions and clean held-handle termination; they do not establish equal Open completion or the creator of any path.

The source project is `imports/claude-moproj2mst/evidence/gui_mo/oracle_b/98_B_DS3F_before/Gear_blank.moproj`, 10,932 bytes, SHA-256 `e769faf514b335d56ffbb22f2c76490d029f2714ada9b26a75496fbd62ade7fd`. Its `Operation_List Number="2"` contains `Die Stress` (`Id=1`, `Type=SfOprDiestress`) and `Forming (2)` (`Id=2`, `Type=SfOprForming`). The corresponding final diagram rows are `<DiagramItem Expanded="yes" Operation_Id="1">` and `...Operation_Id="2">`.

The operation parser contract in `source/docs/re/moproj2mst/s1-01/spec.md` says `ParseOperation` reads a signed integer ID, substitutes the one-based row ordinal only when the parsed ID equals zero, and dispatches at `Type`. `140050820(OPI,id)` performs directory setup only when `id >= 1`, through `140051190` and `14008c630`; the latter requests `Operations`, `Objects`, `Materials`, `Equipments`, then `Lubricants`. The static contract predicts `-1,-2` skip that specific branch while still passing through operation construction.

The same specification's `140091f00 ParseDiagramItem` reads `Operation_Id` with `toInt()` and passes it to the diagram-item factory (`model->vt[0x58](cls, opId)`). It does not establish whether negative diagram IDs are accepted, matched to negative operations, or harmless later. This is exactly the remaining confound: keeping diagram IDs positive tests the operation branch with broken references; making both sides negative preserves equality but changes diagram semantics too. CP217's candidate-only preflight showed operation-only negative IDs are accepted by the candidate parser, but is not evidence about the original or this paired-reference variant.

## Proposed disposable input, byte edits, and preflight

Use a new, absent short destination pair under `C:\PROBLEM`, for example `C:\PROBLEM\p220b` and `C:\PROBLEM\p220n`. Recheck absence at preparation, and copy the source fixture independently into both destinations. Do not reuse CP217/CP219 clones, edit the source, or derive the control from an earlier run. Keep all full paths below 100 characters. The control project must be byte-identical to the pinned source project and the two full-entry source/control manifests must agree.

Construct exactly one variant file from the control's raw bytes. Insert ASCII `-` before each of these four decimal text tokens, scoped by XML ancestry and attribute context:

1. direct `Operation_List/Operation[1]/Id`: `1` -> `-1`;
2. direct `Operation_List/Operation[2]/Id`: `2` -> `-2`;
3. the `DiagramItem` whose `Operation_Id` is `1`: `1` -> `-1`;
4. the `DiagramItem` whose `Operation_Id` is `2`: `2` -> `-2`.

Require exactly four inserted bytes, resulting size 10,936 bytes, and a byte diff with no deletion or replacement. Preserve XML encoding, BOM state, line endings, whitespace, element order, all `<Name>` and `<Type>` values, `Operation_List Number="2"`, all object IDs, inline data, and every other file. Do not blanket-replace `>1<`, `>2<`, or `Operation_Id`; object IDs and unrelated integer values must remain untouched. Record original byte offsets, variant SHA-256, and an independently generated expected-byte vector. Parse control and variant independently and assert two ordered operation IDs `[-1,-2]`, two matching diagram references `[-1,-2]`, exact operation types and child order, and zero other XML changes. If any preflight assertion fails, do not launch.

Retain CP219's correct `-f` transport discipline: encode each actual absolute project path as exactly four lowercase hex digits per UTF-16 code unit, decode it independently, and verify exact round-trip. Bind executable identity to the CP219 receipt's original DEFORM v11.0.2 image and SHA-256 immediately before any later authorized run. Use the owned suspended-process helper only, with each child created suspended and validated before one resume; no hooks, debugger, GUI input, Save, or app-control session. This document itself authorizes no launch.

## Interpretation and comparability limits

First compare each post-run manifest against its own before manifest. Then compare the effect deltas. The control must again add `Lubricants` under valid encoded-path transport. If it does not, the pair cannot speak to the CP219 contrast. Preserve all newly added files, including the root-level `ObjProperties*.KEY` files; do not treat their presence as a harmless noise filter.

- If the paired-reference variant adds `Lubricants`, the CP219 absence may have depended on the stale positive diagram references or some later path they affect. This weakens the interpretation that the operation-ID gate alone explains the absence. It still does not identify the directory creator.
- If it again lacks `Lubricants` and adds the same `ObjProperties*.KEY` files, stale references alone are less likely to explain the contrast, but Open-stage equivalence remains unproved; both the operation IDs and diagram IDs changed.
- If the variant fails, exits early, omits the property files, or shows a materially different effect set, classify directory absence as inconclusive. Do not infer successful comparable Open from process survival or clean termination.
- If either case has incomplete ownership, helper, or process-absence receipts, preserve all evidence and classify the run incomplete. Never delete or reuse the trial directories as cleanup.

The CP219 run should be treated as a baseline observation, not a directly comparable same-session control for a later CP220 trial: fresh processes and clones reduce contamination, but do not control environmental or nondeterministic differences. The strongest useful claim from this experiment is about whether repairing the operation-to-diagram ID correspondence changes the observed effect pattern. It cannot prove the exact creator, complete Open equivalence, Save behavior, or branch causality. For those claims, prioritize the direct original call/branch witness noted above.

## Ownership and cleanup

The trial owner is the single bounded runner for this experiment. Before launch it owns the two new clone roots and must persist source/control/variant manifests, the four-byte patch proof, executable/helper hashes, exact argv/cwd, decoded path, and per-case preflight receipt. Keep raw helper JSONL/stdout/stderr and error artifacts. Preserve creation FILETIME as decimal text, launcher PID, child PID, primary TID, and image hash without converting the FILETIME through a floating-point number.

After the bounded observation, request termination at most once through the helper's held process handle; close the command stream once, drain output, and verify helper exit, signaled process handle, closed handles, and an independent zero-row process-absence query. Record intentional termination distinctly from natural exit and never call it application Close. Preserve the clones and receipts regardless of success, failure, or ambiguity; cleanup means verified process/handle cleanup only, not deleting trial inputs or outputs. Since this proposal acquires no GUI-control session, there is no GUI-control release claim.
