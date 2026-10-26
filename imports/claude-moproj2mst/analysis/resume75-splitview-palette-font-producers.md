# CP75 — SplitView placeholder palette/font producers

## Finding

The isolated placeholder prefix currently stops at `PrivatePalette`: it admits a private-construction observation, then returns `PaletteCopyUnrecovered`. The next bounded native mutation is the QWidgetPrivate `QPalette` copy into `+0x170`, with the placeholder's raw group/resolve word `0`. The evidence supports this as a same-process shared-data identity with MainView and SplitView; it does **not** yet supply provenance for the current candidate runtime's QApplication default palette/cache owner. The corresponding font copy at `+0x180` is a separate subsequent mutation and must remain unclaimed.

## Original evidence and producer order

The retained original startup capture is `build/original-splitter-probe-002/capture-evidence.json`, hash `bdb95dbe5840180a1b1bb178faafe997accbff835f66242e1a3c4618d8d47fca`. Its reconciliation is `analysis/wave3-close-splitter-probe-002-reconciliation.json`, hash `3be51baafe0f1a89d5611943656c89ef42ccdd08a084c3af4a3739d7bbd2c07f`. This capture reaches the original `d35a80` return, records 100 observations and 43 states, and reports no failed reads, callback errors, or truncations. It is startup evidence only; it does not reach Open/Save acceptance.

In that single original process, MainView, SplitView, placeholder0, and placeholder1 carry the same font data pointer (`0x4919e30`, resolve mask `0`) and palette data pointer (`0x400edb0`, group/resolve word `0`). Their locale index is `0x187`. All 18 final font/palette/locale equality calls return true. Reconciliation explicitly limits that result: it closes comparison outcomes, **not** the earlier QApplication getter/cache effects. The observations therefore qualify same-process identity and value shape; they do not prove that the current candidate's runtime aliases point at those original addresses or that its getter cache has been prepared.

Static QtGui evidence binds the producer order:

1. `QWidgetPrivate` constructor `0x6505e780` calls `QPalette::QPalette()` `0x6504abb0` into private `+0x170` at `0x6505e8d4`. The palette constructor calls `QApplication::palette()` `0x65016f70`, which reads/initializes the application default wrapper and copies its value to the output.
2. It next calls default `QFont` constructor `0x651f3560` into private `+0x180` at `0x6505e8de`. This calls `QApplication::font()` `0x6501b330`, copies the data, and leaves the destination resolve mask `0`.
3. QLocale construction follows at `0x6505e900`; it is a later producer and outside the proposed palette-only increment.

Useful static artifacts: `analysis/wave3-closure-next-6505e780.asm`, `...-6504abb0.asm`, `...-651f3560.asm`, `...-65016f70.asm`, `...-6501b330.asm`, and the getter analyses `analysis/wave3-closure-next-close-splitter-{palette-global-getter,font-global-getters,widget-palette-getter}-static.json`. These are source/binary path qualifications, not runtime getter observations.

## Counter ownership and placeholder ordering

Original probe002 starts with QApplication `0xcfe480`, threadData `0x3690c70`, widget-list owner `0x36943a0`, and QWidget total/maximum `173/173`. SplitView's QWidget init begins with 173 and returns at 174. The parentless placeholder0 begins with 174 and returns at 175. Then handle0 advances 175→176, placeholder1 advances 176→177, and handle1 advances 177→178. The QWidget total and maximum are QtGui globals at `+0x9c9948` and `+0x9c994c`; the native increment/max update is in QWidget constructor tail `0x6506e7ab..0x6506e7c9`, after palette/font/device/attribute initialization. This is distinct from QObject's separate global count. Thus the placeholder palette/font step itself must not advance the QWidget counter; that remains a later constructor-tail effect.

The native SplitView initializer calls QWidget for a parentless placeholder before `QSplitter::addWidget` reparents it. This agrees with probe002's `widget_init_enter` parent null followed by post-construction parent assignment. The constructor's palette/font values are therefore produced before its later splitter-parent context.

## Candidate gap and missing provenance

Current prefix: `source/apps/moproj2mst/src/engine/splitview_placeholder.cpp` allocates and binds the actual `WidgetState`, resets host thread metadata, initializes the observed QObjectPrivate/QWidgetPrivate null prefix, and enters `PrivatePalette`. It requires `runtime->privateConstructionInputs(*widget)->defaultPalette` with matching qualification, but deliberately returns `PaletteCopyUnrecovered` even when supplied. It does not mutate palette, font, locale, or QWidget counters.

The existing candidate startup observation is in `source/apps/moproj2mst/src/engine/conversion_session.cpp::freshConverterWidgetValues`: it creates runtime-local font/palette aliases and default values (`font {alias 1, mask 51}`, palette `{alias 1, word 13893616}`), plus cache flags. Those aliases are candidate IDs, not native pointers. `WidgetRuntimeState::observePrivateConstructionInputs` in `view_manager.cpp` interns observations in the same runtime and verifies default-data-owner agreement, while retaining the per-widget palette word. This is a useful admission mechanism, but the startup profile is not a new observation from this actual placeholder's QApplication getter/copy stage. In particular, the original placeholder's local palette word is `0`, whereas the candidate startup default profile records `13893616`; shared data identity does not make those local value fields interchangeable.

Missing input provenance is a qualified, same-candidate-runtime observation that binds the current QApplication default palette data owner to this actual placeholder's constructor input and records the local group/resolve word `0` (and, later, an independently qualified QFont owner plus mask `0`). No cross-process address or alias equality should satisfy it. Probe002 did not hook getter entry/return or read the getter cache slot, so it cannot fill this gap on its own.

## Smallest next increment / file boundary

After a source-qualified actual-widget input is available, the smallest next state mutation is exactly one copy of the qualified palette value into the placeholder's private palette field, retaining the data owner and setting the observed group/resolve word to `0`. Advance the internal stage/cursor only after that copy is represented once; preserve a distinct pending state if owner or value provenance is absent. Do not include font, locale, QObject completion, QWidget counter, parent insertion, class installation, or a public SplitView callsite in this increment.

Candidate boundary: extend the existing isolated primitive in `source/apps/moproj2mst/src/engine/splitview_placeholder.cpp` (and its header stage/issue contract if needed); keep evidence admission tied to `WidgetRuntimeState::observePrivateConstructionInputs` and the exact `WidgetState`. No generic runtime fallback to `Values::defaultPalette`, no public callsite wiring, and no claim of complete QWidget construction. Parent/public composition remains outside this prefix.

## Evidence limits

The native capture is the original shipped QtGui process; the assembly artifacts are offline static analysis of the retained QtGui binary. Candidate observations are source state only. No new original capture, build, GUI interaction, or REA mutation was performed for this report. Relevant hashes: reconciliation `3be51baafe0f1a89d5611943656c89ef42ccdd08a084c3af4a3739d7bbd2c07f`; current placeholder source `80427dacd058628cfdb6af0b699ee4cd43b861f1c041abf99e1f20e8a1ae0e04`; candidate startup source `99731a89662bac4b9162c22f0bbb649da3e65c64a5431c96048060774b4ab8a3`; input admission source `dc6ad8356e061bf7e9aeba1d32ef2caa8a0d4d41da89720c023a4b825899a4c`.
