// kw_g1b/read_g1b.cpp: cases 0x27..0x45 of the group 1 KEY parser FUN_14039aa90 (registry ids
// 61..91: CVGERR .. FRQNCY), and the value checks they call.
//
// Offsets in comments are model offsets (M+...) as in the decompile; the simulation-control block
// is m.sim_ctrl = M+0x750 (S+... = M+0x750+...). Specs: docs/re/keywords/<KW>.md.
#define _CRT_SECURE_NO_WARNINGS
#include <defkey/kw_g1b.hpp>

#include <cstdio>
#include <cstring>
#include <string>

#include <defkey/line_io.hpp>
#include <defkey/table_io.hpp>
#include <defmodel/cvname/cvname.hpp>
#include <defmodel/dynary_init.hpp>
#include <defmodel/step/name_arrays.hpp>
#include <defmodel/strtrim.hpp>
#include <defmodel/layout/db_step_data.hpp>

#include "kw_g1b/common.hpp"

namespace def::key::g1b {

using def::model::DYNARY;
using def::model::Cls_1401e7b10;

// @src 1401ea4d0  ITRMTH value check: 1..3 (same range in 2D and 3D)
static bool itrmth_valid(Cls_1401e7b10* s, const int* v)
{
    bool ok;
    if (!is_2d(*s->owner)) {
        ok = !(*v < 1 || 3 < *v);
    } else {
        ok = !(*v < 1 || 3 < *v);
    }
    return ok;
}

// @src 1401ea570  SOLMTD value check: 1..6 (same range in 2D and 3D)
static bool solmtd_valid(Cls_1401e7b10* s, const int* v)
{
    bool ok;
    if (!is_2d(*s->owner)) {
        ok = !(*v < 1 || 6 < *v);
    } else {
        ok = !(*v < 1 || 6 < *v);
    }
    return ok;
}

// @src 1401ea610  SOLMTT value check: 1..5 (same range in 2D and 3D)
static bool solmtt_valid(Cls_1401e7b10* s, const int* v)
{
    bool ok;
    if (!is_2d(*s->owner)) {
        ok = !(*v < 1 || 5 < *v);
    } else {
        ok = !(*v < 1 || 5 < *v);
    }
    return ok;
}

// @src 14039d2a0  index of the text entry (region 21) in the ENVTMP region list
static int region_text_entry(def::model::Cls_1403753f0* /*list*/) { return 0x15; }

bool handles(int id) { return 61 <= id && id <= 91; }

// Messages go to stdout: FUN_14047903c() + 0x30 is &_iob[1].
static std::FILE* msg_out() { return stdout; }

// part of @src 14039aa90  group 1 parser, cases 0x27..0x45 (id - 0x16)
int read_keyword(DB_STEP_DATA& m, std::FILE* f, int id)
{
    Cls_1401e7b10& s = m.sim_ctrl;
    int status = 0;  // local_5e8
    int iv[5] = {};  // local_620[5] (local_60c follows it as the 4th SOLMTD int)
    int iv4 = 0;     // local_60c[0]
    switch (id - 0x16) {
    case 0x27:  // CVGERR: M+0xa20, M+0xa28
        status = read_dd(f, &s.cvgerr[0], &s.cvgerr[1]);
        break;
    case 0x28:  // INIGES: one int, read and discarded
        status = read_i(f, iv, false);
        break;
    case 0x29:  // ITRMTH
        status = read_i(f, iv, false);
        if (!itrmth_valid(&s, iv)) {
            std::fprintf(msg_out(), "ITRMTH %d - ignored\n", iv[0]);
        } else {
            s.itrmth = iv[0];  // M+0xa30
            if (is_2d(m) && s.itrmth == 3)
                s.itrmth = 1;
        }
        break;
    case 0x2a:  // ITRMXD: M+0xa60
        status = read_i(f, &s.itrmxd, false);
        break;
    case 0x2b:  // DEFBWD: M+0xa64
        status = read_i(f, &s.defbwd, false);
        break;
    case 0x2c:  // SOLMTT
        status = read_i(f, iv, false);
        if (!solmtt_valid(&s, iv)) {
            std::fprintf(msg_out(), "SOLMTT %d - ignored\n", iv[0]);
        } else {
            s.solmtt = iv[0];  // M+0xa68
            if (is_2d(m) && s.solmtt == 2)
                s.solmtt = 1;
        }
        break;
    case 0x2d:  // ITRMXT: M+0xa6c
        status = read_i(f, &s.itrmxt, false);
        break;
    case 0x2e:  // TMPBWD: M+0xa70
        status = read_i(f, &s.tmpbwd, false);
        break;
    case 0x2f:  // LATENT: M+0xa74
        status = read_i(f, &s.latent, false);
        break;
    case 0x30:  // SOLMTD: solver M+0xa34, parameter count M+0xa38, parameters M+0xa40..0xa58
        status = read_ii(f, &iv[0], &iv[1]);
        if (!solmtd_valid(&s, iv)) {
            std::fprintf(msg_out(), "SOLMTD %d - ignored\n", iv[0]);
        } else {
            s.solmtd = iv[0];
            s.u2e8 = 0;
            if (s.solmtd == 2) {
                if (iv[1] == 4) {
                    s.u2e8 = 4;
                    status = read_i4(f, &iv[2], &iv[3], &iv[4], &iv4);
                    s.solmtd_p[0] = static_cast<double>(iv[2]);
                    s.solmtd_p[1] = static_cast<double>(iv[3]);
                    s.solmtd_p[2] = static_cast<double>(iv[4]);
                    s.solmtd_p[3] = static_cast<double>(iv4);
                } else {
                    s.u2e8 = 4;
                    s.solmtd_p[0] = 2.0;  // DAT_140509fb8
                    s.solmtd_p[1] = 4.0;  // DAT_14051f248
                    s.solmtd_p[2] = 1.0;  // _DAT_140509c90
                    s.solmtd_p[3] = 1.0;  // _DAT_140509c90
                    std::fprintf(msg_out(), "SOLMTD %d - parameters set to default values\n", 2);
                    // 14039c06c: S4 is (int) of the 8 bytes at M+0xa60 (ITRMXD, DEFBWD) read as
                    // a double, not M+0xa58.
                    double d9 = load_double(&s.itrmxd, 0);
                    double d10 = s.solmtd_p[2];
                    double d1 = s.solmtd_p[1];
                    double d2 = s.solmtd_p[0];
                    std::fprintf(msg_out(), "S1:%d;  S2:%d;  S3:%d;  S4:%d\n", static_cast<int>(d2),
                                 static_cast<int>(d1), static_cast<int>(d10), static_cast<int>(d9));
                }
            } else if (s.solmtd == 6) {
                if (iv[1] == 4) {
                    s.u2e8 = 4;
                    double a, b, c, d;  // local_678, local_670, local_668, local_660[0]
                    status = read_d4(f, &a, &b, &c, &d);
                    s.solmtd_p[0] = a;
                    s.solmtd_p[1] = b;
                    s.solmtd_p[2] = c;
                    s.solmtd_p[3] = d;
                } else {
                    std::fprintf(msg_out(), "SOLMTD %d - missing parameters\n", 6);
                }
            }
        }
        break;
    case 0x31:  // GEOERR: M+0xdb0, 0xdb8, 0xdc0, 0xdc8
        status = read_d4(f, &s.geoerr[0], &s.geoerr[1], &s.geoerr[2], &s.geoerr[3]);
        break;
    case 0x32:  // PENINF: M+0xaf8
        status = read_d(f, &s.peninf);
        break;
    case 0x33:  // UNTE2H: M+0xb00
        status = read_d(f, &s.unte2h);
        break;
    case 0x34:  // TINTGF: M+0xb08
        status = read_d(f, &s.tintgf);
        break;
    case 0x35: {  // ENVTMP: table M+0xb18, region flag M+0xb10, region list M+0xb90
        int t;     // local_628
        double v;  // local_5e0
        status = read_i_d(f, &t, &v);
        if (status != 0)
            return 2;
        DYNARY* tab = &s.envtmp;
        def::model::dynary_reinit_keep(tab, 2);
        s.furnace_on = 0;
        if (t == 0) {
            tab->ndim = 0;
            tab->value = v;
            status = read_table_body(f, tab, 0);
        } else if (t == 1) {
            tab->ndim = 1;
            tab->var[0] = 1;
            tab->n[0] = static_cast<int>(v);
            status = read_table_body(f, tab, 0);
        } else if (t == 10) {
            s.furnace_on = 1;
            int n[2] = {static_cast<int>(v), 0};  // local_5b8
            s.furnace.setup_curves(n);  // 140375600
            // The entry readers also get local_5bc (= 1) and the entry number; they ignore both.
            for (int i = 1; i <= n[0]; ++i) {  // local_5c0
                DYNARY* e = s.furnace.curve(&i);  // 140375980
                if (e != nullptr) {
                    if (i == region_text_entry(&s.furnace))
                        read_region_string_table(f, e);
                    else
                        read_region_table(f, e);
                }
            }
        }
        break;
    }
    case 0x36:  // FRNNAM: second line into M+0xb90 (the region list's name, 0x40 bytes)
        status = read_string_next_line(f, s.furnace.name, 0x40);
        break;
    case 0x37:  // BLZMAN: M+0xc08
        status = read_d(f, &s.blzman);
        break;
    case 0x38: {  // CNVCOF: table M+0xc10, template 1
        int t;
        double v;
        status = read_i_d(f, &t, &v);
        if (status != 0)
            return 2;
        DYNARY* tab = &s.cnvcof;
        def::model::dynary_reinit_keep(tab, 1);
        if (t == 0) {
            tab->ndim = 0;
            tab->value = v;
        } else if (t == 1) {
            tab->ndim = 1;
            tab->var[0] = 3;
            tab->n[0] = static_cast<int>(v);
        }
        status = read_table_body(f, tab, 0);
        break;
    }
    case 0x39: {  // ENVATM: table M+0xc88, template 0x1e
        int t;
        double v;
        status = read_i_d(f, &t, &v);
        if (status != 0)
            return 2;
        DYNARY* tab = &s.envatm;
        def::model::dynary_reinit_keep(tab, 0x1e);
        if (t == 0) {
            tab->ndim = 0;
            tab->value = v;
        } else if (t == 1) {
            tab->ndim = 1;
            tab->var[0] = 1;
            tab->n[0] = static_cast<int>(v);
        }
        status = read_table_body(f, tab, 0);
        break;
    }
    case 0x3a: {  // ACVCOF: table M+0xd00, template 0x1d
        int t;
        double v;
        status = read_i_d(f, &t, &v);
        if (status != 0)
            return 2;
        DYNARY* tab = &s.acvcof;
        def::model::dynary_reinit_keep(tab, 0x1d);
        if (t == 0) {
            tab->ndim = 0;
            tab->value = v;
        } else if (t == 1) {
            tab->ndim = 1;
            tab->var[0] = 3;
            tab->n[0] = static_cast<int>(v);
        } else if (t == 2) {
            tab->ndim = 1;
            tab->var[0] = 0xc;
            tab->n[0] = static_cast<int>(v);
        } else if (t == 3) {
            tab->ndim = 1;
            tab->var[0] = 1;
            tab->n[0] = static_cast<int>(v);
        }
        status = read_table_body(f, tab, 0);
        break;
    }
    case 0x3b:  // ENVMPR: M+0xd78
        status = read_d(f, &s.envmpr);
        break;
    case 0x3c:  // ENVMPT: M+0xd80
        status = read_d(f, &s.envmpt);
        break;
    case 0x3d:  // EHRATE: M+0xd88
        status = read_d(f, &s.ehrate);
        break;
    case 0x3e:    // TIMEID: table M+0x11a8, template 0xb0
    case 0x3f:    // ERECID: table M+0x1220, template 0xb1
    case 0x40: {  // NRECID: table M+0x1298, template 0xb2
        status = read_ii(f, &iv[0], &iv[1]);
        if (status != 0)
            return 2;
        DYNARY* tab;
        int type;
        if (id - 0x16 == 0x3e) {
            tab = &s.timeid;
            type = 0xb0;
        } else if (id - 0x16 == 0x3f) {
            tab = &s.erecid;
            type = 0xb1;
        } else {
            tab = &s.nrecid;
            type = 0xb2;
        }
        def::model::dynary_reinit_keep(tab, type);
        if (iv[0] < 1 || iv[1] < 1)
            return 3;
        tab->n[0] = iv[0];
        tab->ncomp = iv[1];
        def::model::dynary_alloc(tab);
        status = read_table_body(f, tab, 0);
        break;
    }
    case 0x41: {  // USRDEF: up to ten lines into M+0xdd8 + 0x51*i
        if (read_i(f, iv, false) != 0)
            return 2;
        status = 0;
        char line[1024];  // local_598
        for (int i = 0; i < iv[0] && i < 10; ++i) {
            if (std::fgets(line, 0x400, f) == nullptr)
                return 2;
            def::model::trim_right_blanks(line);
            std::strncpy(s.usrdef[i], line, 0x50);
        }
        break;
    }
    case 0x42: {  // UENAME: names of user element variables, M+0x33a8 / M+0x33b0
        status = read_ii(f, &iv[0], &iv[1]);
        if (status != 0)
            return 2;
        if (iv[0] < 2)
            def::model::resize_element_names(&m, iv[1]);
        char line[64];  // local_158
        for (int i = 0; i < m.u33a8; ++i) {
            if (std::fgets(line, 0x40, f) == nullptr)
                return 2;
            if (iv[0] < 2) {
                def::model::trim_right_blanks(line);
                if (std::strlen(line) != 0)
                    std::strcpy(m.u33b0[i], line);
            }
        }
        break;
    }
    case 0x43: {  // UNNAME: names of user node variables, M+0x3398 / M+0x33a0
        status = read_ii(f, &iv[0], &iv[1]);
        if (status != 0)
            return 2;
        if (iv[0] < 2)
            def::model::resize_node_names(&m, iv[1]);
        char line[64];  // local_198
        for (int i = 0; i < m.u3398; ++i) {
            if (std::fgets(line, 0x40, f) == nullptr)
                return 2;
            if (iv[0] < 2) {
                def::model::trim_right_blanks(line);
                if (std::strlen(line) != 0)
                    std::strcpy(m.u33a0[i], line);
            }
        }
        break;
    }
    case 0x44: {  // CVNAME: collection at M+0x33b8 (FUN_14019a4e0)
        status = read_ii(f, &iv[0], &iv[1]);
        if (status != 0)
            return 2;
        int cid = iv[0];  // local_d4
        int n = iv[1];    // local_d8
        char buf[64];     // local_118
        status = read_string_line(f, buf, 0x40);
        if (status != 0)
            return 2;
        def::model::cv_set_name(&m.u33b8, cid, std::string(buf));
        def::model::cv_clear_items(&m.u33b8, cid);
        for (int i = 0; i < n; ++i) {
            status = read_string_line(f, buf, 0x40);
            if (status != 0)
                return 2;
            def::model::cv_append_item(&m.u33b8, cid, std::string(buf));
        }
        break;
    }
    case 0x45:  // FRQNCY: rows M+0xa78, columns M+0xa7c, table M+0xa80 (ncomp M+0xac0)
        status = read_ii(f, &iv[0], &iv[1]);
        if (0 < iv[0] && 0 < iv[1]) {
            s.frqncy_rows = iv[0];
            s.frqncy_cols = iv[1];
            s.frqncy.ncomp = iv[0] * iv[1];
            status = read_table_body(f, &s.frqncy, 0);
        }
        break;
    default:
        return 2;
    }
    return status == 0 ? 0 : 2;
}

}  // namespace def::key::g1b
