# CP130 two-row P07 Oracle-A comparison

## Result

The frozen CP129 headless P07 test passed both omitted Oracle-A cases against their retained MST outputs: **2 pass, 0 failures, 0 skipped**, process exit code 0. The first disposable run is preserved as a diagnostic: it produced 0/0/2 because the copied sample names did not match the basenames named by `cases.tsv`. The corrected rerun added disposable aliases under each case directory, without changing retained samples or source.

## Frozen inputs and binding

- CP129 source tree receipt: `analysis/resume129-final-build.json`; source tree SHA-256 `eb54421d31c4fb42e23c1339459ce6b7cfb706ebd2967f870da7a0b5157b3490` (3,471 files). Build receipt records matching before/after fingerprints.
- CP129 P07 executable: `build/candidate/bin/m2m_p07_corpus.exe`, 3,744,768 bytes, SHA-256 `a73da05c23da83b71792a4e6f53eb9bf834ff059d905fdb7de4d89570c701753` (verified immediately before use).
- Retained sample pairs, verified before/after the comparison:
  - Case `29_2026_04_INC718_diam_450_Option_1_v35`: project SHA-256 `e697ab046770302abdc4b42d6adaf7edea6bef2b9f6b365131ce22ae14c17060` (2,245,588 bytes); MST SHA-256 `169643743e58c0d2118b786dac1e78c3fafda7e7eecc82a8e9518973ab8d2418` (700,618 bytes).
  - Case `30_2026_06_Inc718_diam_420_v41`: project SHA-256 `7653453156f0958afdc0042c9b134bdf57c06a9426b5afd74a56b0ddf92df290` (2,245,531 bytes); MST SHA-256 `5250a52ff18292026b27d3703a8d26ebbaf736a357bae93e6400c607dd2f03b0` (687,442 bytes).
- Disposable selection TSV: `build/test-runs/cp130-p07-two-row/selection/docs/re/moproj2mst/runner/cases.tsv`, SHA-256 `2efc7f7b55bdb8a000451070dcf30c84a099b9a510a202f2111d5b60a3516931` (1,025 bytes). It contains the retained header and exactly these two Oracle-A rows.

## Execution evidence

The test implementation (`source/apps/moproj2mst/tests/p07/test_corpus.cpp`) looks for the TSV's `moproj`/`mst` basename under `<M2M_CORPUS>/<case_id>/`. The retained sample filenames differ, so the corrected disposable view includes byte-for-byte copied files under both the retained names and TSV-requested aliases. Environment for the successful command was:

```text
DEF_PORT_REPO=<workspace>\build\test-runs\cp130-p07-two-row\selection
M2M_CORPUS=<workspace>\build\test-runs\cp130-p07-two-row\corpus
<workspace>\build\candidate\bin\m2m_p07_corpus.exe
```

Raw stdout is preserved in `build/test-runs/cp130-p07-two-row/output-rerun.log` (SHA-256 `59d6ac188b986a913edf892f9941e32730160b87863482152c1b55cb5db949e9`):

```text
p07 corpus: 2 pass, 0 failures, 0 skipped
```

The initial diagnostic output is retained separately at `build/test-runs/cp130-p07-two-row/output.log` (SHA-256 `cbcfcfdd7cf9f548ef294b4fa9b4ef6565619d107a711ffa343203d12b089279`): both rows skipped because the TSV basenames were absent. This was a disposable-view naming issue, not a candidate conversion failure.

## Qualification

This is a two-case headless candidate comparison against retained Oracle-A MST bytes. P07 emits and checks the header/main prefix and trailer; it does not emit operation bodies. It narrows the two missing-corpus skips from the broader CP129 P07 run. It does not establish full MST parity, fresh-original runtime acceptance, or completion of the overall recovery objective.
