# CP178 R300 L6 local SimControl closure implementation

## Outcome and scope

The translation-unit-local raw-return `simCtl(Project&, Operation&)` seam in `source/apps/moproj2mst/src/engine/l6_slots.cpp` is replaced by `withOpSimCtl`, constrained to an exact `void` callback result. Its one `engine::opModel(p, op)` lookup is followed by immediate `std::invoke` with `m ? m->simCtl() : nullptr`. The callback and control pointer are not stored or returned. All 13 migrated lambdas are explicitly `-> void` and noncapturing (`[]`); their bodies only guard null and write the original field values inline. Needed standard headers are explicit.

This implements the staged source cut from `analysis/resume178-r300-next-cut.md`; it does not claim runtime acceptance. Only the two reserved source/test files and this report were written. Ownership of both reserved files is released to root after this receipt.

## Static audit

The 13 acquisitions remain independent and in original order:

| Function | Count | Writes |
| --- | ---: | --- |
| `heatTreatPrologue` | 4 | TRANS 1=0, 3=0, 2=0, 0=1 |
| `distortionPrologue` | 5 | TRANS 1=1, 0=0; grouped +0x16c=2, +0x164=1, +0x160=-1, +0x1d0=1.0; +0x2e0=1; +0x2e4=1 |
| `slotNamesPostLoad` | 2 | TRANS 1=1, 0=0 after object/rename loop |
| `dieStressPrologue` | 2 | TRANS 1=1, 0=0 |

All three prologues retain their preceding `baseLoadPrologue` call. Distortion retains `if (!c.current) return` immediately before its separate final lookup using `*c.current`. Its four defaults remain one callback in their original order. Null only suppresses the current callback's writes; later helper calls remain present. The helper retains `m->simCtl()` rather than reading `m->step` or using `withOpStep`; source `gui_model.cpp:179-182` shows the geometry refusal precedes the nullable step borrow. The direct accesses in `dieStressFromDbStep` and other out-of-cut sites are unchanged.

A bounded Python source audit asserted 13 helper callsites, 13 explicit void noncapturing lambdas, absence of any old `simCtl(p, ...)` calls, HeatTreat count 4, Distortion count 5, and post-load count 2. Diestress count 2 was inspected directly. This is a source-level pointer escape/type/order audit, not an executed compiler check. No C ABI header, cache0/A/refusal implementation, or cache1 publication was edited. `simcontrol300.cpp` and exported C header hashes remain their staged values below.

## Focused test addition

`distortion_prologue_null_step_preserves_simcontrol` is one new direct slot case in the existing copied-corpus fixture. It reclassifies the admitted owner, obtains its actual cached `engine::opModel`, asserts `op->engineModel.get() == model` and `cls->current == op`, and checks the registered prologue exists. Distinct sentinels cover four TRANS words, all four grouped defaults, +0x2e0/+0x2e4 and base prologue TNOW fields. The actual step is moved out only around the direct slot call and restored on normal and exceptional exits. Checks after restoration establish that the saved step fields and owner/model identity remain intact. Existing positive cases and the separate props-slot null-step case were retained rather than duplicated.

The new case has not been compiled or executed. It cannot establish parked-geometry behavior or dynamic lookup counts. No build, CTest, full suite, freeze, GUI/app-control, live mutable REA, original execution, deployment or shared checkpoint state was performed by this worker. Root must validate these exact hashes through the staged acceptance sequence before freezing. The old spec path mentioned by source comments (`docs/re/moproj2mst/mdt/L6/spec.md`) is absent in this imported checkout; this report makes only current-source preservation claims and does not upgrade original-binary evidence or public Open/Save reachability.

## SHA-256 receipt

| File | Before | After |
| --- | --- | --- |
| `source/apps/moproj2mst/src/engine/l6_slots.cpp` | `42b36efbffcfbf997fed53416459670d58db31ce0c235026baacc9bd8ee37e58` | `aea0e5363c5680fdeb645089b6fe33c1969f6c6d0a36494bd2aab903b0e1d9be` |
| `source/apps/moproj2mst/tests/mdt/test_l6_slots.cpp` | `ecc675d7711b40370efc170e8a38b76a7bb46c4a7375918791e615fa5cf0ff3e` | `9e1f3f5d8a031e27dbd3fd2a058940fa1549ffae921aa55c9b37412f481c768f` |
| `source/apps/moproj2mst/include/moproj2mst.h` | unchanged | `34e55cdd58280b056d21e94986c11ac32ccea6f9a6c956d5dad32dbb9864fc40` |
| `source/apps/moproj2mst/src/engine/simcontrol300.cpp` | unchanged | `67b144f6df3e95843c82eae1a77389e5cfab8b65e2534419d3eae4e7f9d34dae` |

Before hashes are the staged CP177 receipt in the next-cut plan; after hashes were computed from current bytes. Rollback restores the two reserved files together to their before hashes. CP163 cache0/A/refusal and no-cache1 boundary remain outside this migration.
