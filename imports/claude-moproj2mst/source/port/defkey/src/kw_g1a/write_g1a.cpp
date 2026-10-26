// kw-g1a: group 1 KEY writer, sections "Process Definition" and "Stopping & Step Controls" of
// FUN_1403d4990, and the per-keyword writers it calls (registry column 2 for EMAX .. CNTERR).
// Formats are the binary's strings; the keyword name comes from FUN_1402f7720 (keyword_label).
// Output goes through def::rt::crt::fprintf, the VS2008 CRT's printf, so that doubles round as
// DEF_PRE rounds them.
#include <defkey/kw_g1a.hpp>

#include <cstdint>

#include <defkey/key_writer.hpp>
#include <defkey/table_io.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

#include "sim_ctrl_ops.hpp"

namespace def::key::g1a {

namespace {

namespace rtcrt = def::rt::crt;

constexpr double kTnowMasked = -1e21;  // DAT_140522f78: TNOW[0] when write option bit 8 is set

} // namespace

// @src 1403d7720  EMAX
int write_emax(std::FILE* f, model::DB_STEP_DATA* m, const KeyWriteVec&)
{
    rtcrt::fprintf(f, "%-6s %20.10E\n", keyword_label(0x29), m->sim_ctrl.emax);
    return 0;
}

// @src 1403d77a0  TMAX
int write_tmax(std::FILE* f, model::DB_STEP_DATA* m, const KeyWriteVec&)
{
    rtcrt::fprintf(f, "%-6s %20.10E\n", keyword_label(0x2a), m->sim_ctrl.tmax);
    return 0;
}

namespace {

// SMAX, VMIN and LMAX share one shape: three values in 3D, two in 2D (the test is "not 2D").
int write_triple(std::FILE* f, const model::DB_STEP_DATA* m, int id, const double* v)
{
    if (!ops::is_2d(*m)) {
        rtcrt::fprintf(f, "%-6s %20.10E %20.10E %20.10E\n", keyword_label(id), v[0], v[1], v[2]);
    } else {
        rtcrt::fprintf(f, "%-6s %20.10E %20.10E\n", keyword_label(id), v[0], v[1]);
    }
    return 0;
}

} // namespace

// @src 1403d7820  SMAX
int write_smax(std::FILE* f, model::DB_STEP_DATA* m, const KeyWriteVec&)
{
    return write_triple(f, m, 0x2b, m->sim_ctrl.smax);
}

// @src 1403d7910  VMIN
int write_vmin(std::FILE* f, model::DB_STEP_DATA* m, const KeyWriteVec&)
{
    return write_triple(f, m, 0x2c, m->sim_ctrl.vmin);
}

// @src 1403d7a00  LMAX
int write_lmax(std::FILE* f, model::DB_STEP_DATA* m, const KeyWriteVec&)
{
    return write_triple(f, m, 0x2d, m->sim_ctrl.lmax);
}

// @src 1403d7af0  DEMAX
int write_demax(std::FILE* f, model::DB_STEP_DATA* m, const KeyWriteVec&)
{
    rtcrt::fprintf(f, "%-6s %20.10E\n", keyword_label(0x2f), m->sim_ctrl.demax);
    return 0;
}

// @src 1403d7b70  DSMAX
int write_dsmax(std::FILE* f, model::DB_STEP_DATA* m, const KeyWriteVec&)
{
    rtcrt::fprintf(f, "%-6s %20.10E\n", keyword_label(0x30), m->sim_ctrl.dsmax);
    return 0;
}

// @src 1403d7bf0  DTMAX: both values when the time flag is 1; else "0 DTMAX[1]" when DTMAX[1] > 0;
// else nothing
int write_dtmax(std::FILE* f, model::DB_STEP_DATA* m, const KeyWriteVec&)
{
    model::Cls_1401e7b10& s = m->sim_ctrl;
    if (ops::time_flag(s) == 1) {
        rtcrt::fprintf(f, "%-6s %20.10E %20.10E\n", keyword_label(0x32), s.dtmax1, s.dtmax2);
    } else if (0.0 < s.dtmax2) {  // _DAT_14051bed8 = 0.0
        rtcrt::fprintf(f, "%-6s %20.10E %20.10E\n", keyword_label(0x32), 0.0, s.dtmax2);
    }
    return 0;
}

// @src 1403d7ce0  DTTYPE table: header code 1/2 (flag 0) or 3/4 (flag 1) for var 1/2, no header
// for any other var code; the data are written either way
int write_dttype(std::FILE* f, model::DB_STEP_DATA* m, const KeyWriteVec&)
{
    model::Cls_1401e7b10& s = m->sim_ctrl;
    if (s.dttype.u58 == 0) {  // M+0x990, read directly here
        if (s.dttype.var[0] == 1) {
            rtcrt::fprintf(f, "%-6s %7i %7i\n", keyword_label(0x33), 1, s.dttype.n[0]);
        } else if (s.dttype.var[0] == 2) {
            rtcrt::fprintf(f, "%-6s %7i %7i\n", keyword_label(0x33), 2, s.dttype.n[0]);
        }
    } else if (s.dttype.var[0] == 1) {
        rtcrt::fprintf(f, "%-6s %7i %7i\n", keyword_label(0x33), 3, s.dttype.n[0]);
    } else if (s.dttype.var[0] == 2) {
        rtcrt::fprintf(f, "%-6s %7i %7i\n", keyword_label(0x33), 4, s.dttype.n[0]);
    }
    write_table_body(f, &s.dttype, 0);  // result ignored
    return 0;
}

// @src 1403d7e80  DTTYPE registry column 3
int dttype_table(model::DB_STEP_DATA* m, void*, const KeyWriteVec&, model::DYNARY** out)
{
    // The original tests S = M + 0x750 against null.
    if (reinterpret_cast<std::uintptr_t>(m) + 0x750 != 0) *out = &m->sim_ctrl.dttype;
    return 0;
}

// @src 1403d7ef0  SLDERR
int write_slderr(std::FILE* f, model::DB_STEP_DATA* m, const KeyWriteVec&)
{
    rtcrt::fprintf(f, "%-6s %20.10E\n", keyword_label(0x34), m->sim_ctrl.slderr);
    return 0;
}

// @src 1403d7f70  DTPMAX: three values in 3D (not 2D), five in 2D
int write_dtpmax(std::FILE* f, model::DB_STEP_DATA* m, const KeyWriteVec&)
{
    model::Cls_1401e7b10& s = m->sim_ctrl;
    if (!ops::is_2d(*m)) {
        rtcrt::fprintf(f, "%-6s %20.10E %20.10E %20.10E\n", keyword_label(0x35), s.dtpmax1,
                     s.dtpmax2_5[0], s.dtpmax2_5[1]);
    } else {
        rtcrt::fprintf(f, "%-6s %20.10E %20.10E %20.10E %20.10E %20.10E\n", keyword_label(0x35),
                     s.dtpmax1, s.dtpmax2_5[0], s.dtpmax2_5[1], s.dtpmax2_5[2],
                     s.dtpmax2_5[3]);
    }
    return 0;
}

// @src 1403d80a0  DVMAX
int write_dvmax(std::FILE* f, model::DB_STEP_DATA* m, const KeyWriteVec&)
{
    rtcrt::fprintf(f, "%-6s %20.10E %20.10E\n", keyword_label(0x36), m->sim_ctrl.dvmax[0],
                 m->sim_ctrl.dvmax[1]);
    return 0;
}

// @src 1403d8130  MDSOBJ
int write_mdsobj(std::FILE* f, model::DB_STEP_DATA* m, const KeyWriteVec&)
{
    model::Cls_1401e7b10& s = m->sim_ctrl;
    rtcrt::fprintf(f, "%-6s %7i %7i %7i %20.10E\n", keyword_label(0x37), s.mdsobj[0], s.mdsobj[1],
                 s.mdsobj[2], s.mdsobj4);
    return 0;
}

// @src 1403d81d0  CNTERR
int write_cnterr(std::FILE* f, model::DB_STEP_DATA* m, const KeyWriteVec&)
{
    rtcrt::fprintf(f, "%-6s %7i %20.10E\n", keyword_label(0x38), m->sim_ctrl.cnterr1,
                 m->sim_ctrl.cnterr2);
    return 0;
}

// @src 1403d6580  STNOUT
int write_stnout(model::DB_STEP_DATA* m, std::FILE* f)
{
    model::Cls_1401e7b10& s = m->sim_ctrl;
    rtcrt::fprintf(f, "%-6s %7i %7i %7i %7i %7i %7i %7i\n", keyword_label(0x22), s.stnout[0], s.stnout[1],
                 s.stnout[2], s.stnout[3], s.stnout[4], s.stnout[5], s.stnout[6]);
    return 0;
}

// @src 1403d6620  ELMNOD: always the count 10, then the ten ints on one line
int write_elmnod(model::DB_STEP_DATA* m, std::FILE* f)
{
    rtcrt::fprintf(f, "%-6s %7i\n", keyword_label(0x23), 10);
    write_ints(f, 10, m->sim_ctrl.elmnod, 10, false);
    return 0;
}

// part of @src 1403d4990  group 1 KEY writer: "Process Definition" and "Stopping & Step Controls"
int write_sections(std::FILE* f, model::DB_STEP_DATA& m)
{
    model::Cls_1401e7b10& s = m.sim_ctrl;
    write_section_banner(f, "Process Definition");
    rtcrt::fprintf(f, "%-6s\n%s\n", keyword_label(0x16), s.title);
    rtcrt::fprintf(f, "%-6s %7i %7i %7i\n", keyword_label(0x17), s.cursim[0], s.cursim[1], s.cursim[2]);
    rtcrt::fprintf(f, "%-6s\n%s\n", keyword_label(0x18), s.oprnam);
    rtcrt::fprintf(f, "%-6s\n%s\n", keyword_label(0x19), s.simnam);
    ops::recompute_smode(s);
    rtcrt::fprintf(f, "%-6s %7i\n", keyword_label(0x1b), s.smode);
    rtcrt::fprintf(f, "%-6s %7i\n", keyword_label(0x1a), s.stype);
    if (ops::is_2d(m)) rtcrt::fprintf(f, "%-6s %7i\n", keyword_label(0x1c), s.geotyp);
    if ((m.u1744 & 4) == 0) rtcrt::fprintf(f, "%-6s %7i\n", keyword_label(0x1d), s.meshno);
    rtcrt::fprintf(f, "%-6s %7i\n", keyword_label(0x1e), s.unit);
    if ((m.u1744 & 8) == 0) {
        rtcrt::fprintf(f, "%-6s %20.10E %20.10E %7i %20.10E\n", keyword_label(0x1f), s.tnow1, s.tnow2,
                     s.tnow3, s.tnow4);
    } else {
        rtcrt::fprintf(f, "%-6s %20.10E %20.10E %7i %20.10E\n", keyword_label(0x1f), kTnowMasked, 0.0,
                     s.tnow3, s.tnow4);
    }
    rtcrt::fprintf(f, "%-6s %7i %7i %7i %7i %7i\n", keyword_label(0x20), s.trans[0], s.trans[1],
                 s.trans[2], s.trans[3], s.trans[4]);
    rtcrt::fprintf(f, "%-6s %7i\n", keyword_label(0x21), s.htmthd);
    write_stnout(&m, f);  // results ignored
    write_elmnod(&m, f);

    write_section_banner(f, "Stopping & Step Controls");
    if ((m.u1744 & 1) == 0) rtcrt::fprintf(f, "%-6s %7i\n", keyword_label(0x24), s.nstart);
    rtcrt::fprintf(f, "%-6s %7i\n", keyword_label(0x25), s.stpinc);
    rtcrt::fprintf(f, "%-6s %7i %7i\n", keyword_label(0x26), s.pdie1, s.pdie2);
    if (ops::is_2d(m)) {
        rtcrt::fprintf(f, "%-6s %7i %7i %7i %7i\n", keyword_label(0x27), s.osctrl[0], s.osctrl[1],
                     s.osctrl[2], s.osctrl[3]);
    }
    rtcrt::fprintf(f, "%-6s %7i\n", keyword_label(0x28), s.nstep);

    // Each per-keyword writer gets a fresh vector (FUN_140159860); the first non-zero result
    // ends the KEY write.
    int r;
    if ((r = write_emax(f, &m, KeyWriteVec{})) != 0) return r;
    if ((r = write_tmax(f, &m, KeyWriteVec{})) != 0) return r;
    if ((r = write_smax(f, &m, KeyWriteVec{})) != 0) return r;
    if ((r = write_vmin(f, &m, KeyWriteVec{})) != 0) return r;
    if ((r = write_lmax(f, &m, KeyWriteVec{})) != 0) return r;
    rtcrt::fprintf(f, "%-6s %7i\n", keyword_label(0x2e), s.stpdef);
    if ((r = write_demax(f, &m, KeyWriteVec{})) != 0) return r;
    if (s.dttype.ndim == 0) {
        if (ops::time_flag(s) == 0) {
            if ((r = write_dsmax(f, &m, KeyWriteVec{})) != 0) return r;
        }
        if ((r = write_dtmax(f, &m, KeyWriteVec{})) != 0) return r;
        rtcrt::fprintf(f, "%-6s %7i %7i\n", keyword_label(0x33), 0, 0);
    } else {
        if (s.dttype.ndim != 1) return 3;
        if ((r = write_dttype(f, &m, KeyWriteVec{})) != 0) return r;
    }
    if ((r = write_slderr(f, &m, KeyWriteVec{})) != 0) return r;
    if ((r = write_dtpmax(f, &m, KeyWriteVec{})) != 0) return r;
    if ((r = write_dvmax(f, &m, KeyWriteVec{})) != 0) return r;
    if ((r = write_mdsobj(f, &m, KeyWriteVec{})) != 0) return r;
    if (!ops::is_2d(m)) {
        if (s.vfactr_mode == 0) {
            rtcrt::fprintf(f, "%-6s %7i\n", keyword_label(0x39), s.vfactr_mode);
        } else if (s.vfactr_mode == 1 && 0 < s.vfactr_n) {
            rtcrt::fprintf(f, "%-6s %7i %7i\n", keyword_label(0x39), s.vfactr_mode, s.vfactr.ncomp);
            const double* v = s.vfactr.values;
            for (int i = 0; i < s.vfactr.ncomp; ++i) rtcrt::fprintf(f, " %7i", ops::trunc_i32(v[i]));
            rtcrt::fprintf(f, "\n");
        } else {
            rtcrt::fprintf(f, "%-6s %7i %7i\n", keyword_label(0x39), s.vfactr_mode, s.vfactr_n);
        }
    } else {
        if ((r = write_cnterr(f, &m, KeyWriteVec{})) != 0) return r;
        rtcrt::fprintf(f, "%-6s %7i\n", keyword_label(0x39), s.vfactr_mode);
    }
    rtcrt::fprintf(f, "%-6s %20.10E\n", keyword_label(0x3a), s.dtsub);
    if (ops::is_3d(m)) {
        rtcrt::fprintf(f, "%-6s %20.10E\n", keyword_label(0x3b), s.dplen);
        double od = ops::get_odmax(s);
        rtcrt::fprintf(f, "%-6s %20.10E\n", keyword_label(0x3c), od);
    }
    return 0;
}

} // namespace def::key::g1a
