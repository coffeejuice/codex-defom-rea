# Editor font-tail inputs after opacity

The existing tail API can consume the actual editor receiver without a substitute
widget. Retained editor probes 003 and 004 directly captured its QThread-object
comparison, cached display DC, metric, shared font and widget counters. They close
the input values for those captured editor calls, independently of the SplitView
startup profile. Both calls used style1 brush opacity, however; neither is an
original style24 selected-null texture call. The public Session's post-Open
private-value bridge and intervening widget-counter population remain unclosed.

This is a read-only prerequisite audit. Only this report was written. No source,
build, CTest, GUI, original execution or mutable REA operations occurred. Source
freeze remains respected. Token usage is unavailable.

## Same editor, directly observed values

Probe004 uses native CLI Open through first-grid publication. Its selected editor
is 0x383e780, QWidgetPrivate 0x50ade90, QPaintDevice 0x383e790 and application
0xafe1c0. Hook selection is explicit: the QWidgetPrivate init checks its q_ptr is
this editor, the font constructor requires device == editor+0x10, and metric8
requires the active font call. Font/DC/thread records therefore belong to that
same actual editor call, rather than merely sharing a process or thread.

| Input | Editor004 evidence | Editor003 corroboration |
|---|---|---|
| Receiver/private | Records15/27; state3: 0x383e780 / 0x50ade90 | Records12/22; state3: 0x350e780 / 0x4e59b20 |
| Current vs application QThread objects | Record30: both 0x3490b90, equality true | Record25: both 0x3110b90, equality true |
| Natural TLS / QObject affinity | Record29/state2: QThreadData 0x3490c70; state3 and application state1 agree | Record24/state1: QThreadData 0x3110c70; same receiver/application affinity |
| Display cache at metric call | Records31/32, globals state1: cachedDC and returned DC both 0x11011293 | Records26/27, globals state2: both 0x2c012f9c |
| Actual metric8 result | Record33: signed32 168 | Record28: signed32 168 |
| Source font/private+0x180 | Record27: FontData 0x4748690, mask0, field38=168, field3c=0 | Record22: FontData 0x44f9810, same mask/fields |
| Returned font | Record34: temporary 0xaf93b8 references SAME 0x4748690, mask0; source unchanged | Record29: temporary 0x3c98e8 references SAME 0x44f9810 |
| Virtual implementation | State3: QWidget metric RVA99110, devType RVA97b0 | State3: same destinations |
| Receiver metric chain | State3: parent null; extra pointer null | State3: parent null; extra pointer null |
| Before counter increment | Record36/globals state1: current760, maximum761 | Record31/globals state2: current760, maximum761 |
| After constructor init | Record46/globals state6: current761, maximum761 | Record41/globals state6: current761, maximum761 |
| Compatibility | Same globals: compileVersion0x40804, applicationAttributes0 | Same values |

Probe004 records27..35 are one font context and records28..33 one metric context.
Both captures report one selected init/font/metric, zero failed reads and callback
errors, no message truncation and empty truncation lists. These are historical
capture receipts, not newly executed validation.

The QThread equality is directly recorded at QtGui65099181 from RAX and RBX.
It is not inferred from equal QThreadData, the OS TID, a reconstructed
WidgetThreadState or the editor's QObject affinity. The cached DC is directly
recorded before/after 6507f9d0, not synthesized from the DPI or a widget HWND.

The probe did not hook GetDeviceCaps directly. Source65099214..6509925e explains
its metric: absent extra skips the word+52 override, null parent skips parent
virtual dispatch, and the same acquired DC reaches GetDeviceCaps index0x5a (90).
Thus the observed returned metric168 plus this actual receiver state qualifies
logical DPI-Y168 for this call. It does not establish every device capability or
a permanent display configuration. Representing dpiYOverride0 in the current API
normalizes the observed absent extra's no-override branch; it does not assert a
native extra allocation containing a zero word.

Source651f2e30 compares actual FontData+38 with that metric and +3c with zero;
the matching branch shares the source, preserves the mask and skips font-data
copy. Actual QWidget devType1 skips printer-device4. Caller6506e75b passes the
same widget+10/private+180, then assigns the temporary and destroys it before
attributes34/35 and the counters. The observed source and return aliases confirm
this branch without replacing the editor font with an application-global clone.

## What can be reused

The production API is already receiver-directed:
`WidgetRuntimeState::observeConstructorTailValues(actualEditorWidget, observation)`.
It requires a live component owner in THIS runtime, its existing font owner, and
an expectedFontAlias that resolves to that same interned FontData. Metric inputs
likewise name actual live same-runtime widget owners, and a claimed null parent
is checked against actual QObject ownership. WidgetConstructorTail::resume binds
one receiver/runtime and rejects replacement on later resume.

For an explicitly admitted editor profile, this API needs no MainView or SplitView
receiver substitution and no new success boolean. Its values can be supplied as:

- Existing actual editor font alias, screen field3c0; retain its current mask and
  field38 rather than making a second font data owner.
- One profile-local positive DC observation alias for the captured cached handle,
  logicalDpiY168, currentQThreadIsApplicationQThread=true.
- Metric input on the SAME editor: kind QWidget, no override, known null parent.
- At the BEFORE-INCREMENT boundary, currentWidgets760, maximumWidgets761;
  compatibility0x40804/attributes0. Notification depth and Create receiver/filter
  inputs are separate event-phase requirements, not consequences of a font match.

The current isolated editor Fixture::tailInputs already uses exactly these
numeric editor values (not startup173) against its actual factory widget, font
alias11 and same runtime. Its tests deliberately construct their own private
inputs and brush branches. They demonstrate this composition mechanism; they do
not turn the style24-null scenario into an original runtime observation.

## What cannot be reused from startup

ConversionSession seeds one runtime for the FIRST SplitView startup constructor:
font alias1, cached-DC alias1, DPI168, QThread equality true, current/max173, and
metric inputs for the actual SplitView -> MainView parent chain. The shared tail
then advances the current widget count to174. This qualification is explicit in
ConversionSession and splitter004 reconciliation. The editor is parentless and
has a different actual font data identity in its own capture.

The DC/DPI/thread fields live on the same WidgetRuntime and the font fields on
interned FontData; metric kind/override/parent fields live on actual WidgetState.
These ownership locations are reusable architecture. Their known values are not
proof that a later native receiver has been observed. In particular, the tail
observer merges unknown/equal values and rejects a conflicting known value. It
cannot legitimately overwrite startup174 with editor760. Repeating the observer
or passing a different receiver does not recover the intervening 586 live-widget
count difference. QObject construction counters are a distinct counter and do not
supply this QWidget current/max transition. Only source-recovered lifecycle
effects or an explicitly scoped new current-state admission can establish it.

Nor may alias1 from the startup profile be reused as a new editor-font or HDC
identity merely because the displayed numeric fields match. Native pointer
identities differ across captured processes; per-profile alias joins must map the
actual current owner. No HWND is involved in this font/DC decision.

## Selected-null scope and next dependency

The static caller suffix is common: once the CP66 selected null-image finish and
CP67 bit11-clear native helper return have completed, execution reaches6506e75b.
The font tail does not read the selected brush. Its API can therefore consume
qualified actual current values on that path, and isolated tests already exercise
the composition. This is a source statement about control/data flow, not evidence
that an original selected-null capture had the same historical values.

Editor003/004 actually observed style1 brush data and opacity=true, with raw120
false. They establish the same receiver tail and its captured inputs. They cannot
be relabeled as style24, given an invented active-image producer, or replayed as
fresh public runtime defaults. Public beginEditor300Factory currently binds the
actual selected Project/Operation/GuiModel editor to the Session runtime, then
stops at PrivatePaletteUnavailable because its post-Open private aliases are not
bridged. Startup's default font/palette may not be substituted to get past it.

The smallest safe next work needs no new font-tail executor: reuse the existing
receiver-directed observer for an explicit editor profile where the actual
private/font owner has already been admitted, and retain all conflicting-state
checks. Production continuation additionally needs the source-qualified producer
of that actual post-Open profile and the intervening widget count/lifetime state.
Do not weaken mergeObserved into replacement or seed760 at SharedTail. Unknown
QThread/DC/DPI/font, foreign-thread DC, null-cache allocation, mismatching font copy
and incompatible native-window creation remain guards. Queue/post continuation
and public application-wrapper completion remain separate.

## Reproducible evidence identity

Original QtGui4_64.dll SHA-256:
`c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`.
Original QtCore4_64.dll SHA-256:
`bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.
All native addresses above use their preferred65000000/67000000 images; captured
module bases differ, and the probe resolves hooks by module-relative RVA.

| Retained artifact | SHA-256 |
|---|---|
| build/original-editor-probe-003/capture-evidence.json | 42fe8c13cfafe22d9beeac4cf87c95f48635e8f91af399241366f836e7bc6a2b |
| build/original-editor-probe-004/capture-evidence.json | e4d0a277c4bbb074fa1e038ce3870e64ebb87ec884665cb1bb0ddd0221f99ced |
| build/original-editor-probe-004/probe.js | 0251ee49c0378af50e0778c8a584a5243402b10f26ab64fc315466bf10c68203 |
| analysis/wave3-close-probe004-constructor-tail-reconciliation.json | 11b8d10f0e09f89c2555d15c11dc2cba3d0c837070a3ec2c9db0e63757f24dd6 |
| source/apps/moproj2mst/src/engine/view_manager.cpp | dc6ad8356e061bf7e9aebaa1d32ef2caa8a0d4d41da89720c023a4b825899a4c |
| source/apps/moproj2mst/src/engine/widget_events.cpp | 1ddfbe54765865f795262f93279a9ae2db36d6b640eedcce54b7c0c6f2ca14aa |
| source/apps/moproj2mst/src/engine/conversion_session.cpp | 99731a89662bac4b9162c22f0bbb649da3e65c64a5431c96048060774b4ab8a3 |
| source/apps/moproj2mst/tests/mdt/test_editor_base.cpp | 998751ffdccb5fc582ba626f993c9d0a3b62bc74f8c38172443d9150011b484a |

Source hashes identify the read snapshot only. No test was authored or executed;
root retains source/build integration ownership and the existing public guard.
