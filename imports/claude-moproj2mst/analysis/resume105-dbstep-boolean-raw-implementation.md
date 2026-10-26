# CP105 strict public DB-step Boolean raw comparison: release

A new focused public WRITE_SAVE test now compares the retained
133_B_DBSTEP_BOOL3 fixture's entire MST and four side files as raw bytes.
It exercises one public conversion, including actual Session Open/Save and
adapter teardown on return. It does not implement or certify accepted native
Close, fresh original behavior, modal-loop parity or CP97 mesh admission.

Root authorized only NEW
`source/apps/moproj2mst/tests/s/test_session_dbstep_boolean_raw.cpp` plus this
receipt. Production/shared files, existing tests, CMake and retained fixtures
were not edited. Target m2m_s_session_dbstep_boolean_raw / CTest
m2m.s.session_dbstep_boolean_raw contains one Release-active case. No worker
build/test/GUI/original execution or REA activity occurred.

## Exact fixture/profile qualification

The input is the full 133_B_DBSTEP_BOOL3_before directory, with its
COG_P1_B1.moproj and 9,216,000-byte DB. Retained mkdbstep2.py intentionally
prepares active operation2 as Boolean3D, OprFlag0 and empty Properties using
a copied completed3D COG_P1_B1 DB. It is a constructed historical Open/Save
profile, not an unmodified production project or simulation-continuity proof.
The after directory is retained original Save output. Historical rs_log.tsv
records its saved capture; prior CP103 public folder sweep records SAME.
Neither supplies fresh original execution or native Close acceptance here.

The test requires exact expected sizes, path-free expected bytes, an initial
MST/SimCtrl difference and three absent-file preconditions. It calls public
convert(...WRITE_SAVE,1252) once, without private effect services, locks,
callbacks, alternate settings or failure injection. Public M2M_OK and all five
raw byte comparisons are mandatory:

| Relative output | Bytes | Required transition |
|---|---:|---|
| COG_P1_B1.MST | 7196 | Replaces differing before MST. |
| Objects/Object00002.KEY | 2411 | Creates an absent file. |
| Operations/Task00002/Boolean.BWZ | 182 | Creates an absent file. |
| Operations/Task00002/ObjProperties00004.KEY | 169 | Creates an absent file. |
| Operations/Task00002/SimCtrl.KEY | 2111 | Replaces differing before bytes. |

No relocation, newline/number normalization, tolerance, view or banner mask
is applied. Unlike CP103's MST-existence check, this compares the entire MST.
Any native adapter refusal or missing/different selected file fails the test.
The comparison is exactly five outputs, not full-folder equality.

## Disposable path and full mismatch evidence

The project copy uses a fresh owned C:/PROBLEM/g105_<pid>_<tick> leaf. Its
maximum numeric-root length plus the retained maximum relative length43 is
under100; copied/generated paths are checked. Recursive removal requires its
owned flag, absolute parent C:/PROBLEM and exact numeric-prefix provenance.
Normal cleanup is checked after restoring the thread directory; constructor/
assertion/conversion unwind has bounded fallback cleanup of only that leaf.
The test never removes C:/PROBLEM or a preexisting leaf. Denied filesystem
access fails without permission broadening or silent relocation. Root owns
any supported execution/approval decision if that occurs.

On a conversion refusal, output-read error or raw mismatch, the test first preserves all five
expected binary outputs and every available actual binary output in a fresh
`M2M_TEST_SCRATCH/b133_raw_<pid>_<tick>` diagnostic directory. Missing regular
files receive explicit .missing markers. Status/open/read failures receive
.error markers, and available bytes from an incomplete read receive .partial
files; result.txt records code/message and
the disposable project path. These checkout-scratch diagnostics survive the
short project's cleanup. A diagnostic write failure is a test failure, not a
normalization or pass. No raw candidate data is discarded to make a comparison
succeed, and large binary differences are not expanded into the test log.

## Static preflight and acceptance limits

Static checks passed: one Testkit case, quoted include resolution through real
roots, run_all main, no REQUIRE_EQ/C assert; exact public flag/result gate and
raw comparisons; complete failure diagnostics before result assertion;
owned/bounded cleanup and directory-restore timing; verified absent/changed
before states, all expected sizes/path-free bytes and numeric path bound.
Existing production/Session and CP103 strict test hashes remain unchanged.
Root must configure/build/run the new target and retain the resulting evidence.
No runtime success or actual cleanup completion is claimed by this receipt.

## Frozen source and raw oracle SHA-256

| File | SHA-256 |
|---|---|
| source/apps/moproj2mst/tests/s/test_session_dbstep_boolean_raw.cpp | 5778b1d79175fb2eae29d2d4d043d56a7881d0241206f7cd3a4cfdcbe34873c1 |
| analysis/resume105-raw-comparison-coverage.md | 595ddad85d96578451df75a20c9bd6984251818c6a5ad732f32c43c5563425c6 |
| evidence/gui_mo/oracle_b/133_B_DBSTEP_BOOL3_before/COG_P1_B1.moproj | 8e5e896cce67164430324cddc25324043449428b789d201bb3bef9c49cc89f79 |
| evidence/gui_mo/oracle_b/133_B_DBSTEP_BOOL3_before/COG_P1_B1.DB | 707326e90e4f8fb0fee2aa1fa9f174e8d6e40b213fe2a7c0a828be00829671df |
| evidence/gui_mo/oracle_b/133_B_DBSTEP_BOOL3/COG_P1_B1.MST | 9bad363c4454ed9339129c6845ee82ebc044565e9165bcdb846f4fc35ed75ef1 |
| evidence/gui_mo/oracle_b/133_B_DBSTEP_BOOL3/Objects/Object00002.KEY | 7adeee1681d831de2c49c972735ee9d76d102fe13d2b5a0917bafe33402f94e1 |
| evidence/gui_mo/oracle_b/133_B_DBSTEP_BOOL3/Operations/Task00002/Boolean.BWZ | 0d696c7c8b4ed38bd4b0a98dc2e24f5bf1bd68059a902c1ccc3387c99eb00ca8 |
| evidence/gui_mo/oracle_b/133_B_DBSTEP_BOOL3/Operations/Task00002/ObjProperties00004.KEY | 2dc78ce7840dc99712a35dd66e6ee132324960dd97c15902276fcf6fc6fa0f4a |
| evidence/gui_mo/oracle_b/133_B_DBSTEP_BOOL3/Operations/Task00002/SimCtrl.KEY | 6f0dc482e89e19de938530257297c098fc239abfaacb4ba1344f1d530897fe15 |
| source/apps/moproj2mst/src/core/convert.cpp (unchanged) | 1fd1592c8bd8623e2d4736af077cdef4cd1f530227d87059721f4dbe205f6a30 |
| source/apps/moproj2mst/src/engine/conversion_session.cpp (unchanged) | f1ff5d1c3934b1daa769f58c233b397a48c6eacb7cfdf790dba36edfea820604 |
| source/apps/moproj2mst/tests/s/test_session_cogging_save_geometry.cpp (unchanged) | 8c9adae443a3962ee9e0de52e50a5bdc05e5e2761f05a0a06942adaf427006a1 |

The evidence-only CP105 report retains generator/log qualification and initial
MST/SimCtrl hashes. This new test/source is frozen for root integration.

## Independent-review diagnostic repair

Root's review found that a failed actual-output open/read could assert before
the raw bundle was written. The repair replaces only post-conversion collection
with an assertion-free result per output. It uses error-code filesystem status,
checks stream open, reads binary chunks, preserves bytes already obtained, and
distinguishes badbit/failure from normal EOF. Standard collection exceptions
are recorded as errors. All five outputs are collected before any result or
comparison assertion, even if an earlier file failed.

The bundle retains every expected file, complete actual bytes or partial bytes
as available, and distinct missing/error markers. An unreadable regular file
is no longer described as missing or as an empty complete output. Successful
comparison still requires a complete/error-free read and exact raw equality;
no acceptance condition was relaxed. Diagnostic-destination write failure
remains a hard test failure; an unwritable destination cannot promise a bundle.

Static checks passed for the assertion-free collector, error/EOF order, full
set collection before bundle/result failure, all diagnostic suffixes and the
unchanged strict byte/result predicates. One test case remains. No build or
test was run. The prior released test hash was
f54a95161a5b4b29529141b9c1aed3c74b848644e3ddf3205d19e5baa60de073;
the updated frozen hash is in the receipt table above.
