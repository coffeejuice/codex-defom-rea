# CP141: disposable fingerprint benchmark

A four-worker content-hash prototype reduced warm fingerprint time by 20.5% on one disposable CP140 source copy, with exact path/content SHA-256 and file-count parity. Median sequential time was 0.682121s; parallel time was 0.542057s, a 1.258× microbenchmark speedup. The production build/test runners and all validation gates remain unchanged. No compiler, CMake, CTest, GUI or original executable was run.

## Scope and reproduction

The experiment is retained under `build/efficiency141-fingerprint/`: `benchmark.py`, `result.json`, and the disposable extracted `source/`. It uses `build/wave3-checkpoint140-verified/source.zip`, verifies the archive SHA before extraction, checks containment of every archive entry, then verifies the extracted hash/count against the frozen receipt. It never writes the active `source/` or candidate build.

Input archive SHA-256 is `c054e3c70033f74663eba6b197122100e9464a933a8331f413b7ac217d9f7a79`. Expected source identity is `9a9eb34f0a934ffd5fde3e02b5bb6b48152121ab91f608b215cdeb820b9bf981` over 3,502 files. The sequential baseline is the existing imported `admin/build_candidate.py::source_fingerprint`; its script SHA before/after is unchanged at `9e04249fd5778bc06192b4d0188d173cd8dce9f9260cde5d7dadb4d33fa54eec`.

The prototype retains the baseline's sorted `rglob` inventory, `is_file` admission, relative POSIX path encoding, NUL separators, per-file SHA-256 reduction order and count. A four-thread executor computes independent file-content digests; `executor.map` yields those digests in original order. A new executor is created for every snapshot, so measured time includes its setup/teardown. No digest cache or prior count is reused. Both functions are warmed once; the following four pairs alternate execution order. Every warm/measured result and a final sequential recheck match the frozen source hash/count exactly. Warm-up, extraction and final verification are excluded from the table.

| Pair | Order | Sequential | Four workers |
| --- | --- | ---: | ---: |
|1|Sequential, parallel|0.678104s|0.550952s|
|2|Parallel, sequential|0.690553s|0.596848s|
|3|Sequential, parallel|0.686138s|0.533161s|
|4|Parallel, sequential|0.667032s|0.532391s|
|Median| |0.682121s|0.542057s|

The median difference is 0.140065s per snapshot. Multiplying that observed difference by the existing eleven successful grouped-build snapshots gives a **projection** of 1.541s, not a measured production build saving. CP137–139 production fingerprints were roughly 0.5s per occurrence, faster than this copy's sequential baseline; path, host activity and cache state differ. The frozen-copy result therefore does not establish that the proposed implementation beats production snapshots, and does not support 10× build, test, checkpoint or token savings. Tokens are unavailable.

## Decision and limits

The measured result justifies keeping this prototype for one later controlled production-path comparison at a safe boundary. It does not justify deploying it now. Four warm pairs on one Windows host do not establish cold-cache benefit, stability during concurrent I/O, error-path behavior or an atomic source snapshot. The worker pool may read ahead after a file error, whereas the baseline sequential loop stops at its first failure; integration must preserve failure propagation and never turn partial work into an accepted fingerprint. No such error/lifetime test was claimed here.

Any future adoption must retain all eleven existing fingerprint occurrences and hash/count checks, source/cache drift retry decisions, artifact/test-executable binding, full build/regression, fixture integrity and archive read-back validation. Test empty/nested files, addition/deletion/content drift, provider failures and unchanged path admission before modifying the production runner. Broad build dependency fanout remains the larger variable cost identified in CP140; this small hashing result does not replace that qualification.

Benchmark script SHA-256: `64f94eb5837f6a7a2ab635cec469d839a76bc063d0bdb0d1604fa603f79e82fc`.

Raw result SHA-256: `4438a5ba653bd2414150f17d02ae0d2bbc2136b5e3080923b1ec41dfc01813e9`.
