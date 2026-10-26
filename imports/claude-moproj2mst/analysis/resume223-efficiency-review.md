# CP219–222 efficiency review

## Scope and limits

Receipts reviewed: CP219 revised focused build; CP220–222 full build and complete converter-suite timing receipts. The reviewed full suites report zero exit status. These are task-specific incremental builds with different source edits and fan-out; they are not clean-build benchmarks. Token and active effort telemetry are unavailable. Wall-clock observations below do not estimate model tokens or labor.

## Measured costs

| Checkpoint | Configure | Build command | CTest | Full launcher | Registered tests |
|---|---:|---:|---:|---:|---:|
| CP219 focused revised target | 1.703 s | 2.375 s | — | — | — |
| CP220 | 1.672 s | 27.578 s | 17.422 s | 34.859 s | 366 |
| CP221 | 1.860 s | 167.187 s | 17.875 s | 35.906 s | 366 |
| CP222 | 2.062 s | 114.719 s | 18.391 s | 35.485 s | 367 |

CP220–222 CTest exit codes are zero. Launcher phases show fixture SHA-256 snapshots of 12.031 s, 12.656 s, and 11.688 s respectively; before/after source and binary execution bindings total 4.078 s, 4.125 s, and 4.140 s. Thus integrity accounting consumes roughly 33–35% of the launcher, and remains required evidence. Suite totals stayed in a narrow 34.859–35.906 s band as the suite grew by one test.

The large variable cost is compilation: CP221 build command 167.187 s and CP222 114.719 s, compared with CP220's 27.578 s. This is approximately 749 and 514 times the CP219 focused-target build respectively, but target scope differs. These measurements signal substantial dependency fan-out, not a speed regression attributable to a particular edit. The focused CP219 receipt builds only `m2m_s_b98_negative_id_native_blobs`; no full suite time is available for it.

## Safe next experiment

On the next naturally occurring long full build, retain Ninja's explain output and count scheduled compile/link edges by target, then compare the identified broad dependency with the CP221/222 build logs. If one shared header is responsible, make one narrowly scoped internal dependency-boundary change in a candidate, and compare an equivalent incremental edit on the same machine/build tree. Preserve source and executable bindings, uncached fixture hashes, all 367+ registered tests and historical diagnostics. Treat the experiment as a proposal until a matched build measurement shows improvement; do not trade away any freeze or validation gate.

## Evidence

`analysis/resume218-efficiency-review.md`; `analysis/cp219-native-blob-focused-revised-build.json`; `analysis/cp220-conditional-directory-full-build.json`, `cp220-conditional-directory-full-tests.json`; `analysis/cp221-task-attempt-full-build.json`, `cp221-task-attempt-full-tests.json`; `analysis/cp222-step-identity-full-build.json`, `cp222-step-identity-full-tests.json`; each full test receipt's `timing.json`.
