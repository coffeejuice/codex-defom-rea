# CP130 efficiency review (CP127–129 receipts)

This review compares retained, source-bound checkpoint receipts. CP130 has no frozen build or full-run receipt in the retained record yet, so its preflight cost cannot be reported as a measured result. The safe CP130 preflight plan is to run the static Testkit/include preflight first, then one grouped focused build for every affected target on the frozen source and unchanged CMake cache, followed by the unchanged full build and full launcher. That keeps early diagnostics cheap while ensuring the complete build and all acceptance bindings still run.

## Measured checkpoint results

| Checkpoint | Configure | Focused preflight | Full build | CTest | Full launcher | Suite result |
|---|---:|---:|---:|---:|---:|---|
| CP127 | 2.109 s | 0.734 s | 23.234 s | 13.516 s | 30.265 s | 313 registered: 306 ordinary pass, 6 gap-qualified, 1 diff-qualified |
| CP128 | 2.125 s | 0.797 s | 23.266 s | 13.453 s | 30.562 s | 314 registered: 307 ordinary pass, 6 gap-qualified, 1 diff-qualified |
| CP129 | 2.109 s | 0.781 s | 126.937 s | 13.406 s | 30.312 s | 315 registered: 308 ordinary pass, 6 gap-qualified, 1 diff-qualified |

Each full CTest run exited zero, with no failed, skipped, or incomplete tests; the accepted validation still records six gap-qualified and one diff-qualified test. This is historical/candidate evidence, not end-to-end original-session acceptance. Tokens were unavailable in these receipts.

CP127–129 CTest varied by only 0.110 s across three checkpoints, while the full launcher varied by 0.297 s. Coverage increased by one registered/ordinary passing test per checkpoint. The fixture hashing configuration stayed SHA-256 over 16,728 files (~1.868 GB), threaded with four workers and parent-cache paths; source and binary fingerprints were bound before/after and fixtures were unchanged. Thus the stable ~30.3 s launcher already includes meaningful complete evidence capture; no receipt supports reducing hashes or cases.

The CP129 full build is the outlier: 126.937 s versus ~23.25 s in CP127/128. CP129’s own configure and focused preflight remained ~2.1 s and ~0.8 s. Its broad public `model.hpp` dependency change legitimately invalidated many objects, and the final target completed successfully. Earlier CP129 preflight attempts found compile/link defects and required repair attempts (33.129 s first attempt, then 6.205 s failed repair and 6.577 s successful repair). Those are diagnostic-rework costs, not evidence that the focused preflight or CMake configuration itself regressed. The CP128 two-/three-lane preflights (6.501/5.837 s) also show that grouped multi-target work can take several seconds when it actually has work; they do not prove a general speedup over a no-op ~0.8 s final preflight.

## CP130 recommendation

Keep the preflight sequence explicit and source-bound: run the static entrypoint/quoted-include check for newly added tests; after integration and freeze, build the complete CP130 affected-target set in a single focused-preflight invocation; then run the full build on the same source fingerprint and CMake cache; finally run the full test launcher with its existing source, binary, and complete fixture hashes. Do not accept a focused build as a replacement for full build or regression.

The one low-risk optimization is earlier targeted static preflight of each newly authored test before grouped compilation, because CP129 demonstrates that Testkit syntax errors can evade a basic entrypoint/include checker and waste an entire build attempt. Extend/use that checker only for deterministic syntax properties it can prove (including identifier-only `TEST_CASE` names); retain the grouped focused build as the actual compile/link check. Record preflight elapsed time and failed build attempts separately. This should reduce avoidable rework if it catches a known class of mistakes; the present receipts do not quantify the saving, so treat it as a proposed measured experiment, not an achieved speedup.

No evidence supports a configuration change, a reduced fixture/hash set, fewer full-suite cases, or a 10x claim. The validation objective remains incomplete pending the separate acceptance gates.
