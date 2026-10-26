# Phase 1 work packets

Phase 1 of `docs/port-plan-def-pre-batch.md` (the `-m` CLI and the keyword-model skeleton) has
1 439 ledger functions (`first_phase = P1`, 256 KB). This file splits them into 14 packets that
separate threads can port in parallel. Rules for every packet are in `docs/port-conventions.md`.

Machine-readable:

* `docs/phase1-packets.tsv`: one row per packet (id, title, modules, owned paths, counts,
  dependencies, done-check).
* `docs/re/phase1_packet_map.tsv`: one row per Phase 1 function (address, size, area, via, packet,
  vtable slots it fills). Regenerate with `python -I docs/re/phase1_packets.py`.

## How the split was made

MSVC and the PathScale Fortran compiler place the functions of one object file next to each other,
so an address range of Phase 1 functions is roughly one source file. Packets are unions of such
ranges, with two corrections: every virtual method goes to the packet that owns its class
(wherever the linker put it), and helpers that only the DB core uses moved to the DB-core packet.
Sizes are counted as all functions and "substantial" ones (64 bytes or more); many of the small
ones are STL instantiations, trivial accessors and scalar-deleting destructors that become
`not-needed` or one line of C++.

Not in any packet:

| Group | Functions | Why |
|---|---:|---|
| `m-cli` | 6 | `FUN_14001b7b0`, `FUN_14001c500`, `FUN_14001bc80`, `FUN_14001be50`, `FUN_14001bf40`, `FUN_14001c100`: the `-m` thread |
| `licence` | 37 | four functions whose strings show licence checks or the SDSPM process monitor (`1403c725d`, `1403c768e`, `14044cf00`, `14044cfc0`) and the 33 functions that, within Phase 1, only they call. Never analysed or ported; a packet that finds one of them on a non-licence path takes it back and says so |

The keyword registry (`FUN_1402f76c0` and accessors) is not a Phase 1 ledger function; it is done
(`port/defmodel`, `docs/re/status/keyword-registry.md`).

## Packets

| Packet | Title | Module | Functions | Substantial | Bytes |
|---|---|---|---:|---:|---:|
| p1-01 | C++ runtime: STL, locale, iostream/filebuf, std::string | defrt | 152 | 74 | 15 845 |
| p1-02 | Static option-label tables (54 initialisers) | defmodel | 54 | 38 | 7 276 |
| p1-03 | `_DB_STEP_DATA` / `_DB_WRITE_VAR`: the model root | defmodel, defdbio | 76 | 46 | 21 459 |
| p1-04 | `LLOBJ_DATA` and the model's object containers | defmodel | 98 | 54 | 18 615 |
| p1-05 | `SdctlGList`, `SdctlPtrList<_OBJ_GEO2D>`, `SdctlPtrCollection` | defmodel | 96 | 34 | 7 291 |
| p1-06 | `DYNARY_DATA`, `OPSTOP_DATA`, `SfLinkedList` family | defmodel | 79 | 29 | 6 692 |
| p1-07 | Geometry objects: `OBJ_GEO_BASE`, `_OBJ_GEO2D`, `_OBJ_GEO3D`, `C2d3dGeometry`, `_DEF_FACE` | defmodel | 113 | 54 | 19 485 |
| p1-08 | 2D mesh: `GEO2D_AND_MESH2D`, `C3dCrossSection`, `OBJ_MESH2D`, `_DEF_GEOPRIM` | defmodel | 172 | 82 | 14 811 |
| p1-09 | State variables: `StateVarBase` and its 9 subclasses | defmodel | 143 | 71 | 17 406 |
| p1-10 | Model members from the Fortran-area objects; string helpers | defmodel, defrt | 81 | 36 | 18 422 |
| p1-11 | Topology: `_DEF_LOOP`, `_DEF_EDGE`, `_DEF_VERTEX`, `OBJ_GEO2D_DIGITIZED` | defmodel | 89 | 35 | 10 222 |
| p1-12 | DB core: `datarm23.f` / `DBOperations23.f`, low-level record I/O | defdbio | 78 | 72 | 60 417 |
| p1-13 | DB result-set reader (`OPENING DATABASE FILE`) | defdbio | 67 | 42 | 12 680 |
| p1-14 | Tree-container class `Cls_14042a870` and its `std::map`/`set` instantiations | defmodel | 98 | 36 | 11 926 |

Function lists: `docs/re/phase1_packet_map.tsv`, column `packet`.

### p1-01 C++ runtime

Ranges `0x140001000-0x14001c770` (without the `-m` functions), `0x1401b1ff0-0x1401b5140`,
`0x14050402c-0x1405041ec`. RTTI: `std::basic_filebuf`, `std::basic_streambuf`, `std::ctype<char>`,
`std::ios_base::failure`, `std::bad_alloc`, `std::locale::facet`, `std::runtime_error`.
Hubs: `140005c80` (string assign, 38 callers), `140001010`/`140001000` (reference counts).
Most functions are VS2010 STL/CRT instantiations: mark `not-needed` with `stl:`/`crt:` and the
standard facility that replaces them. Port only behaviour the program's output depends on (for
example the filebuf write path used for KEY/text output, and `std::string` growth if it changes
observable text) into `defrt`.
Owns: `port/defrt/include/defrt/stdlib/`, `port/defrt/src/stdlib/`, `port/defrt/tests/test_stdlib_*.cpp`.
Depends on: nothing. Done-check: `ctest -L p1-01`; every function `ported` or `not-needed` with a
reason.

### p1-02 Static option-label tables

Range `0x140501bd0-0x140503b80`: 54 C++ static initialisers called from `_initterm` that fill
global `std::string` tables ("Isothermal", "Heat transfer", "English", "System", "Newton-Raphson",
"Direct", ...) at `DAT_1405be8c0` onward. Port them as `inline const` tables in `defmodel`, each
with its `@data` address; strings copied byte for byte.
Owns: `port/defmodel/include/defmodel/options/`, `port/defmodel/src/options/`,
`port/defmodel/tests/test_options*.cpp`.
Depends on: p1-01 (only `std::string`; use the standard one). Done-check: `ctest -L p1-02`
compares every table entry with the string read from `DEF_PRE_64.exe` (read-only, like
`gen_keyword_registry.py --check`, skipped when DEFORM is absent).

### p1-03 `_DB_STEP_DATA` and `_DB_WRITE_VAR`

Range `0x1401b8000-0x1401ee5a0`. The model root object: ctor `FUN_1401c8950` (base ctor
`FUN_1401bae10`, init `FUN_1401c8c00`, dimension setup `FUN_1401d09c0`), dtor `FUN_1401cb2c0`,
scalar-deleting dtor `FUN_1401c8ba0` (vtable slot 0; reaches 741 Phase 1 functions, so this packet
calls into almost every other one), `Cls_1401dbfe0` (`FUN_1401dbfe0`/`FUN_1401dc030`), the step
read `FUN_1401dc3f0` used by the `-m` modes, and the operation/simulation name code
(`FUN_1401e7b10`.., strings "SIMULATION", "OPERATION 1").
DB-reading methods (`FUN_1401dc3f0` and what only it calls) are defined in `defdbio`
(conventions section 1), the rest in `defmodel`.
Owns: layout headers `db_step_data.hpp`, `db_write_var.hpp`, `cls_1401dbfe0.hpp` (and their
`src/layout/*.cpp` stubs), `port/defmodel/{include/defmodel,src}/step/`,
`port/defdbio/{include/defdbio,src}/step/`, tests `test_step*.cpp` in both modules.
Depends on: p1-10, p1-08, p1-05, p1-04, p1-06, p1-01, p1-13, p1-12 (members and sub-objects;
stub with `DEF_NOT_PORTED` until they land). Done-check: `ctest -L p1-03` (layout asserts,
constructed field values, construct/destroy for dim 2 and 3); oracle: the `-m header` and
`-m stepList` cases go through `FUN_1401dc3f0` (shared with the `-m` thread).

### p1-04 `LLOBJ_DATA` and object containers

Range `0x140130530-0x140197fff`. RTTI `LLOBJ_DATA` (vtable `0x140522f00`), `IObjectData`; the
container classes that the model root holds (ctors `14017a6c0`, `140182840`, `1401854f0` ...)
and their list/vector instantiations.
Owns: layout headers for `LLOBJ_DATA`, `IObjectData` and the container classes this range
constructs; `port/defmodel/{include/defmodel,src}/llobj/`; tests `test_llobj*.cpp`.
Depends on: p1-05, p1-01, p1-10, p1-06, p1-09. Done-check: `ctest -L p1-04` (layout, insert,
find, iterate, destroy against the decompiled semantics).

### p1-05 Sdctl lists

Range `0x1401999c0-0x1401b0fff` plus the `SdctlGList`/`SdctlPtrCollection` virtual methods in
`0x140330ea0`-`0x140333f50`. RTTI `SdctlGList`, `SdctlPtrList<_OBJ_GEO2D>`, `SdctlPtrCollection`
(the shared template slots also serve `SdctlPtrList<CsdspmSimulationProcessMonitorDataSource::CRecord>`,
whose own use is process-monitor code and skipped).
Owns: layout headers `sdctl_*.hpp`; `port/defmodel/{include/defmodel,src}/sdctl/`; tests `test_sdctl*.cpp`.
Depends on: p1-01, p1-10, p1-04, p1-09. Done-check: `ctest -L p1-05`.

### p1-06 Linked-list data

Range `0x1401f4390-0x14026ffff` without the DB helpers moved to p1-12. RTTI `DYNARY_DATA`,
`DynAryLinkedList`, `OPSTOP_DATA`, `OpStopLinkedList`, `SfLinkedData`, `SfLinkedList`; helpers at
`0x140220540`-`0x140222620` used by the geometry classes.
Owns: layout headers for these classes; `port/defmodel/{include/defmodel,src}/linked/`; tests `test_linked*.cpp`.
Depends on: p1-04, p1-05, p1-12, p1-10, p1-01. Done-check: `ctest -L p1-06`.

### p1-07 Geometry objects

Ranges `0x140275b00-0x140280760` and `0x1402b7400-0x1402fe990`, plus the class methods placed
elsewhere (`1401b0650`, `1402567c0`). RTTI `OBJ_GEO_BASE`, `_OBJ_GEO2D`, `C2d3dGeometry`,
`_OBJ_GEO3D`, `_DEF_FACE`/`IFace`. Strings "OBJ_GEO_BASE", "_OBJ_GEO2D" (class-name texts).
Owns: layout headers for these classes; `port/defmodel/{include/defmodel,src}/geo/`; tests `test_geo*.cpp`.
Depends on: p1-08 (77 calls: the 2D mesh members), p1-01, p1-11, p1-06, p1-04, p1-09, p1-05.
Done-check: `ctest -L p1-07`. Coordinate with p1-08 early: these two share the most calls.

### p1-08 2D mesh and cross-section

Range `0x1402838d0-0x14029e210`, plus `OBJ_MESH2D`/`_DEF_GEOPRIM` methods (`1402f0610`,
`1403e2240`). RTTI `GEO2D_AND_MESH2D`, `C3dCrossSection`, `OBJ_MESH2D`, `_DEF_GEOPRIM`; also the
`TEXTURE_DATA` code (string "Error in TEXTURE_DATA::countNumODFs()").
Owns: layout headers for these classes; `port/defmodel/{include/defmodel,src}/mesh2d/`; tests `test_mesh2d*.cpp`.
Depends on: p1-01, p1-04, p1-06, p1-07, p1-11, p1-13. Done-check: `ctest -L p1-08`.

### p1-09 State variables

Ranges `0x1402fe991-0x140309550` and `0x14032a000-0x14032d1ff`. RTTI `StateVarBase`,
`StateVarNode`, `StateVarNodePost`, `StateVarElem`, `StateVarElemSTS`, `StateVarElemTEPS`,
`StateVarElemDAMAG`, `StateVarElemMATAXI`, `StateVarElemCURRNT`, `StateVarCustom` (5-slot
vtables, shared base slots `140301690`-`1403018d0`).
Owns: layout headers `state_var_*.hpp`; `port/defmodel/{include/defmodel,src}/statevar/`; tests `test_statevar*.cpp`.
Depends on: p1-01, p1-05, p1-04, p1-07, p1-03, p1-11. Done-check: `ctest -L p1-09`; Phase 2
(DBREAD of node/element state variables) gives the oracle check.

### p1-10 Model members from the Fortran-area objects

Range `0x1403239f0-0x1403769f0` without the StateVar and Sdctl methods. Contents: string helpers
`1403239f0`/`140323bf0` called from many modules (put them in `defrt`, `def::rt`), the members of
`_DB_STEP_DATA` built here (`140339c90` at `+0x1358`, `14032f100` at `+0x33b8`), the container
methods `140329c70`-`140329ef0` used by p1-04, and the furnace-model object (`1403753f0`,
`1403769f0` of 7 KB, string "Furnace model").
Owns: `port/defrt/{include/defrt,src}/ftnstr/`, layout headers for the member classes above,
`port/defmodel/{include/defmodel,src}/members/`; tests `test_members*.cpp`, `test_ftnstr*.cpp`.
Depends on: p1-01, p1-09, p1-04, p1-05, p1-12, p1-11. Done-check: `ctest -L p1-10`. Several
packets call the string helpers: port and push them first.

### p1-11 Topology and digitized geometry

Ranges `0x140412010-0x14041f590`, `0x14008c3c0-0x14008ffff`, `0x1404cf5c0-0x1404dffff`. RTTI
`_DEF_LOOP`/`ILoop`, `_DEF_EDGE`/`IEdge`, `_DEF_VERTEX`/`IVertex`, `OBJ_GEO2D_DIGITIZED`
("to be implemented" stubs), and three Fortran FEM/geometry routines.
Owns: layout headers for these classes; `port/defmodel/{include/defmodel,src}/topo/`; tests `test_topo*.cpp`.
Depends on: p1-08, p1-07, p1-04, p1-12, p1-01. Done-check: `ctest -L p1-11`.

### p1-12 DB core

Ranges `0x1403b1e45-0x140407e81` (without p1-13), `0x14043658f-0x14046f6d0` (without licence),
`0x1402248c0-0x140224b10`, `0x140235620-0x14023b720`. `datarm23.f` / `DBOperations23.f`
(signatures "SFTC-DEFORM-2D-DB450", "SFTC-DEF-3D-DB100-00" ...), Fortran `OPEN` of the DB
(`140407e81`, "DIRECT", "UNFORMATTED"), `RMALLOC`/`RFREE`, low-level record read/write
(`FUN_14044b62a`), byte swapping and RLE. Most of the logic exists in `deformdb`
(`docs/db-format-spec.md`): the packet maps each function onto `deformdb` (status `ported` with
the `deformdb` function in the note, or a thin `defdbio` wrapper that keeps the original
interface and error texts) and ports what is missing. `1403b5a04` has a licence-error branch:
omit that branch only.
Owns: `port/defdbio/{include/defdbio,src}/dbcore/`; tests `test_dbcore*.cpp`; changes to
`deformdb/` only additive and announced.
Depends on: p1-10 (string helpers). Done-check: `ctest -L p1-12` on copies of the two test DBs;
oracle: the Phase 1 gate (`-m header|stepList|lastStepNo|simStepList` identical on every corpus
DB) runs mostly through this packet (57 of the functions the `-m` modes reach).

### p1-13 DB result-set reader

Range `0x1403d84b0-0x1403e5d60`. `FUN_1403d84b0` ("OPENING DATABASE FILE =", 4.7 KB) and
`FUN_1403d96fc` ("END OF READING DATABASE RESULT SET INFORMATION"), the result-set object (ctor
`FUN_1403ddf20`, dtor `FUN_1403de190`, an `eh vector constructor iterator` array of 100 x 0x30)
and its member containers `0x1403e2240`-`0x1403e5d60`.
Owns: `obj_mesh2d.hpp` (the result-set object `Cls_1403ddf20` is `OBJ_MESH2D`, 0x1700) and its members;
`port/defdbio/{include/defdbio,src}/resultset/`; tests `test_resultset*.cpp`.
Depends on: p1-04, p1-01, p1-12, p1-05, p1-08, p1-07. Done-check: `ctest -L p1-13`; oracle via
the `-m` cases that read result-set information.

### p1-14 Tree-container class

Range `0x14042a870-0x140431930`: a class with `std::map`/`std::set` members (ctor/dtor
`14042a870`, `14042b0c0`; used from `1402f91f0` in the `_OBJ_GEO3D` code) and its red-black tree
instantiations (two 2.2 KB insert routines `14042f160`, `14042fc50`). Expect most functions to be
`not-needed` (`std::map`), with iteration order and key comparison reproduced exactly.
Owns: layout header `cls_14042a870.hpp`; `port/defmodel/{include/defmodel,src}/tree/`; tests `test_tree*.cpp`.
Depends on: p1-11, p1-01, p1-04. Done-check: `ctest -L p1-14`.

## Layout headers: owners and stub rule

Headers in `port/defmodel/include/defmodel/layout/`, stubs in `port/defmodel/src/layout/`
(`docs/re/model-layout.md` gives the evidence for each class). The owner adds methods and fixes
offsets; anyone else asks the owner through the coordinator.

| Header | Class | Owner |
|---|---|---|
| `db_write_var.hpp`, `action_db_step.hpp`, `db_step_data.hpp` | `DB_WRITE_VAR`, `ACTION_DB_STEP`, `DB_STEP_DATA` | p1-03 |
| `cls_1401e7b10.hpp` | simulation control at `DB_STEP_DATA+0x750` | p1-03 |
| `cls_1401dbfe0.hpp` | `Cls_1401dbfe0` (used by the `-m` modes) | p1-03 |
| `cls_14001c770.hpp` | main's object | `-m` thread (Phase 5 later) |
| `llobj_data.hpp`, `iobject_data.hpp`, `cls_1401364a0.hpp`, `cls_14017a6c0.hpp` | `LLOBJ_DATA`, `IObjectData`, object containers | p1-04 |
| `sdctl_g_list.hpp`, `sdctl_ptr_list_obj_geo2d.hpp`, `sdctl_ptr_collection.hpp` | Sdctl lists | p1-05 |
| `sf_linked_data.hpp`, `sf_linked_list.hpp`, `dynary_data.hpp`, `dyn_ary_linked_list.hpp`, `cls_1401fa020.hpp`, `cls_1401f46e0.hpp`, `cls_1401fb020.hpp` | linked lists, `DYNARY_DATA` (RTTI, 0x40 bytes), inter-object data | p1-06 |
| `obj_geo_base.hpp`, `obj_geo2d.hpp`, `obj_geo3d.hpp`, `c2d3d_geometry.hpp`, `def_face.hpp`, `geo2d_and_mesh2d.hpp` | geometry objects (written by p1-07) | p1-07 |
| `c3d_cross_section.hpp`, `def_geoprim.hpp` | 2D mesh classes (written by p1-08) | p1-08 |
| `obj_mesh2d.hpp` | `OBJ_MESH2D` (0x1700) = the result-set object `Cls_1403ddf20` | p1-13 (p1-08 keeps vtable slots 0/1) |
| `def_loop.hpp`, `def_edge.hpp`, `def_vertex.hpp`, `obj_geo2d_digitized.hpp` | topology classes (written by p1-11) | p1-11 |
| `material_list.hpp` (was `cls_14029b4c0.hpp`) | material list at `DB_STEP_DATA+0x1328` | p1-08 |
| `material.hpp` (was `cls_14028c6f0.hpp`) and its sub-objects `cls_140288ae0.hpp` (FSTRES), `cls_140289930.hpp` (DIFCOE), `cls_140289b80.hpp` (CREEP), `cls_140289fe0.hpp` (ANISO), `cls_14028a4c0.hpp`/`cls_14028b850.hpp` (GRNDAT), `cls_14028bcc0.hpp` (BURGRS..RECVRY), `cls_14028c040.hpp` (FRCMOD/UTSDAT), `cls_14028c500.hpp` (TXTURE) | material object, 0x2218 bytes | material threads (Phase 2/3); p1-08 for anything the Phase 1 list code needs |
| `dynary.hpp` (was `cls_14012d050.hpp`) | 0x78-byte curve object, what the keyword specs call "DYNARY" | material threads; ask before changing |
| `phase_pair_list.hpp` (was `cls_140339c90.hpp`) | inter-material list at `DB_STEP_DATA+0x1358` | p1-10 |
| `phase_pair.hpp` (was `cls_140333f60.hpp`) | inter-material (phase pair) object, 0x780 bytes | phase threads (Phase 2/3); p1-10 for list code |
| `cls_14032f100.hpp`, `cls_14032f920.hpp`, `cls_1403753f0.hpp` | `DB_STEP_DATA` members from the Fortran-area objects, furnace model | p1-10 |
| `state_var_base.hpp` and the StateVar subclasses | state variables | p1-09 |
| `layout_check.hpp` | `DEF_LAYOUT_OFFSET` | foundation thread |

Old `cls_<addr>.hpp` names stay as one-line alias headers (`using Cls_14028c6f0 = Material;`),
so branches based on earlier commits still compile.

**Stub rule.** `src/layout/<class>.cpp` holds `DEF_NOT_PORTED` bodies so that vtables link. The
packet that owns a class defines the real bodies in its own `src/` folder, and deletes only the
matching stub definitions in a small separate commit. It deletes the whole stub file once the file
is empty. Nobody edits another owner's stub file.

## Dependencies and order

Calls between packets (edge counts, `phase1_packets.py` prints the full matrix) form cycles, so
no packet waits for another to finish: a call into another packet's function is declared in that
packet's header (owned by it) or stubbed with `DEF_NOT_PORTED` until it lands. Suggested start
order, so that the most-called pieces land first: p1-01, p1-10 (string helpers), p1-12, p1-06,
p1-04, p1-05; then p1-03, p1-07 with p1-08, p1-09, p1-11, p1-13, p1-14, p1-02.

Shared headers: the layout headers in `port/defmodel/include/defmodel/layout/` are owned per
class as listed above. A packet that needs a member of another packet's class asks the owner
through the coordinator (conventions section 4).

## What the oracle can check in Phase 1

Phase 1 output is only the `-m` text files. They exercise the `-m` functions, p1-12, part of
p1-03 (`FUN_1401dc3f0`), p1-13, and a few helpers in p1-06 and p1-10. The model constructors,
destructors and virtual methods (most of p1-03 to p1-11, p1-14) produce no output in Phase 1:
their done-check is tier 1 (layout asserts and the behaviour the decompile shows, with expected
values taken from the code or the binary's tables, never from running the port); the oracle checks
them from Phase 2 on (DBREAD + KFWRIT), which is when their status moves to `verified`.
