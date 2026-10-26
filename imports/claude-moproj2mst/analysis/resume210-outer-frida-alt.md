# CP210 outer-loader Frida contingency — repaired, HOLD

The default-disabled function-boundary contingency now has source-level attribution repairs and a passing harmless fixture. It remains HOLD: both literal guards are false and `captureComplete` is unconditionally false. The selected hardware method in `resume210-outer-capture-plan.md` remains preferred. No original process or GUI control was used; no fixture, build, deployment or shared plan was modified. Work owns this report, the prepared runner and its new harmless fixture.

Applied the REA skill through retained qualified evidence and offline PE reads. Read AGENTS/TAKEOVER, bounded takeover/import state, CP207 successful two-hook runner/review, CP210 static/plan/independent Frida review, and CP209 headless-entry/independent byte-matched main review. Target remains original installation v11.0.2 DEF_GUI_MO_64.exe, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, null version resource per retained qualification. No reconstructed executable is involved.

## Observer and repairs

`admin/original_outer_loader_frida_probe.mjs` reuses CP207's exact target/helper/hash, fixture/project digest, fixed argv/cwd, suspended-child ownership, creation FILETIME text, one-resume contract, first-AV/core/backtrace capture, CP205 parity, natural exit window, raw helper retention, unchanged fixture check and independent Windows/Frida absence plus unload/detach checks.

The six entries are `1ca30`, `c78c10`, `c79390`, `146f8fc`, `44a90`, `8a9b0`; exact caller continuations are respectively `1c94`, `1cf11`, `1d08a`, `1d0bd`, `1d0d8`, `1d0fb`. The main call is `1c8f`, returning at `1c94`. Helpers record normal-return scalars only, including AL for acceptance; absence of leave is incomplete, never zero. Pairing requires ordered prerequisites through the first failure and a consistent outer result.

Every outer entry now inspects/poisons active selected frames BEFORE filtering its caller. Thus an unselected nested outer call cannot borrow the selected frame. Helpers additionally require entry RSP = selected outer entry RSP - `0x1e0`: the checked outer prologue subtracts `0x1d8` without pushes, then CALL consumes eight bytes. Pending calls, order errors, selected overlap and cross-thread attribution invalidate the occurrence.

The Project prefix now covers the complete 21-byte possible displaced window: `48894c24084881ec8800000048c7442448feffffff`. This consists of complete 5-, 7-, and 9-byte instructions. Offline byte identity is established; pinned Frida patch/trampoline extent, incoming interior targets, shared RTTI behavior and native neutrality remain independent release requirements.

Active state is capped at 16 threads, eight frames per thread and 64 frames overall. Reaching a state cap sets a persistent invalid latch included in the first-AV snapshot and host qualification. Emission remains bounded to eight events per kind and 56 total. Helper transport now has a cumulative 64-KiB cap and stops buffering after overflow. Host retains helperError and transport status explicitly. The actual completion prerequisite helper requires no helperError (including late errors), no partial line/overflow, drained stdout, natural AV exit code `0xc0000005`, all capture/identity/parity/cleanup gates and raw receipts. The unconditional incomplete guard remains separate.

## Runtime path claim boundary

A decoder is not needed to identify the **first prerequisite of the fixed owned CLI Open invocation**, provided exact command/cwd/fixture identity, the corrected caller, a unique primary-thread occurrence, the main-local pointer relation and callback/frame parity qualify. CP209 independently byte-matches main: it selects `f`, converts to a QString local at main RSP+1918, then directly supplies that local as RDX and the main at RSP+40 to `1ca30`. Host now requires entry TID = owned primary TID and raw RDX = raw RCX + `0x18d8`.

This is a qualified ordinary-flow attribution to the command invocation. It does not verify runtime QString characters, conversion/encoding or unchanged derived DB identity; no runtime B98 path-content equality is claimed. `runtimePathQualification` explicitly says `raw_qstring_address_only_content_unverified`, and `causalClaimScope` names the fixed CLI invocation. A future claim about exact selected filename contents still needs a statically qualified bounded Qt4 decoder and memory fixtures. No layout is invented from newer Qt versions.

## Harmless verification and remaining HOLD

`admin/test_outer_loader_frida_probe_fixture.mjs` extracts and executes the runner's actual generated callback code AND its actual host event/AV/outer-pair qualification code in a mocked Frida VM. It also extracts the actual completion prerequisite helper. The fixture passes 78 explicit top-level assertions, in addition to harness assertions.

Coverage includes all five failure prefixes, all-success, low-byte acceptance, missing leave, selected overlap, unselected nested outer at each stage, pending reentry, cross-thread/wrong primary thread, wrong frame/path-pointer relation, lost/reordered events, creation identity mismatch, first-AV counter mismatch, frame/event caps, natural exception handler `false`, late helper errors after resume/cleanup, partial receipts, overflow, wrong exit code and absent command attribution. This is callback/host semantics, not native Interceptor delivery or native exception disposition. The fixture does not launch any child, import Frida or open an original target.

`node --check` passed for the runner. `node admin/test_outer_loader_frida_probe_fixture.mjs` returned PASS. Exact-token invocation of the prepared runner returned PRELAUNCH_FAILURE at its false review gate before target launch. No original-ready receipt is claimed. Independent repaired-source/fixture review, six-window relocation/shared-RTTI qualification, and owned harmless native exception/cleanup failure-injection qualification remain required. Original execution remains disabled.

Runner SHA-256: `9e584f00ad1b6eeef6a384e069cf6d3e075060a6de062220488d5fcf829cbe58`.
Fixture SHA-256: `69eeb32bb3d581c8243f16edae7c547674538f53c86a95941972c9c3802d6d82`.
Pinned unchanged helper SHA-256: `8f770388862580700063254663ac029eaf45ee269a3528aec3e61f946c32411d`.

## Pre-resume five-byte redirect gate

The current injected source flushes all six installed Interceptors and rereads each entire verified prefix while the owned child remains suspended. It permits only an `E9 rel32` at bytes 0..4 with bytes 5..end exactly unchanged. Each readiness receipt retains the pinned/previously verified before-prefix, observed after-prefix, signed displacement and decoded entry redirect destination. No destination contents or private Frida trampoline are read. A mismatch/read error emits `install-readiness-failure` with all six receipts and throws before handler-ready.

The host explicitly records that failure, commands termination through its exact owned helper, rejects readiness and never sends resume. An independent `patchReadinessQualified` helper rechecks all six raw receipts, address/base/RVA, prefix suffix, signed displacement and decoded destination before the normal resume statement. The completion helper also requires the host gate and absence of any install failure. A rejection can arrive during script.load, so its promise has an immediate rejection observer while preserving the later awaited error.

The actual-code mock now passes 78 explicit top-level assertions, including non-E9 and suffix-change rejection at every entry, missing readiness, malformed destination, missing receipt and late install failure. The native exact-prologue gate fixture passed nine trials, including real bad-suffix rejection before resume. See `analysis/resume210-native-patch-gate-fixture.md`. Both production guards remain false and completion remains unconditionally disabled. The gate rejects longer redirects rather than assuming their relocation works.
