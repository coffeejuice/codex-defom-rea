// SfOprCogging: class object and MST writer entry (vt+0x280) with its block loop (s1-03 4.1, 4.2, 4.9).
#include "cogging.hpp"
#include "m2m/data/common.hpp"
#include "m2m/datafiles.hpp"
#include "m2m/props_base.hpp"
#include "m2m/mst/loader_l1.hpp"
#include "m2m/mst/simcount.hpp"

namespace m2m::ops::cog {

using mst::endl;

static const char* const SEP =
    "*-------------------------------------------------------------------------*";   // 0x1425e3f90

// @src 0x140752190
void writeBlocks(Blk& k, int only)
{
    mst::Stream& out = k.w.ts;
    const CogModel& m = k.m;
    bool first = true;
    bool firstOp = isChainHead(*k.op);                   // 0x140050bc0
    int chkMethod = 0;
    double tr0 = 0.0, tr1 = 0.0, aht = 0.0;
    const bool adapt = adaptive(m, &chkMethod, &tr0, &tr1, &aht);
    bool bDecomp = false, bDwell = false, bBHT = false;
    const bool heat = m.heatTransfer != 0;               // 0x14075dc20
    double dwell = m.dwellTime;                          // 0x14075dc60
    const double decomp = m.decompressionTime;           // 0x14075dc80
    const int ttype = m.temperatureType;                 // 0x14142ff20
    if (heat && ttype != 0) {
        if (1e-6 < decomp) bDecomp = true;
        if (1e-6 < dwell) bDwell = true;
        if (m.remainingCycleTimeDwell == 0) {            // 0x14075dc40
            bBHT = true;
        } else {
            bDwell = true;
            dwell = 0.0;
        }
    }
    int state = 0;
    const int nR = numReheats(m);
    for (int r = 1; r <= nR; r++) {
        const int nP = numPasses(m, r);
        for (int p = 1; p <= nP; p++) {
            int nb = 0, nAuto = 0, chkSim = 0;
            bool manipFirst = true;
            if (1 < p) first = false;
            const CogPass& P = pass(m.cog, r, p);
            if (1e-6 < P.ht.tmax && ttype != 0 && (++*k.sim, only == 0 || *k.sim == only)) {
                writePHT(k, r, p, &first, &firstOp, state, &manipFirst);
                if (only != 0) break;
                state = 3;
            }
            bool autoB = false;
            if (1e-6 < P.dwellTime && ttype != 0) {
                ++*k.sim;
                if (only == 0 || *k.sim == only) {
                    writePDWL(k, r, p, &first, &firstOp, state, &manipFirst);
                    if (only != 0) break;
                }
                state = 2;
            }
            if (P.autoBite == 0) {
                nb = numBites(m, r, p);
            } else {
                nAuto = autoBites(m.cog, r, p);
                nb = nAuto * 2 - 1;
                chkSim = *k.sim + 1 + nb * (static_cast<int>(bBHT) + 1 + static_cast<int>(bDecomp) +
                                            static_cast<int>(bDwell));
                autoB = true;
            }
            for (int b = 1; b <= nb; b++) {
                if (1 < b) first = false;
                const CogBite* B = bite(m.cog, r, p, b);
                if (!B) continue;
                ++*k.sim;
                if (only == 0 || *k.sim == only) {
                    writeBDEF(k, r, p, b, &first, &firstOp, state, &manipFirst);
                    if (only != 0) break;
                }
                state = 1;
                if (heat) {
                    if (bDecomp && (++*k.sim, only == 0 || *k.sim == only)) {
                        writeBDCT(k, r, p, b, decomp);
                        if (only != 0) break;
                    }
                    if (bDwell) {
                        ++*k.sim;
                        if (only == 0 || *k.sim == only) {
                            writeBDW(k, r, p, b, dwell);
                            if (only != 0) break;
                        }
                        state = 2;
                    }
                    if (bBHT) {
                        ++*k.sim;
                        if (only == 0 || *k.sim == only) {
                            writeBHT(k, r, p, b, state);
                            if (only != 0) break;
                        }
                        state = 3;
                    }
                }
                if (only == 0 && autoB && 1 < b) {
                    // bite-level CHKSTOP (auto bites)
                    const int rs = m.rotationalSymmetry;            // 0x14075dca0 (byte)
                    out << SEP << endl;
                    out << "CHKSTOP" << endl;
                    out << static_cast<int>(adapt) + 1 << endl;
                    out << 1 << endl;
                    out << 0 << "\t" << chkSim << endl;
                    out << 2 << "\t" << 3 << "\t" << P.minDieDis << "\t" << rs << endl;
                    out << P.dieMovDir << "\t" << 0.0 << "\t" << 0.0 << endl;
                    out << 0.5 << endl;                             // 0x142101640
                    out << B->nomBite << endl;
                    out << P.diePosMethod << endl;
                    if (P.diePosMethod == 1)
                        out << P.dieStopNod << endl;
                    else
                        out << P.dieStopPos << endl;
                    if (adapt) {
                        out << 2 << endl;
                        out << chkSim + 1 << "\t" << chkSim << endl;
                        out << 0 << endl;
                        out << tr0 << endl;
                        out << chkMethod << endl;
                    }
                    out << SEP << endl;
                }
            }
            // A break in the bite loop ends only the bite loop (C semantics, as compiled); the
            // following blocks still count but can no longer match `only`.
            if (adapt && (++*k.sim, only == 0 || *k.sim == only)) {
                if (!autoB) {
                    // pass-level CHKSTOP (adaptive reheat without auto bites)
                    out << SEP << endl;
                    out << "CHKSTOP" << endl;
                    out << 1 << endl;
                    out << 2 << endl;
                    out << 0 << "\t" << *k.sim + 1 << endl;
                    out << 1 << endl;
                    out << tr0 << endl;
                    out << chkMethod << endl;
                    out << SEP << endl;
                }
                writeSHT(k, r, p, tr1, aht);
                if (only != 0) break;
            }
        }
    }
}

template <class F> F l1(F f)
{
    if (!f) mst::loaderSlotsMissing();
    return f;
}

class CoggingClass : public mst::OpClass {
public:
    CoggingClass() : OpClass(qt::L("SfOprCogging")) {}

    // vt+0x280  @src 0x1407520c0
    void writeMst(mst::W& w, Project& prj, const String& db, int* sim, Operation* op, int dbreadArg,
                  int onlySim) override
    {
        Props* bp = propsOf(op);                              // 0x1405ef320
        auto* cp = dynamic_cast<CoggingProps*>(bp);           // __RTDynamicCast 0x14146f8fc
        if (!cp) return;
        if (!cp->model)                                        // 0x1401cc6b0: the original always has one
            throw Error(M2M_E_INPUT, "cogging operation without process data");
        Operation* o = op ? op : current;
        Blk k{w, prj, db, sim, o, *cp->model, dbreadArg};
        writeBlocks(k, onlySim);
    }

    // ---- MDT L1: own loader slots (bodies in src/engine/cogging_load.cpp, loader_l1.hpp) -----------
    // vt+0x58, +0x1b8 and +0x1f8 (false) stay the base slots (0x1405ef430, 0x1405f6520, 0x1405f79a0).
    // vt+0xb8  @src 0x140750650
    void loadPrologue(Project& p, Operation& op) override { l1(mst::g_loaderSlotsL1.cogLoadPrologue)(*this, p, op); }
    // vt+0x78  @src 0x140750790: empty
    void loadClassData(Project&, Operation&) override {}
    // vt+0xc0  @src 0x1407507b0
    void postLoad(Project& p, Operation& op, Ctx& ctx, engine::FileEffects& fx) override
    {
        l1(mst::g_loaderSlotsL1.cogPostLoad)(*this, p, op, ctx, fx);
    }
    // SfOprCoggingProperties vt+0x58  @src 0x14075ee20
    void propsLoad(Project& p, Operation& op) override { l1(mst::g_loaderSlotsL1.cogPropsLoad)(*this, p, op); }
    // vt+0xd8 of save current operation  @src 0x14075c260 (body: src/engine/cogging_save.cpp)
    void saveNames(Project& p, Operation& op, engine::FileEffects& fx) override
    {
        l1(mst::g_loaderSlotsL1.cogSaveNames)(*this, p, op, fx);
    }

    // vt+0xa8  @src 0x140750f20: the tree's editors "Process" (SfCoggingProcess, shown after the open),
    // "Pass Table", "Manipulators", then vt+0x228 0x140751de0 (tree states, UI; vt+0x230 is the empty
    // base) and the tree widget (UI) (MDT L4)
    bool objectTreeFill(Project& p, Operation& op, engine::FileEffects& fx) override
    {
        objectTreeAdd(p, op, fx);
        processShown_ = &op;
        return false;
    }
    // vt+0xd0 with the Process editor shown: its vt+0x1c0 0x140789410 (MDT L4)
    bool updateDataInWidgetArea(Project& p, Operation& op, engine::FileEffects&) override
    {
        if (processShown_ != &op) return false;
        return l1(mst::g_loaderSlotsL1.cogProcessUpdate)(p, op);
    }

    // vt+0x298  @src 0x140759d50 (P14)
    void simList(int pos, int* counter, Operation* op, std::vector<mst::SimEntry>& out) override
    {
        mst::simListCogging(*this, pos, counter, op, out);
    }

    // vt+0x98: SfOprCoggingProperties ctor 0x14075df60: base props (0x1405f8bb0, P02's default
    // tree), then 0x14075e020 allocates the CogModel (ctor 0x140761310 -> defaults 0x1407613b0, P06).
    // The model's back pointer +0x548 to the props is not kept: the port passes the props along.
    // Unlike the other props ctors (e.g. 0x140142720), 0x14075df60 does not call the tree builder
    // (props+0x10)->vt+0x20 = 0x14075eb30 (its only reference is that vtable slot, 0x142678b50), so
    // the props have no properties: every Cogging operation of the corpus and of oracle B 105 / 115
    // is saved with <Properties Number="0"/> (MDT L4).
    std::shared_ptr<Props> makeProps(int guiUnit) const override
    {
        auto p = std::make_shared<CoggingProps>();
        p->model = std::make_shared<data::CogModel>();
        datafiles::cogModelDefaults(*p->model, guiUnit);
        data::initCommonBlock(*p->common, guiUnit);    // base ctor 0x1405f8ca0 -> 0x1405fd250
        return p;
    }

    // @src 0x14075eb30  (props+0x10)->vt+0x20, the tree builder: class root (0x140375ba0, P02), then
    // the base groups. Cogging adds no Friction / HeatTransCoef children (0x1405fad10 / 0x1405fa7d0).
    // Not reached by the conversion (see makeProps).
    static void buildTree(CoggingProps& p, const String& key, int guiUnit)
    {
        p.tree = defaultPropTree(key);
        base::addStepCounts(p, false, false, 100, 10, qt::L("0.0 sec"));      // 0x1405f9eb0, 0x142678228
        base::addEnvTemp(p, guiUnit, qt::L("68.00 F"), qt::L("20.00 C"));     // 0x1405fa560, 0x142678238/230
        base::addConvection(p, guiUnit, qt::L("0.0 Btu/sec/in^2/F"),          // 0x1405faaa0, 0x142678250
                            qt::L("0.0 N/sec/mm/C"));                          //              0x142678240
        base::addStepBy(p, guiUnit, 0, qt::L("0.0 in"), qt::L("0.0 mm"),       // 0x1405fb020, 0x142678278/270
                        qt::L("0.0 sec"));                                     //              0x142678268
    }

private:
    const Operation* processShown_ = nullptr;   // the op whose Process editor the GUI shows
};

}  // namespace m2m::ops::cog

namespace m2m::mst {
std::unique_ptr<OpClass> makeCogging()
{
    return std::make_unique<ops::cog::CoggingClass>();
}
}  // namespace m2m::mst
