// defmodel/obj_scale.hpp: model functions behind the command keywords SVSCAL and DOESCL
// (packet p67-4): table interpolation and scaling, the strain remap, the DOE scaling of
// materials, objects and movements, and the on-demand field accessors they use.
// Specs: docs/re/commands/SVSCAL.md, DOESCL.md (branch claude/project-thread-6cvah9).
//
// Field accessors: `set` is the node state-variable set at LLOBJ_DATA+0x8a0 (Cls_14017ef20) or
// the element set at LLOBJ_DATA+0x9e0 (Cls_140182880). Each makes sure its field exists (the set's
// allocation with the field id as key, 14017ffe0 / 1401836c0), runs the region loader on the
// set's model and returns the array.
#pragma once

#include <cstdint>

namespace def::model {
class DB_STEP_DATA;
class DYNARY;
class LLOBJ_DATA;
class Material;
} // namespace def::model

namespace def::model::scale {

// 140132a60: the segment of `x` in the table's first axis (extrapolating segments at both ends);
// *r = (x - X[i]) / (X[i+1] - X[i]) (0.0 when no segment). -1 when the point counts are not all
// positive or no segment contains x.
std::int32_t table_segment(const DYNARY* t, double x, double* r);
// 140132bf0: piecewise-linear value of the table at x; 0.0 without a segment.
double table_interpolate(const DYNARY* t, double x);
// 140136220: constant (ndim 0) or every value multiplied by *factor; returns 1.
std::int32_t table_scale(DYNARY* t, const double* factor);

// 1401d48d0: the region loader of state-variable `*id` (-1 = all). A no-op in this build: it
// only acts when 1401d2b70 returns 0, which it never does.
void region_load(DB_STEP_DATA* m, const std::int32_t* id);

// 14017f020 / 140182980: the model's layout table (model+0x15f8) of a node / element set, or
// null when the set has no model.
const std::int32_t* node_set_dims(void* set);
const std::int32_t* element_set_dims(void* set);
// 140180c40: node temperature NDTMP (id 8, set+0x40)
double* node_temperature(void* set);
// 140181320: nodal strain (id 0x1b, set+0x98)
double* node_strain(void* set);
// 140183fd0: element strain STRAIN (id 0x2b, set+0x48)
double* element_strain(void* set);

// 140204bc0: strain := F(strain) for every element strain value and, with the ELMNOD strain
// option 2 (model+0x88c), every nodal strain value. Only when the table's variable is 7.
void remap_strain(LLOBJ_DATA* obj, const DYNARY* t);

// 14029b3f0: DOESCL on a material (FSTRES 0x5e, YOUNG 0x5f, FPERV 0x68). 1, or 0 when nothing
// applies. `ints` are the keyword line's integers i1..i7.
std::int32_t scale_material(Material* mat, std::int32_t id, const double* factor, std::int32_t n,
                            const std::int32_t* ints);
// 1401954d0: DOESCL MOVCTL on the movement block at obj+0x2160. 1, or 0 for an unhandled type.
std::uint8_t scale_movement(std::uint8_t* mv, const double* factor, std::int32_t n,
                            const std::int32_t* ints);
// 14021d9c0: DOESCL on an object (MOVCTL 0x97, NDTMP 0xaa, LOCTMP 0xe6). 1, or 0 for other ids.
std::int32_t scale_object(LLOBJ_DATA* obj, std::int32_t id, const double* factor, std::int32_t n,
                          const std::int32_t* ints);

} // namespace def::model::scale
