# CP272 Slice adaptation gate design review — PASS

The revised design receives PASS. It now admits only the actual View owner equal to the caller `&m`, with a present step and dimension exactly 2, at the existing post-bind boundary. Existing entry/post-bind geometry checks therefore apply to that same owner. Foreign, null, missing-step and other-dimension profiles retain the existing refusal. The design explicitly labels owner equality as a conservative adapter restriction rather than a recovered native gate. This resolves the earlier independently parked-owner admission hold.

No remaining correction is required for this bounded design. No source/tests/build/shared state, original process, GUI control or mutable REA was changed or run. Only this review was written.

## Native sequence and scope supported

Read retained `evidence/gui_mo/s1/mdt/V/v0.c::FUN_1411f17f0`, `FUN_1411f1980`, and `V/v1.c::FUN_1411fdd30`, plus current bind/owner/admission and Slice fixtures. Native nonnull-object bind performs partial unbind, stores object, executes the object-discriminator 2D/3D bind, resolves color work, tests View+338, calls `1411fdd30` if present, then calls `1411fc900`. Current converter already qualifies omitted color/node behavior; moving the Slice dimension decision ahead of object/reps binding would alter its effect prefix.

`1411f1980` resolves the View's actual model guard at +10. `1411fdd30` then requires nonnull owner, owner-is-3D, `c911c0` false, and nonnull database before reaching collection replacement/graphics/enumeration. Therefore a live owner with a present step of dimension 2 exits before **any** adaptation-body collection mutation, regardless of Slice presence. Object dimension is a different discriminator and must not replace the owner-model check.

The report correctly limits this leaf to explicit dimension 2. It does not admit arbitrary non-3D integer values, no-step/default getter results, missing owner, c911c0 semantics, database work or adaptation-body exceptions. Original target remains retained **DEF_GUI_MO_64.exe v11.0.2**, previously independently established SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. This review used retained decompilation, not fresh PE decoding or original execution; removed-unreachable-body warnings do not affect the observed outer short circuit but do prevent body-completeness claims.

## Actual-owner admission resolved

Current private `View` captures `ownerModel` at construction. The canonical `viewerSync(m,...)` route enters `GeometrySyncAccess::requireAvailable(m)`, obtains `viewerOf(m)`, binds, and checks `GeometrySyncAccess::pending(m)` before the proposed Slice boundary. That establishes canonical owner=m admission, including the post-bind park check. However `GuiModel::viewer` is represented by shared storage and public aliases are not generally closed; an actual View owner can differ from the caller in such represented states. `GuiModel::dimension()`/`modelDim` is an unguarded value query. The caller's `m` checks cannot be asserted to cover a separately resolved parked owner.

The revised design selects the canonical-only admission **at the existing Slice boundary after bind and the pending(m) return**: resolve actual owner, check `owner == &m` before dereference, then require present step and dimension 2. Because `m` is the live caller and its existing geometry checks precede this boundary, this does not admit a separately parked foreign owner through the unguarded dimension getter. Every failed eligibility check retains the same adaptation refusal rather than adding another owner query/error. The report correctly does not equate this adapter restriction with native `c911c0`.

Owner lookup/admission does not move ahead of binding. No public API or object/collection-backlink owner substitution is proposed. Missing owner/step cannot acquire success through a scalar fallback, and no larger ObjectKey/currentness migration is claimed.

## Actual-caller tests and collection preservation

The existing `test_viewer_slice.cpp::slice_rebind_reaches_adaptation_guard_without_replacing_collection` is concrete supported state: ordinary `GuiModel(nullptr,2)`, actual bound object/view, and a collection created by `readViewerSlice` with process scratch runtime and no database. `viewerSync(model,object,false)` currently throws at the unconditional guard, so success after this leaf is a direct caller discriminator.

Replace that exception expectation with success plus a repeat call. Require the collection nonnull before dereferencing it; capture its actual pointer and `viewIdentity`, then require both remain identical. Check unchanged literal constructor/no-database state: optional `model` disengaged, `boundObject==nullptr`, `backendKnownNull==true`, `graphicCount==0`, and `opaqueMembersConstructorOnly==true`. The collection's view backlink is nonnull from construction, whereas its model/object backlinks intentionally remain unbound in this no-DB profile. Do not rewrite them merely because the View has rebound. Existing constructor scratch must not be recreated/reset by this branch.

A 3D negative fixture is feasible without geometry fabrication: an ordinary dimension-3 model with a default empty bound object, existing no-DB Slice collection and actual `viewerSync` reaches the post-bind guard without requiring unsupported rep/mesh data. Preserve the collection pointer and state and require the existing object-rebind refusal. Label this as the converter's conservative unrecovered-profile refusal: native 3D with a null DB would itself short-circuit later, so this test is not evidence that native rejects every 3D/no-DB rebind. Keep that later short circuit outside this leaf as the design already proposes.

Configured target/CTest confirmed read-only: `m2m_mdt_viewer_slice` / `m2m.mdt.viewer_slice`. Existing Testkit entry point and includes are present; root owns binding validation, focused/group/full integration and source-stable freeze. Tests establish the constructed converter caller and collection identity/state, not natural original reachability, color/node completion, original exception disposition, malformed aliases or the unrecovered 3D adaptation body.

PASS applies to the finite viewer.cpp/test_viewer_slice.cpp design and proposed caller-test contract, not an implemented/tested/frozen candidate. The other screened setter/saved-Settings gaps remain separate and do not justify expanding this patch.
