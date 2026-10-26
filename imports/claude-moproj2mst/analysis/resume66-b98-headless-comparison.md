# CP66 B98 headless candidate-to-retained-output comparison

## Result

The already-built B98 regression passes against the retained `98_B_DS3F` output. Raw candidate bytes differ because the single `KFREAD` path names the candidate's copied `Gear_blank.KEY`; replacing only that exact path occurrence in a separate in-memory comparison buffer produces bytes exactly equal to the retained MST. This is a candidate-versus-retained-output check, not a new original application run or fresh original lifecycle evidence.

## Frozen test/build evidence

Inspected the CP65 integrated full-suite run at `build/test-runs/1792880735338521700`:

- Test: `m2m.s.b98_kfread`, status `passed`, 0.0546889 s; log: `build/test-runs/1792880735338521700/m2m.s.b98_kfread.log`, SHA-256 `f446e8c0aafee6f6b01e65610145106f605698de80dd3dc2146c07f2c260d17d`. It prints `[ ok ] b98_diestress_kfread_uses_current_project_key_path` and `0 of 1 cases failed`.
- Test receipt: `analysis/resume65-integrated-full-tests.json`, SHA-256 `70e8443b53376783d615f6a803861aca7c7edb3469ee294c270a6a47cfc6b274`; this run records 205 passed, six passed-with-logged-gaps, one passed-with-logged-differences, and no failures/skips/incomplete outputs.
- Build receipt: `analysis/resume65-integrated-full-build.json`, SHA-256 `cf3a01c4588d6d90508f5b2d9b49b6119a82c2038a03b03d786549374d9cd307`. Its source tree SHA-256 is `3b4b3056d55817402fbbc82a57569809ee1d768025909ba6c48d63955c2189c9`.
- Executable: `build/candidate/bin/m2m_s_b98_kfread.exe`, 4,099,584 bytes, SHA-256 `0c894439923ba2f5d9a954298a6d2fb64146da879b934908eefe27d654a91316`.
- The run receipt records identical before/after source-tree hashes and `stable_during_test: true`. The executable was not rebuilt.

## Direct headless run and exact inputs

For a durable raw-byte receipt, invoked the already-built test executable without arguments with these environment values:

```powershell
$env:M2M_ORACLE_B = 'C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\gui_mo\oracle_b'
$env:M2M_TEST_SCRATCH = 'C:\PROBLEM\cp66b98'
& 'C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\build\candidate\bin\m2m_s_b98_kfread.exe'
```

Exit status was 0. The test copied the retained fixture into `C:\PROBLEM\cp66b98\b98r-32196-22540500`, converted only the copy using the existing test's `M2M_FLAG_WRITE_SAVE | M2M_FLAG_STRICT` and code page 1252 call, then compared output. Project path length is 56 characters; candidate KEY path length is 53. The source fixture was not edited. A sorted manifest over all 31 fixture files, formed from each file's SHA-256, byte length and full path, had digest `1a9066f35f83705e0ecf881ec2f426bf749c1e5a3df6e083c49beb5c1d7957d2` both before and after. The retained `Gear_blank.MST` is 2,756 bytes, SHA-256 `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1`.

## Raw and path-qualified comparison

| View | Bytes | SHA-256 | Result |
|---|---:|---|---|
| Raw candidate `C:\PROBLEM\cp66b98\b98r-32196-22540500\candidate.MST` | 2,729 | `4b9cd7c91bd648a42fae7e0384521866d3185637d4d164ca4d34f155d2504e00` | Differs from retained output; first differing byte at offset 1341. |
| Retained `98_B_DS3F/Gear_blank.MST` | 2,756 | `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1` | Immutable historical byte anchor. |
| Separate normalized buffer | 2,756 | `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1` | Byte-for-byte equal to retained output. |

The comparison buffer replaced exactly one occurrence of
`C:/PROBLEM/cp66b98/b98r-32196-22540500/Gear_blank.KEY` with the retained line's
`C:/Users/alext/Projects/deform-rea-work/gui_mo/oracle_b/98_B_DS3F/Gear_blank.KEY`.
The raw candidate remains unchanged in scratch. This matches the test source's
single-path substitution assertion; it does not normalize other content or
change production output.

## Scope and execution note

The retained anchor is an earlier output, not a new original session. The test
qualifies the candidate Diestress `KFREAD` path behavior only. It does not close
the historical 93 SAME / 5 DIFF lifecycle denominator or establish original
runtime parity.

An initial diagnostic invocation created a second candidate scratch (a
test-generated mutation below the current build's scratch directory) under
`build/candidate/t/cp66-b98-headless/b98r-2476-22485515`; its KEY path is 136
characters. Read-only inspection found a 2,812-byte raw candidate
(`f0b0d9b106d7666b858923e7fd66acbd4ad0604da842f8458518a7e931ef5e71`) whose
same one-path replacement also equals the retained bytes. That path was too
long for the short-path project guidance; the `C:\PROBLEM` receipt above is the
reported result. The diagnostic scratch was left untouched. No source,
compiled build artifact, or retained fixture was edited; the scratch-directory
mutation is disclosed above. No GUI or mutable REA session was used.

Source/test qualification: `source/apps/moproj2mst/tests/s/test_b98_kfread.cpp`
contains the candidate conversion and one-path assertion; its retained source
description is `analysis/resume53-b98-regression.md`. The test tests the
Diestress writer path, not general B98 conversion parity.
