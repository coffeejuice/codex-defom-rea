# CP214 postOpen runner: prepared, HOLD for independent review

The new disabled `admin/original_postopen_frida_probe.mjs` observes the synchronous boundary after the loader's return: delivery to postOpen, newly-current Project, activation entry/normal leave, and postOpen normal leave. It is derived from the repaired frozen CP213 transport SHA-256 `bb88fb26717683c7c97627453b742d042ab0f1074388d54a98ed0772b7519318`; that file remains unchanged. All three literal guards are false. No original application, GUI input/control, C++ source, shared state, DOE data or installed executable was changed or run by this lane.

The intended original is installation v11.0.2 `DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, PE version-resource qualification null. This report involves no reconstructed executable. It prepares only the fixed fresh `C:\PROBLEM\p214a` clone documented by root in `resume214-fixture-preparation.md`. Source/clone full-entry manifest, 27 files, original project hash, runtime identities, no-link/path bounds and the exact 136-hex `p214a` CLI argument are checked before launch. CP213's modified disposable project is never reused.

## Instrument and interpretation

The original six outer/path/prerequisite hooks and actual QString reader remain in the agent. Three new function entry/leave hooks use the independently reviewed copied-PE prefixes/callers:

| Hook | RVA | Selected return site | Witness |
|---|---|---|---|
| postOpen | 2a0e0 | 1cae | Same main and nonzero outer-return P delivered, then normal void return |
| newly-current accessor | 45780 | 2aa72 | Same postOpen occurrence, normal returned Project |
| activation | a7250 | 2aaa1 | Same occurrence/current P, raw RCX/RDX and EDX, then normal void return |

Static review established postOpen entry RSP=S, body S-0x498, nested 2a9b0 entry S-0x4a0 and both selected helper entries S-0x4e0. Both agent and host gate the helper stack delta; neither assumes a fixed relationship between the outer and postOpen stack frames. PostOpen and activation are void: RAX is never interpreted as success for either.

The activation index is the **observed raw EDX**. Copied code loads it indirectly through 2a9b0's R8 pointer after intervening callbacks; this runner does not claim the source stack slot remains zero. Normal activation of subproject zero requires observed EDX=0, matching current/outer P, and normal activation leave. A normal current-null postOpen return is retained separately as a valid observation of the skipped activation branch. Different current P or nonzero index cannot satisfy `intendedProjectActivationComplete`. Entry without leave can retain `activationAttempted` while activation/postOpen return completeness stay false.

The agent retains ordered bounded events, primary thread/owner identity, single invocation/post identifier, reentry poisoning before caller filtering, selected callers, pending-call state and counter snapshots. Host validation independently checks raw pointers/index, event order, stack/caller relationships, snapshot parity, normal leaves and current/delivered equality. Unselected accessor calls are counted as rejected callers. Duplicated selected postOpen occurrences send an explicit error even after a first normal return; late errors cannot silently preserve qualification.

PostOpen return is the endpoint; outer return alone no longer ends observation. The monotonic deadline is 12,000 ms from immediately before the sole resume command, with natural exit, first AV and observer errors also ending observation. Save, accepted Close and full lifecycle acceptance remain false. The bounded nine-hook observation does not establish XML/P+55 success, admitted diagram binding, timer/receiver delivery, busy restoration or a fully usable Open.

## Reviewed amendment: activation displaces nine bytes

The first harmless native attempt correctly failed the original strict five-byte/suffix gate **before resume**. The activation prefix starts with a four-byte instruction followed by a five-byte instruction. Frida replaced the displaced nine bytes with an E9 rel32 and four-byte NOP; the remaining SUB was unchanged:

```text
before 8954241048894c24084883ec38
after  e946fcfeff0f1f40004883ec38
```

The failure receipt is retained in `analysis/resume214-native-postopen-2026-10-26T13-11-46-536Z`. Its child was terminated with 0xe201 before resume, handles closed and independent Windows/Frida absence confirmed. This failure was not erased or relabeled as an executed activation.

Root authorized a bounded gate amendment after independent static review. Both injected and host predicates now admit **activation only** with exact original 13-byte prefix, E9 at byte0, `redirectSize=5`, separate `displacedLength=9`, exact `0f1f4000` bytes5..8, unchanged suffix from byte9, exact address identity and independently recomputed signed rel32/destination. The other eight sites still require `displacedLength=5` and unchanged suffix from byte5. No arbitrary padding, larger redirect or alternate hook site is admitted. Native normal entry/leave/register/stack witnesses qualify the synthetic relocated execution mechanics; public readable code does not expose every private Frida trampoline implementation detail.

## Owner and cleanup

The new CP214-only helper `admin/cp214_owned_suspended_process.py` differs from the old helper only in the accepted upper bound 20→30 seconds and its matching validation error text. Its SHA-256 is `0e5389f7dd3f734557d87725e8dad422a3daafc507b230f060fb900da5c14edf`. The old helper remains untouched. The 30-second independent watchdog includes setup, cannot be reset, and cannot satisfy a completed observation if it fires first. Native trials prove that the new helper accepts the requested 30-second bound; they do not claim to have waited through a full watchdog expiration.

Every command uses the inherited repaired idempotent command channel: one enqueued termination, one stdin end, writableEnded/finished/destroyed checks and retained stream errors. Held-child termination is not application Close. JSONL bounds/drain, signaled exit/closed handles, zero helper exit, script/session dispositions, successful Windows query with creation metadata and same-device Frida absence remain required. A present/reused PID or failed query is not absence and never triggers a broad kill. Before/after disposable manifests and source preservation are separate from observation completeness. No files are restored during cleanup.

## Harmless verification

Syntax checking passed. Actual-generated-agent/host mock tests passed: delivery mismatches, selected/wrong callers, nested/duplicate postOpen, current null/different, index mismatch, activation missing leave, late/deadline/AV endpoints, all nine bad redirects, activation padding/suffix rejection, host displaced-length/rel32 recomputation, missing/partial transport, watchdog and cleanup/absence rejection. The actual generated endpoint/finally statements and real Node Writable stream regression also passed, preserving the CP213 double-close repair. Receipts: `resume214-postopen-mock-receipt.json` and `resume214-postopen-stream-receipt.json`.

The harmless synthetic native fixture `scratch/cp214p02/fixture.exe` has SHA-256 `f8b0bfe8e64da5e6adb03350954d53b661e8a5476cfcb79c1b420fb8d15af842`. It contains the nine exact prologue shapes, sentinel nonvolatile-register checks, actual stack frames and a synthetic QString/header. Its synthetic target/Qt identity substitutions are explicit; it is not original application behavior evidence and its live continuation is a Sleep loop, not a Qt event loop.

Five native trials passed in `analysis/resume214-native-postopen-2026-10-26T13-13-39-106Z`:

| Case | Endpoint | Result |
|---|---|---|
| Current equals delivered P, activation returns, postOpen returns to caller | postopen-return | RCX=0x100, EDX=0, normal activation leave, helper stack delta 0x4e0, intentional 0xe201 |
| Current null, no activation, normal postOpen return | postopen-return | Current=0, no activation entry, intentional 0xe201 |
| Activation enters and stays in a harmless Sleep loop | deadline | Activation entry retained, no activation/postOpen leave, intentional 0xe201 |
| Early natural child exit | natural-exit | Code42 |
| Intentional rejection after hook readiness, before resume | pre-resume rejection | No resume, intentional 0xe201 |

All five retained complete raw owner JSONL and adapted agent/messages. Every child was signaled, handles closed, helper exited zero, stdout drained, session detached, and independent Windows/Frida absence verified. The activation-only nine-byte amendment passed actual installation and resumed native normal-leave checks. Installed Frida runtime access required approved sandbox escalation; every escalated command was the fixed synthetic test script accepting no arbitrary target/PID arguments. No original/GUI run occurred.

## Frozen review boundary

| Artifact | SHA-256 |
|---|---|
| `admin/original_postopen_frida_probe.mjs` | `54ce58f2cc0ca7451d37689ef490af98497137734d0f7a64be89c8cde2bba902` |
| `admin/cp214_postopen_agent.inc.js` | `fa4069997209af016cee1ddd5067ecde901e0662bee4630adab8eea5e9c32c53` |
| `admin/cp214_postopen_host.inc.js` | `47d771197fc96fb319eb1b55ad0d6152a95e4d645229300c1bd1d13a29132556` |
| `admin/cp214_owned_suspended_process.py` | `0e5389f7dd3f734557d87725e8dad422a3daafc507b230f060fb900da5c14edf` |

The new generator and tests are `prepare_cp214_postopen_runner.py`, `prepare_cp214_postopen_tests.py`, `test_cp214_postopen_frida_fixture.mjs`, `test_cp214_postopen_native_fixture.mjs`, `test_cp214_postopen_command_cleanup.mjs` and `build_cp214_postopen_native_fixture.py`. The standalone generated runner is the execution artifact; it asserts the frozen CP213 ancestor hash when regenerated. Source is held for independent exact-artifact review. All launch guards remain false, and this report releases no original execution.
