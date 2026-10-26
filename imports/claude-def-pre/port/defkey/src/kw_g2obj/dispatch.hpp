// kw-g2obj: calls from the group 2 shells (parser switch FUN_14038b8a0, object writer
// FUN_14029e250) into the other group 2 packets and into group 1.
//
// Each packet's header is optional: while it is not on the branch, its calls fall back to the
// stubs in dispatch.cpp (a parser case returns 2 as if it failed, block writers write nothing).
// The API a packet header must declare is in kw_g2obj.hpp ("Packet API"); in short, every
// packet <x> in {bc, mg, geo, sv} declares in namespace def::key::g2<x>:
//
//   bool handles(int id);                                            parser ids it ports
//   int  read_case(model::DB_STEP_DATA& m, std::FILE* f, model::LLOBJ_DATA& obj, int id);
//                                                                    the case body, 0/2/3
//   bool handles_write(int id);                                      registry handler2 ids
//   int  write_keyword(int id, std::FILE* f, model::DB_STEP_DATA& m,
//                      const std::vector<std::int32_t>& ref);        handler2 by id
//
// plus the block writers named in the wrappers below.
#pragma once

#include <cstdint>
#include <cstdio>
#include <vector>

#include <defkey/kw_g2obj.hpp>

#if __has_include(<defkey/kw_g2bc.hpp>)
#include <defkey/kw_g2bc.hpp>
#define DEF_KW_G2BC 1
#endif
#if __has_include(<defkey/kw_g2mg.hpp>)
#include <defkey/kw_g2mg.hpp>
#define DEF_KW_G2MG 1
#endif
#if __has_include(<defkey/kw_g2geo.hpp>)
#include <defkey/kw_g2geo.hpp>
#define DEF_KW_G2GEO 1
#endif
#if __has_include(<defkey/kw_g2sv.hpp>)
#include <defkey/kw_g2sv.hpp>
#define DEF_KW_G2SV 1
#endif
#if __has_include(<defkey/kw_g1a.hpp>)
#include <defkey/kw_g1a.hpp>
#define DEF_KW_G1A 1
#endif

namespace def::key::g2obj::ext {

// Parser: the case body of a keyword another packet owns. 2 when no packet handles `id`.
int read_case(model::DB_STEP_DATA& m, std::FILE* f, model::LLOBJ_DATA& obj, int id);

// Registry handler2 of keyword `id` (this packet's or another's). 0 (nothing written) when no
// packet handles it yet, like the registry's default handler FUN_1402f75d0.
int write_keyword(int id, std::FILE* f, model::DB_STEP_DATA& m, const KeyRef& ref);

// Group 1 writers FUN_14029e250 calls under flags 0x1000 / 0x2000 (kw-g1a).
int write_stnout(model::DB_STEP_DATA& m, std::FILE* f);  // 1403d6580
int write_elmnod(model::DB_STEP_DATA& m, std::FILE* f);  // 1403d6620
// FUN_1401c5a20: file header, only for a single object written alone (withHeader).
void write_file_header(model::DB_STEP_DATA& m, std::FILE* f);

// kw-g2bc
int write_winprs_block(model::DB_STEP_DATA& m, std::FILE* f, model::LLOBJ_DATA& obj,
                       std::uint32_t no);  // 1402ab880 (type-5 objects, flags & 0x1000000)
// part of 14029e250: the WINPRS (2D, then 3D), WINTMP cleanup FUN_1402696f0 + WINTMP, WINATM and
// WINRSE list loops (writer-map steps 48-52); nothing is checked.
void write_window_lists(model::DB_STEP_DATA& m, std::FILE* f, model::LLOBJ_DATA& obj,
                        std::uint32_t no);
int write_edge_face_bcs(model::DB_STEP_DATA& m, std::FILE* f, model::LLOBJ_DATA& obj,
                        std::uint32_t no);  // 1402abf20
int write_rsedef(std::FILE* f, model::DB_STEP_DATA& m, const KeyRef& ref);  // 1402b7140

// kw-g2mg
int write_mesh_block(model::DB_STEP_DATA& m, std::FILE* f, model::LLOBJ_DATA& obj,
                     std::uint32_t no);  // 1402a9180 (RM*, MG*)
int write_opstop(model::DB_STEP_DATA& m, std::FILE* f, model::LLOBJ_DATA& obj,
                 std::uint32_t no);  // 1402a4020
int write_dynary(model::DB_STEP_DATA& m, std::FILE* f, model::LLOBJ_DATA& obj,
                 std::uint32_t no);  // 1402a41c0

int write_local_bcs(model::DB_STEP_DATA& m, std::FILE* f, model::LLOBJ_DATA& obj,
                    std::uint32_t no);  // 1402ac560

// kw-g2geo
// part of 14029e250: writer-map steps 19-22 (DIEGEO FUN_1402aaa70, geometry FUN_1402a26c0,
// 2D TPLOGY FUN_1402a9f90, FLDGEO/FLWNET FUN_1402a2b70) with their conditions.
void write_geometry(model::DB_STEP_DATA& m, std::FILE* f, model::LLOBJ_DATA& obj,
                    std::uint32_t no);
int write_nodes(model::DB_STEP_DATA& m, std::FILE* f, model::LLOBJ_DATA& obj,
                std::uint32_t no);  // 1402a2db0 (RZ)
int write_elements(model::DB_STEP_DATA& m, std::FILE* f, model::LLOBJ_DATA& obj,
                   std::uint32_t no);  // 1402a30c0 (ELMCON)
int write_symsuf(model::DB_STEP_DATA& m, std::FILE* f, model::LLOBJ_DATA& obj,
                 std::uint32_t no);  // 1402a3880
int write_wpaxis(model::DB_STEP_DATA& m, std::FILE* f, model::LLOBJ_DATA& obj,
                 std::uint32_t no);  // 1402a39e0

// kw-g2sv
// FUN_1402a4a80(model, f, obj, code, N, k): state-variable writer by code (k = 0 from the driver;
// FUN_1402b7310 passes ref[1]).
int write_state_var(model::DB_STEP_DATA& m, std::FILE* f, model::LLOBJ_DATA& obj, int code,
                    std::uint32_t no, std::int32_t k);
// The custom state-variable list obj+0xb58: count (FUN_14032a140), entry i as StateVarCustom or
// null (FUN_140185f80), entry id (FUN_14032ec90). The entry writer FUN_1402b7310 is ported here.
int custom_count(model::LLOBJ_DATA& obj);
void* custom_at(model::LLOBJ_DATA& obj, int i);
std::int32_t custom_id(void* entry);

} // namespace def::key::g2obj::ext
