# Independent audit of frozen checkpoint68

The archive/source/artifact/test bindings verify. The 214-test suite and historical/fixture diagnostics remain stable, and the five focused targets execute 114 active cases. The new journal meaningfully preserves caller-owned recording observations through the guarded engine Save path. It does not provide native/public conversion diagnostics, native durable residues, path-bound fault injection or completed Type2/refresh behavior.

Only this report was written. The audit read frozen archives, receipts/logs and journal/SaveEffects source qualifications; it ran no executable/build/test and changed no source/shared state or GUI/REA state. Token use is unavailable; no extra acceptance is invented.

## Binding identity and integrity

| Evidence | Independently verified identity |
|---|---|
| Frozen receipt | build/wave3-checkpoint68-verified/receipt.json; SHA-256 f7510cc1b952eb62024760a9470c3924f0da9b189e157a005d23ba7712cf84b3 |
| ZIP bytes | e0708a9ba0ca535fc9aadcd3324f2ebd23614b5bc55137e2532f42cf15639a7e |
| Source tree | 3292 members; canonical WindowsPath-order SHA-256 5fd4d38d5a27563822e9d54e7a3be46c393fed9d25672d82d8ffacdc5e9db314 |
| Build | analysis/resume68-integrated-full-build.json; matching stable source before/after |
| Validation | analysis/wave3-checkpoint68-validation.json; SHA-256 0002e639f872f719f0146a72a028e5ed271ed7565cf123b304358eb3ae16486e |
| Full run | build/test-runs/1792882412491453100 |
| Test receipt | analysis/resume68-integrated-full-tests.json; SHA-256 927aeecec074d7cdd8211a2f817417f371399572a7efbabc1cdd9fb8452a6952 |

Recomputed ZIP byte hash and all source-member fingerprints. Archive/build/test/freeze source hashes and counts agree. The analysis test receipt is identical to the run receipt. All five hashed contract reports and five focused log hashes match validation.

execution_binding has equal 3292-file source boundaries and 211 equal binary manifests, stable_during_test=true. Independently matched 209 built-output sizes/hashes to the selected build; the other two entries are cmake.exe/python.exe. No mismatch was found. Both CTest/effective launcher exit 0, with no unreported or incomplete outputs.

Rehashed the preserved production artifacts:

| Artifact | SHA-256 |
|---|---|
| moproj2mst.exe | 16575cbe4181ddcddee9d07c296eae1c971c84ae415e11bfda6c7f411f6ab7e1 |
| moproj2mst.dll | d5999e13dfa9056a55dd204a8643e29bcdf6fbc6350746fb385fb373f87e928b |
| deformdb.dll | c89256fd4a9b226adc9e1eeb57dcaf36a99ab1dfae66b1fc1f419077e9577aa3 |

## Assertions and historical stability

The five focused summaries match frozen TEST_CASE declarations: table_owner_state 9, type2_save_node_prefix 10, type2_xml_prefix 13, widget_area_owner_state 12 and save_io 70, 114 cases total. Scanned frozen test_*.cpp files contain no raw C assert. No zero-case success is present. The known empty Python keyword-table log remains meaningful exit-status validation.

Full counts are 214 registered: 207 clean, six passed-with-logged-gaps and one passed-with-logged-differences; failed/skipped/incomplete counts 0. The six gap targets remain mdt.mdt_write, mdt.save_l6, moproj.deflate, moproj.props_l2, p07.corpus and s.save_current. Their explicit omissions/refusals and the separate permissive ABI smoke remain qualifications, not positive original acceptance.

Against checkpoint67, historical result/difference records are equal and changed_diagnostics is empty. The folder comparator source is unchanged (SHA-256 38c6fb80a3bc30a9cf15b49233a88e83f33a8dd4c4b1da5570254dd33679ad83); the full folder log is byte-identical (SHA-256 58dd03cef8bfc3a4f22ce0ca8b8296fd63fa5bcda50aa8811de986c58002f77b). It remains 93 SAME / 5 DIFF with the same four exclusions/noncomparison notes. A55/B98 retain one differing file each; B120/B121/B122 retain one differing file and 7/8/10 missing files.

All 16,728 fixture records are equal before/after; both snapshot-file hashes are ddcc3d21078789fc77fa292c110796e6fe70a15692103339436e8462b2d050b7. Normalization, capped diagnostic listings and removed work trees still prevent interpreting this as full regenerated raw-tree byte equality.

## Journal claims checked against frozen source

**One observation owner and one recording store.** StudyIo owns a StudyXmlJournal whose private data are attempt/event vectors and a recording-file map. xmlWrites() is the journal's exact attempt view. XML/PAR recording writes and later reads consult the same store, so the alias does not manufacture a separate diagnostic file model. Unknown recordedFile differs from a known empty file.

**Recording accepted bytes are exact simulation data.** Fault injection is below Text translation. The short-span test sends abcd/newline/Z and observes accepted memory bytes ab/CRLF/Z, including exact per-raw-span bytes. Its stream acceptance count is not used to synthesize a prefix of requested source data. Native attempts are explicitly recordingStoreTracked=false with no recording raw spans or recordedBytesAfterClose. The physical native component case independently reads its ordinary scratch-file bytes while asserting the journal does not claim those bytes as recorded/native contents.

**Open/result ordering is observational, not reversed effects.** OpenRequested precedes the factory. Successful recording truncation occurs during that open before OpenResult is observed; a false open produces no truncation/body/post-close snapshot. Stream request is recorded before delegation and a returned count only after actual return, avoiding invented success after a thrown boundary.

**Positional faults are not path-bound native fault plans.** RecordingXmlFault contains open/write-return/flush/error values and a label, but no expected path. Entries are consumed per XML-open ordinal; an exhausted vector falls back to the default successful simulation. Tests check attempt counts, labels, history and event indices, and some check current-file contents through the returned path. They do not establish a general path-matching/unconsumed-plan contract. Future claims about faulting a specific attempt/path across a larger Save sequence must add that binding evidence; the present per-instance, positional test-only label is accurate.

**The const alias is a live read-only owner, not an immutable snapshot.** It can outlive StudyIo/Session and retains values/history without holding those native owners alive. While the I/O owner continues writing, the alias sees appended events and updated current recording state. References into its vectors need not remain stable across appends. The journal has no per-file version IDs or complete global sequencing of later PAR/main-project writes, so it is not a fully versioned SaveEffects/durability ledger.

## SaveEffects and native/public lifetime boundary

Frozen engine.hpp adds FileEffects.xmlJournal as shared_ptr<const StudyXmlJournal>, explicitly excluded from applyEffects replay/SideBytes. engine::saveProject assigns out.project.xmlJournal before invoking the project writer. The three integration cases keep caller-owned SaveEffects outside the guarded call, require the actual refresh guard, and inspect that same journal after normal recording XML, failed-open profile and post-close error profile. This is genuine caller-owned recording observation through the shared engine path; immediate XML is not scheduled for a second replay.

That result must not be described as public-conversion diagnostic lifetime closure. The native convert branch still has local saved effects and invokes saveProjectFileNative without an external observation sink. saveProjectFileNative still passes a local unused FileEffects into its body. No C ABI/public result handle exports this journal after the guard/unwind. Separately retained StudyIo aliases can preserve their observations, but this new SaveEffects hook does not itself expose native/public conversion diagnostics.

The journal records device phases and observed counts/statuses, not the outer Type2 writer Boolean, last-error value, GuardedStop or RefreshBoundaryReached outcome. The XML result remains local in write.cpp and is followed by the same unsupported refresh guard after ordinary true/false. ErrorAfterClose therefore cannot by itself prove outer helper return, refresh delivery, PAR continuation, final project serialization or public success.

Native durability remains unknown from journal data. The new KEY-lock/PPV test supplies a host lock pattern and verifies the KEY-open model effect, callback at the PPV boundary, KEY-writer return and PPV rewrite. It intentionally does not assert KEY residue/durability or original physical fault parity. Keep that distinction rather than using the recording-memory fault model or adjacent lock test to invent native durable bytes.

## Completion conclusion

No frozen integrity/binding defect or newly hidden suite refusal was found. The inspected qualifications correctly call XML profiles positional test-only simulation and exclude native durability/public diagnostics; refresh UI/timer ownership, public Type2 Save and fresh original capture remain guarded. Freeze/validation retain objective_complete=false.

Preserve these exact claim limits. Native/public output handles, versioned multi-write provenance and outer writer/refresh outcomes are real follow-on acceptance dimensions, not solved by a shared const alias. No optional duplicate run, assertion weakening, transactional rollback or native-behavior redesign is warranted by this audit.
