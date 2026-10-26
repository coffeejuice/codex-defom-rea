# CP64 remove unqualified geometry arithmetic from the recovered result path

The unqualified populated fallback should become a guarded adapter refusal.
Original evidence does not supply a route that bypasses unknown getters and
still enters the rectangle builder. This is a correction to reconstruction
admission, with a real reduction in historical synthetic acceptance. It is not
evidence that the original application returns an unsupported error.

Owner: `gradient_state`. Only this report was written. No source/build/test,
GUI, new native capture or mutable REA action was performed. Token measurement
is unavailable.

## Exact current route

Reviewed frozen viewer SHA-256
`ed6cec89c659c52dc2e3de4fd99ffe4aefac6fbd2199d2f5e42dd98726be5f83`
and viewer-test SHA-256
`c06f25b6a0ec89a0b0f04f74c4db1a3a28ffdecd7b96ef1bb8f2d662cde4b720`.
Relevant source locations in that freeze:

1. `viewer.cpp:488`, `qualifiedGeometryReadiness`, returns Unqualified for
   missing coordinates, any E owner, noncanonical registry, mismatched/unavailable
   actual N owner, or actual N owner with zero u1b58. These are preflight guards
   before newly admitted native getter effects.
2. `viewer.cpp:578`, `refreshGeometryItem`, handles Skip only. Unqualified still
   calls `geometryMeshRect` at582's preceding expression.
3. `viewer.cpp:514`, `geometryMeshRect`, invokes the second getters only when
   readiness is Populated. Unqualified uses raw N+8/E+8 instead. With otherwise
   valid populated arrays/layout/counts/indices/values, arithmetic succeeds and
   the mesh rectangle is unioned into the edge rectangle with rectKnown=true.

The resulting classification is inconsistent: a state explicitly denied getter
qualification can still be published as a known result of the getter-dependent
native builder path. Missing coordinates already fail raw builder checks; the
affected success cases have both arrays present.

## Why raw arithmetic is insufficient

Original target is the retained DEF_GUI_MO_64.exe, SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
The complete predicate and caller qualifications are in
`resume60-geometry3d-next.md` and `resume62-geometry-owner.md`; CP63's owner gate
qualification is in `resume63-geometry-owner.md`. Their original binary,
function bounds, native-byte hashes and capture hashes remain controlling.

- Native `1408ee940` calls the coordinate getter, tests its returned pointer,
  then calls connectivity and tests its returned pointer. Caller `14126d266`
  branches on that result at `d26d`; false jumps past mesh work to `d3c7`.
- On true, the caller reads count/count/width and calls coordinate getter at
  `14126d2c5`, connectivity getter at `d2d4`, then builder `14124bec0` at `d305`.
  There is no demonstrated raw-builder bypass for an unqualified getter state.
- Coordinate getter `140854af0` dispatches registered variables. Collection
  `140b3e8e0` performs mutable/COW lookup at `e937`, dereferences the variable's
  vtable at `e949`, and invokes slot2 at `e95f`. Null entries can fault there;
  custom implementations have unknown effects. They cannot be assumed inert
  merely because source coordinates are populated.
- Exact Node id0 takes the key0 resize branch in `1409e49d0`, including a
  dimension read at `1409e4a1e`. With no owner, dims is null. With an actual owner,
  this may mutate variable state. The raw fallback does neither.
- Opaque nonnull N owners supply an address used as owner+15f8 and subsequently
  owner+1b58; opaque E owners cause a read of owner+1b59. The raw fallback avoids
  these native dependencies and therefore cannot establish the same path/result.
- Actual N owner with u1b58=0 has a real native gate-false route. The shared
  port's unconditional counterpart cannot qualify omitting that synchronization.

These facts require retaining an unresolved boundary in the reconstruction.
They do not imply every noncanonical registry or nonnull owner is invalid in the
original. Some narrower cases may later be admitted with separate method/owner
proof. Native faults and unresolved mutations must not be recast as native
known-false readiness or silent edge-only skips.

## Smallest source successor

In the positive-count block, handle all three readiness states explicitly:

- Skip: preserve the already sampled edge rectangle as known and return.
- Unqualified: set `it.rectKnown=false` and return before builder/count-width
  arithmetic. Preserve the edge prefix but do not advertise it as a complete
  known contribution.
- Populated: use the existing count/count/width/second-getter/builder route.

Optionally make `geometryMeshRect` immediately reject a readiness other than
Populated at its entry, before any field reads, so its single current caller
cannot accidentally reintroduce the fallback. Do not add a new owner, dimension
default, flag write, allocator or virtual call to turn an unqualified state into
a result. Keep zero-count early skipping before readiness, the CP62 ownerless
route, CP63 actual-owner/nonzero fast path, and existing malformed-builder guards.

Adapter refusal should use existing propagation: rectKnown=false feeds
modelRectKnown at `viewer.cpp:836`. Only when `viewerSceneBox` actually selects
the rectangle candidate does its existing check at1102 throw
`M2M_E_UNSUPPORTED`. Other display modes using the already recovered model box
are not forced to fail. Do not throw during refresh or describe the original
native selector as producing that adapter error.

## Historical acceptance impact

CP56's raw rectangle reconstruction accepted populated arrays without obtaining
native getter readiness. CP60/62/63 deliberately preserved that behavior while
adding qualified effects to narrower branches. The CP63 report records this
limitation explicitly; it must not be retroactively described as already refused.

`geometry_actual_owner_getter_admission_keeps_unqualified_profiles_guarded`,
`test_viewer_box.cpp:685`, currently requires rectangle success for populated
mode0 (actual owner/zero byte), mode2 (opaque E owner), mode3 (another model's
valid N owner), and mode4 (id0 variable). Those four matrix entries become
unsupported when rectangle selection needs this contribution. Populated mode1
(missing coordinates) and all five null-connectivity entries already refuse.

The CP63 integrated receipt `resume63-integrated-full3-tests.json` records
`m2m.mdt.viewer_box` passed in0.0804321 seconds with no gap events, diagnostic
differences or truncated/incomplete output. I read that receipt, not reran it.
It validates the historical assertions too; it does not certify the fallback as
original behavior. No retained real-project record proves the optional mesh's
populated unqualified owner/registry profile, so external corpus impact remains
unmeasured. Do not claim this is behavior-neutral or that corpus parity improves
until root compares the required fixtures.

## Exact focused test changes

Root successor owns viewer.cpp and test_viewer_box.cpp; no shared interface or
CMake addition is needed. Target `m2m_mdt_viewer_box`, CTest
`m2m.mdt.viewer_box`.

1. Replace the CP63 matrix's populated raw-success exception with unsupported
   for every unqualified profile. Retain zero getter dispatch, unchanged COW
   body and unchanged actual flag assertions. Cover opaque N owner as well as
   another valid model identity; compare either without dereferencing it.
2. Extend the existing registry matrix to populated connectivity as well as null:
   id0, null entry, exact custom-derived method and changed registry count/order
   remain unsupported. Custom slot call count stays0. Hold a COW list-storage
   lease to require no detach on rejected preflight, with proper registry restore
   before destruction and no duplicate ownership of variable pointees.
3. Add known-populated -> unqualified-populated -> known-populated refresh:
   valid state yields Ymax1; unqualified state makes selected scene rect
   unsupported; restoring actual source owner/registry yields Ymax1. This proves
   stale successful arithmetic is not reused after refusal and rectKnown can
   recover on the next fully qualified refresh. Preserve array identities.
4. Preserve known connectivity-null skip as successful edge-only Ymax0, including
   shared-list detach once, and both populated getter/COW profiles as success.
   A rejected unqualified profile must not be silently turned into that skip.
5. Keep zero-count contribution known even with irrelevant unqualified owner or
   registry fields, since native counts precede all getters. Keep ordinary
   malformed builder refusals and current field/fixture cleanup discipline.

Use CHECK/REQUIRE with the existing Testkit entry point. Root must run focused
tests and complete source-stable validation and record the deliberate admission
change. No new source or test was authored or executed in this audit.
