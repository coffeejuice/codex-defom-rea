# CP135 headless P07 comparison using CP134 executable

## Result

The frozen CP134 `m2m_p07_corpus.exe` passed both omitted Oracle-A rows: **2 pass, 0 failures, 0 skipped**, process exit code 0. Raw stdout is preserved in `build/test-runs/cp135-p07-cp134-exe-01/raw_stdout.log` (SHA-256 `14e22f734970174df2b7fc8ac0f1e45aff7c9a1265213e94a6e4dca45dc5c6c2`):

```text
p07 corpus: 2 pass, 0 failures, 0 skipped
```

## Binary and source binding

- CP134 final build receipt: `analysis/resume134-final-build.json`, source tree SHA-256 `e52d0f8a59e46b3c251f71f1088233694e5554165e63cbf8fc420208926da911` (3,475 files).
- CP134 final test receipt: `analysis/resume134-final-tests.json`, 319 registered tests and candidate binary inventory.
- Executable: `build/candidate/bin/m2m_p07_corpus.exe`, 3,745,792 bytes, SHA-256 `70c9cf1518275ff4853c585b699073a3279402a1f39eed828fb7d0c21e3b9675`; matches the build and test inventories.

## Inputs and disposable view

Disposable run root: `build/test-runs/cp135-p07-cp134-exe-01`. Its selection TSV contains the original header and exactly cases `29_2026_04_INC718_diam_450_Option_1_v35` and `30_2026_06_Inc718_diam_420_v41`; TSV SHA-256 `7b3185401c03b068383be855ba2dadeb0e0056bd964c66177d63595391d714a2` (1,022 bytes). The four disposable project/MST files use the basenames requested by the TSV and match the retained samples exactly:

- Case 29 project: 2,245,588 bytes, SHA-256 `e697ab046770302abdc4b42d6adaf7edea6bef2b9f6b365131ce22ae14c17060`.
- Case 29 MST: 700,618 bytes, SHA-256 `169643743e58c0d2118b786dac1e78c3fafda7e7eecc82a8e9518973ab8d2418`.
- Case 30 project: 2,245,531 bytes, SHA-256 `7653453156f0958afdc0042c9b134bdf57c06a9426b5afd74a56b0ddf92df290`.
- Case 30 MST: 687,442 bytes, SHA-256 `5250a52ff18292026b27d3703a8d26ebbaf736a357bae93e6400c607dd2f03b0`.

Each copied file's hash and byte count were checked against its retained source. The retained files' hashes were also checked after the run and unchanged. Environment was set only for the child command: `DEF_PORT_REPO` pointed to the disposable selection root and `M2M_CORPUS` to its disposable corpus root.

## Qualification

This headless comparison narrows the current CP134 executable's P07 result for two retained Oracle-A inputs. The P07 test compares the emitted header/main prefix and trailer bytes; it does not emit operation bodies. It does not close any of the five historical `s.save_folder` DIFFs, establish fresh-original runtime behavior, or satisfy the full acceptance objective. No source, build, retained fixture, GUI, original process, or mutable REA state was changed or used.
