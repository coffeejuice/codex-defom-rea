# CP196 L5 corpus and oracle follow-up

## Scope and qualification

Read-only follow-up to `analysis/resume196-l5-reachability.md`. Inspected the retained corpus manifest/index, L5/MDT tests and retained oracle-B cases. The qualified behavior source remains `source/docs/re/moproj2mst/mdt/L5/spec_L5.md`, derived from historical disassembly/decompiles; this pass did not run the original application or use a mutable REA database.

## Corpus and test findings

- `source/docs/corpus/manifest.tsv` and `source/docs/re/keywords/tools/g2_corpus_index.py` have no identified native Convert2Dto3D test fixture: the manifest is the general KEY/DB corpus, while the indexed public MDT loader tests are `source/apps/moproj2mst/tests/mdt/test_l5_loader.cpp`.
- `test_l5_loader.cpp` explicitly says corpus 46 contains no Convert2Dto3D or CopyMirror operation. It retags another operation for slot coverage. Thus its converted load assertions do not qualify as native admission.
- `source/apps/moproj2mst/tests/s/test_save_current.cpp` records cases 69_B_CNV (native Convert2Dto3D appended to corpus 51) and 85_B_CNVF (native Convert2Dto3D substituted for the first operation). The retained index `evidence/gui_mo/s1/corpus_b/cases.tsv` reports both cases were opened and saved with DEFORM v11.0.2 GUI, executable PE timestamp 2014-12-04 and SHA-256 prefix `fd8ae1f8dd50`; 69 appends Convert2Dto3D (4 operations total), 85 retags the first operation (3 total). These are native-class admissions, but not sufficient step-owned loop witnesses by themselves.

## Narrow oracle inspection

- `evidence/gui_mo/oracle_b/69_B_CNV/MO2_Operation_Cycles2.MST` includes `KFREAD Operations/Task00004/M23_INPUT.KEY` and a following TEMP_STEP reference. The retained case directory has no `M23_INPUT.KEY`, `SfOprConvert2Dto3D_TEMP_STEP.KEY`, or Task00004 SimCtrl.KEY file. Therefore literal M23 bytes and the ordered file effects are not retained for that task.
- `evidence/gui_mo/oracle_b/85_B_CNVF/MO2_Operation_Cycles2.MST` includes `KFREAD Operations/Task00001/M23_INPUT.KEY` and the TEMP_STEP reference. The Task00001 bundle likewise does not retain either output file. Its corresponding SimCtrl.KEY exists, but without the output bytes and an effect trace it does not meet the witness requirement.
- The adjacent saved object/property files were not treated as evidence of mesh contents or positive `step->u1378` reachability. No bulk mesh payload was inspected.
- The earlier retained database comparison `analysis/wave2-db129-capture.json` has M23 hashes and a TEMP_STEP hash for a synthetic modified case; `analysis/wave2-db129-diagnosis.json` states that case was created by changing operation 2 to Convert2Dto3D while retaining an a3D database. It is not a native public fixture or same-case original GUI Save trace with retained literal bytes and ordered effects.

## Outcome and next action

The search found native admitted public cases, but no qualifying retained witness: available MSTs only reference M23_INPUT.KEY and do not retain its literal bytes or a verified ordered file-effect trace. No existing test independently pins positive nonempty step ownership, selected object branch, and expected output/effect order. Keep CP196 guarded; do not refactor the step-owned interval based on these cases. The next useful action is to locate a retained original capture bundle that includes the actual M23 bytes plus ordered effects for one of these exact input cases. If none exists, a new original capture may be needed, but it is outside this read-only follow-up.

No source, tests, build outputs, GUI state, mutable REA database, parent/DMO/Claude checkout, or original files were modified; only this report was added.
