# CP275 viewer post-bind color tail: HOLD

## Result and source qualification

The retained static source establishes the color tail's dispatch and its position in a nonnull-object bind, but does not establish a finite color effect that can be implemented faithfully. The smallest tempting branch is mode 1, yet its two live object paths call unretained helpers. Mode 2 and the remaining modes likewise call unretained graphics helpers. This is a **HOLD** for a source/test leaf, not evidence that color work is harmless.

The target is retained original **DEF_GUI_MO_64.exe v11.0.2**, previously qualified SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` in the CP257/CP262/CP274 reports. I did not rehash or execute it. Evidence below is retained Ghidra-style pseudocode, principally `evidence/gui_mo/s1/mdt/V/v0.c`, `v1.c`, `v8.c`, and `L3/s3.c`; pseudocode is not original C++ source or an execution observation. The old `spec_V.md` calls these helpers display-only, but that is an earlier analyst assertion, not a demonstrated complete effect boundary. No REA target was active; no analysis database was opened or mutated.

## Exact caller and order

`V/v0.c::FUN_1411f17f0` returns before color work for a null object, after clearing View+0x18, full unbind, and display refresh. With a nonnull object it performs partial unbind, stores the object at View+0x18, then invokes the 3D or 2D binder according to `FUN_140065800`. It reads the owner through View+0x10 twice: if the owner is nonnull and its +0x25c mode (`FUN_141201910` in `V/v8.c`) is nonzero, it uses that mode; otherwise it calls `FUN_141696820` for a global fallback. The fallback is decompiled as `void` in `v8.c` despite its return value being consumed in `v0.c`; its actual value contract needs qualification before a default-mode fixture is asserted.

The binder then calls `FUN_1411f28f0(view, mode)`, conditionally calls `FUN_1411fdd30(view, flag)` when View+0x338 is nonnull, and finally calls `FUN_1411fc900(view)`. Thus color effects, or a failure inside them, precede Slice adaptation and node display. Current `source/apps/moproj2mst/src/engine/viewer.cpp::bind` reaches its Slice admission after representation binding and explicitly leaves color/node behavior unimplemented. An eventual color implementation belongs **before** the Slice gate to preserve native partial-effect order; this is a future implementation constraint, not a request to move the current guard without the color contract.

`viewer.cpp::viewerSync` is an actual converter caller: after its geometry availability check it sizes/views the model, resolves the View by object number, calls `bind`, then handles pending geometry, optional default Settings, and refresh. `test_viewer_slice.cpp::slice_rebind_for_canonical_2d_owner_preserves_constructor_only_collection` and the corresponding canonical 3D/null-DB case are existing ordinary nonnull-object caller fixtures. Their successful `viewerSync` calls prove reachability of the converter bind boundary, not native color-mode selection or color side effects. `viewerRebuild` is another actual caller, but its loop can fail later in the Settings/default-flags stage; it is not a smaller witness.

## Bounded color dispatch

`V/v1.c::FUN_1411f28f0` replaces a zero supplied mode with the same global fallback. Mode 1 calls `FUN_1411f2430(view)`. Mode 2 obtains an object-derived selector via `FUN_1411f1ab0`, looks up a color through `FUN_1402d2b00(0x143d77590, selector)`, then calls `FUN_1411f2970`. Every other value obtains the same selector and calls `FUN_1411f2b60`.

`V/v0.c::FUN_1411f1ab0` returns zero when View+0x18 is null; otherwise it reads a 32-bit value at object+8. On the nonnull bind route the View has already stored the object, but the selector's semantic range and palette lookup behavior are not established here.

`V/v8.c::FUN_1411f2430` returns if View+0x18 is null. For a nonnull object, it calls `FUN_140065800`: a 2D object goes to `FUN_1411f24a0`; otherwise it calls `FUN_1400c3530` and, when true, `FUN_1411f26e0`. `L3/s3.c` identifies `FUN_1400c3530` as the object-dimension-3 predicate. The only syntactic no-call case is a nonnull object that is neither dimension 2 nor 3; no supported ordinary test fixture or Open-to-Save caller for that case was found. Neither `1411f24a0` nor `1411f26e0` has a retained body under `evidence/gui_mo/s1/mdt`.

`V/v8.c::FUN_1411f2970` constructs a temporary color-like value from its argument and passes it to unretained `FUN_1411f2a30`. `FUN_1411f2b60` obtains four colors in order from `FUN_141201930(0x1447c4860, selector, index)`, with indices **3, 2, 1, 0**, and calls unretained setters `FUN_1411f2da0`, `FUN_1411f2f60`, `FUN_1411f31b0`, `FUN_1411f32a0` after each assignment, then `FUN_1411f3430`. That order supports a possible partial prefix if a helper fails; it does not reveal the receiver writes, graphics backend calls, or exception behavior. The mode-2 lookup `FUN_1402d2b00`, mode-2 setter `FUN_1411f2a30`, mode-1 setters, four other-mode setters and finalizer have no retained definitions in this bounded evidence set. Therefore even the first concrete color write is unqualified.

## Next evidence needed

Recover read-only bodies and direct callee effects for `1411f24a0`, `1411f26e0`, `1411f2a30`, and, if the selected actual caller enters the other-mode path, `1411f2da0` through `1411f3430`. Qualify `141696820`'s return convention and global default or identify an actual owner with a nonzero +0x25c mode. Then select the smallest `viewerSync` fixture whose ordinary object and mode reach one branch, and specify the observable view/Settings/rep state and failure prefix before writing source or tests. A direct mode-unit test alone would not establish the required caller path. The independent `1411fc900` node-list leaf remains separately analyzable after this ordered color boundary.

No source, test, build, original runtime, GUI-control or mutable REA action was performed. Only this report was written.
