# Resume 46 geometry fixture binding: Cogging 131R

## Exact case and retained input

`B:131R_B_COG_NEWOP_RESAVE` is an Oracle-B generated/resave fixture. The exact project basename and Oracle-B directory are established by `build/test-runs/1792861755173151100/fixtures-before.json` entries:

`gui_mo\oracle_b\131R_B_COG_NEWOP_RESAVE\Cogging_Multiple_Ops_Lab2.moproj`

The same manifest lists `Cogging_Multiple_Ops_Lab2.MST`, `.MST.oracleA`, `Objects\Object00001.KEY` through `Object00016.KEY`, and `Operations\Task00001` through `Task00004` files. The expected local fixture root is therefore `imports/claude-moproj2mst/evidence/gui_mo/oracle_b/131R_B_COG_NEWOP_RESAVE/`, based on the imported evidence corpus layout; the exact project path relative to the checkout is:

`C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\gui_mo\oracle_b\131R_B_COG_NEWOP_RESAVE\Cogging_Multiple_Ops_Lab2.moproj`

This mapping comes from the fixture manifest and imported evidence layout, not the Oracle-A `runner/cases.tsv`. The Oracle-B source index is `source/docs/re/moproj2mst/runner/cases_b.tsv`, but it does not include this label in its current imported rows; do not use an A-corpus `COG_P1_B1` project as a substitute.

## Status correction

The original `source/docs/re/moproj2mst/s/folder_sweep.txt:135` and `analysis/wave1-final-tests.json` record an earlier refusal: 3D scene box requested its geometry-list item rect (`0x140c8e0f0`, `m+0x108`) while that item method was then untraced. That refusal is historical, not the current expected result.

The later `analysis/wave2-geometry-validation.json` reports `B:131R_B_COG_NEWOP_RESAVE` as `SAME` with zero differing/missing/extra entries. The checkpoint 45 test receipt `analysis/wave3-candidate45-doe-fix-d-tests.json` and its matching fixture snapshot receipt under `build/test-runs/1792861755173151100/receipt.json` also report `SAME`, zero differences, and no comparator adjustments. Thus the smallest current regression reproduction is the single retained Oracle-B fixture above through the existing folder-save comparator; expected outcome is comparator `SAME`, not a geometry refusal. This is historical comparator evidence and does not prove raw byte identity or fresh-original parity.

## Smallest geometry refusal condition still evidenced by source

For a focused branch reproduction without running a project, the minimum viewer state is:

1. A bound 3D view with a nonempty geometry list (`view.items` nonempty).
2. At least one geometry item with an embedded `geometry->mesh` whose node and element counts are both positive, making `refreshGeometryItem` set `rectKnown = false`.
3. A shown view (`settings.flag[1]`) with flags 2, 3, and `0x2d` all clear, so `viewerSceneBox` takes the model-rectangle fallback and detects `modelRectKnown == false`.

That state returns `M2M_E_UNSUPPORTED` from `viewerSceneBox` with the current message `3D viewer: a bound geometry-list mesh item rectangle (0x14126d0c0 -> 0x14124bec0) is not traced`. This is the narrow source-level reproduction condition. The `131R` fixture is a useful end-to-end regression but is no longer a valid refusal reproducer after packet V/viewer changes.

No converter, fixture, build, app, or REA database was modified or executed for this binding. The expected refusal status is derived from current source guards; identifying a corpus fixture that meets all three predicates still requires inspecting copied project geometry records.
