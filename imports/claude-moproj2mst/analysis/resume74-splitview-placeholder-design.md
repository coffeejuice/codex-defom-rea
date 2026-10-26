# Isolated first-placeholder private prefix

Yes: an isolated native-equivalent prefix can be implemented without reading or
changing widget counts and without opening any SplitView/public editor gate.
Use a distinct placeholder value owning ONE canonical WidgetState, plus a retained
call cursor that stops before the current QPalette copy. The helper is a conditional
internal primitive; its existence must not imply that the current public SplitView
cursor has reached dab94c.

Only this design report was written. No source/build/tests, original execution,
GUI, mutable REA or new raw artifact. Token usage is unavailable. Root owns any
source assignment, integration, target wiring and validation.

## Native cut and earliest guard

CP73 qualifies application dab915/91a normal nonnull allocation0x28 and dab94c's
QWidget65070d70 call with parentnull and flags0. QWidget allocates private0x1e0,
constructs QObjectPrivate and the early QWidgetPrivate fields, then reaches
6505e8d4 QPalette6504abb0 at private+170. Current application palette ownership is
the first external value producer in this selected prefix. Font at+180, locale,
application-present/version checks, QObject construction, actual thread retain,
registry, QWidget class, init and count point are later.

The smallest helper should implement ONLY successful normal allocation ownership
and source-closed private prefix through6505e8ca. Earliest terminal dependency:

- Stage PrivatePalette, issue CurrentPaletteUnavailable when the actual receiver
  has no qualified private default palette input.
- If explicit correct inputs were already supplied, still stop at the SAME
  pre-copy boundary with PaletteCopyUnrecovered. This prefix-only increment does
  not secretly become a constructor implementation when an observer runs.

Neither guard copies palette or font, commits count174->175, sets QWidget classes,
returns the base, installs application placeholder classes, calls addWidget,
allocates a handle/second placeholder, or changes queue/callback state. Unknown
inputs remain unknown. Allocation failure and exception-unwind behavior remain
outside the normal nonnull cut; no safe failure-success return is invented.

## Owner and retained API proposal

New conceptual names under namespace m2m::engine:

- SplitViewPlaceholderValue: noncopyable, nonmovable native-value owner. It owns
  shared_ptr<WidgetState>, whose nativeIdentity is this value's stable address.
  The WidgetState's actual componentOwner control block is established exactly
  once. A const diagnostic widgetState accessor uses the same owner, not a copied
  projection or second WidgetState.
- SplitViewPlaceholderCall: noncopyable retained constructor frame. Constructor
  takes shared_ptr<WidgetRuntimeState> and nonempty source qualification; stores
  the runtime weakly and freezes that identity/qualification. First resume creates
  one unique_ptr<SplitViewPlaceholderValue>, binds its WidgetState through the
  existing public bindWidgetComponent and executes the selected private prefix.
- Report: stage NotStarted/PrivatePalette, attempted, QWidgetReturned=false,
  precise issue, plus ordered prefix effect receipts. No completed=true or native
  application-wrapper completion exists in this increment.
- Effects distinguish application storage allocation, QWidgetPrivate allocation,
  QObjectPrivate construction and QWidgetPrivate prefix. They describe the
  conditional native normal path; they are not evidence of original malloc calls
  executed by the host helper.

Keep value lifetime separate from call-frame lifetime. The retained call owns the
unique value now; no takeValue API is needed before QWidgetReturned exists. Future
unique transfer can preserve the SAME value/WidgetState identity into the actual
SplitView owner after constructor return, without cloning fields or aliases.
A standalone shared WidgetState diagnostic lease must not keep native lifetime
alive after its value is abandoned: value teardown invalidates that native owner.
Such host cleanup is not Qt destructor/counter/posted-event cleanup success.

Runtime validation precedes allocation/effects: supplied raw address AND control
block must equal the runtime's weak_from_this canonical owner; runtime must be
live/qualified. A rewrapped equal raw pointer cannot substitute its control
block. Every resume uses the same weak runtime; expired/replaced owner remains
unavailable and does not recreate the pending value. Two calls using one runtime
still produce distinct value/WidgetState identities and independent cursors.

bindWidgetComponent currently also assigns WidgetState::thread_ from runtime
metadata. For this pre-QObject interval, clear the native thread field before
publishing the prefix's value/report, as the existing editor prefix does. Binding
host runtime affinity is not a native QObject thread-selection/retain effect.
No intermediate fabricated native thread state or reference receipt may escape.
The actual runtime thread owner is not changed.

## Prefix field scope

Populate only the same source-closed private semantics already qualified by
QObjectPrivate67131160 and QWidgetPrivate6505e780:

| State | Prefix handling |
|---|---|
| Native thread and parent pointers | Null at the private constructor phase; no thread retain or parent-child insertion |
| Object name / child bookkeeping | Source empty QString and empty child/pending-property population, retained on SAME widget |
| Fresh posted count | Source QObjectPrivate67131213 initializes actual new receiver postedEvents+24 to0; no event posted/count increment |
| Object filters / send and receive child flags | Source private initial state; no application filter registry created or emptied |
| Dynamic property extra/data | Source absent extra with empty names/values; no property append or QVariant effect |
| graphics effect, fieldD0, installed layout, autofill/style/proxy/stylesheet projection | Only semantics supported by the early null private pointers; no global style or palette assignment |
| Private prefix QWidgetData flags/rect | Do not pretend all QWidgetData/attributes or the entire allocation is zero; retain represented fields as unknown until their source point |

Do not initialize extended-attribute words here: native zero stores occur AFTER
palette/font/locale and app/version checks at6505e9ab onward. Likewise opaque
private-tail masks occur later6505e94f..96c, not in this prefix. QWidget class and
QPaintDevice constructor, event receiver virtual identity, window flags, geometry,
self-focus and allWidgets registration remain unavailable. There is no flattened
private memory blob and no copying SplitView/editor state to initialize them.

The posted-count0 is specific to the source new private object, not a default for
all adapted WidgetState instances. CP69 separately pinned those bytes and their
native interval hash. It does not initialize the shared thread queue to empty.

## Candidate files and integration boundary

Suggested exclusive implementation set:

1. NEW source/apps/moproj2mst/include/m2m/engine/splitview_placeholder.hpp:
   owner/call/report/stage/effect declarations and const diagnostics.
2. NEW source/apps/moproj2mst/src/engine/splitview_placeholder.cpp:
   canonical owner checks, same WidgetState construction/binding, bounded private
   prefix, immutable guard, native-owner invalidation on abandonment.
3. NEW source/apps/moproj2mst/tests/mdt/test_splitview_placeholder.cpp:
   focused Release-active Testkit cases below.
4. Existing source/apps/moproj2mst/include/m2m/engine/view_manager.hpp:
   ONLY forward declarations and narrow friend declarations on WidgetState for
   the two new owner/call classes. Its private constructor/fields/release are
   deliberately inaccessible otherwise. No public arbitrary widget factory.

Existing WidgetRuntimeState public bindWidgetComponent and
privateConstructionInputs are sufficient; this cut needs no new runtime storage,
view_manager.cpp mutation, editor_base edit or counter API. Existing private input
observer can admit explicit inputs to the actual placeholder if later needed;
merely doing so must not resume this call or copy palette in the prefix increment.
Root separately owns CMake/test registration, suggested target
m2m.mdt.splitview_placeholder. No CMake change is authorized by this report.

There must be NO production callsite in ensureDefaultSplitView or ConversionSession
in the isolated increment. Current SplitView is still before completed QWidget
post/private+c8/Create teardown, then QFrame style/helper, QSplitter policy/event
and derived class/QString gates. No successful-count observation, source string,
ready boolean or isolated helper return can bypass them. A later hookup belongs
only at the actual derived initializer's source cursor after those effects return.
Then it must move/use this same placeholder value and preserve parentnull until
addWidget. The isolated helper does not qualify current startup counter continuity.

This option is smaller than extracting all editor_base logic into a generic
constructor now. EditorBase is explicitly tied to StagedEditor300 and factory
self-focus and has many retained opacity/helper states. A broad refactor is not
needed for a pre-palette prefix; a fake editor would be an owner substitution.
Some source-identical prefix assignments may be repeated locally in this bounded
helper until a separately reviewed common primitive is justified. That duplication
does not duplicate live state: each is the constructor of a DIFFERENT actual
native component, with one canonical WidgetState.

## Focused test contract

Use testkit/check.hpp with TEST/TEST_CASE syntax actually supplied by the target,
CHECK/CHECK_EQ/REQUIRE only and one Testkit main. No C assert or REQUIRE_EQ.
Before release, verify includes and entry point statically; root runs builds/tests.

- First resume reaches PrivatePalette/CurrentPaletteUnavailable with SAME
  canonical value/widget, null native thread/parent, source empty private fields,
  posted count0 and exact ordered once-only prefix effects.
- Repeat resume retains pointer/control block, stage and effect counts. No second
  allocation or private initialization. Runtime/thread registry membership,
  QApplication widget count/current maximum, QObject count, font/palette/locale,
  QWidget flags/geometry/attributes remain unchanged or unknown as appropriate.
- Correct receiver-specific palette observation alone changes no report/effects;
  next explicit resume changes only the precise issue to PaletteCopyUnrecovered,
  not palette data or stage. No global startup fallback is consumed.
- Null/unqualified/noncanonical runtime rejected before allocation/effects. Runtime
  expiration while pending keeps historical prefix, guards continuation and does
  not replace the owner. Foreign control blocks and foreign-receiver observations
  cannot authorize this value.
- Two calls sharing one runtime have separate value/widget identities and effects;
  abandoning one invalidates its escaped WidgetState lease without affecting the
  other's native lifetime or any runtime/global counters.
- On abandonment both before first resume and after prefix, no native QWidget
  destructor, counter decrement, registry removal, posted cleanup or addWidget
  success is reported. External diagnostic leases do not revive the value.
- With an actual isolated fixture's known historical174 runtime count, the prefix
  still leaves174 unchanged and never claims175. With unknown counts, they stay
  unknown. Use an actual independent startup fixture owner to qualify a count;
  do not fabricate an editor as the placeholder or seed a future value through it.

The prototype helper can be tested conditionally without public SplitView entry.
No test profile proves that the earlier gate chain is complete or that editor760
has been produced. Counter increment tests belong to a later actual WidgetCounts
continuation, not this prefix-only increment.

## Evidence and snapshot identities

Original QtGui SHA-256:
`c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`.
Application SHA-256:
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
CP73 retains exact bounded native/ABI captures and source order. This audit adds
only owner/API design based on the same retained evidence.

| Artifact | SHA-256 |
|---|---|
| analysis/resume73-editor-placeholder0.md | 6fdde2ee0fac142775c098400df45d379222ffb74711840a8d77697101291194 |
| analysis/resume69-editor-after-opacity.md | 3483ebec5e18f351a779248b3a8987e3a5c1e20f7a43ca0ddd46ca7703967208 |
| source/apps/moproj2mst/include/m2m/engine/view_manager.hpp | c0c146eddb8cba22f30b573a9109c97494046305dd81fdb03c2e6164433d9c53 |
| source/apps/moproj2mst/src/engine/view_manager.cpp | dc6ad8356e061bf7e9aebaa1d32ef2caa8a0d4d41da89720c023a4b825899a4c |
| source/apps/moproj2mst/src/engine/editor_base.cpp | 20063efa3b650fec2dce5274894c10d5448ad8566b26bd4dad956512e06f3817 |

No proposed source file exists from this lane. No validation pass or public
startup/editor/Save completion is claimed.
