// DBREAD helper W2 (packet p-dbread): ReadSimCtrl 1401cf4a0 and what it calls: the sim-control
// block 1401eb870, the node / element name blocks 1401cf930 / 1401cfc30, the furnace record
// 1403759b0 and the self-describing function reader 140130dc0 (+ 140131e30, 140131fa0).
// Word map: docs/re/dbmap/simctrl.md (branch claude/project-thread-bz5k5e). Reference: decompile
// out/dbread/w2/ann_<addr>.c / raw_<addr>.c and the listing out/dbread/w2/listing.txt
// (deform-rea-work).
//
// I[k] is IISIM word k (int32), L[k] LLSIM word k (real64). Conversions (int)L[k] are CVTTSD2SI
// (truncation), (double)I[k] CVTSI2SD. Everything else is a plain copy.
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS // strncpy as in the original
#endif
#include <defdbio/dbread/w2.hpp>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <defdbio/gendb/ext_p4_06.hpp>
#include <defdbio/gendb/gate_ports.hpp>
#include <defmodel/dynary_init.hpp>
#include <defmodel/layout/cls_1401364a0.hpp>
#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/cls_1403753f0.hpp>
#include <defmodel/linked/accessors.hpp>
#include <defmodel/step/db_step.hpp>
#include <defmodel/step/name_arrays.hpp>
#include <defrt/ftnstr/ftnstr.hpp>
#include <defrt/port.hpp>
#include <defrt/stdlib/refcount.hpp>

#include "dbread/pending_w2.hpp"

namespace def::dbio::dbread {

using model::Cls_1401e7b10;
using model::DB_STEP_DATA;
using model::DYNARY;

namespace {

// value of the A-table index global DAT_<addr> (DAT_1406e82d0.. = g_a_index)
inline std::int32_t A(std::uint64_t addr) { return model::g_a_index[(addr - 0x1406e82d0ull) / 4]; }
// value of the MAR-table index global DAT_<addr> (DAT_1406e81fc.. = g_mar_index)
inline std::int32_t M(std::uint64_t addr) { return model::g_mar_index[(addr - 0x1406e81fcull) / 4]; }
// the address of such a global (the binary passes &DAT_1406e8328 as a region argument)
inline const std::int32_t* A_ptr(std::uint64_t addr) { return &model::g_a_index[(addr - 0x1406e82d0ull) / 4]; }

// 1-based region start of the A / MAR address tables (DB_STEP_DATA+0x1dc0 / +0x1b90)
inline std::int32_t a_start(const DB_STEP_DATA* m, std::uint64_t addr) { return m->u1dc0[A(addr)]; }
inline std::int32_t mar_start(const DB_STEP_DATA* m, std::uint64_t addr) { return m->u1b90[M(addr)]; }

// 140158e90 + 1401c7d40
inline void select_tables(DB_STEP_DATA* m)
{
    std::int32_t dim = m->dimension;
    model::select_index_tables(&dim);
}

// part of @src 1401eb870: 1401ee5a0 with a local
inline void set_dttype_mode(Cls_1401e7b10* s, std::int32_t v) { model::sim_ctrl_set_u240(*s, &v); }

// part of @src 1401eb870: 1401309b0 with a local flag byte = 0
inline void read_plain(DYNARY* f, const std::int32_t* iifnc, const double* llfnc, std::int32_t fid)
{
    const char flag = 0;
    fun_1401309b0(f, iifnc, llfnc, fid, &flag);
}

// part of @src 1401eb870: DTTYPE type code -> xvar (S+0x1f8) and mode (S+0x240)
void dttype_from_code(Cls_1401e7b10* s, std::int32_t code)
{
    if (code == 1) {
        s->dttype.var[0] = 1;
        set_dttype_mode(s, 0);
    } else if (code == 2) {
        s->dttype.var[0] = 2;
        set_dttype_mode(s, 0);
    } else if (code == 3) {
        s->dttype.var[0] = 1;
        set_dttype_mode(s, 1);
    } else if (code == 4) {
        s->dttype.var[0] = 2;
        set_dttype_mode(s, 1);
    } else {
        s->dttype.var[0] = 1;
        set_dttype_mode(s, 0);
    }
}

// part of @src 1401eb870: USRDEF 1..10 from IIUVL, 80 characters each: copy, int-word byte order
// (140324020(.., 10, 1)), trim (1403239f0), then a name of blanks only becomes ""
void read_usrdef(Cls_1401e7b10* s, const char* iiuvl)
{
    for (std::int32_t i = 0; i < 10; i = i + 1) {
        char* u = s->usrdef[i];
        std::strncpy(u, iiuvl + i * 0x50, 0x50);
        fun_140324020(u, s->owner->u748, 10, 1);
        def::rt::trim_fortran(u, 0x50);
        const std::size_t len = std::strlen(u);
        std::int32_t blanks = 0;
        const char* first = nullptr;
        for (std::int32_t k = 0; k < static_cast<std::int32_t>(len); k = k + 1) {
            if (u[k] != ' ') {
                first = u + k;
                break;
            }
            blanks = blanks + 1;
        }
        if (first == nullptr && 0 < blanks) {
            u[0] = 0;
        }
    }
}

// part of @src 1401eb870: ACVCOF type 0..3 (the same in both branches)
void read_acvcof(Cls_1401e7b10* s, double type, double cf, const std::int32_t* iifnc, const double* llfnc)
{
    const std::int32_t t = static_cast<std::int32_t>(type);
    if (t == 0) {
        s->acvcof.ndim = 0;
        s->acvcof.value = cf;
    } else if (static_cast<std::int32_t>(type) == 1) {
        s->acvcof.ndim = 1;
        s->acvcof.var[0] = 3;
        read_plain(&s->acvcof, iifnc, llfnc, static_cast<std::int32_t>(cf));
    } else if (static_cast<std::int32_t>(type) == 2) {
        s->acvcof.ndim = 1;
        s->acvcof.var[0] = 0xc;
        read_plain(&s->acvcof, iifnc, llfnc, static_cast<std::int32_t>(cf));
    } else if (static_cast<std::int32_t>(type) == 3) {
        s->acvcof.ndim = 1;
        s->acvcof.var[0] = 1;
        read_plain(&s->acvcof, iifnc, llfnc, static_cast<std::int32_t>(cf));
    }
}

// part of @src 1401cf930 / 1401cfc30: fill the names that are still empty from `count` blocks of
// `w` 64-character names starting at `base`
void read_name_blocks(DB_STEP_DATA* m, const char* base, std::int32_t w, char** names)
{
    for (std::int32_t i = 0; i < m->u1378; i = i + 1) {
        const char* blk = base + static_cast<std::int32_t>(i * w * 0x40);
        for (std::int32_t j = 0; j < w; j = j + 1) {
            if (std::strlen(names[j]) == 0) {
                // char[64] stack buffer; strncpy fills 63 bytes. The 64th byte is whatever was on
                // the stack in the original: the 8-word swap of 140324020 can move it into byte
                // 60, which the trim then examines. The port starts from zero.
                char local[0x40] = {};
                std::strncpy(local, blk + j * 0x40, 0x3f);
                fun_140324020(local, m->u748, 8, 0);
                def::rt::trim_fortran(local, 0x3f);
                if (std::strlen(local) != 0) {
                    std::strncpy(names[j], local, 0x40);
                }
            }
        }
    }
}

} // namespace

// @src 1401cf4a0  _DB_STEP_DATA::ReadSimCtrl
void read_sim_ctrl(DB_STEP_DATA* m)
{
    select_tables(m);
    m->u2220 = static_cast<std::int32_t*>(m->u1b88) + (mar_start(m, 0x1406e8204) - 1); // IIFNC
    m->u2228 = static_cast<double*>(m->u1b78) + (a_start(m, 0x1406e82d0) - 1);        // LLFNC
    m->u2230 = static_cast<double*>(m->u1b78) + (a_start(m, 0x1406e82f4) - 1);        // LLWIN
    m->u2238 = static_cast<double*>(m->u1b78) + (a_start(m, 0x1406e82f8) - 1);        // LLWRZ
    const std::int32_t llsim = a_start(m, 0x1406e82dc);                                 // LLSIM
    double* a = static_cast<double*>(m->u1b78);
    std::int32_t* iisim = nullptr;
    char* iiuvl = nullptr;
    if (!gendb::step_fun_1401d2bb0(m)) {
        // 1401cf632..1401cf654: 1401cf040(m, &IISIM) / (m, &IIUVL) re-read one MAR region; dead
        // code (1401d2bb0 always returns 1)
        DEF_NOT_PORTED(0x1401cf040);
    } else {
        iisim = static_cast<std::int32_t*>(m->u1b88) + (mar_start(m, 0x1406e8214) - 1); // IISIM
        iiuvl = reinterpret_cast<char*>(static_cast<std::int32_t*>(m->u1b88) + (mar_start(m, 0x1406e8228) - 1));
    }
    if (!def::rt::dimension_is_2(m)) {
        m->u1378 = iisim[0x17];       // +0x5c objects
        m->n_materials = iisim[0x16]; // +0x58
        m->u13f0 = iisim[0x18];       // +0x60 boundaries
        m->n_phase_on = iisim[0x21];  // +0x84 relations
        m->u13e8 = iisim[0x13];       // +0x4c NUMNP
        m->u13ec = iisim[0x14];       // +0x50 NUMEL
        m->u13fc = iisim[0x1a];       // +0x68 NFN
    } else {
        m->u1378 = iisim[0x1e];       // +0x78
        m->n_materials = iisim[0x1d]; // +0x74
        m->u13f0 = iisim[0x1f];       // +0x7c
        m->n_phase_on = iisim[0x25];  // +0x94
        m->u13e8 = iisim[0x1a];       // +0x68
        m->u13ec = iisim[0x1b];       // +0x6c
        m->u13fc = iisim[0x21];       // +0x84
    }
    m->u13f8 = iisim[0x27]; // +0x9c windows, both dimensions (the 3D writer puts them in I[34])
    sim_ctrl_read_from_db(&m->sim_ctrl, iisim, a + (llsim - 1), iiuvl, static_cast<const std::int32_t*>(m->u2220),
                          static_cast<const double*>(m->u2228));
    if (!model::names_up_to_date(m)) {
        m->u3398 = m->u1dc0[A(0x1406e8414)]; // width LLND1
        m->u33a8 = m->u1dc0[A(0x1406e844c)]; // width LLET1
    } else {
        read_node_names(m);
        read_element_names(m);
    }
    if (!gendb::step_fun_1401d2bb0(m)) {
        // dead code, see above (the MAR copies would be freed here)
        std::fprintf(stdout, "NoObjects = %d\n", m->u1378);       // str 14052a830
        std::fprintf(stdout, "NoMaterials = %d\n", m->n_materials); // str 14052a628
        std::fprintf(stdout, "NoWindows = %d\n", m->u13f8);       // str 14052a840
        std::fprintf(stdout, "NoRelations = %d\n", m->n_phase_on); // str 14052a850
        if (iisim != nullptr) std::free(iisim);
        if (iiuvl != nullptr) std::free(iiuvl);
    }
}

// @src 1401eb870  sim-control ReadInfoFromDBArray
void sim_ctrl_read_from_db(Cls_1401e7b10* s, const std::int32_t* iisim, const double* llsim, const char* iiuvl,
                           const std::int32_t* iifnc, const double* llfnc)
{
    const std::int32_t* I = iisim;
    const double* L = llsim;
    if (!def::rt::dimension_is_2(s->owner)) {
        // ---- 3D ----
        s->meshno = I[1];
        s->smode = I[0];
        s->stype = I[0x31];
        if (s->stype < 1) {
            s->stype = 1;
        }
        s->unit = I[0x12];
        s->trans[0] = I[0x2c];
        s->trans[1] = I[0x2d];
        s->trans[2] = I[0x2e];
        s->trans[3] = I[0x2f];
        s->trans[4] = I[0x30];
        s->htmthd = I[0x2b];
        for (std::int32_t k = 0; k < 7; ++k) {
            s->stnout[k] = I[0x32 + k];
        }
        for (std::int32_t k = 0; k < 10; ++k) {
            s->elmnod[k] = I[0x39 + k];
        }
        s->nstart = I[2];
        s->stpinc = I[5];
        s->pdie1 = I[7];
        s->nstep = I[4];
        s->tmax = L[9];
        s->emax = L[10];
        s->smax[0] = L[0xb];
        s->smax[1] = L[0xc];
        s->smax[2] = L[0xd];
        s->vmin[0] = L[0xe];
        s->vmin[1] = L[0xf];
        s->vmin[2] = L[0x10];
        s->lmax[0] = L[0x11];
        s->lmax[1] = L[0x12];
        s->lmax[2] = L[0x13];
        s->stpdef = I[6];
        s->dtmax1 = L[3];
        s->dtmax2 = L[0x37];
        model::linked::set_dsmax(s, L[6]); // 1401f4390
        if (I[0x43] == 0) {
            s->dttype.ndim = 0;
            s->dttype.value = 0.0; // 8-byte zero store
        } else {
            s->dttype.ndim = 1;
            dttype_from_code(s, I[0x43]);
            read_plain(&s->dttype, iifnc, llfnc, I[0x44]);
        }
        model::sim_ctrl_update_u240(*s); // 1401ee510
        s->demax = L[5];
        s->slderr = L[4];
        s->dvmax[0] = L[7];
        s->dvmax[1] = L[8];
        s->mdsobj[0] = static_cast<std::int32_t>(L[0x22]);
        s->mdsobj[1] = static_cast<std::int32_t>(L[0x23]);
        s->mdsobj[2] = static_cast<std::int32_t>(L[0x24]);
        s->mdsobj4 = L[0x25];
        s->dtpmax1 = L[0x26];
        s->dtpmax2_5[0] = L[0x27];
        s->dtpmax2_5[1] = L[0x28];
        s->dtsub = L[0x21];
        s->dplen = L[0x34];
        s->odmax = L[0x36];
        s->cvgerr[0] = L[0x14];
        s->cvgerr[1] = L[0x15];
        s->itrmth = I[9];
        s->solmtd = I[0x20];
        if (s->solmtd == 2) {
            s->u2e8 = 4;
            s->solmtd_p[0] = static_cast<double>(I[0x48]);
            s->solmtd_p[1] = static_cast<double>(I[0x49]);
            s->solmtd_p[2] = static_cast<double>(I[0x4a]);
            s->solmtd_p[3] = static_cast<double>(I[0x4b]);
        } else if (s->solmtd == 6) {
            s->u2e8 = 4;
            s->solmtd_p[0] = L[0x3d];
            s->solmtd_p[1] = L[0x3e];
            s->solmtd_p[2] = L[0x3f];
            s->solmtd_p[3] = L[0x40];
        }
        s->frqncy_rows = I[0x4c];
        s->frqncy_cols = I[0x4d];
        if (s->frqncy_rows < 1 || s->frqncy_cols < 1) {
            model::sim_ctrl_init_u328(*s); // 1401e9850
        } else {
            s->frqncy.ncomp = s->frqncy_rows * s->frqncy_cols;
            read_plain(&s->frqncy, iifnc, llfnc, I[0x4e]);
        }
        s->vfactr_mode = I[0x50];
        s->vfactr_n = I[0x51];
        s->vfactr.ncomp = I[0x51];
        if (s->vfactr_mode == 1 && 0 < s->vfactr_n) {
            read_plain(&s->vfactr, iifnc, llfnc, I[0x52]);
        }
        s->itrmxd = I[10];
        s->defbwd = I[0xb];
        s->solmtt = I[0xf];
        s->itrmxt = I[0x10];
        s->tmpbwd = I[0x11];
        s->latent = I[0xd];
        s->peninf = L[0x16];
        s->unte2h = L[0x1b];
        s->tintgf = L[0x1a];
        s->blzman = L[0x20];
        if (static_cast<std::int32_t>(L[0x39]) == 0) {
            s->furnace_on = 0;
            if (static_cast<std::int32_t>(L[0x1c]) == 0) {
                s->envtmp.ndim = 0;
                s->envtmp.value = L[0x1d];
            } else {
                s->envtmp.ndim = 1;
                s->envtmp.var[0] = 1;
                read_plain(&s->envtmp, iifnc, llfnc, static_cast<std::int32_t>(L[0x1c]));
            }
        } else if (static_cast<std::int32_t>(L[0x39]) == 10) {
            s->furnace_on = 1;
            furnace_read_from_db(&s->furnace, s->owner, iifnc, llfnc, static_cast<std::int32_t>(L[0x3a]));
        }
        if (static_cast<std::int32_t>(L[0x1e]) == 0) {
            s->cnvcof.ndim = 0;
            s->cnvcof.value = L[0x1f];
        } else {
            s->cnvcof.ndim = 1;
            s->cnvcof.var[0] = 3;
            read_plain(&s->cnvcof, iifnc, llfnc, static_cast<std::int32_t>(L[0x1e]));
        }
        if (static_cast<std::int32_t>(L[0x29]) == 0) {
            s->envatm.ndim = 0;
            s->envatm.value = L[0x2a];
        } else {
            s->envatm.ndim = 1;
            s->envatm.var[0] = 1;
            read_plain(&s->envatm, iifnc, llfnc, static_cast<std::int32_t>(L[0x2a]));
        }
        read_acvcof(s, L[0x2b], L[0x2c], iifnc, llfnc);
        s->envmpr = L[0x30];
        s->envmpt = L[0x31];
        s->ehrate = L[0x35];
        s->tnow1 = L[0];
        s->tnow2 = L[0x32];
        s->tnow3 = static_cast<std::int32_t>(L[0x33]);
        s->tnow4 = L[0x38];
        s->geoerr[0] = L[0x17];
        s->geoerr[1] = L[0x18];
        s->geoerr[2] = L[0x3b];
        s->geoerr[3] = L[0x3c];
        if (0 < I[0x47]) {
            dynary_read_selfdesc(&s->timeid, iifnc, llfnc, I[0x47]);
        }
        if (0 < I[0x53]) {
            dynary_read_selfdesc(&s->erecid, iifnc, llfnc, I[0x53]);
        }
        if (0 < I[0x54]) {
            dynary_read_selfdesc(&s->nrecid, iifnc, llfnc, I[0x54]);
        }
        read_usrdef(s, iiuvl);
        s->pdie2 = I[0x23];
    } else {
        // ---- 2D ----
        s->smode = I[0];
        s->stype = I[1];
        s->geotyp = I[2];
        s->meshno = I[3];
        s->htmthd = I[0x2b];
        s->trans[0] = I[0x2c];
        s->trans[1] = I[0x2d];
        s->trans[2] = I[0x2e];
        s->trans[3] = I[0x2f];
        s->trans[4] = I[0x32];
        for (std::int32_t k = 0; k < 7; ++k) {
            s->stnout[k] = I[0x34 + k];
        }
        for (std::int32_t k = 0; k < 10; ++k) {
            s->elmnod[k] = I[0x3b + k];
        }
        s->nstart = I[4];
        s->nstep = I[6];
        s->stpinc = I[7];
        s->stpdef = I[8];
        s->pdie1 = I[9];
        s->emax = L[0x14];
        s->tmax = L[0x15];
        s->dtmax1 = L[2];
        s->dtmax2 = L[9];
        model::linked::set_dsmax(s, L[5]); // 1401f4390
        if (I[0x45] == 0) {
            s->dttype.ndim = 0;
            s->dttype.value = 0.0;
        } else {
            s->dttype.ndim = 1;
            dttype_from_code(s, I[0x45]);
            read_plain(&s->dttype, iifnc, llfnc, I[0x46]);
        }
        model::sim_ctrl_update_u240(*s); // 1401ee510
        s->demax = L[4];
        s->slderr = L[3];
        s->dtpmax1 = L[6];
        s->dvmax[0] = L[0x18];
        s->dvmax[1] = L[0x19];
        s->mdsobj[0] = static_cast<std::int32_t>(L[0x1c]);
        s->mdsobj[1] = static_cast<std::int32_t>(L[0x1d]);
        s->mdsobj[2] = static_cast<std::int32_t>(L[0x1e]);
        s->mdsobj4 = L[0x1f];
        s->dtpmax2_5[0] = L[0x1a];
        s->dtpmax2_5[1] = L[0x1b];
        s->dtpmax2_5[2] = L[0x2f];
        s->dtpmax2_5[3] = L[0x30];
        s->dtsub = L[0x31];
        s->cvgerr[0] = L[7];
        s->cvgerr[1] = L[8];
        s->solmtd = I[0xf];
        if (s->solmtd == 2) {
            s->u2e8 = 4;
            s->solmtd_p[0] = static_cast<double>(I[0x48]);
            s->solmtd_p[1] = static_cast<double>(I[0x49]);
            s->solmtd_p[2] = static_cast<double>(I[0x4a]);
            s->solmtd_p[3] = static_cast<double>(I[0x4b]);
        } else if (s->solmtd == 6) {
            s->u2e8 = 4;
            s->solmtd_p[0] = L[0x3d];
            s->solmtd_p[1] = L[0x3e];
            s->solmtd_p[2] = L[0x3f];
            s->solmtd_p[3] = L[0x40];
        }
        s->frqncy_rows = I[0x4c];
        s->frqncy_cols = I[0x4d];
        if (s->frqncy_rows < 1 || s->frqncy_cols < 1) {
            model::sim_ctrl_init_u328(*s); // 1401e9850
        } else {
            s->frqncy.ncomp = s->frqncy_rows * s->frqncy_cols;
            read_plain(&s->frqncy, iifnc, llfnc, I[0x4e]);
        }
        s->itrmth = I[0x10];
        if (s->itrmth == 3) {
            s->itrmth = 1;
        }
        s->itrmxd = I[0x11];
        s->defbwd = I[0x13];
        s->solmtt = I[0x16];
        s->itrmxt = I[0x17];
        s->tmpbwd = I[0x18];
        s->unit = I[0x19];
        s->peninf = L[10];
        s->tintgf = L[0xf];
        s->unte2h = L[0x10];
        s->blzman = L[0x16];
        if (static_cast<std::int32_t>(L[0x17]) == 0) {
            s->envtmp.ndim = 0;
            s->envtmp.value = L[0x11];
        } else {
            s->envtmp.ndim = 1;
            s->envtmp.var[0] = 1;
            read_plain(&s->envtmp, iifnc, llfnc, static_cast<std::int32_t>(L[0x17]));
        }
        if (static_cast<std::int32_t>(L[0x12]) == 0) {
            s->cnvcof.ndim = 0;
            s->cnvcof.value = L[0x13];
        } else {
            s->cnvcof.ndim = 1;
            s->cnvcof.var[0] = 3;
            read_plain(&s->cnvcof, iifnc, llfnc, static_cast<std::int32_t>(L[0x12]));
        }
        if (static_cast<std::int32_t>(L[0x23]) == 0) {
            s->envatm.ndim = 0;
            s->envatm.value = L[0x24];
        } else {
            s->envatm.ndim = 1;
            s->envatm.var[0] = 1;
            read_plain(&s->envatm, iifnc, llfnc, static_cast<std::int32_t>(L[0x24]));
        }
        read_acvcof(s, L[0x25], L[0x26], iifnc, llfnc);
        s->envmpr = L[0x28];
        s->envmpt = L[0x29];
        s->ehrate = L[0x2e];
        s->tnow1 = L[0];
        s->tnow2 = L[0x2a];
        s->tnow3 = static_cast<std::int32_t>(L[0x2b]);
        s->tnow4 = L[0x27];
        s->geoerr[0] = L[0xc];
        s->geoerr[1] = L[0xd];
        s->geoerr[2] = L[0x32];
        s->geoerr[3] = L[0x33];
        s->u680 = L[0xe];
        read_usrdef(s, iiuvl);
        s->latent = I[0x28];
        s->osctrl[0] = I[0x29];
        s->osctrl[1] = I[0x2a];
        s->osctrl[2] = I[0x30];
        s->osctrl[3] = I[0x31];
        s->cnterr1 = static_cast<std::int32_t>(L[0x2c]);
        s->cnterr2 = L[0x2d];
        s->pdie2 = I[0x24];
        s->vfactr_mode = I[0x33];
        if (0 < I[0x47]) {
            dynary_read_selfdesc(&s->timeid, iifnc, llfnc, I[0x47]);
        }
        if (0 < I[0x53]) {
            dynary_read_selfdesc(&s->erecid, iifnc, llfnc, I[0x53]);
        }
        if (0 < I[0x54]) {
            dynary_read_selfdesc(&s->nrecid, iifnc, llfnc, I[0x54]);
        }
    }
    if (s->stnout[0] == 0 && s->stnout[1] == 0 && s->stnout[2] == 0 && s->stnout[3] == 0 && s->stnout[4] == 0
        && s->stnout[5] == 0 && s->stnout[6] == 0) {
        s->stnout[4] = 1;
    }
}

// @src 1401cf930  _DB_STEP_DATA: UNNAME node names
void read_node_names(DB_STEP_DATA* m)
{
    select_tables(m);
    m->u3398 = m->u1dc0[A(0x1406e8414)]; // width LLND1
    const char* base = nullptr;
    if (!def::rt::dimension_is_2(m)) {
        std::int32_t off = 0;
        if (!model::names_up_to_date(m)) {
            off = 0;
            const char elem = 1;
            if ((read_single_a_array(m, A_ptr(0x1406e8328), &elem) & 0xff) == 0) { // LLND1
                return;
            }
        } else {
            off = a_start(m, 0x1406e8328) - 1;
        }
        base = reinterpret_cast<const char*>(static_cast<double*>(m->u1b78) + (off + m->u13e8 * m->u3398));
    } else {
        // +0x1ea4: A table slot 57 (2D)
        base = reinterpret_cast<const char*>(static_cast<double*>(m->u1b78) + (m->u1dc0[0x39] - 1));
    }
    alloc_node_names(m, &m->u3398);
    read_name_blocks(m, base, m->u3398, m->u33a0);
}

// @src 1401cfc30  _DB_STEP_DATA: UENAME element names
void read_element_names(DB_STEP_DATA* m)
{
    select_tables(m);
    m->u33a8 = m->u1dc0[A(0x1406e844c)]; // width LLET1
    const char* base = nullptr;
    if (!def::rt::dimension_is_2(m)) {
        std::int32_t off = 0;
        if (!model::names_up_to_date(m)) {
            off = 0;
            const char elem = 0;
            if ((read_single_a_array(m, A_ptr(0x1406e8360), &elem) & 0xff) == 0) { // LLET1
                return;
            }
        } else {
            off = a_start(m, 0x1406e8360) - 1;
        }
        base = reinterpret_cast<const char*>(static_cast<double*>(m->u1b78) + (off + m->u13ec * m->u33a8));
    } else {
        // +0x1ea0: A table slot 56 (2D)
        base = reinterpret_cast<const char*>(static_cast<double*>(m->u1b78) + (m->u1dc0[0x38] - 1));
    }
    alloc_element_names(m, &m->u33a8);
    read_name_blocks(m, base, m->u33a8, m->u33b0);
}

// @src 1401cb9e0
void alloc_node_names(DB_STEP_DATA* m, const std::int32_t* n)
{
    if (*n != 0) {
        m->u33a0 = static_cast<char**>(std::calloc(static_cast<std::size_t>(static_cast<std::int64_t>(*n)), 8));
        for (std::int32_t i = 0; i < *n; i = i + 1) {
            m->u33a0[i] = static_cast<char*>(std::calloc(0x40, 1));
        }
    }
}

// @src 1401cb730
void alloc_element_names(DB_STEP_DATA* m, const std::int32_t* n)
{
    if (*n != 0) {
        m->u33b0 = static_cast<char**>(std::calloc(static_cast<std::size_t>(static_cast<std::int64_t>(*n)), 8));
        for (std::int32_t i = 0; i < *n; i = i + 1) {
            m->u33b0[i] = static_cast<char*>(std::calloc(0x40, 1));
        }
    }
}

// @src 1403759b0  furnace ReadFromDB: 20-word record [0] 20.0, [1..8] FRNNAM, [9] first and
// [10] last zone function
void furnace_read_from_db(model::Cls_1403753f0* f, DB_STEP_DATA* m, const std::int32_t* iifnc, const double* llfnc,
                          std::int32_t fid)
{
    const std::int32_t start = iifnc[fid * 2 - 2];
    if (0 < iifnc[fid << 1] - start) {
        const double* rec = llfnc + (start - 1);
        const std::int32_t len = gendb::ext::f140375e70(f); // 0x14
        if (rec[0] == static_cast<double>(len)) {         // UCOMISD: unordered is "not equal"
            f->clear_curves();                             // 140375520
            std::strncpy(f->name, reinterpret_cast<const char*>(rec + 1), 0x40);
            fun_140324020(f->name, m->u748, 8, 0);
            // 1403239f0(name, 0x40): a name of 64 non-blank characters gets its NUL at name[64],
            // the first byte of the curve list, as in the original
            def::rt::trim_fortran(f->name, 0x40);
            const double first = rec[9];
            std::int32_t nzones = (static_cast<std::int32_t>(rec[10]) - static_cast<std::int32_t>(first)) + 1;
            f->setup_curves(&nzones);                      // 140375600
            const std::int32_t n = f->curve_count();       // 140375950
            for (std::int32_t i = 1; i <= n; i = i + 1) {
                DYNARY* c = f->curves.at(i);               // 140136860
                if (c != nullptr) {
                    dynary_read_selfdesc(c, iifnc, llfnc, static_cast<std::int32_t>(first) - 1 + i);
                }
            }
        }
    }
}

// @src 140130dc0  ReadFuncFromDBArray, self-describing layout (TIMEID / ERECID / NRECID, furnace
// zones, DIFBND / GBENGY / NUCSIZ, FSTRES 14)
void dynary_read_selfdesc(DYNARY* f, const std::int32_t* iifnc, const double* llfnc, std::int32_t fid)
{
    model::dynary_clamp_shape(f); // 14012fc00
    const std::int32_t start = iifnc[fid * 2 - 2];
    if (0 < iifnc[fid << 1] - start) {
        const double* p = llfnc + (start - 1);
        if (p[0] == 10.0) { // DAT_14051bee8; UCOMISD, unordered is "not equal"
            f->ndim = static_cast<std::int32_t>(p[1]);
            if (f->ndim == 0) {
                f->ncomp = static_cast<std::int32_t>(p[2]);
                model::dynary_alloc(f); // 14012fad0
                f->value = p[3];
            } else if (f->ndim == 1) {
                f->var[0] = static_cast<std::int32_t>(p[2]);
                f->n[0] = static_cast<std::int32_t>(p[3]);
                if (f->n[0] == 0) {
                    f->value = p[4];
                } else {
                    f->ncomp = static_cast<std::int32_t>(p[4]);
                    fun_140131ea0(f, p + 10);
                }
            } else if (f->ndim == 2) {
                f->var[0] = static_cast<std::int32_t>(p[2]);
                f->var[1] = static_cast<std::int32_t>(p[3]);
                f->n[0] = static_cast<std::int32_t>(p[4]);
                f->n[1] = static_cast<std::int32_t>(p[5]);
                f->ncomp = static_cast<std::int32_t>(p[6]);
                dynary_read_data(f, p + 10);
            }
        }
    }
}

// @src 140131e30
void dynary_read_data(DYNARY* f, const double* data)
{
    if (data != nullptr && f->ndim != 0) {
        if (f->ndim == 1) {
            fun_140131ea0(f, data);
        } else if (f->ndim == 2) {
            dynary_read_axes_values(f, data);
        }
    }
}

// @src 140131fa0
void dynary_read_axes_values(DYNARY* f, const double* data)
{
    model::dynary_clamp_shape(f); // 14012fc00
    model::dynary_alloc(f);       // 14012fad0
    std::int32_t k = 0;
    for (std::int32_t d = 0; d < f->ndim; d = d + 1) {
        for (std::int32_t j = 0; j < f->n[d]; j = j + 1) {
            f->axis[d][j] = data[k + j];
        }
        k = k + f->n[d];
    }
    const std::int32_t cnt = model::dynary_value_count(f); // 14012ff20
    std::memcpy(f->values, data + k, static_cast<std::size_t>(static_cast<std::int64_t>(cnt) << 3));
}

} // namespace def::dbio::dbread
