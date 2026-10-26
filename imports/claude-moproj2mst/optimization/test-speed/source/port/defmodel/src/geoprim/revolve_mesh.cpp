// GEOPRIM revolve, mesh path 1402ce310 (packet p9-05): the 2D mesh boundary chain is revolved
// into quadrilateral strips, the shell is built in a scratch object and copied to the target.
// Spec: docs/re/commands/GEOPRIM_REVOLVE.md §4. Decompile: deform-rea-work/out/p905/ce310.c.
#include <defmodel/geoprim/revolve.hpp>

#include <defmodel/bc/boundary_topology.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/geo_kw/misc_model.hpp>
#include <defmodel/geoprim/extrude.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/linked/record_e8_list.hpp>
#include <defmodel/members/ftn_geom.hpp>

#include <bit>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <list>
#include <new>

namespace def::model::geoprim {

namespace {

constexpr double kEps12 = 1e-12;   // DAT_14051f260
constexpr double kEps6 = 1e-6;     // DAT_14051f1b8
constexpr double kEps8 = 1e-8;     // DAT_14051bed0
constexpr double k360 = 360.0;     // DAT_14051f210

template <class T>
T& at(void* base, std::size_t off) { return *reinterpret_cast<T*>(static_cast<std::byte*>(base) + off); }

// Boundary record 0 of an object (LLOBJ_DATA+0xbe0): face count +0x10, faces +0x18.
std::int32_t& face_count(LLOBJ_DATA* o) { return at<std::int32_t>(o, 0xbf0); }
std::int32_t* face_ptr(LLOBJ_DATA* o) { return at<std::int32_t*>(o, 0xbf8); }
std::int32_t node_count(LLOBJ_DATA* o) { return at<std::int32_t>(o, 0x880); }
double* coords(LLOBJ_DATA* o) { return p905_ext().node_coords(o->s8a0); }   // 1401808d0(obj+0x8a0)

// 1401599e0 pushes the 8 bytes at the int's address into the 8-byte-value list: the int is the
// low half (the high half is the next stack slot in the binary; 0 here).
double flag_value(std::int32_t v)
{
    return std::bit_cast<double>(static_cast<std::uint64_t>(static_cast<std::uint32_t>(v)));
}

} // namespace

// @src 1402ce310
bool fun_1402ce310(DEF_GEOPRIM* prim)
{
    std::int32_t t = 2;                              // local_168
    char full = 0;                                   // local_194
    char flip = 0;                                   // local_160
    double pt[3] = {0.0, 0.0, 0.0};                  // local_1d0
    double dir[3] = {0.0, 0.0, 1.0};                 // local_1b0
    prim->u23e4 = 0;
    prim->u23e0 = 0;
    prim->u23e8 = 0;
    prim->u23ec = 0;
    if (!(kEps12 < prim->angle || kEps12 == prim->angle)) return false;
    const std::int32_t nn = fun_1402e9b50(prim);     // local_140
    const double* xy = fun_1402e9b90(prim);          // local_148
    if (nn < 1 || xy == nullptr) return false;
    prim->angle = (k360 <= prim->angle) ? k360 : prim->angle;
    if (std::fabs(prim->angle - k360) < kEps8) full = 1;
    std::int32_t nsec = prim->sections;              // local_190
    if (prim->sections < 1) return false;
    if (prim->int_lists.n < 1) fun_1402cc080(prim);
    if (prim->int_lists.n < 1) {
        std::fprintf(stdout, "cross section mesh boundary can not be constructed.\n");   // iob+0x30
        return false;
    }
    const double step = prim->angle / static_cast<double>(nsec);   // local_1e0
    double start = 0.0;                              // local_180
    if (prim->revolve != nullptr) {
        fun_1402d9bb0(prim, dir, &t, pt);
        start = prim->revolve->start;
    }
    std::int32_t* ctx = static_cast<std::int32_t*>(prim->ctx);   // node -> 2D node (0-based)
    double* xyz = static_cast<double*>(std::malloc(static_cast<std::size_t>((nsec + 1) * nn) * 0x18));
    if (xyz == nullptr) throw std::bad_alloc();
    const RevolveRec* rev = prim->revolve;
    for (std::int32_t i = 0; i < nn; ++i) {
        double* q = xyz + 3 * static_cast<std::int64_t>(i);
        const double x = xy[2 * i], y = xy[2 * i + 1];
        if (t == 2) {
            if (rev == nullptr || rev->axis_kind != 3) { q[0] = 0.0; q[1] = x; q[2] = y; }
            else { q[0] = y; q[1] = 0.0; q[2] = x; }
        } else if (t == 1) {
            if (rev == nullptr || rev->axis_kind != 2) { q[0] = x; q[1] = y; q[2] = 0.0; }
            else { q[0] = 0.0; q[1] = x; q[2] = y; }
        } else if (t == 0) {
            if (rev == nullptr || rev->axis_kind != 3) { q[0] = x; q[1] = y; q[2] = 0.0; }
            else { q[0] = y; q[1] = 0.0; q[2] = x; }
        }                                            // t 3: not written (heap contents)
        if (ctx != nullptr) ctx[i] = i;
    }
    std::int32_t cnt = nn;                           // local_228
    prim->u23e4 = nn;
    prim->u23e0 = nn;
    prim->u23e8 = nn;
    prim->u23f0 = pt[0];
    prim->u23f8 = pt[1];
    prim->u2400 = pt[2];
    prim->u2408 = dir[0];
    prim->u2410 = dir[1];
    prim->u2418 = dir[2];
    prim->u2420 = 0.0;
    prim->u2428 = prim->angle;
    if (kEps6 < std::fabs(start)) {
        for (std::int32_t i = 0; i < nn; ++i) fun_140325691(xyz + 3 * static_cast<std::int64_t>(i), pt, dir, &start);
        prim->u2420 = start;
    }
    if (prim->revolve != nullptr) {
        const char keep = 1, invert = 1;             // local_110, local_10f
        flip = fun_1402d6bf0(prim, dir, pt, xyz, &invert, &keep);
    }
    const std::int32_t nlay = nsec;                  // local_1d8
    IntArray* chain = &prim->int_lists.items[0];
    std::int32_t nb = chain->n;                      // local_164
    if (fun_1402e91d0(chain)) {                      // closed chain: drop the repeated last node
        --nb;
        chain->n = chain->n - 1;
    }
    for (std::int32_t s = 1; s < nlay; ++s) {
        double a = step * static_cast<double>(s);    // local_1e8
        for (std::int32_t i = 0; i < nb; ++i) {
            const std::int32_t id = chain->data[i] - 1;
            double q[3];
            std::memcpy(q, xyz + 3 * static_cast<std::int64_t>(id), 0x18);
            fun_140325691(q, pt, dir, &a);
            std::memcpy(xyz + 3 * static_cast<std::int64_t>(cnt), q, 0x18);
            if (ctx != nullptr) ctx[cnt] = id;
            ++cnt;
        }
    }
    prim->u23e4 = (nlay / 2) * nb + prim->u23e4;
    prim->u23e8 = nlay * nb + prim->u23e8;
    prim->u23ec = nb;
    if (full == 0) {                                 // last layer: the whole 2D mesh again
        double a = prim->angle;
        for (std::int32_t i = 0; i < nn; ++i) {
            double q[3];
            std::memcpy(q, xyz + 3 * static_cast<std::int64_t>(i), 0x18);
            fun_140325691(q, pt, dir, &a);
            std::memcpy(xyz + 3 * static_cast<std::int64_t>(cnt), q, 0x18);
            if (ctx != nullptr) ctx[cnt] = i;
            ++cnt;
        }
    }
    const std::int32_t ne = fun_1402e9b70(prim);     // local_18c
    std::int32_t nq = nb * nsec + ne * 2;            // local_178
    std::int32_t* quads = static_cast<std::int32_t*>(std::malloc(static_cast<std::size_t>(nq) * 16));
    if (quads == nullptr && nq != 0) throw std::bad_alloc();
    nq = fun_1402bc980(prim, &nsec, xyz, quads, &flip, &full);

    // scratch model and object (operator new 0x3408 / 0x3f28)
    DB_STEP_DATA* model = new DB_STEP_DATA(p905_ext().obj_dimension(prim->obj));   // 140220520
    p905_ext().model_copy_u15f8(model, prim->model->u15f8);                       // 1401d7320
    LLOBJ_DATA* tmp = new LLOBJ_DATA(model);                                       // 14015ec80
    p905_ext().mesh_alloc(tmp, cnt, 0, 4, model->u15f8, 0);                        // 140202700
    for (std::int32_t i = 0; i < cnt; ++i)
        std::memcpy(coords(tmp) + 3 * static_cast<std::int64_t>(i), xyz + 3 * static_cast<std::int64_t>(i), 0x18);
    bc::set_boundary_count(*tmp, 1);                 // 140163300
    bc::set_boundary_face_count(*tmp, nq, 0);        // 140200a70
    for (std::int32_t f = 0; f < nq; ++f) std::memcpy(face_ptr(tmp) + 4 * f, quads + 4 * f, 0x10);
    // nodes on the axis (x == y == 0) take the 2D node's id
    for (std::int32_t j = 1; j <= nq * 4; ++j) {
        const std::int32_t id = face_ptr(tmp)[j - 1];
        if (coords(tmp)[(id - 1) * 3] == 0.0 && coords(tmp)[(id - 1) * 3 + 1] == 0.0)
            face_ptr(tmp)[j - 1] = ctx[id - 1] + 1;
    }
    std::list<std::int32_t> keep;                    // local_200 (CowList, 140159860)
    for (std::int32_t f = 1; f <= nq; ++f) {
        const std::int32_t* F = face_ptr(tmp) + 4 * (f - 1);
        const double* c = coords(tmp);
        bool any = false;
        for (std::int32_t k = 0; k < 4 && !any; ++k)
            any = c[(F[k] - 1) * 3] != 0.0 || c[(F[k] - 1) * 3 + 1] != 0.0;
        if (any) keep.push_back(f);                  // 1401599e0
    }
    std::int32_t* out = static_cast<std::int32_t*>(std::malloc(keep.size() * 16));
    if (out == nullptr && !keep.empty()) throw std::bad_alloc();
    for (std::int32_t k = 1; k <= static_cast<std::int32_t>(keep.size()); ++k) {
        const std::int32_t f = keep.front();         // 140159a40
        const std::int32_t* F = face_ptr(tmp) + 4 * (f - 1);
        const std::int32_t a = F[0], b = F[1], c = F[2], d = F[3];
        std::int32_t* o = out + 4 * (k - 1);
        if (a == b) {
            o[0] = a; o[1] = c; o[2] = d; o[3] = b;
            if (prim->u24fc == 0) prim->u24fc = 1;
            prim->l2518.push_back(flag_value(0));
        } else if (c == d) {
            o[0] = c; o[1] = a; o[2] = b; o[3] = d;
            if (prim->u24fc == 0) prim->u24fc = 1;
            prim->l2518.push_back(flag_value(0));
        } else if (b == c) {
            o[0] = b; o[1] = a; o[2] = d; o[3] = c;
            if (prim->u24fc == 0) prim->u24fc = 1;
            prim->l2518.push_back(flag_value(0));
        } else {
            o[0] = a; o[1] = b; o[2] = c; o[3] = d;
            if (prim->u24fd == 0) prim->u24fd = 1;
            prim->l2518.push_back(flag_value(1));
        }
        keep.push_back(keep.front());                // 140220700 push_back(front)
        keep.pop_front();                            // 140159970
    }
    face_count(tmp) = static_cast<std::int32_t>(keep.size());
    std::memcpy(face_ptr(tmp), out, keep.size() * 16);
    p905_ext().surface_update(geo_kw::surface(*tmp, 0));                              // 1402fc1b0
    p905_ext().mesh_alloc(prim->obj, node_count(tmp), 0, 4, prim->model->u15f8, 0);
    std::memcpy(coords(prim->obj), coords(tmp), static_cast<std::size_t>(node_count(tmp) * 3) * 8);
    bc::set_boundary_count(*prim->obj, 1);
    bc::set_boundary_face_count(*prim->obj, face_count(tmp), 0);
    std::memcpy(face_ptr(prim->obj), face_ptr(tmp), static_cast<std::size_t>(face_count(tmp) * 4) * 4);
    keep.clear();                                    // 14015a620
    std::free(out);
    ext().obj_delete(tmp);                           // 14019a290(tmp, 1)
    ext().model_delete(model);                       // 140158fc0(model, 1)

    auto& l = *reinterpret_cast<linked::Cls_140258580List*>(reinterpret_cast<std::byte*>(prim->obj) + 0x20a0);
    auto* r = reinterpret_cast<std::byte*>(geo_kw::wpaxis_get(l, 1));   // 140259bc0
    const std::int32_t zero = 0;
    std::memcpy(r + 4, &zero, 4);
    if (prim->revolve == nullptr) {
        std::memcpy(r + 8, pt, 0x18);
        std::memcpy(r + 0x20, dir, 0x18);
    } else {
        std::memcpy(r + 8, prim->revolve->axis_pt, 0x18);
        std::memcpy(r + 0x20, prim->revolve->axis_dir, 0x18);
    }
    std::free(quads);
    std::free(xyz);
    return true;
}

} // namespace def::model::geoprim
