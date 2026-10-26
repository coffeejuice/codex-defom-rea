# Release snapshot hash optimization

The isolated `test_unchanged.cpp` now reads each file through 64 KiB blocks into an incremental Windows BCrypt SHA-256 state. It previously constructed a whole-file vector through `istreambuf_iterator<char>` and then called one-shot BCryptHash. This removes byte-by-byte vector growth and bounds input-buffer storage at 64 KiB per call. Digest encoding reserves its known 64-character length.

No fixture selection, copy, CLI invocation, directory traversal, metadata collection, snapshot comparison, allowed output exception, or original-application behavior was changed. Every before/after snapshot still hashes all file bytes; no size/mtime cache is used. The existing self-test changes `abc` to `abd` and restores the prior last-write time, so it continues to detect the same-size/same-time mutation through the digest.

## Baseline profile

`profile-v1-ctest.log` contains 22 `M2M_RELEASE_PROFILE` rows (11 tier-2 cases times two output modes). Their cumulative measured durations are:

| Category | Seconds |
| --- | ---: |
| Copy and output-directory creation | 0.677018 |
| CLI launch and wait | 0.923115 |
| Before snapshots | 0.597258 |
| After snapshots | 0.595863 |
| Comparison plus output-folder scan | 0.002612 |
| Hashing (subset of snapshots) | 0.484583 |
| File metadata (subset of snapshots) | 0.171332 |

Hashing accounts for 40.61% of snapshot time in this run. The CTest duration for `m2m.release.unchanged` is 3.10 seconds. Even eliminating all recorded hash time would save at most about 0.485 seconds from this test under comparable conditions; this is an upper bound, not a measured improvement. The concurrent `m2m.s.save_folder` test takes 19.50 seconds, so speeding release hashing alone does not establish a full-suite wall-time reduction.

## Validation and limits

Added independent fixed SHA-256 vectors for empty input, `abc`, exactly 65,536 `a` bytes, and 65,537 `a` bytes. These exercise empty-file finalization, a partial first block, exact-block EOF, and a partial second block. Python hashlib verified the embedded full-block and multiblock expected values while editing; the C++ checks await the coordinator's build and focused release-test run. The existing mutation self-check remains unchanged.

Read/open failures now return the existing `<hash failed>` marker instead of permitting a failed stream to appear as an empty or partial digest. As before, the marker is not a successful integrity proof; this optimization does not redesign snapshot error handling. BCrypt failures retain the existing marker. Incremental hashing also avoids the old one-shot ULONG length truncation for files above 4 GiB, although no such fixture was tested here.

No build, CTest invocation, original application, GUI control, fixture mutation, or full evidence-root hash pass was performed by this worker. No new elapsed-time speedup or token saving is claimed. The coordinator should build the isolated target, run `m2m.release.unchanged` with `M2M_RELEASE_PROFILE=1`, compare all 22 ordinary result rows and self-checks, then repeat comparable timings before reporting a performance gain.

## Diagnostic parity correction

The coordinator's 150-test run passed with streaming hashing but its strict log comparison detected an extra printed testkit case and changed case total from the standalone digest-vector registration. The four vector assertions now run as an unregistered helper called inside the existing `harness_detects_changes` case. This preserves their execution even without corpus environment settings and restores the original two registered case names/count. The helper emits no successful-case diagnostic of its own. No build or test was run by this worker after the correction; the coordinator must verify exact ordinary output parity on the rebuilt candidate.
