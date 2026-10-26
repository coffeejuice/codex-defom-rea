# CP119–123 validation runtime review

No measured, fidelity-preserving change is ready to deliver a 10× validation speedup. Recent full launcher runs take 29.8–32.2 s, and CTest takes 13.4–14.0 s. A 10× reduction would require launcher time below roughly 3 s or CTest below roughly 1.4 s. The existing phase receipts and worker benchmark do not support either target without a new, evidence-backed change to dominant work.

| Checkpoint | Registered / clean passes | CTest | Full launcher | Fixture SHA passes | Source/binary bindings |
|---|---:|---:|---:|---:|---:|
| CP119 | 307 / 300 | 13.406 s | 30.031 s | 11.921 s | 3.673 s |
| CP120 | 307 / 300 | 13.485 s | 29.782 s | 11.593 s | 3.656 s |
| CP121 | 308 / 301 | 13.953 s | 32.219 s | 12.672 s | 4.547 s |
| CP122 | 310 / 303 | 13.656 s | 30.907 s | 12.063 s | 4.141 s |
| CP123 hash-only benchmark | same CP122 fixture manifest | not run | not run | see worker trials | CP122-bound inputs |

CP119–122 all report six tests with logged gaps, one with a logged difference, zero failures, skips, or incomplete output, and zero fixture changes. They hash all 16,728 fixture files (1,867,669,827 bytes) before and after CTest, use four SHA-256 workers with cache disabled, and preserve stable source/binary bindings. Token usage is unavailable.

The launcher cost floor is substantial: the fixture SHA passes alone use 11.6–12.7 s and source/binary bindings another 3.7–4.5 s. The remaining inventory/report work is about one second. These integrity checks must stay in place. CP123’s six alternating 4/8-worker pairs produced identical ordered manifests, but median wall times were 6.566 s at four workers and 6.553 s at eight (0.013 s, about 0.2%); eight workers used more median process CPU. That result does not justify changing the setting. The earlier 4 MiB read-chunk trial was slower than the current 1 MiB size.

The full folder sweep is the dominant individual CTest: `m2m.s.save_folder` took 11.59–11.91 s in CP119–122, while `save_io` took 4.86–5.17 s and `save_current` 4.38–4.55 s. The folder test already has CTest `COST=25` and runs under `-j4`. A prior `-j4`/`-j8` comparison showed no reliable CTest gain; its timing spread exceeded the job-count difference. The 98-case sweep, all other registered tests, failure/gap collection, and source/binary/fixture bindings are acceptance evidence, so removing cases, omitting snapshots, changing semantics, or weakening assertions is not a ready optimization.

## Next safe action

There is no ready safe cut with measured potential near 10×. At the next source-stable boundary, instrument per-case wall time inside the existing 98-case folder sweep without changing case order, copies, conversion calls, comparisons, diagnostics, or fixture snapshots. Preserve the full raw output and compare exact per-case results with the frozen receipt. This can reveal a specific avoidable stage for later review; it does not itself claim a speedup. Do not adopt a hash-worker change or CTest scheduling change on current evidence.
