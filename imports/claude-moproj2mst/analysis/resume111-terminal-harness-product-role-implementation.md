# CP111 product worker role and explicit scratch root

This source-only increment lets the opt-in CLI supervisor select its own-image product child and use a caller-validated scratch root without requiring the test runner environment. It does not invoke conversion, alter the wire protocol, relay a terminal status, or change the public C ABI. Root owns the build, focused execution, full suite and candidate freeze.

The exclusive source files are frozen with SHA-256:

| File | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/src/engine/terminal_harness_supervisor.hpp` | `7c256a901571ddbb786f44033f1e348010096343a6a9ae8375c26536ba2f2c3e` |
| `source/apps/moproj2mst/src/engine/terminal_harness_supervisor.cpp` | `bc8a00d8593f12f116b25e98a35ef96d1342f206db046cb207ea26cdff058ce6` |
| `source/apps/moproj2mst/tests/mdt/test_terminal_harness_product_role.cpp` | `d4046441d68faa4f38eacad1f642b47aca3df281c81d301946fc18518e87b8d0` |

No protocol, core, CLI, CMake, existing test or fixture file was edited by this worker.

## Exact interface and launch envelope

`WorkerRole::ProductIsolatedSave` is appended as value 2. `ChildCase::ProductIsolated` is appended as value 25. That role accepts only `ProtocolProfile::RemeshV2` and that dedicated case; older roles reject case 25. Existing self-test and conversion role/profile/case matrices remain intact.

`Options::explicitScratchRoot` is an appended `std::filesystem::path`, empty by default. Nonempty values require the product role. The root must be an existing absolute directory, with canonical spelling after preferred separator and case-insensitive Windows comparison. Every existing root component is inspected before canonical resolution and must be a directory without a reparse attribute; dot components are rejected. `Options::directory` must name a new direct child leaf. The leaf is created exclusively, then its components and canonical identity are rechecked. A preexisting leaf fails. Empty explicit roots retain the previous `M2M_TEST_SCRATCH` and build-ancestor validation path, including for the product role.

The exact child command retains argc 7, including executable:

```text
argv[0] = own executable
argv[1] = --m2m-isolated-save-worker-v1
argv[2] = --allow-owned-worker-termination
argv[3] = 25
argv[4] = inherited protocol write HANDLE as uintptr decimal
argv[5] = cryptographic nonzero uint64 nonce as decimal
argv[6] = unrelatedHandle as uintptr decimal (ordinary product value 0)
```

The dedicated product worker owns that role entrypoint. Existing `childMain` does not admit it. The new test provides only a local transport child publishing Started and Returned(0); it never calls Open, Save, a native handler, or a terminal endpoint.

At root's request, `PreparedCase` now has default no-op `virtual void bindNonce(std::uint64_t)`. It runs once after nonce generation and before any child process exists. A product preparation lease can publish its exclusive bounded job containing this exact nonce. A standard exception is a sticky `StopReason::Preparation`; no child is created. The existing end-to-end deadline is checked after binding and again immediately before CreateProcess; expiration prevents launch. The existing callbacks need no implementation changes.

## Ownership and limits

Own-image equivalence, expected SHA-256 before/after, bounded captures, restricted handle inheritance, suspended creation, job assignment before resume, sticky failure precedence and bounded cleanup remain in force. The prepared lease remains declared before all child/job handles, so handles are destroyed first. After confirmed child stop, inspection runs while the lease is alive. If child stop cannot be confirmed, the existing fail-closed branch releases the unique pointer without destruction and sets `preparationHeldUntilParentExit`; OS process teardown owns eventual release. This increment does not claim that exceptional branch safely cleaned a live child.

Reparse checks are validation of owned paths at the inspection points. They are not handle-pinned protection against a hostile same-user concurrent replacement. The new explicit-root branch is a product ownership policy, not a general filesystem sandbox.

The tests reserve one exclusive checkout-build scratch root and keep captures within it. Their bounded summary is `terminal-product-role-last-run.txt` beneath configured scratch. Successful runs remove their checked exclusive root; failures preserve it. The junction fixture owns its link and target, removes only the link before recursive root cleanup, and does not skip its check when construction fails.

## Static verification and root integration

`python admin/check_test_source_preflight.py source/apps/moproj2mst/tests/mdt/test_terminal_harness_product_role.cpp` passed with no issues: Testkit main and supported Release-active macros, resolved direct quoted includes, and no C assert. All three source files passed masked delimiter balance and trailing-whitespace checks. No compile, link, executable, GUI or original target was run. In particular, Win32 junction fixture construction remains subject to root runtime validation.

The CMake test glob derives target `m2m_mdt_terminal_harness_product_role`, CTest `m2m.mdt.terminal_harness_product_role`. Eight cases cover:

1. Exact role/profile/case matrix and unchanged option defaults.
2. Explicit root without M2M_TEST_SCRATCH, same nonce observed by child, once-only binding and held lease inspection.
3. Nonempty explicit root refused for both older roles before preparation/spawn.
4. Relative/missing/file/noncanonical roots, nested/outside/preexisting leaves.
5. Actual owned junction as root and ancestor, rejected before leaf creation.
6. Empty-root legacy environment/build behavior.
7. Binding exception retained as Preparation, no child, unlaunched lease destroyed.
8. Bounded binding deadline overrun retained as Timeout, no child.

Run the existing v1 safety, v2 protocol, role and real remesh worker targets with this new target before the source-stable full launcher. This report establishes implementation and static preflight only; it is not a passing runtime or product fidelity receipt. Token usage was not measured.

## Focused-run assertion correction

Root's first focused run exposed an incorrect expectation in the new deadline case. The bounded receipt `build/candidate/t/43533993/terminal-product-role-last-run.txt`, exclusive run root `terminal-product-36032-52066750-0`, case 16 records `disposition=timeout; reason=timeout; created=0; stopped=0; elapsed=63` and `preparation exceeded child deadline`. The tested executable hash before/after was `99a96a09d2369ebe146f0bd536c0855166ec40f8614220ba0116522bcd43b0ed`. `build/candidate/Testing/Temporary/LastTest.log` records seven cases passing and the sole failure at the HarnessFailure expectation in this deadline case.

The existing classifier deliberately returns `Disposition::Timeout` for `StopReason::Timeout`. The correction changes only that one test expectation from HarnessFailure to Timeout; the no-child, prepared lease, sticky timeout and bounded elapsed assertions remain. Both supervisor file hashes remain unchanged. The source table above contains the corrected test hash. Static Testkit preflight passed again; no worker build or rerun was performed. Root must rerun the corrected focused target before acceptance.
