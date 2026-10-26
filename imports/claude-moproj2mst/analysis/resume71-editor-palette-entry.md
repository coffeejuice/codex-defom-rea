# CP71 editor palette/font entry: actual owner and count boundary

## Original producer-to-widget transition

The first values consumed by the editor's actual QWidgetPrivate are produced
by that same construction call, from the current QApplication state:

1. `QWidget` entry at preferred QtGui address `0x65070d70` calls
   `QWidgetPrivate` at `0x6505e780` after allocating the private object.
2. Private ctor instruction `0x6505e8d4` calls `QPalette::QPalette()`
   `0x6504abb0` with output at private `+0x170`. That constructor calls
   `QApplication::palette()` `0x65016f70` and copies the returned palette's
   shared data into the output.
3. Instruction `0x6505e8de` calls default `QFont` ctor `0x651f3560` with
   output at private `+0x180`. It calls `QApplication::font()` `0x6501b330`,
   copies the returned shared FontData, and sets destination resolve mask 0
   at `0x651f35c6`.
4. QWidget initialization `0x6506e4d0` later consumes the private palette/font.
   For palette, its parentless branch calls `QWidget::palette()`
   `0x65064410`, resolves the current group/role brush, and uses that brush in
   opacity selection. For font, common tail call `0x6506e75b` passes the same
   widget paint-device and private `+0x180` font to `QFont(QFont const&,
   QPaintDevice*)` `0x651f2e30`.

Probe004 directly observes the actual editor's private palette/font and later
palette getter/brush use. Its receipt records font data/mask/metric and palette
data/group at private-return, then a palette getter and returned group0 role10
brush. These are one native editor call, but the raw pointers belong to that
captured process. Probe003 corroborates the editor font/device and widget-count
tail, while its palette getter hook was not selected; it does not add a direct
getter receipt retroactively.

## Candidate handoff and missing same-owner bridge

The existing candidate runtime already holds application-global observations
for the startup `MainView`/`ViewManager` profile: `freshConverterWidgetValues()`
interns application font/palette aliases and sets those same values on the
actual startup widgets. The editor is a different `WidgetState` later created
by `StagedEditor300`. Its `EditorBaseConstruction::resume()` looks up
`privateConstructionInputs(*widget)` by that exact widget identity; it does
not fall back to the runtime's application-default values. After the source
private prefix, it stops at `PrivatePaletteUnavailable` when no per-widget
record exists. `ConversionSession::beginEditor300Factory()` preserves that
guard and does not allocate editor setupUi.

`observePrivateConstructionInputs(actualWidget, observation)` is the intended
same-runtime entry. It requires a live widget owned by this runtime, interns
palette/font aliases, and checks that supplied default data does not conflict
with the current global shared owner. This permits binding an editor to an
already-qualified identical owner, but does not prove that the startup alias
still names the application palette/font consumed later during Open. The
retained probe003 and probe004 are separate native processes with unrelated
pointer identities; matching metric values or resolve words do not join those
owners. No evidence reviewed here certifies an intervening global palette/font
replacement or proves its absence. Consequently, neither a probe pointer nor
the startup local alias is itself a valid editor-owner bridge.

The smallest useful production boundary is before the first palette copy at
private `+0x170`: for the actual selected editor and current runtime, qualify
the application palette data identity and raw group/resolve word returned by
`QApplication::palette()`; then qualify the corresponding application font
data identity, source fields and returned mask at `QApplication::font()`.
Bind these through the existing interned runtime owners and the actual editor
widget's private-input record, preserving the source order. If the values
cannot be joined to the candidate's existing current application owners,
retain the guard and recover the state-changing producer instead of replacing
shared runtime values.

## Keep the QWidget count history separate

At editor `QWidget` initialization, the native current-widget counter is
loaded from QtGui `+0x9c9948`, incremented at `+0x506e7b2`, and written at
`+0x506e7b5`; maximum is at `+0x9c994c` and is raised only when the new signed
current count exceeds it. Probe003/004 observe the editor's boundary as
current `760 -> 761`, maximum `761` unchanged.

Candidate startup seeds current and maximum `173` for its actual SplitView
constructor tail; that one tail increments them to `174`. This is a different
time and receiver. The editor observation is a net current-count gap of 586,
with a larger high-water mark. The available evidence does not enumerate the
widget creates and destroys that produce the intervening state. The
`WidgetConstructorTail` observer merges values for the same shared runtime; it
cannot be used to overwrite startup `174` with later `760`. The count must be
advanced by recovered actual widget lifetimes or admitted from a qualified
same-session state boundary before the editor's own single increment. QObject
construction count is a separate global and does not supply this history.

Palette/font owner qualification and widget-count history are independent
prerequisites: closing one does not authorize the other, QObject wrappers, or
setupUi.

## Evidence limits and identities

Addresses above use preferred QtGui image base `0x65000000`. Original binary
evidence uses `QtGui4_64.dll` SHA-256
`c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`.
The startup/editor count reconciliation and prior font-tail audit are retained
in `analysis/resume70-editor-font-tail-inputs.md`. Probe data contracts are
`analysis/wave3-editor-probe004-palette-binding-contract.json` and
`analysis/wave3-editor-probe003-base-grid-reconciliation.json`.

Inspected candidate files: `src/engine/conversion_session.cpp`,
`src/engine/view_manager.cpp`, `src/engine/editor_base.cpp`,
`src/engine/widget_events.cpp`, `include/m2m/engine/view_manager.hpp`, and
`include/m2m/engine/editor_base.hpp`. This report changes no source and claims
no build, test, GUI, mutable REA, or public Open-to-Save success.
