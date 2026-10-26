// kw-g1a: group 1 KEY reader, cases 0x00..0x26 (ids 22..60) of FUN_14039aa90.
// Per-keyword grammar: docs/re/keywords/<KW>.md. The case bodies follow the decompile line by
// line; "st" is the original's local_5e8 (helper status), mapped to 2 at the end.
#define _CRT_SECURE_NO_WARNINGS  // the original's plain CRT calls (strncpy, sscanf, fscanf)
#include <defkey/kw_g1a.hpp>

#include <cmath>
#include <cstdint>

#include <defrt/ftn/unit_system.hpp>
#include <defmodel/step/name_arrays.hpp>
#include <defmodel/layout/db_step_data.hpp>

#include <defmodel/dynary_init.hpp>

#include <defkey/line_io.hpp>
#include <defkey/table_io.hpp>
#include "sim_ctrl_ops.hpp"

namespace def::key::g1a {

namespace {

constexpr double kUnset = -1e20;  // _DAT_14055adc0: TNOW fields <= this keep the old value

void rebuild_state_layout(model::DB_STEP_DATA& m)
{
    if (model_hooks().rebuild_state_layout != nullptr)
        model_hooks().rebuild_state_layout(&m, 0);
    else
        def::model::call_1401d0fc0(&m, 0);  // FUN_1401d0fc0(M, 0), kw-g4b
}

} // namespace

ModelHooks& model_hooks()
{
    static ModelHooks hooks;
    return hooks;
}

// part of @src 14039aa90  group 1 parser: the cases of ids 22..60
int read_keyword(model::DB_STEP_DATA& m, std::FILE* f, int id)
{
    using namespace ops;
    model::Cls_1401e7b10& s = m.sim_ctrl;
    int st = 0;                // local_5e8
    std::int32_t dummy = 0;    // local_5f8: MESHNO/NSTART sink under read options
    switch (id - 0x16) {
    case 0x00:  // TITLE
        st = read_string_next_line(f, s.title, 0x51);
        break;
    case 0x01:  // CURSIM
        st = read_iii(f, &s.cursim[0], &s.cursim[1], &s.cursim[2]);
        break;
    case 0x02:  // OPRNAM
        st = read_string_next_line(f, s.oprnam, 0x41);
        break;
    case 0x03:  // SIMNAM
        st = read_string_next_line(f, s.simnam, 0x51);
        break;
    case 0x04:  // STYPE
        st = read_i(f, &s.stype, false);
        break;
    case 0x05: {  // SMODE
        std::int32_t v = 0;  // local_624
        st = read_i(f, &v, false);
        set_smode(s, v);
        break;
    }
    case 0x06:  // GEOTYP: 2D only; in 3D nothing is read
        if (is_2d(m)) {
            st = read_i(f, &s.geotyp, false);
            rebuild_state_layout(m);
        }
        break;
    case 0x07:  // MESHNO
        if ((m.u1740 & 2) == 0) {
            st = read_i(f, &s.meshno, false);
        } else {
            st = read_i(f, &dummy, false);
        }
        break;
    case 0x08: {  // UNIT: only a change to 1 or 2 is applied
        std::int32_t v = 0;  // local_620[0]
        st = read_i(f, &v, false);
        if (v != s.unit && (v == 1 || v == 2)) {
            s.unit = v;
            reset_unit_defaults(s);
            if (model_hooks().set_unit_system != nullptr)
                model_hooks().set_unit_system(s.unit);
            else
                def::rt::ftn::set_unit_system(s.unit);  // FUN_140235c30
        }
        break;
    }
    case 0x09: {  // TNOW
        double a, b, c, d;  // local_678, local_670, local_668, local_660[0]
        st = read_d4(f, &a, &b, &c, &d);
        if (kUnset < a) s.tnow1 = a;
        if (kUnset < b) s.tnow2 = b;
        s.tnow3 = trunc_i32(c);
        if (kUnset < d) s.tnow4 = d;
        break;
    }
    case 0x0a: {  // TRANS: only after a successful read; negative values keep the old flag
        std::int32_t v[5];  // local_620
        st = read_i5(f, &v[0], &v[1], &v[2], &v[3], &v[4], false);
        if (st == 0) {
            for (int i = 0; i < 5; ++i) {
                if (-1 < v[i]) s.trans[i] = v[i];
            }
        }
        break;
    }
    case 0x0b:  // HTMTHD (S+0x118)
        st = read_i(f, &s.htmthd, false);
        break;
    case 0x0c: {  // STNOUT: nine ints, the last two discarded
        std::int32_t x8, x9;  // local_620[0], local_620[1]
        std::int32_t* const out[9] = {&s.stnout[0], &s.stnout[1], &s.stnout[2], &s.stnout[3],
                                      &s.stnout[4], &s.stnout[5], &s.stnout[6], &x8,          &x9};
        st = read_i9(f, out, false);
        rebuild_state_layout(m);
        break;
    }
    case 0x0d: {  // ELMNOD: count, then up to 10 ints on the next line (its status is ignored)
        std::int32_t n = 0;  // local_688
        st = read_i(f, &n, false);
        read_i10(f, s.elmnod, &n);
        rebuild_state_layout(m);
        break;
    }
    case 0x0e:  // NSTART
        if ((m.u1740 & 4) == 0) {
            st = read_i(f, &s.nstart, false);
            if (st == 0) m.u2f88 = 1;
        } else {
            st = read_i(f, &dummy, false);
        }
        break;
    case 0x0f:  // STPINC
        st = read_i(f, &s.stpinc, false);
        break;
    case 0x10:  // PDIE (M+0x8b8, M+0x1104)
        st = read_ii(f, &s.pdie1, &s.pdie2);
        break;
    case 0x11:  // OSCTRL: 2D only
        if (is_2d(m)) st = read_i4(f, &s.osctrl[0], &s.osctrl[1], &s.osctrl[2], &s.osctrl[3]);
        break;
    case 0x12:  // NSTEP
        st = read_i(f, &s.nstep, false);
        break;
    case 0x13:  // EMAX
        st = read_d(f, &s.emax);
        break;
    case 0x14:  // TMAX
        st = read_d(f, &s.tmax);
        break;
    case 0x15:    // SMAX
    case 0x16:    // VMIN
    case 0x17: {  // LMAX: three values in 3D, two in 2D
        double* v = id - 0x16 == 0x15 ? s.smax : id - 0x16 == 0x16 ? s.vmin : s.lmax;
        if (!is_2d(m)) {
            st = read_d3(f, &v[0], &v[1], &v[2]);
        } else {
            st = read_dd(f, &v[0], &v[1]);
        }
        break;
    }
    case 0x18:  // STPDEF
        st = read_i(f, &s.stpdef, false);
        break;
    case 0x19:  // DEMAX
        st = read_d(f, &s.demax);
        break;
    case 0x1a:  // DSMAX: a positive value clears DTMAX[0] and recomputes the time flag
        st = read_d(f, &s.dsmax);
        if (0.0 < s.dsmax) {  // _DAT_14051bed8 = 0.0
            s.dtmax1 = 0;
            recompute_time_flag(s);
        }
        break;
    case 0x1b: {  // DSMAX_NSTEP: DSMAX = (|d| - |PDIE[0] object's stroke|) / n
        double d;            // local_5d8
        std::int32_t n = 0;  // local_5c8
        read_d_i(f, &d, &n);  // its status is not kept
        if (st != 0) return 2;   // tests local_5e8, which is still 0 here
        model::LLOBJ_DATA* obj = find_object(m, s.pdie1);
        if (obj != nullptr && n != 0 && d != 0.0) {
            double a = std::fabs(d);                     // FUN_140484558
            double b = std::fabs(object_stroke(*obj));
            s.dsmax = (a - b) / static_cast<double>(n);
        }
        break;
    }
    case 0x1c: {  // DTMAX: a positive DTMAX[0] clears DSMAX and recomputes the time flag
        st = read_dd(f, &s.dtmax1, &s.dtmax2);
        if (0.0 < s.dtmax1) {
            set_dsmax(s, 0.0);
            recompute_time_flag(s);
        }
        break;
    }
    case 0x1d: {  // DTTYPE: type and count, then the table data
        std::int32_t v[2];  // local_620
        st = read_ii(f, &v[0], &v[1]);
        if (st != 0) return 2;
        model::dynary_reinit_keep(&s.dttype, 0x80);
        if (v[0] == 0) {
            s.dttype.ndim = 0;
            s.dttype.value = 0;  // stored as a zero quadword
            recompute_time_flag(s);
        } else {
            s.dttype.ndim = 1;
            s.dttype.n[0] = v[1];
            std::int32_t flag;  // local_d0 .. local_c0
            if (v[0] == 1) {
                s.dttype.var[0] = 1;
                flag = 0;
            } else if (v[0] == 2) {
                s.dttype.var[0] = 2;
                flag = 0;
            } else if (v[0] == 3) {
                s.dttype.var[0] = 1;
                flag = 1;
            } else if (v[0] == 4) {
                s.dttype.var[0] = 2;
                flag = 1;
            } else {
                s.dttype.var[0] = 1;
                flag = 0;
            }
            set_time_flag(s, &flag);
        }
        st = read_table_body(f, &s.dttype);
        break;
    }
    case 0x1e:  // SLDERR
        st = read_d(f, &s.slderr);
        break;
    case 0x1f:  // DTPMAX: three values in 3D (one value resets all), five in 2D
        if (!is_2d(m)) {
            st = read_d3_not_one(f, &s.dtpmax1, &s.dtpmax2_5[0], &s.dtpmax2_5[1]);
        } else {
            double* const out[5] = {&s.dtpmax1, &s.dtpmax2_5[0], &s.dtpmax2_5[1],
                                    &s.dtpmax2_5[2], &s.dtpmax2_5[3]};
            st = read_d5(f, out);
        }
        break;
    case 0x20:  // DVMAX
        st = read_dd(f, &s.dvmax[0], &s.dvmax[1]);
        break;
    case 0x21:  // MDSOBJ
        st = read_iii_d(f, &s.mdsobj[0], &s.mdsobj[1], &s.mdsobj[2], &s.mdsobj4);
        break;
    case 0x22:  // CNTERR: 2D only
        if (is_2d(m)) st = read_i_d(f, &s.cnterr1, &s.cnterr2);
        break;
    case 0x23:  // VFACTR
        if (!is_2d(m)) {
            std::int32_t n = 0;  // local_5c4
            st = read_ii(f, &s.vfactr_mode, &n);
            if (s.vfactr_mode == 1 && 0 < n) {
                s.vfactr_n = n;
                s.vfactr.ncomp = n;
                read_table_vector(f, &s.vfactr);  // status ignored
            } else {
                s.vfactr_n = 0;
            }
        } else {
            st = read_i(f, &s.vfactr_mode, false);
        }
        break;
    case 0x24:  // DTSUB
        st = read_d(f, &s.dtsub);
        break;
    case 0x25:  // DPLEN: 3D only
        if (is_3d(m)) st = read_d(f, &s.dplen);
        break;
    case 0x26:  // ODMAX: 3D only
        if (is_3d(m)) {
            double v = 0;  // local_5f0
            st = read_d(f, &v);
            set_odmax(s, v);
        }
        break;
    default:
        return 2;
    }
    return st == 0 ? 0 : 2;
}

} // namespace def::key::g1a
