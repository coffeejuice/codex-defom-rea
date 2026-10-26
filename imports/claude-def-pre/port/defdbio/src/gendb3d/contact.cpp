// gendb3d/contact.cpp: BCCDEF contact codes and LLBND attach words, object -> LLBND record
// (packet p4-05; docs/re/dbmap/objarrays.md section 6).
#include <defdbio/gendb3d/obj3d.hpp>

#include <defmodel/layout/cls_14017a6c0.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defrt/stdlib/refcount.hpp>

#include "gendb3d/access.hpp"
#include "gendb3d/ext_obj3d.hpp"

namespace def::dbio::g3d {

// @src 140166e60  2D: over the boundary loops (obj+0xbd8 of the records at obj+0xbe0, node list
//                 +8) find the node nearest to (x, y) in LLCRZ (obj+0x8a8, 2 per node; index
//                 clamped to [0, nodes-1]); *node = 1-based node, *loop = its loop, *px/*py = (x, y).
//                 False when there are no loops, no nodes or no coordinates
bool nearest_loop_node(LLOBJ_DATA* obj, double x, double y, std::int32_t* node, std::int32_t* loop,
                       double* px, double* py)
{
    double best = 0.0;
    *px = 0.0;
    *py = 0.0;
    *node = -1;
    *loop = 0;
    if (fld<std::int32_t>(obj, 0xbd8) < 1) return false;
    if (fld<std::int32_t>(obj, 0x880) < 1) return false;
    const double* rz = fld<double*>(obj, 0x8a8);
    if (rz == nullptr) return false;
    for (std::int32_t l = 0; l < fld<std::int32_t>(obj, 0xbd8); l = l + 1) {
        char* rec = at(obj, 0xbe0 + static_cast<std::ptrdiff_t>(l) * 0x30);
        const std::int32_t* list = fld<std::int32_t*>(rec, 8);
        if (list == nullptr) continue;
        for (std::int32_t k = 0; k < fld<std::int32_t>(rec, 0); k = k + 1) {
            std::int32_t n = list[k] - 1;
            if (n < 0) n = 0;
            if (fld<std::int32_t>(obj, 0x880) <= n) n = fld<std::int32_t>(obj, 0x880) - 1;
            const double dx = x - rz[n * 2];
            const double dy = y - rz[n * 2 + 1];
            double d = dx * dx;
            d = d + dy * dy;
            if (l == 0 && k == 0) {
                best = d;
                *px = x;
                *py = y;
                *node = n + 1;
                *loop = 0;
            }
            else if (best > d) {
                best = d;
                *px = x;
                *py = y;
                *node = n + 1;
                *loop = l;
            }
        }
    }
    return true;
}

// @src 14017e190  BCCDEF codes -object (3D also -(200 + object)) become -LLBND record: one record
//                 -> its record; 2D several loops -> first record + loop of the nearest loop node;
//                 3D several surfaces -> first record whose type word (LLBND word 0) is < 3.0.
//                 Then LLBND attach words (3D word 0x3e if word 0xf == 5; 2D 0x2f / 7): object ->
//                 its first record
void convert_contact_codes(model::Cls_14017a6c0* objs, double* llcrz, std::int32_t* iibcd, double* llbnd,
                           const std::int32_t* rec_w)
{
    DB_STEP_DATA* m = objs->owner;
    const std::int32_t nnp = numnp(objs->owner);
    std::int32_t dim = 3;
    if (rt::dimension_is_2(objs->owner)) dim = 2;
    for (std::int32_t n = 0; n < nnp; n = n + 1) {
        for (std::int32_t c = 0; c < dim; c = c + 1) {
            std::int32_t code = iibcd[n * dim + c];
            if (code >= 0) continue;
            code = -code;
            bool all = false;
            if (rt::dimension_is_3(objs->owner) && 200 < code && code < 300) {
                code = code - 200;
                all = true;
                for (std::int32_t o = 0; o < dim; o = o + 1) {
                    if (o != c && iibcd[n * dim + o] == -code) {
                        iibcd[n * dim + o] = 0;
                    }
                }
            }
            model::LLOBJ_DATA* obj = ext::f14017aaf0(&m->objects, code);
            if (obj == nullptr) continue;
            std::int32_t first = fld<std::int32_t>(obj, 0x3588);
            const std::int32_t last = fld<std::int32_t>(obj, 0x358c);
            if (first == last) {
                iibcd[n * dim + c] = -first;
                if (all) iibcd[n * dim + c] = iibcd[n * dim + c] - 200;
            }
            else if (!rt::dimension_is_3(objs->owner)) {
                const double* p = llcrz + n * dim;
                std::int32_t node = 0;
                std::int32_t loop = 0;
                double px = 0.0;
                double py = 0.0;
                if (nearest_loop_node(obj, p[0], p[1], &node, &loop, &px, &py)) {
                    iibcd[n * dim + c] = -(first + loop);
                }
            }
            else {
                for (; first <= last; first = first + 1) {
                    if (3.0 > llbnd[(first - 1) * *rec_w]) { // DAT_14051f1c8
                        iibcd[n * dim + c] = -first;
                        if (all) iibcd[n * dim + c] = iibcd[n * dim + c] - 200;
                        break;
                    }
                }
            }
        }
    }
    const std::int32_t nb = n_boundaries(objs->owner);
    std::int32_t w_att = 0x3e;
    std::int32_t w_typ = 0xf;
    if (rt::dimension_is_2(objs->owner)) {
        w_att = 0x2f;
        w_typ = 7;
    }
    for (std::int32_t r = 1; r <= nb; r = r + 1) {
        const std::int32_t o = static_cast<std::int32_t>(llbnd[(r - 1) * *rec_w + w_att]);
        if (static_cast<std::int32_t>(llbnd[(r - 1) * *rec_w + w_typ]) == 5 && 0 < o) {
            model::LLOBJ_DATA* obj = ext::f14017aaf0(&m->objects, o);
            if (obj != nullptr) {
                llbnd[(r - 1) * *rec_w + w_att] = static_cast<double>(fld<std::int32_t>(obj, 0x3588));
            }
        }
    }
}

} // namespace def::dbio::g3d
