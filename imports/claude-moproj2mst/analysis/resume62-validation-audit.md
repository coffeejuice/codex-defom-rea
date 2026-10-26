# CP62 independent audit of CP61 validation and public PPV test

The CP61 public PPV result is a real Release-active candidate success for one local-row Open/Save call: it requires M2M_OK and exact rewritten PPV bytes. It does not hide an unsupported return. The main acceptance risk is source/run binding: CP61 executed one public test case, while current source contains a second unexecuted root-only case and the current whole-source fingerprint differs from the stable CP61 build. Do not attribute the new case or current source to the old run.

This audit reads existing source, receipts and artifacts only. It edits no source/test/build/fixture/shared state and runs no application, GUI or mutable REA. Only this report was written. CP61 validation/integration/freeze JSON were absent when initially inspected; this report binds directly to the retained full-run/build artifacts below and makes no objective-completion claim.

## Run and artifact identity

Run folder: `build/test-runs/1792877344044145300`.

| Evidence | SHA-256 / result |
|---|---|
| receipt.json | 8a1eb0b8f82e5b99a28d7717c0473b2357caf028745c065b36d49d86f4485dd8 |
| inventory.json | afeb996ded64218bbc9eef83d2ed1526526db07348d061a4a0b38f71e038b069 |
| junit.xml | cf4bbaa7faafeda8330f3901a302f3853a1f06a9056bc5df6d31bd8dc97478cd |
| fixtures-before.json | ddcc3d21078789fc77fa292c110796e6fe70a15692103339436e8462b2d050b7 |
| fixtures-after.json | Same exact hash as before; both contain 16,728 files |
| m2m.mdt.ppv_public_lifecycle.log | 686b9cd9138d2bf025649a3ea4a248eca0e54cdb68355e9828881f0773f358ea |
| CP61 public-test executable | a753cd0da7c7eac99267b4808178e3c8fe2c50cb67dad0e1649d5d86816a714d |
| CP61 sibling moproj2mst.dll | b1ab70be797e50e43fd395d9ee46d52365b85417f7f7f368985ccee9cb51d045 |

The executable and DLL hashes in `analysis/resume61-integrated-full-build.json` still matched their current files when checked. The build reports stable source tree `60fe2cb33fa7863409e04b811a4cba46e13d647befea3089511a31bee327cca3`, 3246 files. That receipt is distinct from the earlier focused build's source hash.

Full-run summary: 198 registered tests, 191 clean passes, six logged-gap passes, one diagnostic-difference test, no reported failures/skips/incomplete outputs or unreported tests. This is not 198 independently complete behaviors. The six gap tests are mdt.mdt_write, mdt.save_l6, moproj.deflate, moproj.props_l2, p07.corpus and s.save_current. They retain explicit refusals, missing optional fixtures, excluded inputs and comparison omissions. The folder test retains five historical differences and excluded/noncompared cases. These qualifications must survive final reporting.

The public PPV log contains exactly:

```
[ ok ] public_ppv_local_input_is_rewritten_by_one_public_open_save_call
0 of 1 cases failed
```

No SKIP, refusal, unsupported allowance or difference normalization appears in that target's output.

## Release assertions and effective coverage

The test uses Testkit REQUIRE/CHECK/CHECK_EQ and returns testkit::run_all(); there are no raw C assert calls. Its current two-case source has 26 REQUIRE, 8 CHECK and 9 CHECK_EQ calls. These current-source counts are not retrospectively the CP61 one-case assertion count. In the executed local-row case the public status check is exact M2M_OK, not M2M_OK-or-unsupported; missing DLL/export, source files, PPV output or malformed setup produce actual failures.

The test locates the DLL beside its own executable, resolves m2m_convert_w and invokes it with M2M_FLAG_WRITE_SAVE. It requires an output MST and exact PPV bytes:

```
1\r\n0\tfalse\t7\t8\t9.5\t10.5\t0\r\n
```

The actual output bytes are 27 bytes, SHA-256 `e03950a2ea47b52c8ef5dcdae44a93d6a48b88e3311bc462e55a8d41e3dd13d1`. This establishes public candidate execution of the admitted local-row profile. It does not establish fresh-original parity, other encodings/shared rows, partial failures, all side effects or private Close cleanup. The test and worker report correctly state those limits.

A read-only scan of all 198 per-test logs found no `0 of 0 cases failed` output. One empty log, m2m.p08.keyword_table, is legitimate: inventory binds it to the Python PE keyword-table checker, whose --check path calls sys.exit(0 or 1). It is not a zero-assert C++ helper accidentally registered as a test. Its skip code 77 is explicit; this run records passed, not skipped. Do not remove that meaningful checker merely because successful output is empty.

## Fixture fidelity

The test copies retained `oracle_b/93_B_MB3ACT_before`. It pins the original project SHA-256 `0e3bbd030ae3eda05898a6138a223ff560bd6611631ae276a69ac87ca9694b78` and Task00002/SimCtrl.KEY SHA-256 `7a822135c109f96c7b7cff32752c0d1bfecebe5814f1fef431b137d79716bc9c`. Both match the full-run fixture-before record. Rehashed all 31 files of this retained source fixture and verified their current bytes/sizes match that run's records. Before/after full manifests are exactly equal.

The disposable copy changes Sub_Project Active from 2 to 1 and adds an ASCII local PPV row. The retained project lists operation 1 Forming, operation 2 Forming and operation 3 MultiBlow3D; Task00002's KEY contains CURSIM 2 2 0. The changed Active index intentionally selects the operation with the retained KEY. This is an explicitly modified candidate fixture, not an untouched original oracle or held-out original Save result. Copy-manifest equality and post-run source-manifest equality protect the retained input; they do not turn the injected PPV row into independent original evidence.

Retained copies live under `build/candidate/t/355932f9/ppv_public_lifecycle/.../project`. Three copies exist from prior focused/full attempts. Only the MST in `3588-18839906` has mtime inside the full-run launcher window; that is useful timestamp association, not an immutable per-case run receipt. All three retained PPVs have the same expected bytes/hash.

## Exact risks and smallest corrections

1. **Source/run mismatch before freeze.** Current public-test source SHA-256 is `ad6175f8e18312ae484051983c27f3dc8ea303e75f165536a7f3c3660d8f2e29`; it contains the additional root-only test. The current whole-source fingerprint measured `0c92c4d3b44b228cb4074776df6914a58d3237a920a31baef1a9cc1d144d461f`, 3246 files, which differs from CP61's build hash. No packaged CP61 receipt existed at that check. Smallest correction: preserve an exact immutable source snapshot that hashes to the CP61 build before naming it CP61-frozen; otherwise build/revalidate/package current source as a new candidate. Do not attach current source or root-only acceptance to the one-case old log. An in-memory removal of only the second case did not recreate the build fingerprint, so this audit cannot supply a proven reverse patch.
2. **Root-only test is not yet covered by CP61.** Current second case checks preservation of a shared/root-only input at the local-empty Save gate. Smallest correction: execute it under its own newly bound build/full-run receipt and retain its active-case summary. Source presence alone is not a result. As two cases share a process/DLL, any later claim about fresh-process isolation needs separate process evidence; ordinary regression success can remain qualified.
3. **Short-path compliance is absent.** The retained full-run MST/project paths are 150/154 characters and its PPV path 169 characters; other copies reach 170. They live under build, not the parent instruction's C:\PROBLEM short-root convention. The test only checks scratch containment and does not enforce the 100-character rule. Smallest correction: root reconciles the parent short-path rule with the subtree's local-scratch convention, selects an authorized short disposable root and asserts actual generated path lengths before relying on short-path conformity. Do not claim current paths are short or silently move inputs outside authorized scratch.
4. **Artifact assertions are focused, not whole-Save/Close acceptance.** The final manifest checks presence of MST/PPV and the PPV's exact bytes; it does not compare every generated file against an independent oracle, observe KEY-before-PPV physical failure order, or inspect private Close/global state. Smallest correction is reporting scope accurately. Add only the independently evidenced next lifecycle assertion when its oracle exists; do not invent whole-folder parity from candidate output.
5. **DLL handle lifetime is unbalanced in the helper.** loadPublicConvert frees the module only on missing export; a successful load returns the function pointer without an eventual FreeLibrary. This is bounded by test-process exit but can keep the same DLL alive across the new two-case source. Smallest correction if module-unload/cleanup becomes part of the test is a scoped module owner whose function pointer cannot outlive it. Current success proves neither unload nor kernel-handle cleanup, so avoid those claims.

The first two items affect source/build/result acceptance immediately. The path setup needs explicit qualification/correction; the last two are bounded coverage/lifetime limits, not evidence that the local-row PPV assertion is false. Preserve the successful candidate result and all remaining gaps together. No new original execution or broader scientific acceptance is invented by this audit.
