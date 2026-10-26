# CP256 MultiBlow save-time PDIE owner command implementation

The PASS-reviewed CP254 loader leaf is implemented. `GuiModel::setMultiBlowPrimaryDie(int)` performs exactly one `simCtl()` admission, returns on absent control, and writes only `pdie1`. It is void and deliberately not noexcept because admission can throw. `multiBlowSaveNames` retains the base save-name call, model lookup, first control admission, then evaluates `vt120()` once and delivers its value to the command. The successful final segment therefore still has two admissions, without a third lookup.

## Evidence and qualifications

Read `analysis/resume254-loader-next-leaf.md` and its PASS review, `source/docs/re/moproj2mst/mdt/L4/spec.md` section 1, retained `evidence/gui_mo/s1/mdt/L4/d0.c::FUN_140602780`, and the registered `MultiBlowClass::v120()` in `src/ops/multiblow.cpp` (pure `return 2`). Native d0 resolves/caches the second control before virtual +0x120, then stores through that cached pointer; current C++20 evaluates the RHS provider before the assignment LHS. This implementation preserves current C++ provider order and does not claim native timing equivalence. Equivalence is bounded to the current pure production provider and synchronous stable model storage with no intervening callback/storage replacement or barrier change.

Native's second control dereference is unchecked. The owner's nullable no-op is a safe value-command contract, not proof of that old unchecked behavior. Arbitrary side-effecting providers, reentrant storage replacement and changing geometry barriers require separate review. No C ABI, native layout or pointer-return API changes were made; wider loader alias closure is not claimed.

## Focused validation

Only `admin/build_candidate.py` was used to build, targeting `m2m_mdt_multiblow_save` and `m2m_mdt_viewer_box`. The final receipt is `analysis/cp256-multiblow-save-focused-verified-build.json`; both configure and build exit 0, source stable during build. The configured exact CTest names were inspected before execution:

```
ctest --test-dir build/candidate -R '^m2m\.mdt\.(multiblow_save|viewer_box)$' -V
```

`analysis/resume256-multiblow-save-focused-verified-ctest.log` records 2/2 CTests passing: multiblow_save 4/4 internal cases and viewer_box 58/58. The direct owner test seeds unrelated fields and compares the full expected DB_STEP_DATA byte snapshot after changing only PDIE, verifies step identity, and separately verifies absent-step no-op/no recreation. The existing parked-geometry refusal helper now directly invokes the new command; its enclosing full step, table, viewer, timer, index and effect/state checks all pass.

The independent caller test uses a synthetic registry-backed OpClass and existing model with a counted constant-valued provider. Success evaluates the provider exactly once, commits 37 from sentinel 71 and preserves the remaining full step snapshot. With the step absent, the first admission returns before provider evaluation (0 calls), without recreation or writes/deletes. Counter instrumentation observes invocation only; this test does not dynamically establish pointer/provider order or production callback equivalence. The synthetic class leaves `current` empty to isolate the final caller segment from base name mutation; base invocation/order is retained by source inspection.

All optional Oracle B cases actually executed and passed with PDIE seeded to 73 after open and asserted 2 after save:

| Case | Executed | Pass | Skip | Recorded KEY writes | Open deletes |
| --- | ---: | ---: | ---: | ---: | ---: |
| 93_B_MB3ACT | 1 | 1 | 0 | 11 | 2 |
| 93D_B_MB3ACT_DWELL | 1 | 1 | 0 | 11 | 2 |
| 94_B_MB2ACT | 1 | 1 | 0 | 14 | 2 |

These integration tests compare recorded save.writes paths/bytes to historical Save-step fixtures and open.deletes. They do not serialize or compare the full .moproj and do not run the original application. Final output equality does not establish intermediate hook timing.

The first two runs failed only in the new synthetic fixture: first missing loader-slot installation, then missing operation project/registry association. The fixture now installs loader slots and supplies a real registry entry; all cases pass. Earlier build/test receipts and logs are retained (`focused` and `focused-final` labels). No production repair was needed after those failures. The read-only binding preflight passed (`analysis/resume256-multiblow-save-binding-preflight.json`), and scoped `git diff --check` passed.

## Source and artifact hashes

The following SHA-256 hashes describe working files after final validation, including pre-existing CP254/CP255 changes in shared files. `analysis/resume256-multiblow-save-hashes.json` is the machine-readable companion. This task added only the owner declaration/definition, save call-site replacement, direct owner test and parked invocation, and caller/integration sentinel coverage.

- `source/apps/moproj2mst/include/m2m/engine.hpp`: `b9cc0e030acdc5c6bc08a92212823fea3c0caab80fa60dbf54e6f891e4c8e72e`
- `source/apps/moproj2mst/src/engine/gui_model.cpp`: `d9cda70a50249ee26c45ab38372c635e1742bd8ae217424adb005c6b821ad0f7`
- `source/apps/moproj2mst/src/engine/multiblow_load.cpp`: `c8ee18c55bc2c0f833aa338aa24baf9a7b53f226d7611dc49c0a05b43f5ee67a`
- `source/apps/moproj2mst/tests/mdt/test_multiblow_save.cpp`: `9f0517a83bc38871e803e6c0df59a6af85a9a6334d573f750124c2560499559e`
- `source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp`: `df25200cc5933154858194c867b9987e76c29617d3d6100967b76b6d6822a73b`
- `build/candidate/bin/m2m_mdt_multiblow_save.exe`: `e04a3821810149c0b3e2c5f8f6fe27024ebda967a56c5d972bbf5067fdf8df0b`
- `build/candidate/bin/m2m_mdt_viewer_box.exe`: `68e34631b0342aa497fa071a81fbc963230103197e55a74a75643abbcdd069e6`

Final build source tree SHA-256: `374ca5345d2c69034b64900481081973fe2b9abf6b8585b40cab252ee4e3f40a`.

No full suite, candidate freeze, deployment, GUI-control session, original application, mutable REA work, CMake edits or shared IMPLEMENTATION_PLAN/takeover-state edits were performed. Source ownership is released after this report.
