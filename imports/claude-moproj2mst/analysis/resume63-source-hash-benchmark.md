# CP63 frozen-source SHA benchmark

## Scope and exactness

The live `source/` tree no longer equals CP62: its first read-only fingerprint was `fd77e63f43d47801966dd7fd26e6adf6b58feddf45fa403b736390a1a9d03169` across 3,258 files, while CP62 is `c7b042d0a2e0c016a997acc45452aa4af9fd16c357abbabb9dfd02308d097013` across 3,252 files. Those live-tree measurements were excluded from the comparison.

The benchmark used the frozen `build/wave3-checkpoint62-verified/source.zip`, whose receipt pins the CP62 fingerprint and file count. Its entries were extracted to an automatically removed temporary directory outside the checkout; a path traversal guard rejected any archive entry outside that directory. Three serial and three four-worker passes then measured full path enumeration plus content hashing using the same sorted relative-path, NUL separator, per-file SHA-256, and aggregate SHA-256 recipe as `admin/build_candidate.py`. Each of the six fingerprints matched CP62 exactly: `c7b042d0…d097013`, 3,252 files.

## Measurements

| Trial | Serial seconds | Four-worker seconds |
|---|---:|---:|
| 1 | 9.1522 | 0.8720 |
| 2 | 1.3317 | 0.9085 |
| 3 | 1.3081 | 0.8669 |
| Median | 1.3317 | 0.8720 |

The first serial pass is a clear outlier, consistent with cold filesystem data after extraction. Cache state could not be cleared safely; trials 2–3 are warm-cache observations. Their medians show about 0.46 s (34.5%) lower time for one source fingerprint with four workers. The initial read-only measurements of live `source/` were similarly cache-sensitive and are not used as frozen-source timing evidence.

## Recommendation

Do not change the test runner from this microbenchmark alone. Two source fingerprints per run would save about 0.92 s at the measured warm median, roughly 3.1% of CP62’s 29.375 s launcher duration before considering contention with the existing four-worker fixture hashing. That end-to-end saving is modest, and a cold comparison was unavailable. Keep the current behavior until a dedicated end-to-end runner trial measures source hashing concurrently with actual fixture-load conditions; require exact fingerprints and binary manifests and a repeatable material launcher reduction before changing implementation.

No tracked source, build output, fixture, runner, or test result was changed. The only write was this analysis note; the temporary extraction was removed automatically.
