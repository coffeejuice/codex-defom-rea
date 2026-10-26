# CP220 B98 native-blob candidate test review

**PASS for the candidate-only regression test and its recorded build/CTest result.** It checks the two-ID input variant, Open outcome, exact candidate filesystem delta, and fixture byte/hash equality. This does not establish successful Open or behavioral parity in the original DEFORM process.

## Review evidence

- Reviewed `source/apps/moproj2mst/tests/s/test_b98_negative_id_native_blobs.cpp`, the CP219 candidate comparison report, the fixture-bound build receipt, and the focused CTest log.
- The build receipt records successful configure and target build (`m2m_s_b98_negative_id_native_blobs`), exit code 0 for both steps, and identical before/after source-tree fingerprint `6acc85f3a72b62039e357eb1ca85262b65e90ac6afb8f0dde3caffcf6dc36f82`.
- Focused CTest passed 1/1. Its output records `acceptedProject=true`, `openedOk=true`, no caught error, and exactly three additions: root `ObjProperties00001.KEY` through `ObjProperties00003.KEY`; no other changed or removed entries.
- The test reads the B98 fixture through `M2M_ORACLE_B`, pins its project hash to `e769faf514b335d56ffbb22f2c76490d029f2714ada9b26a75496fbd62ade7fd`, copies it into isolated CTest scratch, and writes the variant only to the copy. No `C:\PROBLEM` path or dependency appears in the test. The fixture's current hash independently matches the pin.
- `replaceDirectId` restricts each edit to the named operation's direct `<Id>` and requires values `1 -> -1` and `2 -> -2`. The expected-byte construction and full-string equality enforce exactly those two inserted minus signs; the run reports 10,932 original bytes and 10,934 variant bytes, with variant hash `33537d2a1fd412537f17f3a9f1381a35b0c8569a290d1aacab4c492dde2e4fcf`.
- Each generated file is asserted byte-equal to its tracked task-local fixture and to a fixed SHA-256. The three resulting hashes match both the CTest receipt and the prior CP219 original filesystem-comparison report: `021caff...`, `26e9d62...`, and `4e19975...` (full values are in the test and CTest receipt). The report's separate retained-clone comparison also described byte equality; that clone is not read by this test.

## Qualification

The native-blob test itself validates candidate Open and candidate outputs against fixtures. CP219's original-process receipt is explicitly held for comparable successful Open: the original child survived a bounded observation and was then terminated without a direct Open return or acceptance witness. Thus the matching property-file hashes establish artifact-level agreement with the recorded original filesystem artifacts, not original-runtime Open parity, causal equivalence, diagram binding, or successful operation parsing. The CP219 comparison report preserves that distinction and does not claim otherwise.

No source, fixture, build, original-runtime, GUI, or shared-state edits were made. This review adds only this report.
