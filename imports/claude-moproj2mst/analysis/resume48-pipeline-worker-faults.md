# Checkpoint 48: direct post-worker fault driver

Implemented `admin/test_folder_pipeline_faults.py` for the eleven direct-worker cases W0–W10 in `analysis/resume47-pipeline-fault-test-plan.md`. Root owns execution and acceptance. No folder executable, original application, conversion, build, GUI session or REA session was run by this worker, and the C++ test/shared runner were not edited.

Driver SHA-256 at handoff: `de74006d023d4c39839a59a29fc500db625a7084984eed72e8e21e38ece0270d`.

C++ source SHA-256 observed unchanged at handoff: `38c6fb80a3bc30a9cf15b49233a88e83f33a8dd4c4b1da5570254dd33679ad83`.

## Invocation

Supply an absolute already-built test executable below this checkout's build directory, its explicit verified hash, and a new absolute scratch directory below build:

```powershell
python admin/test_folder_pipeline_faults.py --exe <absolute-build-path-to-m2m_s_save_folder.exe> --exe-sha256 <verified-64-digit-SHA-256> --scratch <new-absolute-build-scratch-path>
```

Default limits are 5 seconds per worker and a 120-second suite deadline; overrides are bounded to 0.1–20 seconds per worker and 1–120 seconds for the suite. Process termination/reaping adds at most the driver's bounded cleanup waits. Windows is required. The driver refuses an executable outside this checkout's build tree, the wrong basename, a mismatched hash, nonabsolute paths, an existing scratch directory, or scratch that resolves outside build. The generated inputs and all logs remain beneath the new scratch directory.

Only `[exe, --folder-post-worker, job.bin]` is dispatched, with shell disabled and a hidden console. It clears inherited folder/corpus settings, sets scratch/TEMP/TMP/TMPDIR to each owned case directory, and records an environment digest without printing inherited values. No other app, parent sweep, converter dispatch, debugger or child-injection mode is implemented. Timeout cleanup uses only the Popen handle obtained for that direct worker; the direct dispatch creates no children.

## Checks and receipts

The driver constructs exact minimal-protocol manifests/requests before each launch and tests empty jobs, truncated numeric/string framing, oversized job count, truncated request, wrong ordinal, invalid compare operation, trailing request data, no-compare results, filesystem comparison failure and successful comparison cleanup.

It verifies expected exit codes and diagnostic prefixes/substrings, empty outer launcher streams, exact result schema/manifest bytes/ordinal/operation/tally/output/EOF, published result file sets, retained work leaves for failures/no-compare, removal after successful comparison, and unchanged synthetic expected fixtures. W9 additionally requires a filesystem/recursive-directory-iterator diagnostic; root should retain the native message if its runtime wording differs rather than weakening the check silently.

Each attempt retains command, PID, launch timestamp, environment digest/overrides, input hash, pre/post file-and-directory manifests, file sizes/hashes, elapsed time, exit code and failure details. The suite writes `receipt.json` after every attempt so completed results survive later interruption. Final `receipt.json` and `receipt.md` remain under the supplied scratch root. The executable is hashed again at the end. Exit 0 requires all eleven cases to pass and its hash to remain unchanged; timeout, launch failure, deadline exhaustion, malformed result and executable changes fail acceptance. No normalizations hide byte differences.

Source-only AST parsing and dispatch/mutation-boundary assertions passed. They confirmed one Popen launch site, only the post-worker argument, no parent debugger/injection path, no sharded/copy-worker dispatch and no recursive deletion. These are implementation checks, not runtime acceptance; no generated success receipt exists yet.

The driver does not establish parent failure propagation, parent child cleanup, descriptor restoration, converter parity, fixture-corpus fidelity or speed. Those claims remain outside this direct-worker task.
