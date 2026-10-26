# Port conventions (DEF_PRE_64 batch port)

Rules every porting thread follows, so that many packets can work in parallel without stepping on
each other and every line of C++ can be traced back to the binary. The plan is
`docs/port-plan-def-pre-batch.md` (fidelity rules in its section 4); the per-function ledger is
`docs/re/pre64_batch_ledger.tsv`; packet lists are `docs/phase1-packets.md`,
`docs/phase4-packets.md` and `docs/phase5_8_9_10-packets.md`. The entry point for agents is
`AGENTS.md` at the repo root (modes, commands, paths); these rules are the faithful-mode code rules
it refers to, and they apply to the later recoveries (solver harness, DEF_GUI_MO) as well, with
their own addenda in their plans.

## 1. Tree and layering

```
deformdb/                 .DB container (existing, plan "Already done")
port/
  CMakeLists.txt          superbuild: libraries, def_pre, tier 1/2/3 tests
  cmake/DefPort.cmake     def_port_module(), FP fidelity flags
  testkit/                header-only tier-1 test macros
  defrt/                  runtime support: @src markers, pinned clock, CRT/Fortran-runtime behaviour
  defmodel/               keyword model: classes, registry, units, model globals
  defkey/                 KEY text reader and writer
  defdbio/                model <-> .DB arrays (DBREAD, GENDB), datarm23/DBOperations23 on deformdb
  defcmd/                 command keywords (FUN_140378e00 cases), Fortran kernels, imports
  def_pre/                the executable: main, -m CLI, INI/DEF_MULTI.INI, menus, stdin protocol
```

Dependencies only point down: `deformdb -> defrt -> defmodel -> defkey, defdbio -> defcmd -> def_pre`
(`defdbio` and `defmodel` also use `deformdb`). A function that seems to need an upward call
takes a callback or moves down; ask the coordinator before adding a dependency edge.

Where a ledger area goes:

| Ledger `area` | Module |
|---|---|
| `text_pre_ui` | `def_pre/` |
| `cpp_data_model` | `defmodel` (classes, registry, lists, StateVar), `defkey` (KEY formatting), `defdbio` (`ReadFromDB`/`WriteToDBArrays` methods may live in `defdbio` as free functions over model classes) |
| `datarm23_dbops` | `defdbio` (over `deformdb`) |
| `utils_io_units_iges` | `defrt` (string, file and Fortran-runtime helpers), `defmodel` (units), `defcmd` (DXF/IGES) |
| `fortran_contact_cmd`, `fortran_pre_kernels`, `fortran_fem_geom` | `defcmd` |
| `static_init` | the module that owns the global the initialiser builds |

Each module directory:

```
port/<module>/CMakeLists.txt      one line: def_port_module(<module> DEPS ...); never needs editing
port/<module>/include/<module>/   public headers, #include <defmodel/llobj_data.hpp>
port/<module>/src/                sources, any subfolders (globbed)
port/<module>/tests/test_*.cpp    tier-1 tests, one executable per file (globbed)
port/<module>/tests/data/         small golden files (a few KB each; large inputs come from the corpus)
```

Sources and tests are globbed (`CONFIGURE_DEPENDS`), so adding a file never means editing a shared
CMakeLists.txt. Build: `port\build.cmd` (MSVC + Ninja, then tier 1), or by hand
`cmake -S port -B port/build -G Ninja && cmake --build port/build`.

## 2. Naming

* Namespaces: `def::rt`, `def::model`, `def::key`, `def::dbio`, `def::cmd`; `def_pre/` code in `def::pre`.
* Classes with RTTI keep the RTTI name with leading underscores removed, unchanged otherwise:
  `_DB_STEP_DATA` -> `DB_STEP_DATA`, `LLOBJ_DATA`, `OpStopLinkedList`, `StateVarElemTEPS`. This keeps
  the port greppable against the decompile. Classes without RTTI are `Cls_<ctor address>` (e.g.
  `Cls_1401dbfe0`) until their role is clear; renaming then is one commit by the owning packet.
* Functions, methods, variables: `snake_case` with a descriptive name (`read_keyword_file`,
  `step_count`). If the role is not yet clear, `fun_<address>` (e.g. `fun_1401c8c00`); the `@src`
  tag carries the address either way.
* Fortran routines whose name is known (ident strings, error texts, symbol names such as `MDSP2S`,
  `PNTEDG`) keep it, lowercase: `def::cmd::ftn::mdsp2s`.
* Data members recovered from offsets: `u<hex offset>` (`u1b60`) until named; named members are
  `snake_case`. Members must stay at the recovered offset (section 4).
* Globals: `g_<name>` or `dat_<address>` (see section 5).
* Addresses are written as 9 lowercase hex digits without `0x` in names and tags (`1401c8950`) and
  as `0x1401c8950` in the ledger and status files.
* Files: `snake_case` of the class or topic, one class per header/source pair
  (`defmodel/include/defmodel/llobj_data.hpp`, `defmodel/src/llobj_data.cpp`).

## 3. Citing the source address

Every ported function carries one tag line directly above its definition:

```cpp
// @src 1401c8950  DB_STEP_DATA ctor (FUN_1401c8950)
DB_STEP_DATA::DB_STEP_DATA(int dim) { ... }
```

* Format: `// @src <address>` then free text. A script reads `//\s*@src\s+(14[0-9a-f]{7})`.
* A C++ function that absorbs several binary functions (an inlined helper, a thunk, a
  compiler-split part) carries one tag per address:
  `// @src 1401c8c00 (init part of the ctor, inlined here)`.
* One binary function split over several C++ functions: the tag goes on the entry; helpers get
  `// part of @src <address>` (not matched by the script).
* Inside a body, cite instruction-level evidence when it is not obvious: `// 1401c8cd5: sets 0x1b5d = 1`.
* Literal strings are copied from the binary byte for byte, including trailing spaces and the
  exact format specifiers; cite the string's address when it is not next to the function:
  `// str 14055a2c8`.
* Code the original runs but the port omits on purpose (licence checks, Sentinel, process
  monitor sockets) gets `// @src <address>  omitted: licence` and no body; if it must exist,
  `def::rt::omitted(0x..., "licence", __FILE__, __LINE__)`. Never analyse what such code does.
* Virtual functions and constructors that exist only so that a vtable links use
  `DEF_NOT_PORTED(0x1401...)` as the body until their packet ports them.

`python -I docs/re/fold_status.py --check` lists ported functions that have no tag.

## 4. Classes and layout

Layout headers (`defmodel/include/defmodel/layout/*.hpp`, recovered in Phase 1, see
`docs/re/model-layout.md`) are the real classes, not a separate mirror:

* Data members sit at the binary's offsets: same types (`int32_t` for Fortran `INTEGER` and C
  `int`, `double`, `char[n]`, pointers), same order, explicit `std::byte pad_<off>[n]` for unknown
  gaps. `static_assert(sizeof(X) == ...)` and `DEF_LAYOUT_OFFSET(X, member, 0x...)` follow the
  class. This lets a porter read `*(int*)(this + 0x1b60)` in the decompile and find the member.
* Single inheritance mirrors MSVC: vptr at 0, base members first. Virtual functions are declared
  in vtable slot order, so slot k of the port is slot k of the binary.
* Whoever owns the class (the packet that lists it in "files owned") adds methods in its header
  and source; other packets call public methods, and ask the owner (through the coordinator) for
  a member they need. Adding a member never moves an existing one.
* STL members: the original uses VS2010-era `std::` containers. Where layout matters (the
  member's offset is used by ported code) keep a same-size placeholder and reproduce the
  container's behaviour; where it does not, use the modern container and adjust the padding
  so that later offsets stay unchanged.

## 5. Globals and Fortran COMMON blocks

* A global or COMMON block is declared once, in the module that owns the code initialising it, as
  an `inline` variable in a header, tagged with its address:
  `// @data 140707468  DB version word (DAT_140707468)` then `inline std::int32_t g_db_version = 0;`.
  A COMMON block is one struct with members at the block's offsets, tagged with the block start.
* A packet that needs a global no-one has declared yet adds it to its own header
  `include/<module>/globals_<packet>.hpp` (e.g. `globals_p1_04.hpp`). When two packets declared
  the same address, the later merge moves it to the owner's header; `fold_status.py --check`
  reports duplicate `@data` addresses.
* No hidden function-local statics for state the original keeps globally; tests must be able
  to reset state (`reset_globals()` per module, added as globals appear).

## 6. Fidelity rules (plan section 4, expanded)

* Floating point: keep the original operation order and the rounding points; one C++ expression
  per decompiled expression where in doubt. Build flags are fixed in `cmake/DefPort.cmake`
  (`/fp:precise`, no FMA contraction, SSE2 only); do not add `/fp:fast`, `/arch:AVX*` or
  `-ffast-math`. Where `-O2` Fortran reassociated, reproduce the compiled order (read the
  assembly, not only the decompile). `defrt.fp_fidelity` checks the flags at run time.
* Integers: `int32_t` where the binary uses 32-bit; reproduce wraparound and truncation exactly
  (`(int32_t)` casts, unsigned arithmetic where the binary uses it).
* Output text goes through the C stdio of the MSVC CRT with the original format strings
  (`std::fprintf(f, "%-6s %20.10E\n", ...)`), not iostreams or `std::format`, unless the
  original used iostreams there (`SfOStream`, `std::ofstream` paths). Fortran formatted I/O
  goes through `defrt` helpers that reproduce the Fortran runtime's output.
* Time comes only from `def::rt::now()` / `local_now()` (`defrt/clock.hpp`).
* Recover from code only. Corpus files are test data, used on copies; never derive behaviour from
  expected output. A mismatch is traced back to the code, never patched to fit.
* No DEFORM binary is modified, no debugger is attached to DEFORM, licence/Sentinel code is never
  analysed or ported.

## 7. Tests

* Tier 1 (seconds, per module and packet): `port/<module>/tests/test_<topic>.cpp`, built with
  `testkit` (`TEST_CASE`, `CHECK`, `CHECK_EQ`, `CHECK_BITS` for bit-identical doubles,
  `int main() { return testkit::run_all(); }`).
  * ctest name `<module>.<topic>`, labels `<module>;tier1` automatically.
  * Add the packet label (and any topic labels) on a `// ctest-labels:` line anywhere in the file
    (only the first 20 such lines are read):
    `// ctest-labels: p1-04 registry`.
  * Run what you touched: `ctest --test-dir port/build -L p1-04`, or `-L defmodel`; all tier 1:
    `-L tier1`.
  * Expected values come from the oracle (cached DEF_PRE output, `tools/oracle`), from tables in
    the binary, or from hand calculation shown in the test. Never from running the port.
  * Golden files under `port/<module>/tests/data/` stay small; read them via `testkit::repo_root()`.
* Tier 2 (`cmake --build port/build -t tier2`, under a minute) before every push that changes
  behaviour visible to `def_pre`.
* Tier 3 (`ctest --test-dir port/build -L tier3 -j 12`, about an hour, 1,907 corpus cases) and
  the end-to-end command check (`python -I tools/int/e2e_stdout.py --all -j 8`, every
  `tests/commands` case through `def_pre.exe`, whole stdout and every file byte for byte) run at
  gates and before a merge to `main`; `.claude/skills/gate` has the full list.
* Command-keyword cases are JSON files under `tests/commands/<CMD>/` (`cases*.json` lists or
  single-case `<stem>.json`), recorded with the oracle runner and replayed in tier 1 through
  `port/defcmd/tests/cmd_oracle_runner.hpp`; `.claude/skills/record-oracle` shows how to add one.
  Inputs are referenced by path relative to the case file or copied under the case folder, never
  by a path inside a worktree that will be removed.
* A packet's done-check (in its packet list) names its tier-1 tests and what the oracle
  verifies.

## 8. Ledger status without merge conflicts

* Packets never edit `docs/re/pre64_batch_ledger.tsv`.
* Each packet owns `docs/re/status/<packet>.tsv` (e.g. `p1-04.tsv`), tab-separated with a header:

  ```
  address	status	note
  0x1401c8950	ported	port/defmodel/src/db_step_data.cpp
  0x140484f90	not-needed	crt: strcpy
  0x14017a6c0	not-needed	stl: std::vector<int>::vector, replaced by std::vector
  ```

  `status`: `todo`, `ported` (C++ exists, tier 1 passes), `verified` (the oracle confirms output
  it influences), `not-needed` (CRT/STL/inlined/licence/unreachable; the note says which).
* `python -I docs/re/fold_status.py` folds all status files into the ledger's `status` column and
  prints per-phase counts; `--check` also cross-checks `@src` tags; `--dry-run` writes nothing. Only
  the step that merges packets into `main` runs it and commits the ledger, so packet branches
  never conflict on it.
* An address the ledger lacks (a function the port found necessary) goes into the status file
  anyway with a note; the fold reports it so the ledger can be extended.
* Lines starting with `#` are comments. Ownership comment rows (section 10) use the form
  `# 0x140198230<TAB>-<TAB>owner row in p-dbread-w1.tsv`.
* Run results, reports and integration tables go in `docs/re/int/`, never in `docs/re/status/`:
  the fold reads every `status/*.tsv` and would take a results table for status rows.
* Before a merge to `main`, `fold_status.py --check` must report 0 errors.

## 9. Packet workflow

1. Branch from `main` into your own worktree; take your own Ghidra project copy
   (`deform-rea-work/ghidra_<packet>`, copied from the master `deform-rea-work/ghidra`) for
   headless runs, and your own build folder.
2. Decompile the packet's functions into `deform-rea-work/out/<packet>/` with
   `GPROJ=ghidra_<packet> scripts/run.sh DEF_PRE_64.exe Decomp.java ...` and read one function
   with `scripts/fn.sh <file> <addr>`; use REA for interactive per-function recovery and call
   chains, headless Ghidra for bulk sweeps (`.claude/skills/port-function`).
3. Touch only the files your packet owns (its packet list); a needed change elsewhere goes to the
   owning packet through the coordinator, or is done as a minimal additive commit (new member at
   the end of padding, new declaration) and announced.
4. Calls into code another packet has not landed yet go through an `ext` seam (`ext::fun_<addr>`
   with a `DEF_NOT_PORTED` default and a test hook); section 10 says how they are removed.
5. Commit locally often (the network drops); push when tier 1 for your label passes.
6. Keep `docs/re/status/<packet>.tsv` current; finish when every function of the packet is
   `ported`, `verified` or `not-needed`, and refresh the module cards
   (`python -I tools/agents/gen_module_cards.py`).

## 10. Ownership, forwards and seams

* One owner per binary function. The owner's status file keeps the row; every other packet that
  also ported it turns its row into a comment row naming the owner (section 8), and its C++
  forwards to the owner's function (a one-line call or `using`), so the body exists once and the
  `@src` tag is counted once. "Copies forward" in commit messages means this.
* Dependencies only point down (section 1), so when two layers need a function the lower layer
  owns it: DBREAD owns `140198230`, `14027fe20`, `140280290`, `1402571b0`; GENDB owns
  `1402399b0`, `140239a30`; `defmodel/obj_mesh2d/rc_array.hpp` (C09) owns `1403e5cb0`,
  `1403e6320`, `1403e6260`.
* Interim copies of another packet's function are compiled only while the owner's header is
  absent (`__has_include`), and the merge that brings the owner deletes them. Ninja does not see a
  `__has_include` change, so force a recompile after a header appears or disappears; otherwise a
  stale switch silently keeps the `DEF_NOT_PORTED` fallback. `python -I docs/re/stub_sweep.py port`
  lists stubs whose address already has an `@src` port elsewhere.
* One `// @data` declaration per address; shared `.rdata` constants live in `defrt/rdata.hpp`.

## 11. Evolve-mode changes

Optimisations, module swaps, new APIs and fixes of DEFORM's own bugs are evolve mode
(`AGENTS.md` section 2). They keep the faithful build as the baseline:

* The faithful path stays buildable: the change sits behind a CMake option or in its own target
  or library, named in the module's `MODULE.md`.
* The same tier 1/2/3 and e2e tests run on the evolved build; every difference from the oracle
  is listed with its cause in `docs/evolve/<topic>.md` and accepted case by case.
* Before/after numbers come from `.claude/skills/profile` and go in the module card.
* Faithful-mode rules on tags and ledger still apply to faithful code that the change touches;
  new code that has no binary counterpart carries no `@src` tag.
