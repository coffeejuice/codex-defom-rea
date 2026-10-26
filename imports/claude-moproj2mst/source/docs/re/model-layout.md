# DEF_PRE_64 keyword model: class layouts (Phase 1)

This document records the C++ class layouts of the in-memory keyword model of `DEF_PRE_64.exe`
(v11.0.2, x64 MSVC, image base 0x140000000), recovered from the code only. These are the classes
that Phase 1 constructs and destroys: `_DB_STEP_DATA` and everything its constructor, init and
destructor reach, main's session object, and the material / phase-pair / object / inter-object
records hanging off its lists. The headers are the real port classes, not a mirror:

* headers: `port/defmodel/include/defmodel/layout/<name>.hpp` (namespace `def::model`), with
  `static_assert(sizeof)` and `DEF_LAYOUT_OFFSET` checks (macro in `layout_check.hpp`);
* link stubs: `port/defmodel/src/layout/<name>.cpp`. Every ctor, dtor and virtual is
  `DEF_NOT_PORTED(0x...)`, so vtables link;
* alias headers keep the pre-rename names compiling (`cls_14012d050.hpp` -> `DYNARY`,
  `cls_14028c6f0.hpp` -> `Material`, `cls_14029b4c0.hpp` -> `MaterialList`,
  `cls_140333f60.hpp` -> `PhasePair`, `cls_140339c90.hpp` -> `PhasePairList`).

All headers and stubs compile with MSVC 2022 (`/std:c++20 /W4 /permissive-`) and every layout
assertion holds. The GCC/Clang branch of `DEF_LAYOUT_OFFSET` was not compiled (no such compiler on
the build machine).

## Method

* **Inputs:** Phase-1 bulk decompile `deform-rea-work/out/p1/p1_all.c`, plus headless Ghidra
  decompiles on the `ghidra_p1g` copy (`deform-rea-work/out/p1/layout/d1.c` … `d11.c`).
* **RTTI:** the vtables, their slot targets and the RTTI class hierarchies were read directly from
  the PE image (Complete Object Locator, Class Hierarchy Descriptor, Base Class Descriptors). The
  slot counts were checked against the COL pointer that follows each table. For example, the
  hierarchy of `_DB_STEP_DATA` lists `_DB_WRITE_VAR` (mdisp 0) and `_ACTION_DB_STEP` (mdisp 320).
* **Offsets:** every member offset comes from a write in a constructor or init function, or from
  a read or write in a cited method. Fields that only some method touches are marked as such.
  Bytes the code never writes are explicit `pad_<off>` arrays.
* **Sizes:** one of the following, named per class.
  * the `operator new` size at the allocation site (list add functions);
  * the packing of `Cls_14001c770`, main's local, which holds DB_STEP_DATA, Cls_1401dbfe0,
    DB_STEP_DATA, Cls_1401dbfe0 back to back and ends exactly where main's next local starts;
  * the offset of the next member in the containing class;
  * the last written field.
* **Types:**
  * 4-byte fields are `int32_t`;
  * 8-byte fields are `double` when a double constant or a double compare is seen, pointers
    when freed or dereferenced, `uint64_t` when they are only ever zeroed;
  * single-byte flags are `uint8_t`.
* **Virtuals:** declared in slot order. A slot that is `_purecall` in a base is pure there and
  named after the derived implementation's role. Destructors sit in their real slot (MSVC scalar
  deleting dtor), and classes without a virtual dtor in the binary have none in the port.
* **Field names:** the keyword specs (`docs/re/keywords/*.md`, branch
  `claude/project-thread-ujpfry`) and the DB mapping (`docs/re/db-model-mapping.md` and
  `docs/re/dbmap/*.md`, branch `claude/project-thread-bz5k5e`) were used as leads for names. Each
  offset they give was checked against the code. Disagreements are listed below.

## All classes

| class | bases | size | header |
|---|---|---|---|
| DB_STEP_DATA | DB_WRITE_VAR, ACTION_DB_STEP | 0x3408 | db_step_data.hpp |
| DB_WRITE_VAR | - | 0x140 | db_write_var.hpp |
| ACTION_DB_STEP | - | 0x200 | action_db_step.hpp |
| Cls_1401dbfe0 | - | 0x1918 | cls_1401dbfe0.hpp |
| Cls_14001c770 | - | 0x9bd0 | cls_14001c770.hpp |
| Cls_1401e7b10 | - | 0xbd0 | cls_1401e7b10.hpp |
| DYNARY | - | 0x78 | dynary.hpp |
| Cls_1403753f0 | - | 0x70 | cls_1403753f0.hpp |
| Cls_1401364a0 | - | 0x18 | cls_1401364a0.hpp |
| MaterialList | - | 0x30 | material_list.hpp |
| Material | - | 0x2218 | material.hpp |
| Cls_140288ae0 | - | 0x618 | cls_140288ae0.hpp |
| Cls_140289930 | - | 0x178 | cls_140289930.hpp |
| Cls_140289b80 | - | 0x448 | cls_140289b80.hpp |
| Cls_14028a140 | - | 0x458 | cls_14028b850.hpp |
| Cls_14028b640 | - | 0x270 | cls_14028b850.hpp |
| Cls_14028b850 | - | 0x778 | cls_14028b850.hpp |
| Cls_14028a4c0 | - | 0x1f0 | cls_14028a4c0.hpp |
| Cls_14028bcc0 | - | 0x1e0 | cls_14028bcc0.hpp |
| Cls_14028c040 | - | 0x1f8 | cls_14028c040.hpp |
| Cls_140289fe0 | - | 0x88 | cls_140289fe0.hpp |
| Cls_14028c500 | - | 0xc | cls_14028c500.hpp |
| PhasePairList | - | 0x20 | phase_pair_list.hpp |
| PhasePair | - | 0x780 | phase_pair.hpp |
| Cls_14017a6c0 | - | 0x20 | cls_14017a6c0.hpp |
| IObjectData | - | 0x8 | iobject_data.hpp |
| LLOBJ_DATA | IObjectData | 0x3f28 | llobj_data.hpp |
| StateVarBase | - | 0x38 | state_var_base.hpp |
| SfLinkedData | - | 0x18 | sf_linked_data.hpp |
| DYNARY_DATA | SfLinkedData | 0x40 | dynary_data.hpp |
| SfLinkedList | - | 0x20 | sf_linked_list.hpp |
| DynAryLinkedList | SfLinkedList | 0x20 | dyn_ary_linked_list.hpp |
| SdctlPtrCollection | - | 0x10 | sdctl_ptr_collection.hpp |
| SdctlGList | SdctlPtrCollection | 0x38 | sdctl_g_list.hpp |
| SdctlPtrList_OBJ_GEO2D | SdctlGList | 0x38 | sdctl_ptr_list_obj_geo2d.hpp |
| Cls_1401fa020 | - | 0x20 | cls_1401fa020.hpp |
| Cls_1401f46e0 | - | 0x3a0 | cls_1401f46e0.hpp |
| Cls_1401fb020 | - | 0x168 | cls_1401fb020.hpp |
| Cls_14032f100 | - | 0x10 | cls_14032f100.hpp |
| Cls_14032f920 | - | 0x8 | cls_14032f920.hpp |

## Size evidence

| class | size | how the size was established |
|---|---|---|
| DB_STEP_DATA | 0x3408 | main's `Cls_14001c770`: DB_STEP_DATA at 0x190 and 0x4eb0, the next member at +0x3408 (0x3598, 0x82b8); last write int 0x3400 (1401d8200) |
| DB_WRITE_VAR | 0x140 | RTTI: second base `_ACTION_DB_STEP` at mdisp 0x140; last write 0x13c (1401baf30) |
| ACTION_DB_STEP | 0x200 | text buffer filled from a 0x400-byte line at +0x140; DB_STEP_DATA's own `strncpy(+0x340, s, 0x400)` bounds it (see open questions) |
| Cls_1401dbfe0 | 0x1918 | packing in `Cls_14001c770` (0x3598 -> 0x4eb0 minus 0x3408; 0x82b8 + 0x1918 = 0x9bd0) |
| Cls_14001c770 | 0x9bd0 | main's frame: local_a7e8 to the next local local_c18 = 0x9bd0; members end there |
| Cls_1401e7b10 | 0xbd0 | last write 0xbc8 (1401e8fb0); DB_STEP_DATA's next field at 0x750 + 0xbd0 = 0x1320 |
| DYNARY | 0x78 | back-to-back members in four classes; last field 0x68 |
| Cls_1403753f0 | 0x70 | last write 0x6f (140375470); at most 0x78, because BLZMAN is S+0x4b8 |
| Cls_1401364a0 | 0x18 | 3 fields; owner's next byte field at +0x18 |
| MaterialList | 0x30 | owner at +0x28; next DB_STEP_DATA member at +0x30 |
| Material | 0x2218 | `operator new(0x2218)` in 14029c060 |
| Material sub-objects | 0x618 / 0x178 / 0x448 / 0x778 / 0x1e0 / 0x1f8 / 0x88 / 0xc | next Material member; each ends with its last written field |
| PhasePairList | 0x20 | owner at +0x18; next field at +0x20 |
| PhasePair | 0x780 | `operator new(0x780)` in 140339df0 |
| Cls_14017a6c0 (object list) | 0x20 | owner at +0x18 |
| LLOBJ_DATA | 0x3f28 | `operator new(0x3f28)` in 14017a880 |
| IObjectData | 0x8 | vptr only |
| StateVarBase | 0x38 | last write 0x30; next LLOBJ_DATA member at +0x38 |
| SfLinkedData / DYNARY_DATA | 0x18 / 0x40 | next pointer at 0x10 / `operator new(0x40)` in 14025ba40 |
| SfLinkedList / DynAryLinkedList | 0x20 | last write 0x18; derived adds nothing |
| SdctlPtrCollection / SdctlGList / SdctlPtrList_OBJ_GEO2D | 0x10 / 0x38 / 0x38 | ctor writes; fills LLOBJ_DATA 0x3a60..0x3a98 |
| Cls_1401fa020 (inter-object list) | 0x20 | owner at +0x18 |
| Cls_1401f46e0 (inter-object) | 0x3a0 | `operator new(0x3a0)` in 1401faaa0 |
| Cls_1401fb020 | 0x168 | last write 0x160; record's own fields from 0x168 |
| Cls_14032f100 / Cls_14032f920 | 0x10 / 0x8 | ctor writes; DB_STEP_DATA's next field at 0x33c8 |

## Cross-checks with the keyword specs and the DB mapping

Agreements are the rule: every simulation-control offset of `dbmap/simctrl.md` matches a write in
the Cls_1401e7b10 init functions, and every material and phase-pair field named in the keyword
specs matches the Material / PhasePair constructors. Where the sources and the code disagree:

1. **"DYNARY_DATA, 0x78 bytes"** (`_common_g457.md` §4) mixes up two classes:
   * the 0x78-byte tabulated function built by 14012d050 has no vptr and no RTTI; the port names
     it `DYNARY`;
   * RTTI `DYNARY_DATA` (vtable 0x140534ea0) is a polymorphic 0x40-byte SfLinkedData node
     created by `DynAryLinkedList::create_item` 14025ba40 (`operator new(0x40)`).
2. **MTNAME length.** `dbmap/material.md` gives `+0xc char[64]`; the reader passes 0x41
   (`FUN_140413ab0(fp, +0xc, 0x41)`, MTNAME.md) and the next member is at 0x50. The port uses
   `char name[0x41]` plus 3 padding bytes; the DB copies 64 of them.
3. **FRCMOD type-11 reals.** `dbmap/material.md` puts the 9 reals at `+0x1bb0..0x1bf0`; the
   code and FRCMOD.md put them at `+0x1c80..0x1cc0`. The ctor 14028c040 zeroes sub+0x1b0..0x1f0
   with the sub-object at 0x1ad0, and 0x1bb0 lies inside the FRCMOD t12 DYNARY (0x1b90..0x1c07).
   The mapping's offsets look wrong; to check in its writer 140294ae0.
4. **"LLMAT_DATA"** (`dbmap/material.md`) is not an RTTI name in the binary (no vtable among the
   474 pointer tables); the material class is not polymorphic. The port calls it `Material`.
5. **PhasePair type-9 TTTD table.** TTTD.md lists type 0x2c, the ctor types it 0x37; the reader
   re-types the DYNARY, so both can hold.
6. **MSTMTR flag values.** MSTMTR.md has 1 / cleared, `dbmap/material.md` adds 2 for the listed
   component materials (1401d6250 on DB read). This is not a layout conflict.
7. **DB_STEP_DATA+0x13f4.** The DB mapping reads it from IISIM[33] ("NoRelations"), PHASTF.md has
   1401d6450 recount active pairs into it. Same field, two writers.
8. **LLOBJ_DATA correction.** The first version of llobj_data.hpp put `Cls_140197a50` and the six
   heap handles 0x300 bytes too low (0x724·8 was computed as 0x3620; it is 0x3920). Packet p1-04
   reported it; it was re-checked in 14015ec80 / 14015fbf0 and fixed in c46e87c, together with
   the 0x10..0x823 block and the name length (char[0x54]) from 14015fbf0.
9. **LLOBJ_DATA opaque slots hold LLOBJ fields** (`dbmap/object.md`, consistent with the code;
   the headers are unchanged because these are not offset errors, the slots only need splitting):
   * `s2e68` (0x4c0) = Cls_1401956d0 0x2e68..0x3007 (lists at 0x2e80..0x2fd0) + the 800-byte
     raw copy of the first LLBND record at 0x3008..0x3327 (140174e40 `memcpy(+0x3008, .., 800)`);
   * `s3458` (0x140) = _OBJ_GEO3D (0x130, as at 0x3328) + int first/last LLBND record at
     0x3588/0x358c and the count at 0x3590 for the internal function at 0x3598;
   * `pad_890`: 0x890 2D OBJTYP flag, 0x894 nodes per element; `pad_bc8`: 0xbd8 boundary-loop
     count; `pad_3980`: 0x3984/0x3988 first global node/element, 0x398c first boundary face;
   * `s2d10`+0x40 = 0x2d50 is the remesh window list (MGUSER).

## Open questions

* ACTION_DB_STEP's extent. Its ctor only clears byte 0. Whether the 0x400-byte buffer at
  DB_STEP_DATA+0x340 and the ints 0x740..0x748 belong to it or to DB_STEP_DATA is not visible.
  Offsets are unaffected.
* Furnace block size 0x70 vs 0x78 (0x4b0..0x4b7 of the simulation control is never written).
* DB_STEP_DATA bytes never written by ctor/init/dtor: 0x13a0..0x13a7, 0x1402..0x15f7,
  0x2f8a..0x338f.
* The second object list at DB_STEP_DATA+0x13a8: role unknown.
* Cls_1401dbfe0 0x1770..0x18ff unused by ctor/init/dtor; the meaning of its tables (DB file
  state).
* The heap list behind DB_STEP_DATA+0x33b8 (CVNAME container): value type (0x38 bytes: string at
  +8, 8-byte member at +0x30).
* LLOBJ_DATA opaque sub-objects (sizes are slot sizes, i.e. upper bounds) and gaps 0x3610..0x391f,
  0x39a8..0x39c7, 0x39f8..0x3a5f, 0x3aa0..0x3bf7. `_OBJ_GEO3D` is owned by packet p1-07.
* Material: 0x004..0x00b, 0x1ed0 and the DYNARY at 0x2058 have no keyword in groups 4/5/7.
* Inter-object record: the 0x30-byte sub-object at 0x188 (ctor 140269250, owner at +0x20; next
  to the LLWINDEF_DATA / LLRANGEDEF_DATA code).
* `DYNARY` 0x5c and 0x70 never written; the 8 bytes at 0x08 are a double but some types clear them
  as two ints.
* Classes named in the task but not reached from these constructors: FRICTION_DATA,
  LLWINDEF_DATA, LLRANGEDEF_DATA, OPSTOP_DATA / OpStopLinkedList, StateVar* subclasses. Their
  vtables are in `ledger/ptrtables.tsv`; they are left to the owning packets.

## Classes

Per class: header comment (addresses, size evidence, open points), then the ctor/dtor/virtual
table (`size` = function size in bytes from `pre64_funcs.tsv`) and the member table. Offsets are
bytes from the start of the class; for Material sub-objects the comment also gives the absolute
material offset used in the keyword specs.

## DB_STEP_DATA

Header `port/defmodel/include/defmodel/layout/db_step_data.hpp`, stubs `port/defmodel/src/layout/db_step_data.cpp`.

```
DB_STEP_DATA (RTTI .?AV_DB_STEP_DATA@@): the keyword model of one step (simulation control,
materials, objects, inter-object/inter-material data, DB address tables and buffers).
Bases (RTTI class hierarchy, 3 entries): _DB_WRITE_VAR at 0 (primary, has the vptr) and
_ACTION_DB_STEP at 0x140 (mdisp=320, not polymorphic).

  vtable   0x14052ae10 (1 slot)
  ctor     1401c8950(this, dim) -> 1401bae10, 1401d8380, members, owners, 1401c8c00(this, dim)
  init     1401c8c00 (also run by slot 0 when the dimension changes)
  free     1401cb440 (frees arrays and list contents; the dtor calls it when 0x173c == 0,
           otherwise it only resets the lists without freeing their nodes)
  dtor     1401cb2c0 (non-virtual: the class has no virtual dtor)
  size     0x3408: main (FUN_14001c9f0) holds Cls_14001c770 whose DB_STEP_DATA members are
           0x3408 apart from the following Cls_1401dbfe0 (0x190 -> 0x3598, 0x4eb0 -> 0x82b8);
           last field written: int at 0x3400 (1401d8200). FUN_14001c500's stack copies are
           0x3410 apart (stack alignment).
  open     0x13a0..0x13a7, 0x1402..0x15f7, 0x2f8a..0x338f never written by ctor/init/dtor
           (0x3008 is an 800-byte memcpy target in 140174e40 but that function's param may be
           another class); ownership of 0x340..0x74f between ACTION_DB_STEP and this class.
```

Bases: DB_WRITE_VAR, ACTION_DB_STEP.

| @src | size (bytes) | role | declaration |
|---|---|---|---|
| 1401c8950 | 574 | ctor; dim = 2 or 3 (callers pass 3) | `explicit DB_STEP_DATA(std::int32_t dim);` |
| 1401cb2c0 | 306 | dtor (non-virtual) | `~DB_STEP_DATA();` |
| 1401c8ba0 | 87 | slot 0: if *dim != dimension: DB_WRITE_VAR::set_dimension, 1401cb440, 1401c8c00(this, *dim) | `void set_dimension(const std::int32_t* dim) override;` |

| offset | type | member | evidence / meaning |
|---|---|---|---|
| 0x340 | `char[0x400]` | u340 | ctor strcpy "DEFORM"; KEY command (140378e00) strncpy 0x400 |
| 0x740 | `std::int32_t` | u740 | 1401c8c00 =0; 1401cb440 =0; read by 140378e00 |
| 0x744 | `std::int32_t` | u744 | =-1; step number passed to DBREAD 1401cc2a0 (1401c5810) |
| 0x748 | `std::int32_t` | u748 | =0; platform code for text byte swap (FUN_140324020, mapping) |
| 0x74c | `std::byte[4]` | pad_74c |  |
| 0x750 | `Cls_1401e7b10` | sim_ctrl | simulation control (owner set by 1401e7c10) |
| 0x1320 | `std::int32_t` | n_materials | number of materials (140384a50 stores the list count; read by 1401d09c0) |
| 0x1324 | `std::byte[4]` | pad_1324 |  |
| 0x1328 | `MaterialList` | materials | material list (owner by 1401d8300) |
| 0x1358 | `PhasePairList` | phase_pairs | phase-pair (inter-material) list (owner by 1401d8320) |
| 0x1378 | `std::int32_t` | u1378 | number of objects (mapping); zeroed by 1401cb440 |
| 0x137c | `std::byte[4]` | pad_137c |  |
| 0x1380 | `Cls_14017a6c0` | objects | object list (owner by 1401d8340) |
| 0x13a0 | `std::byte[8]` | pad_13a0 | never written |
| 0x13a8 | `Cls_14017a6c0` | objects2 | second object list (owner by 1401d8340) |
| 0x13c8 | `Cls_1401fa020` | inter_obj | inter-object list (owner by 1401d8360) |
| 0x13e8 | `std::int32_t` | u13e8 | NUMNP (mapping); multiplied by u3398 in GENDB 1401c4a30 |
| 0x13ec | `std::int32_t` | u13ec | NUMEL (mapping); multiplied by u33a8 in 1401c4be0 |
| 0x13f0 | `std::int32_t` | u13f0 | boundary count (mapping) |
| 0x13f4 | `std::int32_t` | n_phase_on | pairs with phastf_on == 1 (1401d6450, PHASTF.md); read by 1401d09c0 |
| 0x13f8 | `std::int32_t` | u13f8 | number of windows (mapping) |
| 0x13fc | `std::int32_t` | u13fc | NFN, functions in IIFNC (mapping) |
| 0x1400 | `std::uint8_t` | u1400 | =0 |
| 0x1401 | `std::uint8_t` | u1401 | =0 |
| 0x1402 | `std::byte[0x1f6]` | pad_1402 | never written by ctor/init/dtor |
| 0x15f8 | `std::int32_t[0x4f]` | u15f8 | state-variable layout table (1401d09c0 fills; 1401d0fc0 rebuilds) |
| 0x1734 | `std::int32_t` | u1734 | =0; from step header double [8] (1401cca80) |
| 0x1738 | `std::int32_t` | u1738 | =1; ==3 tested by main's INI loader 140009f20 |
| 0x173c | `std::uint8_t` | u173c | =0; nonzero = the dtor does not free list contents |
| 0x173d | `std::byte[3]` | pad_173d |  |
| 0x1740 | `std::int32_t` | u1740 | ctor =0; set by KEY reader 1401b5f00 |
| 0x1744 | `std::int32_t` | u1744 | ctor =0; set by KEY writer 1401c5810 |
| 0x1748 | `char[0x400]` | u1748 | ctor strcpy "DEFORM", then [0] = 0 |
| 0x1b48 | `std::int32_t` | u1b48 | 1401ca960 =0 |
| 0x1b4c | `std::int32_t` | u1b4c | =0 |
| 0x1b50 | `std::uint64_t` | u1b50 | =0 |
| 0x1b58 | `std::uint8_t` | u1b58 | ctor =1 |
| 0x1b59 | `std::uint8_t` | u1b59 | ctor =1 |
| 0x1b5a | `std::uint8_t` | u1b5a | ctor =0 |
| 0x1b5b | `std::uint8_t` | u1b5b | ctor =0 |
| 0x1b5c | `std::uint8_t` | u1b5c | setter 140158fa0 (ctor passes 0) |
| 0x1b5d | `std::uint8_t` | u1b5d | init =1 |
| 0x1b5e | `std::byte[2]` | pad_1b5e |  |
| 0x1b60 | `std::int32_t` | u1b60 | setter 1401cc120 |
| 0x1b64 | `std::int32_t` | step_version | setter 1401cc160, getter 1401cc180 |
| 0x1b68 | `std::int32_t` | u1b68 | MAR length (1401cd1f0) |
| 0x1b6c | `std::int32_t` | u1b6c | A length (1401cd1f0) |
| 0x1b70 | `std::int32_t` | u1b70 | set by 1401cca80 |
| 0x1b74 | `std::int32_t` | u1b74 | element count of a calloc(n, 8) array (1401cd1f0) |
| 0x1b78 | `void*` | u1b78 | A array (DBREAD), freed by 1401cb440 |
| 0x1b80 | `double*` | u1b80 | step header (doubles), freed by 1401cb440 |
| 0x1b88 | `void*` | u1b88 | MAR array (DBREAD), freed by 1401cb440 |
| 0x1b90 | `std::int32_t[0x8c]` | u1b90 | MAR address table: 70 starts + 70 widths (1401cb170) |
| 0x1dc0 | `std::int32_t[0x118]` | u1dc0 | A address table: 140 starts + 140 widths (1401cb170) |
| 0x2220 | `void*` | u2220 | IIFNC region (DBREAD) |
| 0x2228 | `void*` | u2228 | LLFNC region |
| 0x2230 | `void*` | u2230 | LLWIN region |
| 0x2238 | `void*` | u2238 | LLWRZ region |
| 0x2240 | `void*` | u2240 | freed by 1401cb440 |
| 0x2248 | `void*` | u2248 | freed by 1401cb440 |
| 0x2250 | `void*` | u2250 | freed by 1401cb440 |
| 0x2258 | `void*` | u2258 | freed by 1401cb440 |
| 0x2260 | `std::int32_t[0x8c]` | u2260 | 1401cb1f0 |
| 0x2490 | `std::int32_t` | u2490 | 1401cb1f0 |
| 0x2494 | `std::int32_t[0x8c]` | u2494 | 1401cb1f0 |
| 0x26c4 | `std::int32_t[0x118]` | u26c4 | 1401cb1f0 |
| 0x2b24 | `std::int32_t` | u2b24 | 1401cb1f0 |
| 0x2b28 | `std::int32_t[0x118]` | u2b28 | 1401cb1f0 |
| 0x2f88 | `std::uint8_t` | u2f88 | 1401d7f10 =0 |
| 0x2f89 | `std::uint8_t` | u2f89 | 1401d7ef0/1401d7f10 =0 |
| 0x2f8a | `std::byte[0x406]` | pad_2f8a | never written by ctor/init/dtor |
| 0x3390 | `Cls_1401dbfe0*` | db | companion DB object (14001c770, FUN_14001c500) |
| 0x3398 | `std::int32_t` | u3398 | number of strings in u33a0 |
| 0x339c | `std::byte[4]` | pad_339c |  |
| 0x33a0 | `char**` | u33a0 | UNNAME node names (mapping; 1401cb9e0 alloc, 1401cb910 free) |
| 0x33a8 | `std::int32_t` | u33a8 | number of strings in u33b0 |
| 0x33ac | `std::byte[4]` | pad_33ac |  |
| 0x33b0 | `char**` | u33b0 | UENAME element names (mapping; 1401cb730/1401cb7d0 alloc, 1401cb660 free) |
| 0x33b8 | `Cls_14032f100` | u33b8 | CVNAME container (mapping: KEY-only); int + heap list |
| 0x33c8 | `std::int32_t` | u33c8 | init =1; nodal DAMAGE width (mapping) |
| 0x33cc | `std::byte[0xc]` | pad_33cc |  |
| 0x33d8 | `std::int32_t` | u33d8 | init =1; element DAMAGE width (mapping) |
| 0x33dc | `std::byte[0xc]` | pad_33dc |  |
| 0x33e8 | `DB_STEP_DATA*` | companion | other-dimension model (DIM.md; 1401d8000 sets, 1401d8050 forces it to the other dim) |
| 0x33f0 | `std::uint8_t` | use_companion | DIM flag: 1401d81b0 returns companion when set; 1401d7fd0 =0 |
| 0x33f1 | `std::byte[7]` | pad_33f1 |  |
| 0x33f8 | `std::uint64_t` | u33f8 | 1401d8200 =0 |
| 0x3400 | `std::int32_t` | u3400 | 1401d8200 =0 |
| 0x3404 | `std::byte[4]` | pad_3404 |  |


## DB_WRITE_VAR

Header `port/defmodel/include/defmodel/layout/db_write_var.hpp`, stubs `port/defmodel/src/layout/db_write_var.cpp`.

```
DB_WRITE_VAR (RTTI .?AV_DB_WRITE_VAR@@): base of DB_STEP_DATA, holds the DB header/version
state and the GENDB output buffers.

  vtable   0x140528290 (1 slot)            ctor  1401bae10 (inits via 1401baf30)
  dtor     1401baf10 (non-virtual: only restores the vptr; the class has no virtual dtor)
  size     0x140: DB_STEP_DATA's second base ACTION_DB_STEP sits at 0x140 (RTTI base class
           descriptor of _DB_STEP_DATA: mdisp=320), and 1401baf30 writes up to 0x13c.
  open     8-byte fields at 0xe8/0xf0/0xf8 are only zeroed (pointer or double unknown);
           0x4c and 0x0a..0x0b are padding by alignment (never written).
```

| @src | size (bytes) | role | declaration |
|---|---|---|---|
| 1401bae10 | 245 | ctor (sets vptr, small fields, then calls 1401baf30) | `DB_WRITE_VAR();` |
| 1401baf10 | 21 | dtor (non-virtual) | `~DB_WRITE_VAR();` |
| 1401c5700 | 26 | slot 0: stores *dim at 0x1c (getter 140158e90 reads it back) | `virtual void set_dimension(const std::int32_t* dim);` |

| offset | type | member | evidence / meaning |
|---|---|---|---|
| 0x008 | `std::uint8_t` | u008 | 1401bae10 =1; setter 1401c5720; GENDB direct-write flag (mapping: 0 = write nothing) |
| 0x009 | `std::uint8_t` | u009 | 1401bae10 =1 |
| 0x00c | `std::int32_t` | version | 1401baf30 =0x44c (1100) |
| 0x010 | `char[0xc]` | date | 1401baf30 strcpy "06/30/14" |
| 0x01c | `std::int32_t` | dimension | 1401bae10 =3; set_dimension, getter 140158e90 |
| 0x020 | `std::int32_t` | u020 | 1401bae10 =0 |
| 0x024 | `std::int32_t` | u024 |  |
| 0x028 | `std::int32_t` | u028 |  |
| 0x02c | `std::int32_t` | u02c |  |
| 0x030 | `std::int32_t` | u030 | MAR buffer length (GENDB, per DBREAD/GENDB brief) |
| 0x034 | `std::int32_t` | u034 | A buffer length (GENDB) |
| 0x038 | `std::int32_t` | u038 |  |
| 0x03c | `std::int32_t` | u03c |  |
| 0x040 | `std::int32_t` | u040 | 1401bae10 =0 |
| 0x044 | `std::int32_t` | u044 |  |
| 0x048 | `std::int32_t` | u048 |  |
| 0x050 | `void*` | u050 | GENDB MAR buffer, freed by 1401bb320 |
| 0x058 | `void*` | u058 | GENDB A buffer, freed by 1401bb320 |
| 0x060 | `void*` | u060 | GENDB step header buffer, freed by 1401bb320 |
| 0x068 | `std::int32_t` | u068 | 1401baf30 =0 |
| 0x06c | `std::int32_t` | u06c | setter 1401c5740; NUMNP for GENDB (mapping, IISIM[19]) |
| 0x070 | `std::int32_t` | u070 | setter 1401c5760; NUMEL for GENDB (mapping) |
| 0x074 | `std::int32_t[21]` | u074 | ..0x0c4 1401baf30 zeroes each; GENDB counts (mapping: 0x78 materials, |
| 0x0c8 | `std::int32_t` | u0c8 | setter 1401c5780 |
| 0x0cc | `std::int32_t` | u0cc | setter 1401c57a0 |
| 0x0d0 | `std::int32_t` | u0d0 | setter 14019a310 |
| 0x0d4 | `std::int32_t` | u0d4 | setter 14019a330 |
| 0x0d8 | `std::int32_t` | u0d8 |  |
| 0x0dc | `std::int32_t` | u0dc |  |
| 0x0e0 | `std::int32_t` | u0e0 | 1401baf30 =0x10 |
| 0x0e4 | `std::int32_t` | u0e4 |  |
| 0x0e8 | `std::uint64_t` | u0e8 | zeroed (8 bytes) |
| 0x0f0 | `std::uint64_t` | u0f0 | zeroed (8 bytes) |
| 0x0f8 | `std::uint64_t` | u0f8 | zeroed (8 bytes) |
| 0x100 | `std::int32_t` | u100 |  |
| 0x104 | `std::int32_t` | u104 |  |
| 0x108 | `std::int32_t` | u108 |  |
| 0x10c | `std::int32_t` | u10c | GENDB counter (mapping) |
| 0x110 | `std::int32_t` | u110 | setter 14019a2d0 |
| 0x114 | `std::int32_t` | u114 | setter 140199d00 |
| 0x118 | `std::int32_t` | u118 | setter 140199d40 |
| 0x11c | `std::int32_t` | u11c | setter 14019a0e0 |
| 0x120 | `std::int32_t` | u120 |  |
| 0x124 | `std::int32_t` | u124 | setter 14019a120 |
| 0x128 | `std::int32_t` | u128 | setter 14019a160 |
| 0x12c | `std::int32_t` | u12c | setter 14019a1a0 |
| 0x130 | `std::int32_t` | u130 | setter 14019a1e0 |
| 0x134 | `std::int32_t` | u134 | GENDB window-row cursor (mapping) |
| 0x138 | `std::int32_t` | u138 | GENDB window-row cursor (mapping) |
| 0x13c | `std::int32_t` | u13c |  |


## ACTION_DB_STEP

Header `port/defmodel/include/defmodel/layout/action_db_step.hpp`, stubs `port/defmodel/src/layout/action_db_step.cpp`.

```
ACTION_DB_STEP (RTTI .?AV_ACTION_DB_STEP@@, base class only, not polymorphic): second base of
DB_STEP_DATA, at DB_STEP_DATA+0x140.

  ctor     1401d8380 (writes byte 0 = 0)       dtor 1401d83a0 (empty)
  vtable   none
  size     0x200 (assumed, see open question). Evidence: command keyword 0x11e in
           FUN_140378e00 calls 140382320(step+0x140, line) = strcpy into this+0 (line read with
           fgets(..., 0x400)); DB_STEP_DATA's own char buffer at 0x340 is filled with
           strncpy(step+0x340, s, 0x400) in the same function, so this class ends at or before
           0x340-0x140 = 0x200.
  method   140382320 sets the text (strcpy); not declared here (layout only).
  open     whether the 0x400-byte buffer at DB_STEP_DATA+0x340 (and the ints at 0x740..0x748)
           belong to this class or to DB_STEP_DATA cannot be told from the code seen: the
           ctor only touches byte 0. Byte offsets in DB_STEP_DATA are the same either way.
```

| @src | size (bytes) | role | declaration |
|---|---|---|---|
| 1401d8380 | 19 | ctor: text[0] = 0 | `ACTION_DB_STEP();` |
| 1401d83a0 | 6 | dtor (empty) | `~ACTION_DB_STEP();` |

| offset | type | member | evidence / meaning |
|---|---|---|---|
| 0x000 | `char[0x200]` | text | cleared by 1401d8380; strcpy by 140382320 (KEY command 0x11e) |


## Cls_1401dbfe0

Header `port/defmodel/include/defmodel/layout/cls_1401dbfe0.hpp`, stubs `port/defmodel/src/layout/cls_1401dbfe0.cpp`.

```
Cls_1401dbfe0 (no RTTI, not polymorphic): companion object of a DB_STEP_DATA, linked from
DB_STEP_DATA+0x3390 (14001c770 stores &db at step+0x3390; FUN_14001c500 does the same for its
stack pair). DBREAD gets it as its second argument: 1401cc2a0(step, *(step+0x3390), stepno, 0)
(KEY writer 1401c5810). Other users: GENDB 1401bb550, 14013dba0, 1401d7680, 1401b9c00,
1401e4d80. It holds .DB file/step-directory state (two 0x400-char buffers, a 100-entry and a
400-entry table, two arrays deleted with vector delete).

  ctor     1401dbfe0 (1401dc030, then 0x1908 = 0, 0x1910 = 0)
  init     1401dc030
  dtor     1401dc1c0 (1401dc220: delete[] 0xad8/0xae8 when 0xac8 != 0, free the 100 pointers at
           0x330; then free 0x1908)
  size     0x1918: in main (FUN_14001c9f0) the model object Cls_14001c770 packs
           DB_STEP_DATA(0x3408) + this + DB_STEP_DATA + this with no gaps (offsets 0x190,
           0x3598, 0x4eb0, 0x82b8, end 0x9bd0 = next stack local); last field int at 0x1910.
           (FUN_14001c500 spaces its two stack instances 0x1920 apart: stack alignment.)
  open     0x1770..0x18ff never written by ctor/init/dtor; role of the tables.
```

| @src | size (bytes) | role | declaration |
|---|---|---|---|
| 1401dbfe0 | 60 | ctor | `Cls_1401dbfe0();` |
| 1401dc1c0 | 87 | dtor | `~Cls_1401dbfe0();` |

| offset | type | member | evidence / meaning |
|---|---|---|---|
| 0x000 | `std::int32_t` | u000 | =0 |
| 0x004 | `std::int32_t` | u004 | =0 |
| 0x008 | `std::int32_t` | u008 | =1 |
| 0x00c | `Pair[100]` | u00c | loop i<100 |
| 0x330 | `void*[100]` | u330 | loop i<100 = 0; each freed by 1401dc220 |
| 0x650 | `char[0x400]` | u650 | [0] = 0 |
| 0xa50 | `char[0x51]` | ua50 | [0] = 0 (end inferred from the next written byte) |
| 0xaa1 | `char[8]` | uaa1 | [0] = 0 |
| 0xaa9 | `char[8]` | uaa9 | [0] = 0 |
| 0xab1 | `char[8]` | uab1 | [0] = 0 (0xab9..0xabb padding) |
| 0xabc | `std::int32_t` | uabc |  |
| 0xac0 | `std::int32_t` | uac0 |  |
| 0xac4 | `std::int32_t` | uac4 |  |
| 0xac8 | `std::int32_t` | uac8 | nonzero = arrays at 0xad8/0xae8 allocated (1401dc220) |
| 0xacc | `std::int32_t` | uacc |  |
| 0xad0 | `std::int32_t` | uad0 |  |
| 0xad4 | `std::byte[4]` | pad_ad4 |  |
| 0xad8 | `void*` | uad8 | array, delete[] (vector deleting dtor, flag 3) |
| 0xae0 | `std::int32_t` | uae0 | =-1 |
| 0xae4 | `std::byte[4]` | pad_ae4 |  |
| 0xae8 | `void*` | uae8 | array, delete[] (vector deleting dtor, flag 3) |
| 0xaf0 | `std::uint64_t[400]` | uaf0 | loop i<400 = 0 (8-byte entries) |
| 0x1770 | `std::byte[0x190]` | pad_1770 | not written by ctor/init/dtor |
| 0x1900 | `std::uint8_t` | u1900 | =0 (also reset by 1401dc220) |
| 0x1901 | `std::byte[7]` | pad_1901 |  |
| 0x1908 | `void*` | u1908 | malloc'd, freed by 1401dc1c0 |
| 0x1910 | `std::int32_t` | u1910 | =0 |
| 0x1914 | `std::byte[4]` | pad_1914 |  |


## Cls_14001c770

Header `port/defmodel/include/defmodel/layout/cls_14001c770.hpp`, stubs `port/defmodel/src/layout/cls_14001c770.cpp`.

```
Cls_14001c770 (no RTTI, not polymorphic): main's interactive-session object (local
local_a7e8 of main FUN_14001c9f0): menu state for TEXT_PRE plus two keyword models (step A and
step B), each with its companion DB object. main calls 140009f20(this) (INI/DEF_MULTI.INI) and
the menu loop 14001ad90(this).

  ctor     14001c770 (menu defaults, 1401c8950(+0x190, 3), 1401dbfe0(+0x3598),
           1401c8950(+0x4eb0, 3), 1401dbfe0(+0x82b8), links step->db, 1401d8000(A, B))
  dtor     none: inlined in main (1401dc1c0(+0x82b8), 1401cb2c0(+0x4eb0), 1401dc1c0(+0x3598),
           1401cb2c0(+0x190))
  size     0x9bd0: main's frame puts the next local (local_c18) at 0xa7e8 - 0x9bd0; the last
           member Cls_1401dbfe0 (0x1918) ends there (0x82b8 + 0x1918 = 0x9bd0).
  open     0x000..0x18f is a run of small menu records {int on, int n_items, ptr, ptr} (0x18
           bytes) that the menu loop 14001ad90 fills with model pointers; their classes and
           the 8-byte fields that are only zeroed are not recovered.
```

| @src | size (bytes) | role | declaration |
|---|---|---|---|
| 14001c770 | 634 | ctor | `Cls_14001c770();` |

| offset | type | member | evidence / meaning |
|---|---|---|---|
| 0x000 | `std::int32_t` | u000 | =0, then =1 at the end of the ctor |
| 0x004 | `std::int32_t` | u004 | =0, then =9 |
| 0x008 | `std::int32_t` | u008 | =1   (record at 0x008 -> 14001a050(this+8)) |
| 0x00c | `std::int32_t` | u00c | =10 |
| 0x010 | `std::byte[8]` | pad_010 |  |
| 0x018 | `void*` | u018 | = &step_a (14001ad90) |
| 0x020 | `std::int32_t` | u020 | =0 |
| 0x024 | `std::byte[0xc]` | pad_024 |  |
| 0x030 | `std::int32_t` | u030 | =1 |
| 0x034 | `std::int32_t` | u034 | =5 |
| 0x038 | `void*` | u038 | = &step_a.sim_ctrl (this+0x8e0, 14001ad90) |
| 0x040 | `void*` | u040 | = &step_a (14001ad90) |
| 0x048 | `std::int32_t` | u048 | =1 |
| 0x04c | `std::int32_t` | u04c | =5 |
| 0x050 | `std::byte[0x10]` | pad_050 |  |
| 0x060 | `std::int32_t` | u060 | =1 |
| 0x064 | `std::int32_t` | u064 | =0x11 |
| 0x068 | `std::byte[0x10]` | pad_068 |  |
| 0x078 | `std::int32_t` | u078 | =1 |
| 0x07c | `std::int32_t` | u07c | =10 |
| 0x080 | `std::byte[0x10]` | pad_080 |  |
| 0x090 | `std::int32_t` | u090 | =1 |
| 0x094 | `std::int32_t` | u094 | =5 |
| 0x098 | `std::byte[0x10]` | pad_098 |  |
| 0x0a8 | `std::int32_t` | u0a8 | =1   (record at 0x0a8 -> 1400157c0(this+0xa8)) |
| 0x0ac | `std::int32_t` | u0ac | =9 |
| 0x0b0 | `std::byte[8]` | pad_0b0 |  |
| 0x0b8 | `void*` | u0b8 | = &step_a (14001ad90) |
| 0x0c0 | `std::int32_t` | u0c0 | =1   (record at 0x0c0 -> 140019c20(this+0xc0)) |
| 0x0c4 | `std::int32_t` | u0c4 | =4 |
| 0x0c8 | `std::byte[8]` | pad_0c8 |  |
| 0x0d0 | `void*` | u0d0 | = &step_a (14001ad90) |
| 0x0d8 | `std::uint64_t` | u0d8 | =0 |
| 0x0e0 | `std::uint64_t` | u0e0 | =0 |
| 0x0e8 | `std::uint64_t` | u0e8 | =0 |
| 0x0f0 | `std::uint64_t` | u0f0 | =0 |
| 0x0f8 | `std::uint64_t` | u0f8 | =0 |
| 0x100 | `double` | u100 | =-1.0 |
| 0x108 | `std::int32_t` | u108 | =1 |
| 0x10c | `std::int32_t` | u10c | =2 |
| 0x110 | `double` | u110 | =1e-4 (0x3f1a36e2eb1c432d) |
| 0x118 | `std::uint64_t` | u118 | =0 |
| 0x120 | `std::uint64_t` | u120 | =0 |
| 0x128 | `std::uint64_t` | u128 | =0 |
| 0x130 | `std::uint64_t` | u130 | =0 |
| 0x138 | `std::uint64_t` | u138 | =0 |
| 0x140 | `double` | u140 | =1.0 |
| 0x148 | `std::uint64_t` | u148 | =0 |
| 0x150 | `std::int32_t` | u150 | =1 |
| 0x154 | `std::byte[4]` | pad_154 |  |
| 0x158 | `double` | u158 | =1e-4 |
| 0x160 | `std::int32_t` | u160 | =1 |
| 0x164 | `std::int32_t` | u164 | =1000 |
| 0x168 | `std::int32_t` | u168 | =0 |
| 0x16c | `std::int32_t` | u16c | =0 |
| 0x170 | `std::int32_t` | u170 | =1 |
| 0x174 | `std::int32_t` | u174 | =9 |
| 0x178 | `void*` | u178 | = &step_a (14001ad90) |
| 0x180 | `void*` | u180 | = &db_a (this+0x3598, 14001ad90) |
| 0x188 | `std::int32_t` | u188 | =1   (record at 0x188 -> 14000afd0(this+0x188)) |
| 0x18c | `std::int32_t` | u18c | =10 |
| 0x190 | `DB_STEP_DATA` | step_a | 1401c8950(.., 3); main passes it to the KEY/INI code |
| 0x3598 | `Cls_1401dbfe0` | db_a | step_a.db = &db_a |
| 0x4eb0 | `DB_STEP_DATA` | step_b | 1401c8950(.., 3); step_a.companion = &step_b (1401d8000) |
| 0x82b8 | `Cls_1401dbfe0` | db_b | step_b.db = &db_b |


## Cls_1401e7b10

Header `port/defmodel/include/defmodel/layout/cls_1401e7b10.hpp`, stubs `port/defmodel/src/layout/cls_1401e7b10.cpp`.

```
Cls_1401e7b10 (no RTTI, not polymorphic): simulation control block S = DB_STEP_DATA+0x750
(keyword group 1; DBREAD 1401eb870, GENDB 1401effc0, KEY writer 1403d4990, KEY reader 14039aa90).
Members carry the keyword names of docs/re/dbmap/simctrl.md (branch claude/project-thread-
bz5k5e), which lists S+x for every DB word; each offset there was matched against the writes in
the ctor/init functions below (no disagreement found; fields that the mapping does not mention
keep u<off> names).

  ctor     1401e7b10 (constructs the DYNARYs and the furnace block, owner = 0)
  init     1401e7c50 (-> 1401e7e90, 1401e8490, 1401e8650, 1401e8fb0, 1401e96b0, 1401ea6b0,
           1401ead60); DB_STEP_DATA's ctor calls 1401e7c10(this, step) = set owner + init
  clear    1401e7da0 (frees the DYNARYs' arrays, zeroes frqncy_rows/cols)
  dtor     1401e7cb0 (1401e7da0, then DYNARY and furnace dtors in reverse order)
  size     0xbd0: 1401e8fb0 writes 8 bytes at 0xbc8 (ODMAX), and DB_STEP_DATA's next field is at
           0x1320 = 0x750 + 0xbd0.
  open     8-byte fields only ever zeroed and not in the mapping are typed uint64_t; 0x4b0..0x4b7
           is never written (BLZMAN at 0x4b8 is a sim-ctrl field per the mapping, so the
           furnace block is at most 0x78 bytes); DTTYPE's mode (S+0x240) is DYNARY dttype.u58.
```

| @src | size (bytes) | role | declaration |
|---|---|---|---|
| 1401e7b10 | 241 | ctor | `Cls_1401e7b10();` |
| 1401e7cb0 | 231 | dtor | `~Cls_1401e7b10();` |

| offset | type | member | evidence / meaning |
|---|---|---|---|
| 0x000 | `char[0x51]` | title | TITLE; 1401e7e90 strcpy "SIMULATION" |
| 0x051 | `char[0x41]` | oprnam | OPRNAM; "OPERATION 1" |
| 0x092 | `char[0x51]` | simnam | SIMNAM; "SIMULATION 1" (0xe3 is padding) |
| 0x0e4 | `std::int32_t[3]` | cursim | CURSIM v1..v3; init 1, 0, 0 |
| 0x0f0 | `std::int32_t` | meshno | MESHNO =1 |
| 0x0f4 | `std::int32_t` | smode | SMODE =1 |
| 0x0f8 | `std::int32_t` | stype | STYPE =1 |
| 0x0fc | `std::int32_t` | unit | UNIT (==2 selects the unit constants in 1401ea6b0) |
| 0x100 | `std::int32_t` | geotyp | GEOTYP (2D) =1 |
| 0x104 | `std::int32_t[5]` | trans | TRANS 1..5 (init loop zeroes 6 ints from 0x104; trans[1]=1) |
| 0x118 | `std::int32_t` | htmthd | HTMTHD (6th int of that loop) |
| 0x11c | `std::int32_t[7]` | stnout | STNOUT 1..7 (stnout[4] at 0x12c = 1) |
| 0x138 | `std::int32_t[10]` | elmnod | ELMNOD 1..10, zeroed loop |
| 0x160 | `std::int32_t` | nstart | NSTART; 1401e8490 =-1 |
| 0x164 | `std::int32_t` | stpinc | STPINC =10 |
| 0x168 | `std::int32_t` | pdie1 | PDIE 1 =0 |
| 0x16c | `std::int32_t` | nstep | NSTEP =100 then =1 |
| 0x170 | `double` | emax | EMAX; 1401e8fb0 =0 |
| 0x178 | `double` | tmax | TMAX |
| 0x180 | `double[3]` | smax | SMAX 1..3 (3D) |
| 0x198 | `double[3]` | vmin | VMIN 1..3 (3D) |
| 0x1b0 | `double[3]` | lmax | LMAX 1..3 (3D) |
| 0x1c8 | `std::int32_t` | stpdef | STPDEF; 1401e8650 =1 |
| 0x1cc | `std::byte[4]` | pad_1cc |  |
| 0x1d0 | `double` | dtmax1 | DTMAX 1 |
| 0x1d8 | `double` | dtmax2 | DTMAX 2 |
| 0x1e0 | `double` | dsmax | DSMAX; setter 1401f4390 |
| 0x1e8 | `DYNARY` | dttype | DTTYPE function, type 0x80; mode at +0x58 (S+0x240) |
| 0x260 | `double` | demax | DEMAX; _DAT_14051bef8 in 2D, else 0 |
| 0x268 | `double` | slderr | SLDERR |
| 0x270 | `double` | dtpmax1 | DTPMAX 1 |
| 0x278 | `double[2]` | dvmax | DVMAX 1, 2 |
| 0x288 | `std::int32_t[3]` | mdsobj | MDSOBJ 1..3 (int); 1401e8fb0 |
| 0x294 | `std::byte[4]` | pad_294 |  |
| 0x298 | `double` | mdsobj4 | MDSOBJ 4 |
| 0x2a0 | `double[4]` | dtpmax2_5 | DTPMAX 2..5 (4, 5 2D only) |
| 0x2c0 | `double` | dtsub | DTSUB; _DAT_140509c90 in 3D, else 0 |
| 0x2c8 | `double` | dplen | DPLEN (3D); DAT_14051f240 |
| 0x2d0 | `double[2]` | cvgerr | CVGERR 1, 2; 1401e96b0 2D/3D constants |
| 0x2e0 | `std::int32_t` | itrmth | ITRMTH; 1 (2D) / 2 (3D) |
| 0x2e4 | `std::int32_t` | solmtd | SOLMTD =1 |
| 0x2e8 | `std::int32_t` | u2e8 | =0 (not in the mapping) |
| 0x2ec | `std::byte[4]` | pad_2ec |  |
| 0x2f0 | `double[4]` | solmtd_p | SOLMTD parameters |
| 0x310 | `std::int32_t` | itrmxd | ITRMXD =200 |
| 0x314 | `std::int32_t` | defbwd | DEFBWD =1 |
| 0x318 | `std::int32_t` | solmtt | SOLMTT; 1 (2D) / 2 (3D) |
| 0x31c | `std::int32_t` | itrmxt | ITRMXT =200 |
| 0x320 | `std::int32_t` | tmpbwd | TMPBWD =1 |
| 0x324 | `std::int32_t` | latent | LATENT =0 |
| 0x328 | `std::int32_t` | frqncy_rows | FRQNCY rows; 1401e9850 =6 |
| 0x32c | `std::int32_t` | frqncy_cols | FRQNCY cols; 1401e9850 =1 |
| 0x330 | `DYNARY` | frqncy | FRQNCY table, type 0xb5, 6x1 values (1401e9850) |
| 0x3a8 | `double` | peninf | PENINF; DAT_140522f50 |
| 0x3b0 | `double` | unte2h | UNTE2H; unit-dependent constant |
| 0x3b8 | `double` | tintgf | TINTGF; DAT_14052e8d0 |
| 0x3c0 | `std::uint8_t` | furnace_on | ENVTMP furnace flag =0 (FRNNAM written only when set) |
| 0x3c1 | `std::byte[7]` | pad_3c1 |  |
| 0x3c8 | `DYNARY` | envtmp | ENVTMP (const at 0x3d0 set by 1401ea6b0) |
| 0x440 | `Cls_1403753f0` | furnace | FRNNAM furnace model (140375600(.., 0x16), 140375e90) |
| 0x4b0 | `std::byte[8]` | pad_4b0 | never written |
| 0x4b8 | `double` | blzman | BLZMAN; unit-dependent constant |
| 0x4c0 | `DYNARY` | cnvcof | CNVCOF (const at 0x4c8) |
| 0x538 | `DYNARY` | envatm | ENVATM (const at 0x540 = DAT_14052e8a0) |
| 0x5b0 | `DYNARY` | acvcof | ACVCOF (const at 0x5b8) |
| 0x628 | `double` | envmpr | ENVMPR; unit-dependent constant |
| 0x630 | `double` | envmpt | ENVMPT |
| 0x638 | `double` | ehrate | EHRATE =0 |
| 0x640 | `double` | tnow1 | TNOW 1 (global time); 1401ead60 =0 |
| 0x648 | `double` | tnow2 | TNOW 2 |
| 0x650 | `std::int32_t` | tnow3 | TNOW 3 (int) |
| 0x654 | `std::byte[4]` | pad_654 |  |
| 0x658 | `double` | tnow4 | TNOW 4 |
| 0x660 | `double[4]` | geoerr | GEOERR 1..4 (DAT_14051f1b8, DAT_14051f250, 0, 0) |
| 0x680 | `double` | u680 | DAT_140524cd8 in 2D (left as is in 3D); not in the mapping |
| 0x688 | `char[10][0x51]` | usrdef | USRDEF 1..10, each strcpy DAT_14052c7da |
| 0x9b4 | `std::int32_t` | pdie2 | PDIE 2 (0x9b2..0x9b3 padding) |
| 0x9b8 | `std::int32_t[4]` | osctrl | OSCTRL 1..4 (2D); zeroed in 2D init |
| 0x9c8 | `std::int32_t` | cnterr1 | CNTERR 1 (int, 2D) |
| 0x9cc | `std::byte[4]` | pad_9cc |  |
| 0x9d0 | `double` | cnterr2 | CNTERR 2 (2D) |
| 0x9d8 | `std::int32_t` | vfactr_mode | VFACTR mode |
| 0x9dc | `std::int32_t` | vfactr_n | VFACTR count |
| 0x9e0 | `DYNARY` | vfactr | VFACTR list, type 0xb5 (init in 3D only) |
| 0xa58 | `DYNARY` | timeid | TIMEID, type 0xb0 |
| 0xad0 | `DYNARY` | erecid | ERECID, type 0xb1 |
| 0xb48 | `DYNARY` | nrecid | NRECID, type 0xb2 |
| 0xbc0 | `DB_STEP_DATA*` | owner | set by 1401e7c10; 2D/3D tests read it (1400015e0/1400015f0) |
| 0xbc8 | `double` | odmax | ODMAX (3D); 1401e8fb0 =0 |


## DYNARY

Header `port/defmodel/include/defmodel/layout/dynary.hpp`, stubs `port/defmodel/src/layout/dynary.cpp`.

```
DYNARY (no RTTI, not polymorphic; ctor 14012d050): a tabulated property function (constant,
or a table over up to 3 independent variables with ncomp values per point). Used for most
material, phase-pair, simulation-control and inter-object properties.
The name follows docs/re/keywords/_common_g457.md section 4. That document puts it in the
"RTTI family DYNARY_DATA", but it is not that class: RTTI DYNARY_DATA (dynary_data.hpp) is a
polymorphic 0x40-byte SfLinkedData node (operator new(0x40) in 14025ba40). This class has no
vptr: its ctor writes the int type at offset 0.

  ctor     14012d050 (zero, then 14012d2a0(this, 0))
  init     14012d2a0(this, type): free (14012f480), set type/shape, per-type defaults, then
           allocate (14012fad0) unless type is 0x51/0x65/0x66/0x67. 14012fc70 is the same but
           keeps +0x54 and +0x60 (spec).
  dtor     14012f410 (14012f480 frees 0x28/0x30/0x38/0x48 unless shared, then deletes the
           object at 0x68 through its scalar deleting dtor 14013c170(p, 1))
  size     0x78: back-to-back members in Cls_1401e7b10 (0x9e0, 0xa58, 0xad0, 0xb48), Material
           (0x670, 0x6e8, 0x760) and PhasePair (0x478, 0x4f0, 0x568); highest write is the
           8-byte field at 0x68.
  open     0x5c and 0x70 are never written by ctor/init/dtor. 0x08 is a double (case 0xae
           stores 1.0 there), but cases 0xaf/0xb3/0xb6 clear it as two ints.
```

| @src | size (bytes) | role | declaration |
|---|---|---|---|
| 14012d050 | 174 | ctor | `DYNARY();` |
| 14012f410 | 99 | dtor | `~DYNARY();` |

| offset | type | member | evidence / meaning |
|---|---|---|---|
| 0x00 | `std::int32_t` | type | property id (14012d2a0 param_2) |
| 0x04 | `std::int32_t` | ndim | 0 = constant, 1..3 = table dimensions |
| 0x08 | `double` | value | constant value (ndim 0) |
| 0x10 | `std::int32_t[3]` | var | independent-variable code per dimension (3 = temperature, |
| 0x1c | `std::int32_t[3]` | n | points per dimension (14012f480 tests > 0 before freeing) |
| 0x28 | `double*[3]` | axis | abscissae per dimension (calloc in 14012fad0) |
| 0x40 | `std::int32_t` | ncomp | values per table point (default 1) |
| 0x44 | `std::byte[4]` | pad_44 |  |
| 0x48 | `double*` | values | ncomp * prod(n) doubles (14012ff20 gives the count) |
| 0x50 | `std::int32_t` | u50 | cleared on init; per-keyword flag (FSTRES table flag) |
| 0x54 | `std::int32_t` | u54 | kept by 14012fc70 |
| 0x58 | `std::int32_t` | u58 | cleared on init |
| 0x5c | `std::byte[4]` | pad_5c | never written in ctor/init |
| 0x60 | `std::int32_t` | u60 | init [0x18]=0; 0x60..0x67 kept by 14012fc70 |
| 0x64 | `std::int32_t` | u64 | init [0x19]=0 |
| 0x68 | `void*` | u68 | owned object, deleted by 14012f410 via 14013c170(p, 1) |
| 0x70 | `std::byte[8]` | pad_70 | never written in ctor/init/dtor |


## Cls_1403753f0

Header `port/defmodel/include/defmodel/layout/cls_1403753f0.hpp`, stubs `port/defmodel/src/layout/cls_1403753f0.cpp`.

```
Cls_1403753f0 (no RTTI, not polymorphic): "Furnace model" block of the simulation control
(Cls_1401e7b10+0x440): a name and a list of curves (140375600 appends 21+ curves of types
0x8e..0xa3 to the list).

  ctor     1403753f0 (list ctor at 0x40, then 140375470: zero bytes 0x58..0x6f,
           sprintf(this, "Furnace model"))
  dtor     140375430 (1403754e0 -> 140375520 clears the list, then 140136510 list dtor)
  size     0x70 (lower bound, taken as the size): 140375470 writes bytes up to 0x6f; the owner
           writes an 8-byte value at its own offset 0x4b8 = 0x440 + 0x78, and its next member
           (a curve) starts at 0x4c0 = 0x440 + 0x80.
  open     the real size is 0x70, 0x78 or 0x80: if it is larger, Cls_1401e7b10::u4b0/u4b8
           belong to this class. Byte offsets are unaffected.
```

| @src | size (bytes) | role | declaration |
|---|---|---|---|
| 1403753f0 | 54 | ctor | `Cls_1403753f0();` |
| 140375430 | 48 | dtor | `~Cls_1403753f0();` |

| offset | type | member | evidence / meaning |
|---|---|---|---|
| 0x00 | `char[0x40]` | name | sprintf "Furnace model" (140375470) |
| 0x40 | `Cls_1401364a0` | curves | curve list (140375600, 140136670) |
| 0x58 | `std::uint8_t[0x18]` | u58 | each byte zeroed by 140375470 (flags?) |


## Cls_1401364a0

Header `port/defmodel/include/defmodel/layout/cls_1401364a0.hpp`, stubs `port/defmodel/src/layout/cls_1401364a0.cpp`.

```
Cls_1401364a0 (no RTTI, not polymorphic): intrusive singly linked list head used by the
furnace model (Cls_1403753f0+0x40). Nodes link through node+0x60 and are deleted with their
scalar deleting dtor 14013c170(node, 1) (the same deleter DYNARY uses for its 0x68 object).

  ctor     1401364a0 (-> 1401364d0: head = tail = 0, count = 0)
  dtor     140136510 (-> 140136910: delete every node, reset)
  add      140136670 (appends a node, returns it; used by 140375600)
  size     0x18: three fields written by 1401364d0; the furnace class zeroes its own bytes from
           0x58 = 0x40 + 0x18 on (140375470).
```

| @src | size (bytes) | role | declaration |
|---|---|---|---|
| 1401364a0 | 29 | ctor | `Cls_1401364a0();` |
| 140136510 | 24 | dtor | `~Cls_1401364a0();` |

| offset | type | member | evidence / meaning |
|---|---|---|---|
| 0x00 | `void*` | head | first node (next pointer at node+0x60) |
| 0x08 | `void*` | tail | last node |
| 0x10 | `std::int32_t` | count | number of nodes |
| 0x14 | `std::byte[4]` | pad_14 |  |


## MaterialList

Header `port/defmodel/include/defmodel/layout/material_list.hpp`, stubs `port/defmodel/src/layout/material_list.cpp`.

```
MaterialList (no RTTI, not polymorphic; ctor 14029b4c0): singly linked list of Material owned
by DB_STEP_DATA (+0x1328), with two number<->index arrays that are built only while the
material set changes (_common_g457.md section 5).

  ctor     14029b4c0(this, owner) (owner, then 14029b500: head = tail = 0, count = 0,
           arrays 0x18/0x20 = 0)
  reset    14029b500
  clear    14029c310 (deletes every node through 14029e210(node, 1), then resets)
  dtor     14029b550 (14029c310, then free numbers / index_of)
  add      14029c060 (operator new(0x2218), 14028c6f0(node, owner); links via node+0x2200)
  find     14029c2c0(list, number): walk until node->number == number
  nth      14029c260(list, i) (1-based, used by 14029ca10); count 14029cd50 = +0x10
  rebuild  14029ca10(list, free_only): frees both arrays; if free_only == 0, callocs count
           ints for each and fills numbers[i] = nth(i+1)->number, index_of[i] =
           14029c870(list, number) (helper not decompiled; the spec calls the pair
           "number->index arrays")
  size     0x30: owner at +0x28 (ctor, 1401d8300); DB_STEP_DATA's next member is at
           0x1358 = 0x1328 + 0x30.
```

| @src | size (bytes) | role | declaration |
|---|---|---|---|
| 14029b4c0 | 48 | ctor (DB_STEP_DATA passes owner = 0, then sets it with 1401d8300) | `explicit MaterialList(DB_STEP_DATA* owner = nullptr);` |
| 14029b550 | 102 | dtor | `~MaterialList();` |

| offset | type | member | evidence / meaning |
|---|---|---|---|
| 0x00 | `Material*` | head | first node |
| 0x08 | `Material*` | tail | last node (new nodes are linked at tail+0x2200) |
| 0x10 | `std::int32_t` | count | number of nodes (14029cd50) |
| 0x14 | `std::byte[4]` | pad_14 |  |
| 0x18 | `std::int32_t*` | numbers | material number per index (14029ca10) |
| 0x20 | `std::int32_t*` | index_of | 14029c870(list, number) per index (14029ca10) |
| 0x28 | `DB_STEP_DATA*` | owner | ctor / 1401d8300; passed to each new node |


## Material

Header `port/defmodel/include/defmodel/layout/material.hpp`, stubs `port/defmodel/src/layout/material.cpp`.

```
Material (no RTTI, not polymorphic; ctor 14028c6f0): one material (keyword group 4), node of
MaterialList at DB_STEP_DATA+0x1328. Members are named after the keyword that reads and
writes them (docs/re/keywords/<KW>.md, _common_g457.md section 5). Every offset below was
checked against the ctor/init/dtor code; the keyword names come from those specs.
DBREAD 14028f300, GENDB 14029bdf0, KEY writer 1403cdca0 (dbmap brief); group-4 reader
140384a50 (spec).

  alloc    14029c060: operator new(0x2218), 14028c6f0(node, list->owner)
  ctor     14028c6f0(this, owner) (sub-object and DYNARY ctors, owner, next = 0, 14028cdc0)
  init     14028cdc0 (number = 0, name "", defaults, DYNARY types, then 14028d030)
  dtor     14028cb70 (free mstmtr_list, member dtors in reverse order); the list deletes
           nodes through 14029e210 (scalar deleting dtor, called directly)
  size     0x2218 (operator new size in 14029c060); the skip byte at 0x2210 is the last field.
  open     0x004..0x00b and 0x1ed0 are not used by any spec; the DYNARY at 0x2058 has no
           keyword in groups 4/5/7; 0x2211..0x2217 is padding.
```

| @src | size (bytes) | role | declaration |
|---|---|---|---|
| 14028c6f0 | 578 | ctor | `explicit Material(DB_STEP_DATA* owner);` |
| 14028cb70 | 571 | dtor | `~Material();` |

| offset | type | member | evidence / meaning |
|---|---|---|---|
| 0x0000 | `std::int32_t` | number | material number (key of 14029c2c0); 14028cdc0 =0 |
| 0x0004 | `std::byte[8]` | pad_0004 | not written |
| 0x000c | `char[0x41]` | name | MTNAME (FUN_140413ab0(fp, +0xc, 0x41)); init strcpy "" |
| 0x004d | `std::byte[3]` | pad_004d |  |
| 0x0050 | `Cls_140288ae0` | fstres | FSTRES block |
| 0x0668 | `std::int32_t` | hdnrul | HDNRUL; init =0 |
| 0x066c | `std::byte[4]` | pad_066c |  |
| 0x0670 | `DYNARY` | young | YOUNG |
| 0x06e8 | `DYNARY` | poison | POISON |
| 0x0760 | `DYNARY` | expand | EXPAND |
| 0x07d8 | `double` | expand_ref | EXPAND extra (reference temperature); init =0 |
| 0x07e0 | `DYNARY` | thrcnd | THRCND |
| 0x0858 | `DYNARY` | heatcp | HEATCP |
| 0x08d0 | `std::int32_t` | heatcp_extra | HEATCP extra, stored as (int)tmp; init =0 |
| 0x08d4 | `std::byte[4]` | pad_08d4 |  |
| 0x08d8 | `DYNARY` | emsvty | EMSVTY |
| 0x0950 | `Cls_140289930` | difcoe | DIFCOE block |
| 0x0ac8 | `Cls_140289b80` | creep | CREEP block |
| 0x0f10 | `DYNARY` | hdnpha | HDNPHA |
| 0x0f88 | `DYNARY` | hdntim | HDNTIM; init type 0x53 |
| 0x1000 | `DYNARY` | jominy | JOMINY; init type 0x52 |
| 0x1078 | `Cls_14028b850` | grndat | GRNDAT block |
| 0x17f0 | `DYNARY` | gbengy | GBENGY; init type 0x5d |
| 0x1868 | `double` | gbmobi_p | GBMOBI p; init =0 |
| 0x1870 | `double` | gbmobi_q | GBMOBI q; init =0 |
| 0x1878 | `DYNARY` | nucsiz | NUCSIZ; init type 0xb6 |
| 0x18f0 | `Cls_14028bcc0` | disloc | BURGRS / ALPHA / NDISFM / RECVRY |
| 0x1ad0 | `Cls_14028c040` | frcmod | FRCMOD / UTSDAT block |
| 0x1cc8 | `std::int32_t` | mstmtr_flag | MSTMTR: 1 = mixture material; init =0 |
| 0x1ccc | `std::int32_t` | mstmtr_count | MSTMTR n; init =0 |
| 0x1cd0 | `std::int32_t*` | mstmtr_list | MSTMTR entries (calloc(n, 4)); freed by the dtor |
| 0x1cd8 | `DYNARY` | elrst | ELRST |
| 0x1d50 | `DYNARY` | pmeab | PMEAB |
| 0x1dc8 | `DYNARY` | pmitt | PMITT |
| 0x1e40 | `Cls_140289fe0` | aniso | ANISO block |
| 0x1ec8 | `double` | frae2h | FRAE2H; init DAT_14051f220 (0.9 per spec) |
| 0x1ed0 | `std::uint64_t` | u1ed0 | init =0; no keyword |
| 0x1ed8 | `double` | tmpsol | TMPSOL; init =0 |
| 0x1ee0 | `double` | tmpliq | TMPLIQ; init =0 |
| 0x1ee8 | `double` | matden | MATDEN (3D only on write); init =0 |
| 0x1ef0 | `DYNARY` | fperv1 | FPERV first table; init type 0 |
| 0x1f68 | `DYNARY` | fperv2 | FPERV second table; init type 0 |
| 0x1fe0 | `DYNARY` | masden | MASDEN |
| 0x2058 | `DYNARY` | u2058 | no keyword found |
| 0x20d0 | `std::int32_t` | coarse_flag | COARSE flag; init =0 |
| 0x20d4 | `std::byte[4]` | pad_20d4 |  |
| 0x20d8 | `DYNARY` | coarse | COARSE |
| 0x2150 | `std::int32_t` | sizemd | SIZEMD; init =0 |
| 0x2154 | `Cls_14028c500` | txture | TXTURE a, b, c |
| 0x2160 | `DYNARY` | difbnd | DIFBND; init type 0xaf |
| 0x21d8 | `std::int32_t` | sinter_model | SINTER model; init =0 |
| 0x21dc | `std::byte[4]` | pad_21dc |  |
| 0x21e0 | `double[4]` | sinter_p | SINTER p1..p4; init =0 |
| 0x2200 | `Material*` | next | list link |
| 0x2208 | `DB_STEP_DATA*` | owner | model back-pointer (ctor param; read by 14028cdc0) |
| 0x2210 | `std::uint8_t` | skip_key | 1 = skip in KEY output (tested by 1403d4970); init =0 |
| 0x2211 | `std::byte[7]` | pad_2211 |  |


## Cls_140288ae0

Header `port/defmodel/include/defmodel/layout/cls_140288ae0.hpp`, stubs `port/defmodel/src/layout/cls_140288ae0.cpp`.

```
Cls_140288ae0 (no RTTI, not polymorphic): flow-stress block of a Material (Material+0x50), the
data of keyword FSTRES (docs/re/keywords/FSTRES.md; offsets there are absolute material
offsets = this + 0x50).

  ctor     140288ae0 (10 DYNARYs, model = 1, zeroes, DYNARY types)
  dtor     14029cdc0 (DYNARY dtors in reverse order)
  size     0x618: Material's next field (HDNRUL int) is at 0x668 = 0x50 + 0x618; the last write
           is the int at 0x610 (param_1[0x184]).
  open     meaning of the coefficient sets for FSTRES types 5, 9, 10, 11 (spec: "unknown").
```

| @src | size (bytes) | role | declaration |
|---|---|---|---|
| 140288ae0 | 1169 | ctor | `Cls_140288ae0();` |
| 14029cdc0 | 199 | dtor | `~Cls_140288ae0();` |

| offset | type | member | evidence / meaning |
|---|---|---|---|
| 0x000 | `std::int32_t` | model | =1; FSTRES model code M (KEY type -> M per spec) |
| 0x004 | `std::byte[4]` | pad_004 |  |
| 0x008 | `double[4]` | c_t1 | =0; FSTRES t1 coefficients (mat+0x58..0x70) |
| 0x028 | `DYNARY` | table | type 0x4f; FSTRES t2/t3 table (mat+0x78), flag in u50 |
| 0x0a0 | `double[5]` | c_t4 | =0; FSTRES t4 (mat+0xf0..0x110) |
| 0x0c8 | `double[4]` | c_t5 | =0; FSTRES t5 (mat+0x118..0x130) |
| 0x0e8 | `DYNARY` | h | type 0x25; FSTRES t6 "Coefficient (H)" (mat+0x138) |
| 0x160 | `DYNARY` | y | type 0x24; FSTRES t6 "Coefficient (Y)" (mat+0x1b0) |
| 0x1d8 | `DYNARY` | s | type 0x50; FSTRES t7/t8 "Strain-Atm-Temp" (mat+0x228) |
| 0x250 | `double[14]` | c_t9 | =0; FSTRES t9 (mat+0x2a0..0x308) |
| 0x2c0 | `double[6]` | c_t10 | =0; FSTRES t10 (mat+0x310..0x338) |
| 0x2f0 | `double[5]` | c_t11 | =0; FSTRES t11 (mat+0x340..0x360) |
| 0x318 | `double[4]` | c_t14 | =0; FSTRES t14 (mat+0x368..0x380) |
| 0x338 | `DYNARY` | bmd_n | type 0xae; FSTRES t14 "Bird-Mukherjee-Dorn n" (mat+0x388) |
| 0x3b0 | `DYNARY` | f | type 0x26; FSTRES t12 "Initial Flow Stress" (mat+0x400) |
| 0x428 | `std::int32_t` | cnt | =0; FSTRES t13 count (mat+0x478) |
| 0x42c | `std::byte[4]` | pad_42c |  |
| 0x430 | `DYNARY` | a | type 0xa6; FSTRES t13 A (mat+0x480) |
| 0x4a8 | `DYNARY` | b | type 0xa7; FSTRES t13 B (mat+0x4f8) |
| 0x520 | `DYNARY` | c | type 0xa8; FSTRES t13 C (mat+0x570) |
| 0x598 | `DYNARY` | p | type 0x51; FSTRES t16 "Strain-StrainRate-Temp" (mat+0x5e8) |
| 0x610 | `std::int32_t` | user_routine | =0; FSTRES t<0 routine number (mat+0x660) |
| 0x614 | `std::byte[4]` | pad_614 |  |


## Cls_140289930

Header `port/defmodel/include/defmodel/layout/cls_140289930.hpp`, stubs `port/defmodel/src/layout/cls_140289930.cpp`.

```
Cls_140289930 (no RTTI, not polymorphic): diffusion-coefficient block of a Material
(Material+0x950), data of keyword DIFCOE (docs/re/keywords/DIFCOE.md).

  ctor     140289930 (3 DYNARYs, mode = 0, types 0x29/0x27/0x28, routine = 0)
  dtor     14029ce90
  size     0x178: last write is the int at 0x170 (param_1[0x5c]); Material's next member is at
           0xac8 = 0x950 + 0x178.
```

| @src | size (bytes) | role | declaration |
|---|---|---|---|
| 140289930 | 182 | ctor | `Cls_140289930();` |
| 14029ce90 | 73 | dtor | `~Cls_140289930();` |

| offset | type | member | evidence / meaning |
|---|---|---|---|
| 0x000 | `std::int32_t` | mode | =0; DIFCOE mode 0..4 (mat+0x950) |
| 0x004 | `std::byte[4]` | pad_004 |  |
| 0x008 | `DYNARY` | d0 | type 0x29; DIFCOE t0 constant / t1 table (mat+0x958) |
| 0x080 | `DYNARY` | d2 | type 0x27; DIFCOE t2 (mat+0x9d0) |
| 0x0f8 | `DYNARY` | d3 | type 0x28; DIFCOE t3 (mat+0xa48) |
| 0x170 | `std::int32_t` | user_routine | =0; DIFCOE -t (mat+0xac0) |
| 0x174 | `std::byte[4]` | pad_174 |  |


## Cls_140289b80

Header `port/defmodel/include/defmodel/layout/cls_140289b80.hpp`, stubs `port/defmodel/src/layout/cls_140289b80.cpp`.

```
Cls_140289b80 (no RTTI, not polymorphic): creep block of a Material (Material+0xac8), data of
keyword CREEP (docs/re/keywords/CREEP.md).

  ctor     140289b80 (9 DYNARYs, model = 0, types, routine = 0, cnt = -1)
  dtor     14029cee0
  size     0x448: the last DYNARY starts at 0x3d0 (param_1[0xf4]); Material's next member is at
           0xf10 = 0xac8 + 0x448.
```

| @src | size (bytes) | role | declaration |
|---|---|---|---|
| 140289b80 | 425 | ctor | `Cls_140289b80();` |
| 14029cee0 | 181 | dtor | `~Cls_140289b80();` |

| offset | type | member | evidence / meaning |
|---|---|---|---|
| 0x000 | `std::int32_t` | model | =0; CREEP model M (mat+0xac8) |
| 0x004 | `std::byte[4]` | pad_004 |  |
| 0x008 | `DYNARY` | d6 | type 0x43; "Creep - TableData" (mat+0xad0) |
| 0x080 | `DYNARY` | d5 | type 0x44; "Creep - TableDataStr" (mat+0xb48) |
| 0x0f8 | `DYNARY` | d1 | type 0x45; "Creep - Prezyna" (mat+0xbc0) |
| 0x170 | `DYNARY` | d2 | type 0x46; "Creep - Power Law" (mat+0xc38) |
| 0x1e8 | `DYNARY` | d3 | type 0x47; "Creep - GE" (mat+0xcb0) |
| 0x260 | `DYNARY` | d4 | type 0x48; "Creep - WG" (mat+0xd28) |
| 0x2d8 | `std::int32_t` | user_routine | =0; CREEP -t (mat+0xda0) |
| 0x2dc | `std::int32_t` | cnt | =-1; CREEP t7/t8 count (mat+0xda4) |
| 0x2e0 | `DYNARY` | a | type 0xa9 (mat+0xda8) |
| 0x358 | `DYNARY` | b | type 0xaa (mat+0xe20) |
| 0x3d0 | `DYNARY` | c | type 0xab (mat+0xe98) |


## Cls_14028a140, Cls_14028b640, Cls_14028b850

Header `port/defmodel/include/defmodel/layout/cls_14028b850.hpp`, stubs `port/defmodel/src/layout/cls_14028b850.cpp`.

```
Cls_14028b850 (no RTTI, not polymorphic): grain/recrystallization block of a Material
(Material+0x1078), data of keyword GRNDAT (docs/re/keywords/GRNDAT.md, "Model mapping").
Holds three parts with their own ctors, defined here as nested classes:
  Cls_14028a140 (+0x000, 0x458)  type-1 data: 4 flags, 9 DYNARYs, g2, g1
  Cls_14028b640 (+0x458, 0x270)  type-2 data: c, main record, 5 record lists
  +0x6c8 DYNARY built by 14028a3e0 (type 0xac), +0x740 double[6] built by 14028a440

  ctor     14028b850 (14028a140, 14028b640(+0x458), 14028a3e0(+0x6c8), 14028a440(+0x740),
           type = 0, ref = 0)
  dtor     14028b8e0 (14028a4b0(+0x740) empty, 14028a420(+0x6c8) = DYNARY dtor,
           14028b6f0(+0x458), 14028a320(+0))
  size     0x778: last write is the int at 0x774; Material's next member is at
           0x17f0 = 0x1078 + 0x778.
```

### Cls_14028a140

| @src | size (bytes) | role | declaration |
|---|---|---|---|
| 14028a140 | 464 | ctor (flags = 0, DYNARY types 0x55, 0x54, 0x56..0x5c, g = 0) | `Cls_14028a140();` |
| 14028a320 | 181 | dtor | `~Cls_14028a140();` |

| offset | type | member | evidence / meaning |
|---|---|---|---|
| 0x000 | `std::int32_t` | flag_d3 | (mat+0x1078) spec: DRX on (inferred) |
| 0x004 | `std::int32_t` | flag_d5 | (mat+0x107c) spec: MDRX on (inferred) |
| 0x008 | `std::int32_t` | flag_d7 | (mat+0x1080) spec: SRX on (inferred) |
| 0x00c | `std::int32_t` | flag_d9 | (mat+0x1084) spec: grain growth on (inferred) |
| 0x010 | `DYNARY[9]` | d | D1..D9, types 0x55,0x54,0x56,0x57,0x58,0x59,0x5a,0x5b,0x5c |
| 0x448 | `double` | g2 | (mat+0x14c0) =0 |
| 0x450 | `double` | g1 | (mat+0x14c8) =0 |

### Cls_14028b640

| @src | size (bytes) | role | declaration |
|---|---|---|---|
| 14028b640 | 169 | ctor (record ctor, 5 list ctors (FID-labelled COleControlSiteOrWnd::COleControlSiteOrWnd), c = 0, record.is_main = 1) | `Cls_14028b640();` |
| 14028b6f0 | 127 | dtor (14028af40 on each list, 14028a530 on the record) | `~Cls_14028b640();` |

| offset | type | member | evidence / meaning |
|---|---|---|---|
| 0x000 | `std::int32_t` | c | (mat+0x14d0) GRNDAT 3rd int, type 2 |
| 0x004 | `std::byte[4]` | pad_004 |  |
| 0x008 | `Cls_14028a4c0` | main | (mat+0x14d8) main record |
| 0x1f8 | `RecordList` | l3 | (mat+0x16c8) spec: L3 |
| 0x210 | `RecordList` | l2 | (mat+0x16e0) L2 |
| 0x228 | `RecordList` | l1 | (mat+0x16f8) L1 (first in the file) |
| 0x240 | `RecordList` | l4 | (mat+0x1710) L4 |
| 0x258 | `RecordList` | l5 | (mat+0x1728) L5 |

### Cls_14028b850

| @src | size (bytes) | role | declaration |
|---|---|---|---|
| 14028b850 | 123 | ctor | `Cls_14028b850();` |
| 14028b8e0 | 87 | dtor | `~Cls_14028b850();` |

| offset | type | member | evidence / meaning |
|---|---|---|---|
| 0x000 | `Cls_14028a140` | type1 | (mat+0x1078) |
| 0x458 | `Cls_14028b640` | type2 | (mat+0x14d0) |
| 0x6c8 | `DYNARY` | table4 | (mat+0x1740) 14028a3e0: type 0xac; GRNDAT type 4 table |
| 0x740 | `double[6]` | p | (mat+0x17b8) 14028a440 zeroes; GRNDAT type 5 p1..p6 |
| 0x770 | `std::int32_t` | type | (mat+0x17e8) =0; GRNDAT type |
| 0x774 | `std::int32_t` | ref | (mat+0x17ec) =0; -type for type < 0 |


## Cls_14028a4c0

Header `port/defmodel/include/defmodel/layout/cls_14028a4c0.hpp`, stubs `port/defmodel/src/layout/cls_14028a4c0.cpp`.

```
Cls_14028a4c0 (no RTTI, not polymorphic): GRNDAT type-2 record (docs/re/keywords/GRNDAT.md).
One "main" record is embedded in Cls_14028b640 at +8; list records hang off its five lists.

  ctor     14028a4c0 (3 DYNARYs, next = 0, then 14028a580 init)
  init     14028a580 (DYNARY types 0x5e/0x5f/0x60, n = 12 via 14029ccd0, values = 0,
           0x68 = 0, 0x70 = 0, byte 0x1e0 = 0)
  dtor     14028a530
  size     0x1f0: ctor writes next at 0x1e8; Cls_14028b640's lists start at 0x1f8 = 8 + 0x1f0.
```

| @src | size (bytes) | role | declaration |
|---|---|---|---|
| 14028a4c0 | 106 | ctor | `Cls_14028a4c0();` |
| 14028a530 | 73 | dtor | `~Cls_14028a4c0();` |

| offset | type | member | evidence / meaning |
|---|---|---|---|
| 0x000 | `std::int32_t` | n | number of values (setter 14029ccd0, getter 14029cd20) |
| 0x004 | `std::byte[4]` | pad_004 |  |
| 0x008 | `double[12]` | values | zeroed for i < n (14028a580) |
| 0x068 | `double` | d | =0 |
| 0x070 | `std::int32_t` | i | =0 |
| 0x074 | `std::byte[4]` | pad_074 |  |
| 0x078 | `DYNARY` | sub1 | type 0x5e |
| 0x0f0 | `DYNARY` | sub2 | type 0x5f |
| 0x168 | `DYNARY` | sub3 | type 0x60 |
| 0x1e0 | `std::uint8_t` | is_main | 1 = main record (14029cda0 from 14028b640) |
| 0x1e1 | `std::byte[7]` | pad_1e1 |  |
| 0x1e8 | `Cls_14028a4c0*` | next | =0 |


## Cls_14028bcc0

Header `port/defmodel/include/defmodel/layout/cls_14028bcc0.hpp`, stubs `port/defmodel/src/layout/cls_14028bcc0.cpp`.

```
Cls_14028bcc0 (no RTTI, not polymorphic): four DYNARYs at Material+0x18f0, the data of
keywords BURGRS, ALPHA, NDISFM and RECVRY (docs/re/keywords/<KW>.md).

  ctor     14028bcc0 (4 x DYNARY ctor, all type 0)
  dtor     14028bd20
  size     0x1e0: 4 x 0x78; Material's next member is at 0x1ad0 = 0x18f0 + 0x1e0.
```

| @src | size (bytes) | role | declaration |
|---|---|---|---|
| 14028bcc0 | 90 | ctor | `Cls_14028bcc0();` |
| 14028bd20 | 84 | dtor | `~Cls_14028bcc0();` |

| offset | type | member | evidence / meaning |
|---|---|---|---|
| 0x000 | `DYNARY` | burgrs | (mat+0x18f0) BURGRS |
| 0x078 | `DYNARY` | alpha | (mat+0x1968) ALPHA |
| 0x0f0 | `DYNARY` | ndisfm | (mat+0x19e0) NDISFM |
| 0x168 | `DYNARY` | recvry | (mat+0x1a58) RECVRY |


## Cls_14028c040

Header `port/defmodel/include/defmodel/layout/cls_14028c040.hpp`, stubs `port/defmodel/src/layout/cls_14028c040.cpp`.

```
Cls_14028c040 (no RTTI, not polymorphic): fracture block of a Material (Material+0x1ad0), the
data of keywords FRCMOD and UTSDAT (docs/re/keywords/FRCMOD.md, UTSDAT.md).

  ctor     14028c040 (3 DYNARYs, zeroes, types 0x1b/0x1c/0xb4)
  dtor     14029cfa0
  size     0x1f8: last write is the 8-byte field at 0x1f0 (param_1[0x7c]); Material's next field
           (MSTMTR flag) is at 0x1cc8 = 0x1ad0 + 0x1f8.
```

| @src | size (bytes) | role | declaration |
|---|---|---|---|
| 14028c040 | 420 | ctor | `Cls_14028c040();` |
| 14029cfa0 | 73 | dtor | `~Cls_14028c040();` |

| offset | type | member | evidence / meaning |
|---|---|---|---|
| 0x000 | `std::int32_t` | model | =0; FRCMOD model M (mat+0x1ad0) |
| 0x004 | `std::byte[4]` | pad_004 |  |
| 0x008 | `double` | b_t2 | =0; FRCMOD b for t 2 (mat+0x1ad8) |
| 0x010 | `double` | b_t4 | =0; t 4 (mat+0x1ae0) |
| 0x018 | `double` | b_t5 | =0; t 5, 15 (mat+0x1ae8) |
| 0x020 | `double` | b_t7 | =0; t 7 b (mat+0x1af0) |
| 0x028 | `double` | c_t7 | =0; t 7 c (mat+0x1af8) |
| 0x030 | `double` | a | =0; every type (mat+0x1b00) |
| 0x038 | `std::int32_t` | user_routine | =0; FRCMOD -t (mat+0x1b08) |
| 0x03c | `std::byte[4]` | pad_03c |  |
| 0x040 | `DYNARY` | uts | type 0x1b; UTSDAT (mat+0x1b10) |
| 0x0b8 | `double` | e | =0; FRCMOD e, every type (mat+0x1b88) |
| 0x0c0 | `DYNARY` | g | type 0x1c; FRCMOD t 12 table (mat+0x1b90) |
| 0x138 | `DYNARY` | k | type 0xb4; FRCMOD t 17 table (mat+0x1c08) |
| 0x1b0 | `double[9]` | c_t11 | =0; FRCMOD t 11 (mat+0x1c80..0x1cc0; file order in spec) |


## Cls_140289fe0

Header `port/defmodel/include/defmodel/layout/cls_140289fe0.hpp`, stubs `port/defmodel/src/layout/cls_140289fe0.cpp`.

```
Cls_140289fe0 (no RTTI, not polymorphic): anisotropy block of a Material (Material+0x1e40),
data of keyword ANISO (docs/re/keywords/ANISO.md).

  ctor     140289fe0 (DYNARY at 8, t = 0, ref = 0)
  dtor     14029cff0
  size     0x88: last write is the int at 0x80 (param_1[0x20]); Material's next field (FRAE2H)
           is at 0x1ec8 = 0x1e40 + 0x88.
```

| @src | size (bytes) | role | declaration |
|---|---|---|---|
| 140289fe0 | 60 | ctor | `Cls_140289fe0();` |
| 14029cff0 | 28 | dtor | `~Cls_140289fe0();` |

| offset | type | member | evidence / meaning |
|---|---|---|---|
| 0x000 | `std::int32_t` | t | =0; ANISO t (mat+0x1e40) |
| 0x004 | `std::byte[4]` | pad_004 |  |
| 0x008 | `DYNARY` | d | ANISO table, types 0x68/0x69/0x67/0x65 (mat+0x1e48) |
| 0x080 | `std::int32_t` | ref | =0; ANISO -t (mat+0x1ec0) |
| 0x084 | `std::byte[4]` | pad_084 |  |


## Cls_14028c500

Header `port/defmodel/include/defmodel/layout/cls_14028c500.hpp`, stubs `port/defmodel/src/layout/cls_14028c500.cpp`.

```
Cls_14028c500 (no RTTI, not polymorphic): texture triple at Material+0x2154, data of keyword
TXTURE (docs/re/keywords/TXTURE.md).

  ctor     14028c500 (a = 0, b = 1, c = 1)
  dtor     14028c540 (empty)
  size     0xc: three ints; Material's next member (DIFBND DYNARY) is at 0x2160 = 0x2154 + 0xc.
```

| @src | size (bytes) | role | declaration |
|---|---|---|---|
| 14028c500 | 46 | ctor | `Cls_14028c500();` |
| 14028c540 | 6 | dtor (empty) | `~Cls_14028c500();` |

| offset | type | member | evidence / meaning |
|---|---|---|---|
| 0x0 | `std::int32_t` | a | =0 (mat+0x2154) |
| 0x4 | `std::int32_t` | b | =1 (mat+0x2158) |
| 0x8 | `std::int32_t` | c | =1 (mat+0x215c) |


## PhasePairList

Header `port/defmodel/include/defmodel/layout/phase_pair_list.hpp`, stubs `port/defmodel/src/layout/phase_pair_list.cpp`.

```
PhasePairList (no RTTI, not polymorphic; ctor 140339c90): singly linked list of PhasePair
(inter-material / phase-transformation records) owned by DB_STEP_DATA (+0x1358).

  ctor     140339c90(this, owner) (owner, then 140339cd0: head = tail = 0, count = 0)
  reset    140339cd0
  clear    14033a120 (deletes every node through 14033a990(node, 1), then resets)
  dtor     140339d10 (-> 14033a120)
  add      140339df0 (operator new(0x780), 140333f60(node, owner); links via node+0x770)
  find     14033a040(list, from, to): walk until node->from == from && node->to == to
  count    1401b05f0 returns +0x10
  size     0x20: owner at +0x18 (ctor, 1401d8320); DB_STEP_DATA's next field is at
           0x1378 = 0x1358 + 0x20.
```

| @src | size (bytes) | role | declaration |
|---|---|---|---|
| 140339c90 | 48 | ctor (DB_STEP_DATA passes owner = 0, then sets it with 1401d8320) | `explicit PhasePairList(DB_STEP_DATA* owner = nullptr);` |
| 140339d10 | 24 | dtor | `~PhasePairList();` |

| offset | type | member | evidence / meaning |
|---|---|---|---|
| 0x00 | `PhasePair*` | head | first node |
| 0x08 | `PhasePair*` | tail | last node (new nodes are linked at tail+0x770) |
| 0x10 | `std::int32_t` | count | number of nodes (1401b05f0) |
| 0x14 | `std::byte[4]` | pad_14 |  |
| 0x18 | `DB_STEP_DATA*` | owner | ctor / 1401d8320; passed to each new node |


## PhasePair

Header `port/defmodel/include/defmodel/layout/phase_pair.hpp`, stubs `port/defmodel/src/layout/phase_pair.cpp`.

```
PhasePair (no RTTI, not polymorphic; ctor 140333f60): one inter-material (phase
transformation) record, keyword group 5, keyed by (from, to) material numbers; node of
PhasePairList at DB_STEP_DATA+0x1358. Members are named after the group-5 keywords that use
them (docs/re/keywords/<KW>.md, _common_g457.md section 5); every offset was checked against
the ctor/dtor. DBREAD 140336740, GENDB 14033a5a0, KEY writer 1403cbf00 (dbmap brief); group-5
reader 140383120 (spec).

  alloc    140339df0: operator new(0x780), 140333f60(node, list->owner)
  ctor     140333f60(this, owner) (12 DYNARYs, owner, next = 0, defaults, DYNARY types)
  dtor     140334490 (DYNARY dtors in reverse order); the list deletes nodes through
           14033a990 (scalar deleting dtor, called directly)
  size     0x780 (operator new size in 140339df0); owner at 0x778 is the last field.
  note     the ctor gives tttd_t9 type 0x37 and tttd_t1 type 0x2c; TTTD.md lists the type-9
           table as 0x2c: the reader re-types the DYNARY, so both can be true.
```

| @src | size (bytes) | role | declaration |
|---|---|---|---|
| 140333f60 | 1320 | ctor | `explicit PhasePair(DB_STEP_DATA* owner);` |
| 140334490 | 235 | dtor | `~PhasePair();` |

| offset | type | member | evidence / meaning |
|---|---|---|---|
| 0x000 | `std::int32_t` | from | =0; TTTN/PHASTF from-material (key, 14033a040) |
| 0x004 | `std::int32_t` | to | =0; to-material (key) |
| 0x008 | `std::int32_t` | phastf | =0; PHASTF raw value (2 enables PHEQVF output) |
| 0x00c | `std::int32_t` | phastf_on | =0; PHASTF v != 0 (counted into DB_STEP_DATA+0x13f4) |
| 0x010 | `std::int32_t` | tttd_kind | =0; TTTD line-1 kind |
| 0x014 | `std::int32_t` | tttd_type | =0; TTTD internal kind |
| 0x018 | `char[0x51]` | name | TTTN name line (read, never written); ctor sprintf " " |
| 0x069 | `std::byte[3]` | pad_069 |  |
| 0x06c | `std::int32_t` | tttd_nc | =2; TTTD type 1/9 nc |
| 0x070 | `double` | tttd_f1 | =0; TTTD f1 when nc == 1 |
| 0x078 | `DYNARY` | tttd_t1 | type 0x2c; TTTD type 1 table |
| 0x0f0 | `std::int32_t` | tttd_x | =0; TTTD type 1 x |
| 0x0f4 | `std::byte[4]` | pad_0f4 |  |
| 0x0f8 | `DYNARY` | tttd_t9 | type 0x37; TTTD type 9 table ("Phase Compositions") |
| 0x170 | `double` | tttd_f1b | DAT_14051f258; TTTD f1/f2 (spec default 0.01) |
| 0x178 | `double` | tttd_f2 | DAT_1405501c8; TTTD f2 (spec default 0.99) |
| 0x180 | `DYNARY` | tttd_vec | type 0xb3; TTTD types 10..13 vector |
| 0x1f8 | `DYNARY` | tttd_t2 | type 0x30; TTTD type 2 table |
| 0x270 | `double[11]` | tttd_c3 | =0; TTTD type 3 coefficients |
| 0x2c8 | `DYNARY` | tttd_t4 | type 0x32; TTTD type 4 f(T) |
| 0x340 | `double[4]` | tttd_c4 | =0; TTTD type 4 |
| 0x360 | `double[6]` | tttd_c5 | =0; TTTD type 5 |
| 0x390 | `double[4]` | tttd_c6 | =0; TTTD type 6 |
| 0x3b0 | `double[7]` | tttd_c7 | =0; TTTD type 7 |
| 0x3e8 | `double[2]` | tttd_f9 | =0; TTTD type 9 f1, f2 (nc != 1) |
| 0x3f8 | `DYNARY` | tttd_t8 | type 0x36; TTTD type 8 f(T) |
| 0x470 | `std::int32_t` | tttd_ref | =0; TTTD negative type (abs value) |
| 0x474 | `std::byte[4]` | pad_474 |  |
| 0x478 | `DYNARY` | phaslh | PHASLH (re-typed 0x1f by the reader) |
| 0x4f0 | `DYNARY` | phasvl | PHASVL (re-typed 0x20) |
| 0x568 | `DYNARY` | pheqvf | PHEQVF table (re-typed 0x6c) |
| 0x5e0 | `std::int32_t` | pheqvf_t | =0; PHEQVF t |
| 0x5e4 | `std::int32_t` | trnsfp_on | =0; TRNSFP on |
| 0x5e8 | `DYNARY` | trnsfp | TRNSFP table (type stays 0) |
| 0x660 | `std::int32_t` | coarsn_on | =0; COARSN present |
| 0x664 | `std::byte[4]` | pad_664 |  |
| 0x668 | `double` | coarsn_c1 | =0; COARSN c1 |
| 0x670 | `double` | coarsn_c2 | =0; COARSN c2 |
| 0x678 | `DYNARY` | coarsn | type 0x6d; COARSN table |
| 0x6f0 | `DYNARY` | ipsnrg | type 0x6e; IPSNRG |
| 0x768 | `std::int32_t` | varsel | =0; VARSEL |
| 0x76c | `std::byte[4]` | pad_76c |  |
| 0x770 | `PhasePair*` | next | list link |
| 0x778 | `DB_STEP_DATA*` | owner | ctor param |


## Cls_14017a6c0

Header `port/defmodel/include/defmodel/layout/cls_14017a6c0.hpp`, stubs `port/defmodel/src/layout/cls_14017a6c0.cpp`.

```
Cls_14017a6c0 (no RTTI, not polymorphic): intrusive singly linked list of LLOBJ_DATA owned by DB_STEP_DATA
(objects; two instances at DB_STEP_DATA+0x1380 and +0x13a8). Same shape as the other DB_STEP_DATA lists: head, tail, count, owner.

  ctor     14017a6c0(this, owner) (stores owner, then 14017a700 resets head/tail/count)
  reset    14017a700 (head = tail = 0, count = 0; no free)
  clear    14017ab70 (deletes every node through 14019a290(node, 1), then resets)
  dtor     14017a740
  add      14017a880 (operator new(0x3f28), 14015ec80(node, owner); links via node+0x3940)
  size     0x20: owner at +0x18 (ctor, 1401d8340); 0x13a8 - 0x1380 = 0x28 leaves 8 unknown bytes in DB_STEP_DATA.
  open     the role of the second list (0x13a8); 14019a290 is LLOBJ_DATA's non-virtual scalar deleting dtor (dtor 14015f790); 14017aa80(list, n) returns node n.
```

| @src | size (bytes) | role | declaration |
|---|---|---|---|
| 14017a6c0 | 48 | ctor (DB_STEP_DATA passes owner = 0, then sets it with 1401d8340) | `explicit Cls_14017a6c0(DB_STEP_DATA* owner = nullptr);` |
| 14017a740 | 24 | dtor | `~Cls_14017a6c0();` |

| offset | type | member | evidence / meaning |
|---|---|---|---|
| 0x00 | `LLOBJ_DATA*` | head | first node |
| 0x08 | `LLOBJ_DATA*` | tail | last node (new nodes are linked at tail+0x3940) |
| 0x10 | `std::int32_t` | count | number of nodes |
| 0x14 | `std::byte[4]` | pad_14 |  |
| 0x18 | `DB_STEP_DATA*` | owner | ctor / 1401d8340; passed to each new node |


## IObjectData

Header `port/defmodel/include/defmodel/layout/iobject_data.hpp`, stubs `port/defmodel/src/layout/iobject_data.cpp`.

```
IObjectData (RTTI .?AVIObjectData@@): interface base of LLOBJ_DATA; three pure virtual slots,
no data, no virtual dtor.

  vtable   0x140522f20 (3 slots, all _purecall 1404852f0)
  ctor     140199a70 (stores the vptr only; called by LLOBJ_DATA's ctor 14015ec80)
  dtor     none in the binary (LLOBJ_DATA's dtor 14015f790 never restores this vptr)
  size     8 (vptr only): LLOBJ_DATA's ctor writes nothing at 0x08..0x0f.
  slots    named after LLOBJ_DATA's overrides (14015fa60 get_name, 14015faa0 set_name,
           14015fbd0 returns LLOBJ_DATA+0x3c00).
```

| @src | size (bytes) | role | declaration |
|---|---|---|---|
| 140199a70 | 26 | ctor (vptr only) | `IObjectData();` |
| 1404852f0 | 54 | slot 0: _purecall (LLOBJ_DATA: copy the name out) | `virtual void get_name(char* out) const = 0;` |
| 1404852f0 | 54 | slot 1: _purecall (LLOBJ_DATA: set the name) | `virtual void set_name(const char* name) = 0;` |
| 1404852f0 | 54 | slot 2: _purecall (LLOBJ_DATA: returns the 8-byte field at 0x3c00) | `virtual void* vslot2() = 0;` |


## LLOBJ_DATA

Header `port/defmodel/include/defmodel/layout/llobj_data.hpp`, stubs `port/defmodel/src/layout/llobj_data.cpp`.

```
LLOBJ_DATA (RTTI .?AVLLOBJ_DATA@@, base IObjectData): one simulation object (keyword group 2),
node of the DB_STEP_DATA object lists (Cls_14017a6c0 at step+0x1380 / +0x13a8).
Recovered at member level only: embedded sub-objects without their own header are opaque
byte slots named s<offset>, sized to the next known member (their real size can be smaller;
the rest of the slot is then LLOBJ_DATA's own, unwritten by the ctor).

  vtable   0x140522f00 (3 slots)
  ctor     14015ec80(this, owner) (IObjectData ctor 140199a70, vptr, member ctors, 1401999d0,
           140199a00, then 14015ef60(this, owner) when owner != 0)
  dtor     14015f790 (non-virtual; 14015fbf0(this, 1), 140197ec0, member dtors reversed);
           list nodes are deleted through 14019a290 (scalar deleting dtor, called directly)
  size     0x3f28: the list's add function 14017a880 does operator new(0x3f28) before
           14015ec80; last field written: bytes 0x3f20..0x3f22.
  fixed    the first version put Cls_140197a50 and the six heap handles 0x300 too low (index
           0x724*8 is 0x3920, not 0x3620); reported by packet p1-04 and re-checked in 14015ec80 /
           14015fbf0. The 0x10 sub-object and name split also come from 14015fbf0.
  open     roles of the opaque sub-objects; the gaps 0x3610..0x391f, 0x3980..0x398f,
           0x39a8..0x39c7, 0x39f8..0x3a5f, 0x3aa0..0x3bf7; s3328/s3458 are _OBJ_GEO3D (owned by
           packet p1-07, kept opaque here).
```

Bases: IObjectData.

| @src | size (bytes) | role | declaration |
|---|---|---|---|
| 14015ec80 | 720 | ctor (owner may be 0) | `explicit LLOBJ_DATA(DB_STEP_DATA* owner);` |
| 14015f790 | 515 | dtor (non-virtual) | `~LLOBJ_DATA();` |
| 14015fa60 | 46 | slot 0: strcpy(out, name) | `void get_name(char* out) const override;` |
| 14015faa0 | 41 | slot 1: strcpy(name, in) | `void set_name(const char* in) override;` |
| 14015fbd0 | 21 | slot 2: return u3c00[0] | `void* vslot2() override;` |

| offset | type | member | evidence / meaning |
|---|---|---|---|
| 0x008 | `std::byte[8]` | pad_008 | not written by the ctor |
| 0x010 | `std::int32_t` | u010 | 140199970 =0; 14015fbf0 =0 |
| 0x014 | `std::int32_t` | u014 | =0 |
| 0x018 | `char[0x400]` | u018 | 14015fbf0 strcpy DAT_14051f7d6 |
| 0x418 | `char[0x400]` | u418 | 14015fbf0 strcpy DAT_14051f7d7 |
| 0x818 | `std::int32_t` | u818 | =0 |
| 0x81c | `std::int32_t` | u81c | =0 |
| 0x820 | `std::int32_t` | u820 | =0 |
| 0x824 | `std::byte[4]` | pad_824 |  |
| 0x828 | `char[0x54]` | name | object name (slots 0/1); ends where 0x87c starts |
| 0x87c | `std::int32_t[5]` | u87c | 14015fbf0 =0 each (0x87c..0x88c) |
| 0x890 | `std::byte[0x10]` | pad_890 |  |
| 0x8a0 | `std::byte[0x140]` | s8a0 | Cls_14017ef20 (writes +0x130, +0x138; owner 14017efc0) |
| 0x9e0 | `std::byte[0x108]` | s9e0 | Cls_140182880 (writes +0xf0, +0xf8; owner 140182920) |
| 0xae8 | `std::byte[0x70]` | sae8 | Cls_140185570 (writes +0x60, +0x68; owner 1401855d0) |
| 0xb58 | `std::byte[0x18]` | sb58 | Cls_140185c60 (140199a90 owner, 140199ab0 this) |
| 0xb70 | `StateVarBase` | ub70 | StateVarBase(-1, 0) |
| 0xba8 | `DynAryLinkedList` | uba8 | dynamic arrays |
| 0xbc8 | `std::byte[0x18]` | pad_bc8 | not written |
| 0xbe0 | `std::byte[100][0x30]` | sbe0 | 100 x Cls_14017e740 (eh vector ctor, dtor 14017e720) |
| 0x1ea0 | `std::byte[0x2c0]` | s1ea0 | Cls_1401860a0 (3 curves, writes +0x2b8; owner 1401861c0) |
| 0x2160 | `std::byte[0xa88]` | s2160 | Cls_14018b220 (16 curves, writes +0xa80; owner 14018b370) |
| 0x2be8 | `std::byte[0x128]` | s2be8 | Cls_1401889a0 (writes +0x120; owner 1401890e0) |
| 0x2d10 | `std::byte[0x158]` | s2d10 | Cls_140189440 (3 Cls_140269250; owner 1401894d0) |
| 0x2e68 | `std::byte[0x4c0]` | s2e68 | Cls_1401956d0 (writes up to +0x198; owner 1401957b0) |
| 0x3328 | `std::byte[0x130]` | s3328 | _OBJ_GEO3D (ctor 1402f9020, vtable 0x140548d18) |
| 0x3458 | `std::byte[0x140]` | s3458 | _OBJ_GEO3D (same class; slot 0x10 larger) |
| 0x3598 | `DYNARY` | u3598 | curve |
| 0x3610 | `std::byte[0x310]` | pad_3610 | not written by ctor/dtor |
| 0x3920 | `std::byte[0x20]` | s3920 | Cls_140197a50 (-> 140197b00; 14015fbf0 calls 140197b40) |
| 0x3940 | `LLOBJ_DATA*` | next | list link (14017a880, 14017ab70) |
| 0x3948 | `std::byte[8]` | pad_3948 |  |
| 0x3950 | `void*` | u3950 | heap handle (14019a8a0: new 0x20, 14019b090) |
| 0x3958 | `void*` | u3958 | heap handle (14019a8a0) |
| 0x3960 | `void*` | u3960 | heap handle (14019a8a0) |
| 0x3968 | `void*` | u3968 | heap handle (140159ad0: new 0x20, 14015a380) |
| 0x3970 | `void*` | u3970 | heap handle (14019a960: new 0x20, 14019b190) |
| 0x3978 | `void*` | u3978 | heap handle (14019aa20: new 0x20, 14019b2c0) |
| 0x3980 | `std::byte[0x10]` | pad_3980 |  |
| 0x3990 | `std::uint8_t` | u3990 | 14015fbf0 =0 |
| 0x3991 | `std::byte[7]` | pad_3991 |  |
| 0x3998 | `void*` | u3998 | ctor =0; 14015fbf0: if set, 140301d10/140301ad0 on it |
| 0x39a0 | `std::uint64_t` | u39a0 | ctor =0 |
| 0x39a8 | `std::byte[0x20]` | pad_39a8 |  |
| 0x39c8 | `std::uint64_t` | u39c8 | 1401999d0 =0 |
| 0x39d0 | `std::int32_t` | u39d0 | 1401999d0 =0 |
| 0x39d4 | `std::int32_t` | u39d4 | 140199a00 =0 |
| 0x39d8 | `std::uint64_t` | u39d8 | 140199a00 =0 |
| 0x39e0 | `std::int32_t` | u39e0 | =0 |
| 0x39e4 | `std::int32_t` | u39e4 | =0 |
| 0x39e8 | `std::int32_t` | u39e8 | =0 |
| 0x39ec | `std::int32_t` | u39ec | =0 |
| 0x39f0 | `std::int32_t` | u39f0 | =1 |
| 0x39f4 | `std::byte[0x6c]` | pad_39f4 |  |
| 0x3a60 | `SdctlPtrList_OBJ_GEO2D` | u3a60 | 2D geometry list |
| 0x3a98 | `DB_STEP_DATA*` | owner | set by 14015ef60; read as *(this+15000) |
| 0x3aa0 | `std::byte[0x158]` | pad_3aa0 |  |
| 0x3bf8 | `std::uint64_t` | u3bf8 | ctor =0 |
| 0x3c00 | `void*[100]` | u3c00 | ctor loop =0; slot 2 returns u3c00[0] |
| 0x3f20 | `std::uint8_t` | u3f20 | ctor =1 |
| 0x3f21 | `std::uint8_t` | u3f21 | =0 |
| 0x3f22 | `std::uint8_t` | u3f22 | =0 |
| 0x3f23 | `std::byte[5]` | pad_3f23 |  |


## StateVarBase

Header `port/defmodel/include/defmodel/layout/state_var_base.hpp`, stubs `port/defmodel/src/layout/state_var_base.cpp`.

```
StateVarBase (RTTI .?AVStateVarBase@@): base of the StateVar* family (StateVarNode,
StateVarNodePost, StateVarElem*, StateVarCustom). Holds an id, three current and three previous
ints and a heap vector of 8-byte values. Embedded in LLOBJ_DATA at 0xb70 as a plain StateVarBase.

  vtable   0x140549ce0 (5 slots)
  ctor     140300c90(this, id, p) (vector handle at 0x28, then 140300da0 resets the values)
  dtor     140300d50 (virtual; scalar deleting dtor 1403018d0 in slot 0; 140300e10, then
           frees the vector handle at 0x28)
  size     0x38: the ctor writes 8 bytes at 0x30 (param_1[6]); LLOBJ_DATA's next member starts
           at 0xba8 = 0xb70 + 0x38.
  fields   setters 140300ec0 (0x0c), 140300f30 (0x10, old value to 0x1c), 140300f90 (0x14 ->
           0x20), 140300ff0 (0x18 -> 0x24); getter 140300ea0 (0x08); vector size 140301020,
           data 140301050 (both on 0x28).
  open     what the pointer at 0x30 points to (140300da0 writes *(*(this+0x30)) = 0).
```

| @src | size (bytes) | role | declaration |
|---|---|---|---|
| 140300c90 | 176 | ctor; LLOBJ_DATA passes (-1, 0) | `explicit StateVarBase(std::int32_t id = -1, void* u30 = nullptr);` |
| 1403018d0 | 54 | slot 0: scalar deleting dtor (dtor body 140300d50) | `virtual ~StateVarBase();` |
| 140301710 | 82 | slot 1: set 0x10/0x14/0x18 (keeping old values), then 140301380(this, 0) | `virtual void vslot1_140301710(std::int32_t a, std::int32_t b, std::int32_t c);` |
| 140301690 | 108 | slot 2: if idx == id or idx == -1: 0x10 = a, 0x14 = arr[id], 140301380 | `virtual void vslot2_140301690(std::int32_t a, const std::int32_t* arr, std::int32_t idx);` |
| 140301770 | 207 | slot 3: copy from another state variable (slot 1, then memcpy of values) | `virtual bool copy_from(const StateVarBase* other);` |
| 140301850 | 119 | slot 4: memcpy other's values into the row chosen by 1403010e0(this, key) | `virtual bool vslot4_140301850(std::int32_t key, const StateVarBase* other);` |

| offset | type | member | evidence / meaning |
|---|---|---|---|
| 0x08 | `std::int32_t` | id | ctor param 2; getter 140300ea0 |
| 0x0c | `std::int32_t` | u0c | 140300ec0 |
| 0x10 | `std::int32_t[3]` | cur | 140300f30 / 140300f90 / 140300ff0 (ctor: 0, 1, 1) |
| 0x1c | `std::int32_t[3]` | prev | previous values of cur (ctor: 0, 1, 1) |
| 0x28 | `void*` | values | heap vector of 8-byte values (14019a6b0: new 0x20) |
| 0x30 | `void*` | u30 | ctor param 3 |


## SfLinkedData

Header `port/defmodel/include/defmodel/layout/sf_linked_data.hpp`, stubs `port/defmodel/src/layout/sf_linked_data.cpp`.

```
SfLinkedData (RTTI .?AVSfLinkedData@@): polymorphic node base of the SfLinkedList lists
(derived: OPSTOP_DATA, DYNARY_DATA).

  vtable   0x140534e20 (4 slots)
  ctor     14025a580 (vptr, then 14025a5e0: 0x08 = 1, 0x0c = 0, next = 0)
  dtor     14025a5c0 (virtual; scalar deleting dtor 14025bf80 in slot 0)
  size     0x18: 14025a5e0 writes the 8-byte next pointer at 0x10; SfLinkedList walks nodes
           through node[2] (14025ab20); DYNARY_DATA's own fields start at 0x18 (14025b0d0).
```

| @src | size (bytes) | role | declaration |
|---|---|---|---|
| 14025a580 | 44 | ctor | `SfLinkedData();` |
| 14025bf80 | 54 | slot 0: scalar deleting dtor (dtor body 14025a5c0) | `virtual ~SfLinkedData();` |
| 14025a610 | 27 | slot 1: copies other->u0c into this->u0c | `virtual void copy_from(const SfLinkedData* other);` |
| 1404852f0 | 54 | slot 2: _purecall (DYNARY_DATA: 14025b190, empty) | `virtual void vslot2() = 0;` |
| 1404852f0 | 54 | slot 3: _purecall (DYNARY_DATA: 14025b1b0, empty) | `virtual void vslot3() = 0;` |

| offset | type | member | evidence / meaning |
|---|---|---|---|
| 0x08 | `std::uint8_t` | u08 | =1 |
| 0x0c | `std::int32_t` | u0c | =0; copied by slot 1 |
| 0x10 | `SfLinkedData*` | next | =0; list link |


## DYNARY_DATA

Header `port/defmodel/include/defmodel/layout/dynary_data.hpp`, stubs `port/defmodel/src/layout/dynary_data.cpp`.

```
DYNARY_DATA (RTTI .?AVDYNARY_DATA@@, base SfLinkedData): node of DynAryLinkedList (a dynamic
array record of an object).

  vtable   0x140534ea0 (4 slots)
  ctor     14025afe0 (SfLinkedData ctor 14025a580, vptr, handles at 0x30/0x38, 14025b0b0 ->
           14025b0d0 init)
  dtor     14025b050 (virtual; scalar deleting dtor 14025c080 in slot 0)
  size     0x40: DynAryLinkedList::create_item (14025ba40) does operator new(0x40).
  open     element types of the two heap containers at 0x30 (14019a6b0: new 0x20, vector-like
           {alloc, first, last, end}) and 0x38 (140159ad0: new 0x20, 14015a380).
```

Bases: SfLinkedData.

| @src | size (bytes) | role | declaration |
|---|---|---|---|
| 14025afe0 | 95 | ctor | `DYNARY_DATA();` |
| 14025c080 | 54 | slot 0: scalar deleting dtor (dtor body 14025b050) | `~DYNARY_DATA() override;` |
| 14025b150 | 49 | slot 1: SfLinkedData::copy_from, then 14025b1d0(other) | `void copy_from(const SfLinkedData* other) override;` |
| 14025b190 | 11 | slot 2: empty | `void vslot2() override;` |
| 14025b1b0 | 11 | slot 3: empty | `void vslot3() override;` |

| offset | type | member | evidence / meaning |
|---|---|---|---|
| 0x18 | `std::int32_t` | u18 | 14025b0d0 =0 |
| 0x1c | `std::uint8_t` | u1c | =0 |
| 0x20 | `std::int32_t` | u20 | =0 |
| 0x24 | `std::int32_t` | u24 | =0 |
| 0x28 | `std::int32_t` | u28 | =0 |
| 0x30 | `void*` | u30 | heap vector handle (14019a6b0), reset by 1402568b0 |
| 0x38 | `void*` | u38 | heap container handle (140159ad0), reset by 14019a7d0 |


## SfLinkedList

Header `port/defmodel/include/defmodel/layout/sf_linked_list.hpp`, stubs `port/defmodel/src/layout/sf_linked_list.cpp`.

```
SfLinkedList (RTTI .?AVSfLinkedList@@): polymorphic list of SfLinkedData nodes (derived:
OpStopLinkedList, DynAryLinkedList). Owns its nodes: clearing deletes each through its virtual
scalar deleting dtor.

  vtable   0x140534e48 (2 slots)
  ctor     14025a680 (vptr, 14025a6f0: head = tail = 0, count = 0)
  dtor     14025a6c0 (virtual; scalar deleting dtor 14025bfc0 in slot 0; 14025ab20 clears)
  size     0x20: 14025a6f0 writes 0x08, 0x10 (8 bytes each) and the int at 0x18.
```

| @src | size (bytes) | role | declaration |
|---|---|---|---|
| 14025a680 | 44 | ctor | `SfLinkedList();` |
| 14025bfc0 | 54 | slot 0: scalar deleting dtor (dtor body 14025a6c0) | `virtual ~SfLinkedList();` |
| 1404852f0 | 54 | slot 1: _purecall (derived lists: allocate a new node) | `virtual SfLinkedData* create_item() = 0;` |

| offset | type | member | evidence / meaning |
|---|---|---|---|
| 0x08 | `SfLinkedData*` | head |  |
| 0x10 | `SfLinkedData*` | tail |  |
| 0x18 | `std::int32_t` | count |  |
| 0x1c | `std::byte[4]` | pad_1c |  |


## DynAryLinkedList

Header `port/defmodel/include/defmodel/layout/dyn_ary_linked_list.hpp`, stubs `port/defmodel/src/layout/dyn_ary_linked_list.cpp`.

```
DynAryLinkedList (RTTI .?AVDynAryLinkedList@@, base SfLinkedList): list of DYNARY_DATA, embedded
in LLOBJ_DATA at 0xba8.

  vtable   0x140534ec8 (2 slots)
  ctor     14025b9d0 (SfLinkedList ctor 14025a680, then vptr; no own fields)
  dtor     14025ba10 (virtual; scalar deleting dtor 14025c0c0 in slot 0)
  size     0x20 (= SfLinkedList; the ctor writes nothing else). LLOBJ_DATA's next member is at
           0xbe0 = 0xba8 + 0x38, so the 0x18 bytes after it belong to LLOBJ_DATA (not written).
```

Bases: SfLinkedList.

| @src | size (bytes) | role | declaration |
|---|---|---|---|
| 14025b9d0 | 44 | ctor | `DynAryLinkedList();` |
| 14025c0c0 | 54 | slot 0: scalar deleting dtor (dtor body 14025ba10) | `~DynAryLinkedList() override;` |
| 14025ba40 | 87 | slot 1: return new DYNARY_DATA (operator new(0x40), 14025afe0) | `SfLinkedData* create_item() override;` |


## SdctlPtrCollection

Header `port/defmodel/include/defmodel/layout/sdctl_ptr_collection.hpp`, stubs `port/defmodel/src/layout/sdctl_ptr_collection.cpp`.

```
SdctlPtrCollection (RTTI .?AVSdctlPtrCollection@@): abstract pointer collection, base of
SdctlGList.

  vtable   0x14054f020 (5 slots)
  ctor     140333090 (vptr, owns = 0)
  dtor     140333100 (virtual; scalar deleting dtor 1403330c0 in slot 2)
  size     0x10: the ctor writes the byte at 0x08; SdctlGList's own fields start at 0x10.
  slots    pure slots are named after what SdctlGList / SdctlPtrList<_OBJ_GEO2D> put there.
```

| @src | size (bytes) | role | declaration |
|---|---|---|---|
| 140333090 | 35 | ctor | `SdctlPtrCollection();` |
| 1404852f0 | 54 | slot 0: _purecall (SdctlGList: returns the count at 0x2c) | `virtual std::int32_t count() const = 0;` |
| 1404852f0 | 54 | slot 1: _purecall (SdctlGList: remove all, calling slot 4 per item) | `virtual void clear() = 0;` |
| 1403330c0 | 54 | slot 2: scalar deleting dtor (dtor body 140333100) | `virtual ~SdctlPtrCollection();` |
| 140333f50 | 16 | slot 3: returns its argument unchanged | `virtual void* vslot3_140333f50(void* item);` |
| 1404852f0 | 54 | slot 4: _purecall (SdctlPtrList: delete the item when owns != 0) | `virtual void delete_item(void* item) = 0;` |

| offset | type | member | evidence / meaning |
|---|---|---|---|
| 0x08 | `std::uint8_t` | owns | =0; SdctlPtrList's slot 4 deletes items only when set |
| 0x09 | `std::byte[7]` | pad_09 |  |


## SdctlGList

Header `port/defmodel/include/defmodel/layout/sdctl_g_list.hpp`, stubs `port/defmodel/src/layout/sdctl_g_list.cpp`.

```
SdctlGList (RTTI .?AVSdctlGList@@, base SdctlPtrCollection): generic singly linked pointer list
with a cached current index; base of SdctlPtrList<T>.

  vtable   0x14054efe8 (6 slots)
  ctor     140330ee0 (SdctlPtrCollection ctor, vptr, zero 0x10/0x18/0x20/0x2c, 0x28 = -1,
           0x30 = 0)
  dtor     140331030 (virtual; scalar deleting dtor 140333120 in slot 2: clear, delete the
           object at 0x30 via 140333190(p, 1), base dtor)
  size     0x38: the ctor writes 8 bytes at 0x30; SdctlPtrList<_OBJ_GEO2D> adds nothing and
           fills LLOBJ_DATA 0x3a60..0x3a98 exactly.
  nodes    malloc'd {item, ?, next}: clear frees them and follows node+0x10.
```

Bases: SdctlPtrCollection.

| @src | size (bytes) | role | declaration |
|---|---|---|---|
| 140330ee0 | 120 | ctor | `SdctlGList();` |
| 14019ab70 | 14 | slot 0: return the count (0x2c) | `std::int32_t count() const override;` |
| 1403321a0 | 197 | slot 1: reset fields, 140333230(u30, 0), slot 4 per item, free the nodes | `void clear() override;` |
| 140333120 | 54 | slot 2: scalar deleting dtor (dtor body 140331030) | `~SdctlGList() override;` |
| 140330ea0 | 55 | slot 5: returns a != b | `virtual bool differs(const void* a, const void* b) const;` |

| offset | type | member | evidence / meaning |
|---|---|---|---|
| 0x10 | `void*` | head | first node |
| 0x18 | `void*` | tail |  |
| 0x20 | `void*` | u20 | (current node?) |
| 0x28 | `std::int32_t` | cur_index | =-1 |
| 0x2c | `std::int32_t` | n | count (slot 0) |
| 0x30 | `void*` | u30 | owned helper object (deleted via 140333190(p, 1)) |


## SdctlPtrList_OBJ_GEO2D

Header `port/defmodel/include/defmodel/layout/sdctl_ptr_list_obj_geo2d.hpp`, stubs `port/defmodel/src/layout/sdctl_ptr_list_obj_geo2d.cpp`.

```
SdctlPtrList_OBJ_GEO2D = RTTI .?AV?$SdctlPtrList@V_OBJ_GEO2D@@@@ (SdctlPtrList<_OBJ_GEO2D>,
bases SdctlGList, SdctlPtrCollection): list of 2D geometry objects, embedded in LLOBJ_DATA at
0x3a60. Written as a plain class named after the instantiation; make it a template once a
second instantiation is ported (SdctlPtrList<CsdspmSimulationProcessMonitorDataSource::CRecord>
also exists, but it is process-monitor code and out of scope).

  vtable   0x1405230c0 (6 slots)
  ctor     14019aae0 (SdctlGList ctor 140330ee0, then vptr)
  dtor     14019ab10 (virtual; scalar deleting dtor 14019ad70 in slot 2: clear, base dtor)
  size     0x38 (= SdctlGList; the ctor writes nothing else; LLOBJ_DATA's next field is at
           0x3a98 = 0x3a60 + 0x38).
```

Bases: SdctlGList.

| @src | size (bytes) | role | declaration |
|---|---|---|---|
| 14019aae0 | 44 | ctor | `SdctlPtrList_OBJ_GEO2D();` |
| 14019ab50 | 24 | slot 0: -> 14019ab70 (count) | `std::int32_t count() const override;` |
| 14019abb0 | 24 | slot 1: -> 1403321a0 (clear) | `void clear() override;` |
| 14019ad70 | 54 | slot 2: scalar deleting dtor (dtor body 14019ab10) | `~SdctlPtrList_OBJ_GEO2D() override;` |
| 14019b980 | 96 | slot 4: if owns and item != 0: item's virtual scalar deleting dtor (flag 1) | `void delete_item(void* item) override;` |


## Cls_1401fa020

Header `port/defmodel/include/defmodel/layout/cls_1401fa020.hpp`, stubs `port/defmodel/src/layout/cls_1401fa020.cpp`.

```
Cls_1401fa020 (no RTTI, not polymorphic): intrusive singly linked list of Cls_1401f46e0 owned by DB_STEP_DATA
(inter-object data, DB_STEP_DATA+0x13c8). Same shape as the other DB_STEP_DATA lists: head, tail, count, owner.

  ctor     1401fa020(this, owner) (stores owner, then 1401fa080 resets head/tail/count)
  reset    1401fa080 (head = tail = 0, count = 0; no free)
  clear    1401fade0 (deletes every node through 1401fb200(node, 1), then resets)
  dtor     1401fa060
  add      1401faaa0 (operator new(0x3a0), 1401f46e0(node, owner); links via node+0x390)
  size     0x20: owner at +0x18 (ctor, 1401d8360); DB_STEP_DATA's next field is at 0x13e8 = 0x13c8 + 0x20.
  open     none (Ghidra labels the element dtor 1401f4950 ~CBitmapButton: a FID false match).
```

| @src | size (bytes) | role | declaration |
|---|---|---|---|
| 1401fa020 | 48 | ctor (DB_STEP_DATA passes owner = 0, then sets it with 1401d8360) | `explicit Cls_1401fa020(DB_STEP_DATA* owner = nullptr);` |
| 1401fa060 | 24 | dtor | `~Cls_1401fa020();` |

| offset | type | member | evidence / meaning |
|---|---|---|---|
| 0x00 | `Cls_1401f46e0*` | head | first node |
| 0x08 | `Cls_1401f46e0*` | tail | last node (new nodes are linked at tail+0x390) |
| 0x10 | `std::int32_t` | count | number of nodes |
| 0x14 | `std::byte[4]` | pad_14 |  |
| 0x18 | `DB_STEP_DATA*` | owner | ctor / 1401d8360; passed to each new node |


## Cls_1401f46e0

Header `port/defmodel/include/defmodel/layout/cls_1401f46e0.hpp`, stubs `port/defmodel/src/layout/cls_1401f46e0.cpp`.

```
Cls_1401f46e0 (no RTTI, not polymorphic): one inter-object record (keyword group 3: contact
between two objects), node of the DB_STEP_DATA inter-object list Cls_1401fa020 (step+0x13c8).
DBREAD 1401f58a0, GENDB 1401fa4f0, KEY writer 1403c9ad0 (dbmap brief).

  ctor     1401f46e0(this, owner) (Cls_1401fb020 at 0, Cls_140269250 at 0x188, 3 curves,
           owner, next = 0, fields, curve types 0x17/0x16, 14026a120(+0x188, owner))
  dtor     1401f4950 (Ghidra FID mislabels it ~CBitmapButton; curve dtors, 140269320 on 0x188,
           1401fb1a0 on 0); deleted through 1401fb200 (scalar deleting dtor, called directly)
  size     0x3a0: the list's add function 1401faaa0 does operator new(0x3a0); owner at 0x398
           is the last field.
  open     the 0x30-byte sub-object at 0x188 (ctor 140269250 stores an owner at +0x20 and
           14026a120 reads the step dimension; it sits next to the LLWINDEF_DATA /
           LLRANGEDEF_DATA code (vtables 0x140535fe0 / 0x140536000), so it is probably a list
           of contact window/range definitions; not verified).
```

| @src | size (bytes) | role | declaration |
|---|---|---|---|
| 1401f46e0 | 607 | ctor | `explicit Cls_1401f46e0(DB_STEP_DATA* owner);` |
| 1401f4950 | 105 | dtor | `~Cls_1401f46e0();` |

| offset | type | member | evidence / meaning |
|---|---|---|---|
| 0x000 | `Cls_1401fb020` | u000 | leading sub-object |
| 0x168 | `std::int32_t` | u168 | =0 |
| 0x16c | `std::int32_t` | u16c | =0 |
| 0x170 | `std::int32_t` | u170 | =0 |
| 0x174 | `std::int32_t` | u174 | =1 |
| 0x178 | `std::int32_t` | u178 | =0 |
| 0x17c | `std::byte[4]` | pad_17c |  |
| 0x180 | `std::uint64_t` | u180 | =0 |
| 0x188 | `std::byte[0x30]` | s188 | Cls_140269250 (owner at +0x20; dtor 140269320) |
| 0x1b8 | `DYNARY` | u1b8 | curve type 0x17 |
| 0x230 | `std::int32_t` | u230 | =0 |
| 0x234 | `std::int32_t` | u234 | =0 |
| 0x238 | `std::uint64_t` | u238 | =0 |
| 0x240 | `DYNARY` | u240 | curve type 0x16 |
| 0x2b8 | `DYNARY` | u2b8 | curve |
| 0x330 | `std::int32_t` | u330 | =0 |
| 0x334 | `std::int32_t` | u334 | =1 |
| 0x338 | `std::uint64_t` | u338 | =0 |
| 0x340 | `std::int32_t` | u340 | =0 |
| 0x344 | `std::byte[4]` | pad_344 |  |
| 0x348 | `std::uint64_t[6]` | u348 | =0 each |
| 0x378 | `std::int32_t` | u378 | =0 |
| 0x37c | `std::int32_t` | u37c | =0 |
| 0x380 | `std::int32_t` | u380 | =0 |
| 0x384 | `std::int32_t` | u384 | =0 |
| 0x388 | `std::uint64_t` | u388 | =0 |
| 0x390 | `Cls_1401f46e0*` | next | list link |
| 0x398 | `DB_STEP_DATA*` | owner | ctor param |


## Cls_1401fb020

Header `port/defmodel/include/defmodel/layout/cls_1401fb020.hpp`, stubs `port/defmodel/src/layout/cls_1401fb020.cpp`.

```
Cls_1401fb020 (no RTTI, not polymorphic): leading sub-object (offset 0) of the inter-object
record Cls_1401f46e0. Two curves of type 0x15 and twelve 8-byte values. Whether it is a base
class or a first member cannot be told (no RTTI, no vtable); it is modelled as a member.

  ctor     1401fb020 (curves at 0x18/0x90, fields, 14012d2a0(curve, 0x15) twice)
  dtor     1401fb1a0 (called last by the record's dtor 1401f4950)
  size     0x168: last write is the 8-byte field at 0x160 (param_1[0x58]); the record's own
           fields start at 0x168 (1401f46e0).
```

| @src | size (bytes) | role | declaration |
|---|---|---|---|
| 1401fb020 | 384 | ctor | `Cls_1401fb020();` |
| 1401fb1a0 | 55 | dtor | `~Cls_1401fb020();` |

| offset | type | member | evidence / meaning |
|---|---|---|---|
| 0x000 | `std::int32_t` | u000 | =1 |
| 0x004 | `std::uint8_t` | u004 | =0 |
| 0x005 | `std::uint8_t` | u005 | =0 |
| 0x008 | `std::int32_t` | u008 | =0 |
| 0x00c | `std::int32_t` | u00c | =0 |
| 0x010 | `std::int32_t` | u010 | =0 |
| 0x014 | `std::int32_t` | u014 | =0 |
| 0x018 | `DYNARY` | u018 | curve type 0x15 |
| 0x090 | `DYNARY` | u090 | curve type 0x15 |
| 0x108 | `std::uint64_t[12]` | u108 | =0 each |


## Cls_14032f100

Header `port/defmodel/include/defmodel/layout/cls_14032f100.hpp`, stubs `port/defmodel/src/layout/cls_14032f100.cpp`.

```
Cls_14032f100 (no RTTI, not polymorphic): member of DB_STEP_DATA at 0x33b8: an int and an
owning handle to a heap std::list.

  ctor     14032f100 (14032f920 at +8, then int at 0 = 0)
  dtor     14032f140 (-> 14032f990 on +8)
  size     0x10: the ctor writes 0x00 (int) and 0x08 (8-byte handle); DB_STEP_DATA's init
           1401c8c00 writes its own int at 0x33c8 = 0x33b8 + 0x10.
  open     meaning of the int (a count?) and of the list (see Cls_14032f920).
```

| @src | size (bytes) | role | declaration |
|---|---|---|---|
| 14032f100 | 45 | ctor | `Cls_14032f100();` |
| 14032f140 | 28 | dtor | `~Cls_14032f100();` |

| offset | type | member | evidence / meaning |
|---|---|---|---|
| 0x0 | `std::int32_t` | u0 | ctor =0 |
| 0x4 | `std::byte[4]` | pad_4 |  |
| 0x8 | `Cls_14032f920` | u8 | heap list handle |


## Cls_14032f920

Header `port/defmodel/include/defmodel/layout/cls_14032f920.hpp`, stubs `port/defmodel/src/layout/cls_14032f920.cpp`.

```
Cls_14032f920 (no RTTI, not polymorphic): owning handle to a heap std::list (VS2010 layout),
member of Cls_14032f100.

  ctor     14032f920: p = operator new(0x18); 1403303a0(p) builds an empty std::list:
             p+0x00 allocator byte (140001000 is a no-op ctor), p+0x08 sentinel node
             (operator new(0x48), 140330880; node->next = node->prev = node), p+0x10 size = 0.
           List nodes are 0x48 bytes: next, prev, value (0x38 bytes) at +0x10. The value ctor
           14032ece0 builds a string-like member at +0x08 (140005040) and an 8-byte member at
           +0x30 (14032f7d0), then 14032eee0(value, 0).
  dtor     14032f990 (-> 14032fe10(p): destroys the list, frees p)
  size     8 (one pointer written by the ctor).
  open     value type of the list; it sits next to the StateVarCustom code (vtable 0x14054ecb0,
           slot 0 14032eca0), so it may describe user-defined state variables.
```

| @src | size (bytes) | role | declaration |
|---|---|---|---|
| 14032f920 | 100 | ctor (allocates the list) | `Cls_14032f920();` |
| 14032f990 | 27 | dtor (14032fe10 frees the list) | `~Cls_14032f920();` |

| offset | type | member | evidence / meaning |
|---|---|---|---|
| 0x00 | `void*` | list | heap std::list (0x18-byte VS2010 list object, see above) |

