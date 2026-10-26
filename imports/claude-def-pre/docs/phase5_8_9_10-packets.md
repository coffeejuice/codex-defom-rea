# Phases 5, 8-9 and 10: work packets

Split of the remaining new functions of `docs/re/pre64_batch_ledger.tsv`: P5 (19), P8 (587),
P9 (506) and P10 (125), 1 237 in all. One row per function, with its packet, is in
[`re/phase5_8_9_10_packet_map.tsv`](re/phase5_8_9_10_packet_map.tsv); the split is reproduced by
`python -I docs/re/phase5_8_9_10_packets.py` (needs `deform-rea-work/out/pre64_funcs.tsv`).

**How the split was made.** Phases 8-9: immediate dominators of the call graph restricted to the
new P8/P9 functions, with the command handlers and menu entries as roots. A function goes to the
packet of its nearest dominator in the script's `CUTS` table, so code that only one command (or
one reader, one generator) reaches stays with it; helpers shared by several commands got a cut of
their own. Functions that no direct call reaches (vtable slots, the DXF entity table, function
pointers, the MORP list built inline in `FUN_140378e00`) are placed by hand. GEOPRIM class code
(0x1402b7400-0x1402f2fff) that only the CNVT3D root dominates goes with the GEOPRIM object packet.
Phase 10: the closure of menus 3-4, then of menus 5-6; the rest are the registry label handlers.

**Same rules as before (every packet).** Take FP order from the disassembly, never from the Ghidra
decompile (it regroups sums). Each function keeps its address in a comment and its status in a
`docs/re/status/<packet>.tsv`. Callees in another packet or an earlier phase that are not merged yet
go through the ext-stub seam (pending/ext pattern of Phase 1) so every packet can start now.
Tests run through the oracle cache; external programs are run unchanged from the install
(`DEFORM_PC_PATH` = DEF_PRE's folder, see `docs/re/commands/common.md`).

## Summary

| Packet | Functions | KB | Scope | Main specs | Oracle cases |
|---|---:|---:|---|---|---|
| p5-01 menu shell and batch drivers | 25 | 13 | `main`, INI / DEF_MULTI.INI driver, main menu loop, selection prompt, menu 8 KEY save, menu 2 file input and menu 9 (19 P5 + 6 P9) | port-plan §2, FILE_INPUT_MENU, MENU9_2D3D | every tier-2/3 case (all run through `main` and the menu loop); FILE_INPUT 8, MENU9 3; new scripted cases listed below |
| p8-01 GFREAD driver and small commands | 71 | 11 | GFREAD dispatch, messages, the 2D reader front end shared by IGES/DXF, `.UNV`/`.PDA` launches; MORP (with its inline argument list), GENINT, IMPLUB, IMPEQP; ARMCTL and EXECMD case bodies | GFREAD, MORP, GENINT, IMPLUB, IMPEQP, ARMCTL, EXECMD, DXF_IGES_READERS | GFREAD driver cases (badext, missing, id0, 3d_key_noop, 3d_unv); MORP 3, GENINT 3, IMPLUB 4, IMPEQP 3, ARMCTL 3, EXECMD 4; corpus `cmd/` cases with IMPEQP (10) and EXECMD (8) |
| p8-02 STL and NASTRAN readers | 74 | 33 | `FUN_140243a70` subtree: STL detect / ASCII / binary, bounds, vertex merge, orientation, `DEF_STL.OUT`, store; NASTRAN type scan, readers, stores | GFREAD_STL_GEO_NAS §2, §4 | gfread_3d_stl, gfread_stl_* (6), gfread_nas_* (5) |
| p8-03 IGES reader | 77 | 57 | `Uic2CObjectIgesFile`, Fortran passes 1/2 (`FUN_14040c220`, `FUN_140409af0`), `DEF_IGS.OUT`, geometry build and tolerance, `Uic2CObject` base vtable | IGES_READER, DXF_IGES_READERS | gfread_2d_igs, gfread_igs_* (17) |
| p8-04 DXF reader | 61 | 27 | `Uic2CObjectDxfFile`, pair loop `FUN_1404093a0`, the entity table at `0x14062bdb0` (23 handlers), segments, chain stage | DXF_READER, DXF_IGES_READERS | gfread_2d_dxf, gfread_dxf_* (26) |
| p8-05 BRDEXT | 76 | 14 | border extraction `FUN_1401a9ac0` → `FUN_14013eb80`, `AMG_DXT`/`AMG_DXT3` launch and read-back, the qsort comparator `FUN_14013ca60` | BRDEXT, REMESH | BRDEXT 5; REMESH 6 as regression |
| p8-06 3D solid mesh chain | 55 | 45 | `FUN_140140110`: `CHK_VOL.INI`, `MESH3D_GUI`/`BRKMSH`/`CHKVOL` read-back (`SCRATCH.KEY`, `mesh.out`), coating mesh, mesh check `FUN_14026a4f0` (`DEF_MHCK`) | DEFAMG (3D), CNVT3D §6 | DEFAMG 3D cases (defamg_3d, defamg_3d_block3), CNVT3D tet cases |
| p8-07 DEFAMG and mesher input files | 64 | 30 | `FUN_1401a9be0`; `DEF_AMG.INI`, `DEF_AMG3.INI`, `DSTPAR*.DAT`, `BLOCK3.DAT`, `NBCD.DAT`, `<prob>.GEO`, `USRWIN.DAT`, `MESH3D.INP`; 2D remesh `FUN_140144a70`; `_MeshGenBase` (vtable, 10 slots) | DEFAMG, CNVT3D §6.3 | DEFAMG 8 |
| p8-08 DEFINT, DEFFRZ, REMESH | 62 | 23 | `FUN_1401aa000`, `FUN_1401aa370`, `FUN_1401a9a10`; DEF_INT 2D `FUN_140155c90` / 3D `FUN_140156950`, `FLDGEO.KEY`, `_REM.KEY` read-back, object replace after remesh (`FUN_1401f3660`, `FUN_1401d7390`), 2D mesh rebuild `FUN_14029b630` | DEFINT, DEFFRZ, REMESH | DEFINT 4, DEFFRZ 5, REMESH 6 |
| p8-09 geometry support library | 47 | 15 | `_DEF_AMGGEO` (2D/3D GEO reader and store, `FUN_140246f70`, `FUN_1402478f0`, `FUN_1402484c0`, `FUN_14024bac0`, topology `FUN_140249700`), point locator (`FUN_14040e620`…`FUN_1404115d0`), token reader `FUN_140411ad0`, shared helpers | GFREAD_STL_GEO_NAS §2.6, §3, §4.6, §5; BRDEXT | gfread_2d_geo, gfread_3d_geo, gfread_geo2d_* (3), gfread_geo3d_* (3); tier-1 golden vectors for the locator |
| p9-01 CNVT3D driver | 42 | 19 | `FUN_1401467f0`: options, objects, FIXGEO `FUN_14027ece0`, property copy `FUN_140148750`, model copy `FUN_1401a91d0`, output | CNVT3D | cnvt3d_n0, no_actout, no_geo23, two_objects, geometry_die_obj1 |
| p9-02 GEOPRIM object | 85 | 11 | `_DEF_GEOPRIM`: ctor/dtor `FUN_1402b7ad0`/`FUN_1402b8090`, revolve/extrude setup `FUN_1402b8980`/`FUN_1402b8520`, `DEF_GEOPRIM.KEY`, profile/section arrays and accessors (0x1402e2…, 0x1402e9…, 0x1402f0-0x1402f2…) | CNVT3D_SWEEP §10 | through p9-04/p9-05 cases; tier-1 on the setup records |
| p9-03 CNVT3D mesh build and sweep | 69 | 71 | `FUN_140147b70`: geometry `FUN_140153710` / `FUN_140153c00` / `FUN_140154270`, in-process sweep `FUN_140148f70`, NBCD.OUT, state transfer `FUN_1401627e0`, DEF_INT step `FUN_140146050` | CNVT3D_SWEEP, CNVT3D | cnvt3d_sweep_* (9), revolve90_brick_obj2, extrude_tet_obj2, revolve360_tet_obj2 |
| p9-04 GEOPRIM builder and extrude | 92 | 25 | `FUN_1402ba500`, extrude `FUN_1402baa60` (geometry path `FUN_1402bb460`, mesh path `FUN_1402bbb60`), face generation `FUN_1402e24f0` with the polygon library at 0x140420… | CNVT3D_SWEEP §10.4 | extrude cases: cnvt3d_extrude_tet_obj2, cnvt3d_sweep_extrude_n3 |
| p9-05 GEOPRIM revolve | 69 | 92 | revolve `FUN_1402ce190`: axis class `FUN_1402d9bb0`, surface generator `FUN_1402d7cd0`, `FUN_1402da250`, mesh-boundary sweep `FUN_1402ce290`, section steps, strips; cap triangulation `FUN_14041f830` (with 0x14045a…/0x14045c… triangulator) | CNVT3D_SWEEP §10.4 | cnvt3d_geometry_die_obj1, revolve cases, sweep_rev* (4) |
| p10-01 edit menus 3-4 and form helpers | 32 | 30 | menu 3 simulation controls `FUN_140017540`, menu 4 material `FUN_1400157c0`, shared form readers/range checks (`FUN_140007a80`, `FUN_140008030`, `FUN_140008220`) | none yet (strings only) | new scripted sessions |
| p10-02 edit menus 5-6 | 44 | 46 | menu 5 objects `FUN_14001a050`, menu 6 inter-object `FUN_140019c20` (positioning, contact, BCC forms) | none yet | new scripted sessions |
| p10-03 registry label handlers | 49 | 9 | registry column 3 (axis labels such as "Coefficient (Y)"); default verdict `not-needed`, see task T10-1 | port-plan §3 | none |
| nn shape-roll extrude | 143 | 97 | `FUN_1402bd210` subtree: shape-rolling extrude, ALE meshing option, `GEOPRIM_INTERPOLATION`, snap to roll shape. **Not reachable in batch**, mark `not-needed` (proof below) | – | – |

17 porting packets plus p10-03 (a check, not a port) and the not-needed group. With ext-stubs all
17 can start at once; if fewer threads are wanted, start the 14 of wave 1 below and the other 3
when their main dependency is merged.

**Wave 1 (independent or stub-only):** p5-01, p8-01 … p8-09, p9-02, p9-04, p9-05, p10-01.
**Wave 2:** p9-01, p9-03 (need p8-07/p8-08 and p9-02/p9-04 for anything beyond unit tests),
p10-02 (needs the p10-01 form helpers).

## Dependencies

Call edges between packets (from the map; counts are call edges):

| Packet | Calls into (edges) | Merge after |
|---|---|---|
| p5-01 | p10-01 (2), p10-02 (2), p9-01 (1); P1 (27), P2 (5), P3 (6), P4 (18) | Phase 4 menu 7 (`FUN_14000a370`, p4-01) |
| p8-01 | p8-09 (4), p8-03 (2), p8-04 (2), p8-02 (1) | p8-09 |
| p8-02 | p8-09 (27); P6 (60: 11 functions, see ownership) | p8-09 |
| p8-03 | p8-01 (14), p8-09 (1), p5-01 (1) | p8-01 |
| p8-04 | p8-01 (12), p8-03 (3), p8-09 (1) | p8-01, p8-03 (shared `Uic2CObject` base) |
| p8-05 | p8-09 (6), p8-07 (3), p8-08 (2) | p8-07, p8-08 |
| p8-06 | p8-09 (8), p8-07 (4) | p8-07 |
| p8-07 | p8-09 (6), p8-08 (2), p8-06 (1) | p8-09 |
| p8-08 | p8-07 (7), p8-09 (3), p8-05 (1) | p8-07 |
| p8-09 | none | – |
| p9-01 | p8-08 (6), p9-02 (4), p8-09 (3), p8-07 (2), p9-04 (1), p9-03 (1) | p8-07, p8-08, p9-02 |
| p9-02 | p8-09 (2) | – |
| p9-03 | p9-01 (12), p8-07 (8), p9-02 (8), p8-08 (7), p8-09 (6), p9-04 (2), p8-06 (1) | p9-01, p9-04 |
| p9-04 | p8-09 (6), p9-02 (3), p9-05 (2), shape-roll (1: the `+0x23a4 == 1` branch, stub it) | p9-02 |
| p9-05 | p9-04 (32), p8-09 (6), p9-01 (5), p9-02 (3), p8-08 (1) | p9-04 |
| p10-01 | p5-01 (9) | p5-01 |
| p10-02 | p10-01 (21), p5-01 (9), p8-07 (4), p8-06 (1) | p10-01 |

Earlier phases: every packet calls Phase 1-4 code (model, KEY writer/reader, GENDB); use the
merged `kw-merge` API. Calls into Phase 6-7 functions are listed under ownership.

## Ownership of shared functions

* **Inside this plan** the map is the owner list: each function is ported once, by its packet. The
  most shared ones and their owners:
  * p8-09: point locator, `_DEF_AMGGEO`, token reader `FUN_140411ad0`, `FUN_140256800`
    (`_DEF_AMGGEO` vtable `0x1402566d0`), helpers `FUN_140202700`, `FUN_1402027e0`,
    `FUN_140158fc0`, `FUN_140198270`, `FUN_1401ffd80`, `FUN_14023a380`, `FUN_140006730`.
  * p8-07: mesher input writers used by DEFAMG, REMESH and CNVT3D (`FUN_14013ee00`,
    `FUN_14013f410`, `FUN_140143920`, `FUN_140141580` NBCD.OUT, `FUN_140148840`,
    `FUN_140155340`, `FUN_1402062b0`) and `_MeshGenBase`.
  * p8-08: `FUN_14013e880` (exchange-file clean-up), `FUN_1401563c0`, `FUN_1401f3660` family,
    `FUN_1401d7390`, `FUN_14029b630`, `FUN_140339d30`.
  * p8-01: the GFREAD 2D front end used by p8-03/p8-04 (`FUN_140243830`, `FUN_140244010`,
    `FUN_140244200`).
  * p9-01: `FUN_14027ece0` (FIXGEO), `FUN_140220390`/`FUN_140220420`, `FUN_140159970`/`FUN_140159a40`
    (used by p9-03 and p9-05).
  * p9-02: all GEOPRIM accessors in 0x1402b7400-0x1402f2fff that are not in p9-04/p9-05.
* **Phase 6-7 functions** called from here (port them there, call them from here):
  `FUN_14021d2f0` rotate and `FUN_14021d0b0` translate a 3D object are p67-1's (`defcmd/objpos.hpp`;
  used by p9-01, p9-03, p9-04, p10-02). The geometry kernels (`def::rt::geoutl::fun_<addr>`,
  `defrt/ftn/geoutl.hpp`) are owned by claude/project-thread-6cvah9 (body of `1404d41f8` by
  p67-3); only `1404cccfb` (quad normal) is called from here (p8-06). The other Phase 6-7 callees,
  by packet, belong to whichever p67 packet lists them in its status file:
  p8-02 `1401b06e0 1401b83d0 1401fc730 1402009f0 14039d450 1404050b0 140405150 140405540 1404056c0 140405720 1404057a0`;
  p8-06 `1401b83d0 1401fc730 140225640 140225820 140225ff0 14039d450 1403ff870`;
  p8-09 `1402009f0 1404050b0 140405150 140405540 1404056c0 1404056e0 140405720 1404057a0`;
  p8-01/p8-03/p8-05/p8-07 `140158ca0 14019a620 1401dd0d0 1401fc730 140202a20 140268290`;
  p9-01..p9-05 `140159eb0 14015ab80 1401fc730 1401599b0 1401d6d90 1402009f0 140220440 140220460
  140220480 1403de680 1403de6e0 140202a20 140206fd0 1402084c0 14023ace0 1402fc1b0 140308a90`;
  p10-02 `1401a0a10 1401a6c80` (plus the two p67-1 functions).
* **Already ported, reuse:** `FL3_FNDBND` `FUN_140354ed0` (kw-g2geo,
  `port/defmodel/src/geo_kw/boundary_fl3.cpp`, verified) for p8-02; the DYNARY family
  `FUN_14012eff0`/`FUN_14012fad0`/`FUN_14012ff20` (key-common / kw merge) for p8-08, p9-01, and
  `FUN_14012fa90` (a one-line DYNARY test, not in the ledger) for p10-01/02.
* **Functions in the licence address range** (0x140090000-0x14012ffff) that these packets call,
  outside the ledger: CRT/STL instantiations the linker placed there (`strstr` `14009e3c0`,
  `std::length_error` ctor `14009b490`, `memchr` wrapper `1400ba8c0`, `std::string::find`
  `1400bc080`, `operator==` `1400bd310`, list/alloc helpers `1400bb460 1400bb470 1400bb790
  1400bc250 1400bd290 1400bf650`) → use the C++ standard library; `FUN_1400b9130` (write a string
  to a file, used by p8-06/07/08) → p8-07 ports it. **Excluded:** `FUN_14009e330`,
  `FUN_14009e340`, `FUN_14009e350`, called by `main` right after the licence set-up; they sit in
  the licence code and are not analysed; the port omits them (p5-01).

## Licence, Sentinel and process monitor

No function of these packets is licence, Sentinel or process-monitor code. Checked: callees and
strings of all 1 237 functions; the only calls into the licence range are the ones listed above.
In p5-01, `main` (`FUN_14001c9f0`) calls the licence session set-up (not ported) and the three
excluded stubs. `FUN_140224d80` (with `140224e70`, `140224f20`, `140225310`, `140225330`,
`1402253d0`, `1401b9020`) only stores the program name "TEXT_PRE", the version string (plus the
service pack read from `<DEFORM_DIR>/PACK/pack.dat`), a number and "DEFORM-23" in globals; port
them as plain setters and do not follow the readers of those globals.

## Not needed: the shape-rolling extrude (143 functions)

`FUN_1402baa60` (extrude primitive) runs `FUN_1402bbb10` when `prim+0x23a4 != 0`, and that calls
`FUN_1402bd210` only when `prim+0x23a4 == 1`. In the batch scope `+0x23a4` is written only by the
setup functions `FUN_1402b8520`/`FUN_1402b8980` (from their third argument) and set to 0 by
`FUN_1402b7ad0`/`FUN_1402b81a0`. Their three callers pass constants (disassembly):
`FUN_140153c00` and `FUN_140147f90` pass `EDX=1, R8D=0`, `FUN_140154270` passes `EDX=0, R8D=2`.
The functions that write 1 (`FUN_1402b94a0`, `FUN_1402dc7f0`, `FUN_1402ddbe0`, `FUN_1402df740`,
`FUN_1402e13e0`) and the other caller of `FUN_1402bd210` (`FUN_1402ef000`) are outside the batch
scope, and nothing takes their address. So the subtree that `FUN_1402bd210` dominates (143 P9
functions, 97 KB, listed at the end) is dead in batch: mark it `not-needed` (done in `docs/re/status/p9-shape-roll.tsv`), and p9-04 stubs the
call with an assertion.

## Open items, as analysis tasks

| Task | Packet | What |
|---|---|---|
| T5-1 | p5-01 | Write scripted oracle cases for what no case covers yet: DEF_MULTI.INI start-up (a corpus `cmd/…INI` file saved as DEF_MULTI.INI, no stdin), DEF_PRE.INI and DEF_PRE3.INI start-up, menu 1 problem id, menu 8 save with the overwrite prompt (Y and N), invalid selection, `R`, `/`, the 20-trial and 100-iteration limits, exit with `N` at the confirm. |
| T5-2 | p5-01 | Menu 2 options 3-8 launch `DEF_CVT3` with dirKind 0, which this install never finds; port the launch exactly and keep the cases as "fails the same way". Selection range lo/hi still unknown (FILE_INPUT_MENU.md). |
| T8-1 | p8-01 | `PDA2KEY3` hangs without `PDA3.INI`; give the `.PDA` case a `PDA3.INI` or keep it out of tier 2. |
| T8-2 | p8-03 | Fortran `Ew.0` input rounding: compare the runtime's conversion with `strtod` on long mantissas and `D` exponents with the p1-01 binary-routine check harness; fix in `defrt` if they differ. |
| T8-3 | p8-04 | DXF user tolerance `obj+0x3a50/0x3a58`: **closed here.** A full-program search for `[reg+0x3a50]`/`[reg+0x3a58]` finds writes only in the object constructor `FUN_14015f0b0` (0 and a double) inside this object family; the other hits are in unrelated objects at 0x14008…/0x14009…. Port the constructor value and the readers `FUN_140409910`/`FUN_140409930`; no setter exists. |
| T8-4 | p8-05 | BRDEXT qsort comparator `FUN_14013ca60` (now placed in p8-05) and the 2D GEO topology `FUN_140249700` (p8-09) are still "open" in BRDEXT.md; specify both from the listing before porting. |
| T8-5 | p8-06 | No case reaches `CHKVOL` (needs `CHK_VOL.INI` with TRGVOL > 0) or the coating mesh (`obj+0x2de8 > 0`); write one case each. |
| T8-6 | p8-08 | DEFFRZ: meaning of the double argument and the 7 read-back numbers (DEFFRZ.md). 3D interpolation (`DEF_REM3_GUI`) fails on this PC (32-bit `mpich.dll` missing): port and original fail the same way; verify 3D only on a PC where it runs. |
| T9-1 | p9-03 (owner p67-1) | The extrude's +90° rotation multiplies by 6.1232011757e-17 rather than cos(fl(π/2)): follow the node path behind `vtbl+0x40` in `FUN_1402002b0` (called from `FUN_14021d2f0` → `FUN_140221cd0`) and the argument reduction in `FUN_1404b3b30`; the fix lands in p67-1's rotate, p9-03 holds the test. |
| T9-2 | p9-05 | GEOPRIM revolve internals, not decompiled yet: `FUN_1402ce290` (mesh-boundary sweep), `FUN_1402d9bb0` (axis class), `FUN_1402caac0`/`cac30`/`caf40` (non-uniform section steps), `FUN_1402db310`/`dbb20` (strips), `FUN_14041f830` (cap triangulation). Spec them before porting (CNVT3D_SWEEP §14). |
| T9-3 | p9-04 | Extrude primitive `FUN_1402baa60`: resolved to two live paths, geometry `FUN_1402bb460` (`+0x23a0`) and mesh boundary `FUN_1402bbb60` (`+0x23a4 == 2`); spec both. Also the face-split list `local_180` in `FUN_140154270` that the decompile never fills (CNVT3D_SWEEP §10.3, p9-03). |
| T9-4 | p9-03 | WPAXIS entry 1 prints `0 0 0 0` for a partial revolve where CNVT3D.md §6.4 expects angle/start; settle against the KEY writer. Stale NBCD.OUT after a revolve-then-extrude and a failed remesh inside the sweep were never run; add cases. |
| T10-1 | p10-03 | Confirm that no batch path prints the registry column-3 labels: the only accessor `FUN_1402f7a70` has no caller and no reference. Then mark the 49 handlers `not-needed` (default); port them only if a scripted menu session shows a label. |
| T10-2 | p10-01, p10-02 | Menus 3-6 have no spec and no cases: record scripted sessions (stdin through the menus, then menu 8 save) per form, as the oracle tests. |

## New cases still to write (not in `tests/commands`)

* p5-01: T5-1.
* p8-06: T8-5. p9-03: T9-4.
* p10-01/02: T10-2, at least one session per form (simulation controls, material, object,
  inter-object), each ending with a KEY save so the comparison has a file.

## Function lists

### p5-01 menu shell and batch drivers (25)
0x140002a90 0x140006420 0x140007d30 0x140009f20 0x14000a200 0x14000ac60 0x14000af30 0x14000afd0 0x14000b170 0x14000bac0 0x140014ba0 0x14001ad90 0x14001c770 0x14001c9f0 0x14001ce7b 0x14001d1bf 0x1401b8e10 0x1401b9020 0x1401d8000 0x140224d80 0x140224e70 0x140224f20 0x140225310 0x140225330 0x1402253d0

### p8-01 GFREAD driver and small commands (71)
0x140197bc0 0x14019a650 0x14019ab80 0x1401aa750 0x1401aa9a0 0x1401ac020 0x1401ac310 0x1401ac680 0x1401b0710 0x1401b0740 0x1401b0760 0x1401b0790 0x1401b0810 0x1401b82b0 0x1401d6db0 0x1401fad60 0x140201bc0 0x140243340 0x140243360 0x140243370 0x140243500 0x1402437c0 0x140243810 0x140243830 0x140244010 0x140244200 0x1402442f0 0x140244500 0x140244e30 0x140256670 0x140256690 0x1402566b0 0x1402566c0 0x140331790 0x140333340 0x140382510 0x140382580 0x1403825a0 0x140382660 0x1403826a0 0x1403827f0 0x140382890 0x1403828d0 0x1403829a0 0x140382ab0 0x140382b60 0x140382be0 0x140382cd0 0x140382d00 0x140382e80 0x140383000 0x140383020 0x140383060 0x140383080 0x1404092b0 0x140409330 0x140409370 0x1404098d0 0x1404098f0 0x140409910 0x140409930 0x1404099d0 0x140409a50 0x140409a90 0x14040cde0 0x14040ce20 0x14040ce40 0x14040ce60 0x140454900 0x140454980 0x140454a90

### p8-02 STL and NASTRAN readers (74)
0x1401fffa0 0x140222b60 0x140222b80 0x140243a70 0x140244e60 0x140244ea0 0x140244f00 0x140244f20 0x140244fa0 0x140245b90 0x140245cc0 0x140245ee0 0x140246980 0x1402469a0 0x140246ad0 0x14024c170 0x14024c1d0 0x14024c1f0 0x14024c270 0x14024c290 0x14024c4b0 0x14024d4f0 0x14024d650 0x14024d790 0x14024d7f0 0x14024d810 0x14024d890 0x14024d8b0 0x14024dad0 0x14024ee10 0x14024ef70 0x14024f0a0 0x14024f110 0x14024f160 0x14024f1b0 0x14024f1d0 0x14024f270 0x14024f620 0x14024f670 0x14024f6a0 0x14024fa50 0x14024fe90 0x140250280 0x140251be0 0x140251d60 0x1404051b0 0x1404053b0 0x140405700 0x140405800 0x14040ce80 0x14040cf80 0x14040d270 0x14040d750 0x14040dc00 0x14040dfd0 0x14040e980 0x14040f4e0 0x14040f9f0 0x14040fa70 0x14040ff60 0x140410130 0x140410260 0x140410750 0x140410760 0x140410d40 0x140411360 0x140411640 0x1404116a0 0x140411850 0x140411880 0x140459c70 0x140459d50 0x140459fb0 0x140459fe0

### p8-03 IGES reader (77)
0x140197d00 0x140197d40 0x14019abd0 0x14019abf0 0x1401b8420 0x1401b8530 0x1401b9100 0x140331250 0x140409af0 0x14040b100 0x14040c220 0x14040c740 0x14040cd30 0x14040cd90 0x14040cda0 0x14040ce00 0x140454b50 0x140454bc0 0x140454d00 0x140454da0 0x140454e00 0x140454e40 0x140454ee0 0x140455150 0x1404553c0 0x1404557a0 0x140455aa0 0x140455e60 0x1404564c0 0x1404565c0 0x140456720 0x140456770 0x140456790 0x1404567a0 0x1404570f0 0x140457196 0x140457312 0x1404579fc 0x1404587d1 0x140458e6f 0x140459149 0x1404594a3 0x140459607 0x14045979e 0x140459c0d 0x140460f50 0x140461b95 0x140462631 0x1404627bf 0x140462b00 0x140462e15 0x140463267 0x140463374 0x1404635be 0x1404639eb 0x140463e15 0x14046414e 0x140464245 0x1404645b0 0x140464720 0x1404648ff 0x140464abc 0x14046519b 0x140465bb1 0x1404665f3 0x1404668e4 0x140467142 0x1404672b2 0x14046736c 0x140467473 0x140467e54 0x1404681dc 0x140468677 0x140468960 0x14046a810 0x14046aa40 0x14046b2d0

### p8-04 DXF reader (61)
0x140277420 0x14027ba50 0x1403078f0 0x1403fb460 0x1404093a0 0x140409880 0x140409890 0x140409950 0x140409970 0x140409990 0x1404099a0 0x1404099c0 0x140409ae0 0x14040ba80 0x140422ab0 0x140422d40 0x140422d90 0x140422f10 0x140422f70 0x1404231c0 0x140424390 0x140424400 0x140424450 0x140424510 0x140424560 0x1404245b0 0x1404246c0 0x140424700 0x140424b90 0x140424c80 0x140424da0 0x1404250f0 0x1404251e0 0x1404258d0 0x1404259c0 0x140425cf0 0x140425d50 0x1404261f0 0x1404262e0 0x140426810 0x140427dd0 0x140427df0 0x140427ec0 0x140427f00 0x140427f90 0x140428030 0x140428140 0x140428510 0x1404285b0 0x140428650 0x140428850 0x1404288c0 0x1404289a0 0x140428a10 0x140428b10 0x140428b80 0x140428cf0 0x140428e70 0x140428eb0 0x140428ef0 0x140428f70

### p8-05 BRDEXT (76)
0x140006700 0x14008c3f0 0x14013ca60 0x14013cc10 0x14013d230 0x14013d8d0 0x14013d8f0 0x14013d900 0x14013dba0 0x14013e230 0x14013eb80 0x140158c10 0x140158d40 0x140158e20 0x140158e40 0x140159230 0x140159c80 0x140159cd0 0x140159d00 0x140159d30 0x140159db0 0x14015a040 0x14015a0f0 0x14015a190 0x14015a4b0 0x14015a4d0 0x14015a500 0x14015a560 0x14015a590 0x14015a7c0 0x14015ac80 0x14015acd0 0x14015ad40 0x14015e8e0 0x14015e900 0x14015e9f0 0x14015eb50 0x14015eb70 0x1401a9ac0 0x1401b15a0 0x1401b1630 0x1401b1770 0x1401b17f0 0x1401b3490 0x1401b40a0 0x1401b40f0 0x1401b4120 0x1401b4200 0x1401b47b0 0x1401b47e0 0x1401b4e70 0x1401b4eb0 0x1401b4f30 0x1401b5070 0x1401b50a0 0x1401b5160 0x1401b5180 0x1401b51a0 0x1401b5350 0x1401b53c0 0x1401b5400 0x1401b5450 0x1401b5920 0x1401b5990 0x1401b5a10 0x1401b5a30 0x1401b5a80 0x1401b5aa0 0x1401b5ac0 0x1401b5b60 0x1401b5c50 0x1401b5c70 0x1401b5cd0 0x1401b5d00 0x1401b5d30 0x1401b5ef0

### p8-06 3D solid mesh chain (55)
0x140140110 0x140140310 0x1401403c0 0x140141d50 0x140142680 0x1401427e0 0x140142f10 0x140143900 0x1401445f0 0x140159260 0x140232fb0 0x140233000 0x140238173 0x140239da0 0x14023e450 0x140242b50 0x14026a4f0 0x14026a95c 0x14026b256 0x14026c50f 0x14026d0b0 0x14026d7d0 0x14026fa60 0x14026fac0 0x14026fbc0 0x14026fcd0 0x14026fdf0 0x140270360 0x1402703a0 0x140270600 0x140270b80 0x140270d60 0x1402710f0 0x140271250 0x140271820 0x140271930 0x140271a20 0x140271ab0 0x140271b40 0x1402722d0 0x140272480 0x1402725c0 0x140272620 0x1402729c0 0x140272b00 0x140273150 0x140273220 0x140273350 0x140273440 0x1402735e0 0x140273710 0x140273fb0 0x140274210 0x140274830 0x14041dcb5

### p8-07 DEFAMG and mesher input files (64)
0x14013e790 0x14013e840 0x14013ee00 0x14013efb0 0x14013f010 0x14013f410 0x14013f8f0 0x140141580 0x140142240 0x1401438e0 0x140143920 0x140144200 0x1401443c0 0x1401444f0 0x140144620 0x140144810 0x140144a00 0x140144a70 0x140144de0 0x140145480 0x1401455b0 0x140145810 0x1401458d0 0x140145cc0 0x140145ce0 0x140148840 0x140154f20 0x140155340 0x140155600 0x1401559d0 0x1401574d0 0x140158d50 0x140158d70 0x140158d80 0x140158d90 0x140158da0 0x140158db0 0x140158dc0 0x140158dd0 0x140158de0 0x140158ea0 0x140198520 0x14019d420 0x1401a9be0 0x1401b06f0 0x140200560 0x1402062b0 0x14020ae70 0x14020af50 0x14020b0f0 0x14020b1e0 0x14020b260 0x14020b280 0x1402395d0 0x1402489e0 0x140248bf0 0x140248fb0 0x14024ae90 0x14024b600 0x14025c910 0x140268390 0x140268590 0x14027f140 0x14029c3c0

### p8-08 DEFINT, DEFFRZ, REMESH (62)
0x1400010c0 0x14013e880 0x14013e960 0x14013ea10 0x140155c70 0x140155c90 0x1401562f0 0x1401563c0 0x140156640 0x140156950 0x140156ba0 0x140156cd0 0x140157040 0x1401573c0 0x1401575a0 0x140157610 0x140157670 0x1401576a0 0x140157780 0x1401577e0 0x140157830 0x140157890 0x140157be0 0x140159200 0x1401592b0 0x140159300 0x140159dd0 0x140180a40 0x1401a9a10 0x1401aa000 0x1401aa370 0x1401b1520 0x1401b17b0 0x1401d7320 0x1401d7390 0x1401e8040 0x1401e84f0 0x1401e87f0 0x1401e90a0 0x1401e9950 0x1401ea900 0x1401eaf80 0x1401f3660 0x14028ae00 0x14028af60 0x14028d370 0x14029b630 0x14029d010 0x14029d5b0 0x14029d640 0x14029d7a0 0x14029d8f0 0x14029d970 0x14029db80 0x14029dbe0 0x14029dd70 0x14029de40 0x14029de70 0x14029dea0 0x1403357a0 0x140339d30 0x140375550

### p8-09 geometry support library (47)
0x140006730 0x140158fc0 0x140198270 0x1401ffd80 0x140202700 0x1402027e0 0x1402029e0 0x14023a380 0x140246d00 0x140246d40 0x140246d70 0x140246dd0 0x140246e10 0x140246e40 0x140246ef0 0x140246f70 0x1402478f0 0x140248390 0x1402484c0 0x1402488a0 0x140249700 0x14024ae10 0x14024bac0 0x14024bb30 0x14024be40 0x1402566d0 0x1402567e0 0x140256800 0x140256950 0x1402569e0 0x140257080 0x1402570e0 0x14040e620 0x14040e810 0x14040fde0 0x140410fd0 0x140411080 0x140411300 0x1404113b0 0x1404115d0 0x140411620 0x140411920 0x140411940 0x140411970 0x140411a60 0x140411ad0 0x140459d00

### p9-01 CNVT3D driver (42)
0x140007660 0x14013c320 0x14013c6b0 0x14013c970 0x14013d760 0x14013d790 0x1401467f0 0x140147f90 0x140148750 0x140159000 0x140159970 0x140159a40 0x140159f30 0x140189110 0x140198d30 0x1401a91d0 0x1401b1970 0x1401e89c0 0x1401e91d0 0x1401e9c50 0x1401eb220 0x1401f36f0 0x1401fa150 0x140205e40 0x14021d4c0 0x14021d630 0x14021ef80 0x14021f160 0x14021f760 0x140220390 0x140220420 0x140259170 0x14025a270 0x14025a360 0x140266860 0x14027cc00 0x14027ce90 0x14027ece0 0x14027efa0 0x14027f450 0x14029b6c0 0x140339dc0

### p9-02 GEOPRIM object (85)
0x1402b7ad0 0x1402b8090 0x1402b81a0 0x1402b84e0 0x1402b8520 0x1402b8980 0x1402dd0d0 0x1402e2840 0x1402e2960 0x1402e2a50 0x1402e2b10 0x1402e2b50 0x1402e2c00 0x1402e3820 0x1402e3860 0x1402e3880 0x1402e3f20 0x1402e3f50 0x1402e3f70 0x1402e3fa0 0x1402e4030 0x1402e4080 0x1402e9040 0x1402e9070 0x1402e9090 0x1402e90c0 0x1402e9110 0x1402e9180 0x1402e9240 0x1402e9270 0x1402e9290 0x1402e92c0 0x1402e9370 0x1402e94d0 0x1402e9500 0x1402e9aa0 0x1402f0620 0x1402f0660 0x1402f06a0 0x1402f06e0 0x1402f0720 0x1402f0760 0x1402f0890 0x1402f0940 0x1402f0a40 0x1402f0a80 0x1402f0ab0 0x1402f0b20 0x1402f0b40 0x1402f0b60 0x1402f0ba0 0x1402f0c40 0x1402f0c70 0x1402f0ce0 0x1402f0e10 0x1402f0e80 0x1402f0fd0 0x1402f1040 0x1402f13c0 0x1402f13f0 0x1402f1560 0x1402f15b0 0x1402f1610 0x1402f1630 0x1402f1680 0x1402f16d0 0x1402f1760 0x1402f17b0 0x1402f1860 0x1402f18b0 0x1402f19b0 0x1402f19f0 0x1402f1a30 0x1402f1a70 0x1402f1ab0 0x1402f1b80 0x1402f1bb0 0x1402f1c00 0x1402f1c60 0x1402f1cb0 0x1402f1d80 0x1402f2240 0x1402f2490 0x1402f2580 0x1402f2ce0

### p9-03 CNVT3D mesh build and sweep (69)
0x14013c340 0x14013d7c0 0x14013d7e0 0x140146050 0x140146560 0x140147b70 0x140148960 0x140148a20 0x140148f70 0x14014c0a0 0x14014c730 0x14014d730 0x14014fae0 0x140151400 0x140153710 0x140153910 0x140153c00 0x140154270 0x140159030 0x140159050 0x140159090 0x1401590d0 0x1401590f0 0x140159100 0x140159110 0x140159150 0x1401591b0 0x1401595b0 0x14015a260 0x140161fd0 0x1401627e0 0x140180ac0 0x140180b40 0x140180bc0 0x140180d30 0x140180db0 0x140180e30 0x140180eb0 0x140181010 0x140181110 0x140181220 0x1401813a0 0x140184270 0x140184370 0x140184400 0x140184490 0x140184760 0x1401847f0 0x140184880 0x140184910 0x1401cbc20 0x1401cbce0 0x1401cbd40 0x1401cbd90 0x1401d1660 0x1401d72e0 0x140212460 0x14023cdb0 0x14023d410 0x1402778c0 0x1402a7b60 0x1402ac8a0 0x14041e1f0 0x14041e270 0x14041e4d0 0x14041ea80 0x14041f0f0 0x14041f100 0x14045a040

### p9-04 GEOPRIM builder and extrude (92)
0x140220780 0x1402ba500 0x1402ba850 0x1402baa60 0x1402bb460 0x1402bbb10 0x1402bbb60 0x1402bc580 0x1402bc720 0x1402bc980 0x1402c1130 0x1402caac0 0x1402cc080 0x1402dbe70 0x1402dc040 0x1402dc560 0x1402e24f0 0x1402e7f40 0x1402e91d0 0x1402e9470 0x1402e9b50 0x1402e9b70 0x1402e9b90 0x1402e9bb0 0x1402e9bd0 0x1402f07e0 0x1402f0930 0x140308120 0x140308340 0x140309960 0x140420750 0x1404207f0 0x140420840 0x140420890 0x140420a30 0x140421060 0x140421120 0x1404211a0 0x140421210 0x1404213c0 0x140421510 0x140421810 0x1404218a0 0x140421920 0x140421970 0x1404219e0 0x140421a00 0x140421a40 0x140421b00 0x140421b40 0x140421b70 0x140421be0 0x140421c00 0x140421c40 0x140421c80 0x140421ca0 0x140421cf0 0x140421d10 0x140421d50 0x140421d70 0x140421dc0 0x140421de0 0x140421e00 0x140421e30 0x140421e60 0x140421e90 0x140421eb0 0x140421ed0 0x140421f20 0x140421f80 0x140421fd0 0x140422030 0x140422070 0x1404220b0 0x1404220e0 0x140422110 0x140422140 0x1404221c0 0x140422250 0x1404222d0 0x140422360 0x140422410 0x1404224e0 0x1404225d0 0x140422610 0x140422630 0x140422660 0x140422690 0x140422780 0x140422800 0x140422980 0x140422a40

### p9-05 GEOPRIM revolve (69)
0x140205510 0x140205620 0x14023dce0 0x140258140 0x1402cac30 0x1402caf40 0x1402ce190 0x1402ce290 0x1402ce310 0x1402d0470 0x1402d0a30 0x1402d0d10 0x1402d0e50 0x1402d20c0 0x1402d2720 0x1402d2920 0x1402d4420 0x1402d6bf0 0x1402d7220 0x1402d7cd0 0x1402d9bb0 0x1402da250 0x1402da9f0 0x1402db0d0 0x1402db310 0x1402dbb20 0x1402dc160 0x1402e9dc0 0x1402eaba0 0x1402eb330 0x1403086e0 0x140308ad0 0x14041f830 0x14041fc50 0x14045a270 0x14045a300 0x14045a340 0x14045a5c0 0x14045a670 0x14045a690 0x14045b630 0x14045b900 0x14045b920 0x14045bab0 0x14045bc60 0x14045c030 0x14045c4e0 0x14045c510 0x14045c530 0x14045c650 0x14045c670 0x14045c6b0 0x14045c740 0x14045c7e0 0x14045c870 0x14045c900 0x14045c990 0x14045ca20 0x14045cce0 0x14045cfa0 0x14045d0f0 0x14045d840 0x14045d8a0 0x14045d950 0x14045daa0 0x14045dbe0 0x140460920 0x140460990 0x140460e80

### p10-01 edit menus 3-4 and form helpers (32)
0x1400016b0 0x1400016d0 0x1400017d0 0x1400017f0 0x1400018e0 0x140001ae0 0x140001c00 0x140001d20 0x140001e40 0x140003520 0x140003880 0x140005520 0x140007a80 0x140008030 0x140008220 0x14000bc80 0x14000ccf0 0x14000ce70 0x14000d670 0x14000de20 0x14000f340 0x14000f850 0x14000fd10 0x1400150f0 0x1400157c0 0x140017540 0x14019cfb0 0x1401b08d0 0x1401b1240 0x1401b1450 0x1401ca9a0 0x1401ca9c0

### p10-02 edit menus 5-6 (44)
0x140001690 0x1400020c0 0x140002450 0x140002510 0x140002690 0x140002750 0x140002770 0x1400030d0 0x140003f50 0x140004510 0x140004700 0x140004870 0x140004a90 0x140006bd0 0x140006d10 0x140008a00 0x140009100 0x140010a90 0x140011cd0 0x140012110 0x140012340 0x140012570 0x1400126f0 0x140012870 0x140013790 0x1400145d0 0x140014780 0x1400149e0 0x140015e90 0x1400162e0 0x140016c50 0x1400170b0 0x1400179f0 0x140019c20 0x14001a050 0x14019d5a0 0x1401ca9e0 0x1401caa00 0x1401caae0 0x1401cab10 0x1401cab30 0x1401cac80 0x1401cad40 0x1401cae00

### p10-03 registry label handlers (check, default not-needed) (49)
0x1402ae950 0x1402aeda0 0x1402af1f0 0x1402afa70 0x1402afcd0 0x1402b0970 0x1402b0ef0 0x1402b1360 0x1402b3da0 0x1402b4180 0x1403cac80 0x1403cb300 0x1403cb520 0x1403cb740 0x1403cc550 0x1403cc690 0x1403cc970 0x1403ccad0 0x1403cdb20 0x1403cf0f0 0x1403cf220 0x1403cf360 0x1403cf530 0x1403cf670 0x1403cf7a0 0x1403cfbf0 0x1403cff00 0x1403d01f0 0x1403d0320 0x1403d0480 0x1403d05f0 0x1403d0720 0x1403d0850 0x1403d0980 0x1403d0c00 0x1403d1c50 0x1403d1f50 0x1403d20e0 0x1403d2210 0x1403d2a00 0x1403d2f20 0x1403d31f0 0x1403d34c0 0x1403d3790 0x1403d6ec0 0x1403d7050 0x1403d71e0 0x1403d7530 0x1403d7e80

### nn-shape-roll shape-roll extrude (not-needed) (143)
0x140166090 0x140203d80 0x140206570 0x140242950 0x1402bd210 0x1402bd580 0x1402be430 0x1402be480 0x1402be800 0x1402be830 0x1402be910 0x1402bf820 0x1402bfcc0 0x1402c1690 0x1402c17c0 0x1402c2160 0x1402c2180 0x1402c23b0 0x1402c3300 0x1402c52f0 0x1402c5400 0x1402c5940 0x1402c6570 0x1402c7550 0x1402c7610 0x1402c79e0 0x1402c7e00 0x1402c81d0 0x1402c98c0 0x1402c9960 0x1402c99a0 0x1402c99f0 0x1402c9a90 0x1402c9bd0 0x1402c9d20 0x1402c9ee0 0x1402ca0d0 0x1402ca2a0 0x1402ca6b0 0x1402cb1c0 0x1402cbf60 0x1402cc370 0x1402cc450 0x1402cc530 0x1402cc590 0x1402cc6e0 0x1402ccb60 0x1402cdce0 0x1402e3040 0x1402e3180 0x1402e32d0 0x1402e3330 0x1402e3490 0x1402e3620 0x1402e3700 0x1402e3a30 0x1402e3d60 0x1402e4110 0x1402e8540 0x1402e9570 0x1402e9bf0 0x1402e9c30 0x1402eb340 0x1402ebb50 0x1402ec1e0 0x1402ecd50 0x1402ecf30 0x1402ed270 0x1402ed6a0 0x1402edb20 0x1402edd10 0x1402eded0 0x1402ee080 0x1402ee420 0x1402f0800 0x1402f0820 0x1402f0840 0x1402f0860 0x1402f08b0 0x1402f08e0 0x1402f0900 0x1402f09d0 0x1402f09f0 0x1402f0d00 0x1402f0d20 0x1402f0d60 0x1402f0ea0 0x1402f0ee0 0x1402f0fa0 0x1402f1060 0x1402f1080 0x1402f10c0 0x1402f1160 0x1402f1190 0x1402f11c0 0x1402f1420 0x1402f1450 0x1402f1480 0x1402f14a0 0x1402f14d0 0x1402f1500 0x1402f1530 0x1402f1730 0x1402f1810 0x1402f1910 0x1402f1930 0x1402f1c30 0x1402f1d40 0x1402f1e00 0x1402f1e80 0x1402f1f00 0x1402f20b0 0x1402f2110 0x1402f21f0 0x1402f22d0 0x1402f2370 0x1402f2400 0x1402f2600 0x1402f2780 0x1402f28c0 0x1402f29b0 0x1402f2a30 0x1402f2ba0 0x1402f2d30 0x1402f2da0 0x1402f2e10 0x140308240 0x140308c00 0x140309310 0x1403e1860 0x1403e1ab0 0x140405410 0x14041fd70 0x14041fdf0 0x14041fe70 0x140420600 0x1404206e0 0x140420710 0x140428f90 0x1404293dc 0x140429cc2 0x14042a7cd 0x14043797e
