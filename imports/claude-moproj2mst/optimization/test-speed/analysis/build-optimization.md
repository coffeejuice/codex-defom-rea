# Isolated converter build optimization

The cold build is dominated by compilation, so retaining the existing Ninja tree and
building only changed test products is the first useful optimization. This work
changes only the experiment build driver; compiler flags, production source,
fixture contents and validation requirements remain unchanged. Recovery remains
paused.

## Measured baseline

`profile-v1-retry-build.json` records a successful Release build using four jobs:
2.883 seconds for configuration and 139.546 seconds for the build. This is a single
observation, not a stable benchmark distribution. The first attempt in
`profile-v1-build.json` is separate and must not be combined with the retry.

The existing `.ninja_log` contains 1,053 unique command executions (deduplicating
multiple outputs by start, end and command hash):

| Command group | Count | Summed command seconds |
| --- | ---: | ---: |
| Converter test/sample compilation | 192 | 242.594 |
| Converter core compilation | 149 | 102.425 |
| Remaining compilation | 534 | 169.940 |
| Link and generation | 178 | 42.450 |

These are overlapping process durations, not additive wall-clock phases or CPU
times. Their total is 557.409 seconds. The observed aggregate concurrency is
approximately four, consistent with the four-job build setting; this does not
establish that additional jobs will improve throughput. The remaining compilation
group includes shared-library code and entry points, so it is not wholly avoidable.
The default build also produces runtime tests and database utilities unrelated to
an individual converter test.

## Implemented driver controls

`build_isolated.py` retains full configure/full build as its default. It now accepts
repeatable `--target` arguments for known converter libraries, CLI, samples and C++
test executables. Source-derived names follow the existing CMake convention.
Corpus-executing custom targets such as `m2m_tier3` are rejected, along with unknown
targets. Ninja supplies the selected target's normal dependency closure.

`--skip-configure` reuses an existing build only after checking that its cache is
Ninja/Release and refers to the experiment source and imported takeover root. It
does not disable Ninja's normal CMake regeneration or configured glob checks.
Receipts record selected targets and whether explicit configuration was run.

For example, from the converter root:

```powershell
python optimization/test-speed/build_isolated.py --label focused-save-folder --skip-configure --target m2m_s_save_folder
```

This prepares one test; it does not execute that test or establish a full-suite
result. Tests invoking the CLI or inspecting a DLL may need those products named
explicitly because runtime use is not always a CMake build dependency. For a full
suite after core changes, use the default build or explicitly build every required
product. A targeted build must not silently substitute for the required regression.

## Verification and next measurements

Verified the current cache identity and mocked the driver subprocess to confirm
that target-only reuse produces exactly one build command and a correct receipt.
Confirmed an existing native test is allowed and the corpus execution target is
rejected. No compiler, executable test, original application or full suite was run
for this lane; no incremental or selective-build speedup is claimed yet.

Measure separately: unchanged full build with/without explicit configure; a
single test-only edit; a core edit with required relinking; and a genuinely cold
build. Run these serially with the same compiler, source snapshot and job count.
Then compare four jobs with higher concurrency on a separate disposable build
tree, recording memory pressure and elapsed time. Do not erase the existing
candidate or mutate evidence just to obtain a cold measurement. Unity builds,
precompiled headers and altered fidelity/optimization flags are excluded from
this initial experiment because they change compilation conditions and need
their own correctness evaluation.
