# Resume 48 public ABI success-pair feasibility

## Finding

I did not find an existing pair of disposable projects that is already qualified for **successful full Save through the public C ABI**, with per-project fresh original expectations. Existing success evidence is either internal (`m2m::convert` or `convertToBytes`), limited to MST-only/earlier binaries, or a candidate CLI run. The only located public ABI test on a real project (`tests/g0/test_abi.cpp::corpus_project_result`) explicitly accepts either `M2M_OK` or `M2M_E_UNSUPPORTED`, so it cannot provide a success baseline.

Do not represent the requested ABI repeated-success case as an executable acceptance test yet. It needs a source/build-frozen success fixture pair and public DLL binding in a test run owned by the coordinator.

## Candidate fixtures and their limits

| Candidate evidence | What it establishes | Why it is not a qualified success pair |
|---|---|---|
| `analysis/reference-harness-status.json`: `build/reference-harness/billet-training-003` (06_BILLET) | Prepared disposable training case, all three referenced XML paths present under the bounded preflight, candidate artifacts bound to the recorded source/build receipt | No runtime was executed on 003. The later 004 original run crashed immediately after Open and candidate raw comparison was unequal. It has no successful original full lifecycle expectation. |
| `analysis/baseline-checks.json`: 11 historical projects including 01_COG_P1_B1 and 08_HEAT1 | Earlier release smoke and tier2 MST-only results; scratch input hashes were checked | Explicitly not full Save, not fresh original runs, and the runs are not bound as a current successful DLL ABI pair. |
| `tests/s/test_save_folder.cpp` | Full Save internal converter sweep against retained folder references, with diagnostic behavior and historical normalizations | Calls `m2m::convert`, not the exported ABI; several cases are diagnostic or historically qualified, and this is not fresh original state. |
| `tests/mdt/test_db5_keepmesh.cpp` and `tests/mdt/test_db_load.cpp` | Successful bounded internal Save/DB conversion paths for copied fixtures | Internal API tests do not validate `m2m_convert_w`/`m2m_convert_utf8`; their assertions do not create a second independent original expectation. |

The harness status lists `billet-training-003` as the current valid prepared case, but also states `runtime readiness not established`. The only later runtime example is `billet-training-004`, whose original crashed before Save observation. Those facts prevent treating the prepared case or an earlier CLI result as a success-pair oracle.

## Design once a qualified pair is available

Select two different, held-out fixture projects A and B only after the original-first harness has a successful one-project Open → Save → Close reference for each, including default dialog answers and raw output/file manifests. Make two disposable writable copies under test scratch; do not run against evidence or corpus originals. Build the DLL and G0 test executable from the same frozen source receipt.

The G0 test should make these public calls serially in one process:

1. `m2m_convert_w(A, outputA, M2M_FLAG_WRITE_SAVE, ...)`
2. `m2m_convert_w(B, outputB, M2M_FLAG_WRITE_SAVE, ...)`

For each call independently, require the recorded success status, expected message/warnings, expected MST and full Save file manifest/bytes, no write outside the project/test scratch boundary, and a closed/usable output handle state. Compare A against A's fresh original receipt and B against B's fresh original receipt. Preserve raw bytes and any source-justified normalization separately. Add reversed order or A,B,A as an extra history stimulus only if useful; every call is judged against its own independent single-project expectation. Do not derive an expected reset value from A or B's prior state.

To check contamination, include at least one discriminating observable per input (different project names, units, or already source-qualified per-project outputs/settings) and require each output/status to match its own fresh receipt despite execution order. Check output creation and input immutability/expected Save mutations per the recorded lifecycle contract. Do not assert unobservable internal reset or original process-global reset semantics.

## Next executable prerequisite

First complete candidate47 integration/freeze and the remaining source prerequisites that affect Open, Save, Close and the selected fixtures. Then obtain successful fresh original-first receipts for two supported training cases; the current case003 preparation may be reused only after revalidating its freeze and recording a successful full lifecycle. Bind each to an explicit immutable input copy and exact candidate source/build. After those receipts exist, add the public ABI pair test and have the coordinator run focused plus required integrated regression. Until then, an ABI test that insists on success risks cementing an unsupported candidate result as a contract.

## Evidence inspected

- `analysis/reference-harness-status.json` and its current/failed case qualification.
- `analysis/baseline-checks.json`, `analysis/baseline-audit.md`.
- `source/apps/moproj2mst/tests/g0/test_abi.cpp`.
- `source/apps/moproj2mst/tests/s/test_save_folder.cpp` and bounded internal Save tests.
- `analysis/closure-audit.json` C04/C05 validation requirements.

No source, build, app, or REA data was changed. No runtime or tests were run.
