# CP105 — B109 strict raw Forming FWZ test receipt

## Test contract

Added `source/apps/moproj2mst/tests/s/test_session_b109_raw.cpp`. One test invokes the public `convert` entry on a fresh short-path copy of `109_B_FE7_before/Gear_blank.moproj` with `M2M_FLAG_WRITE_SAVE` and codepage 1252, then compares `Operations/Task00003/Forming.FWZ` byte-for-byte with the retained `109_B_FE7` output. It requires the input/output oracle sizes (1,071/1,098) and a changed output before conversion, preventing a no-op from passing. It rejects path-bearing oracle content and applies no line ending, number, path, view, or banner normalization.

The scratch root is a unique `C:\PROBLEM\b109_<pid>_<tick>` leaf. The test checks every copied/generated path is shorter than 100 UTF-16 code units, restores the current directory before cleanup, and recursively deletes only that newly created leaf after validating its parent and prefix. It does not run the original executable, claim fresh-original parity or native Close, or overlap the DBSTEP geometry test.

CMake currently discovers test source files with a `GLOB ... CONFIGURE_DEPENDS` over `tests/*/test_*.cpp`; this new path matches that rule. No CMake file was edited and no configure/build/test was run.

## Retained provenance and immutable input/output binding

The retained fixture tooling `evidence/gui_mo/oracle_b/_tools/mkl4w.py` creates `109_B_FE7` from the GUI-saved `95_B_FE3ACT` profile, sets the active FormingExpress `BlowEnergy1` XML value to `1.23456789`, and makes the `_before` copy. `evidence/gui_mo/oracle_b/_tools/run_109_B_FE7.log` records the GUI Save as 4,300 bytes with GUI exit code 0. `analysis/resume105-raw-comparison-coverage.md` separately identifies B109 as the narrower Forming.FWZ candidate; it notes the FWZ and MST are path-free and this row also appears in the existing save-current oracle table. This test is a new full public WRITE_SAVE raw comparison of the FWZ only, not a claim that the GUI schedule or editor event sequence was reproduced.

| Artifact | Size | SHA-256 |
|---|---:|---|
| `109_B_FE7_before/Operations/Task00003/Forming.FWZ` | 1,071 | `6ff1aa1952526905b741dda3e970f46543b1bd5234250b079fd9084d76929c44` |
| `109_B_FE7/Operations/Task00003/Forming.FWZ` | 1,098 | `35ae7b62904990d976f815235f7e057cfd26dbc258d1d86700175ad4568976c9` |
| `evidence/gui_mo/oracle_b/_tools/mkl4w.py` | 3,220 | `e8f7e8b2507607ee1e495820a48d3d5490f88da18a1604287d057baab125d7ee` |
| `evidence/gui_mo/oracle_b/_tools/run_109_B_FE7.log` | 335 | `38843b7fd332a3ca6efaba188fbfde4918e572a74d73fcd16dd0de236950f1f7` |
| New test source | 4,085 | `65924fe049e89f1d90de18d19e3e48b79cc0f5a6b33502325c8960c9d5999dad` |

Static preflight verified the two retained byte counts/hashes, the B109 source generator and GUI Save log, `tests/s` CMake discovery, the unique output path, bounded scratch deletion, and that only the new test and this receipt were written. No build or runtime result is claimed.
