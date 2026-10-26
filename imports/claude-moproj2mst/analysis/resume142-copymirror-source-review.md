# CP142 CopyMirror writer-prefix independent source review

## Review result

The candidate places the new CopyMirror properties-to-engine call at the correct local boundary: after confirming `CopyMirrorProps` and before `writeBlock`, whose first relevant actions increment `sim` and apply `onlySim`. The focused regression exercises a nondefault `ConvertData` through the installed CopyMirror slot and verifies model mutation despite filtered output. The change is properly qualified as a private writer-prefix correction; CopyMirror remains absent from the public registry, so it does not establish public Open/Save reachability or parity.

## Native and candidate order

- Original `evidence/gui_mo/s1/opw_d0.c::FUN_140e24e80` resolves properties with `0x1405ef320`, casts to the CopyMirror properties type, and for a match calls that object's `+0x60` before `FUN_140e24f60`. The candidate's `writeMst` has the same matched-properties branch and now calls `propsToEngine(prj, *item)` before `writeBlock`.
- The bounded body `evidence/gui_mo/s1/mdt/L5/s0.c::FUN_140e2d230` copies axis/type and the selected revolution or extrusion fields to each existing object except kind 7. Candidate `copyMirrorPropsToEngine` delegates to `convertPropsToEngine`, which performs the same object iteration and field updates. Its return is unused in both the candidate wrapper and the native writer sequence.
- `writeBlock` increments `sim` before filtering. Therefore `onlySim=99` with starting `sim=5` should leave `sim=6`, emit no text, but still have applied the property update. The focused test asserts exactly those observations with type 2 and nondefault extrusion values.
- The broader Save path already runs properties-to-engine in step 2; native step 5's writer independently repeats its own prefix call. Candidate step 2 likewise retains its two existing calls in `saveCurrentOperation`, and this patch adds the distinct MST-writer call. The extra call is not an accidental duplicate within the writer; it mirrors the native writer prefix. This conclusion concerns call structure, not a public CopyMirror project, which the candidate does not register.

## Guard and slot assumptions

`writeMst` only enters for a `CopyMirrorProps`, as the native dynamic cast does. It calls the slot only when `g_l5Slots.cmPropsToEngine` is non-null. Normal candidate project reading installs loader slots in `readProjectFile`; other engine entry points also install them before model creation. The new focused test explicitly calls `installLoaderSlots()`. The null-slot branch is retained for the older standalone P12 writer test, which links no engine slot table and has no model to mutate. That is a test-only compatibility exception; if an engine path can call this writer with a valid model and a null slot table, it would silently omit native behavior. No current retained caller establishes that case, and CopyMirror is not in the public registry.

The test uses a disposable retained `42_vdie` project, retags operation 4 to CopyMirror, creates CopyMirror properties, then loads its model before applying nondefault properties. This establishes a candidate-only exercised call path, not an original CopyMirror project fixture. It validates `+0x2c38`, `+0x2be8`, `+0x2c10`, and `+0x2c18`; it does not independently check the linked WPAXIS member written by the shared helper or confirm that kind-7 objects remain unchanged. Those are useful extensions if this helper's detailed field contract becomes an acceptance target, but neither invalidates the tested call-order regression.

## Remaining boundary

The exact `onlySim` property test does not compare serialized MST bytes or original runtime effects. No claim should be made about output-byte sensitivity; the retained writer source proves the mutation order, not a particular changed output. No change to registration, public admission, or full-project behavior follows from this prefix.

Read-only review only. No source, build, test, GUI, runtime, or mutable REA state was changed.
