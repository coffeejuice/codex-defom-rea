# CP195 R300 L6 plan review

## Verdict: PASS

The selected `dieStressFromDbStep` is a genuine multi-statement owner and a reasonable candidate beyond the exhausted L2 lane. The revised plan specifies separate immediate scopes for the scratch and actual model steps, retains the observed effect order and null behavior, and removes unsupported identity-test claims. The proposed source/test pair is disjoint from the CP195 connector pair. I independently checked the frozen CP194 archive at `build/wave3-checkpoint194-verified/source.zip` (SHA-256 `E837F823A179E5039EA70A05BC2820C0B16C081EA8D28E9DBED9CA39435DE2B2`). The archived `l6_slots.cpp`, `test_l6_slots.cpp`, `gui_model_access.hpp`, and L6 spec match their current counterparts byte-for-byte.

## Source boundary and behavior that must be preserved

`l6_slots.cpp::dieStressFromDbStep` directly accesses both `m->step` and `s->step`. `m->reset()` precedes the first destination step access. It then derives dimension from `s`, sets destination dimension, clears `opi->objIds`, refuses nonempty scratch objects, validates `DiestressProps` and its data, assigns `stepNumber`, copies material count/materials/phase pairs, and obtains the destination `SimControl`. A null destination `SimControl` returns after those mutations. Otherwise GEOTYP is copied only when `is2D(*m)`, PDIE is copied unconditionally, `s.reset()` occurs, then NSTART and the conditional NSTEP/STEPINC/contact-pair behavior run, followed by SOLMTD and DTMAX stores. The callback must preserve this order and the early return exactly.

The revised plan corrects the prior underspecification: `GuiModelAccess::withStep` takes one model, so it prescribes separate immediate scratch and actual callbacks, with a nested pair only for the material/phase-pair copy. No raw step pointer is retained or returned. It keeps `m->reset()` before destination borrowing and preserves direct unchecked dereference behavior for null actual/scratch steps. `withStep`'s geometry check remains at each access point after operation/model admission and scratch acquisition and is explicitly described as a migration precondition, not original geometry behavior.

The direct uses of `s->simCtl()` are unchecked too. The revised plan preserves the conditional GEOTYP read and unconditional PDIE read without null fallbacks, and retains destination-model geometry decisions (`is2D(*m)`, `is3D(*m)`). Its stated duplicate geometry check from the scoped borrow plus `simCtl()` preserves the existing getter call and does not add a fallback.

## Fixture and assertion capacity

`test_l6_slots.cpp::diestress_without_db` reaches this owner through the public loader slot after reclassing operation 4 from `42_vdie/vdie.moproj`. Its documented fixture has no native Diestress operation; it exercises class-substituted, no-DB behavior with an empty scratch model. The existing literal assertions are a useful output regression anchor for empty IDs, step number, NSTART, SOLMTD/DTMAX and post-load TRANS flags. The fixture cannot establish native Diestress admission, nonempty object-copy behavior, or populated material/phase-pair copies.

It also cannot provide a meaningful same-owner positive/negative identity pair: the public route supplies only one model and the empty no-DB scratch state, and no independent expected output distinguishes a valid owner binding from a foreign-owner binding. A pointer compared with itself would be tautological. Keep the current literal output assertions; add only independently specified values actually exposed by this fixture. Do not characterize this as positive/negative ownership coverage. A focused internal seam test would require a separate source-ready fixture and explicit independent expectations, which this plan says it does not have.

## Evidence qualification and ownership

The retained `source/docs/re/moproj2mst/mdt/L6/spec.md` attributes the path and effect sequence to decompiles/disassembly in `deform-rea-work/gui_mo/s1/mdt/L6/` and the retained Ghidra copy. The current test header likewise says its expected values derive from that spec. Those support a source-qualified reconstructed-route claim, not fresh original-app behavior or native-class coverage. The review did not launch the original or inspect a mutable REA database. The frozen CP194 archive comparison confirmed byte-identical source, test, access contract, and spec files; this confirms checkpoint source state only and does not strengthen the original-behavior qualification.

The exclusive files `source/apps/moproj2mst/src/engine/l6_slots.cpp` and `source/apps/moproj2mst/tests/mdt/test_l6_slots.cpp` are separate from the connector pair identified by the broader-next plan. This supports disjoint file ownership, subject to the coordinator retaining those assignments.

## Owner files for assignment

- `source/apps/moproj2mst/src/engine/l6_slots.cpp`
- `source/apps/moproj2mst/tests/mdt/test_l6_slots.cpp`

No CP195 connector file is included in this assignment. The existing no-DB fixture may be used only as route/output regression coverage; it does not prove borrow identity, foreign-owner rejection, native-class behavior, or populated copying. The revised plan's separation of these claims is appropriate.

No source, test, build, GUI/app-control, mutable REA database, parent/DMO/Claude/original file, or deployment was modified or used. Only this review note was written.
