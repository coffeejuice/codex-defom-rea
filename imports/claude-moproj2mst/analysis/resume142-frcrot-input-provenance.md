# CP142 FRCROT reader input and selected-pair provenance

## Finding

The native input selector can be traced from the current keyword record to all three scratch-model lookups, but the selected pair cannot yet be proven from retained PE evidence alone. The key correction is that the conversion reader dispatches **FRCROT handler3 at `0x140b51140`**, not handler2 at `0x140b51050`. `FUN_140922280` indirect-calls registry record `+0x28`; the FRCROT row places handler3 there. Handler2 is a separate callback and is not called by the three reader calls in `FUN_140916530`.

The precise outstanding native witness is the group-3 text parser body at `0x140b667a0` and its pair append/order effects. The reader creates three independently parsed `DB_STEP_DATA` scratch models, then asks handler3 to select from each. Without the parser's effects, neither an exact selected pair nor successful upper/lower outputs can be asserted. Public nonempty keyword conversion remains guarded.

## Input value provenance into the reader

The relevant caller is `evidence/gui_mo/s1/mdt/DOE/d1.c`, in the variable/keyword output loop:

1. `uStack_c0` is the current keyword record.
2. `0x1400654f0(uStack_c0, ...)` supplies a copy of the nominal text from keyword `+0x10`. CP73 (`analysis/resume73-doe-tloc-writer-evidence.md`) closes this getter. The next two QString copies come from keyword `+0x18` and `+0x20` (upper/lower), as identified in CP139's caller ordering.
3. `0x140921930(uStack_c0, ...)` reads the keyword name; `0x1403047c0(uStack_c0, ...)` copies its parameter name.
4. `0x1400d0e20(uStack_c0, ...)` returns the keyword's ordered `KwOperations` IDs (CP73's d0.c:1959–1989 qualification). The caller copies those IDs in encounter order into a local integer vector and `0x140917800` materializes the vector passed as `param_4` to `0x140916530`.
5. The separate signed selector read from keyword `+0x84` via `0x1400d7fa0` is saved for the later nominal commit to `0x1409210d0`; it is not the pair-selection vector passed to the group-3 handler. Do not conflate it with `KwOperations` or the +0x80 parser field.

This shows the source *value* of the selector vector. It does not show that its integers name a pair actually present in any of the three parse results.

## Three scratch models and callback order

`evidence/gui_mo/s1/mdt/DOE/d9.c` is the retained body of `FUN_140916530`:

- It opens and writes the supplied nominal QString into the first `temp.KWD`, creates a `0x3408` model through `0x140821bf0`, and selects a text parser from the registry result `0x140922090(keywordId)`. FRCROT is registry ID 276, group 3; the retained FRCROT registry row reports handler fields `0x140921da0`, `0x140b51050`, `0x140b51140`. The group value selects parser `0x140b667a0` in the reader's 1..5 dispatch.
- If that parser returns zero, it repeats the QFile/TextStream/model creation/parse sequence for upper, then lower. Each successful model remains alive for the subsequent callbacks.
- After the lower parser succeeds, it calls `0x140922280` three times in order: nominal model at `0x140916c96`, upper at `0x140916d76`, lower at `0x140916e56`. Each receives the same keyword `KwOperations` vector and a distinct output slot.
- If a callback leaves an output nonnull, `0x140916530` allocates a `0x78` record and calls `0x140898f40` to copy the returned DYNARY before publishing the output. The three scratch models are destroyed afterward. Thus the published value is meant to outlive the borrowed pair field; it is not a direct pointer into scratch storage.

The reader's output status is not the caller's commit gate. The caller proceeds when upper and lower outputs are nonnull, then optionally commits nominal, followed by upper and lower. Earlier `temp.KWD`/parse effects can precede a missing-output gate.

## What FRCROT handler3 does

The bounded original-PЕ decode in `analysis/resume141-doe-keyword-owner-bounded.asm` (`0x140b51140..0x140b5120d`, bytes SHA-256 `097d1beb4fff7d86194a8a0e937f8a7a4c72625872cd533ae9940bda206a7869`) establishes:

- `0x140922280` dispatches the record's `+0x28` callback. For FRCROT the registry row puts `0x140b51140` in that slot. Handler2 `0x140b51050` is not the callback reached here.
- Handler3 reads vector element 0 without a size check. If it is at least 1, it calls pair-at-index `0x1408e6b70` on the scratch model's `+0x13c8` list using that value as a one-based index.
- If element 0 is below 1, it reads elements 1 and 2 without a size check and calls pair-find `0x1408e6bd0` on the same list using those two values as master/slave IDs.
- If a pair is found, it writes the borrowed address `pair+0x240` through the output pointer. If no pair is found, it leaves the output slot untouched. It returns zero either way and destroys its by-value vector copy.

The existing cross-binary `source/port/defkey/src/kw_g3/registry_g3.cpp` calls the parallel FRCROT table handler `handler3_frcrot`; that is consistency evidence for selector behavior, not a replacement for the original GUI addresses or parser body. Its handler2 writes a KEY record and is semantically separate from the reader's handler3 callback.

## Exact unresolved parser/owner witness

The retained material identifies `0x140b667a0` only as the group-3 parser call in the three branches of `0x140916530`. It does not retain its body or a byte-range capture establishing:

1. how each input text establishes pair records in the model's `+0x13c8` list;
2. the order and identity of those pairs after parsing, including mirroring or duplicate behavior;
3. which textual rows make the `KwOperations` selector resolve on all nominal/upper/lower models;
4. its malformed-input, append-partial-effect, and cleanup behavior.

Separately, CP141's `0x898f40` callsite evidence shows the normal deep-copy path (`0x899010` then `0x89ad60`, which allocates/copies storage through `0x89b840`), but its allocation-failure and unwind behavior is not closed. A candidate may own copied values, but must not claim native allocator/failure parity without those details.

## Next bounded step

No public reader integration is source-closed. The immediate useful evidence action is a bounded original-PE capture/decode of the complete `0x140b667a0` parser body and the callees that append/mirror rows in `DB_STEP_DATA+0x13c8`, limited to its FRCROT group-3 branch. Use an existing fixture only as a later consistency check; do not use it to infer general parser behavior. Once the parser proves the pair rows and order, the next candidate seam can use the already supplied ordered `KwOperations` vector against each parsed scratch model, with the existing `+0x240` borrow lifetime explicitly bounded by the `0x898f40` copy. Public keyword/save admission still additionally requires the canonical actual keyword owner and its three text/list inputs.

## Qualification

Read-only synthesis of `evidence/gui_mo/s1/mdt/DOE/d1.c`, `d9.c`, `d10.c`, the CP73 and CP139 reports, CP141 registry/handler captures and current `keywordValues` source. Original GUI PE SHA-256 is `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`. No source, build, GUI, original execution or mutable REA action occurred.
