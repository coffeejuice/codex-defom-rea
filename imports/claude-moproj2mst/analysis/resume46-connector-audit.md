# Checkpoint 46 connector and project lifecycle audit

## Finding

The connector correction has a credible source-level contract and bounded parser regression coverage, but it does not establish whole-project Open/Save parity. The 16-case original-function probe runs 241 bytes at `0x140074f50` with mocked type and owner-index callbacks. It validates admission branch order and link-state handling only. In particular, it does not execute original type resolution, GUI parsing, serialization, or the project lifecycle.

The static type contract supplies independent evidence for four representative classes and seven parser fixtures. The implementation review additionally finds that two registered classes, Cycle and DoeOutput, needed restored dimension flags; source now records `0x301b` and `0x20097`, and a two-case real-registry/parser test was added. This closes the identified initializer gap in source, but the review explicitly says its retained fixed-v2 build/test receipts predate those two cases. The current checkpoint46 integration must create a build/test receipt tied to the final source identity before treating those cases as regression-checked.

## Scope boundary

There are three distinct evidence levels and they should stay separate:

1. **Native admission probe:** 16 controlled combinations for the original admission function with synthetic callbacks. Narrow native branch evidence, not an end-to-end oracle.
2. **Converter connector tests:** the 4x4 representative matrix, link ordering/occupancy cases, conversion chain, and the Cycle/DoeOutput registry cases exercise converter code and registry initialization. This demonstrates the bounded parser contract, subject to a final-source-bound build/test receipt.
3. **One-project lifecycle acceptance:** open a fresh project in a fresh-equivalent original session, save all outputs (including generated study/DOE artifacts where reachable), close it, and compare internal decisions/statuses plus filesystem effects and cleanup. No retained connector receipt establishes this. `takeover-state.json` expressly says `whole_project_original_parity: false`; the implementation objective remains one-project Open-Save-Close.

The historical 93/98 full-save comparator total is qualified: not a fresh capture in this chat and not raw-byte equality. It cannot be promoted into connector-specific parity evidence.

## Source-supported remaining gaps

- **Final test binding:** run the focused connector contract test and required integrated regression against one frozen source tree that includes the Cycle/DoeOutput additions; retain source-tree hash, executable/test identities, command, result and logs. Existing `connector-admission-fixed-v2-build.json` is a prior build and cannot serve as this receipt.
- **Original registry closure:** the review audits 28 registered keys and flag initialization, but explicitly excludes unregistered `SfOprGroup` and `SfOprCopyMirror` and does not claim independent validation of all 28 original flag constants. Their original registration/lookup behavior remains a separate reachable-closure question. Unknown-class constructor fallback is not evidence that an unknown class opens successfully.
- **Serialization and lifecycle integration:** connector graph links must be carried through the actual project Save path, including ordering, partial failures, generated files, adapter status, and Close cleanup. Parser-only fixtures cannot establish those effects.
- **Fresh original comparison:** freeze a candidate and run original-first comparisons on disposable copied fixtures under the approved fresh-session boundary. Capture both connector-relevant project state and exact file/directory effects; retain raw differences and justify any normalization from source. Prior original GUI Open->Save->Close comparison for this patch is explicitly absent.
- **Failure and cleanup behavior:** integrate connector failures with recovered writer-specific ordering and normal Close/destructor/global-reset behavior. Do not infer transactional behavior or successful conversion from a test that only checks link admission.

## Next concrete integration and validation

At the next authorized integration checkpoint:

1. Reconcile the connector review against the current frozen source hashes; verify the current test actually invokes `makeRegistry` before parsing and includes both Cycle and DoeOutput bridges.
2. Run the focused connector contract test and the integrated converter regression once, serially with the root-owned build. Save a receipt tied to the exact source-tree hash and final test log. If it fails, repair and repeat before freezing.
3. Add or select a disposable ordinary Sub_Project fixture containing the two corrected classes and representative compatible/incompatible edges. Run it through production Open->Save->Close and compare the candidate output to a fresh original capture. Keep the original-function probe result labeled as admission-only.
4. Include at least one case whose Save emits a reachable study/DOE side file if that feature is in the selected project; compare file creation/order/failure and final cleanup under the same one-project boundary.
5. Record acceptance as connector parser correctness, project lifecycle comparison, and remaining registry/unknown-class qualifications separately. Do not call the result whole-project parity unless independent original-first lifecycle acceptance passes.

## Evidence consulted

- `analysis/connector-type-contract.md` and `.json`: four representative source-qualified classes, 4x4 matrix, seven fixtures, probe limitations.
- `analysis/connector-admission-original.json`: target PE hash, exact 241-byte function range, 16 synthetic-interface cases, explicit `original_gui_executed: false` qualification.
- `analysis/connector-implementation-review.md`: Cycle/DoeOutput source correction, registry audit, test coverage and explicit stale-receipt/full-project limits.
- `analysis/connector-admission-fixed-v2-build.json` and `analysis/wave1-connector-first-full-tests.json`: prior build/test history, not proof for the later two-case source addition.
- `analysis/takeover-state.json`: connector status `implemented_and_regression_checked`, probe count 16, regression count 7, and `whole_project_original_parity: false`; active objective remains full single-project Open-Save-Close.
- `IMPLEMENTATION_PLAN.md` and subtree `AGENTS.md`: approved scope, fresh-session qualification, disposable fixtures, continuous integration, and completion gates.

This audit is read-only with respect to converter source, build artifacts, reference applications, and REA databases. Only this report is added.
