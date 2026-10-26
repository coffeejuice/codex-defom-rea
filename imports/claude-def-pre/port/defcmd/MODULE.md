# defcmd: module card

## Purpose

The command keywords of DEF_PRE_64 batch mode (the cases of the dispatcher `FUN_140378e00`) and
the Fortran kernels behind them: object positioning (OBJPOS with interference and drop),
contact generation (INICTC, GENCTC 2D/3D), mesh generation front ends (BRDEXT, DEFAMG, DEFINT,
mesh3d), geometry primitives (GEOPRIM extrude/revolve, CNVT3D), object commands (CPYOBJ, WRTOBJ,
SVSCAL, DOESCL), GENAXS/GENSUB/GENBCC/TCYCLE, DXF/IGES/STL/NASTRAN import, STEP.DAT, and the
launch of external DEFORM programs (DEF_MORP2, DEF_REM, mesh generators).

## Interface

* `defcmd/dispatch.hpp`: `dispatch(model, file, id)` (one command keyword), `write_key_file`,
  `write_key_stream`, `finish_key_read`, `db_find_step`.
* One header per command family (`objpos*.hpp`, `contact_cmds.hpp`, `genctc3d*.hpp`,
  `cnvt3d*.hpp`, `brdext.hpp`, `geoprim/`, `meshgen/`, `drop/`, `ctc3d/`, `ifd/`).
* `defcmd/launch.hpp`: child processes. DEFINT/REMESH 3D paths start 64-bit programs only when
  `64bit.DAT` exists in the working directory (`docs/re/commands/DEFINT.md`).

## Fidelity class

F1 bit-exact. Fortran `-O2` reassociated sums in places; read the assembly, not only the
decompile (Ghidra groups some Fortran sums wrongly).

## Oracle tests worth knowing

The `*_oracle` and `*_replay` tests replay cached runs of `tests/commands/<CMD>/` cases through
`tests/cmd_oracle_runner.hpp`. `tools/int/run_int.py` runs the OBJPOS drop and contact labels one
per process; `tools/int/e2e_stdout.py --all` runs every command case through the real
`def_pre.exe` (518 pass, 3 known failures at int-all).

## Perf baseline

Not measured yet. Record machine, build (Release, commit), input, wall time and peak memory, as
the `profile` skill (.claude/skills/profile) prints them.

## Swap notes

Candidates for evolve-mode swaps: the 3D contact search (`ctc3d/`), geometry kernels and the
triangulator behind GEOPRIM revolve, the DXF/IGES/STL readers. A swap keeps the command's KEY
output and messages within the oracle, or documents each accepted difference. GEOPRIM revolve
calls `srand(time)`, so its cases need the pinned clock.

<!-- BEGIN GENERATED: tools/agents/gen_module_cards.py; do not edit by hand -->

Directory `port/defcmd/`, CMake target `defcmd`.

* Depends on: `defkey`, `defdbio`
* Used by: `def_pre_core`
* Files: 292 sources/headers, 85 test files, 86,702 lines
* Public headers (81): `defcmd (top level)` 53, `defcmd/drop/` 10, `defcmd/ctc3d/` 7, `defcmd/ifd/` 5, `defcmd/meshgen/` 4, `defcmd/geoprim/` 2

### Binary functions ported here: 1116 `@src` addresses

Ledger status: ported 682, verified 415, not-needed 3; 16 not in the ledger (CRT/data rows, see the status files)

Ledger area: `cpp_data_model` 643, `utils_io_units_iges` 268, `fortran_pre_kernels` 112, `fortran_contact_cmd` 69, `kw_group_handlers_dds` 7, `text_pre_ui` 1

First phase: P1 4, P2 25, P3 11, P4 18, P6 464, P7 22, P8 452, P9 104

`@data` globals declared here: 555. `DEF_NOT_PORTED` stubs outside tests: 97 (a stub whose address is ported elsewhere is a seam to rewire: `python -I docs/re/stub_sweep.py port`).

* `1402f0ba0` at `port/defcmd/include/defcmd/geolib.hpp:179`
* `140147b70` at `port/defcmd/src/cnvt3d/ext.cpp:49`
* `1402062b0` at `port/defcmd/src/cnvt3d/ext.cpp:60`
* `1401e8040` at `port/defcmd/src/cnvt3d/ext.cpp:78`
* `1401e84f0` at `port/defcmd/src/cnvt3d/ext.cpp:79`
* `1401ea900` at `port/defcmd/src/cnvt3d/ext.cpp:80`
* `14029b630` at `port/defcmd/src/cnvt3d/ext.cpp:81`
* `140339d30` at `port/defcmd/src/cnvt3d/ext.cpp:82`
* `1401c5810` at `port/defcmd/src/cnvt3d/ext.cpp:91`
* `1402ba500` at `port/defcmd/src/cnvt3d/ext.cpp:102`
* `1402f9b60` at `port/defcmd/src/cnvt3d/ext.cpp:106`
* `140159a40` at `port/defcmd/src/cnvt3d_sweep/geometry.cpp:259`
* ... and 85 more

### Packets with status rows pointing here

`p67-1` 130, `p67-2` 129, `p8-02` 74, `p67-3` 72, `p8-07` 72, `p8-04` 61, `p8-08` 57, `p67-6` 25, `p8-01` 24, `p67-5` 22, `p8-05` 17, `p8-06` 10, `p9-04` 9, `c07` 8, `p8-03` 6, `p9-03` 5, `p2-rest-b` 3, `p2-183d00` 1, `p67-7` 1, `p9-ac8a0` 1

### Tier-1 tests (79)

| ctest | extra labels | oracle | tests/commands |
|---|---|---|---|
| `defcmd.arc_refine` | p67-6 contact tier1 |  |  |
| `defcmd.cmd_oracle_runner` | p67-1 runner | yes |  |
| `defcmd.cnvt3d_oracle` | p9-01 cnvt3d oracle | yes | CNVT3D |
| `defcmd.cnvt3d_sweep_a` | p9-03 cnvt3d | yes |  |
| `defcmd.cnvt3d_sweep_e2e` | p9-03 cnvt3d oracle | yes | CNVT3D |
| `defcmd.cnvt3d_sweepb_oracle` | p9-03 p9-03b cnvt3d | yes |  |
| `defcmd.cnvt3d_sweepb_units` | p9-03 p9-03b cnvt3d |  |  |
| `defcmd.cnvt3d_unit` | p9-01 cnvt3d |  |  |
| `defcmd.contact_cmdoracle` | p67-6 contact oracle | yes | GENCTC INICTC |
| `defcmd.contact_oracle` | p67-6 contact oracle | yes | GENCTC INICTC OBJPOS |
| `defcmd.ctc2d_meshed` | p67-6 contact tier1 |  |  |
| `defcmd.ctc3d_cube` | p67-7 ctc3d cube |  |  |
| `defcmd.ctc3d_decide` | p67-7 ctc3d |  |  |
| `defcmd.ctc3d_pipeline` | p67-7 ctc3d |  |  |
| `defcmd.ctc3d_pipeline_stages` | p67-7 ctc3d |  |  |
| `defcmd.ctc3d_search_kernels` | p67-7 ctc3d |  |  |
| `defcmd.ctc3d_search_mdsps2` | p67-7 ctc3d |  |  |
| `defcmd.ctc3d_srcnt` | p67-7 ctc3d |  |  |
| `defcmd.ctc3d_srcnt4` | p67-7 srcnt4 |  |  |
| `defcmd.ctc3d_surface` | p67-7 ctc3d |  |  |
| `defcmd.dbread_roundtrip` | dbread gendb oracle | yes |  |
| `defcmd.dispatch` | p5-01 dispatch |  |  |
| `defcmd.drop_contact` | p67-3 drop |  |  |
| `defcmd.drop_kernels` | p67-3 drop |  |  |
| `defcmd.drop_mode8` | p67-3 drop |  |  |
| `defcmd.drop_settle` | p67-3 drop |  |  |
| `defcmd.gendb3d_oracle` | p4-05 gendb3d oracle | yes |  |
| `defcmd.gendb_e2e_oracle` | p4-gate gendb oracle | yes |  |
| `defcmd.geoprim_builder` | p9-04 geoprim |  |  |
| `defcmd.ifd_cube` | p67-2 cube |  |  |
| `defcmd.ifd_d2` | p67-2 ifd2d |  |  |
| `defcmd.ifd_oracle` | p67-2 objpos oracle | yes | OBJPOS |
| `defcmd.ifd_path` | p67-2 path |  |  |
| `defcmd.int_oracle` | p67-int oracle | yes | GENCTC INICTC OBJPOS |
| `defcmd.module` |  |  |  |
| `defcmd.obj_cmds_oracle` | p67-4 oracle | yes | CPYOBJ DOESCL SVSCAL WRTOBJ |
| `defcmd.objcheck_oracle` | p4-03 objcheck oracle | yes |  |
| `defcmd.objpos2d_c11` | p67-1 objpos |  |  |
| `defcmd.objpos2d_c12` | p67-1 objpos |  |  |
| `defcmd.objpos2d_c13` | p67-1 objpos |  |  |
| `defcmd.objpos_c07` | p67-1 objpos c07 |  |  |
| `defcmd.objpos_centre` | p67-1 objpos |  |  |
| `defcmd.objpos_drop_oracle` | p67-3 drop oracle | yes | OBJPOS |
| `defcmd.objpos_mirror` | p67-1 objpos | yes |  |
| `defcmd.objpos_oracle` | p67-1 objpos oracle | yes | GENAXS OBJPOS |
| `defcmd.objpos_surface2d` | p67-1 objpos |  | CNVT3D |
| `defcmd.objpos_tensor` | p67-1 objpos oracle | yes | OBJPOS |
| `defcmd.p406_oracle` | p4-06 gendb oracle | yes |  |
| `defcmd.p4_04_gendb_oracle` | p4-04 gendb oracle | yes |  |
| `defcmd.p67_5_genaxs` | p67-5 commands |  | GENAXS OBJPOS |
| `defcmd.p67_5_genbcc` | p67-5 commands |  | GENBCC |
| `defcmd.p67_5_gensub` | p67-5 commands |  | GENSUB |
| `defcmd.p67_5_support` | p67-5 commands |  |  |
| `defcmd.p67_5_tcycle` | p67-5 commands |  | TCYCLE |
| `defcmd.p67_5_widen` | p67-5 commands |  |  |
| `defcmd.p8_01_oracle` | p8-01 oracle | yes | GFREAD |
| `defcmd.p8_01_unit` | p8-01 | yes |  |
| `defcmd.p8_02_oracle` | p8-02 | yes | GFREAD |
| `defcmd.p8_02_units` | p8-02 |  | GFREAD |
| `defcmd.p8_03_iges_oracle` | p8-03 oracle | yes | GFREAD |
| `defcmd.p8_03_iges_unit` | p8-03 |  |  |
| `defcmd.p8_04_dxf` | p8-04 |  | GFREAD |
| `defcmd.p8_04_dxf_oracle` | p8-04 | yes | GFREAD |
| `defcmd.p8_05_brdext_lookup` | p8-05 brdext |  |  |
| `defcmd.p8_05_brdext_oracle` | p8-05 brdext oracle | yes | BRDEXT |
| `defcmd.p8_06_defamg_oracle` | p8-06 | yes |  |
| `defcmd.p8_06_mesh3d` | p8-06 tier1 |  |  |
| `defcmd.p8_06_volume_replay` | p8-06 | yes |  |
| `defcmd.p8_07_defamg_oracle` | p8-07 oracle | yes |  |
| `defcmd.p8_07_launch` | p8-07 tier1 |  |  |
| `defcmd.p8_07_surface_replay` | p8-07 oracle | yes |  |
| `defcmd.p8_08_deffrz_oracle` | p8-08 oracle | yes |  |
| `defcmd.p8_08_defint_oracle` | p8-08 oracle | yes |  |
| `defcmd.p8_08_files` | p8-08 tier1 |  |  |
| `defcmd.p8_08_remesh_oracle` | p8-08 oracle | yes |  |
| `defcmd.p8_09_geolib` | p8-09 geolib |  | GFREAD |
| `defcmd.p8_09_gfread_oracle` | p8-09 geolib oracle | yes | GFREAD |
| `defcmd.short_key_oracle` | p5-shortkey oracle | yes |  |
| `defcmd.step_dat` | p9-ac8a0 cnvt3d oracle | yes |  |

Run: `ctest --test-dir port/build -L defcmd` (after `port\build.cmd`).

<!-- END GENERATED -->
