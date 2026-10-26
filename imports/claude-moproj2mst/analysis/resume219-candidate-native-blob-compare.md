# CP219 B98 negative-ID candidate native-blob comparison

## Scope and method

Added `source/apps/moproj2mst/tests/s/test_b98_negative_id_native_blobs.cpp`. It copies the B98 `Gear_blank.moproj` input into isolated test scratch, changes only the two direct operation IDs (`1` to `-1`, `2` to `-2`), enables `Ctx.nativeOperationBlobs`, and calls candidate `openProject` without Save. The input fixture was checked against its existing SHA-256 (`e769faf514b335d56ffbb22f2c76490d029f2714ada9b26a75496fbd62ade7fd`); the exact two-ID variant is 10,934 bytes with SHA-256 `33537d2a1fd412537f17f3a9f1381a35b0c8569a290d1aacab4c492dde2e4fcf`.

The test snapshots the candidate scratch tree immediately before and after Open. It uses the tracked B98 `Operations/Task00001/ObjProperties00001.KEY` through `ObjProperties00003.KEY` as byte fixtures, with fixed expected SHA-256 values. The test asserts acceptance, `openedOk`, the exact three added paths (with no changed or removed entries), and byte equality plus hash equality for each generated root file. It has no dependency on the retained clone or on `C:\PROBLEM`; it performs no original-process or GUI run.

## Observed candidate result

Candidate `openProject` returned accepted=true and `project.openedOk=true`, with no caught exception. The complete post-Open scratch-tree delta contained exactly these three additions and no changed or removed entries:

- `ObjProperties00001.KEY`
- `ObjProperties00002.KEY`
- `ObjProperties00003.KEY`

Each generated root file is 169 bytes and byte-equal to its tracked task-local fixture. The six task-local files remain unchanged. Hashes are:

| Basename | Bytes | SHA-256 |
|---|---:|---|
| `ObjProperties00001.KEY` | 169 | `021caff357de2d72e5043483a36c35ef5848233258ba55530c0fde9499a0b713` |
| `ObjProperties00002.KEY` | 169 | `26e9d62de1e0faeb32262d6ef2897a50530452f3246089bdcedb2c5cf6ceac2e` |
| `ObjProperties00003.KEY` | 169 | `4e19975afc35c73512b5a1906044799959d5cb7113a1e104039d5b026f2360e8` |

In the earlier read-only comparison, all nine candidate ObjProperties paths (the three root files and six task-local files) existed in retained CP219 `C:\PROBLEM\p217n`, and all nine files were byte-for-byte equal at the matching relative paths. Its snapshot contained 36 file/directory entries and was unchanged by that comparison. This retained-clone result is report evidence only; the test no longer reads that clone.

## Interpretation and limits

This run establishes candidate Open acceptance and the exact candidate filesystem effects for this fixture and flag setting. The test asserts equality to tracked task-local fixtures and fixed hashes; the separate retained-clone comparison is not a new original-runtime parity run. The source-qualified path review predicts root-level writes when a negative operation ID leaves its task-directory field empty, and this candidate result is consistent with that derivation. See `analysis/resume219-objproperties-static-path.md` for source qualification and the limits on the earlier CP219 original-process observation. No conclusion is made here about the earlier original process completing Open or establishing diagram binding.

## Verification

- `python admin/check_new_cpp_bindings.py --project-root source/apps/moproj2mst source/apps/moproj2mst/tests/s/test_b98_negative_id_native_blobs.cpp`: passed, no issues.
- Focused target built through `python admin/build_candidate.py --label cp219-native-blob-fixture-bound --target m2m_s_b98_negative_id_native_blobs`: configure and build succeeded; source tree was stable at SHA-256 `6acc85f3a72b62039e357eb1ca85262b65e90ac6afb8f0dde3caffcf6dc36f82`.
- `ctest --test-dir build/candidate -V -R '^m2m\.s\.b98_negative_id_native_blobs$'`: passed, 1/1, 0.06 seconds. The test reported `acceptedProject=true`, `openedOk=true`, exactly three expected additions, and all three fixture-byte/hash checks true. Full converter suite remains owned by the integration run.
- Detailed test output: `build/logs/cp219-native-blob-fixture-bound-ctest.log`.
