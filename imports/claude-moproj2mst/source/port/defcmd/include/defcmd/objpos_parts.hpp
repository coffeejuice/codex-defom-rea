// defcmd/objpos_parts.hpp: lower-level pieces of the OBJPOS transforms that other parts of the
// packet (and the interference / drop packets) share. Packet p67-1.
#pragma once

#include <cstdint>

namespace def::model {
class LLWINDEF_DATA;
class Cls_140269250;
} // namespace def::model

namespace def::cmd::objpos {

// 140268040: window points += d (2 or 3 comps); 3D windows refresh their faces
void window_translate(model::LLWINDEF_DATA* w, const double* d);
// 140269eb0: every window of a window list
void winlist_translate(model::Cls_140269250* l, const double* d);
// 140194f90: projection of d on the unit movement direction (mv = obj+0x2160)
double movctl_projection(const unsigned char* mv, const double* d, const double* dir);
// 14017f020: the node-set widths (owner+0x15f8), null without owner; ns = obj+0x8a0
const std::int32_t* node_widths(const unsigned char* ns);

// 140268160: window points rotated (3D 140325691 needs dir; 2D 14032ab90); 3D windows refresh faces
void window_rotate(model::LLWINDEF_DATA* w, const double* origin, const double* dir, double angle);
// 140269f20: every window of a window list
void winlist_rotate(model::Cls_140269250* l, const double* origin, const double* dir, double angle);

// 140325884: rotate n 6-component symmetric tensors (3D) by *a degrees about the axis d
void rotate_tensors_3d(double* s, const std::int32_t* n, const std::int32_t* kind, const double* d,
                       const double* a, const std::int32_t* flag);
// 14032e585: rotate n 4-component tensors (2D) by *a degrees
void rotate_tensors_2d(double* s, const std::int32_t* n, const std::int32_t* kind, const double* a,
                       const std::int32_t* flag);
// 140182980: the element-set widths (owner+0x15f8), null without owner; es = obj+0x9e0
const std::int32_t* elem_widths(const unsigned char* es);
// 140184ac0: STRESS, STNCMP, YLDS and MATAXI of the element set rotated by angle (degrees)
void elems_rotate(unsigned char* es, const double* origin, const double* dir, double angle);

} // namespace def::cmd::objpos
