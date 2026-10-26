# CP203 B120 DOE state and serialization audit

The retained B120 pair cannot authorize automatic study creation during ordinary Save. Its before-state has no study; its after-state includes an interactive study addition and Save. Current CP201 source already contains bounded existing-study construction and serialization prefixes. This audit establishes no further source-ready creation or serialization increment from that historical pair.

## Direct retained-file check

On 2026-10-26, Python standard-library SHA-256 and XML parsing of `evidence/gui_mo/oracle_b/120_B_DOE_before/COG_P1_B1.moproj` and `120_B_DOE/COG_P1_B1.moproj` reproduced:

| State | Bytes / SHA-256 | Subprojects |
|---|---|---|
| Before | 27053 / `e5b529b6440ebd55bfbb701da26beae94a67c883f35c4c177a9a80caf530e523` | Type0, Id0, Active0; no SfDoeStudy or DoeStudies files |
| After | 34774 / `60addc2722e85f92a088fa07f77e8a966928ab5b934f161df87dcf6a417ed30e` | Type0 Id0 Active-1; additional Type2 Id1 Active5, name `DOE Study 1`, SfDoeStudy name `DoeStudy1` |

The seven after-state Control files were independently read and hashed. Paths below are relative to `DoeStudies/DoeStudy1/Control/`:

| File | Bytes | SHA-256 |
|---|---:|---|
| COG_P1_B1.DS | 2013 | `97e4aaa28aa545dd08360e98191fba3b2cfb6c39098e10aeb3b45d02c33e563d` |
| COG_P1_B1.MST.TEMP | 7249 | `0dbf96e9d2cf225cf6f19e4d91e32f3dc737913c9a3d34abbfb6c8ecd6d1141b` |
| DEF_DOE.INI | 3080 | `0f40dc9e2e72cafc9c0a4d7137751a5d5340362c6e7e4779a1975727ddbf4393` |
| DEF_DOE.PAR | 495 | `000edf802c560ab52196be82feea8ae0732cf263dcd1fe8b3e6c9f6b1ab0f42a` |
| DEF_DOE_PARSER.INI | 38 | `93ee4373d79f249409a144af1617283338a84582fc13462593a58389f2e3e195` |
| DoeInputTable.XML | 249 | `1777bb9948fb2d6f55411f9dcfdc4a7c9196230086c0f86720255d03e76571e0` |
| Samples_LHS.par | 495 | `000edf802c560ab52196be82feea8ae0732cf263dcd1fe8b3e6c9f6b1ab0f42a` |

These agree with `wave2-doe-evidence.json`. They establish retained bytes, not which historical action wrote each file or its execution order. CP201 `wave3-checkpoint201-validation.json` preserves B120 as one differing project/seven missing files/zero extra. Keep that raw difference and its unequal-action qualification.

## Actual current source, rather than preliminary reports

The current imported CP201 source is `source/apps/moproj2mst/`. The `optimization/test-speed/source/...` path in `resume200-doe-source-ready.md` describes an older location/state: that copy lacks the current keyword reader. Likewise, CP48's first-Image guard and the early wave2 blanket study-writer refusal are historical observations, not current frontiers.

* `src/core/convert.cpp:249` explicitly activates subproject0. `M2M_FLAG_DOE_TEMP` remains refused at line184. There is no public selection argument on this path.
* `src/model/subproject.cpp::makeSubProject` owns a Study for an input Type2 record. `src/doe/doe_study.cpp:138` binds the owning SUB, assigns `DoeStudy<id>`, and attempts directories through the configured I/O; its constructor ignores the directory helper's ordinary failure. This construction is driven by an existing subproject record, not an instruction to add a new record to a no-study input.
* The nonactive DB-name pass reads and ignores `Active`; `activateSubProject` lazily parses the selected contents. `parseSubProject` now calls `readLazyStudy` for Type2 SfDoeStudy. It does not refuse that tag wholesale. Type3 remains separately guarded.
* `src/doe/activation.cpp:39` sets study name, attempts InputTable parsing, invokes Samples LHS/FF/USR wrappers in mode0/1/2 order, then loads DEF_DOE.PAR. Ordinary helper returns are ignored by the parent. A thrown unsupported boundary stops later steps with earlier effects intact.
* `src/doe/sample_io.cpp:5` preserves the original load-wrapper bug: it writes a mode-only file before any memory update. Write/open failure returns without that update. On success, a nonempty requested table clears the CURRENT table and validates the REQUESTED flag. Do not substitute a read, skip empty-row effects, or roll back earlier file changes.

Consequently, the B120 before input reaches no Study owner. The saved after input constructs the inactive Study owner but default conversion still selects Type0. CP49's candidate-only result (`resume49-b120-poststate-result.md`) preserved all seven files; it did not regenerate them or provide fresh-original parity.

## Smallest serialization prefix already present

`src/moproj/write.cpp:140` requires the actual Save Session for Type2. `src/doe/type2_save_node_prefix.cpp:21` validates the live Session/Project/SUB/Study/I/O/manager identities and appends SfDoeStudy to a pending Sub_Project node. That node is not yet installed in the project DOM. A populated variable list stops at dimension reconciliation.

For empty variables, `src/doe/type2_xml_prefix.cpp:71` already opens/truncates the actual InputTable output device, serializes the bounded document, closes it, records stream versus file status and observes the normal writer return. Ordinary open/write failure still reaches the refresh boundary. `write.cpp` obtains the journal's actual return event, records refresh observation and calls the Session-owned table's `requireRefreshDelivery`. `src/doe/table_owner_state.cpp:233` handles suppression or clears notifier flag90 on the unsuppressed entry, then guards timer/UI receiver delivery. Even suppression does not authorize the public continuation. No DBName append, pending-node DOM replacement, later sample writes, or aggregate Save success follows from this prefix.

Thus the narrow evidence-backed prefix is already implemented: owned pending study node, then empty-variable InputTable I/O, then guarded refresh dispatch. The historical output cannot supply the missing actual receiver/timer delivery or its return. An isolated XML writer must not advance the public Save state or fabricate the other six files.

`src/engine/save_current.cpp:779-792` separately retains the current-operation DOE Control/Regions prefix and guard after operation-model lookup. Its preceding effects/failures remain distinct from project XML serialization. The original ordering and ignored failures in `wave2-doe-lifecycle.md:68-70` also prohibit treating all seven files as an atomic Save: template failure need not suppress parser INI; the final current-sample attempt uses the original nested Control/name/Control path; main-MST status is not an aggregate side-writer status. These are retained static contracts, not a newly observed original run.

## Missing same-occurrence action and runtime evidence

For B120 creation, the missing experiment begins at the actual no-study before-state and must identify the natural user/API study-add action, new SUB/Study ownership and id/name, diagram/current-selection transition, sample generation state, and subsequent Save under one process/thread/action sequence. The saved 100 empty sample rows are outputs of that historical state, not evidence that any ordinary existing-study Open or default constructor regenerates them. The after-state's SfDoeStudy tag does not identify an executable creation API or receiver. No complete same-occurrence creation trace was located in this bounded audit of wave2 DOE evidence/lifecycle, CP48-49, CP200-201 and CP203 publication reports.

For nonempty existing Type2 Open, `resume203-doe-publication-static.md` supplies the separate exact gap: natural SetOprHasDoe caller/constructor, consumed QtCore codec CMP flags and selected branch, actual decoder output if nonnull, and QString publication into the same S+0x98 slot. CP201's symbolic Report/TLS successor descriptions do not supply these values. B120 itself has no variables, so it cannot witness that nonempty notifier occurrence. A complete key-publication witness would still not close lookup/receiver dispatch, public Open or Save.

For the empty-variable serialization continuation, capture must join the actual writer-return event and Session-owned refresh notifier to natural key/receiver/timer delivery and return, preserving suppression, flag90 and ordinary writer failure. That is a different occurrence from nonempty SetOprHasDoe. Do not transfer a publication or receiver result between them.

Keep all current guards. A fresh copied existing-study input can support a separately identified Open-Save-Close comparison once its actual selected subproject and reachable runtime boundaries are qualified. Reordering subprojects or adding a study silently would change the experiment.

## Scope and qualifications

Read-only filesystem/source review plus this report only. No source edits, build, tests, original/candidate process, GUI, mutable REA session, fixture mutation or deployment. The reverse-engineer-anything skill was read and its retained-evidence/source-qualification rules applied; no new REA producer was required. Original target qualifications are inherited from `wave2-doe-findings.md` and `resume203-doe-publication-static.md`: GUI SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, QtCore SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. Static source call order is not runtime observation. The search is bounded to the files named above and relevant current DOE source; no exhaustive evidence-absence claim is made.
