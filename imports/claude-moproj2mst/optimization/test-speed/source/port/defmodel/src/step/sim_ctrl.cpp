// Simulation control block Cls_1401e7b10 at DB_STEP_DATA+0x750 (packet p1-03).
// Layout: defmodel/layout/cls_1401e7b10.hpp. Double constants are the binary's values
// (shortest round-trip decimal of each DAT_ address; read from DEF_PRE_64.exe .rdata).
#include <defmodel/step/db_step.hpp>

#include <cstring>

#include "ext.hpp"

namespace def::model {

namespace {

// 1400015e0 / 1400015f0 (p1-01, inlined): the owner's dimension is 2 / 3.
bool owner_is_2d(const Cls_1401e7b10& c) { return c.owner->dimension == 2; }
bool owner_is_3d(const Cls_1401e7b10& c) { return c.owner->dimension == 3; }

} // namespace

// @src 1401e7b10  ctor: C++ constructs the DYNARY members (14012d050) and the furnace block
// (1403753f0) in the binary's order (0x1e8, 0x330, 0x3c8, 0x440, 0x4c0, 0x538, 0x5b0, 0x9e0,
// 0xa58, 0xad0, 0xb48); then owner = 0. Nothing else is written: init runs from the owner.
Cls_1401e7b10::Cls_1401e7b10()
{
    owner = nullptr;
    // GUI1408d3cac/cb2 is an additional constructor store, before the first
    // sim_ctrl_set_owner/init. Preserve the DEF_PRE constructor separately.
    if (g_gui_engine_variant) unit = g_gui_engine_startup_unit;
}

// @src 1401e7cb0  dtor: clear, then C++ destroys the members in reverse (14012f410, 140375430)
Cls_1401e7b10::~Cls_1401e7b10() { sim_ctrl_clear(*this); }

// @src 1401e7c10  set the owning step, then init
void sim_ctrl_set_owner(Cls_1401e7b10& c, DB_STEP_DATA* owner)
{
    c.owner = owner;
    sim_ctrl_init(c);
}

// @src 1401e7c50  init all parts
void sim_ctrl_init(Cls_1401e7b10& c)
{
    sim_ctrl_init_names(c);
    sim_ctrl_init_u160(c);
    sim_ctrl_init_u1c8(c);
    sim_ctrl_init_u170(c);
    sim_ctrl_init_u2d0(c);
    sim_ctrl_init_u3a8(c);
    sim_ctrl_init_u640(c);
}

// @src 1401e7da0  free the tables' arrays
void sim_ctrl_clear(Cls_1401e7b10& c)
{
    ext::fun_14012f480(&c.envtmp);
    ext::fun_140375520(&c.furnace);
    ext::fun_14012f480(&c.cnvcof);
    ext::fun_14012f480(&c.envatm);
    ext::fun_14012f480(&c.acvcof);
    ext::fun_14012f480(&c.dttype);
    ext::fun_14012f480(&c.timeid);
    ext::fun_14012f480(&c.erecid);
    ext::fun_14012f480(&c.nrecid);
    ext::fun_14012f480(&c.vfactr);
    c.frqncy_rows = 0;
    c.frqncy_cols = 0;
    ext::fun_14012f480(&c.frqncy);
}

// @src 1401e7e90  names and the integer controls
void sim_ctrl_init_names(Cls_1401e7b10& c)
{
    std::strcpy(c.title, "SIMULATION");
    std::strcpy(c.oprnam, "OPERATION 1");
    std::strcpy(c.simnam, "SIMULATION 1");
    c.cursim[0] = 1;
    c.cursim[1] = 0;
    c.cursim[2] = 0;
    c.meshno = 1;
    c.smode = 1;
    c.stype = 1;
    c.geotyp = 1;
    for (std::int32_t i = 0; i < 6; ++i) { // 0x104..0x118: trans[0..4], htmthd
        field<std::int32_t>(&c, 0x104 + static_cast<std::size_t>(i) * 4) = 0;
    }
    c.trans[1] = 1; // 0x108
    c.stnout[0] = 0;
    c.stnout[1] = 0;
    c.stnout[2] = 0;
    c.stnout[3] = 0;
    c.stnout[4] = 1; // 0x12c
    c.stnout[5] = 0;
    c.stnout[6] = 0;
    for (std::int32_t i = 0; i < 10; ++i) {
        c.elmnod[i] = 0;
    }
}

// @src 1401e8490  step controls (NSTEP is written twice, 100 then 1)
void sim_ctrl_init_u160(Cls_1401e7b10& c)
{
    c.nstart = -1;
    c.nstep = 100;
    c.stpinc = 10;
    c.pdie1 = 0;
    c.nstep = 1;
}

// @src 1401e8650  time-step controls
void sim_ctrl_init_u1c8(Cls_1401e7b10& c)
{
    c.stpdef = 1;
    c.dsmax = 0.0; // 1401f4390(this, 0) (p1-06 setter of 0x1e0, inlined)
    c.dtmax1 = 0.0;
    c.dtmax2 = 0.0;
    c.slderr = 0.0;
    c.dtpmax1 = 0.0;
    c.dvmax[0] = 0.0;
    c.dvmax[1] = 0.0;
    if (!owner_is_2d(c)) {
        c.demax = 0.0;
    } else {
        c.demax = 0.1; // _DAT_14051bef8
    }
    c.dtpmax2_5[0] = 0.0;
    c.dtpmax2_5[1] = 0.0;
    c.dtpmax2_5[2] = 0.0;
    c.dtpmax2_5[3] = 0.0;
    c.dplen = 0.5; // DAT_14051f240
    if (!owner_is_3d(c)) {
        c.dtsub = 0.0;
    } else {
        c.dtsub = 1.0; // _DAT_140509c90
    }
    ext::fun_14012d2a0(&c.dttype, 0x80);
    sim_ctrl_update_u240(c);
}

// @src 1401e8fb0  limits
void sim_ctrl_init_u170(Cls_1401e7b10& c)
{
    c.tmax = 0.0;
    for (std::int32_t i = 0; i < 3; ++i) {
        c.smax[i] = 0.0;
        c.vmin[i] = 0.0;
        c.lmax[i] = 0.0;
    }
    c.mdsobj[0] = 0;
    c.mdsobj[1] = 0;
    c.mdsobj[2] = 0;
    c.mdsobj4 = 0.0;
    c.emax = 0.0;
    c.odmax = 0.0;
}

// @src 1401e96b0  iteration controls
void sim_ctrl_init_u2d0(Cls_1401e7b10& c)
{
    if (!owner_is_2d(c)) {
        c.cvgerr[0] = 0.005; // DAT_14052e870
        c.cvgerr[1] = 0.05;  // DAT_140524ce0
        c.solmtd = g_gui_engine_variant ? 2 : 1;   // DEF_GUI_MO_64 0x1408d5800: 2
        c.solmtt = 2;
    } else {
        c.cvgerr[0] = 0.001; // DAT_14051f1e8
        c.cvgerr[1] = 0.01;  // DAT_14051f258
        c.solmtd = 1;
        c.solmtt = 1;
    }
    c.u2e8 = 0;
    c.solmtd_p[0] = 0.0;
    c.solmtd_p[1] = 0.0;
    c.solmtd_p[2] = 0.0;
    c.solmtd_p[3] = 0.0;
    sim_ctrl_init_u328(c);
    if (!owner_is_2d(c)) {
        c.itrmth = 2;
    } else {
        c.itrmth = 1;
    }
    c.itrmxd = 200;
    c.defbwd = 1;
    c.itrmxt = 200;
    c.tmpbwd = 1;
    c.latent = 0;
}

// @src 1401e9850  FRQNCY: a 6x1 table of defaults
void sim_ctrl_init_u328(Cls_1401e7b10& c)
{
    c.frqncy_rows = 6;
    c.frqncy_cols = 1;
    ext::fun_14012d2a0(&c.frqncy, 0xb5);
    c.frqncy.ncomp = c.frqncy_rows * c.frqncy_cols; // 0x370
    ext::fun_14012fad0(&c.frqncy);
    double v[6];
    v[0] = 1.0;   // _DAT_140509c90
    v[1] = 0.0;
    v[2] = 100.0; // DAT_140524cd8
    v[3] = 20.0;  // DAT_14051f1f0
    v[4] = 1.0;
    v[5] = 1.0;
    std::memcpy(c.frqncy.values, v, static_cast<std::size_t>(static_cast<std::int64_t>(c.frqncy.ncomp) << 3));
}

// @src 1401ea6b0  environment constants, by unit system (unit == 2 selects the first set)
void sim_ctrl_init_u3a8(Cls_1401e7b10& c)
{
    c.peninf = 1000000000.0; // DAT_140522f50
    c.tintgf = 0.75;         // DAT_14052e8d0
    c.furnace_on = 0;
    ext::fun_14012d2a0(&c.envtmp, 0);
    const std::int32_t n = 0x16;
    ext::fun_140375600(&c.furnace, &n);
    ext::fun_140375e90(&c.furnace);
    ext::fun_14012d2a0(&c.cnvcof, 0);
    ext::fun_14012d2a0(&c.envatm, 0);
    ext::fun_14012d2a0(&c.acvcof, 0);
    if (c.unit == 2) {
        c.unte2h = 0.107;        // DAT_14052e8c8
        c.blzman = 3.306e-15;    // DAT_14052e8c0
        c.envtmp.value = 68.0;   // 0x3d0, DAT_14051f1f8
        c.cnvcof.value = 7.7e-06; // 0x4c8, DAT_14052e8b8
    } else {
        c.unte2h = 1.0;          // _DAT_140509c90
        c.blzman = 5.669e-11;    // DAT_14052e8b0
        c.envtmp.value = 20.0;   // DAT_14051f1f0
        c.cnvcof.value = 0.02;   // DAT_14052e8a8
    }
    if (c.unit == 2) {
        c.envatm.value = 1.69;   // 0x540, DAT_14052e8a0
        c.acvcof.value = 1e-05;  // 0x5b8, DAT_140522f98
        c.envmpr = 3.19e-08;     // DAT_14052e898
        c.envmpt = 2.25e-13;     // DAT_14052e890
    } else {
        c.envatm.value = 1.69;   // DAT_14052e8a0
        c.acvcof.value = 0.00025; // DAT_14052e888
        c.envmpr = 1.26e-09;     // DAT_14052e880
        c.envmpt = 8.8e-15;      // DAT_14052e878
    }
    c.ehrate = 0.0;
}

// @src 1401ead60  time, geometry tolerances, user strings, 2D/3D specific tables
void sim_ctrl_init_u640(Cls_1401e7b10& c)
{
    c.tnow1 = 0.0;
    c.tnow2 = 0.0;
    c.tnow3 = 0;
    c.tnow4 = 0.0;
    c.geoerr[0] = 1e-06;  // DAT_14051f1b8
    c.geoerr[1] = 0.0001; // DAT_14051f250
    c.geoerr[2] = 0.0;
    c.geoerr[3] = 0.0;
    if (owner_is_2d(c)) {
        c.u680 = 100.0; // DAT_140524cd8
    }
    for (std::int32_t i = 0; i < 10; ++i) {
        std::strcpy(c.usrdef[i], ""); // str 14052c7da
    }
    c.pdie2 = 0;
    if (!owner_is_2d(c)) {
        c.vfactr_mode = 0;
        c.vfactr_n = 0;
        ext::fun_14012d2a0(&c.vfactr, 0xb5);
    } else {
        c.osctrl[0] = 0;
        c.osctrl[1] = 0;
        c.osctrl[2] = 0;
        c.osctrl[3] = 0;
        c.cnterr1 = 0;
        c.cnterr2 = 0.0;
        c.vfactr_mode = 0;
    }
    ext::fun_14012d2a0(&c.timeid, 0xb0);
    ext::fun_14012d2a0(&c.erecid, 0xb1);
    ext::fun_14012d2a0(&c.nrecid, 0xb2);
}

// @src 1401ee510  DTTYPE mode from DSMAX when the DTTYPE table is a constant
void sim_ctrl_update_u240(Cls_1401e7b10& c)
{
    if (c.dttype.ndim == 0) { // 0x1ec
        if (c.dsmax <= 0.0) { // _DAT_14051bed8
            const std::int32_t one = 1;
            sim_ctrl_set_u240(c, &one);
        } else {
            const std::int32_t zero = 0;
            sim_ctrl_set_u240(c, &zero);
        }
    }
}

// @src 1401ee5a0  DTTYPE mode (DYNARY +0x58 of dttype, sim_ctrl + 0x240)
void sim_ctrl_set_u240(Cls_1401e7b10& c, const std::int32_t* v) { c.dttype.u58 = *v; }

} // namespace def::model
