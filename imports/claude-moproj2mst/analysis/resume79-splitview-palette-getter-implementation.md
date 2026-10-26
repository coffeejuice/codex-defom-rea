# CP79 retained same-placeholder QApplication getter output

Source is frozen/released for root integration. A runtime-mediated no-provider
read now validates the actual receiver and canonical registered app_pal owner.
The isolated placeholder binds that owner weakly, retains one completed getter
output once, and stops at PrivatePaletteCopy BEFORE6504abcb's private data-pointer
store. Private palette/refcounts/font/counters/public gates remain unchanged.

Root released this six-file lane after CP78 archive. No application_palette_owner
or other gradient/helper source, CMake, editor/session/public callsite, shared plan,
original execution, GUI or mutable REA was changed. No build/compiler/CTest/tests
were run. Token usage is unavailable. Gradient confirmed disjoint geometry files.

## New runtime seam

`QApplicationPaletteRead WidgetRuntimeState::readApplicationPaletteFor(const shared_ptr<WidgetState>& receiver, const shared_ptr<const QApplicationPaletteOwnerState>& expectedOwner)`

Before calling the helper, this method requires the canonical live runtime,
actual live receiver/component control block, same exact runtime binding, and
expected owner raw/control-block equality with the registered Values owner. For
a present wrapper, it verifies that SAME data owner remains interned under its
alias in this runtime. Invalid ownership throws M2M_E_UNSUPPORTED before read.
No receiver private defaultPalette/input record is required or created; the
getter is that value's producer, not its consumer.

The helper read receives NO constructor provider. Unknown returns SlotStateUnknown
with no output/effects. KnownNull returns InitializerUnavailable without wrapper
allocation/construction/cache or startup default substitution. KnownPresent returns
one copied value sharing exact data and preserving wrapper word, with OutputCopied
only. The global wrapper remains unchanged. Repeated direct runtime calls are
separate getter calls; once-only behavior belongs to the retained caller frame.

The shared header adds one result forward declaration, one explanatory comment
and the method signature. It does not include the owner helper and cause a cycle.
The implementation already includes the complete CP77 result/helper definitions.

## Placeholder cursor and ownership

The original private-prefix/legacy isolated-input guards remain intact when no
registered app_pal owner exists. Explicit private observations alone still cannot
manufacture a getter output or execute copying. Once the registered owner exists,
PrivatePalette binds its weak identity once and enters ApplicationPaletteGetter.
A conflicting private-call qualification is not bypassed.

This stage resolves SAME weak owner and compares it with runtime's current
canonical owner before the validated read. Nested pending reports are retained
in applicationPaletteRead(); late observation alone never advances a call. A
successful report/output is stored once, emits caller receipt
ApplicationPaletteGetterReturned and advances to PrivatePaletteCopy. Later resume
validates owner/lifetime but does not reread or replace the completed result.

Const diagnostics added:

- `const QApplicationPaletteRead* applicationPaletteRead() const noexcept`
- `weak_ptr<const QApplicationPaletteOwnerState> applicationPaletteOwner() const noexcept`

The result is a stable member in the same nonmovable call frame. Its returned
temporary/data lease is distinct from app_pal's wrapper and from the actual
placeholder value. Two calls share module owner/data but own separate result
storage/cursors. The caller receipt is a getter-return boundary, not a second
native OutputCopied or private palette construction.

PrivatePaletteCopy reports PaletteCopyUnrecovered. The native next operation
6504abcb has NOT run: widget.palette() remains absent, including a deliberately
nonzero getter word. No temporary deref, private word0 store, destination ref,
font/locale copy, native QObject/thread retain, QWidget data/class/registry or
current/max increment is represented. QWidgetReturned/completeForPublicSave stay
false, and no public SplitView/Editor300 entry is wired.

Runtime expiration preserves pending/completed historical output and effects,
then reports RuntimeUnavailable before further native activity. Escaped diagnostic
owner/data leases cannot keep the runtime callable. Host abandonment releases C++
metadata leases and invalidates the placeholder's native owner, without claiming
Qt temporary/widget destruction, counter decrement or posted cleanup. A failed
owner preflight never becomes a new source binding.

## Source qualification

This is the cut proposed in resume79-splitview-palette-getter.md and CP78's exact
native composition audit:6505e8d4 ->6504abb0 first calls65016f70 into its local
output, then6504abcb stores destination data. Native temporary deref, optional
60-brush destruction/free, private word0 and destination ref are later. The
current optional WidgetPaletteValue cannot represent that intermediate private
pointer with unknown word, so stopping before its store preserves the boundary.
No source refcount branch was inferred from shared_ptr use_count or a success
boolean. Original slot state remains an explicitly qualified input, not a new
observation acquired by this worker.

## Focused tests and preflight

Existing test_splitview_placeholder.cpp keeps its14 earlier prefix cases and adds
7 cases (21 total;230 CHECK expressions,28 REQUIRE expressions):

1. Unknown getter then late KnownPresent observation resumes SAME owner once;
   retains full nonzero output, exactly one OutputCopied/caller-return receipt,
   unchanged private fields and no replay on repeated PrivatePaletteCopy resume.
2. KnownNull remains initializer-pending despite existing startup defaults;
   no wrapper, with known current/max174 and separate QObject98 unchanged.
3. Getter requires no completed private record; retains nonzero word while all
   private/font/thread/registry fields and historical counters stay unadvanced.
4. Completed output/data diagnostics survive runtime loss as history; no reread
   or private copy, abandonment invalidates the widget.
5. Two values share canonical module but have independent output storage and
   native lifetimes; abandoning one does not affect the other's retained result.
6. Wrong private receipt stops before registering a call-bound owner/read result.
7. Runtime loss while Unknown getter is pending retains its cursor/report without
   owner replacement or allocation replay.

Existing test_widget_application_palette_binding.cpp adds4 cases (15 total;
77 CHECK expressions,7 REQUIRE expressions): unknown/null no-provider semantics;
present same-data/full-word OutputCopied without private-input prerequisite;
missing/foreign/copied/rewrapped owner rejection; and null/foreign/rewrapped/dead
receiver/runtime rejection. The canonical wrapper remains unchanged.

Both targets keep one testkit::run_all main and supported Release-active macros.
Static preflight passed: quoted include resolution, complete/forward type and
header-cycle review, comment/literal-stripped balanced delimiters, unique case
names, macro arity, no assert/REQUIRE_EQ, no trailing whitespace/conflict markers.
Direct diff against CP78 source.zip confirmed ONLY the six approved files and
bounded runtime/placeholder/tests changes; no other production caller added.
No execution pass is claimed. Root owns grouped focused targets
m2m.mdt.splitview_placeholder (21) and
m2m.mdt.widget_application_palette_binding (15), then complete validation/archive.

## Frozen source hashes

Paths relative to source/apps/moproj2mst/:

| File | SHA-256 |
|---|---|
| include/m2m/engine/view_manager.hpp | b7ec2ca09e342a2fd5760b7f97c236fc23b159d54a1a9094dadb614d4793aba5 |
| src/engine/view_manager.cpp | 958324aa0d33696057bec472b72bb149be20c28dd155f1eead54d8539e1f8718 |
| include/m2m/engine/splitview_placeholder.hpp | d9d6a335d5d8ac7be4ddff5ec86504ed72fc7c6cac72b9e9b66accf9ce90354b |
| src/engine/splitview_placeholder.cpp | 9db88e6ca7144796e88b91192af3ec5221d3d16ac7a35314cf81401371e1668d |
| tests/mdt/test_splitview_placeholder.cpp | c441260a74879b6b02f53eed8d26747456415d917d81676ed769cdda981db4d2 |
| tests/mdt/test_widget_application_palette_binding.cpp | fe14fb63d1ce700ba32cdcaea8f6b782a02448cc7bd05999a89669d355349bde |

Native QtGui SHA-256 remains
c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887.
No new original runtime value was observed. Next dependency is source-ordered
private pointer store/temporary release/word0/destination-ref closure; known-null
initialization, font, earlier public SplitView gates and counter history stay
separate. No continuous startup/Open-to-editor760 or Save/Close completion claim.
