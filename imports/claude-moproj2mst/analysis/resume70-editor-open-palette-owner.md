# CP70: editor Open palette/font owner and widget-count boundary

## Smallest qualified stage

The smallest source-qualified producer is `QWidgetPrivate` construction for
the actual parentless editor widget, before its private palette/font values are
consumed by QWidget initialization. Native path:

- `QWidget::QWidget` (`QtGui4_64.dll 0x65070d70`) allocates private state and
  calls `QWidgetPrivate` (`0x6505e780`).
- At `0x6505e8d4`, the constructor calls `QPalette::QPalette()` at
  `0x6504abb0` for private `+0x170`. That default constructor obtains the
  current application palette via `QApplication::palette()` at `0x65016f70`.
- At `0x6505e8de`, it calls the default `QFont` constructor at `0x651f3560`
  for private `+0x180`; that obtains the current application font via
  `QApplication::font()` at `0x6501b330`. The Qt font constructor sets its
  output resolve mask to zero at `0x651f35c6`.
- These are values owned by the same runtime: palette/font data identities
  must resolve through its existing interned shared owners. Numeric content or
  aliases from another process are not interchangeable.

The corresponding candidate boundary is
`EditorBaseConstruction::PrivatePalette` in
`source/apps/moproj2mst/src/engine/editor_base.cpp`. It obtains
`runtime->privateConstructionInputs(*widget)` and returns
`PrivatePaletteUnavailable` if the exact widget has no default palette input.
Only after that stage can it enter `PrivateFont` and require the font owner.
`ConversionSession::beginEditor300Factory` explicitly preserves this expected
guard and performs no native UI setup. The smallest admission needs actual
post-Open palette and font shared-data identities, resolve/group words and
qualification bound to this same editor widget/runtime, in constructor order.

## Counter transition and why startup inputs do not close it

The QWidget tail increments the QApplication current-widget counter at
`QtGui4_64.dll 0x6506e7b2` (load `+0x9c9948` at `0x6506e7ab`, increment, store
at `0x6506e7b5`) and updates the maximum at `+0x9c994c` when the new signed
count exceeds it. The original editor probe’s before/after observations are
current `760 -> 761`, maximum `761` unchanged. That is the actual editor
constructor boundary in those captures, not a startup default.

The reconstructed `ConversionSession` seeds current/maximum `173` for the
first SplitView constructor tail. Its one source-ordered increment produces
`174`. Thus the editor capture's pre-increment current value is 586 above that
startup value, and its maximum is 761 rather than 174. The difference is a
net count/history gap; available evidence does not enumerate all intervening
widget constructions and destructions, or prove they all occurred monotonically
before editor entry. QObject's separate counter cannot fill this QWidget
counter gap. Replacing runtime state with 760/761 would erase unmodeled history
and conflict with the existing same-runtime merge rules.

The minimum production contract is therefore an actual-owner editor-entry
admission that supplies the current palette/font owners and the count state at
the correct stage, with provenance tying both to this Session runtime. If the
public Open path itself must derive rather than receive that boundary, the
intervening QWidget create/destroy sequence and max-count history must be
recovered before applying the existing `WidgetConstructorTail` increment.
Current evidence does not choose between those integration forms. The startup
SplitView profile must remain distinct; do not relabel its values or write
future 760/761 into it.

## Retained observations and limit

Original probe003 and probe004 are native CLI Open through first-grid
publication. Probe004 directly observes the editor's private font data,
resolve mask and metric fields; private palette data and group/resolve word;
and current application modal/layout state at qualified endpoints. It also
observes the selected palette brush and opacity route. Probe003 supports the
font/device and editor widget-count results but missed the selected
palette-getter enter/return records. Neither receipt covers full Save/Close,
and each has process-specific addresses. The captures establish a usable
same-native-call profile, not candidate Session state.

Relevant source/evidence:

- `analysis/wave3-editor-parentless-base-cursor-design.json` identifies the
  `PrivatePalette` / `PrivateFont` producer calls and their source order.
- `analysis/wave3-editor-probe004-palette-binding-contract.json` records
  actual probe004 private palette/font and current-group consumption.
- `analysis/wave3-editor-probe003-base-grid-reconciliation.json` records the
  probe003 current/max widget count and its capture limits.
- `analysis/resume70-editor-font-tail-inputs.md` qualifies the actual editor
  tail inputs, startup `173 -> 174`, and why the editor's `760/761` cannot be
  merged into startup state.
- `source/apps/moproj2mst/src/engine/conversion_session.cpp` seeds the
  startup tail values and retains the `PrivatePaletteUnavailable` public gate;
  `source/apps/moproj2mst/src/engine/view_manager.cpp` enforces same-runtime
  private input ownership; `source/apps/moproj2mst/src/engine/widget_events.cpp`
  applies the constructor counter increment.

This is a read-only source/evidence audit. No source, build, test, GUI, or
mutable REA action was performed. No runtime capture or full public Open-to-Save
completion is claimed. Token measurement is unavailable.
