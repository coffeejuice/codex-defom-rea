# Resume 48: parent-helper failure experiment — not executed

## Result

No parent-helper failure experiment was run, and no new scratch directory was created. The requested no-debug deterministic path is not available in the inspected materials. A race-based injector would not be reliable evidence, so I did not substitute one or modify the test executable/source to add a seam.

## Evidence checked

- The referenced `analysis/resume48-pipeline-parent-failure-contract.md` does not exist in this checkout.
- The current checkpoint 47 test executable is available at `build/candidate/bin/m2m_s_save_folder.exe`, SHA-256 `55bd1352512665eb7e841fc85385b4d15ae4fb022e6e9b956e4e841a14f37722` (4,130,304 bytes). The checkpoint 47 build record identifies `build/candidate` as its build directory.
- `analysis/resume47-pipeline-fault-test-plan.md` (SHA-256 `157aac18dede382c99d8ebea97ee1ef5eb20df80822205409181c3522b7e9bb2`) says no hidden fault-injection setting exists. Its parent-side plan needs a verified debugger child-creation barrier; it explicitly rejects file-polling injection because publication/collection may complete between polls and marks the debugger workflow as proposed, not validated.
- `admin/test_folder_pipeline_faults.py` (SHA-256 `de74006d023d4c39839a59a29fc500db625a7084984eed72e8e21e38ece0270d`) dispatches only `[exe, --folder-post-worker, job.bin]`. The implementation report `analysis/resume48-pipeline-worker-faults.md` (SHA-256 `c1ede38b9b07398342db99e2e712fde2fa1619d03277256f6c72f4b9daaf2d3a`) confirms it has no parent debugger/injection mode and cannot establish parent propagation, child cleanup or descriptor restoration.
- Existing `build/pipeline-faults-48a/receipt.json` is the direct-worker run (SHA-256 `7b52f08ba225d6b7a450414211c52c1759ef63e1538c32371915b9013cc72af6`). It is not evidence for parent failure behavior and was left untouched.

## Why no-debug execution cannot satisfy the requested claim

The parent creates the helper and starts conversion before the helper publishes results. There is no source-controlled pause/fault seam between helper creation and parent collection. Polling for a job, ready file, or pending result and then editing/killing the helper can lose the race; it also cannot prove the parent observed the injected state before consuming a result. The documented deterministic approach uses a newly owned debugger-launched process tree to suspend the exact child at creation. That is not a no-debug path and has not been established as supported in the current driver.

The available driver’s W0–W10 cases test the worker protocol directly, not the parent `runPipeline` coordinator. Running them again would not answer this task. No application, REA session, converter test sweep, debugger, process termination, or build was started here. No experiment timeout occurred; timeout behavior is therefore not measured.

## Next evidence needed

Either provide a reviewed, deterministic no-debug parent/helper fault contract and driver that can synchronize without racing, or authorize/use the documented debugger launch barrier after its feasibility is validated. Until then, parent failure propagation, no-partial-output behavior, live-child reaping and stdout/stderr restoration remain untested. Any later run should use a new owned scratch path, preserve its full raw manifests/logs and distinguish injection miss, process timeout and actual parent failure.
