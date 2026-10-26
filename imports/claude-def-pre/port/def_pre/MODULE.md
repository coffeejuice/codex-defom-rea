# def_pre: module card

## Purpose

The `def_pre` executable and its library `def_pre_core`: `main`, the `-m` CLI
(`header`, `stepList`, `lastStepNo`, `simStepList`), `DEF_MULTI.INI` handling, the INI driver,
and the text menus of DEF_PRE_64 batch mode (main menu, menus 3 to 7, forms) that read the stdin
protocol (`\n2\n1\nCMD.KEY\n\nE\nE\nY\n` runs a keyword file).

## Interface

* `def_pre/menu_shell.hpp`: `main_session(argc, argv)`, `ini_driver`, `menu_loop`, menu helpers.
* `def_pre/m_cli.hpp`: the `-m` modes. `def_pre/menu56.hpp`, `menu7.hpp`, `forms.hpp`: menus.
* `def_pre --port-version` identifies the port (tier-1 `def_pre_starts`).

## Fidelity class

F1 bit-exact: stdout, created files and exit codes must match the original for the same stdin
and argv. Known edge: a blank `-d` in header mode makes the original print a Fortran runtime
"Bad file specification" to stderr and then crash (0xC0000005).

## Oracle tests worth knowing

`def_pre_core.p10_01_menus_oracle`, `menu56_replay`, the `m/` corpus cases (129-case `-m` harness
in `harness/mcli`), `harness/shell/shell_cases.py`, and `tools/int/e2e_stdout.py --all`.

## Perf baseline

Not measured yet. Record machine, build (Release, commit), input, wall time and peak memory, as
the `profile` skill (.claude/skills/profile) prints them.

## Swap notes

A new API (library calls instead of the stdin protocol) belongs beside this module, calling
`defcmd`/`defdbio` directly; the menus stay as they are so the oracle keeps working.

<!-- BEGIN GENERATED: tools/agents/gen_module_cards.py; do not edit by hand -->

Directory `port/def_pre/`, CMake target `def_pre_core`.

* Depends on: `defcmd`
* Used by: no other port library
* Files: 18 sources/headers, 7 test files, 6,202 lines
* Public headers (7): `def_pre (top level)` 7

### Binary functions ported here: 107 `@src` addresses

Ledger status: verified 89, ported 12, not-needed 1; 5 not in the ledger (CRT/data rows, see the status files)

Ledger area: `text_pre_ui` 88, `cpp_data_model` 14

First phase: P1 6, P4 7, P5 10, P9 6, P10 73

`@data` globals declared here: 2. `DEF_NOT_PORTED` stubs outside tests: 10 (a stub whose address is ported elsewhere is a seam to rewire: `python -I docs/re/stub_sweep.py port`).

* `140007d30` at `port/def_pre/include/def_pre/ext_p10_01.hpp:38`
* `1401a6c80` at `port/def_pre/include/def_pre/ext_p10_02.hpp:76`
* `14013ee00` at `port/def_pre/include/def_pre/ext_p10_02.hpp:99`
* `140017540` at `port/def_pre/src/menu_shell.cpp:144`
* `1400157c0` at `port/def_pre/src/menu_shell.cpp:153`
* `14000a370` at `port/def_pre/src/menu_shell.cpp:162`
* `1402240d0` at `port/def_pre/src/menu_shell.cpp:172`
* `14001a050` at `port/def_pre/src/menu_shell.cpp:350`
* `140019c20` at `port/def_pre/src/menu_shell.cpp:355`
* `140009f20` at `port/def_pre/src/menu_shell.cpp:593`

### Packets with status rows pointing here

`p10-01` 30, `p4-01` 15, `m-cli` 1, `p2-rest-c` 1

### Tier-1 tests (7)

| ctest | extra labels | oracle | tests/commands |
|---|---|---|---|
| `def_pre_core.m_args` | m-cli def_pre |  |  |
| `def_pre_core.menu56_replay` | p10-02 def_pre oracle | yes |  |
| `def_pre_core.menu56_units` | p10-02 def_pre |  |  |
| `def_pre_core.menu7` | p4-01 def_pre |  |  |
| `def_pre_core.menu_shell` | p5-01 shell |  |  |
| `def_pre_core.p10_01_forms` | p10-01 def_pre |  |  |
| `def_pre_core.p10_01_menus_oracle` | p10-01 def_pre oracle | yes | MENU3 MENU4 |

Run: `ctest --test-dir port/build -L def_pre_core` (after `port\build.cmd`).

<!-- END GENERATED -->
