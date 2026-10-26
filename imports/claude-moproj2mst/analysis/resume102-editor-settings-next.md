# CP102 editor settings branch after remesh reader increments

## Reconciliation and reachability

CP98–101 advanced the actual Project Open remesh reader before editor creation:
signed decimal integer overflow now saturates and reports ERANGE; a narrowly
qualified positive real overflow and nonzero negative-exponent underflow class
have native fixed results; and the guarded `0x`/`0X` prefix keeps the native
signed-zero/errno result. Their focused synthetic and Session/Open tests
exercise the same Session-owned `RemeshState`, with later fields continuing.
They are public Open settings effects, independent of editor palette state.
They do not construct the editor or reach page Fill. The original hierarchy is
`moproj::read.cpp` → `ConversionSession::loadProjectRemesh()` →
`loadRemeshAtProjectOpen()`; `ConversionSession::beginEditor300Factory()` and
the selected QWidget palette gate occur later and remain separate.

The bounded retained editor search in
`analysis/resume98-selected-editor-palette-retained-search.md` found no new
same-call palette getter/current-refcount evidence. Editor probe004 records the
selected actual editor's private palette/font and post-construction base
observations, not a delivered setupUi toggle. Its probe contract explicitly
does not claim a complete QWidget/setupUi/Open-Save path. So the CP98–101
Project Open advances cannot be used to infer checkbox checked state or
effective inherited enablement in the selected editor.

## Smallest independent control-state branch

The next compact source-qualified setup projection edge with no focused test
is:

`modeHeatChckBx (0xa8) --toggled(bool)--> heatCmBx (0xb8) --setEnabled(bool)`

Original setupUi source `analysis/wave2-save-diffs-300-setupui.c` explicitly
calls `setEnabled(false)` for `heatCmBx` at `0x140edc676`. Its later
connection call at `0x140f2198a` connects sender `0xa8` to receiver `0xb8`;
the retained constructor contract classifies all 14 internal routes as
`toggled(bool) -> setEnabled(bool)`, AutoConnection. Candidate
`source/apps/moproj2mst/src/engine/editor_setup.cpp` names the controls
`modeHeatChckBx`/`heatCmBx`, records the explicit false setter and the single
`{0xa8, 0xb8}` route. Existing `test_editor_setup.cpp` has heat combo model,
translation and fresh-default checks, but no focused toggle delivery case for
this route.

This route is independent of the missing selected-editor palette owner: its
bounded model inputs are the already-qualified no-active-translator setup
projection and the existing default combo model. The reportable contract is
requested local enable state only. A small Release-active test in
`tests/mdt/test_editor_setup.cpp` should assert:

- `heatCmBx` begins with `requestedEnabled == false`; its `enabled` state is
  left unknown and `modeHeatChckBx.checked` remains unknown.
- Before connection installation a supplied toggle produces no delivery;
  installing the route does not synchronize the receiver.
- A supplied true then false event yields exactly one ordered delivery each
  (`0xa8` → `0xb8`) and updates only `requestedEnabled`.
- The `heatCmBx` combo's known item/current-index model is unchanged by the
  toggle; no native click, checkbox mutation, Qt ancestry/effective enablement,
  selected-editor setupUi completion or public Fill reach is claimed.

The static source selection and current candidate mapping are enough for this
bounded projection test; native selected-editor runtime observations do not
establish that a true/false event actually occurred. This branch therefore
provides a small settings/control regression independent of the palette
producer, but it is not a public Session Fill path. For an end-to-end public
Fill claim, the existing editor constructor owner chain remains a prerequisite.

## Evidence boundary

- Original setupUi source archive: `analysis/wave2-save-diffs-300-setupui.c`,
  retained source hash `1217ddfefe72fc52f4a963f01cb4f7d10bc3a468d00e7ef1139736979483b8a7`.
- Original connection contract: `analysis/wave3-editor-constructor-prefix-contract.json`,
  SHA-256 `DF9C9A07F9D11072208148C534B92D6619DE9FD339CE7338507BFEA02D6FB670`.
- Candidate source: `source/apps/moproj2mst/src/engine/editor_setup.cpp`;
  existing test target source: `source/apps/moproj2mst/tests/mdt/test_editor_setup.cpp`.
- Remesh changes and their test boundaries are summarized in
  `analysis/resume98-editor300-remesh-overflow-implementation.md`,
  `analysis/resume99-remesh-real-overflow-implementation.md`,
  `analysis/resume100-remesh-real-underflow-implementation.md`, and
  `analysis/resume101-remesh-hex-prefix-implementation.md`.

Only this report was written. No source/build/test, GUI/original execution or
mutable REA operation occurred. Root owns any focused test implementation and
validation.
