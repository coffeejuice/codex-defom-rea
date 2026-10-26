# Priority for the five current historical Save DIFFs

## Selection

Select **B:98_B_DS3F (Diestress)** for the next candidate-only focused regression. The latest `analysis/wave3-native-save-validation.json` says 93 SAME / 5 DIFF / 0 REFUSED and lists exactly: A:RS_55_Multiple_Operations_Example1, B:120_B_DOE, B:121_B_DOE_VAR, B:122_B_DOE_GEO, and B:98_B_DS3F. B98 is the only one with an already source-qualified explanation and a retained raw reproduction proving the whole file differs at one environment-derived absolute-path line. It can be turned into a deterministic candidate test without claiming a fresh original result.

The other four are weaker next candidate-only targets:

- A55 has a plausible source-qualified CG-toggle explanation, but the retained pair is old-session history and does not prove the editor300 UI callback runs in a fresh Open. The current public session path is guarded before that callback; its historical bytes are not yet a valid full-lifecycle expected output.
- B120/121/122 compare different historical state boundaries: the expected DOE files include prior GUI-created study state absent from the before inputs. B121 also depends on a nonempty variable-record reader whose original body is not retained/qualified; its current port correctly keeps that branch unsupported. Treating the missing 7/8/10 files as simple Save errors would misstate the input and recovered closure.

## B98 source qualification and raw delta

The retained candidate-only reproduction is `analysis/resume48-b98-raw-repro.md`. It used a disposable copy of `evidence/gui_mo/oracle_b/98_B_DS3F/`, did not modify that evidence, and ran the already-built checkpoint 47 CLI. Its original historical `Gear_blank.MST` is 2,756 bytes, SHA-256 `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1`; its reproduced candidate MST is 2,786 bytes, SHA-256 `05e6dcfff2d3520b3f4495308a6ccbb65915eba5f33b66364ae3d26fd1e13159`. The first differing byte is offset 1,362 in the `KFREAD` path on line 30. There is exactly one differing line and no other payload difference after replacing only the complete old key-path value with the candidate's actual project-local key path.

The source supports that narrow behavior:

- `source/apps/moproj2mst/src/ops/diestress.cpp:114-121` gets the active operation subproject DB path through `activeSubProject` and `subProjectDbPath`.
- `:169-180` implements the original-address `0x1402b5410` writer: it takes that DB path, replaces `.DB` with `.KEY` case-insensitively, and emits one `KFREAD` path when the simulation is selected.
- `:107-112` binds the emitted `KFREAD` count/path ordering to original `0x1401344a0`.

The historical output line names the old tree:

```text
C:/Users/alext/Projects/deform-rea-work/gui_mo/oracle_b/98_B_DS3F/Gear_blank.KEY
```

The reproduced candidate line names its actual disposable run tree:

```text
C:/Users/alext/Projects/codex-deform-rea/imports/claude-moproj2mst/build/b98-repro-checkpoint48/Gear_blank.KEY
```

That is an exact path-value difference, not a waiver for arbitrary path rewriting or a claim of fresh original parity. Preserve the raw bytes and record both raw hashes before applying a test-only exact-line substitution view.

## Candidate-only regression proposal

Run the current already-frozen candidate (no build in this action) on an independent copy of the retained B98 fixture, with its candidate cwd/project under a short `C:\PROBLEM` runtime directory, for example `C:\PROBLEM\b98r\candidate\project\Gear_blank.moproj` (full project path 51 chars). Keep the original historical fixture read-only; do not launch the original app. Use full `--write-save` and record candidate executable/build/source receipt identities, exact argv/cwd, input manifest, stdout/status, and output manifest.

Expected raw result: candidate `Gear_blank.MST` contains one `KFREAD` key path exactly `C:/PROBLEM/b98r/candidate/project/Gear_blank.KEY` (or the actual recorded project-local path if the selected root differs). After replacing only that exact candidate path byte range with the historical line above, the entire candidate MST must equal the retained 2,756-byte historical MST. Assert that no other bytes/files are changed relative to the candidate's pre-run snapshot except documented Save effects in the copied tree. The path substitution view must report its transformed digest/equality separately; never overwrite or relabel the raw comparison as a match.

This should become a focused test around `source/tests`' existing Save/corpus harness, with a read-only fixture copy and candidate-only execution. The current result is from checkpoint47's candidate; rerun against the intended current frozen candidate before promoting it to a regression. Any extra difference, absent/multiple `KFREAD`, wrong key basename, status failure, or unexpected file effect should fail the focused case and remain raw evidence. This test checks candidate consistency with the qualified writer rule and a historical byte anchor. It does not qualify fresh original behavior, external I/O containment, or the full Open–Save–Close contract.

No source, candidate binary, fixture, original application, GUI control, or mutable REA state was changed or run for this audit.

