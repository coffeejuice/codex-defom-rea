# CP109: earlier writer failures and failed terminal backend

The existing opt-in remesh conversion worker now covers all eight reserved conversion cases17..24. The four additions exercise actual conversion prefix state and failure containment using existing private interfaces; production/core/harness/public ABI/CLI code is unchanged by this lane. The constructed B133 remesh overlay and retained historical raw oracle keep their CP107/108 qualifications. No original process, handler prelude or native Close is executed or claimed.

Frozen source: `source/apps/moproj2mst/tests/s/test_terminal_remesh_worker.cpp`, SHA-256 `dcae996e49c84d8159bf178e975c826cce36ee0a80b31b610caa9eb64f6b110d` (727 lines). Target `m2m_s_terminal_remesh_worker`; CTest `m2m.s.terminal_remesh_worker`. Eight Release-active Testkit cases, one parent/explicit-child main. Root builds and runs it; only static preflight was performed here.

## Source order and actual reached state

The new `ProjectFailureFactory` delegates real project writes/flush/close to `mst::FileDevice`. Its owned device forwards the actual aboutToClose event and records open→aboutToClose→returned close→post-close error query exactly once. The sampled native error must be0 before injecting or accepting the selected declared result. This uses the existing source-private per-call factory, already exercised in `test_save_remesh_failure_matrix.cpp`, rather than replacing conversion or fabricating a SaveCallState. `moproj/write.cpp:596` samples `error()` only after `saveProjectFileBody` has closed the device; `core/convert.cpp:346..355` commits project attempted/returned/succeeded, then properties and MST run before remesh at430..441, with project/MST false-status adapter errors deferred until afterward.

| Added case | Injection and retained effects | Required boundary/stop observation |
| --- | --- | --- |
|19 `ConversionDeniedProjectFalse` | Real project open/write/close succeeds with native error0; the private device declares post-close error7. All later property/MST writers run normally, and the independently held primary remesh lock denies the enabled writer. Full five historical raw prefix outputs must still match. | Started+Boundary, processC0000417, no supervisor stop; exact live SaveSnapshot clears only projectSucceeded (A/R remain true). |
|20 `ConversionDeniedMstFalse` | Actual project aboutToClose callback, after Open and Save2, overwrites the owned main MST with `owned late MST writer-denial sentinel\n` then acquires GENERIC_READ/shareREAD/OPEN_EXISTING. This child-owned handle outlives the conversion call. The real main MST write-open fails, before generation; project result remains successful. Remesh still runs and hits its separate parent-held denial. | Started+Boundary, processC0000417, no supervisor stop; exact live SaveSnapshot clears only mstSucceeded (A/R remain true). Main MST is exactly the sentinel; four earlier raw side outputs still match their pinned oracle. |
|23 `ConversionPublicationFailure` | At the real remesh dispatch, record actual reached SaveSnapshot, then deliberately return private `PublicationFailed` without attempting Boundary publication. The root bridge invokes failClosed with its captured same snapshot; backend publishes explicit tag1 and the leaf parks. | Started+BackendFailure, no Boundary/wire Save/Returned; sticky BackendFailure, confirmed supervisor-owned stopE1050001, HarnessFailure. Never terminal success. |
|24 `ConversionEndpointReturned` | Publish the actual remesh Boundary, then the selected test endpoint deliberately returns `EndpointReturned` instead of invoking TerminateProcess. failClosed publishes explicit tag2 and the leaf parks. | Started+Boundary+BackendFailure, retained exact Boundary SaveSnapshot, no Returned; same sticky BackendFailure/owned stop/HarnessFailure. Never terminal success. |

Case23 is a **declared backend publication failure**, not a physical broken-pipe observation. Intentionally breaking the only protocol pipe would prevent reliable BackendFailure transport and test a different stop route. Case24 is a deliberately returning candidate endpoint. Their source-qualified contract is the existing enum/service return→failClosed→park control flow; neither claims a native returning invalid-parameter handler.

Expected all-success snapshot flags are1015 (all ten defined flags except modalPending), checkedSubprojects1, raw answerNo65536, pendingDefaultNoButton0. Case19 flags951, case20 flags503. Private backend ledger now records event, profile reads, dispatches, failures and every SaveSnapshot field. A terminal endpoint has exactly one ordered dispatch record1/1/0. Each backend failure must have exactly dispatch1/1/0 followed by failClosed1/1/1 with identical actual state. Case23's exact snapshot is asserted from this private value ledger, without inventing an absent wire Save. Case24 also asserts the retained wire Save and tag2.

The MST-shortcut side-file assertion follows actual source timing: Object/ObjProperties/SimCtrl Save2 files precede the project writer; Boolean.BWZ is written in the property loop before main MST open. `engine/boolean_editor.cpp:97..139` writes BWZ solely from unchanged BooleanData fields, so suppressing its repeated MST-generation call does not change the earlier BWZ bytes. All raw files remain observed in the full manifest even when no equality oracle is asserted for that path. The original historical expected MST remains retained; the case20 effective expected prefix additionally records the exact sentinel bytes, without normalizing or discarding the intentional difference.

## No unwind and ownership

A child-local `UnwindMarker` now spans the actual conversion call. Its destructor writes a separate candidate-stack marker. Normal/Unknown/Custom returns require both this marker and the adapter-return marker; cases18/19/20/23/24 require both absent after confirmed stop. This directly checks that the opted-in child stack did not unwind through ordinary continuation on the terminal/parked paths. It is not native Close or proof of native destructor parity.

Every case still starts from a fresh independent 64-file before fixture plus two controls, validated by the full CP107 canonical manifest identity. The parent PreparedCase continues to own the primary physical denial through confirmed child stop and full stopped-case inspection. The additional case20 MST handle belongs to the child factory and remains live while dispatch/park occurs; process stop retires it through OS handle cleanup. The supervisor still validates this own-image role, nonce, job, finite case and protocol bounds. No service or raw pointer escapes into public state.

Before assertions, stopped-case inspection saves complete before/after manifests, every raw difference with before/available-after/missing/error bytes, pinned historical expected bytes, effective expected prefix bytes and all available actual prefix bytes. Parent receipts now retain explicit failureTag. Sticky BackendFailure and supervisor stop status are required for23/24, whereas ordinary terminal cases require StopReasonNone and no requested supervisor stop. The same short-path containment/reparse checks and resolved owned-leaf cleanup remain in force; no original fixture or unrelated process is modified. A failure to establish the requested C:/PROBLEM path remains a failure, with no fallback or escalation.

## Static receipt and remaining limits

All new/existing quoted includes resolve against this test's source directory or actual app/testkit include roots. Only CHECK, CHECK_EQ and REQUIRE are used; no C assert or REQUIRE_EQ. Eight TEST_CASE entries and one main; conservative string/comment-stripped delimiter scan balanced; zero trailing whitespace. No compiler, build, test executable, worker, original, GUI or REA operation ran in this lane. Static checks do not establish passing runtime outcomes.

Inspected dependencies (not edited):

| File under `source/apps/moproj2mst` | SHA-256 |
| --- | --- |
| `src/moproj/project_output_device.hpp` | `586132f1b480112b5b3d4152b35aca508114fe65a93e786d0b11d2ba8cbec669` |
| `src/moproj/write.cpp` | `8af4be80324c39e1f8f452d7723bebd200e4b7372180d8e32c1e6ee32c2399a2` |
| `src/core/convert.cpp` | `71fe67b9c6be7bde83001e663aa7c50f01c4a6421f357c0db893a5df0a0bd632` |
| `src/engine/simcontrol300_remesh.cpp` | `a865bf7aead8ce3b74f7d670340125538cf7903381b9335d851f55dd2bf4c228` |
| `src/engine/boolean_editor.cpp` | `764db1428155c2770423f23a854354620383cb3e2a65b14f7659b437f117dd6e` |
| `tests/s/test_save_remesh_failure_matrix.cpp` | `c45745562a68e98a61f08b0fb9e727c393738ef644fac5a6c375564095c11878` |

Pipeline specialist confirmed the existing V2 collector permits BackendFailure before or after Boundary, false Succeeded bits with A/R true, and sticky owned stop; no harness support change is needed by this test. The separate pipeline lane may add focused protocol regressions. Root must verify build, all eight real worker cases, raw prefix equality, fixed terminal status, declared-failure own-stop status, stopped lock receipts and cleanup before acceptance. Physical pipe publication failure, actual native handler prelude/Close, arbitrary failing devices, and fresh historical enabled-remesh parity remain outside these observations.

Only the assigned existing test and this unique report were written; both are frozen for root integration.
