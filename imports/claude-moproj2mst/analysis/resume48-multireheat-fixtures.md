# Resume 48: Multi-reheat fixture inventory

## Finding

No retained corpus or oracle fixture located in the converter subtree contains a parsed Cogging model with two or more reheats. The 27-record snapshot `source/apps/moproj2mst/tests/p06/data/mdt_golden.txt` has `reheats 1` for all 27 Cogging.MDT records and no `reheats >= 2` record. This field is generated from the parsed model's reheat vector size (`tests/p06/gen_mdt_golden.py` uses `len(m['reheats'])`); `tests/p06/test_mdt.cpp` loads each actual corpus MDT with `readMdtFile` and compares the resulting model dump against that golden. Thus the golden's count is a parsed-model/vector count, not a project filename or number of operation rows.

A read-only pattern scan of retained `evidence`, `build`, and source test fixtures for native `NoReheats` values >=2 returned no hits. This includes repeated captured/build copies and is not a claim about unseen user datasets or all possible DEFORM projects. The external corpus at `C:\Users\alext\Projects\DEFORM_corpus` supplies KEY/DB materials but does not itself retain Cogging.MDT or MOPROJ files in the inspected tree; relevant MOPROJ/MDT copies are in the converter's evidence/build snapshots.

## Relevant multi-operation examples

The names `53_Cogging_Multiple_Ops_Lab1` and `54_Cogging_Multiple_Ops_Lab2` describe multiple operation rows, not multiple reheats. In the parsed model fixture they have these individual file counts:

| Project MDT record | Parsed reheat-vector size | Writer reach evidence |
| --- | ---: | --- |
| `53_Cogging_Multiple_Ops_Lab1/Operations/Task00005/Cogging.MDT` | 1 | Included in the 27-file `corpus_sp2_mdts_reproduced` path in `tests/mdt/test_mdt_write.cpp`; this conversion requests emitted MDT side files and compares all 27. |
| `54_Cogging_Multiple_Ops_Lab2/Operations/Task00001/Cogging.MDT` | 1 | Included in the same corpus round-trip path. Also included in `oracle_b_lab2_last_read_globals`, which expects two emitted MDTs and compares each task output. |
| `54_Cogging_Multiple_Ops_Lab2/Operations/Task00003/Cogging.MDT` | 1 | Included in the same corpus round-trip path and the per-task oracle comparison above. |

For the broad corpus writer check, `test_mdt_write.cpp::corpus_sp2_mdts_reproduced` calls `convertMdts`, iterates returned side files, compares 27 results, and asserts no refusal. Its source comment identifies eight cases where the active diagram item is Cogging and Save saves that active operation first; the project property writer still emits operation MDTs in row order. The `oracle_b_lab2_last_read_globals` case explicitly changes Active from 0 to 2 before conversion, so it establishes row-wise MDT output for both Cogging operations but is not evidence that active-Cogging geometry path ran in that test.

The broader validation `analysis/wave3-distortion-cogging-validation.json` has 110 registered cases; the corresponding test receipt labels `53_Cogging_Multiple_Ops_Lab1` A-excluded in that harness. That skip is not proof of `NoReheats >= 2`, nor does it negate the separate corpus writer path. The label “Multiple_Ops” alone must not be used as a proxy for the `NoReheats` field.

## Attribute versus parsed count and synthetic coverage

`source/apps/moproj2mst/src/datafiles/mdt.cpp` reads the `NoReheats` integer into a local `nReheat`, assigns `c.nReheats = nReheat`, resizes `c.reheats` to `max(nReheat, 0)`, initializes each vector element, and calls `reheatLoad` for each declared entry. Consequently, a raw MDT `NoReheats` token is an input attribute; the relevant writer precondition is the post-reader `CogData::nReheats` and owned `reheats` vector. The current port additionally checks that a positive count does not exceed vector size before iterating.

The only existing `nReheats=2` evidence found is synthetic unit coverage, not a corpus/oracle case: `tests/mdt/test_mdt_write.cpp::two_reheats_refused` creates defaults, sets `nReheats=2`, duplicates a model reheat, and asserts the current port returns `M2M_E_UNSUPPORTED`; it then resets to one and asserts the `NoReheats 1`/`Reheat 1` ordering. This establishes guarded candidate behavior only. The root-added focused check in `tests/mdt/test_cogging_save.cpp` is also synthetic and should not be described as original multi-reheat validation.

## Consequence for next validation

The available corpus provides a real parsed/writer-reached one-reheat control, including the two multi-operation labs, but no real two-reheat input to establish an original comparison. Keep the deterministic one-versus-two unit boundary as candidate guard coverage. If native behavior must be observed, prepare a disposable project fixture by changing an MDT's actual `NoReheats` record and adding a valid second serialized reheat block, then verify after parsing that `c.nReheats` and `c.reheats.size()` both equal 2 and that the conversion actually reaches Cogging MDT output. Retain native termination/status and all resulting bytes as exploratory evidence; the statically recovered loop reads beyond its array, so do not treat one observed output as a stable expected file. No source, build, app, or REA state was changed for this inventory.
