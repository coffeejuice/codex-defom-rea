# CP205 crash and reference-case correlation

## Scope

Read-only converter-side comparison of the successful fresh original crash capture `analysis/resume205-original-crash-stack-2026-10-26T08-58-00-325Z`, CP202 no-Frida/instrumented baseline records, and the reference-harness protocol/status. No original or candidate process was launched; no REA session was used; no source, fixture, queue, installed program, or user session was changed.

## Reproducibility qualification

The evidence supports a narrowly reproducible B98 command-line Open crash, not a universal claim about Gear projects or the GUI lifecycle:

- Each relevant run targets installed `DEF_GUI_MO_64.exe` v11.0.2 with SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, and invokes `-m open -f C:\PROBLEM\p202a\Gear_blank.moproj` from `C:\PROBLEM\p202a`.
- The CP202 uninstrumented baseline (PID 37824) and the earlier instrumented attempt (PID 20712) both report access violation `0xC0000005` in `QtCore4_64.dll` v4.8.4.0 at RVA `0x32c0`. The baseline establishes Frida is not necessary for this fault under the recorded launch conditions. The earlier instrumented report did not have caller context.
- The fresh CP205 capture (PID 31656) reaches the same QtCore module/RVA and reports a read from `0x58`; `RCX=0x58` at the known `mov rax,[rcx]` instruction. Its same-run app stack adds RVAs `0x4af4`, `0xa7377`, `0xa7510`, `0xeb1ec`, `0xeaa7a`, `0xdafad`, then Qt invocation and startup frames. The registered exception handler returned false; the process naturally exited `0xC0000005`. It had zero termination attempts, verified handle closure/absence, and a complete capture receipt.
- The 27-file disposable fixture digest is identical before/after CP205: `55e2a58cf8155995f942f89ac0ebb91454308b521ae76a228edec0ffdc4cedbd`. CP202 reports an unchanged fixture too. The fixture identity is the B98 `Gear_blank.moproj` with project hash `e769faf514b335d56ffbb22f2c76490d029f2714ada9b26a75496fbd62ade7fd`.

Therefore, three observations across instrumented and uninstrumented modes establish repeatability of this fault location for this executable, fixture, and `-m open -f` launch profile. They do not prove the defect's cause, rule out all environmental differences, show that every invocation crashes, or establish behavior for a GUI-launched File > Open. The `RCX` value alone does not identify a null owner; caller/receiver interpretation still needs the retained disassembly analysis. None of these runs reached observed Save or accepted Close.

## Which records can and cannot speak to Save

The B98 evidence is an Open-stage prefix ending in process crash. The byte-identical fixture confirms no project-tree writes in that capture, but says nothing about external settings/registry effects or whether a GUI user could reach Save by a different route. The converter impact note correctly treats the crash as an original defect/robustness datum rather than a converter result to emulate.

The reference harness is explicitly prepared for original fresh-state Open > Save > Close, but its status says runtime has not started for the currently qualified case. The smallest currently valid prepared training case is `build/reference-harness/billet-training-003` (BILLET, one `SfOprHeatTransfer` operation, three selected XML references; candidate receipt/preflight verified). Its preflight is not an original behavior observation and cannot establish that it reaches Save. The harness requires observing actual File > Open, then invoking Save and Close with observed defaults; it warns that historical coordinate guesses and hardcoded dialog answers are not valid evidence.

A later `billet-training-004` record is an actual original/candidate runtime observation, but it is a deviation-qualified run: original crashed immediately after rendered Open and before Save observation; persisted settings were not reset, whole-process side effects were unobserved, and candidate completion/raw output equality is not an original Save oracle. It therefore does not qualify successful Save or Close either. Historical corpus/oracle labels, including the known same-process MultiBlow contamination, must not be promoted to fresh-state expected results.

At the converter API level, `M2M_FLAG_WRITE_SAVE` intentionally traverses Open, operation activation, and Save; public ABI failure tests cover synthetic missing/read-failure branches that can reach the later Save decision. Those fixtures demonstrate converter control-flow coverage only, not original B98 or GUI Save behavior. No available evidence establishes which original fixtures/actions can pass B98's startup crash and witness Save. Specifically, an automatic CLI mode is not equivalent to the harness's required actual File > Open / Save / Close sequence.

## Smallest safe next original experiment

No read-only converter evidence can itself produce an original Save or accepted-Close observation. The smallest informative *future* original experiment is one fresh owned original process, one frozen disposable fixture, and the first lifecycle action only: use the supported observed GUI to open the already prepared `billet-training-003` case through the actual File > Open control, capture whether Open completes or crashes, and stop at that boundary. This tests whether the startup crash is B98/profile-specific while avoiding an unqualified Save attempt. If Open completes, the next gated action is Save on that same disposable case with actual dialog defaults recorded; Close is only attempted after Save completes, again recording any default prompt. Do not infer accepted Close from process termination.

This can be done without REA, but it cannot be done without GUI input/observation (or a separately validated non-CUA GUI driver). Current recovery status says app-control release remains unavailable and GUI validation is blocked. Thus this report does not queue or launch that experiment. A command-line or passive filesystem-only run might refine crash/file effects, but cannot witness the requested GUI Save action or accepted Close under the reference-harness definition.

## Evidence references

- `analysis/resume205-original-crash-capture-qualification.md`
- `analysis/resume205-original-crash-stack-2026-10-26T08-58-00-325Z/summary.json` and its fixture before/after manifests
- `analysis/resume202-baseline-crash-evidence.md`
- `analysis/resume202-original-probe-attempt2-crash.md`
- `analysis/resume202-b98-disposable-preflight.md`
- `analysis/reference-harness-status.json`
- `tools/reference_harness/README.md`
- `analysis/resume205-cpp-crash-impact.md`
