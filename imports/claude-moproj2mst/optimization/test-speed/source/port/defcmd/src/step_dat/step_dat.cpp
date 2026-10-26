// STEP.DAT / DEF_INT_NO_DB.DAT writer FUN_1402ac8a0 (packet p9-ac8a0); defcmd/step_dat.hpp.
// Decompile and listing: deform-rea-work/out/p9ac/top.c, asm.txt. Doubles go through
// def::rt::crt::fprintf ("%15.7E" with the old CRT's three exponent digits).
//
// Callee owners (every accessor allocates the array on demand and syncs the owner model, as the
// original): node set obj+0x8a0 -> defmodel node_access (1401808d0, 1401809c0), p407 getters
// (140180940, 140180c40, 140180f90, 140181320) and kw-g2sv's svkw accessors (1401812a0,
// 1401814b0, 140181530, 1401815b0, and this packet's 140180eb0, 1401813a0); element set
// obj+0x9e0 -> defdbio gendb3d (140183cb0, 140183d00), p407 (140183d50, 140183e50, 140183fd0,
// 1401846d0), objcheck (140184050) and svkw (1401840d0, 140184150, 140184520, 1401845b0,
// 1401849a0, 140184a30, and this packet's 140184400, 140184490, 140184760).
#define _CRT_SECURE_NO_WARNINGS
#include <defcmd/step_dat.hpp>

#include <cstddef>
#include <cstdint>
#include <cstdio>

#include <share.h>

#include <defdbio/gendb/counts_frame.hpp>
#include <defdbio/gendb/gate_ports.hpp>
#include <defdbio/gendb3d/obj3d.hpp>
#include <defdbio/objcheck/objcheck.hpp>
#include <defkey/kw_g2sv.hpp>
#include <defmodel/hdrstats/object_stats.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj/cls_14017ef20.hpp>
#include <defmodel/node_access.hpp>
#include <defmodel/statevar_kw/statevar_kw.hpp>
#include <defmodel/step/name_arrays.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

namespace def::cmd::step_dat {

namespace {

using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;
namespace crt = def::rt::crt;
namespace svkw = def::model::svkw;
namespace p407 = def::model::p407;

template <class T, class B>
T& at(B* base, std::size_t off)
{
    return *reinterpret_cast<T*>(reinterpret_cast<unsigned char*>(base) + off);
}

// @src 14047b934  fopen wrapper: _fsopen(name, mode, _SH_DENYNO)
std::FILE* fsopen(const char* name, const char* mode) { return ::_fsopen(name, mode, _SH_DENYNO); }

// the "%15.7E  " rows with "\n" after each of `n` rows of `w` values (inline loops of 1402ac8a0)
void rows(std::FILE* f, const double* v, std::int32_t n, std::int32_t w)
{
    for (std::int32_t i = 0; i < n; i = i + 1) {
        for (std::int32_t k = 0; k < w; k = k + 1) crt::fprintf(f, "%15.7E  ", v[w * i + k]);
        crt::fprintf(f, "\n");
    }
}

}  // namespace

// @src 1402ac8a0
void write_step_dat(DB_STEP_DATA* m, const char* db, LLOBJ_DATA* obj)
{
    if (obj == nullptr) return;
    std::FILE* f = fsopen("STEP.DAT", "wt");  // 14053c8cc
    if (f == nullptr) return;
    void* nodes = &at<unsigned char>(obj, 0x8a0);  // Cls_14017ef20
    void* elems = &at<unsigned char>(obj, 0x9e0);  // Cls_140182880
    // obj+0x880 / +0x884 are re-read on every loop test, as the original
    const auto NN = [obj] { return at<std::int32_t>(obj, 0x880); };
    const auto NE = [obj] { return at<std::int32_t>(obj, 0x884); };

    crt::fprintf(f, "%s\n", db);
    crt::fprintf(f, "%d\n", at<std::int32_t>(m, 0x840));
    crt::fprintf(f, "%d\n", at<std::int32_t>(m, 0x744));
    crt::fprintf(f, "%d\n", at<std::int32_t>(obj, 8));
    crt::fprintf(f, "%d\n", NN());
    const double* xy = def::model::fun_1401808d0(static_cast<def::model::Cls_14017ef20*>(nodes));  // 1401808d0
    for (std::int32_t i = 0; i < NN(); i = i + 1) crt::fprintf(f, "%15.7E  %15.7E\n", xy[i << 1], xy[i * 2 + 1]);

    crt::fprintf(f, "%d\n", NE());
    const std::int32_t* conn = def::dbio::g3d::element_connectivity(elems);  // 140183cb0
    const std::int32_t npe = 4;                                                // local_ac
    for (std::int32_t i = 0; i < NE(); i = i + 1) {
        for (std::int32_t k = 0; k < npe; k = k + 1) crt::fprintf(f, "%d  ", conn[npe * i + k]);
        crt::fprintf(f, "\n");
    }
    crt::fprintf(f, "%d\n", NE());
    const std::int32_t* grp = def::dbio::g3d::element_material_groups(elems);  // 140183d00
    for (std::int32_t i = 0; i < NE(); i = i + 1) crt::fprintf(f, "%d\n", grp[i]);

    // effective strain by its storage option m+0x88c (0 element, 2 element + nodal, 3 per
    // integration point from obj+0xa28)
    if (at<std::int32_t>(m, 0x88c) == 0) {
        crt::fprintf(f, "%d\n", NE());
        const double* v = p407::elem_var_2b(elems);  // 140183fd0
        for (std::int32_t i = 0; i < NE(); i = i + 1) crt::fprintf(f, "%15.7E\n", v[i]);
    } else if (at<std::int32_t>(m, 0x88c) == 2) {
        crt::fprintf(f, "%d  %d\n", NE(), NN());
        const double* v = p407::elem_var_2b(elems);  // 140183fd0
        for (std::int32_t i = 0; i < NE(); i = i + 1) crt::fprintf(f, "%15.7E\n", v[i]);
        const double* w = p407::node_var_1b(nodes);  // 140181320
        for (std::int32_t i = 0; i < NN(); i = i + 1) crt::fprintf(f, "%15.7E\n", w[i]);
    } else if (at<std::int32_t>(m, 0x88c) == 3) {
        const std::int32_t c = NE() << 2;  // local_58
        crt::fprintf(f, "%d\n", c);
        const double* v = at<double*>(obj, 0xa28);
        for (std::int32_t i = 0; i < c; i = i + 1) crt::fprintf(f, "%15.7E\n", v[i]);
    }

    // strain tensor, width m+0x1698
    crt::fprintf(f, "%d\n", NE());
    const double* strain = p407::elem_var_28(elems);  // 140183e50
    const std::int32_t w_f0 = at<std::int32_t>(m, 0x1698);
    rows(f, strain, NE(), w_f0);

    // stress tensor by its option m+0x890, width m+0x1690 (local_a4; local_138 is the value
    // DEF_INT_NO_DB.DAT gets: times 4 for option 3)
    std::int32_t w_138 = at<std::int32_t>(m, 0x1690);
    const std::int32_t w_a4 = w_138;
    if (at<std::int32_t>(m, 0x890) == 0) {
        crt::fprintf(f, "%d\n", NE());
        rows(f, p407::elem_var_26(elems), NE(), w_a4);  // 140183d50
    } else if (at<std::int32_t>(m, 0x890) == 2) {
        crt::fprintf(f, "%d  %d\n", NE(), NN());
        rows(f, p407::elem_var_26(elems), NE(), w_a4);           // 140183d50
        const double* v = svkw::stress_node_values(nodes);       // 1401813a0
        const std::int32_t w = at<std::int32_t>(m, 0x1660);      // local_48
        rows(f, v, NN(), w);
    } else if (at<std::int32_t>(m, 0x890) == 3) {
        w_138 = w_138 << 2;
        const std::int32_t c = NE() << 2;  // local_30
        crt::fprintf(f, "%d\n", c);
        rows(f, at<double*>(obj, 0xa00), c, w_a4);
    }

    // YLDS, width m+0x16dc
    crt::fprintf(f, "%d\n", NE());
    const double* ylds = svkw::ylds_values(elems);  // 140184760
    const std::int32_t w_b8 = at<std::int32_t>(m, 0x16dc);
    rows(f, ylds, NE(), w_b8);

    // nodal temperature: (T - DTMP, T)
    crt::fprintf(f, "%d\n", NN());
    const double* t = p407::node_var_08(nodes);   // 140180c40
    const double* dt = svkw::dtmp_values(nodes);  // 140180eb0
    for (std::int32_t i = 0; i < NN(); i = i + 1) crt::fprintf(f, "%15.7E  %15.7E\n", t[i] - dt[i], t[i]);

    // relative density
    crt::fprintf(f, "%d\n", NE());
    const double* dens = def::dbio::objcheck::elem_block_density(elems);  // 140184050
    for (std::int32_t i = 0; i < NE(); i = i + 1) crt::fprintf(f, "%15.7E\n", dens[i]);

    // damage by its option m+0x888, width m+0x16ac (local_dc; local_cc goes to
    // DEF_INT_NO_DB.DAT: times 4 for option 3)
    const std::int32_t w_dc = at<std::int32_t>(m, 0x16ac);
    std::int32_t w_cc = w_dc;
    if (at<std::int32_t>(m, 0x888) == 0) {
        crt::fprintf(f, "%d\n", NE());
        rows(f, svkw::damage_values(elems), NE(), w_dc);  // 1401840d0
    } else if (at<std::int32_t>(m, 0x888) == 2) {
        crt::fprintf(f, "%d  %d\n", NE(), NN());
        rows(f, svkw::damage_values(elems), NE(), w_dc);       // 1401840d0
        rows(f, svkw::damage_node_values(nodes), NN(), w_dc);  // 1401812a0
    } else if (at<std::int32_t>(m, 0x888) == 3) {
        w_cc = w_dc << 2;
        const std::int32_t c = NE() << 2;  // local_10
        crt::fprintf(f, "%d\n", c);
        rows(f, at<double*>(obj, 0xa38), c, w_dc);
    }

    // bare matrices (1402f7490 "rows  cols" + rows); each width is read before its accessor
    namespace g2sv = def::key::g2sv;
    const std::int32_t w_98 = at<std::int32_t>(m, 0x1600);  // velocity
    g2sv::write_matrix(f, def::model::fun_1401809c0(static_cast<def::model::Cls_14017ef20*>(nodes)), w_98, NN());
    const std::int32_t w_9c = at<std::int32_t>(m, 0x15fc);  // displacement
    g2sv::write_matrix(f, p407::node_var_01(nodes), w_9c, NN());  // 140180940
    {
        const std::int32_t w = at<std::int32_t>(m, 0x162c);
        g2sv::write_matrix(f, p407::node_var_0d(nodes), w, NN());  // 140180f90
    }
    {
        const std::int32_t w = at<std::int32_t>(m, 0x16c4);
        g2sv::write_matrix(f, svkw::dvolf_values(elems), w, NE());  // 140184400
    }
    {
        const std::int32_t w = at<std::int32_t>(m, 0x16d8);  // local_d0
        g2sv::write_matrix(f, p407::elem_var_38(elems), w, NE());  // 1401846d0
    }
    {
        const std::int32_t w = at<std::int32_t>(m, 0x16c8);  // local_e8
        g2sv::write_matrix(f, svkw::ticf_values(elems), w, NE());  // 140184490
    }

    // VOLFC / VOLFS gathered to the object's phases, or "NE  0" without phase functions
    bool single = true;     // local_11c
    std::int32_t mat = 0;   // local_120
    def::dbio::gendb::object_group_list(obj, &single, &mat);           // 140160260
    const std::int32_t nfun = def::dbio::gendb::dynary_words(&obj->u3598);  // 140130290 (local_c8)
    // local_d4 = m+0x16bc is read here and never used
    if (nfun < 1) {
        crt::fprintf(f, "%d  %d\n", NE(), 0);
    } else {
        g2sv::write_phase_matrix(*m, f, *obj, 0x31, 0);  // 1402a7b60
    }
    if (nfun < 1) {
        crt::fprintf(f, "%d  %d\n", NE(), 0);
    } else {
        g2sv::write_phase_matrix(*m, f, *obj, 0x32, 0);  // 1402a7b60
    }

    {
        const std::int32_t w = at<std::int32_t>(m, 0x16cc);  // local_104
        g2sv::write_matrix(f, svkw::grain_values(elems), w, NE());  // 140184520
    }
    {
        const std::int32_t w = at<std::int32_t>(m, 0x1680);  // local_ec
        g2sv::write_matrix(f, svkw::timerc_values(nodes), w, NN());  // 1401814b0
    }
    {
        const std::int32_t w = at<std::int32_t>(m, 0x16d0);
        g2sv::write_matrix(f, svkw::sizesh_values(elems), w, NE());  // 1401845b0
    }
    {
        const std::int32_t w = at<std::int32_t>(m, 0x16f4);
        g2sv::write_matrix(f, svkw::erecvl_values(elems), w, NE());  // 1401849a0
    }
    {
        const std::int32_t w = at<std::int32_t>(m, 0x1684);
        g2sv::write_matrix(f, svkw::nrecvl_values(nodes), w, NN());  // 140181530
    }
    {
        const std::int32_t w = at<std::int32_t>(m, 0x16b0);  // local_e0
        const bool mark = true;                               // local_c
        g2sv::write_matrix(f, svkw::mataxi_values(elems, &mark), w, NE());  // 140184150
    }
    {
        std::int32_t w = at<std::int32_t>(m, 0x16e8);  // USRELM width (local_b0)
        if (w == 0) {
            def::model::resize_element_names(m, 2);  // 1401d2090
            w = at<std::int32_t>(m, 0x16e8);
        }
        g2sv::write_matrix(f, svkw::usrelm_values(elems), w, NE());  // 140184a30
    }
    {
        std::int32_t w = at<std::int32_t>(m, 0x163c);  // USRNOD width (local_a8)
        if (w == 0) {
            def::model::resize_node_names(m, 2);  // 1401d21c0
            w = at<std::int32_t>(m, 0x163c);
        }
        g2sv::write_matrix(f, svkw::usrnod_values(nodes), w, NN());  // 1401815b0
    }

    // zero blocks (14053ca58 / 50 / 58 / 54 / 60)
    for (std::int32_t i = 0; i < NN(); i = i + 1) crt::fprintf(f, "%d  %d\n", 0, 0);
    crt::fprintf(f, "%d\n", 0);
    for (std::int32_t i = 0; i < NN(); i = i + 1) crt::fprintf(f, "%d  %d\n", 0, 0);
    for (std::int32_t i = 0; i < NN(); i = i + 1) crt::fprintf(f, "%d\n", 0);
    for (std::int32_t i = 0; i < NN(); i = i + 1) crt::fprintf(f, "%15.7E\n", 0.0);  // xorpd -> r8
    crt::fprintf(f, "%d  %d\n", 1, 1);
    // boundary: obj+0xbe0 (boundary node count) - 1 segments (local_b4)
    const std::int32_t nb = at<std::int32_t>(obj, 0xbe0) - 1;
    crt::fprintf(f, "%d  %d  %d  %d  %d\n", 1, 2, nb, 1, nb);
    for (std::int32_t i = 0; i < nb; i = i + 1) crt::fprintf(f, "%d  %d  %d\n", 0, 0, 0);
    for (std::int32_t i = 0; i < nb; i = i + 1) crt::fprintf(f, "%d  %d  %d\n", 0, 0, 0);
    for (std::int32_t i = 0; i < nb; i = i + 1) crt::fprintf(f, "%15.7E  %15.7E  %15.7E\n", 0.0, 0.0, 0.0);
    for (std::int32_t i = 0; i < NN(); i = i + 1) crt::fprintf(f, "%d\n", 0);
    for (std::int32_t i = 0; i < NN(); i = i + 1) crt::fprintf(f, "%d\n", 0);
    std::fclose(f);

    f = fsopen("DEF_INT_NO_DB.DAT", "wt");  // 14053caa4
    if (f != nullptr) {
        crt::fprintf(f, "%d\n", w_f0);
        crt::fprintf(f, "%d\n", w_138);
        crt::fprintf(f, "%d\n", w_b8);
        crt::fprintf(f, "%d\n", w_98);
        crt::fprintf(f, "%d\n", w_9c);
        crt::fprintf(f, "%d\n", w_cc);
        std::fclose(f);
    }
}

}  // namespace def::cmd::step_dat
