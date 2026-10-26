# defkey: module card

## Purpose

The KEY text codec: reading a `.KEY` file into the `defmodel` model (keyword groups 1 to 5 and
7: process definition, objects, inter-object data, materials, controls) and writing the model
back as KEY text exactly as DEF_PRE_64's `KFWRIT` does, including spacing, number formatting
and the order of records.

## Interface

* `defkey/key_reader.hpp` / `key_writer.hpp`: the KEY loop and the writer entry points.
* `defkey/kw_g1a.hpp` ... `kw_g57.hpp`, `kw_g3/`: per-group keyword parsers and writers
  (`kw_g2obj` objects, `kw_g2geo` geometry, `kw_g2mg` mesh generation, `kw_g2sv` state variables,
  `kw_g2bc` boundary conditions, `kw_g4*` materials).
* `defkey/line_io.hpp`, `table_io.hpp`, `crt_scan.hpp`: line and table I/O on the CRT
  (`%Flf` scanning through `def::rt::crt`).

## Fidelity class

F1 bit-exact: a KEY written by the port must equal the original's byte for byte, including CRLF
and trailing spaces. Tests must call `keytest::legacy_printf()` and reset unit globals per case
(`reset_unit_globals`, `g_conv.english = 0`).

## Oracle tests worth knowing

The `*_oracle` and `*_corpus` round trips (`defkey.kw_g4a_corpus` reads 942 corpus KEY files,
`kw_g2sv_oracle` must report 0 mismatches), plus tier 2/3 `kfrw/` corpus cases.

## Perf baseline

Not measured yet. Record machine, build (Release, commit), input, wall time and peak memory, as
the `profile` skill (.claude/skills/profile) prints them.

## Swap notes

A faster or friendlier parser (evolve mode) must keep the writer's output byte-identical, since
users and DEFORM itself read these files. The reader may be replaced if it builds the same model;
the KEY round-trip and corpus tests are the regression baseline.

<!-- BEGIN GENERATED: tools/agents/gen_module_cards.py; do not edit by hand -->

Directory `port/defkey/`, CMake target `defkey`.

* Depends on: `defmodel`
* Used by: `defcmd`, `defdbio`
* Files: 89 sources/headers, 41 test files, 26,761 lines
* Public headers (24): `defkey (top level)` 23, `defkey/kw_g3/` 1

### Binary functions ported here: 379 `@src` addresses

Ledger status: verified 212, ported 167

Ledger area: `utils_io_units_iges` 189, `cpp_data_model` 173, `kw_group_handlers_dds` 13, `fortran_contact_cmd` 4

First phase: P1 1, P2 251, P3 77, P9 1, P10 49

`@data` globals declared here: 2. `DEF_NOT_PORTED` stubs outside tests: 0.

### Packets with status rows pointing here

`kw-g2sv` 58, `kw-g4b` 46, `kw-g2obj` 40, `kw-g2geo` 29, `kw-g3` 28, `kw-g1b` 21, `kw-g2bc` 19, `kw-g4a` 19, `kw-g57` 18, `kw-g2mg` 11, `p2-rest-b` 6, `key-common` 1, `kw-g1a` 1

### Tier-1 tests (33)

| ctest | extra labels | oracle | tests/commands |
|---|---|---|---|
| `defkey.key_reader` | key-common |  |  |
| `defkey.kw_g1a_corpus` | kw-g1a keywords | yes |  |
| `defkey.kw_g1a_golden` | kw-g1a keywords |  |  |
| `defkey.kw_g1a_unit` | kw-g1a keywords |  |  |
| `defkey.kw_g1b` | kw-g1b keys | yes |  |
| `defkey.kw_g1b_corpus` | kw-g1b corpus | yes |  |
| `defkey.kw_g2bc_ec` | kw-g2bc ec | yes |  |
| `defkey.kw_g2bc_local` | kw-g2bc local | yes |  |
| `defkey.kw_g2bc_nodal` | kw-g2bc nodal | yes |  |
| `defkey.kw_g2bc_win` | kw-g2bc win | yes |  |
| `defkey.kw_g2geo_curve` | kw-g2geo group2 curve |  |  |
| `defkey.kw_g2geo_llgry_oracle` | kw-g2geo group2 oracle | yes |  |
| `defkey.kw_g2geo_mesh` | kw-g2geo group2 mesh | yes |  |
| `defkey.kw_g2geo_oracle` | kw-g2geo group2 oracle | yes |  |
| `defkey.kw_g2geo_surface` | kw-g2geo group2 surface | yes |  |
| `defkey.kw_g2mg` | kw-g2mg group2 |  |  |
| `defkey.kw_g2mg_oracle` | kw-g2mg group2 oracle | yes |  |
| `defkey.kw_g2obj` | kw-g2obj group2 |  |  |
| `defkey.kw_g2obj_oracle` | kw-g2obj group2 oracle | yes |  |
| `defkey.kw_g2sv_oracle` | kw-g2sv group2 oracle | yes |  |
| `defkey.kw_g3` | kw-g3 |  |  |
| `defkey.kw_g3_corpus` | kw-g3 corpus | yes |  |
| `defkey.kw_g3_window3d` | kw-g3 |  |  |
| `defkey.kw_g4` | kw-g4b |  |  |
| `defkey.kw_g4_corpus` | kw-g4b | yes |  |
| `defkey.kw_g4a` | kw-g4a keywords |  |  |
| `defkey.kw_g4a_corpus` | kw-g4a keywords corpus | yes |  |
| `defkey.kw_g57` | kw-g57 |  |  |
| `defkey.kw_g57_corpus` | kw-g57 corpus | yes |  |
| `defkey.kw_unit_system_corpus` | units corpus kw-merge | yes |  |
| `defkey.line_io` | key-common |  |  |
| `defkey.module` |  |  |  |
| `defkey.table_io` | key-common |  |  |

Run: `ctest --test-dir port/build -L defkey` (after `port\build.cmd`).

<!-- END GENERATED -->
