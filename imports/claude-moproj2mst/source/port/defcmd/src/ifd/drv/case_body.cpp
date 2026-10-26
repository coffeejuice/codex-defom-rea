// OBJPOS interference modes 2, 12, 102, 112: the case bodies of the command dispatcher 140378e00
// (3D cases 1/0xb/0x65/0x6f, 2D the same) and the automatic target search with its candidate list
// 140378c80 / 140378c20 (packet p67-2). Spec: docs/re/commands/OBJPOS_interference.md sections
// 4 and 6; decompile out/bm_cmd.c lines 699-801 (3D) and 1238-1302 (2D).
//
// Tokens (p67-1's Args.v[k] = token k + 2): 3D v[0] depth, v[1..3] direction, v[4] target
// (0 = automatic search); 2D v[0] area percent, v[1] target, v[2..3] direction.
#include <cstdint>
#include <cstdlib>
#include <vector>

#include <defcmd/ifd/ifd_internal.hpp>
#include <defcmd/objpos_ifd.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj/object_list.hpp>
#include <defmodel/step/db_step.hpp>
#include <defrt/ftn/helpers.hpp>
#include <defrt/stdlib/legacy_printf.hpp>
#include <defrt/rdata.hpp>

#include "vec_interim.hpp"

namespace def::model {
bool llobj_not_kind7(const LLOBJ_DATA* o);  // 140160f10
}

namespace def::cmd::ifd {

namespace crt = def::rt::crt;

namespace {

template <class T, class C>
T& at(const C* base, std::size_t off)
{
    return *reinterpret_cast<T*>(const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(base)) + off);
}

bool is_2d(const DB_STEP_DATA* m) { return at<std::int32_t>(m, 0x1c) == 2; }  // 1400015e0

// rdata 14052c598  1e29 (start of the automatic search)
constexpr double kSearchStart = def::rt::rdata::d_14052c598;

// FUN_1401d6ea0: object by number (14017aaf0 on model+0x1380)
LLOBJ_DATA* object_by_number(DB_STEP_DATA* m, std::int32_t no)
{
    return model::object_list_find(&m->objects, no);
}

} // namespace

// @src 140378c20  true when v is one of list[0..n-1] (raw values)
bool in_coupled_list(const std::int32_t* list, std::int32_t n, std::int32_t v)
{
    for (std::int32_t i = 0; i < n; ++i)
        if (list[i] == v) return true;
    return false;
}

// @src 140378c80  candidates of the automatic target search, in list order (spec 6). The vector
// helpers 1403824e0 (clear) / 140382440 (push_back) are std::vector here.
void auto_search_candidates(DB_STEP_DATA* m, const std::int32_t* mover_no, const std::int32_t* coupled,
                            std::int32_t n_coupled, std::vector<LLOBJ_DATA*>& out)
{
    LLOBJ_DATA* a = object_by_number(m, *mover_no);
    out.clear();
    for (std::int32_t k = 0; k < m->u1378; ++k) {
        auto* c = static_cast<LLOBJ_DATA*>(model::step_object_at(*m, k + 1));  // 1401d6ed0
        if (c == nullptr || !model::llobj_not_kind7(c)) continue;
        if (at<std::int32_t>(c, 8) == *mover_no) continue;
        if (at<std::int32_t>(c, 0xc) == 5) continue;
        if (in_coupled_list(coupled, n_coupled, at<std::int32_t>(c, 8))) continue;
        const std::int32_t type = at<std::int32_t>(c, 0xc);
        const std::int32_t mesh = at<std::int32_t>(c, 0x87c);
        if (!(type == 0 || mesh != 0)) continue;
        if (!(type != 0 || mesh != 0 || ext::has_geometry(c))) continue;
        if (!(at<std::int32_t>(a, 0x87c) != 0 || mesh != 0)) continue;
        out.push_back(c);
    }
}

} // namespace def::cmd::ifd

namespace def::cmd::objpos {

using ifd::at;

// part of @src 140378e00  (OBJPOS 3D cases 1/0xb/0x65/0x6f and 2D cases 1/0xb/0x65/0x6f)
int interference_mode(model::DB_STEP_DATA& m, model::LLOBJ_DATA& obj, Args& a, State& st)
{
    namespace crt = def::rt::crt;
    model::DB_STEP_DATA* mp = &m;
    model::LLOBJ_DATA* mover = &obj;
    model::LLOBJ_DATA* target = nullptr;
    // local_5cc8: the distance out-parameter; the binary leaves it uninitialised until the first
    // kernel call writes it (a candidate rejected by 1401a3aa0's early checks keeps the old value)
    double dist = 0.0;
    double* dir = nullptr;
    if (!ifd::is_2d(mp)) {
        dir = &a.v[1];
        const std::int32_t tgt = static_cast<std::int32_t>(a.v[4]);
        if (tgt == 0) {
            std::vector<model::LLOBJ_DATA*> cand;
            ifd::auto_search_candidates(mp, &at<std::int32_t>(mover, 8), a.coupled, a.n_coupled, cand);
            double best = ifd::kSearchStart;
            model::LLOBJ_DATA* sel = nullptr;
            for (std::size_t i = 0; i < cand.size(); ++i) {
                model::LLOBJ_DATA* c = cand[i];
                ifd::fun_1401a0b00(mp, mover, c, dir, &a.v[0], &dist, g_last_disp, 1);  // result ignored
                double u[3];
                double w[3];
                ifd::iv::copy3(dir, u);
                ifd::iv::copy3(g_last_disp, w);
                def::rt::ftn::vec3_normalize(u);  // 14023b720
                def::rt::ftn::vec3_normalize(w);
                if (ifd::iv::dot(u, w) < 0.0) dist = -dist;  // xor with the sign bit (14051bec0)
                if (dist < best) {
                    best = dist;
                    sel = c;
                }
            }
            target = sel;
        } else {
            const std::int32_t no = tgt < 0 ? -tgt : tgt;  // 140484ab8
            target = ifd::object_by_number(mp, no);
            if (target == nullptr) return 3;
            if (at<std::int32_t>(target, 0xc) == 7) {
                crt::printf("ERROR  : Object %d Undefined\n", at<std::int32_t>(target, 8));
                return 3;
            }
            if (!ifd::ext::has_geometry(target) && at<std::int32_t>(target, 0x87c) == 0) {
                crt::printf("ERROR  : No Mesh or Die Defined for Object %d\n", at<std::int32_t>(target, 8));
                return 3;
            }
        }
    } else {
        dir = &a.v[2];
        const std::int32_t tgt = static_cast<std::int32_t>(a.v[1]);
        const std::int32_t no = tgt < 0 ? -tgt : tgt;
        target = ifd::object_by_number(mp, no);
        if (target == nullptr) return 3;
        if (at<std::int32_t>(target, 0xc) == 7) {
            crt::printf("ERROR  : Object %d Undefined\n", at<std::int32_t>(target, 8));
            return 3;
        }
        if (!ifd::ext::has_die_geometry(target) && at<std::int32_t>(target, 0x87c) == 0) {
            crt::printf("ERROR  : No Mesh or Die Defined for Object %d\n", at<std::int32_t>(target, 8));
            return 3;
        }
    }
    const char flag = (a.mode == 2 || a.mode == 12) ? 1 : 0;
    void* simctl = &at<unsigned char>(mp, 0x750);
    const double t_old = ifd::sim_time(simctl);
    int one = 1;
    char tchg = 0;
    const bool ok = fun_14019fa00(mp, mover, target, dir, &a.v[0], &dist, g_last_disp, flag,
                                  &a.n_coupled, a.coupled, &tchg, &one);
    if (target == nullptr) {
        // the original reads target+8 for the messages below: an access violation when the
        // automatic search selected nothing (spec 6, last quirk)
        std::abort();
    }
    if (!ok) {
        crt::printf("Object %d to Object %d interference positioning failed.\n",
                    at<std::int32_t>(mover, 8), at<std::int32_t>(target, 8));
        st.moved = false;
    } else if (tchg == 0) {
        if (!ifd::is_2d(mp))
            crt::printf("Object %d has been interference positioned w.r.t. Object %d.\n",
                        at<std::int32_t>(mover, 8), at<std::int32_t>(target, 8));
        else
            crt::printf("Object %d is interference positioned w.r.t. Object %d.\n",
                        at<std::int32_t>(mover, 8), at<std::int32_t>(target, 8));
        st.moved = true;
    } else {
        const double t_new = ifd::sim_time(simctl);
        ifd::fun_14019ed10(mp);
        crt::printf("Object %d has been interference positioned w.r.t. Object %d.\n",
                    at<std::int32_t>(mover, 8), at<std::int32_t>(target, 8));
        crt::printf("       Current time has been changed to %g from %g\n", t_new, t_old);
        st.moved = false;
    }
    return 0;
}

} // namespace def::cmd::objpos
