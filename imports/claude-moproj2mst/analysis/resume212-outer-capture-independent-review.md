# CP212 outer-loader original capture independent review: PASS

The retained capture supports **acceptance as the first failed outer-loader prerequisite of this fixed literal-argv owned CLI occurrence**. It does not establish a valid original B98 Open comparison. Duplicate lookup returned zero; factory, RTTI cast and Project accessor returned nonzero; acceptance returned AL zero; the same outer invocation returned zero normally. Its subsequent first observed AV matches the CP205 path. This is a PASS for that bounded observation, not a successful Open/Save/Close result or proof of instrumentation neutrality.

Only this report was written. No original process, GUI-control session, source edit, build, deployment or shared-state change occurred in this lane. Applied the reverse-engineer-anything skill's distinction between shipped-artifact, recorded runtime and inference evidence. Read subtree/root instructions, CP210 capture plan and final release review, CP211 window review, the restored observer, retained generated agent/helper records and root's `resume212-original-trial-release.md`. Root owns status scripts and integration. The earlier hardware capture design was superseded for this one trial by the final reviewed Frida method; this review does not claim that its debug-event protocol ran.

## Identity and release boundary

The target is original installation v11.0.2 `DEF_GUI_MO_64.exe`, not a reconstructed executable. The retained executable version-resource result is null; v11.0.2 identifies its installation. Independently recomputed both installed and copied target SHA-256 as `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

| Artifact | Independently verified SHA-256 |
|---|---|
| Restored disabled `admin/original_outer_loader_frida_probe.mjs` | `9e584f00ad1b6eeef6a384e069cf6d3e075060a6de062220488d5fcf829cbe58` |
| Released runner, reconstructed in memory | `cb038b92021f0599e83322a362b9e2402c8357cb95554d266889e7a2cad03707` |
| Retained `outer-loader-observers.js` | `442389ea800efe2be4c8147b96a58ca2f71c101748eaa32a188f8bfb3d2a6ee7` |
| `admin/owned_suspended_process.py` | `8f770388862580700063254663ac029eaf45ee269a3528aec3e61f946c32411d` |
| Actual-code mock fixture | `69eeb32bb3d581c8243f16edae7c547674538f53c86a95941972c9c3802d6d82` |
| Native patch-gate fixture runner | `eea1f09fc940193ae372de35e5e593582baf22fe9d12fce4b7012a4dca6e4854` |
| Native exact-shape `scratch/cp210p01/fixture.exe` | `6f1ab2fa4bdf47a80ceda881f293e522333fae551a8c30a9e1d15600b473a83c` |

The released-runner hash is reproducible from the current reviewed CRLF bytes by exactly three substitutions: both literal review guards false to true and the unconditional preparation `result.captureComplete = false` before `releaseBlocked` to true. No observer or qualification logic changes are needed to reproduce the captured hash. Current guards and preparation completion value are false again. Root's release receipt records one exact `--execute-reviewed-probe` command and immediate restoration; this lane did not witness that command live. No second original run is released.

`runtime-identities.json` records Node v24.19.0, Frida 17.23.2 and Python312 identities. This lane checked retained identities against capture references, not a new native Frida execution or an independent native-library provenance assessment.

## Fixture and live patches

The retained before/after/source manifests each contain 33 equal entries. Independently recomputed their JSON-entry SHA-256 as `55e2a58cf8155995f942f89ac0ebb91454308b521ae76a228edec0ffdc4cedbd`. `Gear_blank.moproj` is 10,932 bytes, hash `e769faf514b335d56ffbb22f2c76490d029f2714ada9b26a75496fbd62ade7fd`. Capture cwd is `C:\PROBLEM\p202a`, with fixed `-m open -f C:\PROBLEM\p202a\Gear_blank.moproj`. These retained manifests establish the capture's unchanged fixture boundary; they are not a claim about current external files.

Independently parsed the copied PE's section table and matched every full qualified entry prefix. Independently resolved each signed direct-call rel32: main `1c8f -> 1ca30`, continuation `1c94`; helpers `1cf0c -> c78c10 / 1cf11`, `1d085 -> c79390 / 1d08a`, `1d0b8 -> 146f8fc / 1d0bd`, `1d0d3 -> 44a90 / 1d0d8`, `1d0f6 -> 8a9b0 / 1d0fb`. Coordinates are EXE RVAs.

For base `0x7ff648e40000`, all six retained actual patches begin E9 rel32, replace the first complete five-byte MOV and preserve every later byte of the entire qualified prefix. Independently decoded displacement and recomputed destination/address/suffix equality, without trusting `gatePassed`:

| Entry | Prefix bytes checked | Actual redirect destination |
|---|---:|---|
| outer `1ca30` | 17 | `0x7ff648e30008` |
| duplicate `c78c10` | 17 | `0x7ff648e30208` |
| factory `c79390` | 14 | `0x7ff648e30408` |
| cast `146f8fc` | 23 | `0x7ff648e30608` |
| Project `44a90` | 21 | `0x7ff648e30808` |
| acceptance `8a9b0` | 17 | `0x7ff648e30a08` |

Handler-ready precedes the single resume in the retained stage receipt. Original RTTI pushes and Project SUB/store suffix remain in original code. Private trampolines, unwind registration and all unrelated shared RTTI callers are not independently observed; unchanged suffixes and same-path AV parity do not prove global neutrality.

## Raw sequence and attribution

Capture directory: `analysis/resume210-original-outer-loader-frida-2026-10-26T12-41-01-511Z/`. There are ten retained script message objects embedded in `summary.json`: readiness, seven hooks, compact first-AV core and enriched first-AV record. The seven hooks have exact sequence 1 through 7 and match first-AV snapshot counts, one per kind. No hook error, missing sequence, cap/overflow, invalid bounded state, overlap, order, owner or leave mismatch is reported. There are 214 rejected helper callers; this is observed unrelated instrumentation traffic, not 214 additional selected calls or a neutrality guarantee.

All selected records bind PID 29140, textual creation FILETIME `134374920623592835`, primary TID 3736, invocation `1` and main `0x7de010`. Outer entry/normal leave share entry RSP `0x7ddfc8`, caller `base+1c94` and QString address `0x7df8e8`. Every helper entry RSP is `0x7ddde8`, exactly outer entry minus `0x1e0`; each helper caller matches its independently verified continuation. QString address minus main is `0x18d8`. There is one eligible outer pair, and no active selected invocation remains at the AV.

| Sequence | Event | Raw normal return | Independent interpretation |
|---:|---|---|---|
| 1 | outer entry | â€” | selected main caller |
| 2 | duplicate | `0x0` | no duplicate prerequisite failure |
| 3 | factory | `0x3853820` | nonnull child |
| 4 | cast | `0x3853820` | nonnull cast result |
| 5 | Project | `0x3855220` | nonnull Project result |
| 6 | acceptance | `0x0`, AL 0 | first failed prerequisite |
| 7 | outer return | `0x0` | normal zero return of same invocation |

Numeric returns independently agree with each generated `failed` flag. Runtime QString characters were not read. Attribution is to the unique fixed-command invocation through caller/frame/main-local identity, not independently verified filename characters, derived DB identity or parser input content. Root relayed a concurrent CP212 static finding that main sends `-f` through decoder RVA `11a0` (length/4, four input characters per QChar, sscanf-like helper `146dfd4`). This lane has not independently audited that decoder. It makes literal ASCII argv an unresolved input-contract risk: the observed QString may not name B98 at all. Preserve the return observations while withholding any claim of a valid original B98 load or a fixture-format rejection. A documented/qualified CLI encoding and decoded-path witness remain necessary before a B98 scientific comparison. The return sequence does not establish why acceptance failed or the internal reader/parser/allocation subcause.

## Fault and cleanup

First observed AV core is a read of `0x58`, RCX `0x58`, at installed `QtCore4_64.dll` base `0x583e0000`, RVA `32c0`. Enriched record agrees with core process/thread/creation identity, RIP, operation and address. Its accurate backtrace contains contiguous app RVAs `a7510 -> eb1ec -> eaa7a`, plus the later callback/main context `2a725 -> 1cae` in order. Module paths match the original installation. This independently meets the proposed CP205 same-path boundary; it does not prove every omitted stack frame or a separate debugger first/second-chance history. The Frida handler returns false to permit natural exception delivery.

Four persisted raw helper JSONL records are created, resumed, endpoint-target-exited and cleanup. Parsing the four strings retained in the summary gives exactly the same records. Created identity matches the hooks; resume previous suspend count is one. Cleanup reports signaled wait result 0, verified exit `3221225477` (`0xc0000005`), closed handles, no termination request and empty termination attempts; helper exit is 0 with no stderr, partial line or overflow. Session detach is settled after target exit; script unload is explicitly not applicable because the session already detached. Retained successful Windows query and same-device Frida enumeration both report PID absent. This is a complete natural-exit/absence receipt for this owned child, not a current fresh process enumeration. No GUI control was acquired, and debugger/session exit is not described as GUI-control release.

## Summarizer limits and exact accepted claim

The existing `same_fault_path_outer_acceptance` label agrees with independently recalculated raw numerical predicates and ordering. Two general limits merit preserving in downstream interpretation: the host's pair classifier relies on agent `failed` flags rather than independently deriving every predicate from raw returns, and its `cp205FaultPath` test checks the core and contiguous three-frame chain without itself requiring callback/main `2a725 -> 1cae`. Both stronger checks pass manually for this capture, so neither invalidates this result. The summary's `releaseBlocked` array is stale preparation text, as root's explicit release receipt explains; it is not evidence of a runtime failed gate. Do not propagate it as a current HOLD or silently treat it as a new authorization.

**Accepted claim:** In this single six-entry-instrumented, fixed literal-argv owned original CLI occurrence, duplicate lookup was zero, factory/cast/Project returns were nonzero, acceptance AL was zero, and the selected outer loader returned zero normally before the CP205-matching first observed AV and natural `0xc0000005` exit. Acceptance is therefore the first observed failed outer prerequisite for this occurrence. A valid B98 comparison, instrumentation neutrality, actual runtime filename text and CLI decoding, acceptance's internal cause, successful original Save and accepted Close remain unestablished.

## Later CP212 decoder qualification

CP212 decoder analysis subsequently establishes that valid `-f` input uses four hexadecimal digits per UTF-16 QChar. The prior 34-character literal ASCII path instead attempts nine QChar stores into an eight-QChar array. The exact resulting runtime QString remains unverified. This strengthens the input-contract limitation above: **PASS applies only to the observed malformed literal command**, its prerequisite returns, matching fault path and cleanup. It does not establish a valid B98 Open, B98 acceptance failure, or behavior of a correctly encoded project path. No additional original run or source change is implied by this qualification.
