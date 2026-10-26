# B127 DBLIST guard: original route, candidate stop, and isolated seam

## Candidate-only observation

`analysis/resume51-b127-active-guard.md` and `build/b127-active-probe-1792867845145/probe-instrumented.log` record a private candidate-core probe. It explicitly opened the copied B127 project, activated subproject 1, and then opened its active `SfOprDoeOutput`. Activation succeeded. Candidate `doe/session_records.cpp:125-142` had already appended one `SessionPart` and retained its `SessionRoot`; it then threw code 7 at the first child named DBLIST. The observed partial part has operation range 1–5, root present, zero session records, and no current selection. Before that guard, the candidate created empty `Solutions`/`Output`, read InputTable and `DEF_DOE.PAR`, and wrote the LHS/FF/USR sample files. It later opened/read the 2,013-byte `.DS`.

These are candidate observations only. They do not establish original runtime effects or justify the candidate's unconditional refusal as original behavior.

## Original static evidence

The retained decompile `analysis/wave2-doe-session-valid-existing-input-decompile.json` covers original `FUN_140650e60` at `0x140650e60`; its receipt binds it to original GUI SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` and records that the target runtime was not executed. In the decompiled child loop (around exported lines 203–231):

1. The `Session` tag is tested first.
2. For another child, the code compares the tag with the original `DBLIST` literal at `0x1424e3cb0`. On a match it checks `FUN_1402cbec0(0x143d766b8)`; only when that check is true does it call `FUN_140655df0(param_1, node)`.
3. Otherwise it compares against `ReportList` at `0x1424e3cb8`; that handler has the same global gate but calls a different helper, `FUN_1406554a0(param_1, node, param_3)`.
4. The loop advances to the next sibling after these branches. The decompile does not contain an unconditional DBLIST exception.

The original receiver in the DBLIST call is the part-level inner model passed as `param_1` to `FUN_140650e60`; the node is the direct DBLIST child of that `SessionRoot`. This places the behavior at the part/session-root scope, before any per-Session record is allocated. The DBLIST helper body and its exact destination fields are not included in the retained function decompile, so the native DBLIST storage owner is not yet proven. The original global gate's precise candidate equivalent is also unresolved; do not equate it with the presence of a headless `ConversionSession`.

The accompanying raw literal receipt `analysis/wave2-doe-part-state-leaves-receipt.json` binds the `DBLIST`, `ReportList`, and `SessionRoot` ASCII literals to the same original executable hash. Five retained B120/B121/B122/B127/B128 `.DS` files have identical size (2,013 bytes) and SHA-256 `97e4aaa28aa545dd08360e98191fba3b2cfb6c39098e10aeb3b45d02c33e563d`; their common DBLIST form is a single direct `<DBNAME>../../../COG_P1_B1.DB</DBNAME>` child followed by an empty `ReportList`. This establishes the training fixture's observed spelling and shape, not the complete accepted DBLIST grammar or `FUN_140655df0` effects.

## Narrow isolated seam

Keep the active lifecycle refusal and public selection behavior intact. The smallest next seam is a **pure, part-scope DBLIST reader** exercised directly by a focused test, with no dispatch from public Open and no filesystem access:

- Input: one already encountered direct `DBLIST` node from a `SessionRoot`.
- Output: an ordered value object of its observed child tags and raw text values. For the B127 training input, the expected observation is one `DBNAME` with text `../../../COG_P1_B1.DB`.
- Ownership: initially return the value without mutating `SessionPart`, `SessionRecord`, `Study`, global/session state, or files. The current `SessionPart.root` already retains the raw source subtree after the candidate guard. Only after a separate source pass closes `FUN_140655df0` should the value be attached to a recovered owner. `SessionPart` is the nearest candidate abstraction because DBLIST is a sibling under SessionRoot and the original helper receives the part-level inner model; it remains a provisional mapping, not a claimed field identity.
- Dispatch: keep DBLIST and ReportList separate. If later integrated, model the observed original global gate and dispatch DBLIST to its own helper; do not let a format parser implicitly satisfy the different ReportList branch.
- Test: feed the retained B127 DBLIST node to the pure helper; verify element order and raw text, and verify the call has no I/O or document/session-record effects. Add malformed/empty/multiple-child inputs as parser probes only, with outcomes marked unqualified until the original helper's validation behavior is recovered.

This seam can settle the narrow XML extraction and keep the current blocker reproducible without enabling Type 2 through the public converter. It does **not** yet justify removing the live guard, choosing an owner field, resolving the relative path, opening a DB, or producing output files. Those require the `0x140655df0` body/side-effect and gate closure first. The `ReportList` helper/condition remains a separate dependency; its current candidate guard and original gated call are not evidence of DBLIST format behavior.

## Evidence boundary

Original decompilation/literal evidence is static and tied to the imported GUI PE hash above; there was no original process or GUI run in this task. The B127 runtime and file-effect facts are from the candidate-only probe, on a disposable copy, under `build/b127-active-probe-1792867845145/`. No source/build/GUI/mutable REA/shared state was changed for this analysis.
