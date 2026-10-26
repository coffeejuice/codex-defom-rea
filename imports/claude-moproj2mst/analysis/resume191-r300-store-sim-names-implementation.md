# CP191 R300 storeSimNames scoped-borrow implementation

`storeSimNames` now confines the nullable embedded SimControl alias to one immediately invoked, nonstored void callback. The existing single `opModel` lookup remains outside; its existing `m ? m->simCtl() : nullptr` expression supplies the callback argument. The callback returns on null and preserves exactly OPRNAM via local8/strncpy count 0x40, CURSIM[1], SIMNAM via local8/strncpy count 0x50, then CURSIM[0]. No appended terminator or preconversion was introduced. Active-subproject lookup, operation-name snapshot, flow number, live entry vector, first-match selection, no-match return and unnamed-entry refusal remain unchanged and precede model acquisition. GuiModel::simCtl still supplies its existing geometry guard before nullable step access.

## Baseline reconciliation and scope

Before edits, both assigned files were compared byte-for-byte with `build/wave3-checkpoint190-verified/source.zip`, entries `apps/moproj2mst/src/engine/save_current.cpp` and `apps/moproj2mst/tests/s/test_save_current.cpp`. Both matched. Baseline SHA-256 values were `b0d62452408864276dde7825265ced96c1e5fc45cc1fb10b8c047530e84704a0` and `37b8fa328c8605c31faff5559aa4ff8f7955f1431afbe7a1bfb8adc77a50b552` respectively. A post-edit bounded comparison confirms source outside storeSimNames is unchanged and removing the inserted fixture restores the complete CP190 test file byte-for-byte. Equipment, materials, restart, NSTART and all previous fixtures are unchanged.

## Direct fixture

`store_sim_names_step_scope` uses configured corpus46, operation ID 2, requires SfOprHeatTransferExpress2D, installs loader slots, selects current operation and loads its actual model. An independent preflight computes flowNumber and simEntries, selects the first matching entry and requires a named result before each positive helper call and before detachment. The expected entry is copied while its local entry vector lives.

The disposable in-memory operation uses two short ASCII names. Each direct helper call starts with distinct full-buffer OPRNAM/SIMNAM and CURSIM sentinels and checks both scalar values and bounded string bytes, including strncpy zero padding and unchanged bytes at the excluded 0x40/0x50 indices. Changing the operation name before the repeat recomputes independent expected entry data. Each call checks cached-model and step identity. A retained-step RAII guard then detaches the model step, calls only storeSimNames, and requires unchanged complete name buffers, scalar sentinels, cached model and absent step; exceptions fail the test and restore ownership during unwinding. Normal restoration verifies identity and reruns the positive helper. No full sync occurs while detached. No optional no-match fixture was added; the source early return remains unchanged.

Preflight inspected the existing engine_internal declarations, writer SimEntry/simEntries declarations, flowNumber declaration, Testkit main and CHECK/CHECK_EQ/REQUIRE macros, plus existing test-path configuration. CMake source registration maps tests/s/test_save_current.cpp to m2m_s_save_current and m2m.s.save_current. Root owns confirmation of actual registered build state and execution. No new includes are needed. Bounded string construction keeps failures safe even if the helper unexpectedly performs no stores.

## Evidence limits and release

Read Packet S spec section 1, the existing annotated storeSimNames source, simEntries traversal and GuiModel::simCtl. Retained annotation 0x140054540 supports active/name/flow/list selection and the same four stores and widths; this is annotated source evidence, not newly recovered assembly, native execution or GUI Save equivalence. The unnamed-entry refusal remains reconstructed policy. Runtime checks, once executed, establish direct reconstructed helper mechanics; lexical inspection establishes the callback alias does not escape. This change does not pin ownership, prevent destructive reentry, privatize raw access, install the parked Stopping barrier or close broader R300 admission/lifetime questions.

No build, test execution, full suite, GUI/app-control session, original process, REA database, shared state/plan, deployed executable or external fixture was changed. Source/test ownership is released to root for independent review, focused/full validation and freeze.

Released SHA-256:
- source/apps/moproj2mst/src/engine/save_current.cpp: `62d4f2a827a5fc281692e0b5d8199a549a27594585018fed77ac3d399a46c455`
- source/apps/moproj2mst/tests/s/test_save_current.cpp: `780eeec08f58c8a5afc92e512ae01944e5cb9c8e7c19da0b4b8fe071871349cb`
