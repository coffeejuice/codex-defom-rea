# CP210 pre-resume five-byte redirect gate — mock and native fixture PASS

The default-disabled original runner now verifies its actual installed redirect mode before it can resume a held child. All six entries must contain a five-byte `E9 rel32` and an unchanged suffix from byte five through the entire verified prefix. The injected and independent host checks both require this. Unsupported redirects or read errors produce an explicit installation-readiness failure and owned-child termination, with no resume.

No original or GUI ran. Both original guards remain false, and `captureComplete` remains unconditionally false. Original runner SHA-256: `9e584f00ad1b6eeef6a384e069cf6d3e075060a6de062220488d5fcf829cbe58`. No private Frida trampoline or destination contents are inspected by this gate or its new native fixture.

## Implementation and harmless mock

The injected gate runs after all six attach calls and `Interceptor.flush()`, before any handler-ready receipt. It records name/RVA/address, before/after bytes, `E9 rel32` type/size, unchanged suffix, signed displacement and decoded redirect destination for every entry. A failed site does not stop gathering the other bounded receipts. Any failure sends `install-readiness-failure` and throws, preventing handler-ready. The host handler immediately requests termination from its held child's existing helper, rejects the readiness promise and cannot reach resume. A rejection observer prevents a rejection during script.load from becoming unhandled before the subsequent await.

The independent host function recomputes all byte/address/displacement/destination checks from the readiness receipt. The completion prerequisite requires this host result and no install failure, including a late recorded failure. It does not accept the injected boolean alone.

`admin/test_outer_loader_frida_probe_fixture.mjs` executes the actual generated callback/gate code and actual host qualification helpers. PASS: 78 explicit top-level assertions plus harness assertions. New cases reject non-E9 redirects and changed suffixes at EACH of the six sites, require explicit failure and no handler-ready, and reject forged destinations, missing receipts and late install failure. Fixture SHA-256: `69eeb32bb3d581c8243f16edae7c547674538f53c86a95941972c9c3802d6d82`.

## Real native exact-prologue gate fixture

New `admin/test_outer_loader_native_patch_gate_fixture.mjs` SHA-256: `eea1f09fc940193ae372de35e5e593582baf22fe9d12fce4b7012a4dca6e4854`. It reuses the exact-prologue synthetic executable from `scratch/cp210p01`, SHA-256 `6f1ab2fa4bdf47a80ceda881f293e522333fae551a8c30a9e1d15600b473a83c`, and extracts the CURRENT production injected source plus host patch validator. Its only ordinary observer changes bind the synthetic identity/module size and six synthetic hook/caller RVAs/prefixes. The native fixture has no user-selected target/PID interface.

Nine fresh disposable owned children passed: handler-only baseline, five failure routes, hooked all-success, intentional post-readiness/pre-resume failure, and deliberate installed-suffix corruption before readiness. The baseline alone disables all entry hooks and skips the production installation rejection, recording hostPatchGate=false; it is an explicit fixture-only handler reference, never a production fallback.

The six ordinary hooked trials and the post-readiness failure each passed the actual generated five-byte gate and the actual independent host validator. Each ordinary resumed child passed native RSP/register/return checks, retained the same first-AV module RVA/write-at-58 as the handler-only baseline, exited naturally with `0xc0000005`, and had verified cleanup/absence. Their event/count snapshots remained complete and sequential.

For real gate rejection, the fixture uses `Memory.patchCode` only on its disposable synthetic Project entry at byte +5, changing the suffix to CC after Interceptor install but before the gate. The gate records Project=false and the other five=true, emits `install-readiness-failure`, and never emits handler-ready. The host immediately requests helper termination; no resume receipt, hook callback or AV is present. Cleanup verifies one owned termination with exit `0xe201`, closed handles, Frida detach and absence through independent Windows and same-device Frida enumeration. This is a real installation-failure boundary on the held synthetic child, not a fabricated readiness message.

Raw helper JSONL, Frida messages, generated agents, identities, per-trial and final summaries are retained in `analysis/resume210-native-patch-gate-2026-10-26T12-27-02-390Z`. A separate read-only retained-evidence audit confirms all six accepted receipts' E9 bytes, exact suffixes, signed destinations and every child's cleanup/absence, plus the rejected Project receipt and missing resume.

Syntax checks passed. Native result: PASS 9/9; firstAvParity=true. The approved escalation cited the user's bounded child-test authorization (“Yes, run the child test”) and no-DEFORM scope. Tool-reported execution wall time was about 4.90 seconds; no comparative efficiency claim is made.

## Scope of this pass

This closes the hypothetical fourteen-byte redirect gap by refusing such a live install before resume. It does not force or validate a fourteen-byte fallback, inspect private invoke trampolines, or claim their continuation. E9 destinations are decoded and retained without reading their contents. Successful synthetic behavior cannot establish original shared RTTI neutrality or CP205 parity. Independent repaired-runner review and an explicit original release decision remain necessary; neither production guard was enabled.
