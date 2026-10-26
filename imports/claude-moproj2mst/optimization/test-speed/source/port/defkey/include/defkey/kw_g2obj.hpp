// defkey/kw_g2obj.hpp: keyword group 2 (objects), packet kw-g2obj.
//
// This packet owns the group 2 shells:
//   * the parser switch FUN_14038b8a0 (read_object_keyword), which reads the object number,
//     gets or creates the object and dispatches on the keyword id;
//   * the per-object KEY writer FUN_14029e250 (write_object), emission order in
//     docs/re/keywords/group2-writer-map.md;
// and its own keywords: the object header (OBJNAM, OBJTYP, AVGSTR, LMTSTR, ZSTR, PENVOL, REFTMP,
// TMPLMT), the movement block (writer FUN_1402a7c80: PRSNAM, MOVCTL, SPDLMT, PRSTRE, STROKE,
// FORCE, ANGMOV, ANGMVY, CNTRAX, ANGMO2, CNTRA2, OBJUPD, OBJSTP; MOVANG is read and dropped) and
// the remesh / control scalars (RMDPTH, RMSTRK, RMTIME, RMSTEP, FRCSTP, FRCNEL, FRCMTH, CRPTIM,
// ELPSOL, USRSUB, REFPOS, TRGVOL, ROTSYM, OTPRNG, CSFREQ, VOLCRG). OBJPHS has no parser case.
//
// Specs: docs/re/keywords/<KW>.md, group2-helpers.md, group2-parser-map.md, group2-writer-map.md.
//
// ---- Packet API (what the other group 2 packets declare) ---------------------------------------
// The shells include each header below with #if __has_include, so a packet can land at any time.
// Every header declares, in its namespace, with model = def::model, KeyRef = std::vector<int32_t>:
//
//   bool handles(int id);         // parser ids whose case it ports
//   int  read_case(model::DB_STEP_DATA& m, std::FILE* f, model::LLOBJ_DATA& obj, int id);
//                                 // the case body after the object lookup; returns what
//                                 // FUN_14038b8a0 returns: 0, 2 or 3
//   bool handles_write(int id);   // registry handler2 ids it ports
//   int  write_keyword(int id, std::FILE* f, model::DB_STEP_DATA& m, const KeyRef& ref);
//                                 // handler2 of `id`: (FILE*, model, ref) like the original
//
// and these block writers ((model, f, obj, N) like the original unless noted; N is the printed
// object number; return values the driver ignores may still be returned):
//
//   <defkey/kw_g2bc.hpp>  def::key::g2bc   (all BCs: EC*, WIN*, nodal BCC*, BCCFNC, LOC*; RSEDEF)
//     int  write_winprs_block(m, f, obj, N);    // 1402ab880, type-5 objects
//     void write_window_lists(m, f, obj, N);    // part of 14029e250: writer-map steps 48-52
//     int  write_edge_face_bcs(m, f, obj, N);   // 1402abf20
//     int  write_rsedef(f, m, ref);             // 1402b7140 (status ignored by the driver)
//     int  write_local_bcs(m, f, obj, N);       // 1402ac560
//   <defkey/kw_g2mg.hpp>  def::key::g2mg   (MG*, OPSTOP, DYNARY; RM* lines inside the block)
//     int  write_mesh_block(m, f, obj, N);      // 1402a9180
//     int  write_opstop(m, f, obj, N);          // 1402a4020
//     int  write_dynary(m, f, obj, N);          // 1402a41c0
//   <defkey/kw_g2geo.hpp> def::key::g2geo  (DIEGEO, RZ, ELMCON, MTLGRP, TPLOGY, CSPMTX, SYMSUF,
//                                           WPAXIS, FLDGEO, FLWNET, POLYTP, DIEPNT, DIEEDG,
//                                           CRDSYS, GEO23, GEOSEC, MSHSEC)
//     void write_geometry(m, f, obj, N);        // part of 14029e250: writer-map steps 19-22
//     int  write_nodes(m, f, obj, N);           // 1402a2db0
//     int  write_elements(m, f, obj, N);        // 1402a30c0
//     int  write_symsuf(m, f, obj, N);          // 1402a3880
//     int  write_wpaxis(m, f, obj, N);          // 1402a39e0
//   <defkey/kw_g2sv.hpp>  def::key::g2sv   (state variables: DRZ, URZ, ..., CUSVAR, HDNEST)
//     int  write_state_var(m, f, obj, code, N, k); // 1402a4a80(m, f, obj, code, N, k)
//                                           // k = 0, or the custom entry id
//     int  custom_count(obj);                   // 14032a140(obj+0xb58)
//     void* custom_at(obj, i);                  // 140185f80(obj+0xb58, i), null if not custom
//     int32_t custom_id(entry);                 // 14032ec90
//
// Group 1 (kw-g1a, <defkey/kw_g1a.hpp>): write_stnout(&m, f), write_elmnod(&m, f).
#pragma once

#include <cstdint>
#include <cstdio>
#include <vector>

namespace def::model {
class DB_STEP_DATA;
class LLOBJ_DATA;
}

namespace def::key::g2obj {

// The keyword-reference vector a KEY write handler receives (the original passes a refcounted
// heap vector of ints built with 140159860 / 1402b7530): [object number (obj+8)] and, for list
// entries, [object number, entry index].
using KeyRef = std::vector<std::int32_t>;

// FUN_14038b8a0(model, f, keywordId): the group 2 parser. Reads the object number ("%i"), gets
// or creates the object, then runs the keyword's case. Returns 0, 2 ("Reading Error") or 3
// ("Data Error"). `m` is the model FUN_1401d81b0 hands to the parser.
int read_object_keyword(model::DB_STEP_DATA& m, std::FILE* f, int id);

// True for the ids whose parser case this packet ports.
bool handles(int id);
// The case body for one of those ids (after the object lookup). Same return codes.
int read_case(model::DB_STEP_DATA& m, std::FILE* f, model::LLOBJ_DATA& obj, int id);

// FUN_14029e250(model, f, obj, outNo, withHeader): one object's KEY block. Returns 0, or the
// first non-zero code of a checked writer (3 for an unknown OBJTYP or REFPOS mode).
int write_object(model::DB_STEP_DATA& m, std::FILE* f, model::LLOBJ_DATA* obj,
                 std::uint32_t out_no, bool with_header);

// FUN_1402a7c80: the movement block (also used by the equipment-file writer FUN_1401c6d00).
// `no` < 1 means "use obj+8".
int write_movement_block(model::DB_STEP_DATA& m, std::FILE* f, model::LLOBJ_DATA* obj,
                         std::int32_t no);

// Registry column 2 (KEY write handlers) owned here; FUN_1402f79c0 calls them by id.
bool handles_write(int id);
int write_keyword(int id, std::FILE* f, model::DB_STEP_DATA& m, const KeyRef& ref);

int write_avgstr(std::FILE* f, model::DB_STEP_DATA& m, const KeyRef& ref);  // 1402af420
int write_lmtstr(std::FILE* f, model::DB_STEP_DATA& m, const KeyRef& ref);  // 1402af4f0
int write_zstr(std::FILE* f, model::DB_STEP_DATA& m, const KeyRef& ref);    // 1402af5c0
int write_penvol(std::FILE* f, model::DB_STEP_DATA& m, const KeyRef& ref);  // 1402b4400 (2D form)
int write_reftmp(std::FILE* f, model::DB_STEP_DATA& m, const KeyRef& ref);  // 1402af350
int write_tmplmt(std::FILE* f, model::DB_STEP_DATA& m, const KeyRef& ref);  // 1402af6a0
int write_otprng(std::FILE* f, model::DB_STEP_DATA& m, const KeyRef& ref);  // 1402af770
int write_csfreq(std::FILE* f, model::DB_STEP_DATA& m, const KeyRef& ref);  // 1402af830
int write_volcrg(std::FILE* f, model::DB_STEP_DATA& m, const KeyRef& ref);  // 1402afb10
int write_movctl(std::FILE* f, model::DB_STEP_DATA& m, const KeyRef& ref);  // 1402b1400
int write_spdlmt(std::FILE* f, model::DB_STEP_DATA& m, const KeyRef& ref);  // 1402b11e0
int write_prstre(std::FILE* f, model::DB_STEP_DATA& m, const KeyRef& ref);  // 1402afd70
int write_stroke(std::FILE* f, model::DB_STEP_DATA& m, const KeyRef& ref);  // 1402b0350
int write_force(std::FILE* f, model::DB_STEP_DATA& m, const KeyRef& ref);   // 1402b04b0
int write_angmov(std::FILE* f, model::DB_STEP_DATA& m, const KeyRef& ref);  // 1402b0610
int write_angmvy(std::FILE* f, model::DB_STEP_DATA& m, const KeyRef& ref);  // 1402b0a40
int write_cntrax(std::FILE* f, model::DB_STEP_DATA& m, const KeyRef& ref);  // 1402b1000
int write_angmo2(std::FILE* f, model::DB_STEP_DATA& m, const KeyRef& ref);  // 1402afe80
int write_cntra2(std::FILE* f, model::DB_STEP_DATA& m, const KeyRef& ref);  // 1402b01e0
int write_rmdpth(std::FILE* f, model::DB_STEP_DATA& m, const KeyRef& ref);  // 1402b49d0
int write_rmstrk(std::FILE* f, model::DB_STEP_DATA& m, const KeyRef& ref);  // 1402b4aa0
int write_rmtime(std::FILE* f, model::DB_STEP_DATA& m, const KeyRef& ref);  // 1402b4b70

// Registry column 3 (data accessors): *out = the address of the keyword's table object (by
// name for MOVCTL and ANGMVY). 0, or 3 when the object (or the name) is unknown.
int table_movctl(model::DB_STEP_DATA* m, const char* name, const KeyRef& ref, void** out);  // 1402b3da0
int table_spdlmt(model::DB_STEP_DATA* m, const char* name, const KeyRef& ref, void** out);  // 1402b1360
int table_angmov(model::DB_STEP_DATA* m, const char* name, const KeyRef& ref, void** out);  // 1402b0970
int table_angmvy(model::DB_STEP_DATA* m, const char* name, const KeyRef& ref, void** out);  // 1402b0ef0
int table_csfreq(model::DB_STEP_DATA* m, const char* name, const KeyRef& ref, void** out);  // 1402afa70
int table_volcrg(model::DB_STEP_DATA* m, const char* name, const KeyRef& ref, void** out);  // 1402afcd0

// Direct writers FUN_14029e250 calls with (model, f, obj, N).
int write_objtyp(model::DB_STEP_DATA& m, std::FILE* f, model::LLOBJ_DATA& obj, std::uint32_t no);  // 1402a25a0
int write_objnam(model::DB_STEP_DATA& m, std::FILE* f, model::LLOBJ_DATA* obj, std::uint32_t no);  // 1402a2530
int write_otprng_line(model::DB_STEP_DATA& m, std::FILE* f, model::LLOBJ_DATA* obj, std::uint32_t no);  // 1402abd50

// FUN_1402b7310: one custom state-variable entry, ref = (obj+8, entry id).
int write_custom(std::FILE* f, model::DB_STEP_DATA& m, const KeyRef& ref);

// FUN_14018c810: the 2D movement angle in degrees, atan2(dir.y, dir.x) * 180/pi, of the movement
// sub-structure at obj+0x2160.
double movement_angle_2d(const model::LLOBJ_DATA& obj);

// FUN_14017aaf0(model+0x1380, no): the object whose number (obj+8) is `no`, or null.
model::LLOBJ_DATA* find_object(model::DB_STEP_DATA& m, std::int32_t no);

} // namespace def::key::g2obj
