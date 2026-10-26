# CP211 independent RTTI review: PASS with scope limits

## Verdict

**PASS** for the fixed-image RTTI identity and for adding a distinct candidate-side typed-child projection before `initProject`. The evidence supports mapping the existing immutable `DbHandle::Kind::Mo` to the candidate's partial `SfMoModel` projection, and supports placing an identity-preserving gate before Project initialization/publication. It does **not** prove that the original failing invocation reached this factory or cast, establish why the captured native cast returned null, or reconstruct native object construction and side effects.

## Independent bounded PE check

I checked the copied `imports/claude-moproj2mst/evidence/original-binaries/DEF_GUI_MO_64.exe` directly, without executing it. Its SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, matching the report. All eight reported raw-byte range SHA-256 receipts (descriptors, both COL/table regions, both RTTI regions, loader call sequence, constructor, and manager factory) recomputed identically.

Bounded byte decoding confirms:

- At file offsets `0x3567448` and `0x3567470`, descriptor names are `.?AVSfModelBase@@` and `.?AVSfMoModel@@` respectively.
- The Mo vftable prefix at file offset `0x2111070` points back to COL VA `0x1430d5508`. The corresponding RTTI range hash matches the report's COL/hierarchy evidence, including the `SfMoModel -> SfModelBase -> QObject` chain.
- The manager vftable starts at VA `0x142115f68`; its `+0x68` function slot is at VA `0x142115fd0` (raw qword `9054044001000000`) and resolves to `0x140045490`. This independently confirms the concrete factory slot. (The COL pointer occupies the preceding qword at `0x142115f60`.)
- The loader/factory and constructor raw ranges match the report's hashes. This review did not independently disassemble or re-interpret every instruction in those ranges; constructor/factory behavior beyond the checked address linkage remains grounded in the bounded instruction analysis recorded in the CP211 report.

The hash/descriptor/COL checks establish the report is examining the specified copied image and that the class identities and hierarchy metadata are not inferred from source comments. They are static evidence for this binary profile only.

## Candidate-side gate justification

The current candidate source labels `DbHandle` as a partial `SfModelBase/SfMoModel` projection and defines `Kind { Base, Mo }`. In `ConversionSession::prepareProjectModel`, the child is constructed as `Kind::Mo`, receives slots/path, is appended and signal-registered, and is then assigned to `p.dataDb`. `convert.cpp` calls `initProject` after `prepareProjectModel`, with `commitProjectModelConstruction` afterward. The latter already checks `Kind::Mo`, but that is a later ownership/construction invariant and does not represent the earlier typed-child decision.

Therefore the proposed separate identity-preserving projection helper and call immediately before publication, after signal registration and before `initProject`, are a defensible candidate contract projection. The helper must use only the immutable kind: `nullptr -> nullptr`, Base -> failure, Mo -> same pointer. It should not infer the cast from `moProject`, path/read state, IDs, or Project slots. The existing factory always constructs Mo, so success is the expected current path; helper Base/null outcomes can be checked as internal contract cases without asserting native failure reachability. Keep the post-construction ownership/backlink check separate.

The source gate's placement is a candidate seam chosen to preserve current path/append/signal work before Project initialization; it is not byte-for-byte emulation of the native cast's exact timing. No public `nativeFactorySucceeded` claim or native-failure predicate follows from this gate.

## Limits

- The static factory path says what the specified manager table normally returns when allocation and construction complete. It does not show that every runtime manager used that table, that intervening calls preserved its vtable, or that the CP207/209 occurrence completed the same path.
- RTTI proves the base conversion for an intact fully constructed object carrying this vftable and hierarchy. It does not prove the original child was in that state at the failing cast.
- No native runtime witness, GUI action, source edit, test, build, or mutable REA operation was performed.

**Disposition:** PASS for partial class mapping and pre-`initProject` candidate gate; HOLD for any claim of original runtime factory/cast success or causal explanation of the original null result.
