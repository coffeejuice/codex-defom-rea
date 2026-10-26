# CP217 operation input review

## Decision: PASS to attempt one targeted differential input; runtime remains unqualified

The best single-edit candidate is a copied B98 input with the first operation's `<Id>1</Id>` changed to `<Id>-1</Id>`. Do not use `0`: the original parser explicitly substitutes the operation's one-based document ordinal when the parsed ID equals zero, before it constructs the operation. In the B98 `Operation_List Number="2"`, the first row is ordinal 1 and the second is ordinal 2, so setting either ID to zero still yields a positive ID and reaches the `id >= 1` directory branch.

## Evidence and expected effect

The retained `98_B_DS3F_before/Gear_blank.moproj` has two operations. Its first entry is `Die Stress`, with `<Id>1</Id>` followed by `<Type>SfOprDiestress</Type>`; the second is `Forming (2)`, ID 2. The input is copied evidence under `evidence/gui_mo/oracle_b/98_B_DS3F_before/`; leave it untouched and make the experiment copy elsewhere.

The retained original parser evidence in `source/docs/re/moproj2mst/s1-01/spec.md` describes `ParseOperation`: it reads `<Id>` into an integer, then at `<Type>` replaces exactly zero with the one-based ordinal and calls the operation factory. The same specification records `140050820(OPI,id)` storing the ID and entering the directory setup only when `id >= 1`. The five project directories are created within that guarded call. Therefore a negative ID present before `<Type>` is the only one-edit scalar candidate identified here that statically bypasses the gate while still invoking the operation factory and continuing through the rest of `Operation_List` parsing.

Expected static distinction:

- `Id = 0`: ordinal fallback to 1 or 2; directory branch still runs.
- `Id < 0`: no zero fallback; operation construction proceeds with a negative ID; the `id >= 1` directory branch is skipped.
- Removing `Operation_List`: parser skips all operation factories and operation rows, which avoids this branch but changes the project from a two-operation project to one with no operations. This is a much broader perturbation and may affect later Open processing, diagram binding, and active-operation selection. The CP216 collector is pure and currently disconnected from Open/FileEffects, so it supplies no runtime evidence for either alternative.

## Runtime limits

This establishes static plausibility only. The supplied parser/ID evidence does not establish that the original application completes Open for a negative operation ID, that it avoids another directory-creation path, or that the original accepts the altered project without a warning or failure. B98 also contains diagram rows referencing operation IDs 1 and 2; changing the first ID to -1 may affect diagram binding or subsequent Open processing. The CP216 collector tests validate path collection behavior only and do not prove original filesystem effects or the negative-ID case.

Run the differential experiment only on an isolated copy of B98. Observe both successful Open and the five target directory effects. Treat either unsuccessful Open or any alternate creation route as a failed/indeterminate candidate, not as confirmation of the intended branch. No original application was launched for this review.

## Paired two-ID variant assessment (CP217)

### Decision: PASS as the discriminating input to preflight; HOLD on any claim of valid Open or causal result

The proposed paired variant is the stronger experiment input: in a fresh copy of `98_B_DS3F_before/Gear_blank.moproj`, change only the two direct `Operation_List/Operation/Id` values from `1` to `-1` and `2` to `-2`. The earlier one-ID recommendation is insufficient for the specific `Lubricants` absence comparison: its unchanged positive second operation still independently satisfies the documented `id >= 1` condition. The paired edit is therefore required if the intended contrast is whether either of these parsed operations can account for the five-directory request.

The static parser evidence supports the branch prediction. `parseOperationList` visits the two operations in document order with ordinals 1 and 2. `ParseOperation` uses signed integer conversion for the `<Id>` text, applies fallback only for equality to zero, and invokes the factory at `<Type>`. The constructor/setter evidence then gates directory setup on `id >= 1`. Thus `-1` and `-2` should both remain negative through that point and skip this particular branch. Zero would become 1 and 2 and would not discriminate. Removing `Operation_List` bypasses operation construction entirely, making later parse/Open effects much harder to attribute; retain the paired negative-ID edit instead.

No earlier negative-ID rejection is documented in the reviewed ParseOperation/factory/`140050820` path, but the retained source qualification is not a whole-program proof that later Open stages accept negative IDs. A material validity risk remains: B98's `DiagramItem` entries retain `Operation_Id="1"` and `Operation_Id="2"`. Leaving them unchanged follows the experiment's one-variable design, but may break diagram binding or another later Open step. Do not adjust those references in this experiment, since doing so would introduce additional input changes and weaken the contrast. The operation `<Type>`, child ordering, count attribute, and all non-ID fields also stay unchanged.

The prescribed byte mutation is appropriately narrow: replace only the two direct ID text bytes by inserting one ASCII minus sign before each digit. Preserve encoding, byte order, line endings, and every other byte; require a diff consisting of exactly those two insertions, record offsets and SHA-256, and parse control and variant XML independently. Confirm there remain exactly two operations with ordered IDs `[-1, -2]`, unchanged recognized operation types, and the original `Name`/`Id`/`Type` ordering. This is a candidate-only preflight; this review did not create or launch a clone.

Interpret outcomes only after the control reproduces the `Lubricants` addition with correct encoded `-f` transport. If the variant fails Open, exits early, or lacks evidence that it reached a comparable Open stage, absence of `Lubricants` is inconclusive. If both complete comparably and variant adds the directory, this challenges the claim that a positive ID in these parsed rows is necessary for the observed effect, but does not identify the alternate creator. If control adds it and a successfully Opened variant does not, the result is consistent with the guarded operation-ID path; absent a creator/call witness it still does not prove causation. CP216's collector is a pure, disconnected helper and adds no runtime corroboration.

The paired input is therefore **PASS for byte/XML preflight and for the planned discriminating comparison**, with the negative-ID and diagram-binding risks retained. It is **HOLD for asserting that the variant is a valid successful-Open input**, for interpreting a missing-directory result without comparable Open evidence, and for integrating directory effects into public Open behavior.
