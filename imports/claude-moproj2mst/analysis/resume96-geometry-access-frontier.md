# CP96 geometry access frontier after the nonparked query

No additional scalar-only source edit is recommended in this checkpoint.
The useful remaining boundary is the initial low-record query that deliberately
installs a persistent barrier before mutating native version. Its access scope
needs explicit design. Preserve write-only frame snapshots and historical
pointer observations. This report changes no source.

## Remaining access inventory

| Access | Current role | Decision |
|---|---|---|
| modelDim(const GuiModel&) nullable S+0x1c read | Actual dimension for refresh/scene/classification and const guiModelDim diagnostics | Keep const helper/read-only semantics; current mutable withStep is not a drop-in replacement. |
| N/E owner comparison with actual model.step | Provenance qualification before any owner field read | Keep these identity guards; no scalar field remains hidden in them. |
| Frame step/View/object/geometry/mesh/N/E identities | Historical observations of the one suspended call | Do not convert by saving a withStep pointer. |
| frame.oldVersion / entryCache before install | Write-only scalar observations | A void copy is mechanically possible but adds no observable ownership guarantee or direct runtime assertion. Avoid churn. |
| Installed-frame query plus postVersion/stage | Initial invocation continues after installing its own barrier | Ordinary withStep at the current call position self-refuses; deliberate prepared-call access scope needed. |
| Ordinary high-record query | Now scoped by CP95 before any frame install | Complete for the bounded query; existing later pending guard remains. |

A callsite-only dimension copy in the actual-N branch is technically possible
without changing modelDim's signature: void callback could copy nullable
nativeStep.dimension into a stack int. It is not an architecture blocker.
However the normal public geometry-item route has already selected dimension3
in refresh, and that duplicate copy offers little new ownership/test value.
Do not claim there is no live scalar left; there is no justified next scalar
change of comparable value here. Const diagnostics must not gain a new
mutable-capability guard or const_cast merely to remove the shared helper read.

## Frame pointer lifetime: established behavior

Source search finds GeometrySyncBarrier::step declared const DB_STEP_DATA*,
assigned once before installation, and never subsequently read, compared or
dereferenced. The recorded geometry pointers and scalar snapshots are private
metadata. pending/requireAvailable inspect only model.geometrySyncBarrier_
presence; install accepts only a new nonnull frame on an unparked model.
GuiModel's own mutator guard likewise tests presence. No pointer validity test,
frame reset, retirement or resumed native continuation exists.

Current semantics therefore remain safe against treating a stale recorded
pointer as permission: external raw step/Viewer/DB changes cannot remove the
private barrier or initiate its continuation. Existing parked tests alter
version/cache/global tables, swap/reset Viewer and expire DB provenance while
requiring continued pre-effect refusal. The metadata adds no native ownership
or lifetime extension. Model teardown destroys adapter storage without
dereferencing the historical step key. Public raw fields/preheld pointers
remain unintercepted legacy access, not barrier authorization.

GuiModelGuard proves the model's lifetime, not the lifetime of a separately
replaceable native unique_ptr step. A pointer-address match alone also cannot
prove that a future allocation at that address is the same native object.
Saving the borrowed nativeStep into this frame would violate the synchronous
withStep contract even though current code never dereferences it afterward.

## Why a new ordinary borrow cannot start after install

The low-record branch currently prepares all frame identities/scalars, installs
the frame, invokes the closed native directory query, records postVersion and
QueryCommitted, then returns the pending result. Installation precedes the
query's conditional step_version write so another refresh cannot observe a
new low version and silently enter the old-version fast path.

Calling withStep at the query or postVersion position checks the now-present
barrier and throws before the original operation. Moving install after query
would break the established version-write/retry ordering. Weakening general
requireAvailable or clearing the frame would expose existing pending bypasses.
Neither is a scalar-access migration.

There is a possible narrow next design for the same initial invocation:
acquire a checked access scope immediately BEFORE install, then perform only
install→existing closed query→postVersion/stage within that synchronous scope.
It would preserve historical pointer preparation outside the borrow and
return only scalar success/failure. This option does not require pretending
the current installed-frame pointer is a lifetime lease. It does, however,
intentionally cross barrier installation, so it needs separate root review
as a prepared-call scope rather than an incidental access replacement.

Required invariants/tests for that later design:

- Entry must be unparked, the prepared frame must belong to the actual model,
  and an install failure must leave the existing pending result with no query.
- Install stays before native version mutation. No event, DB reload, owner
  replacement, unknown callback or public mutation may intervene in the scope.
  The actual model owns its native step for this same synchronous invocation;
  the existing Project.dataDb lease pins the actual unique reader.
- Preserve queryResult, all four output addresses/values, postVersion and stage
  timing. A native exception must leave the already installed prepared frame
  and committed partial effects, with no replay/retirement.
- Keep model/step pointers out of callback results and deferred captures.
  Public retry/mutator/reset and same-refresh-next-View gates remain unchanged.
- Existing low-record matrix (5999/0/-1/INT32_MIN), high-record twelve rows,
  physical bounds/receipt reload cases, and actual two-View parked retry test
  must pass unchanged. Any new prepared-scope failure behavior needs its own
  meaningful focused regression, not an accessor for private snapshots.

This scope addresses only the current initial query. Any continuation in a
later invocation needs a separately qualified step allocation observer or
generation and complete model/Viewer/item/DB receipt revalidation, plus an
explicit abort/retirement contract. Existing public replaceable step ownership
cannot establish that provenance by an invented counter or pointer equality.

## Evidence and validation gates

CP71 qualifies GUI query0x140825440..0x140825532 and conditional version setter;
CP77 binds actual strong DB owner/current receipt/physical bounds; CP78/79
qualify durable parking before a low-version write. Original target SHA-256 is
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
CP95's report records the closed ordinary query and unchanged installed-frame
branch. No new native evidence or runtime behavior is asserted here.

No source/test/API/layout/CMake/ABI edit is proposed now. Existing45-case
m2m_mdt_viewer_box / CTest m2m.mdt.viewer_box remains the focused runtime target;
root owns source-stable build/full test/export/ABI/archive acceptance. Preserve
existing bool/void access policy, pointer/reference rejection, native layout,
GuiModel step type, APIversion1 and C signatures.

## Read-only SHA-256 bindings

| File | SHA-256 |
|---|---|
| source/apps/moproj2mst/src/engine/viewer.cpp | 7274e7772e6dc0c6dbf02a9a0811ee77e65bf9b683e474831cab54f3f9797f75 |
| source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp | 58ddea703f0695e371d2c379ae18e9f070b0403732d9842a3377390b60510b98 |
| source/apps/moproj2mst/src/engine/gui_model_access.hpp | e05944babf29b0477452ca497d7df836ac57fa822056088e4c693a079465fa26 |
| source/apps/moproj2mst/include/m2m/engine.hpp | 77153701684b2644d0b470c23ba110c9ce51ad8cbabbc82ab008b8b9c174128f |
| source/apps/moproj2mst/src/engine/gui_model.cpp | 26768d26df546b8335a48f2374b5743c4a4b60242ff61631326652e8fdba0d45 |
| analysis/resume95-geometry-access-next.md | 7bfa5d982312bb0dcfbd060dc2c563944956b2b4b3c3fd96339f407c4a1f707b |
| analysis/resume78-geometry-query-cursor.md | 586378b2b903ef6de4dfe99285598c7fb9824f31b75b42ffcc1d2ed8e53ad4b1 |
| analysis/resume79-geometry-pending-barrier-plan.md | b34dae42d845a033be9ba8ce3ec1329cf520980ef149d82c5be4fbb91b464f82 |

Worker wrote only this report; no source/build/CTest/GUI/mutable REA action.

## Completed prepared-query scope design (read-only)

Root requested exact acquisition/release and same-instance qualification after
the initial frontier report. The initial low-record invocation can use the
existing helper without changing the public barrier if its guard is acquired
immediately BEFORE installation, rather than trying to enter it afterward.
This is a bounded same-invocation scope; it does not create a resumable lease.

Exact sequence and ownership:

1. Keep all current model/step/N/E qualification, native prefix, timers, DB
   presence and strong-owner identity checks. Local `dbOwner` already retains
   actual Project.dataDb before resolving `reader`. Preserve this lease through
   all receipt checks and the prepared query; it is released on the same outer
   branch return/unwind as today. Receipt is plain observational data, not an
   independent reader/directory owner.
2. Keep actual View/item qualification, frame allocation, historical identities,
   oldVersion/entryCache and sampledEdges exactly where they are. The local
   frame owns only adapter metadata. Copying its raw step key adds no native
   lease and must remain outside the new callback. No new identity read/check
   of frame->step is needed.
3. At the current install statement position, invoke withStep(*model,...).
   Its existing preflight must see an unparked model before obtaining actual
   model.step.get(). This is the only new step-borrow acquisition. Do not move
   it earlier across frame allocation or later past install.
4. Inside that synchronous void callback, perform exactly current install;
   on install failure return from callback without querying. On success,
   invoke the same step_dir_sizes using nativeStep, write queryResult to the
   same frame, read postVersion from that same nativeStep, then assign the same
   QueryCommitted stage. End callback there. Outer code still returns the same
   HighVersionDbLoaderPending regardless of callback early/normal return.
5. Borrow release is callback/std::invoke return or exception unwind. No pointer
   result or deferred capture survives it. The installed model shared_ptr keeps
   metadata parked; DB/frame locals leave scope at the same outer return.

Concrete review shape, not an implementation in this checkpoint:

```cpp
GuiModelAccess::withStep(*model,
    [model, reader, index, &frame](DB_STEP_DATA* nativeStep) {
        if (!GeometrySyncAccess::install(*model, frame)) return;
        frame->queryResult = def::dbio::dbread::step_dir_sizes(nativeStep, reader, index,
            &frame->hsize, &frame->marSize, &frame->arraySize, &frame->recordVersion);
        frame->postVersion = nativeStep->step_version;
        frame->stage = GeometrySyncBarrier::Stage::QueryCommitted;
    });
return GeometryReadiness::HighVersionDbLoaderPending;
```

This preserves same-instance pointer identity under the existing serialized,
callback-free prefix. Between historical step observation and new acquisition,
the code copies only scalar/raw/weak metadata. Within the callback, install
only writes model.geometrySyncBarrier_; the complete qualified native query
reads actual index/directory and conditionally writes native step_version;
postVersion/stage are observations. None assigns/resets GuiModel.step or
executes an event/unknown callback. The same owning GuiModel supplies the live
unique_ptr allocation for the entire synchronous call. There is therefore no
source-supported replacement interval to cross, and no need to save nativeStep
or treat frame->step as authorization.

Installation continues to precede native query/version mutation. General
requireAvailable is not weakened: later public entries still refuse. The
already executing callback does not re-enter it after install, matching the
original initial invocation's deliberate continuation to its closed query.
It invokes no public mutation entry that would now self-refuse. A native
exception still leaves the installed PreparedForQuery frame and already
committed effects; there is no catch/replay/retirement added.

Meaningful acceptance for a separately approved implementation:

- Preserve the existing four low-version rows and first-invocation exact
  version/timer/COW/pending results, then require no second native getter/query
  on retry. The composite two-View parked test must keep the second View out.
- Strengthen the existing actual directory-query helper with an actual native
  step identity captured before refresh and REQUIRE the same model.step.get()
  immediately after each refresh, before using its saved native reference.
  Apply to high and low rows without a new public diagnostic API. This checks
  the native query never replaces the actual step on these admitted calls.
- Retain actual reader/receipt/index bounds and exact query/version partial
  effects, all public mutator/reset refusals and externally altered provenance
  tests. No observed frame pointer/scalar needs exposure.
- Static reversal must isolate only install/query/postVersion/stage wrapping;
  verify pointer observation stays outside, install remains before query, all
  seven arguments/output addresses/result storage remain, and callback ends
  after stage. The unreachable normal single-thread install-failure branch
  remains an early return with no query. Do not fabricate a callback or corrupt
  a live receipt to force a native failure merely for test coverage.

No missing native-body evidence blocks this initial scope: CP71/77 qualify its
complete closed query and owner/extent chain. The remaining acceptance decision
is root approval of the explicit before-install access scope and its focused
identity assertion. No source edit is made by this design. The hard lifetime
blocker remains any later resumed native continuation: externally replaceable
unique_ptr step allocation has no independent tracked observer/generation,
and public raw writes/preheld pointers cannot be intercepted. Such a later
capability must never reuse historical frame pointers based only on address.
