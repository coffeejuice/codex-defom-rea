# deformdb: module card

## Purpose

An independent C++20 reader and writer of DEFORM v11 `.DB` databases, written from the format
spec (`docs/db-format-spec.md`) before the DEF_PRE port started. It needs no DEFORM install. It
has its own API, is built for very large files (positional reads, parallel step reading) and
exports a C ABI (`deformdb.dll`) for applications such as DMO. `defmodel` and `defdbio` link it,
but in the port it is used by tests (to open and inspect databases); DEF_PRE's own DB core is
ported separately in `defdbio/dbcore`.

## Interface

See `README.md`: `Database::open` / `read_step` / `read_steps` (`database.hpp`), `StepModel`
(`model.hpp`), `Writer` (`writer.hpp`), `purge` and `merge` (`manager.hpp`), the C ABI
(`deformdb.h`), and the tools `dbinfo`, `dbraw`, `dbmanager`.

## Fidelity class

F2 format-exact: files it writes must be readable by DEFORM and equal under `defcmp`; its
internals and API are free. The DEF_PRE behaviour that must be bit-exact lives in `defdbio`.

## Oracle tests worth knowing

`codec`, `writer` (tier 1), `capi_smoke <file.DB>` by hand, and the KEY-export comparison
against DEF_PRE (`docs/re/` deformdb reader notes).

## Perf baseline

Not measured yet. Record machine, build (Release, commit), input (for example the 5 GB
Inc718 DB in `C:\PROBLEM\TEST_REA`), wall time and peak memory, as the `profile` skill prints them.

## Swap notes

This is already the "new API" module. Changes to it are evolve mode by default; keep `defcmp`
equality on written files and the C ABI stable for DMO.

<!-- BEGIN GENERATED: tools/agents/gen_module_cards.py; do not edit by hand -->

Directory `deformdb/`, CMake target `deformdb_core`.

* Depends on: nothing in the repo
* Used by: `defdbio`, `defmodel`
* Files: 18 sources/headers, 3 test files, 2,919 lines
* Public headers (7): `deformdb (top level)` 7

### Binary functions ported here: 0 `@src` addresses

`@data` globals declared here: 0. `DEF_NOT_PORTED` stubs outside tests: 0.

### Tier-1 tests (2)

| ctest | extra labels | oracle | tests/commands |
|---|---|---|---|
| `codec` |  |  |  |
| `writer` |  |  |  |

Run: `ctest --test-dir port/build -L deformdb` (after `port\build.cmd`).

<!-- END GENERATED -->
