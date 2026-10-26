# Actual outer-image finish before editor opacity

Source is frozen for root validation. The selected active-null texture path now
moves its actual unique I1 once into the existing finish consumer. Only that
consumer's Returned result with a present opaque value advances to the existing
OpacityDecision. Inner and outer cleanup remain separate; ordinary image summary/
projection is bypassed. Raw120 and public/private-palette guards remain intact.

Owner: editor300. Only editor_base.hpp/.cpp, test_editor_base.cpp and this report
were edited. No texture/paint helper, runtime/view_manager, public/session,
CMake/plan, original execution, GUI or mutable REA changes. No build/CTest/tests
executed by this lane. Token usage is unavailable. Source remains frozen until
root releases it.

## Evidence and implementation

Controlling read-only report: analysis/resume66-editor-finish.md, SHA-256
`0b3e75863d444638e6c0edea88a6da1225d9f7bd90dc56d8f6b0d4de1b804f1a`.
It independently pins native image predicate/destructor/caller order and hashes
the QWidget bit17/native-opacity intervals. Original QtGui SHA-256 remains
`c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`.
No new actual post-Open private aliases or original UI reachability are asserted.

At TextureImageActiveFinish, first entry validates the held actual value's live
prefix owner and exact weak module origin against the CURRENT receiver's
paintDeviceCleanupGlobals. It then moves the unique value once into a retained
PaintDeviceNullImageFinishCall. Subsequent pending resumes use that same callee;
there is no false missing-value check against the now-empty original holder.
A const textureImageActiveFinishCall() accessor exposes the actual callee and
its same I1 prefix/global diagnostics. The old returned-value accessor becomes
empty after ownership transfer, rather than reconstructing a second value.

Every pending finish resume retains the current receiver/module-origin check.
Cached source lifetime is not required after producer return; actual I1 owns its
own lifetime. Nested finish guards return TextureImageActiveFinishPending and
cannot publish opacity, update bit17 or enter QWidget tail work.

Only Returned plus opaque.has_value() records TextureImageActiveFinishReturned,
assigns opacityDesired_ from that actual result, records TextureImageOpacityResolved
and enters OpacityDecision. The native order is therefore preserved:

`null predicate/private negation -> same I1 QImage class/data prefix -> outer
QPaintDevice word/class/tail -> new outer global getter/getter/exact0 comparison
-> I1 destruction -> result return -> palette opacity comparison -> changed bit17
store -> native-opacity helper -> later QWidget tail`.

The active path never enters TextureImageDataBoundaryReached, the ordinary
projected painting-word/class effects or textureImageCleanup_ frame. It never
reuses inner J's historical cleanup result. The old image getter remains at its
reached active dependency with no copied/retained/destroyed image-data event.
The actual finish callee supplies the independent outer prefix/cleanup receipts.

Existing OpacityDecision/OpacityEffect/SharedTail behavior is reused without
changes. Bit17 is committed before native opacity work; raw120 unknown/true still
retains NativeOpacityUnrecovered. A later raw120=false resumes only that reached
helper and tail, without rechecking an equal bit and skipping pending work or
rerunning any producer/finish operation. Completed finish receipts remain
historical after native-owner loss. No public save completion is exposed.

The former FinishUnrecovered enumerator is retained for source compatibility but
is no longer returned by this integrated stage. No new success flag, source field,
module replacement parameter, artificial yield or test-only value constructor
was introduced. The <utility> header is explicitly included for ownership transfer.

## Focused cases and source checks

Seven CP65 cases were migrated to their next real dependencies, and two new cases
were added. Existing target m2m.mdt.editor_base now has72 Release-active cases,
775 CHECK and68 REQUIRE conditions. Coverage includes:

- Alpha0/255 crossed with source words1/8000/ffff: exact one-time origin/source/
  producer/finish flow, same I1 control block, distinct inner/outer owners and
  cleanup frames, native-dead outer state after finish, no ordinary projection.
- Finish's nine native effects and inner/outer getter sequences before common
  opacity events; active-finish return before texture resolution before palette
  resolution before bit assignment; source/current palette selection preserved.
- Unknown pointer/unknown counter then0: same producer/inner cursor, no alpha,
  getter or copy replay, one consumer only after actual return.
- Active nonnull pixel guard, unknown painting word, cached-pixmap conversion,
  current disabled palette-group brush versus another alias, and per-current-
  receiver module qualification remaining explicit.
- Shared source/module with independent consumers and returned image owners;
  completed finish does not replay after source-view release or on tail retries.
- New raw120 case sets true after the constructor's zeroed-attribute stage using
  the DirectionUnavailable fixture boundary. It verifies I1 destroyed and bit17
  already true before NativeOpacityUnrecovered; repeated resumes preserve all
  producer/finish/prefix/cleanup effects; late false advances only native helper.
- New genuinely pending producer case releases source view or module before
  handoff. No consumer/result/opacity/tail is fabricated, earlier effects stay
  committed, and host abandonment invalidates both held values without claiming
  native destruction success.

No fake pause is added between valid producer return and finish invocation.
Current CP59 snapshots return only after inner counter0, so substituted unknown/
nonzero outer globals are not reachable tests. Standalone cleanup/finish tests
retain those relevant guard/lifetime checks without changing production identity.
Current raw bounded constructor starts opaque=false, so null image's true result
uses the changed-bit path; unrelated unchanged-bit behavior stays in existing
cases rather than introducing a setter to fabricate another constructor state.

Setup/checks use testkit/check.hpp CHECK/REQUIRE and remain active under NDEBUG.
Static checks passed: whitespace/conflict markers, balanced delimiters after
comment/string stripping,72 unique case names, one main, no assert and macro
argument scans. These are source checks only. Root owns focused/full compilation,
execution and acceptance; no build or CTest command was run here.

## Frozen source

Paths under source/apps/moproj2mst/:

| File | SHA-256 |
|---|---|
| include/m2m/engine/editor_base.hpp | 3ddbb205b1f05f4e09482243f1d90131f6e4d700bfdf0bc9e7641cc8bd415f29 |
| src/engine/editor_base.cpp | f6c10d2b80f0e9659ffacc6efe3cc33eef1655fe9e7f69daa3b13da9b4c9a21a |
| tests/mdt/test_editor_base.cpp | 8a4791079e1d6925b57a13b22289de4ae653c4f61d7750c53450b35c6a96690d |

Next dependency is root integration/validation, then the independently qualified
Widget constructor tail/application wrappers and actual post-Open private-value
bridge. Public ConversionSession still expects PrivatePaletteUnavailable and
loader retains not_ported6504abb0. Nonnull active copy, pixmap/conversion,
warning, lazy/nonzero cleanup, actual active-null UI reachability and full fresh
Open-Save-Close fidelity remain unestablished. This bounded value finish is not
a public mode300/constructor/save completion claim.
