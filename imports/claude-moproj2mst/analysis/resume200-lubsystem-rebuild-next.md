# CP200 R300 system-lubricant rebuild: fixture and next-cut audit

A real existing fixture reaches a nonempty system-lubricant rebuild, including the positive lubricant-template import branch. It is already compared indirectly by the CP198 full Save sweep. The missing coverage is a direct, owner-bound state regression; this is not a newly missing implementation. No new production change is justified solely by the queue phrase “positive lubSystem rebuild remains unexercised.” That phrase is accurate for the CP186/188 R46 helper case, not for the whole suite.

## Established evidence and its limits

Read source qualifications in `source/docs/re/moproj2mst/s/spec.md` sections 1, 2 and 4 and `mdt/L3/spec_save.md` sections 1–4, plus the retained native decompile `evidence/gui_mo/s1/mdt/L3/s1.c` (`FUN_14005a9b0`). These are retained original DEF_GUI_MO_64 v11.0.2 evidence, not Claude behavior used as an oracle. The retained editor300 meshing contract (`analysis/wave2-save-diffs-300-meshing-contract.md`, CP129/130 qualification as reconciled by `resume165-meshing-next.md`) proves only the four-double temporary-row projection and its selected-row ordering. It supplies no additional lubricant contact authority, and its missing full-row lifecycle witness must not be imported as a requirement or permission for this separate helper.

The current rebuild is already scoped via `withOpStep`, including its null/system-disabled checks, temporary local step, clear/import/default-pair/appends, since CP175. CP186 scoped `saveInterObject`; CP188 scoped contact flags. A second lexical wrapper is not useful recovery progress. Current files byte-match the CP198 source archive:

- `source/apps/moproj2mst/src/engine/save_current.cpp`: `62d4f2a827a5fc281692e0b5d8199a549a27594585018fed77ac3d399a46c455`.
- `source/apps/moproj2mst/tests/s/test_save_current.cpp`: `780eeec08f58c8a5afc92e512ae01944e5cb9c8e7c19da0b4b8fe071871349cb`.

CP198 retained test logs independently establish that `m2m.s.distortion_contacts` ran `distortion_contacts_reach_system_rebuild_and_mirrored_records` successfully for both dimensions. It sets `lubSystem=true`, calls `syncOperationFiles`, and checks four exact mirrored records and zero defaults. It is synthetic and does not exercise positive lubricant ID/import or real loaded engine objects. CP186/188 `save_interobject_step_scope` explicitly disables rebuild for its positive calls and enables it only while the step is detached.

The old L3 note cites `82_R52_BEND_2D` as rebuild coverage. Its active input is a one-object System/-1 AirConvection operation. Its Task00001 InterObject.KEY is banner-only. Nonempty contacts in Task00002 are another operation's retained file. R52 supports the empty-result branch, not nonempty contact regeneration.

## Concrete real fixture

Use `evidence/gui_mo/oracle_b/RS_39_brick_mesh_01_before/brick_mesh_01.moproj`, with expected `RS_39_brick_mesh_01/Operations/Task00003/InterObject.KEY`. The input has Sub_Project Active=2 and operation Id=3, SfOprForming, System lubricant Id=1, and three slots. Its actual lubricant file `Lubricants/Lubricant00001.KEY` contains a first pair template with FRCFAC=1, IHTCOF=5, and zero separation/restitution fields. Expected Task00003 output has two CNTACT blocks for (1,2) and (1,3), with those same scalar values; 631 bytes, SHA-256 `dd0147d3cf5137a2dfd61b9f9b76589ef93f53165304a576c3dfa2d3302a8764`. Before-project SHA-256 is `f58788b4744f9280ca8d3ca960e564d291faaf87a117258d3109aa241b9ea9d0`.

Producer qualification is `evidence/gui_mo/oracle_b/_tools/rs.py`: copies before/after folders, starts a fresh GUI for each case, Open/Save/Close, default dialog answers except No for the later Close-save question. `rs_log.tsv` lines 70–71 and `rs_run.out` lines 102–103 record this case saved, exit 0, one Close question answered No. These are historical producer records, not a newly acquired original capture or a complete internal state trace. CP198 log `build/test-runs/1792983316458934000/m2m.s.save_folder.log` line 36 reports `SAME A:RS_39_brick_mesh_01: 0 differ, 0 missing, 0 extra`. The sweep actually calls the public converter with WRITE_SAVE on a private copy of this before-folder. Thus it supplies independent original output bytes and current reconstructed reachability, not proof of every intermediate state.

## Exact native contract to retain

`saveInterObject` admits the model/step, conditionally calls rebuild when System is enabled, then synchronizes flags, derives the path, and writes the existing InterObject serializer with overwrite=1; it returns writer status==0. Rebuild first acquires a present step, then checks System; allocates DB_STEP_DATA(3), clears the destination list, and for positive lubricant ID imports the manager's KEY into the temporary step and borrows its first pair only on successful import. It obtains class vt+0x198 defaults in original order, updates each found master object's lubricant/BCC state, then appends a value-based pair or a template copy plus its swapped kind-1 mirror. The temporary survives all template copies and is destroyed afterwards. Native decompile has unreachable-block/type-recovery warnings; precise raw offset claims rely on the accompanying qualified twin/append specifications, not decompiler variable names.

### In-memory expectation, separately derived

This expectation is conditional on explicit fixture admission checks, not inferred from the two output records. Require that the loaded 3D model has exactly three defined objects, that positions 1/2/3 have engine IDs 1/2/3 respectively, and that exactly position 1 is a master candidate under the original object-kind / kind-5 slot `f08` rule. Require the slot-to-engine mapping and actual candidate field values; do not merely check a computed candidate total. If those literal preconditions do not hold, this four-record test is not admitted: retain the independently justified file/owner assertions below and report the unresolved loaded mapping rather than changing the expected list to match production.

With those preconditions, original base vt+0x198 traverses positions 1,2,3. The 3D skip eliminates position 1 because it equals candidate ID 1. It does **not** eliminate the pairs whose *slave* is 1. The surviving default values, in order, are therefore (master=2, slave=1) then (master=3, slave=1). Native rebuild traverses that two-value list in order. Native template append `0x14035e540` sets the first record to kind 2 and appends its swapped kind-1 mirror via `0x140360d30` before visiting the next default value. The distinct literal in-memory expectation is:

| List position | Master | Slave | Kind | 3D flag |
| --- | --- | --- | --- | --- |
| 1 | 2 | 1 | 2 | 1 |
| 2 | 1 | 2 | 1 | 1 |
| 3 | 3 | 1 | 2 | 1 |
| 4 | 1 | 3 | 1 | 1 |

Assert list count 4 and those fields literally. Friction=1 and IHTC=5 come from the independent original lubricant file's first template, not from another call to production default generation or rebuilding. This is a source-derived reconstructed-state expectation once the admission checks pass; historical output does not directly observe the original's four internal records. INICTC expectations should be fixed from the native kind rule and the fixture's actual enable byte, not calculated by duplicating the loop under test.

### Serialized expectation, separately derived

Read `source/docs/re/keywords/CNTACT.md` “KEY writer”, which qualifies original `FUN_1403c9ad0` / `FUN_1403c8560`; its status entry in `source/docs/re/status/kw-g3.tsv` marks the all-pair writer verified. Unlike list traversal, serialization skips a pair when slave < master. Thus in-memory positions 1 and 3 above produce no CNTACT block. Positions 2 and 4 survive, retaining list order. A 3D kind-1 record with flag 1 writes direction code 1. The exact file has **two** CNTACT records, (master=1, slave=2, code=1), then (master=1, slave=3, code=1), each followed by the original template's FRCFAC=1, IHTCOF=5 and other qualified fields. There is no claim that two serialized blocks mean two in-memory records.

The file assertion is literal byte equality against the retained 631-byte original Task00003 file, including its banner, whitespace, line endings and record order. Do not generate expected bytes using `write_interobject_key_file` or another reconstructed serializer. This assertion remains independently justified even if the loaded candidate-field mapping has not yet been proven well enough to admit the in-memory table.

## Ready bounded successor and ownership

The audited source/test pair is disjoint from CP199 connector/view_manager work: `save_current.cpp` and `tests/s/test_save_current.cpp`. Exclusive edit ownership for this cut is **only `source/apps/moproj2mst/tests/s/test_save_current.cpp`**. Production `save_current.cpp` is read-only; any concrete independently exposed defect needs a separately assigned source cut. No new public API/header, cache1 admission, GUI editor path, or broad R300 barrier belongs to this cut.

Add a direct `system_lubricant_rebuild_real_template` case in `m2m.s.save_current`. Copy the entire RS39 before-folder beneath M2M_TEST_SCRATCH; open/activate/load through the existing registry and loader APIs, selecting the real operation 3. Require the original System byte, lubricant ID, present step, dimension 3, three eligible objects, slot candidate classification, and readable actual lubricant path without synthesizing admission fields. Match the existing engine-version/banner setup before comparing exact original KEY bytes; do not add a new comparator normalization. Call the internal `saveInterObject` helper in recording mode. Require one write at its actual path, exact 631-byte oracle contents, no deletes, stable model/step identity, and, only after satisfying every literal loaded-object/candidate precondition above, the separate four-record in-memory table with template scalars. Require the file to contain exactly the two serialized CNTACT blocks specified above; never compare serialized block count to list count. Capture object lubricant/BCC state before/after and assert only fields supported by the existing `bccdef_lubricant` contract; a writer-byte match alone must not be described as full state equivalence.

For proving clear/rebuild rather than retention, first establish the untouched real fixture's exact output. Then a separate explicitly synthetic stale-list sentinel probe may append a discarded extra record and require its removal plus the same expected four records/output. Do not silently treat that sentinel as original corpus state. Reuse noncopyable RAII detachment for null no-effects/no-mutation and restored-owner usability if extending the earlier boundary coverage. A positive-ID missing/malformed lubricant fallback requires a separate qualified input; do not add one by guessing failure behavior.

Root runs focused `m2m.s.save_current`, `m2m.s.distortion_contacts`, relevant L3 loader coverage, then source-stable full launcher/freeze; no build or tests were run in this audit. Preflight uses Testkit CHECK/CHECK_EQ/REQUIRE, resolvable includes, no C assert, and scratch-confined probes. If direct loading/byte comparison disagrees, preserve the exact trace and diagnose it against the retained native path before editing production.

The next missing original witness, if claiming more than this regression, is a same-occurrence original state trace binding the RS39 operation/step, successful template import, list clear, ordered four appends and master-object lubricant writes through the writer. The existing historical output is adequate for byte assertions, not a substitute for that trace. Acquire only that bounded witness when its supported tooling is available; no reason exists to revisit the unrelated L5 positive-input gate or editor300 full-row lifecycle first.

## Audit boundaries

This report is the sole write. No source/test edits, build, test execution, GUI/app control, original executable, mutable REA database, parent state, deployed programs or Claude checkout was changed. Token measurement unavailable. No DMO or reconstructed DEF_PRE instance was built, tested or deployed by this converter audit.

Revision after `analysis/resume200-lubsystem-test-plan-review.md`: explicitly separated the conditional four-record in-memory derivation from the independently retained two-block file oracle, specified the exact 3D master skip and writer orientation filter, and restricted edit ownership to the test file. No source, test or build operation was performed.
