# Ordered-conversion folder pipeline

The folder sweep can overlap filesystem work while retaining all converter calls in their existing serial process and original order. This avoids the demonstrated semantic failure of distributing conversions among fresh processes. It is a candidate architecture, not an accepted replacement: output parity must still establish that changed allocation and filesystem timing do not affect conversion.

No source was edited and no converter or suite was run for this design. Calculations read the existing 98-row `profile-v1-ctest.log`.

## Subsequent implementation candidate

At root's later explicit instruction, the isolated folder test now implements the smallest higher-value increment. The design calculations above remain read-only historical models; this implementation has not been built or run by this worker.

- `M2M_FOLDER_PIPELINE=1` enables the pipeline; absent/0 preserves the serial path. Other values fail after the existing sweep gates. Combining the pipeline with `M2M_FOLDER_WORKERS>1` fails as a configuration error. Existing conversion sharding remains separately available as an unaccepted experiment.
- Discovery freezes the same case/event stream and selected `.moproj` file as the sharding candidate. Copying stays serial on the coordinator. The single `m2m::convert` call site stays in `runCase`, called only by the coordinator in original ordinal order in pipeline mode. No engine settings, global resets or masks were added.
- The former post-conversion block is extracted into `compareCase` without changing its comparison/diagnostic/cleanup decisions. The serial path and experimental conversion workers also use that extracted function.
- One persistent private `--folder-post-worker <manifest>` process reads each completed tree and performs comparison/cleanup. Its TEMP/TMP/TMPDIR are distinct owned directories. The coordinator's engine temp environment and working directory stay inherited. A strict two-case pending bound limits retained trees; the coordinator waits for the oldest result before admitting a third case.
- The coordinator atomically publishes per-ordinal ready files after conversion. The helper atomically publishes framed results containing exact manifest identity, ordinal, operation kind, tally and rendered comparison bytes. It acknowledges SKIP/refused cases without performing comparison or cleanup, because those branches retain existing coordinator behavior. Result identity, framing, tally and terminal-operation checks run before aggregation.
- Both coordinator streams are captured through stable duplicated CRT descriptors across all conversions and restored before replay. Converter stdout is joined with its comparison output at the original event position; notes/summaries remain ordered. Any unframed coordinator/helper stderr is retained privately and fails the experiment. Capture and transport read errors fail closed.
- Root's independent review caught the MSVC exclusive-handle capture issue before acceptance. The helper now redirects flushed stdout to NUL and updates the Win32 standard handle before reading each released capture. Coordinator capture files use explicit `_SH_DENYNO` sharing so flushed slices can be read without changing the engine-visible descriptor between conversions.
- A subsequent review found unchecked standard-handle updates. Pipeline helper stderr/capture/NUL binding and coordinator setup/restore now check `SetStdHandle` and reject invalid CRT OS handles. Constructor binding failure attempts checked rollback and fails the harness; restoration reports failure unless both descriptor and Win32 handle restoration succeed, retaining its saved descriptor for a destructor retry when binding fails. Default serial behavior and the separate experimental sharding path were not changed by this correction.
- Missing results, unsuccessful helper exit, wait timeout, capture failure and malformed IPC fail the harness. The existing owned-child guard terminates/reaps the helper during unwinding; unsuccessful IPC remains in scratch. Overall root cleanup occurs only after successful collection. Ordinary adjusted differences/refusals remain report data.
- The opt-in B:107 raw capture runs in the coordinator after conversion and before helper handoff, identifies its worker as `pipeline-coordinator`, and remains outside normal sweep cleanup. With the capture variable absent, no targeted capture is created.

Exact residual risks remain: predecessor conversion order is preserved, but comparator allocations move out of the engine process; prior work leaves can survive into the next conversion; filesystem contention changes timing; discovery occurs before execution rather than interleaved; and standard-stream descriptor capture may expose cached native-handle behavior. Existing full raw inventory/decision-stage/vector receipts and same-path replay are still missing. The candidate does not claim raw parity or measured speedup, and B:107 equality alone would not establish either. Root must compile, compare all ordered adjusted diagnostics and summaries, inspect B:107 raw captures, preserve fixture hashes and exercise helper failure/skip/refusal probes before considering broader validation.

## Subsequent raw-receipt increment

Root reported a targeted pipeline build/run taking 11.73 seconds versus 14.41 seconds serial, with all 151 ordered diagnostic lines equal. Those measurements precede the new raw-receipt instrumentation and do not establish raw parity or its overhead.

At root's further instruction, `M2M_FOLDER_RAW_RECEIPT=1` now enables atomic per-case JSON evidence in **both serial and pipeline modes**. Presence enables this gate, like the existing profiling gate. The separate unaccepted conversion-sharding mode rejects raw receipts explicitly; it must not silently write a partial run. Root must rebuild the current source before using this increment; this worker did not build, test or mutate fixtures.

Receipts survive cleanup under a unique directory beneath the configured test scratch:

`m2m_folder_raw_<coordinator PID>_<serial|pipeline>_<steady timestamp>/`

Each eligible ordinal gets `<ordinal>.json` before its normal cleanup. Copy skips and converter refusals get explicit terminal receipts too; silent preflight-ineligible entries consume no ordinal, as before. Each receipt includes case/ordinal/mode, writer PID/worker, expected/work paths, whether conversion occurred, exact converter code and **full message even on success**, work existence and a sorted inventory of every generated regular file. Inventory paths retain case and use forward slashes; sizes and SHA-256 describe raw bytes without relocation or masking. Ignored PNG/oracleA/view-state files are included in this raw inventory even though comparator decisions exclude them.

Successful receipts also retain complete adjusted differ/missing/extra vectors, differ before the older-window exception, the older-window and `.moproj` flags, banner-only count, and every comparator key's decision: `raw_equal`, `relocated_equal`, `masked_equal`, `banner_only`, `different`, `missing`, or `extra`. No first-eight truncation applies to the receipt. Human diagnostics keep their existing truncation and text.

The coordinator records the expected ordered ordinal/name/work/expected identities as cases begin. After collecting all pipeline results, it requires the raw directory to contain exactly those case JSON files, with no missing, duplicate, unexpected or unfinished files. Only then does it atomically publish `complete.json`, binding the ordered cases to their receipt SHA-256 values and the executing binary's path/SHA-256. Failures retain partial evidence without a completion marker and fail the test. Consumers must require a valid completion manifest and verify each referenced hash and JSON identity before comparing runs. A receipt directory alone is not a complete run.

SHA-256 uses the system BCrypt API loaded explicitly from System32, avoiding CMake/link changes. Files are streamed, size/read completeness is checked, and digest-provider errors fail the harness. The DLL and hashing work are used only when evidence is requested. The pipeline helper receives the same contained receipt root in post-job schema2. Post-ready schema2 additionally carries exact successful converter status/message; this corrects the initial review finding that successful warnings could otherwise be lost.

This closes the generated-inventory/full-vector/stage evidence gap, but does **not** implement same-path replay, bind source/fixture manifests automatically, retain every generated file's bytes, or prove equivalence. `complete.json` binds the executable; root's source/build and before/after fixture receipts remain necessary. Different work roots can still change raw bytes, and those differences must remain visible alongside their path qualification. Raw hashing allocates and reads between serial conversions but runs in the helper for pipeline comparisons, so instrumentation itself changes timing/heap history and must be enabled identically for paired evidence runs. Its overhead belongs in measurements if retained in the validation path. The historical timing model and the earlier uninstrumented 151-line match must not be presented as measurements of this raw-enabled candidate.

Root's subsequent 98-case receipt comparison localized remaining fields to a success message containing run paths for B:131_B_COG_NEWOP and one generated Gear_blank.MST hash for B:98_B_DS3F (same 2821-byte size; the other 29 generated files equal). A new independent opt-in `M2M_FOLDER_CAPTURE_98=1` captures B:98's exact generated and expected top-level Gear_blank.MST bytes before comparison/cleanup in both serial and pipeline modes. It writes `generated-Gear_blank.MST`, `expected-Gear_blank.MST` and a `M2M_FOLDER_CAPTURE_98_1` TSV identity receipt under the existing unique owned `m2m_folder_capture_<PID>_<timestamp>` scratch pattern. The receipt includes original work/expected/input paths, case, ordinal, worker and PID. This capture is intended to distinguish PID/work-root relocation from a behavioral difference; no such conclusion is assigned before root inspects the captured bytes. The capture gate does not change default output or cleanup.

## Subsequent one-case copy-prefetch increment

Root next authorized an independent opt-in copy-prefetch experiment. The isolated source now accepts `M2M_FOLDER_PREFETCH=1` only with `M2M_FOLDER_PIPELINE=1`; absent/0 is off, other values and prefetch without pipeline fail after the existing sweep gates. No build or tests were performed by this worker.

The former copy block is extracted as `copyCase` and reused by the ordinary serial path and a new persistent `--folder-copy-worker <manifest>` process. It retains remove/copy flags and error-code sequencing, selected top-level project filename check, profiling census and timing. The helper does filesystem work only: it never calls conversion or engine readers. Work paths remain the coordinator's existing global ordinal leaves and are checked against owned scratch before helper mutation.

The coordinator primes the first copy and requests only the current ordinal's immediate successor before running the current case. No request beyond that single successor exists while the current case converts. The helper executes requests in frozen ordinal order. It has distinct TEMP/TMP/TMPDIR directories and private checked stdout/stderr. Framed atomic requests/results bind exact manifest bytes, ordinal, copy status/message and profile fields. Missing, malformed or inconsistent results, process exit, timeout and unframed diagnostics fail the harness. Both helper processes are terminated/reaped on coordinator exceptions and must finish successfully before ordered replay.

The coordinator consumes a copy result at the case's original turn, after its case tally/identity registration, and does not repeat or remove the prepared leaf. An ordinary copy error becomes the same SKIP text at that event position and invokes no converter. Failed partial leaves are not cleaned by the copy helper or post helper; they retain the baseline lifetime until overall sweep-root cleanup. A future copy error is retained privately and is not reported as an earlier case's error. Refusals still come only from coordinator conversions, use existing cleanup, and are acknowledged without comparison by the post worker. The existing two-case bound for pending post-processing remains; the copy experiment permits at most one additional future leaf.

Raw case and completion receipts now record `prefetch: true|false` alongside mode. Success/refusal messages, comparison vectors, inventories and raw hashes retain their existing meanings. The copy helper does not write scientific receipt files: the coordinator records skip/refusal evidence and the post worker records compared output. Profile `copy_us` measures the helper's copy work; coordinator waiting/IPC overhead remains part of external wall time, so summed profile phases do not replace end-to-end timing.

Failure **reporting and attribution** remain ordinal, but physical filesystem timing necessarily changes: the next tree may exist, consume disk space or fail its copy before the current conversion finishes. Storage exhaustion/handle pressure and any engine access to a future sibling tree could therefore change converter failure behavior. Moving copy calls to another process also changes coordinator heap, CRT error and thread last-error history. No code resets those states or claims they are irrelevant. If literal physical copy/conversion failure ordering under those resource conditions is required, prefetch is incompatible with it and must remain off; buffering result messages cannot restore that ordering.

Before accepting this gate, root should compare complete raw receipts and ordered diagnostics against pipeline-only runs, with the same frozen binary/fixtures and path qualifications. Synthetic failed-copy/partial-leaf, refusal, helper crash and incomplete-result probes must verify the event/cleanup contract in owned scratch. Earlier pipeline timings do not measure prefetch speed, and ordinary fixture equivalence does not prove resource-exhaustion fidelity. Default serial and pipeline-without-prefetch remain available while these limits are evaluated.

## Why conversion sharding is rejected

Root's targeted raw B:107_B_HTFF capture establishes numeric changes in SimCtrl.KEY, rather than a logging-only or path-relocation difference. Root reported serial generated bytes matching expected SHA-256 beginning `6f702539`, versus the two-worker output beginning `b6162ff`. Changed values include approximately -17.7778 to 0, 1315.56 to 2400 and 454.44 to 850. These are consistent with temperature-unit conversion differences; the differing values alone do not identify the responsible field/global or prove its source route.

Two and eight workers changed the case; four and ten preserved adjusted rows. Consequently, choosing an apparently passing worker count is insufficient. The process-sharding optimization remains unaccepted. The converter mutex and known engine globals already require one conversion at a time within a process; retaining that process history is the pipeline's central invariant.

## Stage ownership and ordering

Discover the existing event stream once without sorting: corpus cases and summary/notice markers, three fixed re-saves, B cases and NOTE positions, and final summary. Preserve preflight's last exactly `.moproj` top-level entry, missing-expected/input rules, and global eligible ordinal. Bind it to immutable fixture snapshots. Each case keeps `<scratch>/m2m_folder_sweep_<coordinator PID>/<ordinal>`.

| Stage | Owner | Permitted concurrency |
| --- | --- | --- |
| Discover/preflight | Coordinator | Before dispatch; retain the exact ordered manifest |
| Remove/create/copy case input | Filesystem helper | Ahead of conversion, only into its assigned disjoint case leaf |
| `m2m::convert` | Coordinator main thread | Exactly one call; ordinal order; never skips/reorders a successful copy |
| Enumerate/read/compare generated and expected files | Comparator helper | After that case's converter call returns; no later writes to that case tree |
| Per-case cleanup | Assigned filesystem/comparator owner | Only after its own comparison/refusal handling; lifecycle caveat below applies |
| Print and aggregate | Coordinator | Original event order, independent of completion order |

The conversion stage must retain the same initial current directory, TEMP/TMP/TMPDIR, codepage1252, empty output argument, flags and complete sequence of predecessor calls. Filesystem helpers must never invoke conversion, engine readers or initialization/reset APIs. Their private temp roots protect helper operations but must not replace the coordinator's converter temp roots.

Use persistent helper **processes** for the first experiment, rather than threads sharing the converter's CRT/heap/locale environment. Comparators need filesystem/string/regex logic, not a fresh converter session. A process boundary avoids simultaneous comparator allocations in the engine's heap and future accidental engine entry. It still removes the comparator allocations that previously occurred between conversions in the coordinator, so it cannot by itself prove equivalence.

## Case state and bounded flow

Keep an explicit state machine per global ordinal: discovered → copying → copy-ready/copy-skip → converting → refused/converted → comparing → complete. A result contains raw converter status/message, all tally deltas, complete vectors and decision stages, exact case stdout/stderr, optional profile times and raw artifact inventory. Coordinator summaries derive from validated results, never helper summary strings.

Start with one copy slot and one post-processing slot and bound all copied, converting and awaiting-comparison trees together. A two-to-four-case in-flight window is sufficient for the first trial. When full, wait for the oldest outstanding post result and free its tree before preparing another. Bound by bytes as well if large cases demonstrate a disk-space problem; do not pre-copy all 98 projects merely to simplify orchestration.

The next conversion waits for **its own ordinal's** copy-ready result, even if later copies finish first. A copy failure retains one counted SKIP and consumes no conversion, matching baseline. Conversion refusal retains the exact status and 200-character rendered message prefix, performs the original cleanup and needs no comparator. An infrastructure exception is a harness failure, rather than a synthetic SKIP/REFUSED.

Freeze the expected paths and case names in manifests. Identity/count/ordinal/schema checks and strict bounded IPC reads should carry over from the rejected sharder. A bounded input pipe plus per-ordinal result files and completion notification is sufficient for persistent helpers. Avoid a result pipe that workers can fill while the coordinator blocks writing more requests. Startup happens once per helper; spawning 98 comparison executables could consume the intended savings.

Every helper must be reaped before coordinator root cleanup. On failure, stop new jobs, retain uncertain outcomes and diagnostic artifacts, terminate/reap only owned helpers and fail the test. No partial result may produce a successful sweep summary. The coordinator owns overall scratch-root removal; workers own only assigned leaves.

## Comparator extraction and diagnostic replay

Extract the existing post-conversion comparator as one pure case operation. Preserve exclusion case sensitivity, lowercased map keys, raw equality before expected-only path relocation, both slash forms, whole-component checks, cycle/group Expanded masking, DOE-only view masking, banner-only classification, the three older-window exceptions and all diagnostic truncation/text. Do not redesign these while adding concurrency.

Distinguish `resultRoot`, the directory being read, from `relocationTarget`, the original converter work path. They are the same in the initial pipeline. This distinction allows a later immutable snapshot experiment without substituting snapshot paths into the expected bytes.

Store complete comparator results and per-file raw hashes before cleanup. Render existing human rows from those results at their original event positions; full vectors/inventories stay in a separate receipt. Neither helper completion timing nor engine stdout may reorder notes and summaries. Capture converter stdout/stderr around each serial call without parallel writes to its descriptors, and collect comparator diagnostics privately. Preserve Windows newline handling and fail on unreadable captures. A successful helper that emits unframed stderr must fail the initial harness, as in the current experiment.

## Filesystem-lifetime caveat

The baseline compares and removes caseN before converting caseN+1. A pipeline that retains caseN for asynchronous comparison changes that lifetime. If a stale engine path points into caseN, the later conversion could now read a file that previously had been deleted. This is distinct from converter-call ordering and must be tested, not dismissed because case leaves differ.

Three possible treatments have different costs:

1. Begin with copy-prefetch only. Keep conversion, comparison and cleanup serial in the coordinator. This preserves the post-conversion lifetime boundary and limits the first concurrency change to preparing the next disjoint input tree.
2. Trial asynchronous comparison/cleanup under strict raw parity gates. This has greater potential value, but remains experimental until full generated inventories, B:107 capture and ordered results match repeated serial trials.
3. If longer-lived old paths cause a difference, synchronously freeze an immutable output snapshot and remove the original work leaf before the next conversion, then compare the snapshot using the **original** relocation target. Snapshot cost must be measured. Do not hardlink mutable fixtures, adopt a new normalization or retain failed raw parity merely to preserve speed. An immutable-output hardlink scheme would require its own filesystem/handle validation and is not the smallest increment.

Even copy-prefetch makes future case paths exist earlier. It should not alter operations confined to the current case, but that confinement also belongs in validation. Removing allocations from the coordinator can change heap history even without concurrency. These caveats mean “safe” is a design goal with explicit gates, not a completed finding.

## Profile lower bounds

The one historical instrumented profile contains 98 cases and 19.450029 seconds of summed phase work:

| Phase | Sum, seconds |
| --- | ---: |
| Copy, including profiling census | 3.464911 |
| Conversion, necessarily serial | 8.456196 |
| Comparison | 6.370096 |
| Cleanup | 1.158826 |

Serial conversion alone bounds ideal pipeline runtime at 8.456196 seconds. Including the first input copy and final comparison/cleanup yields an optimistic dependency-chain bound of 8.685141 seconds, equivalent to at most approximately 2.24x over the historical summed case work. This excludes discovery, formatting, IPC/startup, receipts, output replay, filesystem contention and profiling changes.

A simple event simulation uses the measured phase durations unchanged, in manifest order, without resource contention or startup cost. Copy workers process ordered copy jobs; the one converter waits for each corresponding copy; post workers take available comparisons plus cleanup. With unbounded prefetch, it gives:

| Model | Modeled seconds | Ratio to 19.450029s phase sum |
| --- | ---: | ---: |
| Serial copy+convert; one async post worker | 12.111528 | 1.61x |
| Serial copy+convert; two or four post workers | 12.111528 | 1.61x |
| One copy helper + serial conversion + one post helper | 9.320841 | 2.09x |
| Two copy helpers + serial conversion + two post helpers | 8.685141 | 2.24x |
| Four/eight helpers per filesystem stage | 8.685141 | 2.24x |

The latter rows are optimistic unbounded-prefetch models, not recommendations to retain all case trees. A bounded window can increase time. More post workers do not improve the first model because serial copy+conversion already supplies results slowly enough. The copy-prefetch-only increment cannot remove the 15.985118 seconds of serial conversion+compare+cleanup work, giving a theoretical ceiling near 1.21x before startup/first-copy overhead.

Root's current unprofiled serial focused wall time is 14.41 seconds, while the profile's original CTest wall time was 19.50 seconds. These are different measurements and must not be mixed into a claimed speedup. Re-profile the same frozen binary if a current phase estimate is needed. The numeric tables are a capacity model, not forecasts of host wall time, full CTest speed or launcher entry-to-exit speed.

## Smallest implementation increments

The **smallest conservative increment** is one persistent copy helper with a one-case lookahead; leave `runCase` conversion, comparator and cleanup on the coordinator. Extract copy/preflight results without changing their error/count semantics. Keep it behind a new explicit pipeline experiment gate, with the current no-argument serial default untouched. This first step has limited upside but does not need to solve deferred comparator lifetime or ordered asynchronous diagnostics.

The **smallest higher-value increment** is serial copy+conversion on the coordinator plus one persistent comparator/cleanup process and an in-flight bound of two cases. It reuses frozen event manifests/ordered aggregation but replaces the conversion worker mode with a comparison-only mode. Extract `compareCase` without algorithm changes, preserve raw inventories and B:107 capture, and gate acceptance on the lifetime/heap concerns above. Do not begin by combining this with parallel copy, more worker counts, duration scheduling or engine-state changes.

After that increment passes raw same-path parity and synthetic copy/refusal/helper-failure tests, add a single copy-prefetch helper and benchmark the complete test, unchanged full CTest membership and launcher including fixture snapshots. Select helper counts only from repeated measured timings. Neither this pipeline nor folder-only work can establish a tenfold complete-launcher gain.
