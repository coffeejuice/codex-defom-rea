# CP126: independent source/admin preflight audit

No probable C++ compile/link blocker was found in the frozen OPI recovery-wrapper, one-row SPRING2 `0xG`, or CRT retained-lease cuts. The admin classifier files parse and match their frozen receipt hashes. One actual target-selection issue was reported immediately to root: the OPI implementation receipt gives an underscore CTest selector, while CMake registers a dotted test name.

## Actual issue: focused CTest selector

`analysis/resume126-opi-recovery-null-prefix.md` recommends:

```text
ctest --test-dir <build> -R '^m2m_mdt_operation_info_cleanup$' --output-on-failure
```

The existing CMake test loop registers `m2m.mdt.operation_info_cleanup`, not the executable target spelling. That selector can select zero tests and still return an ordinary successful no-tests result. Use:

```text
ctest --test-dir <build> -R '^m2m[.]mdt[.]operation_info_cleanup$' --output-on-failure
```

The build target `m2m_mdt_operation_info_cleanup` is correct. This is a receipt-command defect, not a source/build blocker. This audit did not edit the owner's receipt.

## Static source and ownership checks

- OPI: new public declarations/definitions/friend access match. Operation's existing `recoveryB` is `shared_ptr<vector<RecoveryEntry>>`; assignment to `weak_ptr<void>` and cross-type owner_before comparisons are valid standard smart-pointer operations. New tests use actual public progress/Operation APIs and declared RecoveryEntry/vector fields. No private Session method, unsupported path cast, new TU or additional library dependency is introduced.
- SPRING2: production and test remain in existing files and target. The exact malformed-token branch and count-one call parameter introduce no new header, external symbol, filesystem conversion or constructor/interface dependency. Existing Rig paths use the established supported conversions.
- CRT: `matchesRetainedLease(const shared_ptr<FeatureCrtRuntime>&) const noexcept` matches header/source and is public. The static member factory initializes the private Storage weak owner once before publication, within its existing access scope. The existing complete cpp owns Storage; no new translation unit or CMake link registration is needed. Test aliasing/no-op shared_ptr construction uses standard APIs; the private default constructor remains invoked only by its existing factory. The header already supplies memory/optional/array/cstdint types used by these additions.
- CMake: ordinary core/test globs supply all three existing source/test targets. No manual test entry or new library linkage is missing. Correct focused targets/tests are listed below.
- Admin: only `admin/run_converter_tests.py` and `admin/test_run_converter_tests.py` are the classifier owner's frozen code set. Both AST-parse without execution. The new full-line exclusion is limited to the whitespace-tolerant empty active-cogging summary. Nonempty refusal lines and other EVENT matches continue through the unchanged event regex. Existing source/binary stability and failure-status checks remain separate from gap-event extraction. No CMake/generated-source ownership conflict is indicated.

| Build target | CTest name |
| --- | --- |
| `m2m_mdt_operation_info_cleanup` | `m2m.mdt.operation_info_cleanup` |
| `m2m_mdt_simcontrol300_spring_reader` | `m2m.mdt.simcontrol300_spring_reader` |
| `m2m_model_feature_crt_runtime` | `m2m.model.feature_crt_runtime` |

The admin owner's four focused unittest passes are reported in its receipt; this audit did not rerun them. Its retained-log regression is optional when the historical log is unavailable, so that case is not a portable fresh-log acceptance witness. The current frozen report records the log identity and actual owner-run result. Source archives under the `source/` tree do not themselves include admin Python; retain these separately recorded admin hashes when binding the classifier used by the final launcher.

## Verification and exact bytes read

One invocation of `admin/check_test_source_preflight.py` passed for all three C++ test files, with no direct-include, Testkit entrypoint, assertion-macro or Release-assert issue. Python AST parsing checked syntax without import/test execution. All ten source/admin hashes independently match their final owner receipts:

| File | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/include/m2m/engine/operation_info_cleanup.hpp` | `4231a58f2bbffa7f435860e835b6e6c425c7c067349986a8b98c513659c79bbc` |
| `source/apps/moproj2mst/src/model/operation_info_cleanup.cpp` | `52f811446f517211889cbd6d83697567b20286b9a876354b0858057b7ee941bf` |
| `source/apps/moproj2mst/tests/mdt/test_operation_info_cleanup.cpp` | `ac79d6206b2cee955597aa7f3393d8bd1609a97897acb09755f5c1cae92fdcee` |
| `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` | `5f85c75a511caba658873838e329c2c902c3930f3abde2117f3c4b9173139dbd` |
| `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp` | `ddd75e47879ce4736a4babb7856f12ae83eff154492e37d10be1e266442f264f` |
| `source/apps/moproj2mst/src/model/feature_crt_runtime.hpp` | `25b1cc1059fa225017bec07dd5289b9084f92c12fdd78f0bade562d3ce65071c` |
| `source/apps/moproj2mst/src/model/feature_crt_runtime.cpp` | `89989f0d68c6bf7657b8d86e587ebfe550ca95a457c2f385efd5279a01911be8` |
| `source/apps/moproj2mst/tests/model/test_feature_crt_runtime.cpp` | `1529ac9ef022c4e0c0d11b51e94f59832d789f4ea6d941b1054a2e1a5e564c61` |
| `admin/run_converter_tests.py` | `a86a842fbdc31bd5b2ff780872cf7ff558fd7defbf4b9d68d15b649673109b4e` |
| `admin/test_run_converter_tests.py` | `f4970c11714b6a6ab1b8c27ab3a55811db345cc919f19024c5d201b3a59a7513` |

Only this report was written. No source/admin/test/CMake/build/fixture edit, compiler/linker, C++/Python test execution, GUI, original launch or mutable REA operation occurred. Static preflight does not prove MSVC template instantiation, linkage, fixture behavior or native acceptance; root owns grouped build/focused/full frozen validation. Token usage was unavailable.
