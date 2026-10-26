# CP214 activation entry relocation: independent review

**Static patch-window interpretation: PASS. Original-run release: HOLD.** The copied original installation v11.0.2 `evidence/original-binaries/DEF_GUI_MO_64.exe` has SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; its PE version resource is null. This report reviews copied-PE bytes, the synthetic held-child pre-resume receipt, and the runner's existing predicate. It does not report an original execution, normal activation return, or validated nine-byte trampoline. No runner, source, original process, GUI control, mutable REA session, or shared state was changed.

## Instruction coverage and observed patch

Direct copied-PE bytes at `a7250` are `89 54 24 10 48 89 4c 24 08 48 83 ec 38`. DUMPBIN 14.44.35228.0 decodes offsets `[0,4)` as `mov [rsp+10h],edx`, `[4,9)` as `mov [rsp+8],rcx`, and `[9,13)` as `sub rsp,38h`. The second instruction is indivisible. A five-byte E9 overwrites all of the first instruction and one byte of the second; a safe trampoline must relocate both full instructions and continue at `a7259`, the `sub rsp,38h` boundary.

The synthetic CP214 fixture uses the same 13 entry bytes. In `analysis/resume214-native-postopen-2026-10-26T13-11-46-536Z/trial-5-false.json`, Frida 17.23.2 changed its activation entry from `8954241048894c24084883ec38` to `e946fcfeff0f1f40004883ec38`: a five-byte E9, exact four-byte NOP `0f 1f 40 00`, then the original suffix from byte 9. All other eight hooks in that receipt have a five-byte E9 with suffix unchanged from byte 5 and pass the old gate. The activation receipt has `suffixUnchanged=false` and `gatePassed=false`, so the existing injected gate refused readiness. The trial has no resume receipt or hook events. Its held child was intentionally terminated; helper exit 0, closed handles, independent PID absence, Frida absence, and detach are recorded. This is a correct pre-resume stop, not a failed original Open.

## Sufficient activation-specific gate

The amended gate must recognize one exact installed shape while retaining the strict old shape for the other eight sites. For activation only, both the injected readiness gate and independent host predicate should require:

1. The pinned 13-byte original prefix at loaded image `module.base + a7250`, with exact module/process identity.
2. `after[0:5]` is E9 plus little-endian signed rel32; report `redirectSize=5`. Recompute `address + 5 + displacement` independently on the host from the raw bytes and compare with the agent's destination.
3. `after[5:9]` equals the exact observed NOP `0f1f4000`, and `after[9:13]` equals the original `4883ec38`. Report a nine-byte displaced/patch footprint separately from the five-byte redirect. Do not relabel the full original-prefix suffix as unchanged.
4. No alternate NOP, changed byte 9 or later, wrong E9 destination, wrong site/order, or changed prefix at any of the other eight hooks passes. A different Frida patch shape needs new review before resume.

The entry-byte gate cannot inspect a private Frida trampoline merely by decoding the E9 destination. Before original release, run the **actual amended agent and host predicate** against the owned native fixture. Require a resumed nonnull-current occurrence with filtered activation entry and normal leave, normal postOpen return, stable expected Project/index and frame identities, register/stack sentinel preservation, no observer error or AV, and verified child cleanup. Also retain the fixture's null-current normal-return and activation-enter/no-return cases so the observer does not classify those as activation completion. The fixture establishes mechanics for this Frida/runtime/prologue combination; it does not prove original Open behavior.

The current `admin/original_postopen_frida_probe.mjs` still checks `suffixUnchanged` from byte 5 for every hook in both its injected gate and `patchReadinessQualified`, so it cannot yet authorize this activation shape. The synthetic pre-resume receipt proves the needed exception is narrow. Review and freeze the changed runner and native result before any original child is created.
