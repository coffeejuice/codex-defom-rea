# CP121: private diagnostic OPI guard in the actual Editor300 record

StagedEditor300 now retains the original OperationInfo identity generation separately from its GuiModelGuard. An expired OPI or a replacement Operation at the same address cannot authorize continuation merely because the Project/model/remesh addresses still match. The snapshot is diagnostic metadata: it does not execute native130857, expire OPI, advance construction or admit public Close.

Root approved exclusive simcontrol300_owner.hpp/.cpp and test_simcontrol300_owner.cpp edits, then held source changes until the CP119 freeze. The patch began only after root released that freeze. No shared model.hpp, lifecycle/io source/header, Session/main, CMake or plan was edited. No compile, build/test execution, GUI, original launch, REA import/retry or mutable REA database. No reconstructed executable/deployment was inspected or changed.

## Source qualification and behavior

CP120 establishes native editor+38's OPI guard assignment at140130857 after preceding QWidget/base work. Native05f820 may first return a borrowed class model through virtual+e0; model liveness is therefore independent of OPI lifetime. Original DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; controlling analysis/resume120-close-ownership-order.md SHA-256 `95d3f7d24df0a9609ab814be399d8bac3a44cc4bade2106fe3d7b932fd0274c7`.

The private `OperationInfoGuard operationIdentity_` snapshots the supplied actual OperationInfoIdentity during existing factory-record construction. Its constructor shares existing liveness metadata; it creates no second Operation or identity state and retains no Operation allocation. The existing identity checks now compare guard.get() with the explicitly supplied Operation **before** dereferencing Operation fields. All previous Project/model/remesh comparisons remain.

Two record paths without a current binding also check the snapshot: first setup-owner admission refuses an expired OPI, and pageState() hides the live mutable page view after OPI expiry. Historical partialPageState/native-prefix diagnostics remain retained. Previously reached page data/widget allocation is neither erased nor destroyed by guard expiry. Already-bound setup/repeated-construction ordering stays unchanged; no original exception is remapped.

The record never commits or resets OperationInfoIdentity. Destroying its observer cannot expire OPI. Native130857's QPointer binding phase, preceding QWidget/base work, operation/model payload destruction and actual QObject boundary remain separately guarded. This is entry/view admission for this record; it does not poll OPI during lower-level callbacks or revoke a lower-level raw pointer already returned to a caller. Existing lifecycle/public guards continue to apply.

## Focused regression coverage

The target remains `m2m_mdt_simcontrol300_owner`, now13 Release-active Testkit cases (nine retained, four added):

- Explicit declared OPI QObject-boundary expiry while Operation/model/widget allocations remain live rejects identity, base continuation, first setup binding and page construction before model/file effects.
- Expiry after completed isolated pages hides pageState while preserving the exact historical partial page pointer, progress state, model/widget allocation and I/O counts.
- Destroy/re-emplace Operation in one std::optional storage location while retaining the same Project/model/remesh proves the old record rejects the new generation. A fresh record accepts the replacement. Optional remains disengaged if replacement construction throws, so the fixture has no dead-member destructor hazard.
- Destroying the diagnostic editor observer leaves the still-live OPI guard and identity unchanged.

Explicit boundary commits in tests are declared identity fixtures, not proof that native payload cleanup or accepted Close ran. The actual root Close order is unchanged.

Static preflight checked the Testkit entry point, all m2m/testkit/defmodel include resolution against actual roots, supported CHECK/CHECK_EQ/REQUIRE/TEST_CASE macros, absence of C assert/REQUIRE_EQ, field/initializer order and whitespace. It did not compile or run tests. Root owns the focused build/run and source-stable suite:

```text
ctest --test-dir <build> -R '^m2m_mdt_simcontrol300_owner$' --output-on-failure
```

## Frozen receipt

Source paths are under source/apps/moproj2mst/:

| Edited file | SHA-256 |
| --- | --- |
|include/m2m/engine/simcontrol300_owner.hpp|`6f24f1165f65996a7f4d89c76b6006e8ca4802aab7230bae6f50cf0778653b33`|
|src/engine/simcontrol300_owner.cpp|`aca6874a8cc8d77b1469d4a4791d6504308a753e83139a8aad6ddd63c455e9d7`|
|tests/mdt/test_simcontrol300_owner.cpp|`c2b32ede8d343108bbbbf30ac8ec51e4370d9195ee031f87253f8afaabd81e58`|

Read-only dependency: include/m2m/engine/operation_info_guard.hpp and src/model/operation_info_guard.cpp. This receipt records static source preflight, not a tested candidate archive or original runtime witness. Source ownership is released and the three files are held for root integration/build.
