# CP194 R300 plan review — htPostLoad

## Verdict: PASS, with implementation checks

The proposed `htPostLoad` cut is a coherent lexical confinement of the existing SimControl pointer. The callback starts at the current `opSimCtl(p, op)` acquisition and includes subtype reads and writes, object-count/admission work, the existing `OprFlag != 0` early return, defaults, and the conditional STNOUT/ELMNOD copies. It therefore preserves the required acquisition-before-subtype/object/default ordering. Returning from the immediately invoked callback also exits the same function path; the only remaining outer statement is scratch destruction. With no callback result or stored capture, `sc` cannot escape the lexical callback.

The planned acquisition comment must stay precise: `opSimCtl` performs the nested model lookup, and `GuiModel::simCtl()` checks geometry before nullable-step admission. That gate occurs after allocation and predecessor KEY processing but before subtype and object/default effects. It does not guard all function effects. Null SimControl handling remains path-dependent: the OprFlag early return and other non-copy paths may avoid dereference, while the STNOUT/ELMNOD copy path dereferences unchecked. Do not infer native null or geometry behavior from this reconstructed source.

## Fixture and branch coverage

The proposed corpus route is supported by retained `08_HEAT1` evidence. `Task00001/SimCtrl.KEY` has `UNIT 1`, `STNOUT 0 0 0 0 1 0 0`, and `ELMNOD` count 10 followed by ten zeros. Those literals differ from all seeded destination sentinels and make both copies observable. The independent expected subtype/default values are consistent with the retained L2 qualification: predecessor subtype `Heat in furnace`, OprFlag zero, and operation-2 subtype `Transfer through air` result in the air-transfer defaults (15 seconds, ENVTMP 20 in unit 1). Keeping NSTEP and unit unchanged is a useful negative assertion.

The separate OprFlag=1 call exercises the present early-return condition only if slot 0 still holds an object and model object count remains at least one. The plan requires those preconditions and asserts the default/property/array sentinels remain unchanged, which adequately distinguishes this branch from the defaults/copy path. Snapshotting FileEffects sizes after setup avoids incorrectly assuming setup has no effects.

The ownership, cached model/step/control, property/common and slot identity assertions complement the static proof. They do not establish protection against arbitrary reentry or model replacement; the plan explicitly disclaims those guarantees. The existing public loader-slot table and `OpClass::current` seam are sufficient in principle for direct slot invocation without adding an export. During implementation, use the actual exposed slot/object helpers and avoid introducing a test-only production seam.

## Evidence qualification and limits

The L2 spec identifies its basis as historical static analysis of `DEF_GUI_MO_64.exe`, annotated `d0a.c`–`d5a.c`, `ht_post.asm`, and a Ghidra copy. It labels `0x141354ce0` asm checked; ledger row 4 binds that slot to the loader owner. The named original decompile/assembly artifacts are absent from this converter inventory, so this is retained annotated evidence, not a fresh inspection of original assembly or a runtime trace. The plan correctly keeps the scope at reconstructed source mechanics and does not claim native null semantics, reader continuation equivalence, or GUI Open/Save proof.

No blockers found. Preserve the stated CP193 freeze hashes/source comparison and keep execution, source changes, and build/freeze work with the root owner.