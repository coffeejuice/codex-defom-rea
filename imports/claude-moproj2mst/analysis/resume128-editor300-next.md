# CP128: disabled private remesh must not require an enabled-only method

The smallest source-closed next cut is the disabled-remesh branch in private applyMeshing3D. Native Apply completes per-object meshing writeback first, then writes the remesh enabled byte0 and skips method/scaling/internal-checkbox reads. Current C++ rejects any method-radio projection outside1/3 before those object effects, even when remeshEnabled=false. Make that radio admission conditional on the enabled branch; preserve actual object writeback and only change the enabled byte of the shared RemeshState when disabled.

Only this new report was written. No source/test/shared-header/CMake edits, build/test, GUI, original execution or mutable REA operation occurred. This is private page/model behavior; the full selected Editor300 constructor/palette gate is untouched. Original DEF_GUI_MO_64 v11.0.2 SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

## Exact native call and owner order

The original controller is140ec1350. Retained complete instruction view `wave2-save-diffs-300-save-140ec1350.asm` and decompile `wave2-save-diffs-300-apply-140ec1350.c` agree:

| Site | Consumed owner / effect |
| --- | --- |
|140ec138a..13a0|Read UI+9b8 current combo index and explicitly call140eceb60 to commit the current temporary form.|
|140ec13ba..14f0|Walk displayed object entries, parse their remembered number, look up the current actual model object, and when object/temp exist call14085e050 at14df to copy four scalars. Missing object skips that copy and continues.|
|140ec14fd..1507|Require the original dimension3 predicate; non3D skips singleton work. Existing private API still requires a real3D GuiModel.|
|140ec152c..15a4|Read disabled UI+a40 / enabled UI+a48 radios; resolve the remesh singleton through1402c7ed0.|
|140ec15ae..15c9|Disabled branch: prepare byte0, call1400ab150 at15c4, jump directly179d.|
|140ec15ce..15dd|Only enabled branch first stores enabled byte1.|
|140ec15f8/1634|Only then read method radio UI+a00/a08; store method1/3 through14092db70.|
|140ec167c..173e|Only enabled branch reads/parses scaling text UI+a10, applies original bounds and stores scaling.|
|140ec1759..1797|Only enabled branch reads UI+a28 and writes inverse internal-element checkbox.|

Getter1402c7ed0 is a direct LEA of143d91238/RET. Setter1400ab150 loads the supplied byte and stores exactly receiver+29 at1400ab167, then returns; it does not restore method, scaling or other global fields. The existing RemeshState layout and actual Session::remeshState owner already represent that same logical singleton boundary. This patch needs no new global, duplicate state, symbol registration or constructor.

The object's earlier copy is not a broad mesh clone:14085e050 copies only four doubles. Existing C++ reads/writes object+2dc8(interference),+2dd0(size ratio),+2de0(tolerance),+2dd8(node count), preserving unrelated model state. Disabling global remesh must not skip that earlier remembered-object path.

The native disabled branch reads neither method radio. An unavailable/irrelevant private method projection therefore need not be made valid before this branch. This does not claim a GUI group physically has a new radio state or enable native control construction. Method1/3 validation remains required for the existing enabled private profile; the source's broader radio edge cases are not generalized here.

## Current discrepancy and minimal patch

Current applyMeshing3D preflight is dimension3 AND method-radio1/3 regardless of form.remeshEnabled. The object loop then runs and state.enabled is assigned; method/scaling/internal checkbox are already correctly inside if(state.enabled). Only the preflight condition is too broad.

Change the radio exclusion to apply only when form.remeshEnabled is true. Leave dimensional ownership checks, object iteration/order, selected-first control parsing, missing-object skip, state.enabled assignment and enabled-state setters unchanged. Disabled scaling text and inverse checkbox remain unconsumed. A future method validator must not move into the object loop or return early before that loop for a disabled page.

The existing disabled test covers preservation only with a filled valid method radio1; it does not cover an unused/unqualified method projection or prove earlier model copies still occur when that projection is unavailable. This report therefore identifies a missing conditional admission, not a duplicate test of the existing enabled/disabled byte assignment.

## Exact exclusive implementation set

Request after the current frozen candidate:

- Existing `source/apps/moproj2mst/src/engine/simcontrol300.cpp` (one condition plus explanatory comment).
- New `source/apps/moproj2mst/tests/mdt/test_simcontrol300_disabled_remesh.cpp` (dedicated focused target, avoiding edits to the large snapshot test).
- New implementation receipt.

No shared header, CMake, native IO/reader, lifecycle, Session, public loader/factory or Spring file changes are needed. Root owns build/integration. Existing CONFIGURE_DEPENDS test discovery can register the new target when root configures.

Focused acceptance/failure-order cases:

1. Populate an actual3D GuiModel with two eligible objects, fill the existing private page, then set remeshEnabled=false and an unqualified method sentinel0 or99, invalid scaling text and inverted checkbox. Change current model fields after Fill. Apply must still copy the selected-first rounded controls and unvisited remembered four-scalar payloads, then set the supplied same RemeshState.enabled to0. Preserve current method (including non1/3), scaling raw bits, other booleans/bytes, padding-array fields, filter/uniform parameters and unrelated object fields. The state may be the actual ConversionSession::remeshState member supplied explicitly to the private helper; do not label this a public Session callback.
2. Use a remembered missing object number among real entries. That lookup failure skips only its copy; another present object still updates and the final disabled flag is stored. This tests native continuation before the disabled gate, without a synthetic success/rollback policy.
3. Re-enable the same page with the invalid method sentinel: retain UnsupportedSnapshot at the existing reconstruction preflight with no object/state effects. Existing valid1/3 enabled tests continue to cover method/scaling/inverse flag behavior. Do not claim this excluded enabled profile reproduces native partial object effects.
4. Keep wrong-dimension or absent-step admission before effects. A disabled method must not bypass actual GuiModel/3D ownership. If an existing parked geometry-owner fixture is reused, its real object lookup refusal must still precede state.enabled assignment; do not add a fake callback or shared hook merely to inject that failure.

No test was executed here. The small implementation should use Release-active Testkit, inspect real model fields and native-byte flags, and leave source frozen for root's focused/full checks.

## Larger follow-on found during survey

A later independent cut is explicit meshing activation0->1->0. The constructor connects UI+9b8 activated(int) to slotCmboxRemeshObjectsActivated(int); slot dispatch1410db93d calls140eceb60. The handler owns editor+f8 temporary array with stride158 and stored previous index+100; it commits the previous four controls then ALWAYS reloads target, including same row. This differs from Spring's same-row early return. Current MeshingPage only has first-form controls and Apply always roundtrips entry0.

That larger transition needs root-coordinated shared selection/index/report fields and a reviewed numeric/sign/spin policy; it is not part of the present one-condition patch. Retained full handler140eceb60..140ecf2cd hashes to `49b658b9bee06716baddd4df1e69db8d9641b558517df54c3d76bc594bf7c2a9`. A direct COMISD/JBE at140ecefaa/fb3 also shows why decompile prose must not guess NaN radio behavior. Do not implement activation from the broad analogy to Spring or silently reduce current Apply numeric semantics while doing so.

## Read-only receipt

Half-open original PE ranges rehashed directly; instruction semantics verified against retained DUMPBIN plus bounded setter/getter inspection:

| Range | SHA-256 |
| --- | --- |
|140ec1374..140ec13a5 current-form commit|`7da1cba76bb6a03500d06ee41944d73af61cec8f0c80fa1207035718ef66587b`|
|140ec14ae..140ec14f5 earlier object copy/continue|`3fcb127581a013d50067816817219f05f197cf8da07ca40f8c9e91e81dd6bb8b`|
|140ec14f5..140ec15ce dimension/disabled branch|`91d5331729846394d93a71452fc648938349723e5a43f155db2388efe794c8ad`|
|140ec15ce..140ec165e enabled method reads|`7d3ad1f935c35ba393c7bdfc0c1108b1d0bc6a68e5a3fb959f5c4906d84cfcc0`|
|1400ab150..1400ab16b enabled-byte setter|`661638448fe1877ae9f5e2f703a258824fbd2a6990f8e224b3c1953dabb006ca`|
|1402c7ed0..1402c7ed8 singleton getter|`45f853e803df2eacde7ca22b61f2950dcc1c6f989a3957011ed0377fcd0e3918`|

Analysis tool was pinned DUMPBIN BuildTools14.44.35207, SHA-256 `12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`. No original binary/DLL code was executed. Bounded reads created no extra raw capture file in this report-only lane.

Retained save instruction view SHA-256 `d13290f8f37ff4f16ec14e8fd7e5b39b99cb0c24f7e9eb7846e7eb273b7977e2`; Apply decompile `d3b041dfb0f7a1d43fbbc43d9e70d26fe88b97443bfb3ef89418cffa42cea20d`; meshing contract JSON `de34e83b5c7b61954bfa92e532fa8f853ce1802fb8507020742058e50df62174`. Read-time source simcontrol300.cpp `0568dc61f1023d28da4930338dbb99119fc4c2aef78f0399d3cb1be042534b68`; existing snapshot test `0f52dff5154f5fb70c4566be753f03d0183a9fc0fba82c0ea66e4032530ac250`. No reconstructed executable/deployment was inspected or produced.
