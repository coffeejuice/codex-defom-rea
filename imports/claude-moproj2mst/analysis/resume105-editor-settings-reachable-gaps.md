# CP105 editor settings and public Save gaps

## Reconciled reachability

CP98–102 remesh reader changes are source-backed Project Open behavior on the
Session-owned `RemeshState`; they run before editor construction and do not
reach page Fill. CP102 adds the `modeHeatChckBx` → `heatCmBx` setup projection
test, but it supplies a projected toggle event and remains below the native
editor gate. Probe004 observes selected-editor QWidget base values through the
first grid publication, not a delivered setupUi signal or public Save. The
bounded retained search in `resume98-selected-editor-palette-retained-search.md`
still finds no same-call getter/current-palette-refcount witness to pass the
selected editor's palette constructor.

CP103 identifies the first unresolved public Save writer path: enabled remesh
Save's unchecked `fopen("wt")` can reach the original default invalid-parameter
handler and terminate the process before later writes, Close or status return.
This needs a parent-owned process-terminal result design and isolated child
acceptance test; an ordinary writer failure test or editor setting test cannot
stand in for it. CP104 separately concludes there is no candidate
application/main owner with a source-qualified constructor, lifetime and
Open/Save/Close callsites to wire. No safe public editor or Save implementation
slice follows from these reports alone.

## One disjoint bounded settings test

The smallest remaining untested internal enable edge is:

`m_chkbxSTRETCH (0x1538) --toggled(bool)--> groupBox72_2_2_2 (0x1540)`

Original setupUi explicitly calls `setEnabled(false)` on receiver `0x1540` at
`0x140f18418`. Later it connects sender `0x1538` to receiver `0x1540` at
`0x140f21bcb`; the retained
`wave3-editor-constructor-prefix-contract.json` classifies the edge as
`toggled(bool) -> setEnabled(bool)`, AutoConnection type 0. The existing
candidate `editor_setup.cpp` already records the names, initial requested false
setter and `{0x1538, 0x1540}` route. The current test search finds no
`0x1538`/`0x1540` coverage in either `test_editor_setup.cpp` or CP102's new
heat-route test.

Proposed post-CP104 slice: add one standalone Testkit file,
`source/apps/moproj2mst/tests/mdt/test_editor_setup_stretch_route.cpp`, leaving
the shared production source, CP102 test file and large `test_editor_setup.cpp`
untouched. The existing CMake `CONFIGURE_DEPENDS` glob will register it as
`m2m_mdt_editor_setup_stretch_route` / `m2m.mdt.editor_setup_stretch_route`.
Use the existing setup projection: bind the default combo model, apply the
no-active-translator prefix, then exercise this one route. Assert requested
false initially, unknown effective enabled and sender checked state, no event
before connection or synchronization on connection, and exactly one ordered
delivery for each supplied true/false value. Assert only receiver
`requestedEnabled` changes. Do not claim a native toggle occurred, parent-tree
effective enablement, selected editor setupUi completion, or public Fill.

This is a regression slice for the already modeled signal-to-setter contract,
not a public-reachability fix. No production change is justified by the
available evidence. The root may proceed with it independently while the
selected palette and public terminal Save dependencies remain gated.

## Evidence inspected

- Original setupUi decompile: `analysis/wave2-save-diffs-300-setupui.c`,
  SHA-256 `1217ddfefe72fc52f4a963f01cb4f7d10bc3a468d00e7ef1139736979483b8a7`.
- Original constructor connection contract:
  `analysis/wave3-editor-constructor-prefix-contract.json`, SHA-256
  `df9c9a07f9d11072208148c534b92d6619de9fd339ce7338507bfea02d6fb670`.
- Candidate route: `source/apps/moproj2mst/src/engine/editor_setup.cpp`,
  SHA-256 `f6b0075090d77c64fef400bc9cb72c63f7558d6734e95b60184b7028dbacab22`.
- Public Save boundary: `analysis/resume103-editor300-public-failure-next.md`.
- Application owner/lifetime gap: `analysis/resume104-application-owner-integration-review.md`.
- No new capture or state mutation was performed. This report is the only write;
  no source, build, test, GUI or REA database changes were made.
