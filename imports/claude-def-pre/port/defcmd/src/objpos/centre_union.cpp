// defcmd/src/objpos/centre_union.cpp: surface union FUN_1402fa880 (append the points and faces
// of one _OBJ_GEO3D to another), used by the drop packet (OBJPOS_drop.md). Decompile:
// deform-rea-work/out/p67/objpos.c. Integer and copy code only.
#include <defcmd/objpos_centre.hpp>

#include <cstdint>
#include <cstdlib>
#include <cstring>

#include <defdbio/gendb/gate_ports.hpp>
#include <defmodel/layout/obj_geo3d.hpp>

namespace def::cmd::objpos {

using model::OBJ_GEO3D;

// @src 1402fa880  u += s: resize u to (nP_u + nP_s, nF_u + nF_s), then copy s behind
void fun_1402fa880(OBJ_GEO3D* u, OBJ_GEO3D* s)
{
    std::int32_t np0 = u->node_count();  // 1400034d0
    std::int32_t nf0 = u->elem_count();  // 140003500
    std::int32_t np = np0 + s->node_count();
    std::int32_t nf = nf0 + s->elem_count();
    fun_1402f9780(u, &np, &nf);
    fun_1402fa940(u, s, &np0, &nf0);
}

// 1402f9780: ported in defdbio/gendb/gate_ports.hpp (geo3d_resize)
void fun_1402f9780(OBJ_GEO3D* g, const std::int32_t* np, const std::int32_t* nf) { dbio::gendb::geo3d_resize(g, np, nf); }

// 1402f97c0: ported in defdbio/gendb/gate_ports.hpp (geo3d_resize_coords)
void fun_1402f97c0(OBJ_GEO3D* g, const std::int32_t* n) { dbio::gendb::geo3d_resize_coords(g, n); }

// 1402f9810: ported in defdbio/gendb/gate_ports.hpp (geo3d_resize_elements)
void fun_1402f9810(OBJ_GEO3D* g, const std::int32_t* n) { dbio::gendb::geo3d_resize_elements(g, n); }

// @src 1402fa940  copy s's points to u from point *np0 on, its faces from face *nf0 on (node
// numbers shifted by *np0: v + np0 when v >= 1, v - np0 otherwise) and its face flags
void fun_1402fa940(OBJ_GEO3D* u, OBJ_GEO3D* s, const std::int32_t* np0, const std::int32_t* nf0)
{
    std::int32_t n = s->node_count();
    const double* p = s->coords.begin();  // 14026a3c0
    for (std::int32_t i = 0; i < n; ++i)
        for (std::int32_t j = 0; j < 3; ++j)
            u->coords.mutable_at(static_cast<std::size_t>((*np0 + i) * 3 + j)) = p[i * 3 + j];  // 140005e70
    n = s->elem_count();
    const std::int32_t* f = s->elems.begin();  // 1402fe9d0
    for (std::int32_t i = 0; i < n; ++i) {
        for (std::int32_t k = 0; k < 4; ++k) {
            const std::int32_t v = f[k + i * 4];
            std::int32_t& d = u->elems.mutable_at(static_cast<std::size_t>(k + (*nf0 + i) * 4));  // 140005100
            d = v < 1 ? v - *np0 : v + *np0;
        }
        u->elem_flags[*nf0 + i] = s->elem_flags[i];
    }
}

} // namespace def::cmd::objpos
