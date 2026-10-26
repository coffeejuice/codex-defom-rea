# Resume 46: Convert2Dto3D and CopyMirror property-call crosswalk

## Scope and evidence basis

This is a static source and retained-decompilation crosswalk for the two
operation classes. It does not claim a fresh original run. Original addresses
below are tied to the imported GUI-MO MDT packet/decompilation and corroborated
by retained call-site comments in reconstructed source. The crosswalk concerns
the selected one-project Open → Save path; it does not resolve unsupported
geometry or viewer-plane construction.

## Call-chain summary

The converter activates the selected project subproject, then calls
`engine::openActiveOperation` before `saveCurrent` and MST emission. The helper
selects the active flow item, resets its model, and dispatches the class's
`loadOperation` virtual (`loader.cpp`, lines 1052–1079). The retained model is
then available to Save/MST. Therefore a mutation in a reachable load callback
can affect subsequent serialized model data even if it does not directly emit
MST bytes.

| Class / method | Original qualification | Current source path | Status |
|---|---|---|---|
| Convert2Dto3D operation load | Original vt+0x58 `0x1401c3a60`; the retained body checks current operation/model/scene/second model, then dispatches to `0x140c7b720` for the second model. Original property-to-engine method is `0x1401ce590` (properties vt+0x60). The native load body invokes `0x1405f36d0` → `0x1400543a0` in both the chain-head and non-head paths. | `Convert2Dto3DClass::loadOperation` dispatches to `c23Load` (`src/ops/convert23.cpp:53–56`; slot installed in `src/engine/l5_slots.cpp:1232`). `c23Load` calls `baseLoad`, requires active operation/model and present scene, then applies `propsToEngine` for either branch (`l5_slots.cpp:598–652`). It copies `ConvertData` values (`upAxis`, conversion kind, extrusion/revolution controls) to each non-kind-7 object's fields (`l5_slots.cpp:1083–1104`). | **Settled reachable call; behavior is implemented.** Unsupported conditions remain explicit: absent scene, absent second model when work exists, non-2D chain-head model, and related native geometry paths. They do not invalidate the ordinary branch. |
| Convert2Dto3D MST writer | Original writer `0x1401c4440`; class-level property application `0x1401ce590` is not called by this writer. This absence alone does not mean properties never reach the model: the operation-load call above precedes Save/MST. | `Convert2Dto3DClass::writeMst` (`src/ops/convert23.cpp:127+`) calls `writeBlock` without `propsToEngine`; comment records the omission. | **Settled as a writer-local omission, not a lifecycle omission.** Do not add a second application based only on this writer. |
| CopyMirror operation load | Original vt+0x58 `0x140e24b80`; native body performs base load and scene display setup. Original property-to-engine is `0x140e2d230` (properties vt+0x60). Retained source/decompilation does not show a call from this load or its post-load. | `CopyMirrorClass::loadOperation` dispatches to `cmLoad` (`src/ops/copymirror.cpp:51–54`; installed at `l5_slots.cpp:1236`). `cmLoad` calls `baseLoad`, handles scene/second display model and clears viewer state (`l5_slots.cpp:696–711`). The registered `cmPostLoad` calls base post-load and `loadMirrorPlanes`, not `propsToEngine` (`l5_slots.cpp:885–889`). | **Settled unsupported in native open path: no `+0x60` call in the recovered load/post-load.** The property payload is read by `cmPropsLoad` (`0x140e2d080`) through shared `c23PropsLoad`, but that only reads model values into `ConvertData`; the reverse application is a separate operation. |
| CopyMirror MST writer | Original writer `0x140e24e80`; property-to-engine `0x140e2d230` is not called by the writer. | `CopyMirrorClass::writeMst` (`src/ops/copymirror.cpp:114+`) invokes `writeBlock` without `propsToEngine`; file header records this as intentional because “no MST byte depends on them.” | **Unknown output effect.** No direct MST dependency is established, but the fields affect later consumers of the model and current full Save calls shared model/operation processing before MST. No evidence here proves CopyMirror output remains identical for nondefault property values. |

## Reachability and effect distinctions

### Convert2Dto3D

This is not an open silent omission in the selected load path. The original
operation loader reaches `+0x60` after selecting its chain-head Geometry Type or
non-head Configuration path, and the reconstructed loader does the same. The
implementation writes the recovered property values into the model's object
records. The code then continues through the geometry/configuration branches;
later model Save and MST generation can observe those fields. In particular,
the existing loader comment records a concrete CRDSYS/WPAXIS discrepancy when
the property-to-engine call is absent. The `writeMst` comment is accurate only
for direct writer-local behavior and should not be used to justify deleting
the earlier call.

Smallest action: **no implementation change indicated by this crosswalk**.
Keep the writer comment narrowly scoped and preserve the native load call.
Fresh original-first nondefault-property comparison remains needed for
acceptance, especially the second-model/work-key path.

### CopyMirror

The native open path parses the shared property data but does not call
`copyMirrorPropsToEngine`. Its `+0x60` body is implemented as the same object
field mutation as Convert2Dto3D (`l5_slots.cpp:1083–1110`), and its public
property override is wired. Current caller search finds it only through the
generic Save-current helper (`save_current.cpp:147–152`), not from `cmLoad`,
`cmPostLoad`, or `writeMst`. Native Open therefore does not establish the object
fields from the CopyMirror property payload before downstream work.

Reachability is established; observable missing effect is not. The body mutates
conversion kind, axis, and dimensions on non-kind-7 objects, while CopyMirror's
viewer-plane/mirroring path and output writer have explicit unsupported cases.
No retained evidence here proves whether a normal CopyMirror project reaches a
consumer that serializes or otherwise depends on those fields before the
generic Save helper runs. Thus classify the behavior as **reachable call
omission, downstream effect unknown**, not as a confirmed parity defect or a
settled waiver.

Smallest implementable fix if an original call-chain trace confirms the
mutation is required during Open: invoke the existing `propsToEngine(p, op)`
once in the original-qualified CopyMirror load position, after
`baseLoad`/property loading has populated `ConvertData` and before model
consumers. Do not add it blindly: the original native load body currently
supports the contrary conclusion, and moving a Save-time call earlier changes
observable state/order. If no original call exists, retain the current code and
test the relevant downstream consumer instead.

## Minimum next evidence

1. Resolve original `0x140e24b80` xrefs/callers and the CopyMirror
   `0x140e2d230` caller set in the qualified native packet. Confirm whether any
   call exists on Open outside the already inspected load/post-load body or the
   GUI-only editor path.
2. Identify the first reachable Save/MST consumer of the fields written by
   `0x140e2d230`, and compare with the original ordering. If only generic
   `saveCurrent` reaches the slot, the proposed Open-time insertion is not
   supported.
3. If a reachable nondefault CopyMirror fixture and original output effect are
   identified, add the smallest focused regression and then include it in the
   fresh original-first acceptance set. Do not infer parity from historical
   folder comparison counts.

## Qualifications

The original address qualifications come from retained static decompilation
and comments, not a new REA session or process capture. The current sources
contain unsupported Mirror plane construction and other geometry branches;
those are separate in-scope refusals when reachable. Checkpoint 45 did not
validate these two operation families with fresh original runs. No build, source
edit, app control, or REA database mutation was performed for this crosswalk.
