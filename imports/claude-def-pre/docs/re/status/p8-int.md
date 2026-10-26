# p8-int: Phase 8 integration branch (claude/p8-int)

Base origin/main @67aa533, then the branches below, in order. Each conflict resolution is listed.

## origin/claude/p67-gate @05f02c0
- port/CMakeLists.txt, port/cmake/DefPort.cmake: comment / quoting differences only; kept main's.
- port/defmodel/include/defmodel/globals_p1_10.hpp (COMMON 140b84720): both sides alias the one
  object def::rt::geoutl::g_common_140b84720 (main through globals_p1_09.hpp's reference, the gate
  directly). Kept main's block (dat_140b847x0 = views of g_common_140b84720 from globals_p1_09.hpp),
  both includes; one definition remains (defrt geoutl_tri.hpp).
- port/defmodel/src/node_access/node_access.cpp: took the gate's forward of 14017f020 to kw-g2sv
  (svkw::set8a0_dims, the @src owner in statevar_kw.cpp); main's copy carried a second @src tag.

## origin/claude/p8-02 @ab23e6d
- port/defcmd/tests/cmd_oracle_runner.hpp: kept the gate's (05f02c0, newer than the 21cd8f1 copy p8-02 took).

## origin/claude/p8-03 @536fd7b
- no conflicts.

## origin/claude/p8-04 @3b9caab
- cmd_oracle_runner.hpp: kept the gate's (p8-04 had the old p67-1 @1103c68 copy).

## origin/claude/p8-09 @e2ed999
- cmd_oracle_runner.hpp: kept the gate's.

## origin/claude/p8-05 @e780e6e
- defrt str_helpers.hpp/.cpp and their test (added by p5-01 from an earlier p8-05 copy): took p8-05's, a superset (owner p8-05).

## origin/claude/p9-02 @81ae3f0, origin/claude/p67-2 @3691b2d
- no conflicts.

## Ownership decisions applied (one owner per address; other copies forward, @src removed)
- 140183cb0: owner defcmd element_set_conn (contact_cmds.cpp), which p67-2's ifd3d elem_conn
  forwards to. defdbio's gendb3d/node_elem.cpp keeps its static read (defdbio cannot call defcmd);
  its @src tag is removed and the comment names the owner.
- 1401b85d0: owner defdbio gendb::remove_if_exists (driver.cpp). p67-5's gensub::delete_if_exists
  forwards to it (tag removed); p8-02's seam calls it. 1401b83d0 / 1401b8320: no GENDB copy exists
  on main, so p67-5's gensub read_line / get_line stay the only ports.
- Cube (1404050b0 140405150 140405350 140405540 1404056c0 1404056e0 140405720 1404057a0
  140256e20): p67-2's bodies (ifd/cube/cells.cpp). p67-7's ctc3d/stubs_cube.cpp now forwards to
  them; cube.hpp's provisional @src tags removed.
- 140256dc0 / 140257150: owner defmodel bc::calloc_ints / calloc_doubles. The owner gained the
  allocation-failure message (14039d3a0 through llobj_copy::ext, as llobj_copy_parts.cpp had it);
  llobj_copy_parts.cpp calloc_int / calloc_double and p67-2's cells.cpp fun_140256dc0 /
  fun_140257150 forward (tags removed).
- crt_fscanf / crt_sscanf: p67-5's (gensub_scan.cpp) is the only copy; p8-02 uses it.
- 140256800 / 140256950 / 1402569e0: p8-09's geolib already forwards to p9-02's CowVector
  push_back (1402f0ba0 / 1402f2240 / 1402f2580); no change.
- p8-07 items: p8-07 is not merged. The launcher 140223790/140224100/140224130 and 1400b9130 are
  on main once (launch.cpp, file_util.cpp). 140158e60 / 140158e80 are tagged in the P4 gate's
  surface_normals.cpp (owner p8-07: moves when p8-07 lands). 1401b5f00, 140002ff0, 140005dd0: seams.
- str helpers 1401b0870 / 1401b0940 / 1401b1060: owner p8-05 (defrt str_helpers);
  def_pre/src/menu7.cpp to_int / starts_with / trim forward (tags removed).
- Uic2CObject family: p8-03's C++ members are the only bodies (p8-01 not merged); p8-04's
  ext.cpp already constructs p8-03's classes.

## fold_status --check
- docs/re/status/p8-9-commands.tsv is the Phase 8-9 index, not a status file (17 "bad address"
  errors): moved to docs/re/p8-9-commands.tsv (nothing refers to it by path).
- 30 addresses listed by two packets with different statuses (P4 vs P6-7 copies, p8-04/p8-09 vs
  p67-1): each set to the highest status of its copies (todo < not-needed < ported < verified).
  Result: 0 errors.

## Later merges
- p8-03 final head @f6f2a30: no conflicts.
- p8-04 @01627f1 (instead of 3b9caab / df12d2f): cmd_oracle_runner.hpp kept the gate's;
  docs/re/status/p8-04.tsv took p8-04's, then the status harmonisation again.

## @data owners (fold_status --check "defined N times")
- Read-only .rdata constants (addresses below 0x140640000, 22 addresses): the tag stays in
  defrt/rdata.hpp where it has the address, else in the lowest library (defrt < defmodel < defkey
  < defdbio < defcmd); the other files keep their local constants (same bits), the tag becomes
  "(rdata <addr>, @data owner <file>)". 14063c0a0 / 14063c200 (the CRT power-of-ten tables): owner
  defrt legacy_printf.cpp; p67-5's gensub_scan.cpp copy is detagged.
- Writable globals, one object each:
  - 140b74428 / 140b74430: owner defmodel globals_p1_10.hpp; p67-2's ifd/d2/d2.hpp g_geoerr1_in /
    g_geoerr2_in are now references to them.
  - 140b84720 / 140b84728 / 140b84730 (COMMON): owner defrt geoutl_tri.hpp; d2.hpp g_geoerr1 /
    g_geoerr2 / g_ext are now references to its members (they were separate doubles).
  - 1406e8524: owner defmodel hdrstats/globals_p4_07.hpp g_hdr_slots; defdbio's
    step/globals_p67_5.hpp dat_1406e8524 is a reference to it. 1406e8710: owner p4-07's
    g_hdr_label_table; p67-5's pointer view stays (different representation, detagged). 1401c7f80
    itself is still ported twice (p4-07 slot_table.cpp, p67-5 db_header_cache.cpp): not decided.
  - 140b847a4: owner d2.hpp g_msg_unit; contact's ctc2d_msg.cpp (a function returning stderr)
    detagged. 140706bb0: p67-2's interim copy in objpos_ifd.hpp (#ifndef, not compiled with
    p67-1) detagged. 14070bc20: second comment in iges.hpp detagged. 140730310: a mention in
    defmodel topo/ftn_geom.hpp detagged.

## Round 2 merges
- p8-09 @30b4af6 (140280290 shrink, gfread_igs_multicurve): no conflicts.
- p8-01 @1c5b1d2: libm.hpp / libm_crt2.cpp kept ours (kw-merge / p8-04's, with crt_acos and
  crt_tan); surface_normals.cpp, window_orient.cpp, gendb3d/movement.cpp, globals_p1_10.hpp took
  p8-01's (the same one-owner fixes, with the constants initialised from defrt rdata.hpp).
- p8-05 @c79c675 (replaces e780e6e): runner, libm, globals_p1_10.hpp and geoutl_tri.hpp (comment-
  only conflicts on the COMMON 140b84720, defined once in defrt/ftn/common_140b84720.hpp) kept ours.
- p8-07 @777c0a2: libm kept ours (crt_tan). The p8-07 owners (launch.cpp 140223790/140224100/
  140224130, key_file.cpp 1401b5f00, file_util.cpp 1400b9130) are single copies; the dispatcher's
  interim 1401b5f00 is compiled out when key_file.hpp is present. 140158e60 / 140158e80 stay tagged
  in the P4 gate's surface_normals.cpp (p8-07's meshgen uses the members directly, untagged).
- Status harmonisation again (7 entries: m-cli / p5-01 / p67-1 / p67-6 / p8-07): 0 errors.
