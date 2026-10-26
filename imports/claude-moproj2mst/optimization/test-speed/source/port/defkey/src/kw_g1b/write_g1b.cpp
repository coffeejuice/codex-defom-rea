// kw_g1b/write_g1b.cpp: the group 1 KEY writer FUN_1403d4990 from its "Iteration Controls" banner
// to the end, and the registry column 2/3 handlers of those keywords.
//
// Keyword names come from FUN_1402f7720 (keyword_label), which returns one static buffer; as in
// the original each name is fetched right before the fprintf that prints it. Output goes through
// def::rt::crt::fprintf, the VS2008 CRT's float formatting (%20.10E must round like DEF_PRE).
#define _CRT_SECURE_NO_WARNINGS
#include <defkey/kw_g1b.hpp>

#include <cstdio>
#include <cstring>
#include <string>
#include <utility>
#include <vector>

#include <defkey/key_writer.hpp>
#include <defrt/stdlib/legacy_printf.hpp>
#include <defkey/table_io.hpp>
#include <defmodel/cvname/cvname.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/step/name_arrays.hpp>

#include "kw_g1b/common.hpp"

namespace def::key::g1b {

using def::model::DYNARY;
using def::model::Cls_1401e7b10;

// ---- column 2 handlers (also called directly by the KEY writer) ----------------------------

// @src 1403d8260  CVGERR writer
int write_cvgerr(std::FILE* f, DB_STEP_DATA& m, std::vector<int> /*v*/)
{
    const char* name = keyword_label(0x3d);
    def::rt::crt::fprintf(f, "%-6s %20.10E %20.10E\n", name, m.sim_ctrl.cvgerr[0], m.sim_ctrl.cvgerr[1]);
    return 0;
}

// @src 1403d82f0  GEOERR writer
int write_geoerr(std::FILE* f, DB_STEP_DATA& m, std::vector<int> /*v*/)
{
    const char* name = keyword_label(0x47);
    def::rt::crt::fprintf(f, "%-6s %20.10E %20.10E %20.10E %20.10E\n", name, m.sim_ctrl.geoerr[0], m.sim_ctrl.geoerr[1],
                 m.sim_ctrl.geoerr[2], m.sim_ctrl.geoerr[3]);
    return 0;
}

// @src 1403d83a0  PENINF writer
int write_peninf(std::FILE* f, DB_STEP_DATA& m, std::vector<int> /*v*/)
{
    const char* name = keyword_label(0x48);
    def::rt::crt::fprintf(f, "%-6s %20.10E\n", name, m.sim_ctrl.peninf);
    return 0;
}

// @src 1403d8420  UNTE2H writer
int write_unte2h(std::FILE* f, DB_STEP_DATA& m, std::vector<int> /*v*/)
{
    const char* name = keyword_label(0x49);
    def::rt::crt::fprintf(f, "%-6s %20.10E\n", name, m.sim_ctrl.unte2h);
    return 0;
}

// @src 1403d7250  TINTGF writer
int write_tintgf(std::FILE* f, DB_STEP_DATA& m, std::vector<int> /*v*/)
{
    const char* name = keyword_label(0x4a);
    def::rt::crt::fprintf(f, "%-6s %20.10E\n", name, m.sim_ctrl.tintgf);
    return 0;
}

// @src 1403d72d0  BLZMAN writer
int write_blzman(std::FILE* f, DB_STEP_DATA& m, std::vector<int> /*v*/)
{
    const char* name = keyword_label(0x4d);
    def::rt::crt::fprintf(f, "%-6s %20.10E\n", name, m.sim_ctrl.blzman);
    return 0;
}

// @src 1403d6c90  ENVTMP writer: constant / time table, or (region flag M+0xb10) the region list
int write_envtmp(std::FILE* f, DB_STEP_DATA& m, std::vector<int> /*v*/)
{
    Cls_1401e7b10& s = m.sim_ctrl;
    if (s.furnace_on == 0) {
        DYNARY* t = &s.envtmp;
        if (t->ndim == 0) {
            const char* name = keyword_label(0x4b);
            def::rt::crt::fprintf(f, "%-6s %7i %20.10E\n", name, 0, t->value);
        } else {
            if (t->ndim != 1 || t->var[0] != 1)
                return 3;
            const char* name = keyword_label(0x4b);
            def::rt::crt::fprintf(f, "%-6s %7i %7i\n", name, 1, t->n[0]);
        }
        write_table_body(f, t, 0);
    } else {
        int n = s.furnace.curve_count();  // local_20, 140375950
        const char* name = keyword_label(0x4b);
        def::rt::crt::fprintf(f, "%-6s %7i %7i\n", name, 10, n);
        int one = 1;  // local_24
        for (int i = 1; i <= n; ++i) {  // local_28
            DYNARY* e = s.furnace.curve(&i);  // 140375980
            if (e != nullptr) {
                if (i == 0x15)  // FUN_14039d2a0
                    write_region_string_table(f, e, one, i);
                else
                    write_region_table(f, e, one, i);
            }
        }
    }
    return 0;
}

// @src 1403d6f30  CNVCOF writer
int write_cnvcof(std::FILE* f, DB_STEP_DATA& m, std::vector<int> /*v*/)
{
    DYNARY* t = &m.sim_ctrl.cnvcof;
    if (t->ndim == 0) {
        const char* name = keyword_label(0x4e);
        def::rt::crt::fprintf(f, "%-6s %7i %20.10E\n", name, 0, t->value);
    } else {
        if (t->ndim != 1 || t->var[0] != 3)
            return 3;
        const char* name = keyword_label(0x4e);
        def::rt::crt::fprintf(f, "%-6s %7i %7i\n", name, 1, t->n[0]);
    }
    write_table_body(f, t, 0);
    return 0;
}

// @src 1403d70c0  ENVATM writer
int write_envatm(std::FILE* f, DB_STEP_DATA& m, std::vector<int> /*v*/)
{
    DYNARY* t = &m.sim_ctrl.envatm;
    if (t->ndim == 0) {
        const char* name = keyword_label(0x4f);
        def::rt::crt::fprintf(f, "%-6s %7i %20.10E\n", name, 0, t->value);
    } else {
        if (t->ndim != 1 || t->var[0] != 1)
            return 3;
        const char* name = keyword_label(0x4f);
        def::rt::crt::fprintf(f, "%-6s %7i %7i\n", name, 1, t->n[0]);
    }
    write_table_body(f, t, 0);
    return 0;
}

// @src 1403d7350  ACVCOF writer
int write_acvcof(std::FILE* f, DB_STEP_DATA& m, std::vector<int> /*v*/)
{
    DYNARY* t = &m.sim_ctrl.acvcof;
    if (t->ndim == 0) {
        const char* name = keyword_label(0x50);
        def::rt::crt::fprintf(f, "%-6s %7i %20.10E\n", name, 0, t->value);
    } else if (t->ndim == 1 && t->var[0] == 3) {
        const char* name = keyword_label(0x50);
        def::rt::crt::fprintf(f, "%-6s %7i %7i\n", name, 1, t->n[0]);
    } else if (t->ndim == 1 && t->var[0] == 0xc) {
        const char* name = keyword_label(0x50);
        def::rt::crt::fprintf(f, "%-6s %7i %7i\n", name, 2, t->n[0]);
    } else {
        if (t->ndim != 1 || t->var[0] != 1)
            return 3;
        const char* name = keyword_label(0x50);
        def::rt::crt::fprintf(f, "%-6s %7i %7i\n", name, 3, t->n[0]);
    }
    write_table_body(f, t, 0);
    return 0;
}

// @src 1403d75a0  ENVMPR writer
int write_envmpr(std::FILE* f, DB_STEP_DATA& m, std::vector<int> /*v*/)
{
    const char* name = keyword_label(0x51);
    def::rt::crt::fprintf(f, "%-6s %20.10E\n", name, m.sim_ctrl.envmpr);
    return 0;
}

// @src 1403d7620  ENVMPT writer
int write_envmpt(std::FILE* f, DB_STEP_DATA& m, std::vector<int> /*v*/)
{
    const char* name = keyword_label(0x52);
    def::rt::crt::fprintf(f, "%-6s %20.10E\n", name, m.sim_ctrl.envmpt);
    return 0;
}

// @src 1403d76a0  EHRATE writer
int write_ehrate(std::FILE* f, DB_STEP_DATA& m, std::vector<int> /*v*/)
{
    const char* name = keyword_label(0x53);
    def::rt::crt::fprintf(f, "%-6s %20.10E\n", name, m.sim_ctrl.ehrate);
    return 0;
}

// ---- column 3 accessors -------------------------------------------------------------------

// The original receives the model as an integer and skips the store when it equals -0x750 (the
// model pointer computed back from a null simulation-control pointer).
static bool is_null_sim_ctrl(const DB_STEP_DATA* m)
{
    return reinterpret_cast<std::uintptr_t>(m) == static_cast<std::uintptr_t>(-0x750);
}

// @src 1403d6ec0  ENVTMP table accessor
int table_envtmp(DB_STEP_DATA* m, void* /*unused*/, std::vector<int> /*v*/, void** out)
{
    if (!is_null_sim_ctrl(m))
        *out = &m->sim_ctrl.envtmp;  // M+0xb18
    return 0;
}

// @src 1403d7050  CNVCOF table accessor
int table_cnvcof(DB_STEP_DATA* m, void* /*unused*/, std::vector<int> /*v*/, void** out)
{
    if (!is_null_sim_ctrl(m))
        *out = &m->sim_ctrl.cnvcof;  // M+0xc10
    return 0;
}

// @src 1403d71e0  ENVATM table accessor
int table_envatm(DB_STEP_DATA* m, void* /*unused*/, std::vector<int> /*v*/, void** out)
{
    if (!is_null_sim_ctrl(m))
        *out = &m->sim_ctrl.envatm;  // M+0xc88
    return 0;
}

// @src 1403d7530  ACVCOF table accessor
int table_acvcof(DB_STEP_DATA* m, void* /*unused*/, std::vector<int> /*v*/, void** out)
{
    if (!is_null_sim_ctrl(m))
        *out = &m->sim_ctrl.acvcof;  // M+0xd00
    return 0;
}

// ---- the KEY writer sections --------------------------------------------------------------

// Iteration Controls (writer statements after the "Stopping & Step Controls" records).
static int write_iteration_controls(std::FILE* f, DB_STEP_DATA& m)
{
    Cls_1401e7b10& s = m.sim_ctrl;
    write_section_banner(f, "Iteration Controls");
    int rc = write_cvgerr(f, m, std::vector<int>());
    if (rc != 0)
        return rc;
    {
        const char* name = keyword_label(0x3f);  // ITRMTH
        def::rt::crt::fprintf(f, "%-6s %7i\n", name, s.itrmth);
    }
    int solver = s.solmtd;  // local_a8
    if (solver == 2) {
        const char* name = keyword_label(0x46);  // SOLMTD
        def::rt::crt::fprintf(f, "%-6s %7i %7i\n", name, s.solmtd, 4);
        int p4 = static_cast<int>(s.solmtd_p[3]);
        int p3 = static_cast<int>(s.solmtd_p[2]);
        def::rt::crt::fprintf(f, " %7i %7i %7i %7i\n", static_cast<int>(s.solmtd_p[0]),
                     static_cast<int>(s.solmtd_p[1]), p3, p4);
    } else if (solver == 6) {
        const char* name = keyword_label(0x46);
        def::rt::crt::fprintf(f, "%-6s %7i %7i\n", name, s.solmtd, 4);
        int p4 = static_cast<int>(s.solmtd_p[3]);
        int p3 = static_cast<int>(s.solmtd_p[2]);
        def::rt::crt::fprintf(f, " %20.10E %20.10E %7i %7i\n", s.solmtd_p[0], s.solmtd_p[1],
                     p3, p4);
    } else {
        const char* name = keyword_label(0x46);
        def::rt::crt::fprintf(f, "%-6s %7i\n", name, s.solmtd);
    }
    const std::pair<int, int> ints[] = {
        {0x40, s.itrmxd},  // ITRMXD M+0xa60
        {0x41, s.defbwd},  // DEFBWD M+0xa64
        {0x42, s.solmtt},  // SOLMTT M+0xa68
        {0x43, s.itrmxt},  // ITRMXT M+0xa6c
        {0x44, s.tmpbwd},  // TMPBWD M+0xa70
        {0x45, s.latent},  // LATENT M+0xa74
    };
    for (const auto& [kid, value] : ints) {
        const char* name = keyword_label(kid);
        def::rt::crt::fprintf(f, "%-6s %7i\n", name, value);
    }
    // FRQNCY, only when rows * columns matches the table's value count (M+0xac0).
    if (0 < s.frqncy_rows && 0 < s.frqncy_cols && s.frqncy_rows * s.frqncy_cols == s.frqncy.ncomp) {
        const char* name = keyword_label(0x5b);
        def::rt::crt::fprintf(f, "%-6s %7i %7i\n", name, s.frqncy_rows, s.frqncy_cols);
        const double* vals = s.frqncy.values;
        int k = 0;  // local_348
        for (int r = 0; r < s.frqncy_rows; ++r) {
            for (int c = 0; c < s.frqncy_cols; ++c) {
                def::rt::crt::fprintf(f, " %7i", static_cast<int>(vals[k]));
                ++k;
            }
            def::rt::crt::fprintf(f, "\n");
        }
    }
    return 0;
}

static int write_processing_conditions(std::FILE* f, DB_STEP_DATA& m)
{
    Cls_1401e7b10& s = m.sim_ctrl;
    write_section_banner(f, "Processing Conditions");
    int rc;
    if ((rc = write_geoerr(f, m, std::vector<int>())) != 0)
        return rc;
    if ((rc = write_peninf(f, m, std::vector<int>())) != 0)
        return rc;
    if ((rc = write_unte2h(f, m, std::vector<int>())) != 0)
        return rc;
    if ((rc = write_tintgf(f, m, std::vector<int>())) != 0)
        return rc;
    if ((rc = write_blzman(f, m, std::vector<int>())) != 0)
        return rc;
    if ((rc = write_envtmp(f, m, std::vector<int>())) != 0)
        return rc;
    if (s.furnace_on != 0) {
        const char* name = keyword_label(0x4c);  // FRNNAM
        def::rt::crt::fprintf(f, "%-6s\n%s\n", name, s.furnace.name);
    }
    if ((rc = write_cnvcof(f, m, std::vector<int>())) != 0)
        return rc;
    if ((rc = write_envmpr(f, m, std::vector<int>())) != 0)
        return rc;
    if ((rc = write_envmpt(f, m, std::vector<int>())) != 0)
        return rc;
    if ((rc = write_ehrate(f, m, std::vector<int>())) != 0)
        return rc;
    if ((rc = write_envatm(f, m, std::vector<int>())) != 0)
        return rc;
    if ((rc = write_acvcof(f, m, std::vector<int>())) != 0)
        return rc;
    // TIMEID, ERECID, NRECID: only tables with rows.
    const struct {
        int id;
        DYNARY* t;
    } recs[] = {{0x54, &s.timeid}, {0x55, &s.erecid}, {0x56, &s.nrecid}};
    for (const auto& r : recs) {
        if (0 < r.t->n[0]) {
            const char* name = keyword_label(r.id);
            def::rt::crt::fprintf(f, "%-6s %7i %7i\n", name, r.t->n[0], r.t->ncomp);
            write_table_body(f, r.t, 0);
        }
    }
    return 0;
}

static int write_user_defined_variables(std::FILE* f, DB_STEP_DATA& m)
{
    Cls_1401e7b10& s = m.sim_ctrl;
    write_section_banner(f, "User Defined Variables");
    // USRDEF: 1 + index of the last non-empty slot.
    int k = 0;  // local_358
    for (int i = 0; i < 10; ++i) {
        if (std::strlen(s.usrdef[i]) != 0)
            k = i + 1;
    }
    if (0 < k) {
        const char* name = keyword_label(0x57);
        def::rt::crt::fprintf(f, "%-6s %7i\n", name, k);
    }
    for (int i = 0; i < k; ++i)
        def::rt::crt::fprintf(f, "%s\n", s.usrdef[i]);
    // UNNAME (count from FUN_1401cbbc0, loop bound re-read from M+0x3398).
    if (m.u33a0 != nullptr) {
        int n = def::model::node_name_count(&m);
        const char* name = keyword_label(0x59);
        def::rt::crt::fprintf(f, "%-6s %7i %7i\n", name, 1, n);
        for (int i = 0; i < m.u3398; ++i)
            def::rt::crt::fprintf(f, "%s\n", m.u33a0[i]);
    }
    // UENAME
    if (m.u33b0 != nullptr) {
        int n = def::model::element_name_count(&m);
        const char* name = keyword_label(0x58);
        def::rt::crt::fprintf(f, "%-6s %7i %7i\n", name, 1, n);
        for (int i = 0; i < m.u33a8; ++i)
            def::rt::crt::fprintf(f, "%s\n", m.u33b0[i]);
    }
    // CVNAME: every collection entry.
    int count = def::model::cv_count(&m.u33b8);
    if (0 < count) {
        for (int i = 0; i < count; ++i) {
            def::model::CustomVarDef* e = def::model::cv_entry(&m.u33b8, i);
            int cid = def::model::cv_def_id(e);
            int size = def::model::cv_def_item_count(e);
            const char* name = keyword_label(0x5a);
            def::rt::crt::fprintf(f, "%-6s %7i %7i\n", name, cid, size);
            def::rt::crt::fprintf(f, "%s\n", def::model::cv_def_name(e).c_str());
            for (int j = 0; j < size; ++j)
                def::rt::crt::fprintf(f, "%s\n", def::model::cv_def_item(e, j).c_str());
        }
    }
    return 0;
}

// part of @src 1403d4990  KEY writer, "Iteration Controls" banner to the end
int write_sections(std::FILE* f, DB_STEP_DATA& m)
{
    int rc = write_iteration_controls(f, m);
    if (rc != 0)
        return rc;
    rc = write_processing_conditions(f, m);
    if (rc != 0)
        return rc;
    return write_user_defined_variables(f, m);
}

}  // namespace def::key::g1b
