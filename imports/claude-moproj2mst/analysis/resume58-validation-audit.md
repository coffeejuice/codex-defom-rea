# Checkpoint 58: validation and test-effectiveness audit

## Freeze, tests, and fixture identity

The audited freeze is `build/wave3-checkpoint57-verified/receipt.json`: source-tree SHA-256 `45e429b5bbdd882f9c8cafbfa722fa194621ecbf158ad0f3fb8375e032032edc`; source archive SHA-256 `87aebc47e58e22f72a6ddd03ec3b92f743349613d8961de72ad0ef99ffa4134c`. I read the archive entry for `tests/mdt/test_simctrl_ppv_binding.cpp`; its SHA-256 equals the current file and the hash documented in `analysis/resume57-ppv-undersized-flags.md` (`53d3d6ca57d9adad76763470772d36c3cf020dd7b9bc64fcbd376f97bb7213d1`).

The checkpoint57 Release run registered 181 tests: 174 clean passes, 6 with recorded gaps, 1 diagnostic-difference pass, 0 failures, 0 incomplete outputs, and 0 skipped CTest cases. There are 181 per-test logs. The changed PPV targets report live Testkit cases: `m2m.mdt.simctrl_ppv_binding` 6/6, `m2m.mdt.ppv_count_state` 2/2, and `m2m.mdt.simctrl_ppv_writer` 3/3. The new undersized-flags case appears by name and reports `[ ok ]`; its Release-active `REQUIRE` and `CHECK_EQ` macros are not C `assert`.

`fixtures-before.json` and `fixtures-after.json` each contain 16,728 entries and are byte-identical: both SHA-256 `ddcc3d21078789fc77fa292c110796e6fe70a15692103339436e8462b2d050b7`. The validation also records `fixture_changes: []`. The B98 source fixture entries in that manifest are stable at: `Gear_blank.moproj` SHA-256 `dddc325a6d6eb34c1625a904eb1bc055a6403c450c312bbe1703bb26af5f9da5`; `Gear_blank.KEY` SHA-256 `82565a02650afa84774ea45eccea1a32b64ed113b405720b43a7b34e967b5290`; historical `Gear_blank.MST` SHA-256 `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1`. The referenced original executable entry is `DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; it was only in the fixture inventory, not launched.

## Concrete zero-case registration defect

`source/apps/moproj2mst/tests/support/test_paths.py` is a helper module, not a test: it only defines `fixture()` and `output_path()` and has no test cases or top-level validation. However `source/apps/moproj2mst/CMakeLists.txt:192-201` applies the `tests/*/test_*.py` auto-discovery to the `support` directory and registers it as `m2m.support.paths`. The checkpoint57 CTest log shows that script ran and passed, while its retained per-test log is zero bytes. This is a false-positive zero-case test in the 181-test count. The separate `m2m.p08.keyword_table` log is also empty, but that script invokes `gen_keyword_table.py --check`, which computes and compares the generated table and returns nonzero on mismatch; it is an active check, not a zero-case test.

Small correction: keep the helper importable as `test_paths` for `tests/p06/gen_mdt_golden.py` and `tests/p11/gen_xcheck.py`, but exclude `tests/support` from Python test auto-registration (or move the helper without matching the test glob). Re-run CMake discovery and the suite so the registered-test count no longer counts this helper as an executed check.

## Public PPV guards are not accidentally validated by helper tests

The CP57 PPV tests are seams, not public lifecycle coverage. `src/engine/loader.cpp:238-240` still throws the unsupported `SimCtrl.PPV` guard when the file exists. `src/engine/save_current.cpp:297-298` still refuses nonempty local recovery points. `analysis/resume57-ppv-open-binding.md` records the unimplemented native file decoding, per-slot count update wiring, and root-only Save pairing; `analysis/resume57-ppv-writer.md` says the isolated writer is not wired into Save. The new `simctrl_ppv_binding`, `ppv_count_state`, and `simctrl_ppv_writer` tests call model/helper seams, and their logs do not claim public Open/Save acceptance. I found no permissive result assertion in these targets that converts the still-guarded path into a pass claim.

Other validation limits remain visible: six tests log optional/missing/class-registration gaps; `m2m.s.save_folder` remains diagnostic and reports five historical DIFFs. The suite contains no fresh original executable action. This audit changed no source, build, fixture, GUI, or REA state.
