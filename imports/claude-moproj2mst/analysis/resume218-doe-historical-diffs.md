# CP218 DOE historical DIFF reconciliation

## Result

The three retained folder-sweep cases are action-created DOE studies, not plain
Open/Save parity cases. Their historical inputs lack the DOE study/subproject
state that the original GUI adds before Save. The comparisons therefore establish
missing effects, but do not by themselves establish that one writer fix will
close them. The oracle-side original outputs provide useful byte witnesses; the
candidate-side outputs for these sweep runs were not retained.

`analysis/wave3-wizard-native-validation.json` and checkpoints 120, 121 and 122
repeat the same counts: B120 has 7 missing, B121 has the same 7 plus `temp.KWD`,
and B122 has the same 7 plus three GEO files. Each also has one changed
`cog_p1_b1.moproj`, no extras, and no comparator adjustment. The detailed names
are retained in `analysis/resume101-abi-fixture-raw-diff-audit.md` and the B122
inventory in `analysis/resume48-diff-b122.md`.

## Original produced-file witnesses

These sizes and hashes are from the retained original `evidence/gui_mo/oracle_b`
case trees. They describe original output, not candidate output. The common seven
paths are below; the changing values show that several files depend on the DOE
state and cannot be emitted as fixed templates.

| Relative path under `DoeStudies/DoeStudy1/Control` | B120 bytes | B121 bytes | B122 bytes |
|---|---:|---:|---:|
| `cog_p1_b1.ds` | 2,013 | 2,013 | 2,013 |
| `cog_p1_b1.mst.temp` | 7,249 | 7,316 | 7,570 |
| `def_doe.ini` | 3,080 | 3,080 | 3,080 |
| `def_doe.par` | 495 | 1,396 | 1,389 |
| `def_doe_parser.ini` | 38 | 38 | 38 |
| `doeinputtable.xml` | 249 | 707 | 817 |
| `samples_lhs.par` | 495 | 1,396 | 1,389 |

The byte-identical common-file groups have these original SHA-256 values:

- `cog_p1_b1.ds`, all cases: `97e4aaa28aa545dd08360e98191fba3b2cfb6c39098e10aeb3b45d02c33e563d`.
- `def_doe.ini`, all cases: `0f40dc9e2e72cafc9c0a4d7137751a5d5340362c6e7e4779a1975727ddbf4393`.
- `def_doe_parser.ini`, all cases: `93ee4373d79f249409a144af1617283338a84582fc13462593a58389f2e3e195`.
- In each case, `samples_lhs.par` equals that case's `def_doe.par` byte-for-byte (B120 hash `000edf802c560ab52196be82feea8ae0732cf263dcd1fe8b3e6c9f6b1ab0f42a`; B121 `1b6887361a11fb31ddd728ab10c4c35f6405a5399a849e4f6d2b4285d0c50791`; B122 `770d49eaeded318b08b88eec289ff26f133bf874f098a795e7e757a61f20d56f`).

B121 additionally lacks project-root `temp.KWD` (13 original bytes; retained at
`evidence/gui_mo/oracle_b/121_B_DOE_VAR/temp.KWD`). B122 additionally lacks
`workpiece-geometry_workpiece_1_{lower,nominal,upper}.geo`, 41,410 bytes each;
each retained original file has SHA-256
`96f8cfaed97d25fedebd9fbe9e7c25127164700abe051aa78a6e84eedc67a526`.
The GEOs being equal in this witness does not prove general geometry-copy
semantics.

The retained case qualification in
`build/reference-harness/billet-training-002/frozen-source/docs/re/moproj2mst/mdt/L6/doe.md`
identifies B120 as “Add DOE Study,” B121 as TLOC on operation 1 with three
samples, and B122 as Workpiece Geometry. It records that Add DOE Study creates
the `.moproj` and Control effects before Save, and the geometry “+” action copies
the three GEO files before Save. B122's before/after evidence also shows a new
DOE Output operation and DOE subproject, not just a serialization change.

## Existing helper boundary and next evidence

Source already has isolated pieces for this family: `doe::makeStudyDirs`,
`writeInputTable`/`saveInputTable`, sample-table writers,
`writeParserIni`, `writeDefDoeIni`, `mst::writeSubMst`, and the bounded
`keywordValues` path. `analysis/wave2-doe-type2-activation-save-decompile.json`
and the archived DOE workflow note give the original call order: constructor
creates study/Control/Solutions/Output directories; project Save writes the
input table and PAR files; then the DOE save path writes MST.TEMP and parser/DOE
INI outputs. Current `doe_study.cpp` still refuses nonempty variable payloads in
the input-table writer, and the TLOC keyword path refuses group 6. The GEO
read/write path and its variable-to-operation mapping are also not complete for
this case.

There is no source-closed increment that can be claimed to reconcile all three
historical DIFFs from the retained folder sweep alone: the sweep does not supply
the intermediate post-action study state, and the emitted files vary by state.
The next useful original witness is one controlled, staged trace per profile
(empty study / TLOC with three samples / geometry with three variants), preserving
for each step the input project and sidecar hashes, resulting project and files,
and ordered file effects from Add Study, variable/geometry editing, and Save.
For 120, the first bounded slice should end after Add DOE Study and Save, with
the `.DS`, sample PAR, MST.TEMP, parser INI, and DEF_DOE.INI bytes retained; this
would separate constructor/action effects from later variable writers. Then
capture 121's TLOC parse and 122's geometry copy/map effects before implementing
their dependent serialization. The present report does not request or perform
an original/GUI run.

No source, fixture, project input, or shared recovery state was changed.
