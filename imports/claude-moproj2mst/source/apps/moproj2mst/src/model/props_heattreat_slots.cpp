// L2: the qt_static_metacall set and get slots of SfOprHeatTreatProperties (0x1411c41b0; moc
// string table at file offset 0x2cad5c0: slots 0, 1 slotAutoGenerateSymPlanes /
// slotAutoGenerateFixedNodes, then set/get pairs Deformation, Diffusion, PhaseTransformation,
// NO_MEDIA, ENVTMP, TMAX, MEDNAM, CNVCOF, ENVATM, ACVCOF, ENVRAD, STEPDEF, DTMAX, STPINC, DTEMPMAX,
// DTEMPLMT0, DTEMPLMT1), and the Process<i> nodes of the schedule that 0x1411bfd60 (props
// vt+0x58) and setNO_MEDIA 0x1411c0c30 make. Decompiles: gui_mo/s1/moproj_rest/hta.c, htla.c,
// tpla.c. B = PROPS+0x58 (data::HTPropsBlock). Dropped UI: the STEPDEF slots' enabled flags
// (0x140370e90) on props+0x38 .. +0x50, the node-pointer lists B+0x60 .. +0x98 and the messenger
// signal 0x1402cea00.
#include <cstddef>
#include <cstdint>
#include <functional>
#include <type_traits>
#include <utility>

#include <defmodel/layout/cls_1401e7b10.hpp>

#include "m2m/data/heattreat.hpp"
#include "m2m/data/l6_blocks.hpp"
#include "m2m/engine.hpp"
#include "m2m/ht_schedule.hpp"
#include "m2m/props.hpp"
#include "m2m/props_tree.hpp"
#include "m2m/props_variant.hpp"

namespace m2m {

namespace {

using def::model::Cls_1401e7b10;
using qt::Variant;

[[noreturn]] void unsupported(const std::string& what)
{
    throw Error(M2M_E_UNSUPPORTED, "properties: SfOprHeatTreatProperties " + what);
}

data::HTPropsBlock& B(HeatTreatProps& hp)
{
    if (!hp.block) unsupported("without its block (+0x58)");
    return *hp.block;
}

// 0x1405f8d90 -> 0x14005f7e0: synchronously borrow the owner's sim control, or null.
// Callers must not retain the pointer or queue work. simCtl preserves geometry refusal.
template<class F>
    requires std::is_invocable_v<F, Cls_1401e7b10*> &&
             std::is_same_v<std::invoke_result_t<F, Cls_1401e7b10*>, void>
void withOwnerSimCtl(HeatTreatProps& hp, F&& callback)
{
    if (!hp.owner || !hp.owner->project) {
        std::invoke(std::forward<F>(callback), nullptr);
        return;
    }
    engine::GuiModel* m = engine::opModel(*hp.owner->project, *hp.owner);
    std::invoke(std::forward<F>(callback), m ? m->simCtl() : nullptr);
}

Project& ownerProject(HeatTreatProps& hp)
{
    if (!hp.owner || !hp.owner->project) unsupported("without an owner operation (props vt+0x58 needs it)");
    return *hp.owner->project;
}

void setScWord(Cls_1401e7b10* sc, int off, bool v)       // 0x1408e0000 / 0x1408e0040 / 0x1408e0020
{
    *reinterpret_cast<std::int32_t*>(reinterpret_cast<std::byte*>(sc) + off) = v ? 1 : 0;
}

// QList::at after the 0 <= idx < size test the slots make
template <class T> bool inRange(const std::vector<T>& l, int idx)
{
    return idx >= 0 && idx < static_cast<int>(l.size());
}

// The Process child templates (0x1411bd890: 0x1446e2860 .. 0x1446e2898)
tree::Template processT() { return tree::group("Process"); }                         // 0x1446e2860
tree::Template tmaxT() { return tree::doubleProp("TMAX", 3600.0); }                 // 0x1446e2868 (0x1424bb068)
tree::Template mednamT() { return tree::other("MEDNAM", "QString", tree::vText(qt::L("None"))); }   // 0x1446e2870
tree::Template envtmpT() { return tree::doubleProp("ENVTMP", 30.0); }               // 0x1446e2878 (0x1421ce528)
tree::Template cnvcofT() { return tree::doubleProp("CNVCOF", 0.1); }                // 0x1446e2880 (0x1421bfdd8)
tree::Template envatmT() { return tree::doubleProp("ENVATM", 0.0); }                // 0x1446e2888
tree::Template acvcofT() { return tree::doubleProp("ACVCOF", 0.0); }                // 0x1446e2890
tree::Template envradT() { return tree::other("ENVRAD", "bool", tree::vInt(1)); }   // 0x1446e2898

// 0x140375a70(parent, props, template, index): a node appended to parent
PropNode& addNode(std::vector<PropNode>& parent, const tree::Template& t, int index)
{
    PropNode& n = tree::add(parent, t);
    n.index = index;
    return n;
}

// @src 0x1411c0c30  setNO_MEDIA (from 0x1411c20e0)
void setNoMedia(HeatTreatProps& hp, int n)
{
    data::HTPropsBlock& b = B(hp);
    PropNode* g = ht::scheduleGroup(hp);
    if (n < b.stepCount) {
        // child n of the group removed stepCount - n times; the value lists keep their entries
        for (int k = n; k < b.stepCount; ++k)
            if (g && n < static_cast<int>(g->children.size()))
                g->children.erase(g->children.begin() + n);       // 0x1403762e0, 0x140371280
    } else {
        const double envtmp = 30.0, tmax = 3600.0, cnvcof = 0.1, envatm = 0.0, acvcof = 0.0;
        const String mednam = qt::L("None");
        for (int i = 0; i < n; ++i) {
            if (b.stepCount <= i) {
                b.t3.push_back(envtmp);                            // +0xb0
                b.time.push_back(tmax);                            // +0xa0
                b.media.push_back(mednam);                         // +0xa8
                b.cnvcof.push_back(cnvcof);                        // +0xb8
                if (b.b1) {
                    b.t4.push_back(envatm);                        // +0xc0
                    b.acvcof.push_back(acvcof);                    // +0xc8
                }
                b.mediaFlag.push_back(true);                       // +0xd0
            }
        }
        if (b.stepCount < n && !g) unsupported("NO_MEDIA without the Thermal Schedule group (block+0x40)");
        for (int i = b.stepCount; i < n; ++i) {
            PropNode& pr = addNode(g->children, processT(), i);
            addNode(pr.children, tmaxT(), i);
            addNode(pr.children, mednamT(), i);
            addNode(pr.children, envtmpT(), i);
            addNode(pr.children, cnvcofT(), -1);
            if (b.b1) {
                addNode(pr.children, envatmT(), i);
                addNode(pr.children, acvcofT(), -1);
            }
            addNode(pr.children, envradT(), i);
        }
    }
    b.stepCount = n;                                               // B+4
}

// The Mode setters 0x1411c1c30 / 0x1411c1dc0 / 0x1411c1f50: the schedule's engine defaults when
// not applied yet (0x14172d790, 0x1411b3540), the flag, the sim control word, then props vt+0x58
void setMode(HeatTreatProps& hp, bool& flag, int scOff, const Variant& v)
{
    if (hp.data && !hp.data->schedule.engineDefaults) ht::engineDefaults(ownerProject(hp), hp.data->schedule);
    flag = pv::toBool(v);
    withOwnerSimCtl(hp, [scOff, flag](Cls_1401e7b10* sc) -> void {
        if (sc) setScWord(sc, scOff, flag);
    });
    ht::propsRefresh(ownerProject(hp), hp);                        // vt+0x58 = 0x1411bfd60
}

// The Mode getters 0x1411c1d20 / 0x1411c1eb0 / 0x1411c2040 write the sim control word as well
Variant getMode(HeatTreatProps& hp, bool flag, int scOff)
{
    withOwnerSimCtl(hp, [scOff, flag](Cls_1401e7b10* sc) -> void {
        if (sc) setScWord(sc, scOff, flag);
    });
    return pv::vBool(flag);
}

}  // namespace

namespace ht {

PropNode* scheduleGroup(HeatTreatProps& hp)
{
    for (PropNode& r : hp.tree)
        for (PropNode& c : r.children)
            if (c.name == qt::L("Thermal Schedule")) return &c;
    return nullptr;
}

// @src 0x1411c0980 (tree part): child 0 of the group removed stepCount times
void scheduleNodesRemove(HeatTreatProps& hp)
{
    PropNode* g = scheduleGroup(hp);
    const int n = hp.block ? hp.block->stepCount : 0;
    for (int i = 0; i < n; ++i)
        if (g && !g->children.empty()) g->children.erase(g->children.begin());
}

// @src 0x1411bfd60 (second loop body)
void scheduleNodesAdd(HeatTreatProps& hp, int i, const data::HTStep& st)
{
    PropNode* g = scheduleGroup(hp);
    if (!g) unsupported("without the Thermal Schedule group (block+0x40)");
    PropNode& pr = addNode(g->children, processT(), i);
    addNode(pr.children, tmaxT(), i);                              // into B+0x68
    addNode(pr.children, mednamT(), i);                            // B+0x70
    if (st.t20.ndim == 0) addNode(pr.children, envtmpT(), i);      // 0x140a99e10, 0x14089b800: B+0x78
    if (hp.block && hp.block->b1 && st.t98.ndim == 0)              // 0x1411c24c0
        addNode(pr.children, envatmT(), i);                        // B+0x88
    addNode(pr.children, envradT(), i);                            // B+0x98
}

}  // namespace ht

// ---- SfOprHeatTreatProperties: qt_static_metacall 0x1411c41b0 ----------------------------------
// @src 0x1411c1c30 setDeformation, 0x1411c1dc0 setDiffusion, 0x1411c1f50 setPhaseTransformation,
// 0x1411c20e0 setNO_MEDIA, 0x1411bceb0 setENVTMP, 0x1411bcba0 setTMAX, 0x1411bcd00 setMEDNAM,
// 0x1411bd010 setCNVCOF, 0x1411bd170 setENVATM, 0x1411bd2d0 setACVCOF, 0x1411bd430 setENVRAD,
// 0x1411c2190 setSTEPDEF, 0x1411bc880 setDTMAX, 0x1411bc920 setSTPINC, 0x1411bc9c0 setDTEMPMAX,
// 0x1411bca60 setDTEMPLMT0, 0x1411bcb00 setDTEMPLMT1 (even cases 2 .. 0x22)
void HeatTreatProps::applyValue(PropNode& node, const std::vector<PropNode*>& ancestors, const Variant& v)
{
    data::HTPropsBlock& b = B(*this);
    const std::u16string n = node.name.u16();                      // copy: NO_MEDIA changes the tree
    const int idx = node.index;                                    // 0x140370fd0
    auto setD = [&](std::vector<double>& l) {
        if (inRange(l, idx)) l[static_cast<std::size_t>(idx)] = pv::toDouble(v);
    };
    if (n == u"Deformation") setMode(*this, b.b0, 0x108, v);
    else if (n == u"Diffusion") setMode(*this, b.b1, 0x110, v);
    else if (n == u"PhaseTransformation") setMode(*this, b.b2, 0x10c, v);
    else if (n == u"NO_MEDIA") setNoMedia(*this, pv::toInt(v));
    else if (n == u"ENVTMP") setD(b.t3);
    else if (n == u"TMAX") setD(b.time);
    else if (n == u"MEDNAM") {
        if (inRange(b.media, idx)) b.media[static_cast<std::size_t>(idx)] = pv::toString(v);
    } else if (n == u"CNVCOF") setD(b.cnvcof);
    else if (n == u"ENVATM") setD(b.t4);
    else if (n == u"ACVCOF") setD(b.acvcof);
    else if (n == u"ENVRAD") {
        if (inRange(b.mediaFlag, idx)) b.mediaFlag[static_cast<std::size_t>(idx)] = pv::toBool(v);
    } else if (n == u"STEPDEF") {
        b.stpdef = pv::toInt(v);                                   // B+8
        withOwnerSimCtl(*this, [&b](Cls_1401e7b10* sc) -> void {
            if (sc) sc->stpdef = b.stpdef == 0 ? 1 : 3;              // SC+0x1c8
        });
    } else if (n == u"DTMAX") b.dtmax = pv::toDouble(v);           // B+0x10
    else if (n == u"STPINC") b.stepIncr = static_cast<int>(pv::toDouble(v));   // B+0xc
    else if (n == u"DTEMPMAX") b.dtpmax1 = pv::toDouble(v);        // B+0x18
    else if (n == u"DTEMPLMT0") b.dtpmax2 = pv::toDouble(v);       // B+0x20
    else if (n == u"DTEMPLMT1") b.f28 = pv::toDouble(v);           // B+0x28
    else Props::applyValue(node, ancestors, v);
}

// @src 0x1411c1d20 getDeformation, 0x1411c1eb0 getDiffusion, 0x1411c2040 getPhaseTransformation,
// 0x1411c2140 getNO_MEDIA, 0x1411bcf50 getENVTMP, 0x1411bcc40 getTMAX, 0x1411bcdc0 getMEDNAM,
// 0x1411bd0b0 getCNVCOF, 0x1411bd210 getENVATM, 0x1411bd370 getACVCOF, 0x1411bd4c0 getENVRAD,
// 0x1411c2300 getSTEPDEF, 0x1411bc8d0 getDTMAX, 0x1411bc970 getSTPINC, 0x1411bca10 getDTEMPMAX,
// 0x1411bcab0 getDTEMPLMT0, 0x1411bcb50 getDTEMPLMT1 (odd cases 3 .. 0x23). An index out of range
// gives 0.0, false or "" (0x142ca1a5f).
bool HeatTreatProps::getValue(const PropNode& node, const std::vector<PropNode*>&, Variant& out)
{
    data::HTPropsBlock& b = B(*this);
    const std::u16string& n = node.name.u16();
    const int idx = node.index;
    auto dbl = [&](const std::vector<double>& l) {
        return pv::vDouble(inRange(l, idx) ? l[static_cast<std::size_t>(idx)] : 0.0);
    };
    if (n == u"Deformation") out = getMode(*this, b.b0, 0x108);
    else if (n == u"Diffusion") out = getMode(*this, b.b1, 0x110);
    else if (n == u"PhaseTransformation") out = getMode(*this, b.b2, 0x10c);
    else if (n == u"NO_MEDIA") out = pv::vInt(b.stepCount);
    else if (n == u"ENVTMP") out = dbl(b.t3);
    else if (n == u"TMAX") out = dbl(b.time);
    else if (n == u"MEDNAM") out = pv::vText(inRange(b.media, idx) ? b.media[static_cast<std::size_t>(idx)] : qt::L(""));
    else if (n == u"CNVCOF") out = dbl(b.cnvcof);
    else if (n == u"ENVATM") out = dbl(b.t4);
    else if (n == u"ACVCOF") out = dbl(b.acvcof);
    else if (n == u"ENVRAD") out = pv::vBool(inRange(b.mediaFlag, idx) && b.mediaFlag[static_cast<std::size_t>(idx)]);
    else if (n == u"STEPDEF") {
        withOwnerSimCtl(*this, [&b](Cls_1401e7b10* sc) -> void {
            if (sc) sc->stpdef = b.stpdef == 0 ? 1 : 3;              // SC+0x1c8
        });
        out = pv::vInt(b.stpdef);
    } else if (n == u"DTMAX") out = pv::vDouble(b.dtmax);
    else if (n == u"STPINC") out = pv::vInt(b.stepIncr);
    else if (n == u"DTEMPMAX") out = pv::vDouble(b.dtpmax1);
    else if (n == u"DTEMPLMT0") out = pv::vDouble(b.dtpmax2);
    else if (n == u"DTEMPLMT1") out = pv::vDouble(b.f28);
    else return base::getValue(*this, node, out);
    return true;
}

}  // namespace m2m
