# CP90: native Type2 Save manager cache across DOE refresh and outputs

## Native same-instance witness

On the hash-verified original GUI PE (`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`), the public Save arm selects a Type2 SUB and obtains its DOE manager once for the arm:

1. At `0x14009964c..0x140099675`, the code indexes the active SUB list and tests the returned SUB with Type2 predicate `0x140010330`.
2. At `0x14009967b..0x14009969a`, it indexes the active SUB again, dereferences the stored SUB pointer and calls getter `0x1400b5a90`.
3. The getter body is exact: save `rcx`, load the same object, return `qword ptr [SUB+0x20]` at `0x1400b5a9a`.
4. The returned manager is cached at caller stack `[rsp+0x90]`. Null branches to the common suffix at `0x14009a095`; non-null uses that same cached pointer for the study node (`0x1400996ce`), InputTable (`0x140099770`), selected current samples (`0x1400997b2`), and fixed-mode calls (`0x140099a77`, `0x140099d7c`, `0x14009a081`). No later active-SUB/manager lookup occurs in this arm.

This proves one native manager identity is reused across the XML-refresh boundary and all four subsequent sample calls. It does not establish that a normal refresh callback leaves the original manager alive or unchanged; the pointer is a cached raw pointer, not a new allocation/lease.

## Return and continuation behavior

The order is InputTable/`DoeTableRefresh`, selected current `DEF_DOE.PAR`, then modes 0, 1, and 2 (`Samples_LHS.par`, `Samples_FF.par`, `Samples_USR.par`). The selected-table helper `0x1400d75a0` reads the manager mode at `+0x40`, its invalid byte at `+0x70+mode`, and the selected table at `+0x58+8*mode`. For a valid table it calls the common PAR writer `0x1400915290`; invalid or failed-open paths can return false.

Each fixed-mode helper `0x1400d7790` independently checks explicit range 0..2 and that mode's invalid byte, then calls the common table writer `0x1400915360` with the corresponding table. For modes 0/1/2, the public callsites pass literal EDX values 0, 1 and 2. The bounded caller windows show no test/branch on AL after any of these calls: `0x1400997b7`, `0x140099a7c`, `0x140099d81`, and `0x14009a086` continue into path cleanup/next phase. Thus ordinary false from current, LHS, FF or USR does not cancel later calls. A fault/exception that prevents normal return is a separate stop.

The caller likewise does not use the XML writer Boolean to decide whether to reach sample output. Normal return from `0x1400d3200` (including an ordinary false result) reaches `0x1400997b2`; a refusal/fault in `DoeTableRefresh` prevents that continuation. Native selected-current mode indexes the invalid-byte array before any explicit mode-range guard, so malformed mode behavior remains distinct from the candidate's safe refusal.

## Candidate owner binding and unresolved Save witness

The candidate has a structurally source-bound Study under the active SUB: `SubProject::ownedDoeStudy` owns the `doe` alias, and `Study::sub` points back to that SUB. `appendType2SaveNodePrefix` checks the same live Project/SUB/Study/I/O/ModelManager identities and appends the node. `writeType2InputTableXmlPrefix` locks and rechecks the Study, I/O and manager identities during the XML phase.

However, the save-node result stores `studyOwner` as `std::weak_ptr<Study>` and labels it a diagnostic lease, not native lifetime proof. After the XML writer, `source/apps/moproj2mst/src/moproj/write.cpp` calls `DoeTableOwnerState::requireRefreshDelivery`; the current implementation refuses at `signalDoeTableRefresh()` timer/UI delivery. No public Save path therefore reaches `saveCurrentSamples` or the 0/1/2 sample sequence. The active-owner tests establish owner identity during read/lazy sample handling and partial XML Save, but not the same Study identity across a normally returning public-Save refresh callback.

The precise missing same-instance witness is a candidate continuation that caches a strong Study owner/pointer from the selected SUB before XML output, performs/observes the actual refresh on the source-owned receiver, and resumes against that same Study/I/O/manager identity after normal return without reselecting a possibly changed active SUB. No such candidate public continuation exists while the refresh guard remains. This is also why the detached `saveType2SampleSequenceImmediate(const Study&, ..., StudyIo&)` is not sufficient evidence of public reachability.

## Next action

Do not enable public sample output based only on the native manager cache or candidate alias checks. First resolve the actual table/timer receiver and its normal-return lifecycle, then define an owner-bound Save frame retaining the original selected SUB/Study/I/O identities across that callback. Verify the resumed sequence uses that cached owner and preserves ordinary false continuation for each output. Keep variable-dimension, refresh, and Type3 guards unchanged until their separate contracts are closed.

## Evidence

Native offsets were inspected against the original PE using bounded DUMPBIN `/DISASM:BYTES` ranges around `0x1400995e0..0x1400997d0`, `0x1400b5a90`, and the four sample-call windows. Existing receipts provide the complete caller/sample contracts:

* `analysis/resume63-doe-public-save-order.md`, SHA-256 `354b25eb69b39d8aa0896fbe2ffd3d12d84f0f6de824599c7da1483ddf432335`.
* `analysis/resume63-doe-public-save-call.asm`, SHA-256 `7718dd142448531e4f565195829573cb3c78cf2fee4cbdcb330ed06918a54373`.
* `analysis/resume63-doe-public-sample-calls.asm`, SHA-256 `b0dac9a75c7c6ee3655a5d0a3f8c30de1423bf63f0630b189eb6851cebe48d91`
* `analysis/resume64-doe-current-samples.asm`, SHA-256 `0564db43b03c9a23de7857d8e35defa39e081e2e6bfd768ff95bdc9b7cc7b74f`.
* `analysis/resume64-doe-mode-samples.asm`, SHA-256 `30505e423d9ddbc2a6c4f741929939707ad84b336e502c1620dd1c25c54da9d`.
* `analysis/resume70-doe-par-first-output.md` and `analysis/resume71-doe-sequence-static-review.md` qualify callback stop versus ordinary false and fixed-mode continuation.

Candidate source reviewed: `source/apps/moproj2mst/src/doe/type2_save_node_prefix.cpp/.hpp`, `source/apps/moproj2mst/src/doe/type2_xml_prefix.cpp`, `source/apps/moproj2mst/src/moproj/write.cpp`, and `source/apps/moproj2mst/src/doe/table_owner_state.cpp`. This was read-only static/source review; no GUI, source edit, build, test or mutable REA action occurred.
