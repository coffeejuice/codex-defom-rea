# Independent audit of frozen checkpoint67

Checkpoint67's archive/source/artifact/test bindings verify. Its four focused targets execute 145 active cases, with the 212-test inventory and all prior gap/historical/fixture qualifications unchanged. The regenerated B120 XML test now genuinely deletes the copied XML before Save and checks the regenerated physical bytes. This closes the prior stale-copy assertion gap for that pre-refresh prefix, while full Type2/public Save remains guarded.

Only this report was written. The audit read frozen archives, receipts/logs and already-retained scratch/golden bytes. No source/build/shared-state edit, executable/test, GUI or REA action occurred. No fresh original run or broader acceptance is inferred.

## Frozen identity and execution binding

| Evidence | Independently verified identity |
|---|---|
| Frozen receipt | build/wave3-checkpoint67-verified/receipt.json; SHA-256 1a7589a05293d9b37c74d5f95a37362ecefb9f6dc72b4e5f0292369ccc35f1d9 |
| Source ZIP bytes | c14344e80b46441c52598a5b86a910050cb5a277e072e963fd4877afe2926d10 |
| Canonical source tree | 3286 members; SHA-256 9a3ccf5bb4fb90d65b05e3d2f99b0e1ca0ee761f2512bec259b19bcede002852 |
| Build | analysis/resume67-integrated-full-build.json; matching stable source before/after |
| Validation | analysis/wave3-checkpoint67-validation.json; SHA-256 0803dc9249dce22a01be8d8095270d5528bbb52049b60aa228232c84e74c9412 |
| Full run | build/test-runs/1792881658220086500 |
| Selected test receipt | analysis/resume67-integrated-full-tests.json; SHA-256 01344d43c8897da2de0f3f0a251a51ac778dd0ffb6ea5cd6210074b17e6b26af |

Recomputed archive byte hash and every source-member fingerprint using canonical WindowsPath order. Source tree/count agrees with the archive, stable selected build, execution boundaries and freeze. The run receipt is byte-identical to its selected analysis copy. All five hashed contracts and four focused log hashes match validation.

execution_binding reports equal 3286-file source boundaries, 209 equal binary manifests and stable_during_test=true. Independently matched 207 built-output sizes/hashes to the build receipt; the other two entries are cmake.exe/python.exe. No mismatch was found. Raw CTest and effective launcher exits are both 0; unreported/incomplete outputs are empty.

Rehashed all three preserved production artifacts:

| Artifact | SHA-256 |
|---|---|
| moproj2mst.exe | 80ba45e3e89a75410d6dd89c73fb3019146007c3bda158857a766e6e2be61af9 |
| moproj2mst.dll | fa7d50c5630f6a54146a892cd7a5d8145fcfeaf3cac5da24ac31f78d7e504d30 |
| deformdb.dll | c89256fd4a9b226adc9e1eeb57dcaf36a99ab1dfae66b1fc1f419077e9577aa3 |

These hashes bind the frozen candidate; successor working files do not inherit this result.

## Assertions and suite stability

Frozen declarations and retained summaries agree: type2_native_open_guard 1, editor_base 73, editor_setup 59 and simcontrol300_native_io 12, 145 focused cases total. The scanned frozen test_*.cpp files contain no raw C assert calls. No zero-case success appears. The known empty Python keyword-table log remains valid exit-status checking, not a disabled test.

The full run remains 212 registrations: 205 clean, six passed-with-logged-gaps, one passed-with-logged-differences, zero failed/skipped/incomplete. The six gap suites remain mdt.mdt_write, mdt.save_l6, moproj.deflate, moproj.props_l2, p07.corpus and s.save_current. The Type2 focused test deliberately requires exact M2M_E_UNSUPPORTED at the refresh boundary; that is active guarded partial-effect validation, not a hidden positive Save refusal.

Against checkpoint66, independently verified identical historical result/difference records, empty changed_diagnostics, identical folder-comparator source and identical complete folder log:

- Comparator SHA-256: 38c6fb80a3bc30a9cf15b49233a88e83f33a8dd4c4b1da5570254dd33679ad83.
- Folder log SHA-256: 58dd03cef8bfc3a4f22ce0ca8b8296fd63fa5bcda50aa8811de986c58002f77b.
- Result: 93 SAME / 5 DIFF. A55/B98 retain one differing file each; B120/B121/B122 retain one differing file and 7/8/10 missing files respectively. Four folder exclusions/noncomparison notes remain.
- Complete fixture records are equal before/after over 16,728 files; both snapshot-file hashes are ddcc3d21078789fc77fa292c110796e6fe70a15692103339436e8462b2d050b7.

Declared path/view/banner normalization and removed work trees still limit raw-output claims. Identical diagnostics are not fresh full-tree raw parity or fresh original acceptance.

## B120 regeneration is physically asserted

Inspected the frozen apps/moproj2mst/tests/doe/test_type2_native_open_guard.cpp, SHA-256 e5e3f3331b725ed4d3841d1e2dabdfa685c8d541e103bbc493da2b41e9d56d33. Its order is:

1. Copy retained 120_B_DOE into a fresh owned scratch directory, compare recursive source/copy manifests and load the copied project DOM plus InputTable into the actual Session-owned Type2 Study. Require empty variables and zero constraints for this admitted XML branch.
2. At lines 169-172, compute the copied InputTable hash, assert fixed golden 1777bb9948fb2d6f55411f9dcfdc4a7c9196230086c0f86720255d03e76571e0, then REQUIRE(fs::remove(inputTablePath)). Successful filesystem::remove confirms the copied file was removed; there is no recopy before Save.
3. Call saveProjectFileNative on the copied main project. Require exact M2M_E_UNSUPPORTED and `Type2 study Save: signalDoeTableRefresh() receiver delivery remains unsupported`.
4. Require the physical InputTable path to be a regular file again, exactly 249 bytes and matching the pinned golden SHA. The helper reads actual filesystem bytes, not Project.overlay or a simulated I/O ledger. Require the main .moproj remains 0 bytes, view-prefix count 2, old Type2 node identity/serialized bytes retained, earlier Title mutation present and the retained source manifest unchanged.

A stale copied golden file cannot satisfy this sequence: its removal is required before the writer call, and the post-call filesystem file must exist with the exact payload. This is materially stronger than the earlier native guard test, which did not assert InputTable output. It validates regenerated native candidate bytes before the unresolved refresh continuation.

The retained golden file is 249 bytes with SHA-256 1777bb9948fb2d6f55411f9dcfdc4a7c9196230086c0f86720255d03e76571e0. Independently read the retained scratch copy whose XML mtime lies within the full-run launcher window:

```
build/candidate/t/bf16881e/type2_native_open_guard/32180-23156406/project/DoeStudies/DoeStudy1/Control/DoeInputTable.XML
```

It is 249 bytes, matches the golden bytes exactly and has the same hash; its sibling COG_P1_B1.moproj is 0 bytes. Timestamp association supports locating the retained copy, while the bound executable's active assertions establish the test sequence. No new process was launched to obtain this result.

## Remaining claim boundary

The result does not show refresh delivery, PAR generation, final main-project XML serialization, node replacement, public C ABI Type2 selection/completion or a fresh original lifecycle. The guard intentionally stops after XML effects while the main file is already truncated; the retained old subtree is not evidence of transactional rollback. Exact physical XML bytes also do not by themselves report the XML function's post-close Boolean/error value; ordinary XML true/false both reach the same refresh guard.

The integration record correctly says frozen_incomplete, B120 XML matches only the pre-refresh prefix, refresh/public Type2 Save remain guarded, historical five differences are unchanged and fresh original capture is blocked by unverified control release. Freeze/validation retain objective_complete=false. No binding defect, stale-copy false positive or overbroad completion claim was found in those inspected records. Preserve the stronger physical-prefix evidence and its explicit limits; no optional duplicate run or weakened guard is needed.
