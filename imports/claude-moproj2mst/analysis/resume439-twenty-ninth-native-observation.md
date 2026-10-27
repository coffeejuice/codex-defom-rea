# CP439 twenty-ninth monitored inert observation

CP439 used one disposable CP316 inert occurrence `90b20f87eff24e91a64a9a0598f2160d` from clean, pushed commit `453475e1894f9b1fb39d80082eab0a9ebbd03dae`. Independent preflight checked 43 fixed source modules, zero trial bytecode, exact Python/WORKER/source/build identities, absent report and monitor paths, empty CP438 failure namespace, and no `python.exe` or `worker.exe`. Root confirmed REA `open=false`, `cleanup_pending=false` and repeated the source/process/path checks immediately before launch. No original DEFORM app or GUI control was used.

The monitor observed exact controller exit code 1 in about 5.62 seconds, both redirected streams terminal, no monitor kill and `trial_accepted=false`. CP350 reports `accepted=false`, `phase=hold`, `TopFailure: read failed (109)`, cleanup `closed`, and matching before/after source manifests. The exact child trace again reached runner RESULT, failure-only WORKER exit-zero observation and the unchanged parent terminal-read retry deadline Hold. Terminal claim was not observed. Later cleanup showed empty jobs and no matching trial processes; this cannot replace the earlier failure-point count.

The schema-4 failure receipt captures the strict CP329 gate: the exact WORKER wait returned status 0, the immediate budget passed with 4,578 ms remaining, and existing accounting saw WORKER ActiveProcesses=1 and CHILD=0. The subsequent fixed class-3 query on that retained WORKER job returned a structurally valid list with assigned=1, listed=1 and PID `37656`. The original Hold was `both WORKER and CHILD jobs must be empty`. This list is later than the accounting sample. It identifies a member of this occurrence's WORKER job, but no image or process lineage. The CP350 owner PID `41656` is the outer Python child and differs from the listed PID; CP350 does not retain CP329's creation-returned WORKER PID, so identity and cause remain open. Do not infer `conhost.exe` from older outer-job occurrences.

Independent result review verified that the before/after manifest matches all 43 current and frozen-commit source bytes and that the fixed Python, WORKER, source and build receipt hashes match. It independently rehashed all five raw artifacts and found no evidence that identifies PID `37656` by image or cause.

Raw evidence:

- `cp350-inert-trial-90b20f87eff24e91a64a9a0598f2160d.json`: SHA-256 `e697198d2b66f9bef7a0ccb37e92b629a93b943efc1fc8daa9a0de5daa865ff29`.
- `cp350-monitor-90b20f87eff24e91a64a9a0598f2160d.json`: SHA-256 `565f2edcd22d379f0abed7b1d399532140b85bef53f76ba58bb18d41238c766e`.
- `cp438-failure-c76e6bb6e175a59a298fa0ad2930e573d3df617b07b2b21bbbb0ac.json`: SHA-256 `fc845c0f140f83fdd947f413d506e4693e90c4a2e5ab12296f513f2a039ba4a5`, 715 bytes.
- `cp349-child-trace-45e68e2920295238070d895aa9982c34.jsonl`: SHA-256 `5fd4dc08a3a6a231997f273404da6d66308964735b28905d43f25d2120514944`.
- `cp429-runner-a4a69481672fce6ce00fbdd152e471222fe22e1ee468ae4aef3f3bd6ed5fd117.txt`: SHA-256 `4659172d62395abfe828e0ead6a315e159b0380cac4a276db4f76d3ff7342521`.

CP440 next retains CP329's already-captured creation-returned WORKER PID in a separately versioned failure-only receipt. That is the smallest comparison with the exact job-member PID; it adds no native query, process handle, clock or success-path behavior. Freeze/review it before any further occurrence. This remains an auxiliary harness investigation; public C++ converter candidate CP301 and 15 unresolved workstreams/eight gates are unchanged.
