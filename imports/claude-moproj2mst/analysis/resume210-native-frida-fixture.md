# CP210 native six-entry Frida fixture — PASS, original release still HOLD

Seven fresh disposable native-child trials passed: five prerequisite failure routes, all-success, and one injected failure after hook readiness but before resume. Six normal trials delivered the expected function entry/onLeave observations, then naturally exited from a deliberate write access violation at address `0x58` with exit code `0xc0000005`. The injected failure never resumed its child and verified helper-owned termination. Every child has verified process exit, closed owned handles, Frida detach, and independent Windows plus same-device Frida PID absence receipts.

No original executable or GUI was launched, attached or controlled. The two original-runner guards remain false, and the original runner SHA-256 remains `41c2cd035bd023c3728b1dc73bb8ef167d13ae644bd11f4b1ecfcfc61790351b`. This is synthetic child qualification, not evidence of original six-window or shared RTTI patchability. There is no deployment.

## Artifacts and method

`admin/build_outer_loader_native_fixture.py` builds only fixed generated C/MASM sources into `scratch/cp210n03/fixture.exe` with MSVC x64 /Od. Builder SHA-256: `64822f7c5812a97d8e3f000964d44ad57cfdb54e6b383c51a07bebdbc74d6f0e`. Fixture executable SHA-256: `83746ed1a8e56712367d3b2d2f02a5244ccf3249898f926675ac960535f1f9cb`. Generated source hashes, export RVAs/prefixes, compiler commands and build logs are retained in that folder. Two preceding build-only attempts failed because MASM continuation labels needed public global scope; both logs remain in cp210n01/cp210n02. No target was launched by those failed builds.

`admin/test_outer_loader_native_frida_fixture.mjs` accepts no target/PID arguments and pins the fixed synthetic image/hash plus unchanged owned helper hash. Runner SHA-256: `16fa99f092695d1999eba691f3b220b7459bba8ca7b7e3f417ab814cc28b83f6`. It extracts the actual original runner's injectedSource function and remaps only synthetic module identity/size and six exported entry/prefix/caller RVAs. It uses real Frida 17.23.2 Interceptors against a fresh helper-owned suspended child. No source of the original runner was edited.

The assembly outer uses the same observed stack relationship (0x1d8 allocation, helper entry RSP at outer entry minus 0x1e0), direct exported continuation labels and synthetic input pointer relation. Each of the other five functions starts with 24 NOPs, then returns the selected scalar. These easy synthetic entry windows deliberately do not emulate the original Project or shared RTTI prologues. The labels factory/cast/project name analogous predicates only; no native object factory or runtime RTTI operation is being validated.

## Receipts

Full raw helper JSONL, generated agents, raw Frida messages, per-trial summaries, image/source identities and compact final summary are retained under:

`analysis/resume210-native-frida-2026-10-26T12-17-22-785Z/`

| Mode | Owned PID | Observations through normal outer return | Exit | Cleanup |
|---|---:|---|---|---|
| duplicate failure | 26268 | duplicate | c0000005 natural | verified/absent |
| factory failure | 27248 | duplicate, factory | c0000005 natural | verified/absent |
| cast failure | 18700 | duplicate, factory, cast | c0000005 natural | verified/absent |
| Project failure | 35724 | duplicate, factory, cast, Project | c0000005 natural | verified/absent |
| acceptance failure | 29172 | all five, acceptance false | c0000005 natural | verified/absent |
| all-success | 5008 | all five, nonzero outer return | c0000005 natural | verified/absent |
| injected pre-resume failure | 22072 | hooks ready, zero callbacks, never resumed | e201 owned termination | verified/absent |

Each natural trial records an initial outer entry, ordered normal helper returns and normal outer return, then the first AV core/backtrace record. The first-AV event/count snapshot matches all sequential received events, without overflow. A read-only audit of retained evidence additionally verifies exact helper callers, expected failure flags, primary TID, textual creation identity, no rejected callbacks, no state cap and no hook/script errors (`retained-evidence-audit.json`). The exception observer reports `return_false_continue_natural_exception`; the independently verified natural AV exit supports that the synthetic process's exception was not swallowed. It does not establish debugger first/second-chance delivery semantics.

The injected boundary throws after all six readiness records and before any resume command. Its one intentional failure is retained; cleanup records exactly one successful owned-process termination, exit 57857 (`0xe201`), closed handles and both absence checks. This tests one real failure boundary. It does not cover every attach/load/resume/context/cleanup timeout or late helper-error boundary in a native process.

The first sandboxed invocation stopped while reading the pinned external Frida package, before any child launch. The subsequent escalation was approved with the user's explicit bounded child-test authorization (“Yes, run the child test”) and exact disposable-child scope. The approved invocation completed all seven trials in about four seconds of tool-reported wall time; no speed or token-saving comparison is claimed.

## Remaining original release limits

PASS establishes real callback delivery for this synthetic six-entry assembly fixture, normal-return scalar ordering, first-AV snapshot delivery, one pre-resume failure cleanup and exact child absence. The actual-code mock fixture separately covers adversarial attribution and host qualification semantics. Neither proves original entry relocation safety, shared RTTI neutrality, incoming original interior branches, all original cleanup failure boundaries, or original CP205 fault parity under six patches. Independent original-window review and an explicit release decision remain required. Do not enable either original guard from this receipt alone.
