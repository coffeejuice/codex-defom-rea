# Checkpoint 48: parent-helper failure contract and deterministic fixture

The accepted minimal pipeline propagates a post-worker comparison exception to a failed parent test. The standalone W9 result already establishes the regular-file expected-path failure at the worker boundary. A one-case parent fixture can exercise the remaining propagation path without a debugger, race, source hook or malformed conversion input. This report is a read-only source/receipt check and a future fixture design; no parent test, executable, build, GUI, REA or injection was run here.

## Bound evidence

- Minimal source: `source/apps/moproj2mst/tests/s/test_save_folder.cpp`, SHA-256 `38c6fb80a3bc30a9cf15b49233a88e83f33a8dd4c4b1da5570254dd33679ad83`.
- Direct-worker driver: `admin/test_folder_pipeline_faults.py`, SHA-256 `de74006d023d4c39839a59a29fc500db625a7084984eed72e8e21e38ece0270d`.
- Standalone receipt: `build/pipeline-faults-48a/receipt.json`, SHA-256 `7b52f08ba225d6b7a450414211c52c1759ef63e1538c32371915b9013cc72af6`.
- Executable recorded by that receipt: `build/candidate/bin/m2m_s_save_folder.exe`, SHA-256 `55bd1352512665eb7e841fc85385b4d15ae4fb022e6e9b956e4e841a14f37722`. The current file still matched when read for this audit; this does not substitute for the root's next build binding.

The receipt reports registered 11, passed 11, accepted true, and unchanged executable hash. Read-only recomputation verified all 57 retained post-run file hashes, all eleven directory manifests, all case statuses/failure arrays and the source/driver/executable bindings. No execution was needed for these checks.

W9 exited 2, published no done result and retained its work directory. Its actual diagnostic is:

```
Folder post helper infrastructure failure: recursive_directory_iterator::recursive_directory_iterator: The directory name is invalid.: "...\W9\expected"
```

The abbreviated path above represents an owned regular file under the W9 scratch directory. Full unmodified diagnostic: `build/pipeline-faults-48a/W9/worker.stderr`, SHA-256 `1e7f954b6e398049c9fd4028bc198a705b30423ac6157903a6b335d6b1dfe110`. W10 additionally returned exact tally `[1,1,0,0,0,0]` with normal comparison output and removed its owned work leaf. The direct tests establish worker parsing, failure and cleanup behavior; they did not launch the parent sweep.

## Static parent propagation

1. `runCase` (268) selects an input project and checks only `fs::exists(expected)`, not that expected is a directory. In discovery mode it records the event without converting. A regular-file oracle path therefore passes this discovery condition.
2. In the coordinator, the same `runCase` executes the normal scratch copy and sole serial `m2m::convert` call (305). Only an `M2M_OK` conversion reaches `publishPost(ordinal, true)` (314); a refusal would instead publish a no-compare operation later and would miss this intended failure seam.
3. The helper consumes the completed case, enters `compareCase` (209), enumerates the valid work directory, then attempts `files(expected)` on the regular file. W9 demonstrates that this runtime throws the expected recursive-directory-iterator exception. The throw occurs before tally/result publication and before the comparator's work-leaf removal.
4. The helper catch (507) writes its infrastructure diagnostic and returns 2. Its result `.done` is never created because result writing is after comparison. The `.ready` request remains available as evidence that comparison was requested.
5. With one case, the parent drains its pending queue at 721 and calls `collect`/`waitMailbox` (683/433). Missing `.done` plus a signaled exact child handle produces `Folder pipeline helper exited before result; inspect ...`. This check does not wait for the ten-minute timeout once the helper exits.
6. Stack unwinding invokes the stable stdout/stderr capture destructors, then the pipeline reset and child wrapper. `FolderChild` (557) waits/closes the owned process handle; in this fixture the child is already exited, so its terminate-if-live branch is not required. Discovery state is reset as the test unwinds.
7. `testkit::run_all` in `source/port/testkit/include/testkit/check.hpp` catches the exception, prints the failure diagnostic/row and returns a failing-case count. This executable registers one test, so the expected normal result is parent exit 1, child exit 2, `[FAIL] save_folder_sweep` and `1 of 1 cases failed`. Successful event replay, section summaries and scratch removal are not reached.

The failure path is source-established and supported at its worker boundary by W9. Parent runtime propagation and observable stream restoration remain unexecuted until the future root-owned test below runs. This fixture cannot demonstrate parent result-truncation handling or termination of a still-live helper.

## One deterministic fixture

Allocate a new, unique directory beneath this checkout's `build` tree after the root releases its freeze. Do not alter the production fixture corpus or shared CTest scratch. Within that directory create:

```
corpus/
  11_THEXP/                  full copy of the imported successful fixture
  19_XY_H01/                 empty directory
  52_BEND_2D/                empty directory
oracle/
  R11                       regular file, e.g. bytes "owned non-directory oracle\n"
scratch/                    isolated M2M_TEST_SCRATCH and temp root
launcher.stdout
launcher.stderr
```

The source fixture is `evidence/gui_mo/s1/corpus/11_THEXP`: 15 files, 521,873 bytes at audit time. Its `THEXP.moproj` SHA-256 is `c85e9bf8de92b5b6edc880bb2a1fba5dee3cadcc26753c95c5199853f09059b9`. The project text contains no drive-absolute path spelling (`:/` or `:\`). This is a copied accepted fixture, not an invented conversion project; root should retain a full source/copy manifest and its ordinary fixture immutability checks rather than treating this text scan as a complete side-effect proof. A current saved folder log shows successful `SAME B:R11` for this fixture under the normal expected directory; that is baseline evidence, not an executed result for the newly relocated fixture.

Use only the existing built, hash-pinned `m2m_s_save_folder.exe`, with no arguments. Set `M2M_FOLDER_SWEEP=1`, `M2M_FOLDER_PIPELINE=1`, `M2M_CORPUS=<owned corpus>`, `M2M_ORACLE_B=<owned oracle>`, `M2M_TEST_SCRATCH=<owned scratch>` and TEMP/TMP/TMPDIR to that same owned scratch. Preserve required reference/library settings from the root's focused-test environment. Launch hidden, without shell expansion, and capture the outer streams separately. A 20-second owned-process timeout is sufficient as a bounded test policy, not a measured runtime claim. A timeout is a failed/inconclusive test and must not be called successful cleanup.

Discovery is deterministic even though directory-iterator ordering is unspecified:

- No `_tools/rs_log.tsv` is required; its missing input stream yields no saved entries. The corpus pass finds the three directories but queues no RS cases.
- The fixed `11_THEXP -> R11` resave queues exactly one event, `B:R11`, because its copied project exists and R11 exists as a file.
- The two other fixed-resave directories must exist even though their expected R19/82_R52 paths are absent: `runCase` constructs `directory_iterator(input)` before checking expected existence. Omitting either empty placeholder would cause an earlier discovery exception and would fail to test the helper.
- The oracle pass sees only a regular file named R11, so it adds no `_before` cases. No special saved log, DOE label or case-exclusion branch can create a second event.

With one queued case, there is one normal conversion in one parent process and one post helper. No scheduling-dependent injector is involved: the oracle is already a regular file before launch and remains untouched throughout discovery/conversion.

## Required future acceptance receipts

First pin the newly built executable hash and source freeze; do not reuse the historical binary hash above if the root rebuilds. Optionally establish a relocation control in a separate owned attempt using the same copied corpus but R11 as a directory copied from the accepted oracle. A control passes the harness contract when it compares exactly B:R11 with no refusal and exits 0; DIFF rows are report-only and must not be mistaken for converter parity failure or hidden by the test.

For the fault attempt require:

- Parent exit 1 and outer stdout `[FAIL] save_folder_sweep` plus `1 of 1 cases failed`; outer stderr contains the parent helper-exited-before-result diagnostic. Their presence outside the coordinator captures establishes observable descriptor restoration for this path.
- Retained IPC `job.bin` parses exactly as minimal schema, count 1, ordinal 1, name B:R11, expected path equal to the owned regular R11 file and work leaf equal to the owned sweep/1 directory. Retained `1.ready` parses as ordinal 1 and compare=1. No `1.done` or successful section-summary replay is accepted.
- Retained `worker.stderr` contains the same filesystem operation/failure family as W9. `case.stdout` should have no SAME/DIFF row because expected-file enumeration fails before printing comparison diagnostics.
- The converted work leaf remains available; the input source/copy and regular oracle file retain their recorded hashes. No success cleanup is expected on this exception path. Administrative deletion of this isolated attempt, if later desired, is separate from parent cleanup.
- Record parent PID, elapsed time, all stream/file hashes, process exit evidence and whether any driver timeout cleanup was used. External child reaping/handle-close coverage must remain explicitly limited unless the root independently records the exact owned child's exit. The source wrapper's wait/close is static evidence, not a substitute for such a runtime receipt.

Any refusal, copy skip, missing placeholder, fixture hash change, earlier conversion exception, timeout, mismatched manifest or different failure point invalidates the intended seam. Retain the evidence and report it rather than interpreting every nonzero exit as a pass. This design adds one bounded harness-failure test and does not change conversion semantics or claim full parent fault coverage.
