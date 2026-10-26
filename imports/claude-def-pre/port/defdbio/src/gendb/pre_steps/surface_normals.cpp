// Surface normal lists of the GENDB pre-steps (defdbio/gendb/pre_steps.hpp): 1402059a0 and the
// helpers under it that had no port.
#include <defdbio/gendb/pre_steps.hpp>

#include <cmath>
#include <cstdint>
#include <cstring>

#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/geo_kw/misc_model.hpp>
#include <defmodel/geo_kw/surface_model.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/linked/axis_point_list.hpp>
#include <defmodel/llobj/dynary_table.hpp>
#include <defmodel/step/db_step.hpp>
#include <defrt/rdata.hpp>

#include "../../objcheck/ext.hpp"

namespace def::dbio::gendb {

using model::linked::Cls_140257340;
using model::linked::Cls_140257340List;

namespace {

// rdata 14051f1b8  1e-06: smallest normal length that gets a list entry
constexpr double k_min_normal = def::rt::rdata::d_14051f1b8;
// rdata 14051f258  0.01: largest squared distance between two directions taken as equal
constexpr double k_same_direction = def::rt::rdata::d_14051f258;
// rdata 14051f1e8  0.001: plane tolerance, times the surface's char_length
constexpr double k_plane_tol = def::rt::rdata::d_14051f1e8;
// rdata 14051bed8  0.0
constexpr double k_zero = def::rt::rdata::d_14051bed8;

Cls_140257340List* point_list(void* p) { return static_cast<Cls_140257340List*>(p); }

} // namespace

// ---- generic getters ------------------------------------------------------------------------------

// @src 140158e60  self + 0x28
void* addr_at_28(void* self) { return static_cast<std::byte*>(self) + 0x28; }

// @src 140158e80  *(void**)(self + 0x20)
void* ptr_at_20(const void* self)
{
    void* v;
    std::memcpy(&v, static_cast<const std::byte*>(self) + 0x20, sizeof v);
    return v;
}

// ---- list helpers -----------------------------------------------------------------------------

// 1402580f0: the kw-g2geo port (geo_kw::symsuf_find)
Cls_140257340* point_list_find(Cls_140257340List* list, std::int32_t id) { return model::geo_kw::symsuf_find(*list, id); }

// @src 140258260  first free id
std::int32_t point_list_free_id(Cls_140257340List* list)
{
    std::int32_t id = 1;
    while (point_list_find(list, id) != nullptr) { // 1402580f0
        id = id + 1;
    }
    return id;
}

// @src 140257510  point = *p (memcpy 0x18 to +8)
void point_node_set_point(Cls_140257340* node, const double* p)
{
    if (p != nullptr) {
        std::memcpy(node->point, p, 0x18);
    }
}

// @src 140257590  direction = *d (memcpy 0x18 to +0x20)
void point_node_set_direction(Cls_140257340* node, const double* d)
{
    if (d != nullptr) {
        std::memcpy(node->direction, d, 0x18);
    }
}

// ---- 3-vector kernels ---------------------------------------------------------------------------

// @src 14023b850  squared distance; ((dx*dx + dy*dy) + dz*dz) as in the listing
double vec3_dist2(const double* a, const double* b)
{
    double s = (a[0] - b[0]) * (a[0] - b[0]);
    s = s + (a[1] - b[1]) * (a[1] - b[1]);
    s = s + (a[2] - b[2]) * (a[2] - b[2]);
    return s;
}

// @src 14023e050  unit normal (c - b) x (a - b)
void triangle_normal(const double* a, const double* b, const double* c, double* out)
{
    // 14023e075..14023e103: the six differences, kept in stack slots
    const double v0 = c[0] - b[0]; // [rsp+0x40]
    const double v1 = c[1] - b[1]; // [rsp+0x28]
    const double v2 = c[2] - b[2]; // [rsp+0x38]
    const double u0 = a[0] - b[0]; // [rsp+0x48]
    const double u1 = a[1] - b[1]; // [rsp+0x50]
    const double u2 = a[2] - b[2]; // [rsp+0x30]
    out[0] = v1 * u2 - v2 * u1;
    out[1] = v2 * u0 - v0 * u2;
    out[2] = v0 * u1 - v1 * u0;
    double s = out[0] * out[0];
    s = s + out[1] * out[1];
    s = s + out[2] * out[2];
    const double len = std::sqrt(s); // 140484644
    if (len != k_zero) {             // UCOMISD + JP: NaN divides too
        out[0] = out[0] / len;
        out[1] = out[1] / len;
        out[2] = out[2] / len;
    }
}

// @src 14023c770  |n . (p - q)|; ((n0*d0 + n1*d1) + n2*d2), then CRT fabs (140484558)
double plane_distance(const double* p, const double* n, const double* q)
{
    double s = n[0] * (p[0] - q[0]);
    s = s + n[1] * (p[1] - q[1]);
    s = s + n[2] * (p[2] - q[2]);
    return std::fabs(s);
}

// ---- 1402059a0 -----------------------------------------------------------------------------------

// @src 1402059a0  surface normal list of one object
void object_surface_normals(model::LLOBJ_DATA* obj, char flag)
{
    namespace gk = model::geo_kw;
    if (!gk::has_surface_flag(*obj)) { // 140160d90
        return;
    }
    // every 140198620 call below is surface 0 (a fresh index variable of 0 each time)
    point_list(addr_at_28(gk::surface(*obj, 0)))->clear(); // 140158e60, 1402581b0
    if (flag != 0) {
        // 140257c80(surface + 0x28, obj + 0x2e80)
        point_list(addr_at_28(gk::surface(*obj, 0)))
            ->copy_from(*point_list(static_cast<std::byte*>(static_cast<void*>(obj)) + 0x2e80));
    }
    model::OBJ_GEO3D* s = gk::surface(*obj, 0);
    const double size = s->char_length(); // vtable +0x48 (slot 9)
    Cls_140257340* node = nullptr;        // local_78
    char found = 0;                       // local_98
    const std::int32_t nelem = gk::surface(*obj, 0)->elem_count();          // 140003500
    double* coords = gk::geo3d_coords(gk::surface(*obj, 0));                 // 140006410
    std::int32_t* elems = gk::geo3d_elems(gk::surface(*obj, 0));             // 140005510
    std::int32_t* flags = static_cast<std::int32_t*>(ptr_at_20(gk::surface(*obj, 0))); // 140158e80
    Cls_140257340List* list = point_list(addr_at_28(gk::surface(*obj, 0))); // 140158e60
    for (std::int32_t i = 0; i < nelem; i = i + 1) {
        if (elems[i * 4 + 3] > 0) {
            continue; // quadrilateral
        }
        const std::int32_t n0 = elems[i * 4] - 1;
        const std::int32_t n1 = elems[i * 4 + 1] - 1;
        const std::int32_t n2 = elems[i * 4 + 2] - 1;
        double normal[3]; // local_b0
        triangle_normal(coords + n0 * 3, coords + n1 * 3, coords + n2 * 3, normal); // 14023e050
        const double copy[3] = {normal[0], normal[1], normal[2]};                  // local_50
        const double len = objcheck::ext::fun_140239a30(copy);                      // 140239a30
        if (k_min_normal > len) { // 140205ca2: COMISD; JBE goes on (NaN goes on)
            continue;
        }
        node = nullptr;
        found = 0;
        for (std::int32_t k = 0; k < model::llobj::int_at_10(list); k = k + 1) { // 140158e70
            node = list->nth(k + 1); // 140258090
            if (node == nullptr) {
                continue;
            }
            const double d2 = vec3_dist2(normal, node->direction); // 14023b850
            if (k_same_direction > d2) {
                const double d = plane_distance(coords + n0 * 3, node->direction, node->point); // 14023c770
                if (k_plane_tol * size > d) { // 140205d7a: 0.001 * size
                    found = 1;
                    break;
                }
            }
        }
        if (found == 0) {
            const std::int32_t id = point_list_free_id(list); // 140258260
            node = list->add(id);                               // 140257e90
            point_node_set_point(node, coords + n0 * 3);        // 140257510
            point_node_set_direction(node, normal);             // 140257590
        }
        flags[i] = node->id + 600; // 140205e11: ADD EDX,0x258
    }
}

} // namespace def::dbio::gendb
