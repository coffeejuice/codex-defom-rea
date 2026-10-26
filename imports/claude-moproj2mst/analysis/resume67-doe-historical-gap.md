# CP67 B120–B122 historical DOE gap audit

## Current five-DIFF classification

The latest integrated CP65 run's `m2m.s.save_folder` log still reports the same three DOE rows as the CP64 archive:

| Case | Current diagnostic | Retained after-state evidence | Classification |
|---|---|---|---|
| B120 | 1 differing `.moproj`, 7 missing, 0 extra | A Type2 `DOE Study 1` subproject with no variables; its `DoeInputTable.XML` is 249 bytes, SHA-256 `1777bb9948fb2d6f55411f9dcfdc4a7c9196230086c0f86720255d03e76571e0`. | Before/after action-state mismatch: the `_before` input has no study; original adds the study and saves. A single-file prefix match below does not close the other six side effects or project delta. |
| B121 | 1 differing `.moproj`, 8 missing, 0 extra | After-state adds a Type2 study and a nonempty `DoeVariables` element; XML is 707 bytes, SHA-256 `18ec0c482730c82ade12e91771ca67e53e4604573032624b99f3f4829a86b069`. | Before/after action-state mismatch plus later unsupported variable import/output. The no-study `_before` project cannot synthesize the added variable. |
| B122 | 1 differing `.moproj`, 10 missing, 0 extra | After-state adds a Type2 study, nonempty geometry variable and DOE Output operation; XML is 817 bytes, SHA-256 `79d875540a7c5e84442e183be527f02fa3eb345e5c25900268b8478030c3bf9b`. Three retained GEO variants are each 41,410 bytes with SHA-256 `96f8cfaed97d25fedebd9fbe9e7c25127164700abe051aa78a6e84eedc67a526`. | Before/after action-state mismatch plus unimplemented geometry-variable parse/mapping and GEO-copy effects. Do not treat the ten missing files as one Save-only omission. |

The current run log is `build/test-runs/1792880735338521700/m2m.s.save_folder.log` (rows at lines 84, 93 and 103). It is a diagnostic folder sweep; the five historical DIFFs remain 93 SAME / 5 DIFF overall and are not fresh original captures. The before projects are the common 27,053-byte, no-study `COG_P1_B1.moproj` with SHA-256 `e5b529b6440ebd55bfbb701da26beae94a67c883f35c4c177a9a80caf530e523`. The retained action notes identify Add DOE Study, variable addition for B121, and geometry-file creation/DOE Output for B122.

## Earliest missing effect after InputTable XML

The current candidate Type2 native Save prefix reaches the real InputTable XML writer, then stops before delivering `DoeTableRefresh`; the public receiver is deliberately still guarded. Original call-order evidence in `analysis/resume64-doe-save-effects.md` establishes: pending study node → InputTable XML → `DoeTableRefresh` → current `Control/DEF_DOE.PAR` → LHS → FF → USR → DB name and final project serialization.

Therefore the first unimplemented effect after the XML write is actual `DoeTableRefresh` receiver delivery. Within the qualified receiver path, the first effect is `QTimer::stop()` (`slotUpdateUI`, `0x1400f4e10`), followed by the refresh body's `+0x98 = 1` store and live widget enable call before global current-Project/Study lookup (`analysis/resume65-doe-refresh-owner.md`). The earliest subsequent physical output is current-mode `DEF_DOE.PAR` (`0x1400d75a0`). It is too early to wire or claim that file while the actual receiver owner/effects are still refused. This distinction also preserves native failure order: the XML may already have been written when delivery fails; later PAR and final `.moproj` effects then remain unreached.

## Independent pre-refresh comparison available now

Yes. The CP65 `m2m.doe.type2_native_open_guard` run (`build/test-runs/1792880735338521700`, test log status `passed`) operates on a disposable copy of the retained B120 after-state. It parses that copy's empty-variable Type2 study and calls the native project writer. Although the test asserts the project truncation and refresh guard rather than an XML golden, the created `DoeInputTable.XML` is retained in the test scratch. Read-only comparison shows it is exactly 249 bytes and has the same SHA-256 as retained B120 after-state XML: `1777bb9948fb2d6f55411f9dcfdc4a7c9196230086c0f86720255d03e76571e0`. The source copy/manifest check is part of the test. This is a useful isolated pre-refresh physical-side-effect comparison, not complete Type2 Save, receiver execution, public C ABI Type2 selection, or fresh-original parity.

Candidate output inspected:

`build/candidate/t/bf16881e/type2_native_open_guard/11924-22233375/project/DoeStudies/DoeStudy1/Control/DoeInputTable.XML`

The retained CP65 CTest scratch contains four copies of this output; each inspected XML copy has the same 249-byte hash. The existing code also contains strict native write/error-order assertions in `tests/doe/test_type2_xml_prefix.cpp`, but those synthetic device cases are not DOE historical fixture comparisons.

The same pre-refresh claim does not extend to B121 or B122: their parsed `DoeVariables` has a child, and `readInputTable` currently refuses at the nonempty-variable guard (`doe/doe_study.cpp`, around line 341), before the Type2 XML output path. B122's nested `Geometry` data and three GEO files are later still. A candidate B120 XML match cannot stand in for either.

## Receipts and bounds

- `analysis/resume48-diff-b120.md`, `analysis/resume48-diff-b121.md`, `analysis/resume48-diff-b122.md`: retained input/action qualifications and missing-path counts.
- `analysis/wave2-doe-evidence.json`: retained after-state file sizes/hashes and project structure.
- `analysis/resume64-doe-save-effects.md`: original Save call/effect order.
- `analysis/resume65-doe-refresh-owner.md`: actual refresh receiver owner and first slot/body effects.
- `analysis/resume65-type2-native-prefix-test.md` and CP65 full-run `m2m.doe.type2_native_open_guard` log: candidate-only native prefix behavior.

This audit used the retained fixture and test scratch read-only. No source or build artifact was changed, and no GUI, original runtime, or mutable REA session was used.
