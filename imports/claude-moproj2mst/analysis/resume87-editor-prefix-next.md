# CP87 editor300 next-prefix decision

## Decision

Do not treat the CP86 signed-FRQNCY change as a public editor300 advance. It
expands the isolated `fillAdvanced3D()` profile, but the actual Session path
still stops before setupUi/Fill. The smallest evidence-backed next step toward
public editor state is to close the actual editor QWidget private-value
producer-to-owner bridge. The current evidence is insufficient to implement
that transition safely: editor palette/font values and widget count are observed
at editor construction in retained probes, but they are not joined to the
candidate runtime's same-session application owners/count history.

The nearby SplitView placeholder prefix is a different receiver and does not
resolve the editor's missing bridge. Its next source step is also not ready:
the CP82 investigation found no qualified current palette-data refcount at the
native temporary deref point. Do not seed a count from allocation (`1`) or a
later SplitView probe value (`165`/`166`).

## Candidate reachability

`source/apps/moproj2mst/src/engine/conversion_session.cpp`,
`ConversionSession::beginEditor300Factory()`, binds the actual selected
Project/Operation/GuiModel and Session widget runtime, invokes
`continueNativeBase()`, and currently requires the report issue
`PrivatePaletteUnavailable`. The comment explicitly says post-Open private
value aliases are not bridged from startup. This is the public Session gate.

In `source/apps/moproj2mst/src/engine/editor_base.cpp`,
`EditorBaseConstruction::resume()` first records the QWidget private prefix,
then requires `runtime->privateConstructionInputs(*widget)->defaultPalette`.
With no exact-widget input, it returns `PrivatePaletteUnavailable` at
`EditorBaseStage::PrivatePalette`; font, locale, application-presence/thread
selection, QObject count, QPaintDevice and QWidget class installation are not
reached. The candidate's observed startup application defaults are not used as
a fallback.

The retained original editor receipts described in
`analysis/resume71-editor-palette-entry.md` establish that native QWidgetPrivate
construction obtains a palette from `QApplication::palette()` and font from
`QApplication::font()`. Probe004 observes the editor's actual private values;
the editor count crosses 760 to 761. The same report says those capture pointer
identities cannot be compared to a separately captured startup SplitView and
that candidate startup reaches 174 in a different receiver/time boundary.
Therefore no candidate alias or count can be inferred from those receipts.

CP81's `QApplicationPaletteOwnerState` and partial palette pointer store are
for the actual SplitView placeholder path. In
`source/apps/moproj2mst/src/engine/splitview_placeholder.cpp`, the path retains
the real getter token, writes the private data pointer once, then stops at
`PaletteTemporaryDeref`. `analysis/resume82-palette-count-source.md` qualifies
the exact native order and explains why the current word at `0x6504abce` is
unknown: the available probe reads later values on that data, not the value at
the temporary release. The getter contributes a retain before that point.
Consequently the already registered owner alone does not permit completing
the native constructor or claiming the editor has reached Fill.

## Smallest next evidence and source/test boundary

The first useful public-editor evidence item is a same-session observation at
the actual editor QWidgetPrivate default palette/font producer, joined to the
same runtime's application palette/font data identities and the count state
immediately before that editor's QWidget initialization increment. It must
retain the order and identity of the live Open path; probe-local pointers,
matching font metrics, and a later count are not interchangeable inputs.
The isolated candidate observation surface already exists in
`WidgetRuntimeState::observePrivateConstructionInputs()` and the startup owner
registry, but a new call-site must not be wired until original evidence proves
the source owner still matches.

Once that evidence closes, keep the first implementation bounded to the actual
editor's `PrivatePalette` and `PrivateFont` source outputs and a focused
`ConversionSession`/editor-base test. Assert that the same live widget receives
the observed palette/font data and source words, that retry does not repeat
construction effects, and that the next unavailable locale/thread/count
dependency remains a pending stop. Do not broaden to setupUi or advanced Fill
in the same change.

For the parallel placeholder continuation, the prerequisite is one raw read of
the same app_pal-backed palette data word at `0x6504abc3` or immediately before
`0x6504abce`, with the getter retain already reflected and its mutation sequence
joined to the exact token. Only then can the existing once-only temporary-deref
path be implemented and tested in
`tests/mdt/test_splitview_placeholder.cpp`. CP82 identifies this as missing
evidence, not a value to reconstruct from later reads.

## Existing Fill branch and validation boundary

CP86's `fillAdvanced3D()` source/test pair remains useful helper-level evidence:
`tests/mdt/test_simcontrol300_snapshot.cpp` checks the exact signed-int32
FRQNCY projection, including row-zero `-1`, custom signed text, refusal guards,
and preservation through Apply. `applyAdvanced3D()` neither consumes the
remembered frequency projection nor writes FRQNCY. Those cases validate this
bounded profile once root's frozen run executes; they do not exercise the
Session constructor path or prove public Fill reachability.

Source observations above were read from the current candidate. This report
makes no source edits and claims no build, CTest, GUI/original execution, or
mutable REA result. The outer recovery-status report concerns a separate DMO
workflow and was not used as converter/editor evidence.
