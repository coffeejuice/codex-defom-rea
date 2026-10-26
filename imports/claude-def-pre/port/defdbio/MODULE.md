# defdbio: module card

## Purpose

Moves the model between memory and `.DB` result databases: `DBREAD` (read one step into the
model), `GENDB` (check the model and write a new database or append a step, 2D and 3D writers),
the result-set reader, and the port of DEF_PRE's own Fortran DB core (`datarm23` /
`DBOperations23`: direct-access records, RLE, byte swapping) in `dbcore/`. The library code does
not call `deformdb`; tests use `deformdb` to open the databases they check.

## Interface

* `defdbio/dbread/dbread.hpp`: `db_read_step(model, db, stepno, ...)` and its helpers.
* `defdbio/gendb/gendb.hpp`: `gendb(model, path, cur_idx, check, ...)`, `write_db`, `write_step`,
  `check_db_version`, `place_step`.
* `defdbio/gendb3d/`, `objcheck/` (pre-write object checks), `resultset/`, `step/`,
  `dbcore/` (datarm23).

## Fidelity class

F1 bit-exact: a DB written by the port must compare equal to the original's with `defcmp`
(only the masks documented in `harness/defcmp/README.md`, such as timestamps). The round-trip
reference for DBREAD is DEF_PRE's own output (680 of 682 corpus cases at the P5 gate). Note the
layering rule: `defdbio` cannot call `defcmd`, so where both needed a function this module owns
it and `defcmd` forwards to it.

## Oracle tests worth knowing

`dbkey/` tier 3 cases (822), the GENDB checks and writer oracle tests, and in `defcmd`
`test_gendb_e2e_oracle` and `test_dbread_roundtrip` (both compare through `defcmp`).

## Perf baseline

Not measured yet. Record machine, build (Release, commit), input, wall time and peak memory, as
the `profile` skill (.claude/skills/profile) prints them.

## Swap notes

The read side is the natural place for `deformdb`'s parallel positional reader (evolve mode):
`deformdb` already reads steps on many threads. Writes must keep the record layout, RLE choices
(DEF_PRE stores -0.0 as +0.0 inside runs, it compares with COMISD) and step directory exactly.

<!-- BEGIN GENERATED: tools/agents/gen_module_cards.py; do not edit by hand -->

Directory `port/defdbio/`, CMake target `defdbio`.

* Depends on: `defmodel`, `defkey`, `deformdb_core`
* Used by: `defcmd`
* Files: 139 sources/headers, 28 test files, 45,454 lines
* Public headers (33): `defdbio/gendb/` 16, `defdbio/dbread/` 5, `defdbio/dbcore/` 4, `defdbio/step/` 3, `defdbio/resultset/` 2, `defdbio (top level)` 1, `defdbio/gendb3d/` 1, `defdbio/objcheck/` 1

### Binary functions ported here: 693 `@src` addresses

Ledger status: ported 489, verified 153, not-needed 34, todo 12; 5 not in the ledger (CRT/data rows, see the status files)

Ledger area: `cpp_data_model` 565, `datarm23_dbops` 45, `utils_io_units_iges` 39, `fortran_contact_cmd` 23, `text_pre_ui` 8, `kw_group_handlers_dds` 5, `fortran_fem_geom` 3

First phase: P1 77, P2 185, P3 2, P4 420, P6 4

`@data` globals declared here: 98. `DEF_NOT_PORTED` stubs outside tests: 17 (a stub whose address is ported elsewhere is a seam to rewire: `python -I docs/re/stub_sweep.py port`).

* `1401cf040` at `port/defdbio/src/dbread/w2_simctrl.cpp:178`
* `1403adc74` at `port/defdbio/src/dbread/w3_ftn.cpp:116`
* `1403da0b6` at `port/defdbio/src/dbread/w3_ftn.cpp:141`
* `1403daf3e` at `port/defdbio/src/dbread/w3_ftn.cpp:179`
* `1403db9e8` at `port/defdbio/src/dbread/w3_ftn.cpp:191`
* `1403dc13f` at `port/defdbio/src/dbread/w3_ftn.cpp:217`
* `1403dd16d` at `port/defdbio/src/dbread/w3_ftn.cpp:253`
* `1403dd950` at `port/defdbio/src/dbread/w3_ftn.cpp:290`
* `1401dfb80` at `port/defdbio/src/dbread/w3_header_cache.cpp:58`
* `1401cf040` at `port/defdbio/src/dbread/w4_fold.cpp:130`
* `1403ae0b6` at `port/defdbio/src/gendb/db_header_io.cpp:42`
* `1401cf040` at `port/defdbio/src/gendb/gate_ports.cpp:147`
* ... and 5 more

### Packets with status rows pointing here

`p4-05` 77, `p4-02b` 67, `p4-01` 61, `p1-12` 56, `p4-02a` 56, `p4-06` 52, `p4-04` 36, `p-dbread-w2` 32, `p-dbread-w4` 24, `p67-5` 19, `p4-gate` 14, `p4-gate-pre` 12, `p1-03` 4, `p-dbread-w3` 2, `p1-13` 2, `p2-rest-b` 2, `p2-183d00` 1, `p9-ac8a0` 1

### Tier-1 tests (28)

| ctest | extra labels | oracle | tests/commands |
|---|---|---|---|
| `defdbio.dbcore_db` | p1-12 dbcore corpus | yes |  |
| `defdbio.dbcore_units` | p1-12 dbcore |  |  |
| `defdbio.dbread_lead` | p-dbread dbread |  |  |
| `defdbio.dbread_w1` | p-dbread dbread |  |  |
| `defdbio.dbread_w2` | p-dbread dbread |  |  |
| `defdbio.dbread_w2_dbops` | p-dbread dbread |  |  |
| `defdbio.dbread_w2_obj` | p-dbread dbread |  |  |
| `defdbio.dbread_w3` | p-dbread dbread | yes |  |
| `defdbio.dbread_w4` | p-dbread dbread |  |  |
| `defdbio.gendb3d_bclists` | p4-05 gendb3d |  |  |
| `defdbio.gendb3d_movement` | p4-05 gendb3d |  |  |
| `defdbio.gendb3d_writers` | p4-05 gendb3d |  |  |
| `defdbio.gendb_checks_p4_04` | p4-04 gendb |  |  |
| `defdbio.gendb_counts_p4_02b` | p4-02b gendb |  |  |
| `defdbio.gendb_frame` | p4-01 gendb |  |  |
| `defdbio.gendb_obj_counts` | p4-02a gendb |  |  |
| `defdbio.gendb_pre_steps` | p4-gate gendb |  |  |
| `defdbio.module` |  |  |  |
| `defdbio.objcheck` | p4-03 objcheck |  |  |
| `defdbio.p406_frame` | p4-06 gendb |  |  |
| `defdbio.p406_interobj` | p4-06 gendb |  |  |
| `defdbio.p406_material` | p4-06 gendb |  |  |
| `defdbio.p406_obj2d` | p4-06 gendb |  |  |
| `defdbio.p406_simctrl` | p4-06 gendb |  |  |
| `defdbio.resultset_reader` | p1-13 resultset | yes |  |
| `defdbio.step_header` | p4-07 |  |  |
| `defdbio.step_header_oracle` | p4-07 oracle | yes |  |
| `defdbio.step_read` | p1-03 step |  |  |

Run: `ctest --test-dir port/build -L defdbio` (after `port\build.cmd`).

<!-- END GENERATED -->
