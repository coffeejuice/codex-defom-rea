# Checkpoint 70: Cycle member current-context decision

## Decision

Keep the candidate's temporary `current = member` assignment guarded and unchanged. The native Cycle writer passes each member explicitly to that member class's `vt+0x280` but does not rebind the class's `curItem` (s1-04 O-3). Candidate `CycleClass::writeMst` instead snapshots current, binds it to the member, dispatches, and restores on normal return. Those semantics differ whenever the member class's current value at entry is not already the member.

The source evidence identifies reachable writer branches that consume current (`SfOprDistortion2D/3D` and `SfOprConvert2Dto3D`), but the indexed original Cycle projects do not contain either as a Cycle member. A focused fixture-backed regression for that difference is therefore not supported by the retained corpus. A synthetic XML arrangement could test candidate mechanics, but without an original run it would only encode a hypothesized current state and would not establish parity. Do not remove the adapter binding or call it verified.

## Corpus search and concrete retained candidate

I parsed all 305 retained `evidence/gui_mo/oracle_b/**/*.moproj` files and inspected each Cycle `GroupInfo` rectangle against diagram-item `CellPos`. The 46 Cycle-project entries all selected `SfOprForming2D` as their member class. None selected `SfOprDistortion*` or `SfOprConvert2Dto3D`.

The tempting case `evidence/gui_mo/oracle_b/102_B_DIS2F/MO2_Operation_Cycles2.moproj` is not a Distortion-member case. It has Distortion operation 1 at row 0, column 0; Forming2D operation 2 at row 0, column 1; and Cycle operation 3 with `GroupInfo BeginRow=0 EndRow=0 BeginCol=1 EndCol=1`. The selected member is therefore operation 2 (Forming2D). The file is SHA-256 `EB0F3F2214AD68516139C736723B5DACEACAB2971002F374FF4870CC8BC323DC`.

The original native Cycle spec says membership is determined by the group rectangle and the nested writer receives the member as an explicit argument. Thus other registered operation classes may be structurally admissible as members; the 305-file search shows only that the retained evidence corpus does not exercise them.

## Why a current-sensitive member would discriminate

The static native spec records that Distortion SIMNAM construction reads the class's `curItem` (native `0x141363fc0`), and its emitted SIMNAM text incorporates that current item's name. Candidate `src/ops/distortion.cpp::writeDistortionPasses` reads `self.current` for that path. The native s1-04 spec also records that Convert2Dto3D task paths use `self->curItem`, while candidate `src/ops/convert23.cpp` builds those paths from `current`. The candidate Cycle assignment makes these paths use the explicit grouped member even if the entry binding was another operation. Native Cycle does not make that assignment; it preserves the pre-existing binding.

This exposes a real parity hazard, but not the intended value of the stale binding. For fresh active-Cycle model creation, Cycle vt+0xe0 requests the first member's model and native model construction can bind that first member as current. For a non-first member with an already-created model, the current value may have been set by a prior selection or model-creation call. The retained XML plus static code does not identify that GUI history at the nested call boundary. The safe expected assertion for an eventual original/candidate comparison is the actual observed binding and output, not an assumed rule that current is null, the Cycle, the first member, or the member being written.

## Acceptance needed before changing the candidate

A source-only unit test can verify that candidate code sets/restores current, but it cannot adjudicate the original stale-current behavior. A parity test needs a disposable complete project with at least two Cycle members where a current-sensitive writer is not the first member, plus a controlled state that makes the class current differ from that member. At the nested dispatch boundary, capture:

- Cycle ID, ordered member IDs and classes;
- the member writer's explicit item argument;
- that writer class's current item immediately before dispatch;
- the item name/SIMNAM or task path actually emitted;
- the post-dispatch current value and output bytes;
- exception and restoration behavior, since candidate restoration is normal-return-only.

Then collect the same boundary and output from the original on the same disposable input and equivalent Open/selection sequence. The accepted output must match original behavior under the observed context. No such retained complete project or boundary capture is identified here. Until then, preserve the candidate assignment as guarded adapter behavior and do not claim that its semantics match native O-3.

## Evidence

- `source/docs/re/moproj2mst/s1-04/spec.md` §2: native Cycle group-member enumeration, explicit `it` passed to member `vt+0x280`, and O-3 no-current-rebind; §4 / §6: current-sensitive Convert2Dto3D and Distortion writer behavior. SHA-256 `2E1F9F7B75F49B4E3BCEC811755C4C0666E217CCBA8864CF2128E58268B93790`.
- `source/apps/moproj2mst/src/ops/cycle.cpp`: candidate member binding and normal-return restoration. SHA-256 `1BB6772156E94CBE33CD14A99045711C874E087DB846D466B89EA79266371DF1`.
- `source/apps/moproj2mst/src/ops/distortion.cpp`: current-sensitive Distortion SIMNAM path. SHA-256 `C3D7C101AD2220442CFA357BCA2A6A20791265907D9F4832EBD6CAA36756E015`.
- `source/apps/moproj2mst/src/ops/convert23.cpp`: current-derived task path. SHA-256 `D19A7A693615E249C5B185702363E58EAF26AF059BBE733633826BFEA42DBF23`.
- `source/apps/moproj2mst/src/ops/base.cpp`: explicit operation argument and current fallback only when a caller supplies null. SHA-256 `1219B04B216F18FB6CE14B8FFC877B3638678EDB30598BF94B27B69F794E2756`.
- `evidence/gui_mo/oracle_b/102_B_DIS2F/MO2_Operation_Cycles2.moproj`: non-example countercheck showing Distortion adjacent to, but outside, the Cycle rectangle; SHA-256 above.

This was a read-only source/corpus analysis. No source, build, GUI, project fixture, or mutable REA database was changed or run.
