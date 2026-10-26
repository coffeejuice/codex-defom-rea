# CP196 L5 fixture reachability

## Decision

No existing public fixture establishes a positive `step`-owned branch plus independent literal output/effect expectations for any of the four candidates. Do not assign a step-borrow cut from the current evidence.

## Frozen source and qualification

The inspected baseline is `build/wave3-checkpoint194-verified/source.zip`, SHA-256 `e837f823a179e5039ea70a05bc2820c0b16c081ea8d28e9dbed9ca39435de2b2`, matching the baseline recorded in `analysis/wave3-checkpoint194-validation.json` and the CP195 frozen-source review. The retained qualification is `source/docs/re/moproj2mst/mdt/L5/spec_L5.md`: behavior is reconstructed from historical decompiles/disassembly under `deform-rea-work/gui_mo/s1/mdt/L5/` and retained Ghidra copy; no fresh original-app run or mutable REA database evidence was used here.

## Fixture audit

`source/apps/moproj2mst/tests/mdt/test_l5_loader.cpp` admits corpus46 through `openCorpus46`, activates subproject 0, installs L5 slots and uses operation 4, a native Boolean2D operation. The Convert2Dto3D tests retag that operation; its fixture lacks a native Convert2Dto3D or CopyMirror operation. The separate oracle-B chain-head test admits operation 2 as Convert2Dto3D and checks dimension, CRDSYS, TEMP_STEP presence/deletion, and a widget geotyp value. It does not assert object-bound iteration or ordered step-owned save effects.

- `c23PostLoad`: the corpus46 retagged-load test's `load(...)` subsequently exercises the slot, with literal TEMP_STEP write/delete effects and GEO23 mode/angle checks. However it shares a substituted operation path, and its assertions do not independently establish which multi-statement step branch was reached (e.g. predecessor SimCtrl read, TEMP_STEP read, mesh/geometry imports, or work-model propagation). It therefore is not a defensible positive branch witness for a step-scoped refactor.
- `writeM23Input`: no L5 test calls save or asserts `M23_INPUT.KEY`. The current fixture does not provide a literal `step->u1378 > 0` plus active GEO23 object and independently expected writer bytes/effects.
- `c23SaveFiles`: no test calls the Convert2Dto3D save-files slot on a native-class admitted fixture with expected object/file write ordering. Its prelude can write folders, SimCtrl and objects, while its step-conditioned loop needs model/work-model state.
- `c23SyncTail`: tests shown do not execute it through a positive sync route with an admitted display scene, non-loading state and object loop; no ordered writes/reads or model effects are asserted. The code returns early on absent scene or unavailable model/step.

`CopyMirror` cannot supply a native public route: the qualified spec documents that v11.0.2's registry does not register it, so projects naming it fail open; tests register/retag it only to exercise ported slots. Corpus42 predecessor loading does prove inherited-object output for that synthetic route, but not a candidate's step-owned interval.

## Cheapest next investigation and guarded status

The cheapest next step is a bounded, read-only pass over the retained corpus index/manifests and all existing L5/MDT tests (excluding bulk mesh payloads): identify any project whose operation registry admits native Convert2Dto3D, then inspect only its task/object KEY headers, step metadata and matching expected oracle output. If none is present, check retained original oracle captures for a Convert2Dto3D Save trace that records literal `M23_INPUT.KEY` bytes and ordered file effects; do not run the original or create a fixture for this audit. A usable witness must independently pin admission, nonempty step object bound, selected object branch and expected output/effect order.

Status: guarded/no source-ready L5 candidate. Required witness is missing: an admitted native Convert2Dto3D public fixture with positive step-owned loop reachability and independent literal output/effect expectations (or equivalent original-capture witness bound to an existing fixture). No source/test/build, GUI, mutable REA DB, parent/DMO/Claude/original files were modified; only this report was written.
