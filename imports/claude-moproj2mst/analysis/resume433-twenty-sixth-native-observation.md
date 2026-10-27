# CP433 twenty-sixth monitored inert observation

CP433 used one new disposable occurrence `c434d35c851e4619a8e99145c4f956ed` from clean pushed commit `81cf122a8ea5251f0c455694b1fd36faf596d081`. REA's prior read-only session had closed (`open=false`, `cleanup_pending=false`). Independent conditional review and root preflight verified matching local/SSH remote HEAD, 43 fixed CP3xx+CP420 source hashes, zero trial bytecode, fixed Python/WORKER/source/build identities, no `python.exe`/`worker.exe` process, fresh report/monitor paths and an empty CP432 failure-file namespace. Four unrelated `pythonw.exe` processes in a DMO worktree were left untouched. The fixed CP350 monitor ran the inert CP316 WORKER once; no DEFORM process or GUI control was used.

The monitor observed controller exit `1` in about 5.5 seconds, with both streams terminal and no kill. CP350 is `accepted=false`, `phase=hold`, `TopFailure: read failed (109)`, cleanup `closed`, exact runner waited and job empty; no matching trial process remained afterward. Before/after manifests match. The valid 39-stage CP349 child trace ends at terminal read, `cp333_pipe_wait_retry_budget`, runner prefix `result`, failure-only WORKER `exit_0`, then the unchanged absolute pipe deadline Hold. No terminal claim was observed.

The new failure-only CP432 receipt is present. It records the in-memory ACK stage returned with 4,532 ms remaining; CP330 post-ACK budget passed and entered `NativeWorkerCreator.complete_success()` without returning. CP329's exact WORKER wait returned status `WAIT_OBJECT_0` with 4,532 ms supplied; its immediate post-wait budget passed with 4,500 ms remaining. This rejects CP431's *wait-return-at-deadline* explanation for this occurrence. It does not locate the later stall inside CP329 or prove its exit-code query, job checks, cleanup, terminal write, accepted owner, or public converter parity. The later failure-only WORKER exit-zero snapshot cannot substitute for CP329's success gates. The historical v4 sidecar now intentionally stops at `result` because CP432 removed post-ACK storage.

Raw evidence, retained without normalization:

- `cp350-inert-trial-c434d35c851e4619a8e99145c4f956ed.json`: SHA-256 `9ccb317d8d251a3b6667fadf4bc45544b22feaa9c1eadde1c81cb84538ea0861`.
- `cp350-monitor-c434d35c851e4619a8e99145c4f956ed.json`: SHA-256 `43bee784a0ba19679f7c657805486c403708c29934550458b8cc7041db710e87`.
- `cp349-child-trace-99c7ada7e52101bdd0281567ab2ed1b6.jsonl`: SHA-256 `0001b503237d5363a66bcf0851f581dccac44bc2a5d16baf86437d321b9f93f2`.
- `cp429-runner-4a8b595a6b4a4e71ef30da2882a18e1d549246353d53521c9ca6d0bfd702c0f0.txt`: SHA-256 `4659172d62395abfe828e0ead6a315e159b0380cac4a276db4f76d3ff7342521`.
- `cp432-failure-c8fac35aadf71cb9f83d0011bfcd2ba2fadc380067fe4aea7b1ead46f51fa2ef.json`: SHA-256 `cc4ae0bc5f894b49a7c095959a839f42ccfcd134395ee09b97a5785c2843181d`.

Independent raw review confirmed those milestones and noted that CP335's already-caught exception is absent from the CP432 snapshot. CP434 next carries a bounded error/cause plus CP329's last post-wait gate in memory to distinguish exit query, WORKER/CHILD accounting, pending pipe evidence and cleanup. It must add no clock calls or storage on the completion path. Re-test with injected branches, independently review and freeze before considering another occurrence. Do not replay CP433.
