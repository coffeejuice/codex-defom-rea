// Functions of packet p8-07 shared between its sources (not a public header; the cross-packet API
// is defcmd/meshgen/p8_seams.hpp and defcmd/cmd_p8_07.hpp).
#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdio>

#include <defcmd/meshgen/meshgen_base.hpp>

namespace def::model {
class DB_STEP_DATA;
class LLOBJ_DATA;
class LLWINDEF_DATA;
class DEF_AMGGEO;
class OBJ_GEO2D;
}

namespace def::cmd {

using model::DB_STEP_DATA;
using model::LLOBJ_DATA;

// ---- mg_common.cpp ---------------------------------------------------------------------------
bool obj_keep_surface(const LLOBJ_DATA* obj);                  // 140158ea0
bool obj_amg_dxt_ok(const LLOBJ_DATA* obj);                    // 1401b06f0
void set_state_surface_read(MeshGenBase& mg);                  // 1401438e0
void set_state_coating_done(MeshGenBase& mg);                  // 140145cc0
void detect_uniform_temperature(MeshGenBase& mg);              // 140144810
bool meshgen_prepare(MeshGenBase& mg);                         // 140144620
double obj_density(LLOBJ_DATA* obj);                           // 14020b260
void set_obj_density(LLOBJ_DATA* obj, double v);               // 14020b280
void set_elem_density(LLOBJ_DATA* obj, double v);              // 14020b1e0
void elem_density_range(LLOBJ_DATA* obj, double* lo, double* hi);  // 14020af50
void density_text(LLOBJ_DATA* obj, char* out);                 // 14020ae70
void set_density_text(LLOBJ_DATA* obj, const char* text);      // 14020b0f0
void normalise_weights(MeshGenBase& mg, std::byte* w);         // 140144200

// ---- meshgen_2d.cpp --------------------------------------------------------------------------
bool mesh2d_run(MeshGenBase& mg, const char* multi_curve_topology);   // 140144a70
std::int32_t mesh2d_inputs(MeshGenBase& mg, const char* multi);       // 140144de0
std::int32_t write_usrwin(MeshGenBase& mg, std::byte* w, std::uint32_t pts2d);  // 140145ce0
void mesh2d_cleanup(MeshGenBase& mg);                                 // 140145480
bool mesh2d_read_back(MeshGenBase& mg, const char* set_group);        // 1401455b0
void mesh2d_coating(MeshGenBase& mg);                                 // 140145810
bool write_add_coating(MeshGenBase& mg);                              // 1401458d0
void dst_name(MeshGenBase& mg, char* out);                            // 140144a00
void geo23_clear(MeshGenBase& mg, LLOBJ_DATA* obj, double limit);     // 140155340
bool region_materials(MeshGenBase& mg);                               // 1401574d0
std::int32_t material_id(DB_STEP_DATA* m, std::int32_t i);            // 14019d420
std::int32_t first_free_material(void* materials);                    // 14029c3c0
void build_topology(LLOBJ_DATA* obj, const char* flag);               // 140198520
void window_to_polygon(model::LLWINDEF_DATA* w, char invert);         // 140268590

// ---- geo_writer.cpp ----------------------------------------------------------------------------
bool write_geo_file(MeshGenBase& mg, const std::int32_t* ids, std::int32_t* ids_out, std::uint32_t* ntri);  // 140143920
bool curve_points_planar(model::OBJ_GEO2D* c);                        // 14027f140

// ---- meshgen_3d.cpp ----------------------------------------------------------------------------
void surface_cleanup(MeshGenBase& mg);                                // 14013efb0
std::int32_t write_block3(MeshGenBase& mg);                           // 14013f8f0
bool write_nbcd_dat(MeshGenBase& mg);                                 // 140142240
bool surface_has_bc_codes(LLOBJ_DATA* obj);                           // 1402062b0
double size_scaling(LLOBJ_DATA* obj, double area, std::int32_t n, const char* apply);  // 1401559d0
double surface_area(LLOBJ_DATA* obj);                                 // 140155600

} // namespace def::cmd
