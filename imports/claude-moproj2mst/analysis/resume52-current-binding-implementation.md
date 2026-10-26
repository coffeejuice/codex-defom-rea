# Guarded actual OpClass current binding implementation

Status: source implemented after root confirmed checkpoint50 frozen. Eight focused test cases are ready for root validation. No worker build/test run, CMake/plan edits or public Close enabling occurred.

## Exact source ownership

All production paths are beneath `source/apps/moproj2mst/`:

- NEW `include/m2m/engine/current_operation_binding.hpp`, `src/model/current_operation_binding.cpp`.
- `include/m2m/model.hpp`: only Operation identity/stable lifetime API plus required guard header include.
- `include/m2m/mst/writer.hpp`: actual current field changed to the guarded binding.
- `src/mst/master.cpp`, `src/ops/cycle.cpp`, `src/engine/loader.cpp`: the three explicit temporary-current save/restore seams.
- NEW `tests/mdt/test_current_operation_binding.cpp`.

No conversion_session, model_manager, shared build or other worker source was edited.

## Implemented state/adapter contract

Each actual Operation now owns exactly one noncopyable/nonmovable OperationInfoIdentity. It is the first declared member, so automatic metadata expiry occurs after the represented composite member cleanup and is classified `AdapterIdentityLifetime`. This does not claim native OPI destructor order. An explicit `commitOperationInfoQObjectBoundary(qualification)` seam exists for a future qualified driver, but no production cleanup/Close path invokes it automatically.

Actual `OpClass::current` is now one `CurrentOperationBinding`, with no raw mirror. It supports existing pointer assignment/null reset, pointer reads, bool/arrow/dereference/comparisons and pointer conditional forms. Assignment binds the Operation's existing identity; an expired identity cannot resurrect. Its metadata does not own Operation allocation. No implicit constructor from Operation* was added, preventing a second conversion direction for pointer/guard conditional expressions.

All three temporary-current sites save `current.snapshot()` and restore metadata with `current.restore(saved)`:

1. master.cpp around dispatchOperation/dispatchOperationDoe.
2. Cycle writer around member writeMst.
3. loader.cpp around predecessor provideObject.

Each restore remains at the same explicit normal-return point. No RAII exception restoration was introduced: a throwing inner call still skips its restore line. Metadata snapshots prevent restoration through an old raw address after its actual Operation is freed. The existing Cycle current override remains labelled adapter-only and not native Cycle selection.

## Native source qualifications

Controlling native evidence is `resume50-close-destructor-next.md`, and caller analysis is `resume51-opclass-guard-integration.md`. Original DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; QtCore4_64.dll `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.

Native class current is a guard assigned via5ef280/cbc920/changeGuard. Native OPI050440 performs payload/member cleanup then reaches QObject base at050531; clearGuards at6712f8bd invalidates bound current observations before admitted destroyed delivery. The OPI boundary precedes outer SfMoOprItem diagName/base destruction. The new automatic composite fallback is later/coarser and is deliberately not labelled that native boundary.

Unknown Props/model/list/Qt destructor effects and the complete Close coordinator remain guarded. This patch improves actual adapter lifetime representation; it does not establish original acceptance of hypothetical writer reentrancy or validate a new failure policy.

## Focused tests and static checks

New target: `m2m_mdt_current_operation_binding`; ctest: `m2m.mdt.current_operation_binding`.

Eight tests cover actual OpClass.current: existing pointer forms and actual propsOf(nullptr); alias/unrelated target expiry; controlled Props callback sees current before explicit fixture expiry while outer row remains; automatic adapter fallback without owning Operation; restoring a snapshot after its old row is actually freed; nested temporary bindings; unchanged no-restore-on-throw timing; and registry survival across Project release followed by capsule storage fallback expiry. Operation copy/move and wrapper no-implicit-pointer-constructor assertions are included.

Worker static verification: exact owned files have no trailing whitespace, eight TEST_CASEs are present, all three source seams contain snapshot()/restore(), and the production raw saved-current patterns are absent. No compilation or test execution was performed by this worker.

Root commands, using its actual active converter build directory:

```powershell
cmake --build build/candidate --target m2m_mdt_current_operation_binding
ctest --test-dir build/candidate -R '^m2m\.mdt\.current_operation_binding$' --output-on-failure
```

Root must ensure `operation_info_guard.cpp` and `current_operation_binding.cpp` are in every affected standalone/test-double link set, including separate packet CMake configurations. Do not link the full core to test-double targets solely to hide lightweight ownership/guard symbols. Run existing P07 restoration, P13 Cycle, loader inheritance/provideObject and Boolean same-current tests alongside root-required regression.

## Applied file hashes

- `include/m2m/engine/current_operation_binding.hpp`: `485d34bd9b3926d68fbde9ece5562c181ffe2d85ceb13f75b1b88e4d3400546d`.
- `src/model/current_operation_binding.cpp`: `69dce495107c4bfa389a97a96c51b7aac145663bea593c7aa0e14100ff55555b`.
- `include/m2m/model.hpp`: `4392e65e2dd87e2b48f22ab914f5ba2bf5cfd648d09512fd567bdc5db798a6bf`.
- `include/m2m/mst/writer.hpp`: `37c8c1890c240cfd0365bd7ec82309c7fd1609aa7e29eda25570236bde84941c`.
- `src/mst/master.cpp`: `7ea30c5d6bc8f335fcb65e30eac285920f2d19c15802e3ea9351ec59af21edee`.
- `src/ops/cycle.cpp`: `1bb6772156e94cbe33cd14a99045711c874e087db846d466b89ea79266371df1`.
- `src/engine/loader.cpp`: `a4f74b94d6fb219f62d85cd9517a2d2a99fd154b9cb2bf26fef642e457c3d925`.
- `tests/mdt/test_current_operation_binding.cpp`: `8b96a9e009d2d19e9ee5873d5a33e6331e28889750064cd8e26a82e182f00ef5`.

No reconstruction executable was built/deployed by this lane.
