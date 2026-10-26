# Null-default hook audit (kwm-audit)

Scope: `port/defmodel`, `port/defkey`, `port/defdbio`, `port/defcmd` (defcmd holds only its module
skeleton, so it has no hooks). Searched for function-pointer globals with a null default
(`inline ... (*g_...)(...) = nullptr`), hook structs, `ext::` stub layers, `__has_include`
fallbacks, `DEF_NOT_PORTED(0x...)` and `def::rt::not_ported(...)`.

Status values:

* **wired**: with no test hook installed, the program build runs the owner's port. Tests may
  still override it.
* **owner missing**: no port of the target exists in the tree. The default still stops in
  DEF_NOT_PORTED / not_ported.
* **intentionally null**: the default is meant to be empty or to stop, for the reason given.

## Hooks and ext layers

| address | hook / stub location | owner packet | status |
|---|---|---|---|
| 1402f9b60 | `llobj::ext::call` switch (defmodel/src/llobj/ext.cpp), from `llobj_copy::geo3d_assign` 140199c20 | p1-07 `OBJ_GEO3D::assign` (src/geo/obj_geo3d.cpp) | wired (kwm-audit); test_llobj_copy_wired |
| 1402f9dc0 (vslot 4, pseudo 0x4) | `llobj_copy::ext::geo3d_slot4` (src/llobj_ctor/llobj_copy.cpp) | p1-07 `OBJ_GEO3D::copy_from` -> assign | wired: virtual call when no hook |
| 1402f9340 | `llobj::ext::call` | p1-07 `OBJ_GEO3D::clear` | wired |
| 1402f91f0 | `llobj::ext::call` | p1-07 `~OBJ_GEO3D` | wired |
| 1400034d0 / 140003500 | `llobj::ext::call` (llobj_copy.hpp f1400034d0 / f140003500) | p1-07 `node_count` / `elem_count` | wired (kwm-audit) |
| 140257c80 | `llobj::ext::call` and `pending::fun_140257c80` | p1-06 `Cls_140257340List::copy_from` | wired (kwm-audit for llobj::ext; pending.hpp at the Phase 1 gate) |
| 140280490 | `llobj::ext::call` | p1-08 `GEO2D_AND_MESH2D::copy_from` | wired (kwm-audit) |
| 140302090 | `llobj::ext::call` (`list_detach`) | p1-05 `CowList::detach` on geo_kw::GeoList (LLOBJ_DATA+0x3998) | wired (kwm-audit; also reached by p10-01 through 1401d0fc0) |
| 1403020c0 | `llobj::ext::call` (`iter_ctor`) | none: 2-instruction iterator ctor, ported in place with `@src` | wired (kwm-audit) |
| 140301e30 / 140302020 / 140302040 (+140301fc0) | `llobj::ext::call` (`list_end`, `iter_value`, `iter_post_inc`) | std::list iterator helpers of the +0x3998 list, ported in place | wired (kwm-audit) |
| 14019a670 / 140301d10 / 140301ad0 | `llobj::ext::call` | kw-g2geo geo_kw (delete / clear / append default) | wired |
| 14012f480, 140257c60, 1402581b0, 1402598a0, 140259c70, 14025ab20, 14025ae70, 140269320, 1402699c0, 140283f80 | `llobj::ext::call` | p1-04 / p1-06 / kw-g3 / p1-08 | wired |
| 140329c70..140329ef0, 14032a990..14032ceb0 | `llobj::ext::call` | p1-10 Cls_140329c70, p1-09 StateVar subclasses | wired |
| 14012d050, 14012f410, 14019a290, 14019abb0, pseudo 0x1 / 0x2 | `llobj::ext` named wrappers (new_dynary, dynary_dtor, llobj_delete, sdctl_clear, vdelete, vslot1_0) | key-common / p1-04 / p1-05 | wired: owner called directly when no hook |
| 14039d3a0 | `llobj_copy::ext::f14039d3a0` -> `llobj::ext::call` (no case) | kw_group_handlers_dds (Phase 2 message printf, through 14039d570) | owner missing; reached only when calloc fails in 140256dc0 / 140257150 |
| 1401d0fc0 | `g_hook_1401d0fc0` / `call_1401d0fc0` (defmodel/step/name_arrays.hpp) | kw-g4b `step_remap_material_state` | wired for every caller: kw-g1b name_arrays.cpp (UNNAME/UENAME), kw-g1a (GEOTYP/STNOUT/ELMNOD through `ModelHooks::rebuild_state_layout`), kw-g57 |
| 1401d48d0 | `g_hook_1401d48d0` (defmodel/statevar_kw/statevar_kw.hpp), from kw-g2sv `mark_db_var` | kw-g1b step names (`mark_state_var_outputs`, src/step_names/name_arrays.cpp) | wired (kwm-audit): ported from the disassembly; it returns when 1401d2b70 is true, which is always, so the body after that is unreachable (DEF_NOT_PORTED there). Before this, a null hook skipped the call, which has the same effect |
| 140235c10 | `g1a::ModelHooks::set_unit_system` (defkey/kw_g1a.hpp) | p1-12 `def::rt::ftn::set_unit_system` | wired (null hook -> real function) |
| 140235c10 | `llobj_ctor::unit_system` (src/llobj_ctor/obj_defaults.cpp) | p1-12 `def::rt::ftn::unit_system` | wired; the `__has_include` fallback "return 1" was removed (kwm-audit) |
| 1403c4504 | `g_stop_1403c4504` (defmodel/globals_p1_09.hpp), from 14032ac75 "TOO LARGE SWEEPING ANGLE" | none | **excluded (licence code, SFTC/user rule)**: the licence/process-monitor routine is never ported, analysed or wired; the null default stays (def::rt::not_ported stops the run on that error path) |
| (fault) | `dbcore::set_runtime_fault_hook` (defdbio/src/dbcore/ftn_io.cpp) | defdbio | intentionally null: the default runs the real behaviour (print, end the process with the status); tests install a throwing hook |
| pending.hpp list (p1-07 calls) | `DEF_PENDING` pointers (defmodel/geo/pending.hpp) | p1-04/06/08/09/10/11/13/14 | wired: every default is `&wired::<name>` (src/geo/pending_wiring.cpp) |
| members/ext_deps.hpp list | `def::model::ext::fun_*` (src/members/ext_deps_wiring.cpp) | p1-04/05/09/11/12, key-common | wired (the header comment that says "DEF_NOT_PORTED stubs" is out of date) |
| step/ext.hpp (defmodel) | `def::model::ext::fun_*` (src/step/ext_p1_04/06/08/10.cpp) | p1-04/06/08/10 | wired (the header comment is out of date) |
| step/ext.hpp (defdbio) | `def::dbio::ext::fun_*` (src/step/ext_wiring.cpp) | p1-10/12/13 | wired |
| mesh2d::MemberExt / ElemExt | src/obj_mesh2d/obj_mesh2d.cpp, rc_array.cpp | p1-04 sub-objects, p1-05 COW | wired (the header comments are out of date) |
| `__has_include` packet switches | defkey kw_g2obj/dispatch.hpp (g2bc/g2mg/g2geo/g2sv/g1a), kw_g1a/group1_shell.cpp (g1b), kw_g2bc/ec.cpp and kw_g2sv/write_sv.cpp (g2sv/g2geo), defmodel topo/geo_fallback.hpp, topo/ptr_list.hpp, layout/sdctl_ptr_list_obj_geo2d.cpp | the named packets | wired: every header is in the tree, so the real branch compiles |

## DEF_NOT_PORTED sites still in the tree

| address | location | owner packet | status |
|---|---|---|---|
| 140263fc0 (shape 3) | defmodel/src/window/llwindef_data.cpp `window_point` | p1-07 OBJ_GEO3D (+ kw-g2geo `geo3d_coords` 140006410) | wired (kwm-audit): LLWINDEF_DATA+0x220 is now a real OBJ_GEO3D (ctor 1402f9020, reset in 14025c350, clear in 14025d0a0, copy 1402f9360/1402f9b60 in 14025c9b0, dtor 1402f91f0) |
| 14026a3c0 (shape 3) | defmodel/src/bc/window_bc.cpp `window_has_shape` | p1-06 `linked::member10_array_first` | wired (kwm-audit) |
| 14041b7d0 (shape 3 branch) | defkey/src/kw_g3/window_io.cpp reader | kw-g3 (the branch fills the window's OBJ_GEO3D) | wired (kwm-win3): ported from the disassembly (read_i 140413de0, resize_nodes 1402f9600, geo3d_coords 140006410, read_i_d3 140415880, resize_elems 1402f96a0, geo3d_elems 140005510, read_i5 140414200, 1402fbb30); no DEF_NOT_PORTED left. Reached by MGUSER in 3D (FRCWIN / WIN* reject shape 3); tests/test_kw_g3_window3d.cpp |
| 1402f4890 (shape 3 branch) | defkey/src/kw_g3/window_io.cpp writer | kw-g3 | wired (kwm-win3): ported from the disassembly (node_count 1400034d0, elem_count 140003500, geo3d_coords / geo3d_elems, strings 1405423ec / 140542948 / 140542968 / 140542970); no DEF_NOT_PORTED left. Reached by the MGUSER mesh-settings writer and the object writers for a shape-3 window; KEY round trip in tests/test_kw_g3_window3d.cpp |
| 1401c5a20 | defkey/src/kw_g2obj/dispatch.cpp `write_file_header` | kw-g2obj / key_writer `write_file_banner` | owner missing: the banner needs the product name (140224dd0: "DEFORM-2D"/"DEFORM-3D" from .data 1405deec0/1405deed0, trivial) and the version string (140225450 returns the .bss buffer 1406fdb20, filled at start-up by code not ported). Only the single-object writers pass withHeader |
| 1401cf930 / 1401cfc30 | defmodel/src/step_names/name_arrays.cpp | kw-g1b | intentionally DEF_NOT_PORTED: unreachable (1401d2b70 always returns 1) |
| 1401d48d0 (body) | defmodel/src/step_names/name_arrays.cpp | kw-g1b | intentionally DEF_NOT_PORTED: unreachable for the same reason |
| 1403300b0 / 14032ffa0 | defmodel/src/cvname/cvname.cpp detach | cvname | intentionally DEF_NOT_PORTED: the handles are never shared |
| 14001c770 / 14001c9f0 | defmodel/src/layout/cls_14001c770.cpp | main program object (Phase 2+) | owner missing (link stub) |
| 14019b980 (fallback) | defmodel/src/layout/sdctl_ptr_list_obj_geo2d.cpp | p1-07 OBJ_GEO2D | wired: the header exists, so the fallback is not compiled |
| 1403d84b0 / 1403d96fc (print == 1) | defdbio/src/resultset/db_result_sets.cpp | p1-13 | intentionally DEF_NOT_PORTED: no caller passes print = 1 |
| 14041f590 (ncol > 6) | defmodel/src/topo/ftn_geom.cpp | p1-11 | intentionally not_ported: the original overruns its 6-double SAVE buffer |
| StateVar ctor (default) | defmodel/src/llobj/ext.cpp `new_statevar_by_ctor` | p1-09 | intentionally DEF_NOT_PORTED: every registered ctor address has a case |
