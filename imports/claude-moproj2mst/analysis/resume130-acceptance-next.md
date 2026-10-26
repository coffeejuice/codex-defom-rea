# CP130 acceptance next action after CP129 freeze

## CP129 acceptance state

`analysis/wave3-checkpoint129-validation.json` binds to `analysis/resume129-final-build.json` and `analysis/resume129-final-tests.json`. Build and test source fingerprints match at `eb54421d31c4fb42e23c1339459ce6b7cfb706ebd2967f870da7a0b5157b3490`. The full run reports 315 registered tests: 308 ordinary passes, six passed-with-logged-gaps, one passed-with-logged-differences, and zero incomplete, skipped, or failed tests. The validation remains `objective_complete: false`.

The historical folder sweep is still 93 SAME / 5 DIFF. The five rows remain A55, B120, B121, B122, and B98. Their current evidence does not support another meaningful headless raw-diff rerun: A55 lacks an original-process `MoInputMode` binding and callback-delivery witness; B120–B122 compare pre-action inputs against post-GUI-created DOE states; and B98's exact path-only qualification already exists. Do not label rerunning those same comparisons as new acceptance.

The continuation state still has all eight completion gates false with empty evidence arrays. CP129 test success verifies its bounded candidate increments, not end-to-end Open/Save/Close, fresh original comparison, or delivery. The current queue starts CP130 feature-map sentinel and private Editor300 meshing selection; no original-process access is needed for the independent action below.

## Recommended executable headless comparison: two omitted P07 Oracle-A pairs

The CP129 `m2m.p07.corpus` log records 45 passes, zero failures, and five skips. Three skips are explicit `A-excluded` scope decisions; two are retained Oracle-A cases missing from the corpus view. The two exact input/expected-output pairs are available under `evidence/gui_mo/s1/s1-01/samples`:

| Case | `.moproj` bytes / SHA-256 | `.MST` bytes / SHA-256 |
|---|---|---|
| `29_2026_04_INC718_diam_450_Option_1_v35` | 2,245,588 / `e697ab046770302abdc4b42d6adaf7edea6bef2b9f6b365131ce22ae14c17060` | 700,618 / `169643743e58c0d2118b786dac1e78c3fafda7e7eecc82a8e9518973ab8d2418` |
| `30_2026_06_Inc718_diam_420_v41` | 2,245,531 / `7653453156f0958afdc0042c9b134bdf57c06a9426b5afd74a56b0ddf92df290` | 687,442 / `5250a52ff18292026b27d3703a8d26ebbaf736a357bae93e6400c607dd2f03b0` |

The existing P07 test is already narrowly useful: for each Oracle-A row it reads the named project and MST from `<M2M_CORPUS>/<case_id>/`, calls `openProject`, emits the header and main block plus the trailer, and compares those bytes against the retained MST. It does not emit or compare operation bodies. It needs no original GUI or mutable application state.

**Next action after the CP129 binary/source is confirmed unchanged:** make only a disposable two-row corpus view and a disposable `DEF_PORT_REPO/docs/re/moproj2mst/runner/cases.tsv` containing the original header plus the exact Oracle-A rows 29 and 30. Copy each retained pair byte-for-byte into its named corpus subdirectory. Run the CP129-bound `m2m_p07_corpus.exe` with `DEF_PORT_REPO` set to that disposable selection root and `M2M_CORPUS` set to the two-case corpus view. Require the raw summary `2 pass, 0 failures, 0 skipped` and preserve each pair's input hashes plus executable hash. The retained CP129 executable identity is `build/candidate/bin/m2m_p07_corpus.exe`, SHA-256 `a73da05c23da83b71792a4e6f53eb9bf834ff059d905fdb7de4d89570c701753`, from the source-bound CP129 run above.

This is a two-case targeted candidate comparison against retained Oracle-A bytes. It narrows the current P07 corpus omission; it does not turn the three A-excluded cases into coverage, establish full MST body parity, resolve any of the five historical `s.save_folder` DIFF rows, or satisfy the fresh-original comparison completion gate. Keep its receipt separate from the full suite and historical sweep.

No source, build, fixture, GUI, original process, or mutable REA state was changed or run for this review.
