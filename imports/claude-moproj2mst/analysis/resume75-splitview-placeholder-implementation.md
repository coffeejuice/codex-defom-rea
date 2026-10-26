# CP75 isolated first-placeholder prefix implementation

Source is frozen for root build/integration. The new isolated first-placeholder
value/call models the normal nonnull application storage and QObjectPrivate/
QWidgetPrivate prefix, then stops before QPalette copying. It owns one actual
WidgetState and never reads/updates QWidget counts or opens a public SplitView,
Editor300, wrapper or Save gate.

Owned source is three new helper/test files plus exactly four forward/friend
lines in view_manager.hpp. No view_manager.cpp, editor_base, ConversionSession,
CMake/build wiring, shared plan, GUI, original execution or mutable REA changes.
No compiler/build/CTest or test execution occurred. Token usage is unavailable.
Root explicitly released this source window after checkpoint74 archive freeze.

## Native evidence and bounded implementation

The cut follows resume74-splitview-placeholder-design.md and CP73's pinned source
chain: application dab915/91a allocation0x28, dab94c -> QWidget65070d70, private
allocation0x1e0 ->6505e780 prefix through6505e8ca, before palette call6505e8d4.
QObjectPrivate67131213 initializes this NEW receiver postedEvents+24 to0; it does
not post an event, alter a queue or increment receiver/global counters.

SplitViewPlaceholderValue owns one canonical WidgetState/nativeIdentity whose
address is the value itself. Copy/move is unavailable. SplitViewPlaceholderCall
owns that value uniquely and retains the frame independently from diagnostic
shared WidgetState leases. No early takeValue/returned-value API is exposed.
Value abandonment calls existing native-owner invalidation, so leases cannot
observe a live component after the value dies. This is host lifetime cleanup,
not a Qt destructor or a native counter decrement.

First explicit resume validates its frozen nonempty qualification and canonical
runtime owner/control block, then creates/binds the one value and performs the
source prefix. Runtime affinity is metadata: native thread is cleared before
publication, with no QObject thread-selection/retain receipt. It initializes only
source private semantics: null parent/no parent membership, empty object name,
child/pending/property bookkeeping, initial send/receive-child flags, fresh posted
count0, absent effect/layout/style/proxy/extra projections. It does not initialize
later opaque masks or extended attributes, font/palette/locale, QWidget classes/
data, flags, geometry, self-focus, registry or either global counter.

The exact four ordered effects are PlaceholderStorageAllocated,
WidgetPrivateAllocated, QObjectPrivateConstructed and WidgetPrivatePrefix.
Repeated resumes retain the same value/widget/control block and those effects.
Normal allocation failure/unwind remains outside this conditional prefix; no
replacement success behavior is invented.

Earliest stage is PrivatePalette:

- CurrentPaletteUnavailable if this receiver has no qualified private palette.
- PrivateInputsQualificationMismatch if its supplied receipt names another call.
- PaletteCopyUnrecovered for matching explicitly admitted palette inputs.

Even the last case copies nothing and returns no QWidget. The observer itself
does not resume the helper. A global startup palette is not a per-placeholder
input and cannot skip CurrentPaletteUnavailable. A known historical174 count is
left174; unknown count stays unknown. No175 or760 is seeded.

Null/missing/unqualified runtime and foreign control blocks fail before any native
prefix effects. Canonical ownership is checked in the constructor BEFORE storing
its weak runtime; a rejected foreign block is never dereferenced on a later retry
after the canonical object is freed. Valid runtime expiration preserves the
historical prefix/cursor and reports RuntimeUnavailable. An external host bind
that reassigns the native thread in this pre-QObject interval reports
PrefixStateChanged instead of silently reinitializing the source fields.

## Consumer interface and ownership boundary

Header: m2m/engine/splitview_placeholder.hpp; namespace m2m::engine.

`SplitViewPlaceholderCall(const shared_ptr<WidgetRuntimeState>&, string qualification)`

`const SplitViewPlaceholderReport& resume()`

Const diagnostics: report(), effects(), qualification(), value().
Value diagnostics: widgetState(), nativeOwnerAlive().
Stages: NotStarted and PrivatePalette. Report attempted tracks prefix entry;
qwidgetReturned and completeForPublicSave are permanently false.

view_manager.hpp has ONLY two forward declarations and corresponding WidgetState
friend declarations. This was compared directly with checkpoint74 source.zip:
four added lines, zero removals. Existing public runtime bind/input APIs suffice;
no new runtime container or unqualified WidgetState factory was added.

No production caller invokes this helper. Actual future SplitView entry must
first return through QWidget post/private+c8/Create teardown, QFrame style/helper,
QSplitter policy/event/class/orientation and derived QString construction. Its
actual derived cursor can then use this same value. The helper cannot act as a
ready boolean or bypass those earlier gates. AddWidget, handle0, placeholder1,
notifiers, Primary replacement and count-history recovery remain separate.

## Focused cases and static preflight

New test_splitview_placeholder.cpp has14 unique TEST_CASEs,138 CHECK expressions,
11 REQUIRE expressions and one Testkit main. Checks are Release-active; no C
assert or unsupported REQUIRE_EQ. The cases cover:

1. Constructor/report inspection creates no native prefix/value/effects.
2. One pre-QObject canonical widget has source private fields only; later data,
   registry, queue and counter effects are absent.
3. Repeated prefix retries preserve exact value/effects/count0.
4. Correct palette observation does not execute copying or later phases.
5. Actual independent startup-owner fixture keeps global default palette unused
   and historical current/max174 and separate QObject98 unchanged.
6. Missing runtime/qualification/runtime receipt reject without prefix.
7. Equal raw runtime with foreign control block is rejected, including retry
   after canonical owner release without touching the dangling wrapper pointer.
8. Runtime loss before first resume creates no value.
9. Runtime loss after prefix preserves cursor/effects; abandonment invalidates
   the diagnostic widget.
10. Foreign runtime/rewrapped WidgetState input observations cannot authorize it.
11. Another same-runtime placeholder's private inputs do not satisfy this one.
12. Abandonment invalidates only its own value; independent same-runtime call
    remains pending and global counters stay unknown/unchanged.
13. A changed native thread is guarded rather than reset on retry.
14. Input qualification cannot be replaced to join a different source call.

The test's historical-count fixture uses actual independent MainView/manager
owners and an explicitly isolated current/max input, not a fake editor in place
of the placeholder and not a continuous startup/Open claim. An invalidated widget
may retain its historical weak runtime binding while nativeOwnerAlive is false;
tests check this existing semantic and rejection of later private observation.

Static checks completed: new quoted includes resolve against the app include
root; Testkit header/macros/main were read; no trailing whitespace/conflicts;
comment/literal-stripped delimiters balanced; all CHECK/REQUIRE/TEST_CASE macro
arguments have supported arity; names are unique. git diff --check produced no
errors, and an independent whitespace scan covers the new untracked files too.
No execution pass is claimed. Root owns suggested target m2m.mdt.splitview_placeholder
and its14-case focused build/run, then grouped/full validation and source freeze.

## Frozen source hashes

Paths relative to source/apps/moproj2mst/:

| File | SHA-256 |
|---|---|
| include/m2m/engine/splitview_placeholder.hpp | 71dae7b930cb7b9e4f79379dfbe79b89a73b292be4f794954f09e10831ffb0f9 |
| src/engine/splitview_placeholder.cpp | 80427dacd058628cfd6baf0b699ee4cd43b861f1c041abf99e1f20e8a1ae0e04 |
| tests/mdt/test_splitview_placeholder.cpp | cf83d21b881704100e4740875aab569575e62d1724969dd74e214067cb369d7a |
| include/m2m/engine/view_manager.hpp | bb988d0359cff29c84987de66ef11b7166d98725b7738e53c5c12e74c9ea250c |

Native evidence identity remains original application
fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc and
QtGui c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887.
CP73/74 retain bounded source/artifact qualifications. No new native observation
or original runtime value is introduced here.

Next dependency: actual palette copy/current owner producer on this placeholder,
with the earlier public SplitView gates still closed. Full parentless constructor
and its own174->175 native counter point are not part of this increment. No
continuous Open-to-editor760 or public Open-Save-Close completion is claimed.
