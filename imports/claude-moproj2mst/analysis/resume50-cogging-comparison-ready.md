# Resume 50: Action-equivalent Cogging comparison fixture

## Best retained one-reheat boundary case

`evidence/gui_mo/s1/corpus/42_vdie/vdie.moproj` is the cleanest retained project-level one-reheat control. Its `<Sub_Project Active="3">` sets a zero-based active diagram item index. The four `DiagramItem` entries are ordered Operation_Id 1, 2, 3, 4, so index 3 selects operation 4; the operation list identifies operation 4 as `SfOprCogging`. This is the public active Cogging Save state required for `saveActiveCogging` to run.

Its input `Operations/Task00004/Cogging.MDT` has `SIMLST 56` and `NoReheats 1`. The corresponding P06 parsed-model golden record is `42_vdie/Operations/Task00004/Cogging.MDT` with `reheats 1`; the P06 parser test reads actual corpus MDTs and compares the parsed model vector count with that golden. This is an actual parsed count, distinct from the project having four diagram items/operations.

Retained artifact hashes:

- Project input: `evidence/gui_mo/s1/corpus/42_vdie/vdie.moproj` — `014eba76c741dccc678762b2bf38d531f066931b15ccb4f64fd3af7110dc3ca4`.
- Input Cogging MDT: `evidence/gui_mo/s1/corpus/42_vdie/Operations/Task00004/Cogging.MDT` — `b2b8e57be83ff27def086b0b95ce0ee53b68c2262d6b1c583ffd47a9507b786e`.
- Retained oracle-B saved MDT: `evidence/gui_mo/oracle_b/RS_42_vdie/Operations/Task00004/Cogging.MDT` — `714b238dd66674e0927d8b8dc8125aba3b66f21dee6871f5e3cb1592795cfd77`; it also has `SIMLST 56`, `NoReheats 1`.

This saved result is retained oracle-B evidence, not a new fresh-session observation in this task. Treat any historical session qualification carried by the oracle as applicable; no GUI/original process was run.

## Candidate route to action-equivalent comparison

The current `tests/mdt/test_cogging_save.cpp::vdie42_cogging_save_geometry` already performs the operation-level sequence: open the project, read Task00004 MDT, activate the selected operation, open the active Cogging engine model, run `coggingGeometryStep(..., true, ...)`, then serialize and compare the full Cogging.MDT to oracle-B. It verifies derived billet geometry plus unchanged die/manipulator fields and whole-file byte equality. It is stronger than a writer-only synthetic model, but it calls the step directly rather than the public project Save entry point.

For the public comparison, the appropriate harness is `tests/mdt/test_mdt_write.cpp::corpus_sp2_mdts_reproduced`: it invokes `convertMdts` with `M2M_FLAG_WRITE_MDT`, which goes through project conversion/Save orchestration, and checks all returned Cogging MDT side files. The existing test expects 27 generated MDTs and no refusal. A focused action-equivalent case can reuse that helper on an isolated copy of `42_vdie`, then select the `Task00004` result and compare it to the retained `RS_42_vdie` oracle output. The necessary state is the original active index 3, intact input MDT, and one project per `convertToBytes` call; changing Active to another operation would skip active Cogging Save and answer a different question.

## Boundary and limits

The retained corpus/oracle set has a valid real one-reheat public Save case but no real `NoReheats >= 2` case. The two-reheat unit tests in `test_mdt_write.cpp` and the root-added `test_cogging_save.cpp` exercise a hand-built `CogModel` directly at `writeMdtBytes`; they do not execute project parsing, active-operation selection, or public Save. To make a two-reheat action comparison, a later test would need a disposable project copy whose MDT has an actual second serialized reheat block, then verify the parsed `nReheats` and vector size before invoking project Save. No original result can be inferred from merely changing an in-memory count.

The 53/54 “Multiple_Ops” fixtures are not multi-reheat tests. The 54 raw sample starts at active index 0 with Cogging operation id 1 and has two separate Cogging operation MDTs; each actual parsed count is 1. The existing Lab2 oracle test changes Active to index 2 (Forming) and therefore is not an active-Cogging Save comparison. `42_vdie` avoids that ambiguity: active index 3 maps directly to its Cogging row.

No source, build, GUI, or mutable REA state was changed for this fixture qualification.
