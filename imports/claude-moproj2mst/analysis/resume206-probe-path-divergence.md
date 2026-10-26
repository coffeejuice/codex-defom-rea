# CP206 hook-path divergence audit

## Question and scope

This is a converter-side, read-only comparison of the retained CP205 successful crash-stack capture and CP206 incomplete null-owner probe receipt. No REA session, application process, GUI, or runtime was used for this audit; no source or raw evidence was changed. The question is what the four zero per-site counters at CP206's first AV establish, and what evidence is needed to tell whether the intended CP205 call path was reached.

## Evidence compared

- CP205: `analysis/resume205-original-crash-stack-2026-10-26T08-58-00-325Z/summary.json`, qualified by `analysis/resume205-original-crash-capture-qualification.md` and interpreted in `analysis/resume205-rea-crash-chain.md`.
- CP206: `analysis/resume206-original-null-owner-2026-10-26T09-46-02-454Z/summary.json`, `helper-raw.jsonl`, `null-owner-observers.js`, and `runtime-identities.json`.
- The frozen hook coordinates and intended pairings: `analysis/resume206-null-owner-hook-spec.md`.

Both runs used installed original `DEF_GUI_MO_64.exe` v11.0.2 with SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, the same `-m open -f C:\PROBLEM\p202a\Gear_blank.moproj` command, cwd `C:\PROBLEM\p202a`, and the same 27-file fixture content digest `55e2a58cf8155995f942f89ac0ebb91454308b521ae76a228edec0ffdc4cedbd`. CP206's before/after fixture digests match. The runs are separate process occurrences (CP205 PID 31656; CP206 PID 32740), so same image/profile does not prove identical execution path.

CP205 retained a qualified AV payload: QtCore `QListData::size` RVA `0x32c0`, read address and `RCX` both `0x58`, natural exit `0xC0000005`. Its accurate app stack includes returns at RVAs `0x4af4`, `0xa7377`, `0xa7510`, `0xeb1ec`, `0xeaa7a`, `0xdafad`; static call-chain analysis ties `eb1ec` to call `eb1e7 -> a74f0`, and `a74f0` to the Qt size leaf. This is the selected crash path on which CP206's hooks were based.

CP206's handler-ready record says Frida 17.23.2 registered all four interceptors after validating loaded module identity. The runtime module base was `0x7ff648e40000`; at that base the hook addresses were `0x7ff648f2b00f`, `0x7ff648f2b013`, `0x7ff648f2b1e3`, `0x7ff648f2b1e7`, with expected bytes respectively `4c895838`, `c744245400000000`, `488b4938`, `e804c3fbff`. Registration completed. At first AV the handler reported global sequence `0`, all four per-site counts `0`, and all overflow counts `0`. The process then exited naturally with `0xC0000005`; helper cleanup, process-absence, detach, and fixture checks succeeded. However, the retained `scriptMessages` contains only `handler-ready` and `hook-counters-at-first-av`, not a `first-access-violation` payload. Accordingly `exceptionRecordQualified=false`, `captureComplete=false`, and `outcome=INCOMPLETE` are the correct limits.

## What zero counts establish

The counters are sampled by the registered first-access-violation handler. At that point, no observer callback at any of the four configured RVA addresses had incremented the sequence in PID 32740. The zero values are positive observations; they are not zero-valued hook operands, missing records mistaken for zeros, or evidence of cap overflow. They mean CP206 supplied no producer-site, post-store, consumer-load, or call-site operand data. Therefore CP206 cannot distinguish a null producer return from a later field transition or holder replacement, and it cannot support a same-frame producer/consumer pairing.

The handler-ready record plus expected bytes establishes that the script resolved the expected module-relative addresses and that the bytes matched before resume, with interceptor registration calls completed. It does not establish that execution reached the sites, that callbacks would have been delivered if reached, or that the recorded first AV arose from the CP205 path. The counts cannot diagnose hook delivery/failure because there is no independent evidence that the process executed any hooked instruction.

## Different-fault-path possibility

A different first AV path is possible and remains unresolved. CP206 has no retained AV RIP/module/accessed-address/RCX/backtrace payload in `scriptMessages`, so it cannot be compared to CP205's `QListData::size` AV. Natural exit code `0xC0000005` establishes an access-violation process exit, but does not identify the exception site or stack. The equal binary, command, cwd, and fixture make the CP205 path a relevant hypothesis, not a proven identity of occurrence.

If a complete CP206 AV payload were recovered and matched CP205's Qt module/RVA `0x32c0`, read `0x58`, `RCX=0x58`, and app returns including `0xeb1ec` with the same relevant static caller chain, then zero counts would indicate that this occurrence reached an AV on a stack apparently containing the consumer return address while the instruction hook callbacks still recorded no visits. That would be a genuine hook-delivery/path inconsistency requiring focused review; it still would not, by itself, prove the hooks failed. The stack return could also be misleading if the exceptional control flow or unwind evidence differs. The current raw receipt does not permit even this conditional comparison.

Conversely, if a complete AV payload identifies another module/RVA, different access, or backtrace lacking the CP205 selected chain, then zero counts are consistent with the run taking another path that never reached these four sites; they say nothing against the hook mechanics on the intended path. If the fault address matches but stack is missing/ambiguous, fault-site equality alone is not sufficient to bind the fault to `eb1e7 -> a74f0`.

## Exact next evidence to discriminate

First inspect the original CP206 raw probe outputs (including any retained launcher stdout/stderr or alternate raw event file, if present) for the emitted `first-access-violation` message. The currently retained summary shows it was not included. Do not reconstruct its stack from the exit code or counters. Recover at minimum the exception type, fault RIP and containing module path/base/RVA, accessed address and operation, `RCX`, accurate backtrace with module identities/RVAs, and disposition. Preserve the raw message and qualify it against CP205's target identity and exact fault-chain markers.

If the exception payload cannot be recovered, the current run is irreducibly incomplete for path discrimination. A future separately authorized, reviewed capture would need to retain the AV message reliably (and any callback/host serialization error) alongside the counters; at the AV, retain the same minimum fault payload. This is a requirement for evidence quality, not a recommendation to rerun automatically.

Only if the AV is qualified as the selected CP205 call path should hook-delivery diagnosis proceed: establish in one run, with the same process creation identity, that the accurate AV stack contains the intended `eb1ec` caller context and verify callback visitation/counters for those exact sites. A captured site callback must retain its pre-instruction raw registers and sequence. Then pair `eb00f` with `eb013` and `eb1e3` with `eb1e7` by PID, exact creation FILETIME, TID, frame `S`, `O`, `H`, sequence, and loop index as specified in the hook contract. If the selected fault path is present but callbacks remain zero, preserve that as a hook/path-delivery discrepancy and investigate registration/patch/exception timing using bounded evidence; do not label it producer-null, field-clear, or hook failure from these counters alone.

If callbacks are present, the completed same-frame pairs provide the producer/field/holder discrimination described in the spec. No source-level converter behavior follows merely from reproducing an AV. The existing capture also does not resolve Save, accepted Close, or the public m2m behavior contract.

## Conclusion

CP206 proves expected hook-address resolution, expected pre-resume bytes, completed registration calls, zero callback visits by the time of its first observed AV, and natural AV exit with verified cleanup. It does **not** qualify the AV's location or stack and therefore does not show whether the crash followed CP205's `eb1e7 -> a74f0 -> Qt size` path. Both an alternate earlier fault path and an intended-path callback-delivery discrepancy remain open. The discriminator is the complete CP206 first-AV payload; absent that payload, the capture remains incomplete and causal classification stays indeterminate.
