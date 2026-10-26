# CP195 R300 next alias audit after htPostLoad

## Decision

No new L2 scoped-alias callback is justified after CP194 `htPostLoad`. The remaining L2 control-pointer sites either already confine the pointer to a short helper/conditional, or are one-store setters; wrapping them in another callback would add ceremony without closing a meaningful step-owned interval. Do not add a CP195 source/test pair on this audit.

The current inventory was checked in `source/apps/moproj2mst/src/engine/loader_l2.cpp` after CP194. `htPostLoad` now passes `opSimCtl(p, op)` directly to its immediate callback and confines the substantial subtype/object/default/copy interval. `furnacePostLoad` likewise has the CP193 callback around its flag/curve effects. Those owners are closed and must not be repeated.

## Remaining candidates and disposition

- `setTrans2` and `setTrans1` (`loader_l2.cpp:61-70`) use an if-initializer solely for one scalar store. These are the previously excluded one-store setters, not a multi-statement owner.
- `htStepDataFromSimCtl` (`:102-113`) and `furnaceDataFromSimCtl` (`:179-190`) accept `Cls_1401e7b10*` parameters, null-check once, and map a bounded set of fields into property data. Their pointers are already function-local; there is no unscoped acquired alias spanning control flow. Their callers pass `opSimCtl(...)` as a temporary argument (`htPropsLoad :305`, `furnacePropsLoad :352`). An IIFE around these single helper calls would only duplicate the helper boundary and would not shorten a retained caller alias.
- `furnaceDataToSimCtl` (`:407-419`) similarly has a bounded null-checked helper parameter. The caller at `:427` forwards the temporary result of `opSimCtl` and retains no pointer.
- `htSubTypeDefaults` has its own immediate callback from CP192, nested in CP194's callback; no new owner remains there.
- `furnaceObjectTreeAdd` (`:370-385`) keeps a `GuiModel*` only for one loop iteration and uses it to query object count. Its `LLOBJ_DATA*` is immediately checked/consumed in the same iteration. These are model/object aliases rather than a retained SimControl step alias; wrapping the whole loop would broaden the scope and alter the proposal's focus.
- `opSimCtl` and `slotObject` are pointer-return helper seams, not local owners. This audit does not propose changing their APIs or globally claiming alias closure.

The closest apparent owner is `htPropsLoad`'s `opSimCtl` temporary feeding `htStepDataFromSimCtl`. A callback at that call site would have exactly one synchronous helper invocation and no consumer after it; the helper itself already checks null and performs the six field reads/writes. There is no meaningful source boundary to cut further, and the corpus08 route would only retest an already-contained mapping helper rather than demonstrate a newly closed alias.

## Test and file boundary

No exclusive source/test pair and no focused test are proposed because the audit found no worthwhile next owner. In particular, do not add another case to `source/apps/moproj2mst/tests/mdt/test_loader_l2.cpp` merely to wrap or re-exercise the already bounded `htStepDataFromSimCtl`/`furnaceDataFromSimCtl` mappings. Existing corpus08 L2 cases remain useful behavioral coverage, but they do not make a redundant callback a new recovery step.

If a later inventory identifies a multi-statement alias that is acquired and retained in a new owner, first bind it to the current source freeze, then select the narrowest existing public slot/test route and add a same-owner test in that slot's existing test file. No synthetic class, alias escape, model replacement, or new export should be used to manufacture reachability.

## Evidence qualification and limits

The retained L2 specification introduction and section 2 qualify their original basis as historical static analysis of DEF_GUI_MO_64.exe, with annotated decompiles `d0a.c`–`d5a.c`, `ht_post.asm`, and a Ghidra project. The named original decompile/assembly files are absent from this converter inventory. This audit relies on those retained annotations plus current reconstructed source; it is not a fresh original-binary inspection or runtime observation. The corpus08 fixture supports reconstructed route coverage, including class substitution where documented, but does not establish native behavior for a substituted furnace class.

CP194's plan review and implementation report qualify the `htPostLoad` cut as lexical confinement only. Neither this audit nor the existing source tests establish native null/geometry policy, arbitrary reentry or owner-replacement safety, global closure of pointer-return APIs, or original public Open/Save equivalence.

Only this report was written at `analysis/resume195-r300-next.md`. No source, test, build, executable, GUI/app-control, REA database, parent/shared plan, or original evidence was modified or run.