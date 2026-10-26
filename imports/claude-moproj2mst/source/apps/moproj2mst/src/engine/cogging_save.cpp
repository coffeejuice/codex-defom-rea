// MDT L1 part B: the cogging geometry step of File > Save (docs/re/moproj2mst/mdt/L1/spec_L1B.md).
// Decompiles: deform-rea-work/gui_mo/s1/mdt/L1/B/ (b0a.c 0x14075c260 / 0x140762f30 / 0x140763e50,
// e50.asm, f30.asm, b6a.c, b7a.c, b10a.c, impl/), s1/mdt/geo_d0.c (0x140766200), geo_d1.c
// (0x140762ec0). One function per binary function, in the original's order; UI-only calls (viewer
// list growth and release) are dropped and named. Engine calls go to the DEF_PRE port (twins).
//
// Every box read goes through engine::viewerBox (packet V): the cached box of the object's last
// viewer sync, so a box read after an engine move and before viewerSync of that object is the
// pre-move box, as in the original (spec section 1).
#include "cogging_save.hpp"
#include "../ops/cogging.hpp"

#include <cmath>
#include <cstdint>
#include <cstring>
#include <limits>
#include <string>

#include <defcmd/objpos.hpp>
#include <defcmd/objpos_ifd.hpp>
#include <def_pre/menu56.hpp>
#include <defmodel/gui_twins.hpp>
#include <defmodel/gui_twins_delete.hpp>
#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/cls_1401f46e0.hpp>
#include <defmodel/layout/cls_1401fa020.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj_ctor/llobj_copy.hpp>
#include <defmodel/llobj_ctor/obj_defaults.hpp>
#include <defmodel/window/inter_object_ops.hpp>
#include <defmodel/window/llwindef_data.hpp>
#include <defmodel/window/window_list.hpp>

#include "engine_internal.hpp"
#include "gui_model_access.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/props.hpp"
#include "m2m/qt4path.hpp"
#include "ops/cogging.hpp"

namespace m2m::engine {

using def::model::Cls_1401e7b10;
using def::model::Cls_1401f46e0;
using def::model::Cls_1401fa020;
using data::CogModel;
using qt::L;

namespace {

[[noreturn]] void unsupported(const std::string& what)
{
    throw Error(M2M_E_UNSUPPORTED, "cogging save: " + what);
}

// ---- the viewer box (spec section 1) --------------------------------------------------------------

struct Box {
    float mn[3] = {0.0f, 0.0f, 0.0f};
    float mx[3] = {0.0f, 0.0f, 0.0f};
};

// 0x140c82d50(W, &n) + 0x1411f5040(view, out, 0): false = null view (n < 1)
bool viewBox(GuiModel& w, int n, Box& b)
{
    return viewerBox(w, n, b.mn, b.mx);
}

// 0x14121a1a0 / 0x14121a1f0 / 0x14121a240: max - min of the float values widened to double
double extent(const Box& b, int k)
{
    return static_cast<double>(b.mx[k]) - static_cast<double>(b.mn[k]);
}

// 0x14121a700(box, out, &sx, &sy, &sz): coordinate k from max when its selector is set, else min
void corner(const Box& b, int sx, int sy, int sz, double out[3])
{
    out[0] = sx != 0 ? b.mx[0] : b.mn[0];
    out[1] = sy != 0 ? b.mx[1] : b.mn[1];
    out[2] = sz != 0 ? b.mx[2] : b.mn[2];
}

// 0x14121a360: the centre the box holds (+0x60), computed by 0x141219af0 from min and max:
// 0x14121c260 stores the double sum as a float, 0x14121c340 divides it by 2.0 (0x142101520) and
// stores the float again
double centre(const Box& b, int k)
{
    const float sum = static_cast<float>(static_cast<double>(b.mn[k]) + static_cast<double>(b.mx[k]));
    const float c = static_cast<float>(static_cast<double>(sum) / 2.0);
    return static_cast<double>(c);
}

// ---- engine objects ---------------------------------------------------------------------------------

// @src 0x1400c35b0  object's owner (+0x3a98)
DB_STEP_DATA* ownerOf(LLOBJ_DATA* o)
{
    return o->owner;
}

// 0x14005f7e0  the operation's sim control (W ? E+0x750 : 0)
Cls_1401e7b10* opSimCtl(GuiModel* w)
{
    return w ? w->simCtl() : nullptr;
}

// 0x140890fb0 ObjTranslate: no DEF_PRE twin by mask, the same sequence as 14021d0b0 (near-zero
// test 1e-30, 2D / 3D, geometry when *flagB, mesh, parts with flagA); spec 7.1
// @src 0x140890fb0  translate (forwards to the DEF_PRE sequence 14021d0b0)
void objTranslate(LLOBJ_DATA* o, const double* d, const char* flagA, const char* flagB)
{
    def::cmd::objpos::translate(o, d, flagA, flagB);
}

// 0x1408911f0 ObjRotate: DEF_PRE 14021d2f0 except the die-geometry test 0x1408777e0 (owner +0x1c
// == 2 ? 0x140877830 : 0x140877880), which is 140203980's test (2D 1402039d0 / 3D 140203a20) at
// GUI addresses; spec 7.2, L0 spec_B section 5
// @src 0x1408911f0  rotate (forwards to the DEF_PRE sequence 14021d2f0)
void objRotate(LLOBJ_DATA* o, const double* org, const double* axis, double deg)
{
    def::cmd::objpos::rotate(o, org, axis, deg);
}

// @src 0x14085f6d0  movement defaults (+0x2160): DEF_PRE 14018b500 stores the same values in the
// same order, except that it then overrides the direction of a 2D object (+0x50 = 1.0, +0x58 = 0);
// the GUI keeps the loop's value there (-1.0 at index dim - 1, 0x1421d3c90)
void movementReset(std::byte* mv)
{
    def::model::llobj_ctor::movement_defaults(mv);                 // = 0x14085f6d0 for 3D
    const DB_STEP_DATA* owner = *at<DB_STEP_DATA*>(mv, 0xa80);
    if (owner->dimension == 2) {                                   // 0x140065560
        for (int i = 0; i < 3; ++i) *at<double>(mv, 0x50 + 8 * i) = (i == owner->dimension - 1) ? -1.0 : 0.0;
    }
}

// 0x1408731c0 ObjCentre = DEF_PRE 1401ff360 (own_centre). The centre is copied out only when the
// geometry exists; the caller reads it whatever the result. Its only use is the routers' direction
// (dA, dB) for manipulators 4..7, which are copies of 4: when 4 has neither geometry nor a mesh
// (a new operation's primitive-only object, MDT L4, oracle B 131) every router returns false before
// the direction counts (1401a3aa0: rigid without geometry, or no mesh and no geometry; the zero-dir
// test of 1401a0300 only chooses between two false results), so the uninitialised vector reaches
// nothing and 0 stands for it. With geometry or a mesh the read would matter, and it is refused.
void objCentre(LLOBJ_DATA* o, double c[3])
{
    const double nan = std::numeric_limits<double>::quiet_NaN();
    c[0] = nan;
    c[1] = nan;
    c[2] = nan;
    (void)def::cmd::objpos::own_centre(o, c);                      // result unused (0x140762f30)
    if (std::isnan(c[0]) || std::isnan(c[1]) || std::isnan(c[2])) {
        if (def::cmd::objpos::has_geometry(o) || objHasMesh(o))
            unsupported("object centre of manipulator 4 not computed (0x1408731c0): the original reads an "
                        "uninitialised vector");
        c[0] = c[1] = c[2] = 0.0;                                  // never read (see above)
    }
}

// 0x1408c0b50 Router = DEF_PRE 14019fa00 (verified); arguments in binary order
bool router(GuiModel& w, LLOBJ_DATA* mover, LLOBJ_DATA* target, double* dir, double* tol, double* dist,
            double* G, char flag, int* nc, int* list, char* tchg, int* one)
{
    return GuiModelAccess::withStep(w, [&](DB_STEP_DATA* nativeStep) -> bool {
        return def::cmd::objpos::fun_14019fa00(nativeStep, mover, target, dir, tol, dist, G, flag, nc, list,
                                             tchg, one);
    });
}

// 0x140835430 = DEF_PRE 1401613a0 with the flags p4..p8 = 0, p9 = 1 (every copy of this step)
void copyObject(LLOBJ_DATA* dst, LLOBJ_DATA* src, const std::int32_t* sizes)
{
    const bool p4 = false, p5 = false, p6 = false, p7 = false, p8 = false, p9 = true;
    def::model::llobj_copy_from(dst, src, sizes, &p4, &p5, &p6, &p7, &p8, &p9);
}

// 0x1408e6c40 = DEF_PRE 1401fad60 (pair with master m, slave s and flag). The owner (p8-01,
// defcmd/src/p8_01/commands.cpp) keeps it in an anonymous namespace; this is its loop until it is
// exported (shared change).
Cls_1401f46e0* pairFindFlag(Cls_1401fa020* l, std::int32_t master, std::int32_t slave, std::int32_t flag)
{
    Cls_1401f46e0* p = l->head;
    while (p != nullptr && (p->u168 != master || p->u16c != slave || p->u170 != flag)) p = p->next;
    return p;
}

// @src 0x140360d30  mirrored pair: b = a with master and slave swapped, direction 2 <-> 1
void mirrorPair(Cls_1401f46e0* a, Cls_1401f46e0* b)
{
    if (a == nullptr || b == nullptr) return;
    def::model::pair_copy(b, a);                                   // 0x1408e57e0 = 1401f9900
    b->u168 = a->u16c;
    b->u16c = a->u168;
    if (a->u170 == 2) b->u170 = 1;
    else if (a->u170 == 1) b->u170 = 2;
}

// @src 0x1407653a0  inter-object records of src copied to dst (CopyInterObject); M unused
bool copyInterObject(const CogModel&, LLOBJ_DATA* src, LLOBJ_DATA* dst)
{
    bool r = false;
    DB_STEP_DATA* e = ownerOf(src);
    Cls_1401f46e0* ps = pairFindFlag(&e->inter_obj, *at<std::int32_t>(src, 8), 1, 2);
    Cls_1401f46e0* pd = pairFindFlag(&e->inter_obj, *at<std::int32_t>(dst, 8), 1, 2);
    Cls_1401f46e0* pr = pairFindFlag(&e->inter_obj, 1, *at<std::int32_t>(dst, 8), 1);
    if (ps != nullptr && pd != nullptr) {
        def::model::pair_copy_keep_ids(pd, ps);                    // 0x1408e5d60 = 1401f9e80
        r = true;
        for (int i = 0; i < def::model::winlist_count(&pd->u188); ++i) {   // 0x140dbbb10
            def::model::LLWINDEF_DATA* wd = def::model::winlist_nth(&pd->u188, i + 1);   // 0x140a1ce30
            if (wd != nullptr && wd->orient_id == *at<std::int32_t>(src, 8)) wd->orient_id = *at<std::int32_t>(dst, 8);
        }
    }
    if (pr != nullptr) mirrorPair(pd, pr);
    return r;
}

// 0x140060100: owner loader.cpp (engine::refreshSlotItems)
void syncSlotFromObject(Project& p, Operation& op, int n, FileEffects& fx)
{
    refreshSlotItems(p, op, n, fx);
}


}  // namespace

// @src 0x14085f6d0  exported for MDT L6 (DiestressBM vt+0xd8 through 0x14084e770)
void movementDefaults(std::byte* mv)
{
    movementReset(mv);
}

// @src 0x140c7b630  PDIE re-pick: fewer than 2 objects (0x140c7c1e0) -> the first real object
// (0x140823e60 = 1401cabc0); 2, or PDIE's object missing or of kind 7 (0x140c7c360) -> the first
// rigid object (0x140823fe0 = 1401cad40) when > 0, else the first real object
void repickPdie(GuiModel& w)
{
    return GuiModelAccess::withStep(w, [&](DB_STEP_DATA* nativeStep) {
        const int n = nativeStep ? def::pre::step_count_objects(nativeStep) : 0;    // 0x140c7c1e0
        bool first = false;
        bool rigid = false;
        if (n < 2) first = true;
        else if (n == 2) rigid = true;
        else {
            LLOBJ_DATA* pd = w.object(w.pdie());                                   // 0x140c7c360
            const int kind = pd ? *at<std::int32_t>(pd, 0xc) : 7;
            if (kind == 7) rigid = true;
        }
        if (rigid && nativeStep) {
            const int id = def::pre::step_first_rigid(nativeStep);
            if (id < 1) first = true;
            else w.setPdie(id);
        }
        if (first && nativeStep) w.setPdie(def::model::first_real_object_number(*nativeStep));
    });
}

// @src 0x140c7ca50  delete object n
void deleteObject(GuiModel& w, int n)
{
    return GuiModelAccess::withStep(w, [&](DB_STEP_DATA* nativeStep) {
        if (!(0 < n)) return;
        if (!(0 < (nativeStep ? def::pre::step_count_objects(nativeStep) : 0))) return; // 0x140c7c1e0
        // 0x140c82c10: the viewer item of n is unbound and the item list follows the tail number (UI;
        // packet V keeps no release entry)
        if (nativeStep) {
            bool nullHit = false;
            def::model::step_delete_object(*nativeStep, n, &nullHit);               // 0x1408bdf10 = 14019cdc0
            if (nullHit)
                unsupported("object " + std::to_string(n) + " deleted from a model whose object count exceeds its "
                            "list (14019ce70 dereferences a missing object)");
        }
        if (w.pdie() == n) {                                                       // 0x140c7b5a0
            w.setPdie(0);                                                          // 0x140c7b5e0
            repickPdie(w);
        }
    });
}

// @src 0x140766200  model geometry from object n
void cogGeometryFromObject(Project& p, Operation& op, CogModel& m, int n)
{
    GuiModel* w = opModel(p, op);                                  // 0x1405f8d90, 0x14005f770
    if (!w) unsupported("operation without an engine model (0x14005f770 returns null; the original then "
                        "dereferences it in 0x140c7ae80)");
    LLOBJ_DATA* o = w->object(n);                                  // 0x140c7ae80
    if (!o) return;
    const int t = *at<std::int32_t>(o, 0x3aa4);                    // 0x1401b2620
    // 0x140198b20: +0x3ab0 + 8k for 0 <= k < 20, else 0.0
    auto par = [&](int k) { return (0 <= k && k < 0x14) ? *at<double>(o, 0x3ab0 + 8 * k) : 0.0; };
    if (n == 1) {
        data::CogBillet& B = m.cog.billet;                         // 0x14075dcb0, 0x14044b5d0
        if (t - 6 < 0) {
            Box b1, b2, b3, b4;
            if (viewBox(*w, n, b1)) {                              // 0x140c82d50
                B.geoTyp = -1;
                B.length = extent(b1, 0);                          // 0x1411f5040 + 0x14121a1a0
                viewBox(*w, n, b2);
                B.width = extent(b2, 1);                           // 0x14121a1f0
                viewBox(*w, n, b3);
                B.height = extent(b3, 2);                          // 0x14121a240
                viewBox(*w, n, b4);
                B.dia = extent(b4, 2);                             // 0x14121a240 (z extent again)
            }
        } else {
            B.geoTyp = t - 6;
            B.length = par(0);
            B.width = par(1);
            B.height = par(2);
            B.dia = par(3);
            B.inDia = par(4);
            B.radius = par(5);
        }
    } else if (n == 2) {
        data::CogDie& D = m.cog.die;                               // 0x14075dcf0
        D.length = par(0);
        D.width = par(1);
        D.height = par(2);
        D.radius = par(3);
        D.movType = *at<std::int32_t>(o, 0x21c8);
        if (D.movType == 1) D.spddie = *at<double>(o, 0x2250);
        else if (D.movType == 4) {
            D.cycles = *at<double>(o, 0x2480);
            D.totStroke = *at<double>(o, 0x2478);
        }
    } else if (n == 4) {
        data::CogManip& P = m.cog.manip;                           // 0x14075dcd0
        P.length = par(0);
        P.width = par(1);
        P.height = par(2);
        P.radius1 = par(3);
        P.radius2 = par(4);
        P.radius3 = par(5);
        P.height1 = par(6);
        P.width1 = par(7);
        P.f490 = par(8);                                           // P+0x490
    }
}

// @src 0x140763e50  billet centring, dies 2 / 3 (/ 8 / 9), sim control
void cogPlaceBilletAndDies(Project& p, Operation& op, CogModel& m, FileEffects& fx)
{
    GuiModel* w = opModel(p, op);                                  // 0x1405f8d90 / 0x14005f770
    if (!w) return;
    const std::int32_t* sizes = w->step->u15f8;                    // 0x140c7acd0 + 0x15f8
    double xaxis[3] = {1.0, 0.0, 0.0};                             // [0x140] (0x1421249e0)
    double org[3] = {0.0, 0.0, 0.0};                               // [0xe0]
    double d[3] = {0.0, 0.0, 0.0};                                 // [0x128]
    const int nDies = ops::cog::dieCount(m);                       // 0x140765b00
    bool same = true;                                              // [0x169]
    const bool four = true;                                        // [0x168]
    if ((m.useDifferentDies & 0xff) != 0) same = false;            // 0x14075dc10 (byte +0x14); both branches 0
    LLOBJ_DATA* o2 = w->object(2);
    LLOBJ_DATA* o3 = w->object(3);
    double zTop = 0.0;                                             // [0x1c0]
    Box vb1;
    const bool vb = viewBox(*w, 1, vb1);                           // 0x140c82d50(&1)
    LLOBJ_DATA* o1 = w->object(1);
    if (!o2 || !o3 || !o1) return;
    double xMax = 0.0, zMax = 0.0;                                 // [0x160] [0x78]: set only with a view
    if (vb) {
        double hi[3], lo[3];
        corner(vb1, 1, 1, 1, hi);
        zMax = hi[2];
        double yMax = hi[1];
        xMax = hi[0];
        corner(vb1, 0, 0, 0, lo);
        const double zMin = lo[2];
        const double yMin = lo[1];
        const double cy = (yMax + yMin) / 2.0;                     // [0x1c8] (0x142101520)
        const double cz = (zMax + zMin) / 2.0;                     // [0x1d8]
        if ((0.001 < std::fabs(cy) || 0.001 < std::fabs(cz)) && m.rotationalSymmetry == 0) {   // 0x1421b5050, 0x14075dca0
            double t[3] = {0.0, -cy, -cz};                         // XOR sign: exact negation
            const char a = 0, b = 1;
            objTranslate(o1, t, &a, &b);
            yMax = yMax - cy;
            zMax = zMax - cz;
        }
        zTop = zMax;
    } else {
        // n = 1 always has a view in the original; xMax and zMax would be read uninitialised below
        unsupported("billet view missing (0x140c82d50): the original reads uninitialised box values");
    }
    viewerSync(*w, o1, true);                                      // 0x140c80280: the centred billet

    // ---- upper die (object 2)
    double c2x = 0.0, z2min = 0.0;                                 // [0xd8] [0x1a8]
    Box v2;
    if (viewBox(*w, 2, v2)) {
        double lo[3];
        corner(v2, 0, 0, 0, lo);
        z2min = lo[2];
        c2x = centre(v2, 0);
    }
    double refZ = z2min;                                           // [0x158]
    const data::CogPass& ps = ops::cog::pass(m.cog, 1, 1);         // 0x1407692b0
    Cls_1401e7b10* S = opSimCtl(w);                                // 0x14005f7e0
    double tol = 1e-05;                                            // [0x118] 0x1422395f8
    double dist = 0.0;                                             // [0xb0]
    const char flag = 1;                                           // [0x19c]
    int nc = 0;                                                    // [0x198]
    int* list = nullptr;                                           // [0xc0]
    char tchg = 0;                                                 // [0xb8]
    double G[3] = {0.0, 0.0, 0.0};
    if (ps.diePosMethod != 3) {
        d[0] = xMax - c2x;
        d[1] = 0.0;
        d[2] = 0.0;
        const char a = 0, b = 1;
        objTranslate(o2, d, &a, &b);
    }
    Box v2b;
    viewBox(*w, 2, v2b);                                           // stale: VB2 not rebuilt after the move
    // REFPOS (+0x1ea0): mode 1, point (centre x, centre y, refZ); the mode-2 branch is dead
    *at<std::int32_t>(o2, 0x1ec8) = 1;
    *at<double>(o2, 0x1ed0) = centre(v2b, 0);
    *at<double>(o2, 0x1ed8) = centre(v2b, 1);
    *at<double>(o2, 0x1ee0) = refZ;
    double dir[3];
    ops::cog::dieDir(0, dir);                                      // 0x140766010: (0,0,-1)
    int one = 0;
    (void)router(*w, o2, o1, dir, &tol, &dist, G, flag, &nc, list, &tchg, &one);   // rc unused

    if (!same) {                                                   // lower die positioned on its own
        Box v3;
        viewBox(*w, 3, v3);                                        // no null check (n = 3 has a view)
        double hi[3];
        corner(v3, 0, 0, 1, hi);
        const double z3max = hi[2];                                // [0x2b0]
        const double c3x = centre(v3, 0);                          // [0xd8]
        d[0] = 0.0;
        if (ps.diePosMethod != 3) d[0] = xMax - c3x;
        d[1] = 0.0;
        d[2] = -(zTop + z3max);                                    // [0x138]
        const char a = 0, b = 1;
        objTranslate(o3, d, &a, &b);
        ops::cog::dieDir(1, dir);                                  // (0,0,1)
        int one3 = 0;
        int nc3 = 0;
        (void)router(*w, o3, o1, dir, &tol, &dist, G, flag, &nc3, nullptr, &tchg, &one3);
        refZ = z3max;
        if (0.001 < std::fabs(d[2])) refZ = d[2];
        Box v3b;
        viewBox(*w, 3, v3b);                                       // stale (pre-translation)
        *at<std::int32_t>(o3, 0x1ec8) = 1;
        *at<double>(o3, 0x1ed0) = centre(v3b, 0);
        *at<double>(o3, 0x1ed8) = centre(v3b, 1);
        *at<double>(o3, 0x1ee0) = refZ;
    }
    // MDSOBJ (S+0x288 / +0x28c / +0x290), S+0x298 = pass+0x18
    *at<std::int32_t>(S, 0x288) = 2;
    *at<std::int32_t>(S, 0x28c) = 3;
    *at<std::int32_t>(S, 0x290) = 3;
    *at<double>(S, 0x298) = ps.minDieDis;
    if (m.processType == 1) {                                      // 0x1413ee240
        *at<double>(S, 0x180) = 0.0;                               // SMAX
        *at<double>(S, 0x188) = 0.0;
        *at<double>(S, 0x190) = m.swagingStroke;                   // 0x14075dd30 (M+0x4f8)
    }
    viewerSync(*w, o2, true);
    if (o3) {                                                      // always true here
        if (same) {
            copyObject(o3, o2, sizes);                             // 0x140835430 = 1401613a0
            copyInterObject(m, o2, o3);                            // 0x1407653a0
            objRotate(o3, org, xaxis, 180.0);                      // 0x142147108
        }
        syncSlotFromObject(p, op, 3, fx);                              // 0x140060100
        viewerSync(*w, o3, true);
    }
    if (nDies == 4) {
        LLOBJ_DATA* o8 = w->object(8);
        if (o8) {
            if (four) {
                copyObject(o8, o2, sizes);
                copyInterObject(m, o2, o8);
                objRotate(o8, org, xaxis, 270.0);                  // 0x142245870
            }
            viewerSync(*w, o8, true);
        }
        LLOBJ_DATA* o9 = w->object(9);
        if (o9) {
            if (!o8)
                unsupported("object 9 copied from a missing object 8 with four dies (0x1407653a0 dereferences "
                            "the null source; the original crashes)");
            copyObject(o9, o8, sizes);
            copyInterObject(m, o8, o9);
            objRotate(o9, org, xaxis, 180.0);
            viewerSync(*w, o9, true);
        }
        syncSlotFromObject(p, op, 8, fx);
        syncSlotFromObject(p, op, 9, fx);
    } else {
        deleteObject(*w, 8);                                       // 0x140c7ca50
        deleteObject(*w, 9);
    }
    // ---- die strokes and directions
    double s = std::fabs(*at<double>(o3, 0x22e0));                 // [0xd0] 0x141470a78
    if (m.processType == 0 && *at<std::int32_t>(o2, 0x21c8) == 4) {
        const double h = ops::cog::pass(m.cog, 1, 1).minDieDis;    // pass+0x18
        s = *at<double>(o2, 0x2478) - (zMax - h / 2.0);            // [0x78]: DIVSD, SUBSD, SUBSD
    }
    for (int i = 0; i < nDies; ++i) {
        LLOBJ_DATA* o = w->object(ops::cog::dieId(i));             // 0x140765f30, no null check
        if (!o)
            unsupported("die object " + std::to_string(ops::cog::dieId(i)) +
                        " missing in the stroke loop (the original dereferences null)");
        double v[3];
        ops::cog::dieDir(i, v);
        for (int k = 0; k < 3; ++k) {
            *at<double>(o, 0x21b0 + 8 * k) = v[k];
            *at<double>(o, 0x22d0 + 8 * k) = s * v[k];
        }
        if (i != 0) {
            *at<std::int32_t>(o, 0x22c0) = 2;
            *at<double>(o, 0x22c8) = 1.0;
        }
    }
}

// @src 0x140762f30  manipulators 4 .. 7
void cogPlaceManipulators(Project& p, Operation& op, CogModel& m, FileEffects& fx)
{
    GuiModel* w = opModel(p, op);
    if (!w) return;
    const std::int32_t* sizes = w->step->u15f8;
    double xaxis[3] = {1.0, 0.0, 0.0};                             // [0xa8]
    double org[3] = {0.0, 0.0, 0.0};                               // [0x78]
    if (!ops::cog::useManip(m)) {                                  // 0x140765960
        deleteObject(*w, 4);
        deleteObject(*w, 5);
        deleteObject(*w, 6);
        deleteObject(*w, 7);
        return;
    }
    double L1 = 0.0, L4 = 0.0, x1max = 0.0;                        // [0x200] [0x1f8] [0x1a0]
    Box v1;
    if (viewBox(*w, 1, v1)) {                                      // the centred billet (rebuilt above)
        L1 = extent(v1, 0);
        double hi[3];
        corner(v1, 1, 1, 1, hi);
        x1max = hi[0];
    }
    LLOBJ_DATA* o4 = w->object(4);
    if (!o4) return;
    const data::CogManip& P = m.cog.manip;                         // 0x14075dcd0
    const int spring = P.useSpring;                                // +0x8b4
    const double sp = P.startPos;                                  // +0x458
    double x4max = 0.0;                                            // [0x198]
    Box v4;
    if (viewBox(*w, 4, v4)) {
        double hi[3];
        corner(v4, 1, 0, 0, hi);
        x4max = hi[0];                                             // (the centre read after it is unused)
    }
    double d[3] = {(x1max - x4max) - sp, 0.0, 0.0};                // SUBSD, SUBSD
    {
        const char a = 0, b = 1;
        objTranslate(o4, d, &a, &b);
    }
    double tol = 0.001;                                            // 0x1421b5050
    double dist = 0.0;
    const char flag = 1;
    int nc = 0;
    int* list = nullptr;
    char tchg = 0;
    double G[3] = {0.0, 0.0, 0.0};
    LLOBJ_DATA* o1 = w->object(1);
    LLOBJ_DATA* o5 = w->object(5);
    bool couple = false;                                           // [0x1b8]
    if (spring == 0) {
        movementReset(at<std::byte>(o4, 0x2160));                 // 0x14085f6d0
    } else {
        *at<std::int32_t>(o4, 0x21c8) = 5;                         // MOVCTL type 5 (spring)
        *at<double>(o4, 0x2518) = P.stiffness;                     // P+0x478
        *at<double>(o4, 0x2598) = P.preload;                       // P+0x480
        *at<double>(o4, 0x2590) = P.maxDisp;                       // P+0x488
        *at<double>(o4, 0x2588) = 0.0;
        *at<double>(o4, 0x21b0) = 1.0;
        *at<double>(o4, 0x21b8) = 0.0;
        *at<double>(o4, 0x21c0) = 0.0;
        // QFile::exists("MAN_COUPLE.DAT"), relative to the GUI process's current folder, which is the
        // project folder after File > Open (spec Q7, oracle B 2026-10-22); the coupled-manipulator
        // branch that reads it is not ported
        const String prjFile = qt::cleanPath(projectDir(p) + qt::kSep + L("MAN_COUPLE.DAT"));
        if (qt::fileExists(prjFile))
            unsupported("MAN_COUPLE.DAT in the project folder: the coupled manipulators are not ported");
    }
    double c[3];
    objCentre(o4, c);                                              // 0x1408731c0 (rc unused)
    double dA[3] = {0.0, -c[1], -c[2]};                            // [0x110] XOR sign
    double dB[3];
    for (int k = 0; k < 3; ++k) dB[k] = dA[k] * -1.0;              // 0x14093c1c0(dA, 0x1421d3c90, dB)
    int one = 0;
    if (router(*w, o4, o1, dA, &tol, &dist, G, flag, &nc, list, &tchg, &one))
        viewerSync(*w, o4, true);                                  // VB4 rebuilt only when the router succeeded
    if (o5) {
        copyObject(o5, o4, sizes);
        copyInterObject(m, o4, o5);
        objRotate(o5, org, xaxis, 180.0);
        if (couple) {
            *at<std::int32_t>(o5, 0x22c0) = 4;
            *at<double>(o5, 0x22c8) = 1.0;
        }
        int one5 = 0;
        (void)router(*w, o5, o1, dB, &tol, &dist, G, flag, &nc, list, &tchg, &one5);
        viewerSync(*w, o5, true);
    }
    Box v4b;
    if (viewBox(*w, 4, v4b)) L4 = extent(v4b, 0);                  // stale when the o4 router failed
    d[0] = -((L1 - L4) - 2.0 * sp);                                // MULSD 2.0*sp, SUBSD, SUBSD, XOR
    d[1] = 0.0;
    d[2] = 0.0;
    LLOBJ_DATA* o6 = w->object(6);
    if (o6) {
        copyObject(o6, o4, sizes);
        copyInterObject(m, o4, o6);
        {
            const char a = 0, b = 1;
            objTranslate(o6, d, &a, &b);
        }
        *at<double>(o6, 0x21b0) = -1.0;                            // 0x1421d3c90
        int one6 = 0;
        (void)router(*w, o6, o1, dA, &tol, &dist, G, flag, &nc, list, &tchg, &one6);
        viewerSync(*w, o6, true);
    }
    LLOBJ_DATA* o7 = w->object(7);
    if (o7) {
        copyObject(o7, o4, sizes);
        copyInterObject(m, o4, o7);
        {
            const char a = 0, b = 1;
            objTranslate(o7, d, &a, &b);
        }
        *at<double>(o7, 0x21b0) = -1.0;
        objRotate(o7, org, xaxis, 180.0);
        if (couple) {
            *at<std::int32_t>(o7, 0x22c0) = 6;
            *at<double>(o7, 0x22c8) = 1.0;
        }
        int one7 = 0;
        (void)router(*w, o7, o1, dB, &tol, &dist, G, flag, &nc, list, &tchg, &one7);
        viewerSync(*w, o7, true);
    }
    syncSlotFromObject(p, op, 5, fx);
    syncSlotFromObject(p, op, 6, fx);
    syncSlotFromObject(p, op, 7, fx);
}

// @src 0x140762ec0
void coggingGeometryStep(Project& p, Operation& op, CogModel& model, bool full, FileEffects& fx)
{
    if (full) {
        cogGeometryFromObject(p, op, model, 1);
        cogGeometryFromObject(p, op, model, 2);
        cogGeometryFromObject(p, op, model, 4);
    }
    cogPlaceBilletAndDies(p, op, model, fx);
    cogPlaceManipulators(p, op, model, fx);
}

// @src 0x1405f22e0  base vt+0xd8
void baseAfterSave(Project& p, Operation& op)
{
    // 0x1402cb7a0(APP) == 5 (always in DEF_GUI_MO) and the class's current operation (0x1405ef2b0)
    storeSimNames(p, op);                                          // 0x140054540 (packet S, save_current.cpp)
}

// @src 0x14075c260  SfOprCogging vt+0xd8
void coggingAfterSave(Project& p, Operation& op, FileEffects& fx)
{
    auto* props = dynamic_cast<CoggingProps*>(op.props.get());     // 0x1405ef320, 0x14146f8fc
    if (!props) return;
    if (!props->model)                                             // 0x1401cc6b0: *(props+0x40)
        unsupported("cogging properties without a model (0x140762ec0 dereferences it)");
    coggingGeometryStep(p, op, *props->model, true, fx);
    baseAfterSave(p, op);
}

// @src 0x14075ad80  SfOprCogging vt+0x198: default contact pairs of the system-lubricant rebuild
// 0x14005a9b0. One pair object (ctor 0x1400657a0) is filled and appended (0x1400ad3b0) again and
// again, so its +0x18 / +0x19 flags carry over from one push to the next, as in the original
// (asm 0x14075ad80..0x14075b20a; the decompile mis-assigns the spring branch).
std::vector<ContactPair> cogDefaultPairs(Project&, Operation& op)
{
    std::vector<ContactPair> out;
    auto* props = dynamic_cast<CoggingProps*>(op.props.get());     // 0x1405ef320, 0x14146f8fc
    const CogModel* M = props ? props->model.get() : nullptr;      // 0x1401cc6b0
    if (!M) return out;
    const data::CogData& cog = M->cog;                             // 0x14075dcb0
    // 0x140767bc0(cog, 1, 1, 1): with an invalid reheat or pass the original reads an uninitialised
    // pointer; the port's accessor returns null there
    const data::CogBite* b = ops::cog::bite(cog, 1, 1, 1);
    if (!b) unsupported("default contacts of a cogging operation without reheat 1 / pass 1 / bite 1 (0x140767bc0 "
                        "then reads an uninitialised pointer)");
    ContactPair c;                                                 // master 1, slave 1, 0, 0, 0, 1
    const int nd = ops::cog::dieCount(*M);                         // 0x140765b00
    for (int i = 0; i < nd; ++i) {
        c.slave = 1;
        c.master = ops::cog::dieId(i);                             // 0x140765f30: 2, 3, 8, 9
        c.frc = b->frcfac;                                         // bite+0x50
        c.ihtc = b->ihtcof;                                        // bite+0x58
        out.push_back(c);
    }
    const int spring = cog.manip.useSpring;                        // 0x14075dcd0: manip+0x8b4
    if (cog.useManip) {                                            // 0x140765960: cog+0xc8
        if (spring != 0) {
            for (int i = 0; i < 4; ++i) {
                c.slave = 1;
                c.master = ops::cog::manipId(i);                   // 0x140765fa0: 4, 5, 6, 7
                c.lub = 1;
                c.frc = 20.0;                                      // .rdata 0x1421ce520
                c.ihtc = b->ihtcof2;                               // bite+0x60
                c.f19 = 0;                                         // (bite+0x40 is loaded, unused)
                out.push_back(c);
            }
            for (int i = 1; i < 2; ++i) {
                c.master = ops::cog::manipId(0);
                c.slave = ops::cog::manipId(i);
                c.frc = 0.0;
                c.ihtc = 0.0;
                c.f19 = 0;
                out.push_back(c);
            }
            for (int i = 3; i < 4; ++i) {
                c.master = ops::cog::manipId(2);
                c.slave = ops::cog::manipId(i);
                c.frc = 0.0;
                c.ihtc = 0.0;
                c.f19 = 0;
                out.push_back(c);
            }
        } else {
            for (int i = 0; i < 4; ++i) {
                c.slave = 1;
                c.master = ops::cog::manipId(i);
                c.frc = b->frcfac;                                 // bite+0x50
                c.ihtc = b->ihtcof2;                               // bite+0x60
                c.f19 = 0;
                out.push_back(c);
            }
        }
    }
    if (M->useMandrel) {                                           // 0x14075dc00
        c.slave = 1;
        c.master = 10;
        c.frc = b->frcfac;
        c.ihtc = b->ihtcof;
        out.push_back(c);
    }
    return out;
}

}  // namespace m2m::engine
