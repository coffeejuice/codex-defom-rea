# CP196 historical DOE triad: no missing file is ready for public Save integration

The seven/eight/ten missing-file rows remain real retained comparator differences. None identifies a source-ready public Save increment independent of the active connector work. The strongest small existing file contract is B120's empty `DoeInputTable.XML` prefix, but that prefix is already implemented for an actual owned Study and does not create the study absent from this row's input, complete its Open, or return normally from public Save. Re-emitting an isolated helper's bytes would not close a historical omission.

This is a bounded read-only reconciliation of the CP195 raw log, retained before/after project files and original summaries, current guarded source, CP125/164 historical qualifications, and CP196 DOE integration review. Reference behavior is the retained original DEF_GUI_MO_64 v11.0.2 evidence. No reconstructed DEF_PRE instance was inspected, run, built or changed.

## What CP195 actually compared

`analysis/wave3-checkpoint195-validation.json` records 361 tests, 354 clean passes, six logged-gap suites and one diagnostic-difference suite, with no failures/skips/incomplete outputs or fixture changes. Historical totals remain 93 SAME / five DIFF. Its bound log is `build/test-runs/1792979918197143000/m2m.s.save_folder.log`.

| Row | Raw result, no DOE comparator adjustment | Missing project files |
| --- | --- | --- |
| B:120_B_DOE | `cog_p1_b1.moproj` differs; 7 missing; 0 extra | Seven Control files below |
| B:121_B_DOE_VAR | Same differing project path; 8 missing; 0 extra | Seven Control files plus root `temp.kwd` |
| B:122_B_DOE_GEO | Same differing project path; 10 missing; 0 extra | Seven Control files plus three geometry files |

The seven shared relative paths are under `doestudies/doestudy1/control/`: `cog_p1_b1.ds`, `cog_p1_b1.mst.temp`, `def_doe.ini`, `def_doe.par`, `def_doe_parser.ini`, `doeinputtable.xml`, and `samples_lhs.par`. B122 additionally lacks `workpiece-geometry_workpiece_1_lower.geo`, `workpiece-geometry_workpiece_1_nominal.geo`, and `workpiece-geometry_workpiece_1_upper.geo` in that directory. CP195 prints the lower file and `... 2 more`; the complete remaining names are corroborated by the actual B122 original diff summary and CP48 inventory, not inferred from the truncated log.

These are the comparator's retained raw classifications. Its declared view/banner profiles and diagnostic green test status do not establish unrestricted byte identity or fresh original-session acceptance. No comparator, fixture, classification or normalization was changed.

## Original action provenance is the first mismatch

Freshly reading the three retained `_before/COG_P1_B1.moproj` files establishes identical 27,053-byte inputs, SHA-256 `e5b529b6440ebd55bfbb701da26beae94a67c883f35c4c177a9a80caf530e523`: each has `Operation_List Number="5"`, no `SfDoeStudy` and no DoeStudies files. Each after-project instead has six operations and one `SfDoeStudy`.

The original `120_B_DOE_diff/summary.txt`, `121_B_DOE_VAR_diff/summary.txt` and `122_B_DOE_GEO_diff/summary.txt` explicitly list the added project outputs. Their screenshots/view-state sidecars are separate from the counted omissions. The retained frozen `build/reference-harness/billet-training-004/frozen-source/docs/re/moproj2mst/mdt/L6/doe.md` identifies 120 as a study without variables, 121 as TLOC on operation 1 with three samples, and 122 as Workpiece geometry. It records that Add DOE Study already writes the project and Control files, and the geometry '+' action writes the GEO copies before Save. Its mtime discussion is historical supporting evidence, not a new correlated native execution trace.

The actual after-XML confirms empty variables/operations for B120; `Transfer Time`, `TLOC`, three samples and an operation-1 binding for B121; and `Workpiece-Geometry`, three samples, operation-1 binding and three named GEO inputs for B122. B121's retained `temp.KWD` is 13 bytes. Thus a CLI Open–Save from the retained before state lacks the action-created Study/variable/output/geometry input. The after trees document effects; they do not supply a complete executable action recipe or prove a no-op Save should synthesize them.

After-project hashes are B120 `60addc2722e85f92a088fa07f77e8a966928ab5b934f161df87dcf6a417ed30e` (34,774 bytes), B121 `fb271b8ef6ab67d14fd215827bccb3ac9e462f4d3ccd7ca878d39efe3d421c45` (34,757), and B122 `b6d39b513db0abdd0b3c2f20246440bd39d47433dd300a80fd9013617f487032` (34,741).

## Missing-file contracts against current source

Source paths here are relative to `source/apps/moproj2mst/`.

| Missing effect | Current supported prefix and why it cannot close this row |
| --- | --- |
| `DoeInputTable.XML` | `src/doe/type2_save_node_prefix.cpp` requires the live Session/Project/owned Type2 SUB/Study/I/O/ModelManager. Empty variables admit immediate XML in `src/moproj/write.cpp:142-160`, then the exact normal-writer journal event and actual table refresh guard. Populated variables stop at dimension reconciliation; `doe_study.cpp:411,413,452` retains populated writer guards. B120's matching XML helper is already integrated, but the historical before state lacks its owner. |
| `DEF_DOE.PAR`, `Samples_LHS.par` | `saveType2SampleSequenceImmediate` exists in `doe_study.cpp:218`; its selected-mode then 0/1/2 sequence belongs after XML and normal table refresh. It is not a public Save continuation before that guard. Moving it earlier would change partial-effect order. |
| `COG_P1_B1.DS` | The retained active B120 post-action Open probe reaches `session_records.cpp:140-141` and refuses at DBLIST/ReportList import. It does not normally finish Open and hence cannot prove subsequent Save. Current `save_current.cpp:779-791` now reaches owned Regions mkdir and operation-model lookup, then guards nonnull enumeration; the old CP124 statement that Save stops before mkdir is superseded. The later DS output remains unreachable through a completed public path. |
| `COG_P1_B1.MST.TEMP`, `DEF_DOE_PARSER.INI`, `DEF_DOE.INI` | The frozen notes place these after MST/public Save continuation. `doe_study.cpp:776,790` contains INI helpers, not an admitted public continuation across earlier owner/refresh/output boundaries. A small INI payload is not an independent public Save contract. |
| B121 `temp.KWD` | The keyword helper exists, but current nonempty Type2 reader first appends its actual canonical Keyword, validates the actual notifier owners, then unsuppressed entry clears flag90 and stops before signal-key construction. Neither the empty-operations scalar prefix nor FRCROT's separate text prefix supplies real converter/file effects. The fixture-only B121 model writer explicitly disclaims a general native Save contract (`doe_study.cpp:699-727`). |
| B122 three GEO files | Source has bounded geometry/path prefixes, but `Study::geometryVars` still refuses nonempty mapping (`doe_study.cpp:132-134`). The original copies are edit-action effects; an after-XML filename or three retained payloads cannot issue missing import/copy ordering, lifetime and action ownership. |

CP125 correctly separated owned empty-Study XML/notifier prefixes from creation and complete Save. CP164 correctly retained the 7/8/10 historical rows as action-state comparisons. CP196's integration report updates the current nonempty reader and Regions boundaries without promoting either to full acceptance. CP195's constructor JE increment, and CP196's parent-slot investigation, establish no completed TaskInfo publication, codec output, receiver population or file writer. Their progress cannot be substituted for these missing contracts.

## Exact next original witness, not a source assignment

The smallest disjoint dependency investigation remains CP196 integration's same-occurrence nonempty `SetOprHasDoe` QString key publication, controlled by `resume160-doe-codec-next.md` and reconciled by CP171. In one natural selected Type2 occurrence, bind GUI RVAs `0x0d2995 -> 0x2cf3c0`, constructor `0x2cf41f`, caller return `0x0d299a`, actual notifier and source `2signalSetOprHasDoe()` to destination S+0x98. Observe the actual CMP/JE selection at QtCore `0x74126/0x7412e/0x74133`; on nonnull, correlate the reload `0x74184` and virtual decoder target `0x7247e` with its natural return/output. Follow publication through QtCore `0x174be7` to GUI `0x2cf425`, retaining bounded data/header/ref/payload and alias/ownership or the actual exception. At the first point RAX is a data pointer; at the second it is the object pointer and [S+0x98] is data. Stop before GUI `0x2cf454` map lookup. A global codec snapshot, ASCII source or conditional allocation contract cannot replace this witness.

All observations require the same run/input hashes, process generation, thread/event order, actual module paths/hashes/loaded bases and caller/destination identities. This could prepare `src/doe/variable_notify.cpp` / `tests/doe/test_keyword_notifier_entry.cpp`, disjoint from active connector and L6 files, but ownership may also require ProcessCodecOwner changes after review. It is not source-ready, does not itself write `temp.KWD`, and does not close public Save. The independent Regions alternative still needs the actual same-object QDir resolver result at QtCore `0xb01ab`, publication at `0xb01b4`, and separate fresh QFileInfo engine/metadata at `0xb8093` (CP156/171); mkdir alone does not supply them.

For historical row acceptance, retain or newly capture a fully qualified action sequence or a complete frozen pre-Save state including the actual created study/output operation and scalar/geometry inputs, then correlate original Open–Save–Close and ordered partial effects against the same candidate profile. Do not turn after-state artifacts into fabricated before-state owners. A key or Regions witness only advances one dependency; subsequent normal refresh and downstream ownership remain separate obligations.

## Receipt and scope

Root recovery status reports GUI validation blocked. The lifecycle assessment records `unsupported_release_api`; `python ../../tools/gui_validation_status.py` returned `release_ready=false`, `gui_control=release_unverified`, and no available GUI task. No GUI session was acquired, no release is claimed, and no new capture is authorized before a supported release operation and verifiable receipt/status exist. This does not block the completed read-only audit.

Fresh SHA-256 review snapshots: CP195 validation `c2c0c3c23971f5c3358ca75080947fa732dd56ee8cb475d203a589b1c599fb3a`; CP195 raw log `42b18a9762a0eec5550ea2c8bf896a8fe8b094e0844e2c24208fe6dad5d3d791`; CP196 DOE integration note `24d56d927129f988a867dd5afb594a63bb7a7cc94add4e19070f9c5bb1813658`; current `doe_study.cpp` `e70b1709e6b7ef229e3923271b5b579faec490d50aad80c64b342c77380a7180`; `moproj/write.cpp` `8af4be80324c39e1f8f452d7723bebd200e4b7372180d8e32c1e6ee32c2399a2`; `save_current.cpp` `62d4f2a827a5fc281692e0b5d8199a549a27594585018fed77ac3d399a46c455`. Concurrent owners must recheck source before implementation. Native address qualifications are inherited from the named reviewed reports, not new runtime observations.

Only this report was written. No source/test/build, fixture/original, executable/deployment, GUI, mutable REA database, shared parent state or comparator was changed; no tests or original process were run. No source-ready missing-file public Save work is released.
