// kw_g1b_fixture.hpp: test model for the kw-g1b tests.
//
// DB_STEP_DATA's constructor belongs to p1-03 and is not on this branch, so the tests build a
// zero-filled model and set the fields kw-g1b reads to the values the constructor's init chain
// gives them. Values and branches are copied from the decompile of the init functions (cited per
// block); the constants were read from DEF_PRE_64.exe (.rdata) with tools, not from KEY output.
#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#include <string>

#include <defkey/kw_g1b.hpp>
#include <defmodel/dynary_init.hpp>
#include <defmodel/layout/db_step_data.hpp>

#include "../src/kw_g1b/common.hpp"

#include "key_test_util.hpp"

namespace kwg1b_test {

using def::model::DB_STEP_DATA;

// Same switch as def::rt::init_stdio_compat() (p1-01): three-digit exponents, msvcrt inf/nan
// text, the VS2010 CRT's rounding. Needed so that %E prints "1.0000000000E-003" as DEF_PRE does.
inline void stdio_compat() { keytest::legacy_printf(); }

// FUN_1401f4100: what reading a new UNIT value resets in the simulation-control block.
inline void unit_reset(DB_STEP_DATA& m)
{
    using def::model::dynary_init;
    auto& s = m.sim_ctrl;
    dynary_init(&s.envtmp, 0);
    dynary_init(&s.cnvcof, 0);
    dynary_init(&s.envatm, 0);
    dynary_init(&s.acvcof, 0);
    if (s.unit == 1) {
        s.unte2h = 1.0;
        s.blzman = 5.669e-11;
        s.envtmp.value = 20.0;
        s.cnvcof.value = 0.02;
        s.envatm.value = 1.69;
        s.acvcof.value = 0.00025;
        s.envmpr = 1.26e-09;
        s.envmpt = 8.8e-15;
    } else {
        s.unte2h = 0.107;
        s.blzman = 3.306e-15;
        s.envtmp.value = 68.0;
        s.cnvcof.value = 7.7e-06;
        s.envatm.value = 1.69;
        s.acvcof.value = 1e-05;
        s.envmpr = 3.19e-08;
        s.envmpt = 2.25e-13;
    }
}

struct Model {
    DB_STEP_DATA* p;

    explicit Model(int dim, int unit = 0)
    {
        stdio_compat();
        p = static_cast<DB_STEP_DATA*>(std::calloc(1, sizeof(DB_STEP_DATA)));
        DB_STEP_DATA& m = *p;
        m.dimension = dim;
        auto& s = m.sim_ctrl;
        s.owner = p;
        new (&m.u33b8) def::model::Cls_14032f100();  // 14032f100: CVNAME list (calloc'd model)
        new (&s.furnace) def::model::Cls_1403753f0();  // 1403753f0: ENVTMP 10 region list
        s.unit = unit;
        bool is2d = dim == 2;
        // FUN_1401e96b0
        if (!is2d) {
            s.cvgerr[0] = 0.005;
            s.cvgerr[1] = 0.05;
            s.solmtd = 1;
            s.solmtt = 2;
        } else {
            s.cvgerr[0] = 0.001;
            s.cvgerr[1] = 0.01;
            s.solmtd = 1;
            s.solmtt = 1;
        }
        s.u2e8 = 0;
        // FUN_1401e9850: FRQNCY 6 x 1 = 1, 0, 100, 20, 1, 1
        s.frqncy_rows = 6;
        s.frqncy_cols = 1;
        def::model::dynary_init(&s.frqncy, 0xb5);
        s.frqncy.ncomp = s.frqncy_rows * s.frqncy_cols;
        def::model::dynary_alloc(&s.frqncy);
        const double fr[6] = {1.0, 0.0, 100.0, 20.0, 1.0, 1.0};
        std::memcpy(s.frqncy.values, fr, sizeof fr);
        s.itrmth = is2d ? 1 : 2;
        s.itrmxd = 200;
        s.defbwd = 1;
        s.itrmxt = 200;
        s.tmpbwd = 1;
        s.latent = 0;
        // FUN_1401ea6b0 (the region list default fill FUN_140375e90 never reaches the KEY text
        // unless ENVTMP 10 is read, which rebuilds the list)
        s.peninf = 1e9;
        s.tintgf = 0.75;
        s.furnace_on = 0;
        if (s.unit == 2) {
            s.unit = 0;  // reuse the reset's English branch: init tests == 2, the reset == 1
            unit_reset(m);
            s.unit = 2;
        } else {
            int keep = s.unit;
            s.unit = 1;
            unit_reset(m);
            s.unit = keep;
        }
        // FUN_1401ead60
        s.geoerr[0] = 1e-6;
        s.geoerr[1] = 1e-4;
        def::model::dynary_init(&s.timeid, 0xb0);
        def::model::dynary_init(&s.erecid, 0xb1);
        def::model::dynary_init(&s.nrecid, 0xb2);
    }
    Model(const Model&) = delete;
    Model& operator=(const Model&) = delete;
    // The interim helpers keep side tables keyed by address; the model memory is leaked on purpose
    // so that an address is never reused by a later test's model.
    ~Model() = default;

    DB_STEP_DATA& operator*() { return *p; }
    DB_STEP_DATA* operator->() { return p; }
};

// A FILE* over a string (tmpfile, so that fgets/fscanf behave as on a real KEY file).
inline std::FILE* file_from(const std::string& text)
{
    std::FILE* f = std::tmpfile();
    std::fwrite(text.data(), 1, text.size(), f);
    std::rewind(f);
    return f;
}

inline std::string slurp(std::FILE* f)
{
    std::string out;
    std::rewind(f);
    char buf[4096];
    std::size_t n;
    while ((n = std::fread(buf, 1, sizeof buf, f)) > 0)
        out.append(buf, n);
    return out;
}

// Rest of the file after the reader stopped (to check how much it consumed).
inline std::string rest(std::FILE* f)
{
    std::string out;
    char buf[4096];
    std::size_t n;
    while ((n = std::fread(buf, 1, sizeof buf, f)) > 0)
        out.append(buf, n);
    return out;
}

// Read one keyword the way the KEY loop does: the keyword token has been consumed by
// fscanf("%39s"), so `after_keyword` is the rest of the keyword's line and what follows.
inline int read_kw(DB_STEP_DATA& m, int id, const std::string& after_keyword, std::string* left = nullptr)
{
    std::FILE* f = file_from(after_keyword);
    int rc = def::key::g1b::read_keyword(m, f, id);
    if (left)
        *left = rest(f);
    std::fclose(f);
    return rc;
}

inline std::string write_all(DB_STEP_DATA& m, int* rc = nullptr)
{
    std::FILE* f = std::tmpfile();
    int r = def::key::g1b::write_sections(f, m);
    if (rc)
        *rc = r;
    std::string s = slurp(f);
    std::fclose(f);
    return s;
}

}  // namespace kwg1b_test
