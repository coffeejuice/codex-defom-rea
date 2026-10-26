# defrt: module card

## Purpose

Runtime support that every other port library sits on. It reproduces the behaviour of the
runtimes DEF_PRE_64 was linked against (the VS2008-era MSVC CRT, the PathScale Fortran runtime)
so that ported code prints, rounds and fails the same way: legacy `printf`/`scanf` formatting,
`libm` routines that are bit-identical to the binary's, Fortran string and formatted I/O helpers,
Fortran COMMON blocks shared by several modules, the pinned clock, and the `@src` /
`DEF_NOT_PORTED` / `omitted()` markers.

## Interface

* `defrt/port.hpp`: `DEF_NOT_PORTED(addr)`, `def::rt::omitted(...)`, layout helpers.
* `defrt/clock.hpp`: `def::rt::now()`, `local_now()`, `pin_clock()`. All time comes from here.
* `defrt/stdlib/`: `legacy_printf.hpp` / `crt_emulation.hpp` (`def::rt::crt::fprintf`, `sscanf`,
  `%Flf`), `libm.hpp` (`crt_sin`, `crt_cos`, `crt_atan2`, `sqrt`, ...), `crt_qsort.hpp`,
  `msvc9_string.hpp` (old `std::string` behaviour), `refcount.hpp`.
* `defrt/ftn/`: Fortran I/O (`fio.hpp`), units, the GEOUTL geometry kernels, and COMMON block
  structs such as `common_140b84720.hpp`.
* `defrt/rdata.hpp`: shared `.rdata` constants (one copy per address).

## Fidelity class

F1 bit-exact. Output text, rounding and libm results must match the binary bit for bit; the
p1-01 libm sweep and the Ew.0 harness (`deform-rea-work/scripts_p101`, `scripts_p803`) proved
that for the routines marked `verified`.

## Oracle tests worth knowing

`defrt.fp_fidelity` (build flags), the printf/libm tests under `tests/`, and indirectly every
KEY round trip in `defkey` (all text goes through `crt::fprintf`).

## Perf baseline

Not measured yet. Record machine, build (Release, commit), input, wall time and peak memory, as
the `profile` skill (.claude/skills/profile) prints them.

## Swap notes

Swapping is limited. `legacy_printf` and `libm` exist because the modern UCRT rounds 17-digit
output and some transcendental functions differently; replacing them with `std::format` or UCRT
`<cmath>` changes output and is evolve mode only, with the oracle diff accepted case by case.
The clock and string helpers can be swapped freely as long as `pin_clock` keeps working.

<!-- BEGIN GENERATED: tools/agents/gen_module_cards.py; do not edit by hand -->

Directory `port/defrt/`, CMake target `defrt`.

* Depends on: nothing in the repo
* Used by: `defmodel`
* Files: 51 sources/headers, 17 test files, 11,792 lines
* Public headers (27): `defrt/ftn/` 12, `defrt/stdlib/` 11, `defrt (top level)` 3, `defrt/ftnstr/` 1

### Binary functions ported here: 178 `@src` addresses

Ledger status: ported 51, todo 31, verified 12; 84 not in the ledger (CRT/data rows, see the status files)

Ledger area: `fortran_fem_geom` 38, `text_pre_ui` 23, `cpp_data_model` 20, `utils_io_units_iges` 10, `fortran_contact_cmd` 3

First phase: P1 47, P2 8, P4 7, P6 32

`@data` globals declared here: 78. `DEF_NOT_PORTED` stubs outside tests: 2 (a stub whose address is ported elsewhere is a seam to rewire: `python -I docs/re/stub_sweep.py port`).

* `1404d46e6` at `port/defrt/src/ftn/geoutl_pending_d41f8.cpp:9`
* `1404d37d7` at `port/defrt/src/ftn/geoutl_tri.cpp:313`

### Packets with status rows pointing here

`p1-01` 99, `p1-12` 16, `p8-05` 9, `p2-rest-c` 4, `kw-merge` 3, `p1-10` 3, `p8-04` 2, `kw-g2geo` 1, `p67-2` 1, `p67-3` 1, `p8-08` 1

### Tier-1 tests (17)

| ctest | extra labels | oracle | tests/commands |
|---|---|---|---|
| `defrt.clock` |  |  |  |
| `defrt.fp_fidelity` | fp |  |  |
| `defrt.ftn_geoutl_basic` | geoutl |  |  |
| `defrt.ftn_geoutl_poly` | geoutl |  |  |
| `defrt.ftn_geoutl_proj` | geoutl |  |  |
| `defrt.ftn_geoutl_tri` | geoutl |  |  |
| `defrt.ftn_helpers` | p1-12 ftn |  |  |
| `defrt.ftn_unit_system` | units kw-merge |  |  |
| `defrt.ftnstr` | p1-10 ftnstr |  |  |
| `defrt.geoutl_d41f8` | p67-3 geoutl |  |  |
| `defrt.stdlib_cli` | p1-01 cli |  |  |
| `defrt.stdlib_legacy_printf` | p1-01 stdio printf |  |  |
| `defrt.stdlib_libm` | p1-01 libm |  |  |
| `defrt.stdlib_libm_crt2` | geoutl libm |  |  |
| `defrt.stdlib_stdio_compat` | p1-01 stdio |  |  |
| `defrt.stdlib_str_helpers` | p8-05 str |  |  |
| `defrt.stdlib_string` | p1-01 string |  |  |

Run: `ctest --test-dir port/build -L defrt` (after `port\build.cmd`).

<!-- END GENERATED -->
