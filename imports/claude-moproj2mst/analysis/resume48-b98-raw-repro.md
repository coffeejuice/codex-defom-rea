# B98 candidate raw reproduction

## Result

I reproduced the B:98_B_DS3F Diestress Save with the already-built checkpoint 47 candidate CLI, using only a disposable copy of the retained B98 project tree. Scratch is under `build/b98-repro-checkpoint48/`; the historical evidence under `evidence/gui_mo/oracle_b/98_B_DS3F/` was not modified.

Command used the existing `build/candidate/bin/moproj2mst.exe` on the copied `Gear_blank.moproj`, writing `candidate.MST` with `--write-save --strict`. It exited 0. The build receipt `analysis/resume47-combined-full-build.json` binds this CLI by SHA-256 `890deaa69b91be2eeec13619abf0599225e5ce1f892d0b4c401b0cf7b84fe361`.

Raw outputs:

- Historical original: `Gear_blank.MST`, 2,756 bytes, SHA-256 `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1`.
- Candidate: `build/b98-repro-checkpoint48/candidate.MST`, 2,786 bytes, SHA-256 `05e6dcfff2d3520b3f4495308a6ccbb65915eba5f33b66364ae3d26fd1e13159`.
- Both contain one `KFREAD` absolute path at line 30. Original line:

```text
C:/Users/alext/Projects/deform-rea-work/gui_mo/oracle_b/98_B_DS3F/Gear_blank.KEY
```

Candidate line:

```text
C:/Users/alext/Projects/codex-deform-rea/imports/claude-moproj2mst/build/b98-repro-checkpoint48/Gear_blank.KEY
```

The only line delta in a unified diff is that one path line. The original case prefix is 65 bytes; the candidate scratch prefix is 95 bytes, exactly accounting for the 30-byte total file-length delta. Each path prefix occurs once. Replacing only the original prefix bytes with the candidate prefix yields byte-for-byte equality with the candidate (verified on the retained raw files). This is evidence that this candidate reproduces the current B98 payload apart from the environment-derived absolute path. It is not raw-byte equality before mapping.

## Writer attribution and qualifications

The original Diestress writer at PE VA `0x1402b5410` obtains the subproject DB path and substitutes `.KEY` for `.DB`; `0x1401344a0` emits the resulting QString after `KFREAD`. The imported source implementation is `source/apps/moproj2mst/src/ops/diestress.cpp`. This B98 diff is an ASCII absolute-path value change, not evidence of an ACP byte-encoding mismatch. The historical original output is from the earlier `deform-rea-work` tree. Checkpoint 47 validation did not run the original app, and no original action/session/environment was re-established for this reproduction. The current candidate path was generated from its actual scratch project location.

The generated scratch tree, including the updated copied `Gear_blank.moproj`, `candidate.MST`, and side files, is retained at `build/b98-repro-checkpoint48/` for inspection. The original evidence fixture and source were not touched. No new build, original app run, or mutable REA session was used.
