// defcmd/objpos.hpp: the OBJPOS command (case 7 of the command dispatcher FUN_140378e00) and the
// object transforms it calls (translate, rotate, mirror, own centre, 4x4 transform).
// Specs: docs/re/commands/OBJPOS.md, OBJPOS_geo_kernels.md, _common.md (branch
// claude/project-thread-6cvah9). Packet p67-1; the interference modes (2, 12, 102, 112) are
// p67-2's (defcmd/objpos_ifd.hpp), the drop modes (7, 8, 9) p67-3's (defcmd/objpos_drop.hpp).
#pragma once

#include <cstdint>
#include <cstdio>

namespace def::model {
class DB_STEP_DATA;
class LLOBJ_DATA;
} // namespace def::model

namespace def::cmd::objpos {

// The parsed command line (OBJPOS.md section 2). v[k] = token k+2 (`local_5d18`..`local_5ce0`):
// 3D v[0..7] = v2..v9 (v9 = coupled count as read), 2D v[0..4] = v2..v6.
struct Args {
    std::int32_t obj_no = 0;       // local_5708 (strtol of token 0)
    std::int32_t mode = 0;         // local_5704 (strtol of token 1)
    double v[8] = {};              // local_5d18 .. local_5ce0
    std::int32_t n_coupled = 0;    // local_56d8 low half: min(line-2 token count, (int)count)
    std::int32_t coupled[100] = {}; // local_5cb8: (int)atof of each token of line 2 (no abs yet)
};

// The case's flags and the drop result the post-processing uses (OBJPOS.md section 5).
struct State {
    bool moved = false;     // local_5547
    bool rotated = false;   // local_5548
    bool mirrored = false;  // local_5568
    bool dropped = false;   // local_5546
    double drop_m[16] = {}; // local_5628: 4x4 transform of a successful drop (FUN_140240fc0 init)
};

// @data 140706bb0  DAT_140706bb0/bb8/bc0: the last displacement (process lifetime, zero at start)
inline double g_last_disp[3] = {0.0, 0.0, 0.0};

// Case 7 of FUN_140378e00: reads the arguments from `f` (positioned after the keyword) and runs
// the command. Returns 0, 2 (Reading Error) or 3 (Data Error); messages go to stdout.
int run(model::DB_STEP_DATA& m, std::FILE* f);

// ---- object predicates and lookup ----
// @src 1401611f0 (declared): type != 7 and mesh or die present, else prints the error; false then
bool usable(model::LLOBJ_DATA* o);
// 140160cc0: 2D owning model: 2D edge list non-empty; else the 3D rigid-geometry flag +0x3990
bool has_geometry(const model::LLOBJ_DATA* o);
// 140203980: 3D +0x3990 and geometry 0 has points and faces; 2D edge list non-empty and
// 140304e40(o, 0)
bool has_die_geometry(model::LLOBJ_DATA* o);

// ---- transforms (OBJPOS.md section 6) ----
// 14021d0b0: translate by d (2 or 3 comps by the object's dimension). flag_a: also move windows
// of other objects that follow o; flag_b: also move the rigid geometry. By reference as in the binary.
void translate(model::LLOBJ_DATA* o, const double* d, const char* flag_a, const char* flag_b);
// 14021d2f0: rotate by angle_deg about the axis (origin, dir); 2D passes dir = nullptr.
void rotate(model::LLOBJ_DATA* o, const double* origin, const double* dir, double angle_deg);
// 1401fd850: 3D mirror at the plane (p, n), single precision (OBJPOS.md 6.5)
void mirror_3d(model::LLOBJ_DATA* o, const double* p, const double* n);
// 1403066a0: 2D mirror at the line a-b (OBJPOS.md 6.6)
void mirror_2d(model::LLOBJ_DATA* o, const double* a, const double* b);
// 1401fe240: apply the 4x4 transform of a drop (parts 1401fe290, 1401fe310, 1401fd730)
void transform_4x4(model::LLOBJ_DATA* o, const double* m16);

// ---- centres and boxes ----
// 1401ff360: own centre for mode 6 (3D 1401fe7d0 volume centroid, 2D 1401ff4f0 box centre);
// c is not cleared here (callers pass zeros). Returns the success flag OBJPOS ignores.
bool own_centre(model::LLOBJ_DATA* o, double* c);
// 140181ae0: bounding box of the node coordinates of the node set (obj+0x8a0); false without nodes
bool node_bbox(const void* node_set, double* mn, double* mx);

// ---- movement ----
// 1401952e0: displacement of length (p3 - p2) along the normalised MOVCTL direction mv+0x50;
// zero when the direction or p3 is ~0 (1e-6). mv = obj+0x2160.
void move_along_movctl(const void* mv, double p2, double p3, double* out);

} // namespace def::cmd::objpos
