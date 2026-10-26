// GENDB writer of the simulation control block (packet p4-06, group simctrl).
// Header: defdbio/gendb/p4_06_simctrl.hpp. Calls into other packets go through ext_p4_06.hpp.
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS // strncpy as in the original
#endif
#include <defdbio/gendb/p4_06_simctrl.hpp>

#include <cmath>
#include <cstring>

#include <defdbio/gendb/ext_p4_06.hpp>
#include <defmodel/dynary_init.hpp>
#include <defrt/ftnstr/ftnstr.hpp>
#include <defrt/stdlib/refcount.hpp>

namespace def::dbio::gendb {

using model::Cls_1401e7b10;
using model::Cls_1403753f0;
using model::DB_STEP_DATA;
using model::DYNARY;
using model::field;

// @src 140131600  self-describing FUNCTION_DATA writer
void write_func_selfdesc(const DYNARY* f, double* out)
{
    if (out == nullptr) {
        return;
    }
    out[0] = 10.0; // DAT_14051bee8
    out[1] = static_cast<double>(f->ndim);
    if (f->ndim == 0) {
        out[2] = static_cast<double>(f->ncomp);
        out[3] = f->value;
    } else if (f->ndim == 1) {
        out[2] = static_cast<double>(f->var[0]);
        out[3] = static_cast<double>(f->n[0]);
        out[4] = static_cast<double>(f->ncomp);
        // 1401316e0: COMISD 0.0, out[3]; JC skips the store when 0 < out[3]
        if (!(0.0 < out[3])) {
            out[4] = f->value;
        }
        ext::f1401317c0(f, out + 10);
    } else if (f->ndim == 2) {
        out[2] = static_cast<double>(f->var[0]);
        out[3] = static_cast<double>(f->var[1]);
        out[4] = static_cast<double>(f->n[0]);
        out[5] = static_cast<double>(f->n[1]);
        out[6] = static_cast<double>(f->ncomp);
        ext::f140131360(f, out + 10); // 1401317a8: CALL 140131360
    }
}

// @src 14021e170  magnitude of the 2/3 doubles at +0x170 of an OMV_DATA
double omv_magnitude(const void* omv)
{
    const double a = field<double>(omv, 0x170);
    const double b = field<double>(omv, 0x178);
    double r = 0.0;
    if (def::rt::dimension_is_2(field<const void*>(omv, 0xa80))) {
        r = std::sqrt(a * a + b * b); // 14021e1cf: (a*a) + (b*b)
    } else {
        const double c = field<double>(omv, 0x180);
        r = std::sqrt(a * a + b * b + c * c); // 14021e214, 14021e232: ((a*a) + (b*b)) + (c*c)
    }
    return r;
}

// @src 1401f3bb0  PDIE object movement magnitude (0 without a PDIE object)
double sim_pdie_magnitude(Cls_1401e7b10* s)
{
    model::LLOBJ_DATA* obj = ext::f1401d6ea0(s->owner, s->pdie1);
    double r = 0.0;
    if (obj == nullptr) {
        r = 0.0;
    } else {
        r = omv_magnitude(reinterpret_cast<const std::byte*>(obj) + 0x2160);
    }
    return r;
}

// @src 140375bb0  furnace record and its zone functions
void write_furnace_to_db(Cls_1403753f0* f, DB_STEP_DATA* m, std::int32_t* iifnc, double* llfnc, double* fid)
{
    std::int32_t start = 0;
    std::int32_t len = ext::f140375e70(f);
    const std::int32_t attr0 = 0;
    ext::f1401bb3d0(m, &len, iifnc, &attr0, fid, &start);
    double* rec = llfnc + (start - 1);
    rec[0] = static_cast<double>(ext::f140375e70(f));
    def::rt::to_fortran(reinterpret_cast<char*>(rec + 1), f->name, 0x40);
    std::int32_t first = 0;
    std::int32_t last = 0;
    const std::int32_t n = f->curve_count(); // 140375950
    double zfid = 0.0;
    for (std::int32_t i = 1; i <= n; i = i + 1) {
        DYNARY* c = f->curves.at(i); // 140136860
        if (c != nullptr) {
            len = ext::f140130380(c);
            ext::f1401bb3d0(m, &len, iifnc, &i, &zfid, &start); // attribute = zone number
            write_func_selfdesc(c, llfnc + (start - 1));
            if (first == 0) {
                first = static_cast<std::int32_t>(zfid); // cvttsd2si
            }
        }
    }
    last = static_cast<std::int32_t>(zfid);
    rec[9] = static_cast<double>(first);
    rec[10] = static_cast<double>(last);
}

namespace {

// part of @src 1401effc0: a curve (ndim > 0) in the plain layout, 2 * value count words,
// IIFNC attribute 0; the function id goes to *fid.
void put_curve(Cls_1401e7b10* s, DYNARY* f, std::int32_t* iifnc, double* llfnc, double* fid)
{
    std::int32_t start = 0;
    std::int32_t len = model::dynary_value_count(f); // 14012ff20
    len = len << 1;
    const std::int32_t attr = 0;
    ext::f1401bb3d0(s->owner, &len, iifnc, &attr, fid, &start);
    ext::f140131360(f, llfnc + (start - 1));
}

// part of @src 1401effc0: a table or list (FRQNCY, VFACTR) in the plain layout, 140130290 words
std::int32_t put_table(Cls_1401e7b10* s, DYNARY* f, std::int32_t* iifnc, double* llfnc)
{
    std::int32_t start = 0;
    const std::int32_t len = ext::f140130290(f);
    double fid = 0.0;
    const std::int32_t attr = 0;
    ext::f1401bb3d0(s->owner, &len, iifnc, &attr, &fid, &start);
    ext::f140131360(f, llfnc + (start - 1));
    return static_cast<std::int32_t>(fid); // cvttsd2si
}

// part of @src 1401effc0: TIMEID / ERECID / NRECID, self-describing; returns the id (0 if npts < 1)
std::int32_t put_record_id(Cls_1401e7b10* s, DYNARY* f, std::int32_t* iifnc, double* llfnc)
{
    if (f->n[0] < 1) {
        return 0;
    }
    std::int32_t start = 0;
    const std::int32_t len = ext::f140130380(f);
    double fid = 0.0;
    const std::int32_t attr = 0;
    ext::f1401bb3d0(s->owner, &len, iifnc, &attr, &fid, &start);
    write_func_selfdesc(f, llfnc + (start - 1));
    return static_cast<std::int32_t>(fid);
}

// part of @src 1401effc0: DTTYPE code from (mode S+0x240, xvar S+0x1f8)
std::int32_t dttype_code(const Cls_1401e7b10* s)
{
    std::int32_t code = 0;
    if (s->dttype.u58 == 0) {
        const std::int32_t x = s->dttype.var[0];
        if (x == 1) {
            code = 1;
        } else if (x == 2) {
            code = 2;
        } else {
            code = 1;
        }
    } else {
        const std::int32_t x = s->dttype.var[0];
        if (x == 1) {
            code = 3;
        } else if (x == 2) {
            code = 4;
        } else {
            code = 1;
        }
    }
    return code;
}

// part of @src 1401effc0: DTSUB clamp (1401f1429.. 2D, 1401f1f49.. 3D). `other` is the value
// for anything outside [0, 0.5), [0.5, 1] and 2 (1.0 in 3D, 0.0 in 2D), NaN included: every
// COMISD there branches away on unordered.
double dtsub_clamp(double x, double other, double* word)
{
    if (0.0 > x) { // COMISD 0.0, x; JBE (x < 0): 1.0, overwritten below in every path
        *word = 1.0;
    }
    double r = 0.0;
    if (x >= 0.0 && 0.5 > x) {
        r = x;
    } else if (x >= 0.5 && 1.0 >= x) {
        r = 1.0;
    } else if (x == 2.0) { // UCOMISD; JP / JNZ
        r = 2.0;
    } else {
        r = other;
    }
    return r;
}

// part of @src 1401effc0: USRDEF 1..10 into IIUVL, 80 characters each, blank filled
void put_usrdef(const Cls_1401e7b10* s, std::int32_t* iiuvl)
{
    char* out = reinterpret_cast<char*>(iiuvl);
    for (std::int32_t i = 0; i < 10; i = i + 1) {
        for (std::int32_t k = 0; k < 0x50; k = k + 1) {
            out[i * 0x50 + k] = ' ';
        }
        std::int32_t n = static_cast<std::int32_t>(std::strlen(s->usrdef[i]));
        if (0x4f < n) {
            n = 0x50;
        }
        std::strncpy(out + i * 0x50, s->usrdef[i], static_cast<std::size_t>(static_cast<std::int64_t>(n)));
    }
}

} // namespace

// @src 1401effc0  simulation control GENDB writer (IISIM, LLSIM, IIUVL, IIFNC/LLFNC)
void write_sim_ctrl_to_db(Cls_1401e7b10* s, std::int32_t* iisim, double* llsim, std::int32_t* iiuvl,
                          std::int32_t* iifnc, double* llfnc)
{
    std::int32_t* I = iisim;
    double* L = llsim;
    DB_STEP_DATA* m = s->owner;
    if (!def::rt::dimension_is_2(m)) {
        // ---- 3D ----
        L[0] = s->tnow1;
        L[2] = sim_pdie_magnitude(s);
        L[3] = s->dtmax1;
        L[4] = s->slderr;
        L[5] = s->demax;
        L[6] = s->dsmax;
        L[7] = s->dvmax[0];
        L[8] = s->dvmax[1];
        L[9] = s->tmax;
        L[10] = s->emax;
        L[0xb] = s->smax[0];
        L[0xc] = s->smax[1];
        L[0xd] = s->smax[2];
        L[0xe] = s->vmin[0];
        L[0xf] = s->vmin[1];
        L[0x10] = s->vmin[2];
        L[0x11] = s->lmax[0];
        L[0x12] = s->lmax[1];
        L[0x13] = s->lmax[2];
        L[0x14] = s->cvgerr[0];
        L[0x15] = s->cvgerr[1];
        L[0x16] = s->peninf;
        L[0x17] = s->geoerr[0];
        L[0x18] = s->geoerr[1];
        L[0x3b] = s->geoerr[2];
        L[0x3c] = s->geoerr[3];
        L[0x1a] = s->tintgf;
        L[0x1b] = s->unte2h;
        if (s->furnace_on == 0) {
            L[0x39] = 0.0;
            if (s->envtmp.ndim == 0) {
                L[0x1c] = 0.0;
                L[0x1d] = s->envtmp.value;
            } else {
                put_curve(s, &s->envtmp, iifnc, llfnc, L + 0x1c);
            }
        } else {
            L[0x39] = 10.0; // DAT_14051bee8
            write_furnace_to_db(&s->furnace, m, iifnc, llfnc, L + 0x3a);
        }
        if (s->cnvcof.ndim == 0) {
            L[0x1e] = 0.0;
            L[0x1f] = s->cnvcof.value;
        } else {
            put_curve(s, &s->cnvcof, iifnc, llfnc, L + 0x1e);
        }
        L[0x20] = s->blzman;
        L[0x21] = dtsub_clamp(s->dtsub, 1.0, L + 0x21);
        s->dtsub = L[0x21];
        L[0x22] = static_cast<double>(s->mdsobj[0]);
        L[0x23] = static_cast<double>(s->mdsobj[1]);
        L[0x24] = static_cast<double>(s->mdsobj[2]);
        L[0x25] = s->mdsobj4;
        L[0x26] = s->dtpmax1;
        L[0x27] = s->dtpmax2_5[0];
        L[0x28] = s->dtpmax2_5[1];
        if (s->envatm.ndim == 0) {
            L[0x29] = 0.0;
            L[0x2a] = s->envatm.value;
        } else {
            L[0x29] = 1.0;
            put_curve(s, &s->envatm, iifnc, llfnc, L + 0x2a);
        }
        if (s->acvcof.ndim == 0) {
            L[0x2b] = 0.0;
            L[0x2c] = s->acvcof.value;
        } else {
            if (s->acvcof.var[0] == 3) {
                L[0x2b] = 1.0;
            } else if (s->acvcof.var[0] == 0xc) {
                L[0x2b] = 2.0;
            } else if (s->acvcof.var[0] == 1) {
                L[0x2b] = 3.0;
            }
            put_curve(s, &s->acvcof, iifnc, llfnc, L + 0x2c);
        }
        L[0x2d] = 1.0;
        L[0x2e] = 2.0;
        L[0x2f] = 3.0;
        L[0x30] = s->envmpr;
        L[0x31] = s->envmpt;
        L[0x35] = s->ehrate;
        L[0x36] = s->odmax;
        L[0x37] = s->dtmax2;
        L[0x32] = s->tnow2;
        L[0x33] = static_cast<double>(s->tnow3);
        L[0x38] = s->tnow4;
        L[0x34] = s->dplen;
        I[0] = s->smode;
        I[1] = s->meshno;
        I[2] = s->nstart;
        I[3] = dbw_u068(m);
        I[4] = s->nstep;
        I[5] = s->stpinc;
        I[6] = s->stpdef;
        I[7] = s->pdie1;
        I[0x23] = s->pdie2;
        I[8] = 0;
        I[9] = s->itrmth;
        I[10] = s->itrmxd;
        I[0xb] = s->defbwd;
        I[0xc] = 0;
        I[0xd] = s->latent;
        I[0xe] = 0;
        I[0xf] = s->solmtt;
        I[0x10] = s->itrmxt;
        I[0x11] = s->tmpbwd;
        I[0x12] = s->unit;
        I[0x13] = ext::f140199da0(m);
        I[0x14] = ext::f140199db0(m);
        I[0x15] = dbw_u074(m);
        I[0x16] = ext::f140199c10(m);
        I[0x17] = ext::f1401f4610(m);
        I[0x18] = ext::f14019a350(m);
        I[0x19] = dbw_u084(m);
        I[0x1a] = dbw_u088(m);
        I[0x1b] = dbw_u08c(m);
        I[0x1c] = dbw_u090(m);
        I[0x1d] = dbw_u094(m);
        I[0x1e] = ext::f14019a200(m);
        I[0x1f] = dbw_u09c(m);
        I[0x20] = s->solmtd;
        if (s->solmtd == 2) {
            I[0x48] = static_cast<std::int32_t>(s->solmtd_p[0]); // cvttsd2si
            I[0x49] = static_cast<std::int32_t>(s->solmtd_p[1]);
            I[0x4a] = static_cast<std::int32_t>(s->solmtd_p[2]);
            I[0x4b] = static_cast<std::int32_t>(s->solmtd_p[3]);
        } else if (s->solmtd == 6) {
            L[0x3d] = s->solmtd_p[0];
            L[0x3e] = s->solmtd_p[1];
            L[0x3f] = s->solmtd_p[2];
            L[0x40] = s->solmtd_p[3];
        }
        I[0x4c] = s->frqncy_rows;
        I[0x4d] = s->frqncy_cols;
        I[0x4e] = 0;
        if (0 < s->frqncy_rows && 0 < s->frqncy_cols) {
            I[0x4e] = put_table(s, &s->frqncy, iifnc, llfnc);
        }
        I[0x50] = s->vfactr_mode;
        if (s->vfactr_mode == 1 && 0 < s->vfactr_n) {
            I[0x51] = s->vfactr.ncomp; // S+0xa20
            I[0x52] = put_table(s, &s->vfactr, iifnc, llfnc);
        }
        I[0x21] = dbw_u0bc(m);
        I[0x22] = dbw_u0c0(m);
        field<std::int32_t>(m, 0x1378) = I[0x17];
        field<std::int32_t>(m, 0x13f0) = I[0x18];
        field<std::int32_t>(m, 0x13f4) = I[0x21];
        field<std::int32_t>(m, 0x13e8) = I[0x13];
        field<std::int32_t>(m, 0x13ec) = I[0x14];
        field<std::int32_t>(m, 0x13fc) = I[0x1a];
        I[0x2b] = s->htmthd;
        I[0x2c] = s->trans[0];
        I[0x2d] = s->trans[1];
        I[0x2e] = s->trans[2];
        I[0x2f] = s->trans[3];
        I[0x30] = s->trans[4];
        I[0x31] = s->stype;
        for (std::int32_t k = 0; k < 7; ++k) {
            I[0x32 + k] = s->stnout[k];
        }
        for (std::int32_t k = 0; k < 10; ++k) {
            I[0x39 + k] = s->elmnod[k];
        }
        if (s->dttype.ndim == 0) {
            I[0x43] = 0;
            I[0x44] = 0;
        } else {
            I[0x43] = dttype_code(s);
            double fid = 0.0;
            put_curve(s, &s->dttype, iifnc, llfnc, &fid);
            I[0x44] = static_cast<std::int32_t>(fid);
        }
        I[0x47] = put_record_id(s, &s->timeid, iifnc, llfnc);
        I[0x53] = put_record_id(s, &s->erecid, iifnc, llfnc);
        I[0x54] = put_record_id(s, &s->nrecid, iifnc, llfnc);
        put_usrdef(s, iiuvl);
    } else {
        // ---- 2D ----
        L[0] = s->tnow1;
        L[0x2a] = s->tnow2;
        L[0x2b] = static_cast<double>(s->tnow3);
        L[0x27] = s->tnow4;
        L[2] = s->dtmax1;
        L[9] = s->dtmax2;
        L[3] = s->slderr;
        L[4] = s->demax;
        L[5] = s->dsmax;
        if (s->dttype.ndim == 0) {
            I[0x45] = 0;
            I[0x46] = 0;
        } else {
            I[0x45] = dttype_code(s);
            double fid = 0.0;
            put_curve(s, &s->dttype, iifnc, llfnc, &fid);
            I[0x46] = static_cast<std::int32_t>(fid);
        }
        L[6] = s->dtpmax1;
        L[0x1a] = s->dtpmax2_5[0];
        L[0x1b] = s->dtpmax2_5[1];
        L[7] = s->cvgerr[0];
        L[8] = s->cvgerr[1];
        L[10] = s->peninf;
        L[0xc] = s->geoerr[0];
        L[0xd] = s->geoerr[1];
        L[0x32] = s->geoerr[2];
        L[0x33] = s->geoerr[3];
        L[0xe] = s->u680;
        L[0xf] = s->tintgf;
        L[0x10] = s->unte2h;
        if (s->envtmp.ndim == 0) {
            L[0x17] = 0.0;
            L[0x11] = s->envtmp.value;
        } else {
            put_curve(s, &s->envtmp, iifnc, llfnc, L + 0x17);
        }
        if (s->cnvcof.ndim == 0) {
            L[0x12] = 0.0;
            L[0x13] = s->cnvcof.value;
        } else {
            put_curve(s, &s->cnvcof, iifnc, llfnc, L + 0x12);
        }
        L[0x14] = s->emax;
        L[0x15] = s->tmax;
        L[0x16] = s->blzman;
        L[0x18] = s->dvmax[0];
        L[0x19] = s->dvmax[1];
        L[0x1c] = static_cast<double>(s->mdsobj[0]);
        L[0x1d] = static_cast<double>(s->mdsobj[1]);
        L[0x1e] = static_cast<double>(s->mdsobj[2]);
        L[0x1f] = s->mdsobj4;
        L[0x20] = 1.0;
        L[0x21] = 2.0;
        L[0x22] = 3.0;
        if (s->envatm.ndim == 0) {
            L[0x23] = 0.0;
            L[0x24] = s->envatm.value;
        } else {
            L[0x23] = 1.0;
            put_curve(s, &s->envatm, iifnc, llfnc, L + 0x24);
        }
        if (s->acvcof.ndim == 0) {
            L[0x25] = 0.0;
            L[0x26] = s->acvcof.value;
        } else {
            if (s->acvcof.var[0] == 3) {
                L[0x25] = 1.0;
            } else if (s->acvcof.var[0] == 0xc) {
                L[0x25] = 2.0;
            } else if (s->acvcof.var[0] == 1) {
                L[0x25] = 3.0;
            }
            put_curve(s, &s->acvcof, iifnc, llfnc, L + 0x26);
        }
        L[0x28] = s->envmpr;
        L[0x29] = s->envmpt;
        L[0x2e] = s->ehrate;
        I[0x33] = s->vfactr_mode;
        L[0x2c] = static_cast<double>(s->cnterr1);
        L[0x2d] = s->cnterr2;
        I[0] = s->smode;
        I[1] = s->stype;
        I[2] = s->geotyp;
        I[3] = s->meshno;
        I[4] = s->nstart;
        I[5] = dbw_u068(m);
        I[6] = s->nstep;
        I[7] = s->stpinc;
        I[8] = s->stpdef;
        I[9] = s->pdie1;
        I[0xb] = 0;
        I[0xc] = 1;
        I[0xd] = 1;
        I[0xf] = s->solmtd;
        if (s->solmtd == 2) {
            I[0x48] = static_cast<std::int32_t>(s->solmtd_p[0]);
            I[0x49] = static_cast<std::int32_t>(s->solmtd_p[1]);
            I[0x4a] = static_cast<std::int32_t>(s->solmtd_p[2]);
            I[0x4b] = static_cast<std::int32_t>(s->solmtd_p[3]);
        } else if (s->solmtd == 6) {
            L[0x3d] = s->solmtd_p[0];
            L[0x3e] = s->solmtd_p[1];
            L[0x3f] = s->solmtd_p[2];
            L[0x40] = s->solmtd_p[3];
        }
        I[0x4c] = s->frqncy_rows;
        I[0x4d] = s->frqncy_cols;
        I[0x4e] = 0;
        if (0 < s->frqncy_rows && 0 < s->frqncy_cols) {
            I[0x4e] = put_table(s, &s->frqncy, iifnc, llfnc);
        }
        I[0x10] = s->itrmth;
        I[0x11] = s->itrmxd;
        I[0x12] = 1;
        I[0x13] = s->defbwd;
        I[0x16] = s->solmtt;
        I[0x17] = s->itrmxt;
        I[0x18] = s->tmpbwd;
        I[0x19] = s->unit;
        I[0x1a] = ext::f140199da0(m);
        I[0x1b] = ext::f140199db0(m);
        I[0x1c] = dbw_u074(m);
        I[0x1d] = ext::f140199c10(m);
        I[0x1e] = ext::f1401f4610(m);
        I[0x1f] = ext::f14019a350(m);
        I[0x20] = dbw_u084(m);
        I[0x21] = dbw_u088(m);
        I[0x22] = dbw_u08c(m);
        I[0x24] = s->pdie2;
        I[0x25] = dbw_u0bc(m);
        I[0x27] = dbw_u0c0(m);
        I[0x28] = s->latent;
        I[0x29] = s->osctrl[0];
        I[0x2a] = s->osctrl[1];
        I[0x2b] = s->htmthd;
        I[0x2c] = s->trans[0];
        I[0x2d] = s->trans[1];
        I[0x2e] = s->trans[2];
        I[0x2f] = s->trans[3];
        I[0x32] = s->trans[4];
        I[0x30] = s->osctrl[2];
        I[0x31] = s->osctrl[3];
        for (std::int32_t k = 0; k < 7; ++k) {
            I[0x34 + k] = s->stnout[k];
        }
        for (std::int32_t k = 0; k < 10; ++k) {
            I[0x3b + k] = s->elmnod[k];
        }
        L[0x2f] = s->dtpmax2_5[2];
        L[0x30] = s->dtpmax2_5[3];
        L[0x31] = dtsub_clamp(s->dtsub, 0.0, L + 0x31);
        s->dtsub = L[0x31];
        I[0x47] = put_record_id(s, &s->timeid, iifnc, llfnc);
        I[0x53] = put_record_id(s, &s->erecid, iifnc, llfnc);
        I[0x54] = put_record_id(s, &s->nrecid, iifnc, llfnc);
        put_usrdef(s, iiuvl);
    }
}

} // namespace def::dbio::gendb
