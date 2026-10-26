// Simulation numbering (P14): the vt+0x298 sim lists, 0x1400b8360 and nextSim 0x1400b8820.
// s1-04 7.4, s1-05 5.6; decompiles s1-04/d11c.c, d12c.c, d13c.c, s1-03/hooks_d0c.c, s1-05/d10s.c.
//
// An entry (0x1400683f0) also carries the operation name, type, colour, a SIMNAM-like text and step
// data read from the open project DB (0x1408100e0); nextSim reads only +0x8 (position) and +0xc
// (simulation number), so SimEntry keeps those two.
#include <cstdio>
#include "m2m/mst/simcount.hpp"

#include "m2m/data/multiblow.hpp"
#include "../ops/p13.hpp"

namespace m2m::mst {

// @src 0x140068210
void setSimPos(SimEntry& e, int pos, const Operation* op)
{
    const Project* p = op ? op->project : nullptr;
    if (p && p->doeSimPosSet) e.pos = p->doeSimPos;
    else e.pos = pos;
}

namespace {
// Cycle naming context of the entries (globals 0x143d7b7a0 iteration, 0x143d7b7a4 cycle number,
// 0x143d7b7a8 on, 0x143d7b7b0 cycle name), set and cleared by the Cycle list 0x14134e280.
struct CycleNameCtx {
    bool on = false;
    int iteration = 0;
};
thread_local CycleNameCtx g_cycleName;
}  // namespace

// @src 0x1400682a0  entry name: the text, or inside a cycle "CYCL<iteration>-" + text (0x1400687d0)
String simEntryName(const String& text)
{
    if (!g_cycleName.on) return text;
    return qt::L("CYCL") + qt::number(g_cycleName.iteration) + qt::L("-") + text;
}

// 0x1405f5880 (and 0x14029a4b0, 0x1402b7380, which have the same body): entry name = op name
// (0x140050ac0 -> 0x1400682a0, packet S)
void simListOne(int pos, int* counter, Operation* op, std::vector<SimEntry>& out)
{
    SimEntry e;
    setSimPos(e, pos, op);
    *counter = *counter + 1;
    e.sim = *counter;
    if (op) {
        e.name = simEntryName(op->name);
        e.named = true;
    }
    out.push_back(e);
}

// @src 0x1405f5880
void OpClass::simList(int pos, int* counter, Operation* op, std::vector<SimEntry>& out)
{
    simListOne(pos, counter, op, out);
}

// @src 0x14029a4b0
void simListDiestressBM(OpClass&, int pos, int* counter, Operation* op, std::vector<SimEntry>& out)
{
    simListOne(pos, counter, op, out);
}

// @src 0x1402b7380
void simListDiestress(OpClass&, int pos, int* counter, Operation* op, std::vector<SimEntry>& out)
{
    simListOne(pos, counter, op, out);
}

// @src 0x140e32990 / 0x1412b05c0 / 0x14013fbb0
void simListNone(OpClass&, int, int*, Operation*, std::vector<SimEntry>&) {}

// @src 0x14134e280
void simListCycle(OpClass&, int pos, int* counter, Operation* op, std::vector<SimEntry>& out)
{
    if (!op->props) return;                                       // 0x140051230
    // dynamic_cast<SfOprCycleProperties*> (TD 0x143ae2588); the original does not check the result
    auto* cp = dynamic_cast<CycleProps*>(op->props.get());
    Project* prj = op->project;                                   // 0x140055030
    if (!prj) return;                                             // 0x1400a58b0 (operation table)
    SubProject* sub = activeSubProject(*prj);                     // 0x140055050
    if (!sub) return;
    const std::vector<int> ids = groupMembers(*sub, op->id);      // 0x1400b7470
    const int n = cp ? simdata::cycleCount(*cp) : 0;              // 0x14134eeb0
    // 0x140068760(flowNumber, name) turns the entry-name cycle context on, 0x1400687b0(k + 1) sets
    // the iteration, 0x1400687a0 turns it off (packet S; only the iteration reaches the names)
    g_cycleName.on = true;
    for (int k = 0; k < n; ++k) {
        g_cycleName.iteration = k + 1;
        for (int id : ids) {
            Operation* it = operationById(*prj, id);              // 0x140047dc0
            OpClass* h = it ? opBase(*it) : nullptr;              // 0x140054e90
            if (it && h) h->simList(pos, counter, it, out);
        }
    }
    g_cycleName.on = false;                                       // 0x1400687a0
}

// @src 0x1413642d0
void simListDistortion(OpClass& self, int pos, int* counter, Operation* op, std::vector<SimEntry>& out)
{
    auto* dp = dynamic_cast<DistortionProps*>(op->props.get());   // TD 0x1439ee1b0
    if (!dp) return;
    const int n = simdata::distortionPasses(*dp);                 // 0x1401cc6b0 -> 0x140cbc690
    if (0 < n) {
        for (int i = 0; i <= n + 1; ++i) {
            SimEntry e;
            setSimPos(e, pos, op);
            *counter = *counter + 1;
            e.sim = *counter;
            // stage: 1 unload (last), 2 load (first), else 0 (pass i); 0x141363fc0 bare form,
            // then 0x1400683f0 -> 0x1400682a0 (name added by MDT L6T)
            const int stage = i == n + 1 ? 1 : (i == 0 ? 2 : 0);
            e.name = simEntryName(p13::distortionSimnam(self, stage, i, true));
            e.named = true;
            out.push_back(e);
        }
    }
}

// @src 0x140759d50
void simListCogging(OpClass&, int pos, int* counter, Operation* op, std::vector<SimEntry>& out)
{
    auto* cp = dynamic_cast<CoggingProps*>(op->props.get());      // TD 0x1437cf500
    if (!cp) return;
    // one entry object (0x140067f50) reused: position, operation name, then per block the sim
    // number and the name (0x1400683f0 -> 0x1400682a0), appended by copy (0x1400ad490)
    auto add = [&](const String& name) {
        SimEntry e;
        setSimPos(e, pos, op);
        *counter = *counter + 1;
        e.sim = *counter;
        e.name = simEntryName(name);
        e.named = true;
        out.push_back(e);
    };
    using qt::arg;
    using qt::L;
    const double eps = 1e-06;                                     // 0x142157420
    const bool adaptive = simdata::cogAdaptive(*cp);              // 0x140765a10
    const int reheats = simdata::cogReheats(*cp);                 // 0x140765740
    const bool heat = simdata::cogHeatTransfer(*cp);              // 0x14075dc20
    const double dwell = simdata::cogDwellTime(*cp);              // 0x14075dc60
    const double decomp = simdata::cogDecompressionTime(*cp);     // 0x14075dc80
    const int temp = simdata::cogTempType(*cp);                   // 0x14142ff20
    bool bdct = false, bdwl = false, bht = false;
    if (heat && temp != 0) {
        if (eps < decomp) bdct = true;
        if (eps < dwell) bdwl = true;
        if (!simdata::cogRemainingDwell(*cp)) bht = true;         // 0x14075dc40
        else bdwl = true;
    }
    for (int r = 1; r <= reheats; ++r) {
        const int passes = simdata::cogPasses(*cp, r);            // 0x140765770
        for (int p = 1; p <= passes; ++p) {
            int nb = simdata::cogNoBite(*cp, r, p);               // 0x1407657b0
            const bool pht = eps < simdata::cogPassHeatTmax(*cp, r, p);   // PASS+0x38
            if (simdata::cogAutoBite(*cp, r, p) == 0) nb = simdata::cogNoBite(*cp, r, p);
            else nb = simdata::cogAutoBites(*cp, r, p) * 2 - 1;   // 0x140768770
            if (pht && temp != 0) add(arg(L("PS%2-PHT"), p));     // 0x1425e4a18
            if (eps < simdata::cogPassDwell(*cp, r, p) && temp != 0) add(arg(L("PS%2-PDWL"), p));   // 0x1425e4a28
            for (int b = 1; b <= nb; ++b) {
                if (!simdata::cogHasBite(*cp, r, p, b)) continue; // 0x140767bc0
                add(arg(arg(L("PS%2-BDEF%3"), p), b));            // 0x1425e4a38
                if (heat) {
                    if (bdct) add(arg(arg(L("PS%2-BDCT%3"), p), b));   // 0x1425e4a48
                    if (bdwl) add(arg(arg(L("PS%2-BDWL%3"), p), b));   // 0x1425e4a58
                    if (bht) {                                    // 0x1425e4a68, "(Auto)" 0x1425e4a74
                        String n = arg(arg(L("PS%2-BHT%3"), p), b);
                        if (simdata::cogAutoBite(*cp, r, p) != 0) n = n + L("(Auto)");
                        add(n);
                    }
                }
            }
            if (adaptive) add(arg(L("PS%2-SHT"), p));             // 0x1425e4a80
        }
    }
}

// @src 0x141423390
void simListFormingExpress(OpClass& self, int pos, int* counter, Operation* op,
                           std::vector<SimEntry>& out)
{
    auto* p = dynamic_cast<ExpressProps*>(self.propsOf(op));      // 0x14141ce30
    if (!p) return;
    if (!simdata::feExpress(*p)) {                                // 0x14142bf50
        simListOne(pos, counter, op, out);                        // 0x1405f5880
        return;
    }
    auto add = [&] {
        SimEntry e;
        setSimPos(e, pos, op);
        *counter = *counter + 1;
        e.sim = *counter;
        out.push_back(e);
    };
    const int nb = simdata::feBlows(*p);                          // 0x14142c080
    for (int b = 1; b <= nb; ++b) {
        const int ns = simdata::feSteps(*p, b);                   // 0x14142c110
        for (int s = 1; s <= ns; ++s) {
            add();                                                // "BL%1" 0x141423f20
            if (0.0 < simdata::feGap(*p, b, s)) add();            // "DW%1" 0x141424130
            if (simdata::feDrop(*p, b, s) && 0.0 < simdata::feToTime(*p)) add();   // "TO%1" 0x141423d10
        }
    }
}

// @src 0x1411b2790 / 0x1411b0170
void simListHeatTreat(OpClass& self, int pos, int* counter, Operation* op, std::vector<SimEntry>& out)
{
    auto* p = dynamic_cast<HeatTreatProps*>(self.propsOf(op));    // TD 0x143a5b500
    if (!p) return;
    int n = 0;
    if (!simdata::heatTreatSteps(*p, &n)) return;                 // 0x141681de0 null
    for (int k = 0; k < n; ++k) {
        SimEntry e;
        setSimPos(e, pos, op);
        *counter = *counter + 1;
        e.sim = *counter;
        // QString::sprintf("* Operation No. %d ", sim) @0x142c8ecd0, then 0x1400683f0 -> 0x1400682a0
        char buf[48];
        std::snprintf(buf, sizeof buf, "* Operation No. %d ", *counter);
        e.name = simEntryName(qt::String::latin1(buf));
        e.named = true;
        out.push_back(e);
    }
}

// @src 0x140606c10
void simListMultiBlow(OpClass& self, int pos, int* counter, Operation* op, std::vector<SimEntry>& out)
{
    auto* mp = dynamic_cast<MultiBlowProps*>(self.propsOf(op));   // 0x140608ea0
    if (!mp || !mp->data) return;
    const data::MultiBlowData& d = *mp->data;
    // 0x1400683f0(entry, ++*counter, name, DB): the name through 0x1400682a0 (packet S reads it)
    auto add = [&](const String& name) {
        SimEntry e;
        setSimPos(e, pos, op);
        *counter = *counter + 1;
        e.sim = *counter;
        e.name = simEntryName(name);
        e.named = true;
        out.push_back(e);
    };
    const int np = data::mbHeats(d);                              // 0x14060c6f0
    for (int i = 1; i <= np; ++i) {
        const int nb = data::mbBlows(d, i);                       // 0x14060c710
        for (int j = 1; j <= nb; ++j) {
            add(nameBL(op, i, j, true));                          // "BL%1-BL" 0x1406078f0
            if (data::mbDwellOn(d, i, j)) add(nameDW(op, i, j, true));          // 0x140607b00
            if (data::mbHeatOk(d, i, j)) {                        // 0x14060bdd0
                if (0.0 < d.transferInTime) add(nameTI(op, i, j, true));        // 0x1406072c0
                if (0.0 < d.heatingTime) add(nameHT(op, i, j, true));           // 0x1406074d0
                if (0.0 < d.transferOutTime) add(nameTO(op, i, j, true));       // 0x1406076e0
            }
        }
    }
}

// @src 0x1400b8360
std::vector<SimEntry> simEntries(Project& p, SubProject& sub)
{
    std::vector<SimEntry> list;
    Project* prj = sub.project;                                   // 0x1400b5c40
    if (!prj) return list;
    (void)p;
    const std::vector<int> ids = mstOrder(sub);                   // 0x1400b6630
    int counter = 0;
    for (int k = 0; k < static_cast<int>(ids.size()); ++k) {
        Operation* op = operationById(*prj, ids[static_cast<size_t>(k)]);   // 0x140047dc0
        if (!op) continue;
        OpClass* h = opBase(*op);                                 // 0x140054e90
        if (h) h->simList(k + 1, &counter, op, list);
    }
    return list;
}

// @src 0x1400b8820
int nextSim(Project& p, SubProject& sub, int ord)
{
    int r = 0;
    const std::vector<SimEntry> list = simEntries(p, sub);
    for (const SimEntry& e : list) {                              // Q_FOREACH, break on the first match
        if (e.pos == ord) {
            r = e.sim;
            break;
        }
    }
    if (r == 0 && 1 < ord) {
        // 0x1400b8540 builds the list again and keeps the entries of position ord - 1; the last
        // one's sim + 1
        std::vector<SimEntry> prev;
        for (const SimEntry& e : simEntries(p, sub))
            if (e.pos == ord - 1) prev.push_back(e);
        if (!prev.empty()) r = prev.back().sim + 1;
    }
    return r;
}

}  // namespace m2m::mst
