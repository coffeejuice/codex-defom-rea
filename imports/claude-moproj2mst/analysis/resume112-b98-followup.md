# CP112 B98 remaining DIFF: raw path and qualified comparison

## Decision

Keep the B:98_B_DS3F historical folder-comparator result as one raw `.MST` difference. Do not change the Diestress writer or the folder comparator. The existing `tests/s/test_b98_kfread.cpp` already supplies the narrow, source-backed qualification: it verifies the candidate writes the key path belonging to the copied project, substitutes only that exact path in a separate in-memory string, then requires the entire transformed candidate MST to equal the retained historical MST. This is neither raw byte equality nor fresh-original parity.

No additional test or production correction is justified by the retained evidence. Replacing the candidate's project-derived path with the historical workspace root would contradict the writer's ownership rule and make output depend on the old capture environment.

## Evidence reconciliation

The retained B98 `Gear_blank.MST` is 2,756 bytes, SHA-256 `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1`. Its line 30 KFREAD value is:

```text
C:/Users/alext/Projects/deform-rea-work/gui_mo/oracle_b/98_B_DS3F/Gear_blank.KEY
```

The older bounded reproduction in `resume48-b98-raw-repro.md` retained a 2,786-byte candidate MST at a longer build-tree path. Its unified diff had exactly that one path line; replacing the old path prefix with the candidate prefix made all bytes equal. CP95 independently repeated the candidate-only comparison using a unique short `C:\PROBLEM\b98r\...` root: the candidate raw MST was 2,721 bytes (SHA-256 `42a48184d865f1465a5d4a2f23ab935743cada14b9a8a16cf87a5170b416f947`), while the retained original remained 2,756 bytes. In that receipt, replacing only the candidate path with the historical path yielded exactly the retained 2,756-byte vector and hash. CP95 recorded 29 comparable paths on both sides, one raw differing file (`Gear_blank.MST`), and no missing or extra paths.

The folder comparator's B98 row reports one differing MST and no comparator adjustments. This agrees with the raw evidence: the comparator preserves the path change. The separate B98 path test does not revise that row; it answers the narrower question whether the rest of the file matches after one exact source-explained path substitution.

## Source binding and test coverage

Candidate `src/ops/diestress.cpp` implements the original-qualified writer sequence at `writeMst` (`@src 0x1402b5410`): `opSubProjectDb` obtains the active operation subproject's DB path, `.DB` is replaced case-insensitively with `.KEY`, and `emitKfreadOne` emits `KFREAD` followed by the resulting path. The retained original decompile identifies the same source operation, and the bounded original writer evidence attributes KFREAD path emission to `0x1401344a0`. For the copied B98 project, the correct candidate path is therefore the copied project's own `Gear_blank.KEY`, not the retained fixture's former absolute root.

`tests/s/test_b98_kfread.cpp` already checks all of the following:

- retained historical key path occurs exactly once in the oracle;
- candidate conversion succeeds on a disposable copied B98 project;
- the copied project's exact key path occurs once in candidate output;
- the KFREAD marker count agrees between candidate and historical outputs;
- the emitted path line is exactly the copied key path; and
- replacing only that one candidate path yields byte-for-byte equality of the complete MST with the historical bytes.

CP95 adds a repeatable retained-artifact receipt: its candidate executable and dependencies were checked against the CP95 freeze receipt, the child mode was pinned, raw candidate and transformed comparison bytes/hashes were kept separately, and no original GUI or executable was run. This supports the bounded candidate-path explanation, not the original historical action sequence. The project-root-dependent raw length/hash varies by run directory, so neither should be treated as a fixed candidate oracle.

## Why no correction is source-closed

The current writer matches the retained original's project-derived path behavior, and the focused test already demonstrates whole-file equality after the single exact substitution. There is no evidence of an implementation defect in the writer. A production change to emit `C:/Users/alext/Projects/deform-rea-work/...` would be incorrect for a project copied or opened elsewhere. A comparator change would erase a real raw difference and duplicate the already-separated path-qualified view. A fresh original Save with the same short root is unavailable in the retained receipts, so matched-root raw parity and exact historical GUI action equivalence remain unproved.

The bounded next acceptance step is to preserve both views in any later B98 receipt: raw path-bearing candidate bytes/hash and the separate exact-path-qualified whole-file equality result. Do not report the normalized equality as removal of the historical raw DIFF.

This audit was read-only. No source, test, build, comparator, fixture, GUI, original executable, or mutable REA state was changed or run.
