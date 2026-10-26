# CP237 FormingExpress dimension flags implementation

## Change

Added `GuiModel::setFormingExpressDimensionFlags()` and routed the existing
`feDimFlags` leaf through it. The owner command returns for a null step and
does not add a geometry admission check. It checks `dimension == 2`, writes
`+0xa34`, `+0xa68`, then `+0xa30` in that order, and writes `+0x1118 = 0`
only in the 2D branch. Every non-2D value writes 2 to the first three fields
and preserves `+0x1118`. `feLoadPrologue` and its call position were not changed.

The direct Testkit case constructs `GuiModel` fixtures for dimensions 2, 3,
and 7, seeds all four native fields, and verifies the branch values and the
non-2D sentinel. This is a command test; it does not test loader dispatch.

## Validation

- Focused candidate build: `python admin/build_candidate.py --label cp237-forming-express-flags --target m2m_mdt_engine_l3` — passed; source-stable build receipt is `analysis/cp237-forming-express-flags-build.json`.
- Focused CTest: `ctest --test-dir build/candidate --output-on-failure -R "^m2m\.mdt\.engine_l3$"` — 1/1 passed.
- Test executable SHA-256: `cabc8c41bd27e611f78e8fa68cf97a8347baa1342e5fe8712b352b102758f211`.

## Source hashes

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/include/m2m/engine.hpp` | `3065824177bc8102a327caa2decebaca0d8e6f60b38ed2f04b74c6bba0beb04d` |
| `source/apps/moproj2mst/src/engine/gui_model.cpp` | `641641907a36a34e0c08c5bcb791ad0e7c68dfce72435611781ea2df3de46b22` |
| `source/apps/moproj2mst/src/engine/loader_l3.cpp` | `3e8c234fd687df2b9db81e3a02ecf2edee225a67e5b38116b4b7b0a3715731c0` |
| `source/apps/moproj2mst/tests/mdt/test_engine_l3.cpp` | `68a1f75decdd6aa37ec2dc8c0577b9393406b98e188d386b3561b613ad76f37b` |

## Evidence limits and ownership

The retained L3 corpus has no FormingExpress operation. The helper address
annotation and field behavior are source/decompilation-backed, not a fresh
dynamic run of the original. Accordingly, this implementation and test make
no caller/original parity claim. The prologue's existing control and write
order remains unchanged by source review. Assigned files are ready for
integration; no full build, freeze, original application, GUI, or REA work was
performed here.
