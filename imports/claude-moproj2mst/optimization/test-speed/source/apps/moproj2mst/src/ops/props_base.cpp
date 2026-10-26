// SfOprBaseProperties tree adders and set* slots (packet P15): they fill data::CommonBlock, which
// the writers read through 0x1405fb6a0 (SimCtrl), 0x1405fc890 (friction) and 0x1405fc8e0 (IHTC).
// Decompiles: gui_mo/s1/p15/m15.c (adders), m16.c (0x1405fb450), m17.c / m18.c (slots).
#include "m2m/props_base.hpp"

#include <memory>

namespace m2m::data {
// @src 0x1405fd250
void initCommonBlock(CommonBlock& b, int unit)
{
    b = CommonBlock{};
    b.numSteps = 100;
    b.stepIncr = 10;
    b.heatingTime = qt::L("15.0 sec");             // 0x14249d8a0
    if (unit == 2) {
        b.envTemp = qt::L("68 F");                  // 0x14249d898
        b.ihtc = qt::L("0.004 Btu/sec/in^2/F");     // 0x14249d880
        b.convection = qt::L("7.7e-06 Btu/sec/in^2/F");   // 0x14249d868
    } else {
        b.envTemp = qt::L("20 C");                  // 0x14249d860
        b.ihtc = qt::L("11.0 N/sec/mm/C");          // 0x14249d850
        b.convection = qt::L("0.02 N/sec/mm/C");    // 0x14249d840
    }
    b.dsmax = qt::L("0.0 mm");                      // 0x14249d838
    b.dtmax = qt::L("0.0 sec");                     // 0x14249d830
    b.frictionType = 0;
    b.friction = 0.0;
    b.dtMode = 0;
}
}  // namespace m2m::data

namespace m2m::base {

namespace {

data::CommonBlock& B(Props& p)
{
    if (!p.common) p.common = std::make_shared<data::CommonBlock>();
    return *p.common;
}

PropNode node(const char* name, const char* type, qt::Variant v = {})
{
    PropNode n;
    n.name = qt::L(name);
    n.typeName = qt::L(type);
    n.value = std::move(v);
    return n;
}
qt::Variant vInt(int i)
{
    qt::Variant v;
    v.kind = qt::Variant::Int;
    v.i = i;
    return v;
}
qt::Variant vDbl(double d)
{
    qt::Variant v;
    v.kind = qt::Variant::Double;
    v.d = d;
    return v;
}
qt::Variant vStr(const qt::String& s)
{
    qt::Variant v;
    v.kind = qt::Variant::Text;
    v.s = s;
    return v;
}

// QVariant::toInt / toDouble / toString of the reader's value (s1-01 6.3)
int toInt(const qt::Variant& v)
{
    switch (v.kind) {
    case qt::Variant::Double: return static_cast<int>(v.d);
    case qt::Variant::Int: return v.i;
    case qt::Variant::Text: return qt::toInt(v.s);
    default: return 0;
    }
}
double toDouble(const qt::Variant& v)
{
    switch (v.kind) {
    case qt::Variant::Double: return v.d;
    case qt::Variant::Int: return static_cast<double>(v.i);
    case qt::Variant::Text: return qt::toDouble(v.s);
    default: return 0.0;
    }
}
qt::String toString(const qt::Variant& v)
{
    switch (v.kind) {
    case qt::Variant::Text: return v.s;
    case qt::Variant::Int: return qt::number(v.i);
    default: return qt::String();
    }
}

// B+0x50: the TestGroup node (template 0x143d9dcd0), made as a top-level node when missing
PropNode& group(Props& p)
{
    if (PropNode* g = testGroup(p)) return *g;
    p.tree.push_back(node("TestGroup", "group"));
    return p.tree.back();
}

void removeChild(PropNode& g, const char* name)
{
    const qt::String n = qt::L(name);
    for (auto it = g.children.begin(); it != g.children.end(); ++it) {
        if (it->name == n) {
            g.children.erase(it);
            return;
        }
    }
}

}  // namespace

PropNode* testGroup(Props& p)
{
    for (PropNode& n : p.tree)
        if (n.name.u16() == u"TestGroup") return &n;
    return nullptr;
}

// @src 0x1405f9eb0
void addStepCounts(Props& p, bool withTime, bool timeEnabled, int numSteps, int stepIncr,
                   const qt::String& timeText)
{
    PropNode& g = group(p);
    B(p).numSteps = numSteps;
    B(p).stepIncr = stepIncr;
    g.children.push_back(node("NumSteps", "int", vInt(numSteps)));     // 0x143d9dce0
    g.children.push_back(node("StepIncr", "int", vInt(stepIncr)));     // 0x143d9dce8
    if (withTime) {
        PropNode t = node("TimePerStep", "doubleUnits", vStr(timeText));   // 0x143d9dd18 (B+0x60)
        (void)timeEnabled;                          // 0x140370e90: enabled flag, UI only
        g.children.push_back(std::move(t));
    }
}

// @src 0x1405fa560
void addEnvTemp(Props& p, int unit, const qt::String& english, const qt::String& si)
{
    PropNode& g = group(p);
    B(p).envTemp = unit == 2 ? english : si;
    g.children.push_back(node("EnvTemp", "doubleUnits", vStr(english)));   // 0x143d9dcf8
}

// @src 0x1405faaa0
void addConvection(Props& p, int unit, const qt::String& english, const qt::String& si)
{
    PropNode& g = group(p);
    B(p).convection = unit == 2 ? english : si;
    g.children.push_back(node("ConvectionCoef", "doubleUnits", vStr(english)));   // 0x143d9dd08
}

// @src 0x1405fb020
void addStepBy(Props& p, int unit, int stepBy, const qt::String& distEnglish, const qt::String& distSi,
               const qt::String& time)
{
    PropNode& g = group(p);
    B(p).dsmax = unit == 2 ? distEnglish : distSi;  // B+0x38
    B(p).dtmax = time;                              // B+0x40
    g.children.push_back(node("StepBy", "enum", vInt(stepBy)));   // 0x143d9dcd8
    // DistPerStep (0x143d9dd10 -> B+0x58) and TimePerStep (0x143d9dd18 -> B+0x60) are made without
    // a parent; 0x1405fb450 attaches the one the mode needs.
    B(p).dtMode = 1 - stepBy;
    setStepBy(p, stepBy);
}

// @src 0x1405fad10
void addFriction(Props& p, PropNode& grp, int unit, double english, double si)
{
    B(p).frictionType = 0;                          // B+0xc
    grp.children.push_back(node("FrictionType", "enum", vInt(0)));        // 0x143d9dd20
    B(p).friction = unit == 2 ? english : si;       // B+0x10
    grp.children.push_back(node("FrictionCoef", "double", vDbl(english)));   // 0x143d9dd28
}

// @src 0x1405fa7d0
void addHeatTransCoef(Props& p, PropNode& grp, int unit, const qt::String& english,
                      const qt::String& si)
{
    B(p).ihtc = unit == 2 ? english : si;           // B+0x28
    grp.children.push_back(node("HeatTransCoef", "doubleUnits", vStr(english)));   // 0x143d9dd00
}

// @src 0x1405fb450 (the signal 0x1402cea00 is UI only)
void setStepBy(Props& p, int mode)
{
    if (B(p).dtMode == mode) return;
    PropNode& g = group(p);
    if (mode == 0) {
        removeChild(g, "TimePerStep");
        g.children.push_back(node("DistPerStep", "doubleUnits", vStr(B(p).dsmax)));
    } else {
        removeChild(g, "DistPerStep");
        g.children.push_back(node("TimePerStep", "doubleUnits", vStr(B(p).dtmax)));
    }
    B(p).dtMode = mode;
}

// set* slots of 0x1405f7b10 (even cases 2..0x16)
bool applyBaseValue(Props& p, PropNode& n, const std::vector<PropNode*>&, const qt::Variant& v)
{
    const std::u16string s = n.name.u16();   // copy: setStepBy may move `n` (TestGroup children)
    if (s == u"NumSteps") B(p).numSteps = toInt(v);                     // @src 0x1405fd0e0
    else if (s == u"StepIncr") B(p).stepIncr = toInt(v);                // @src 0x1405f85c0
    else if (s == u"HeatingTime") B(p).heatingTime = toString(v);  // @src 0x1405fd180
    else if (s == u"EnvTemp") B(p).envTemp = toString(v);              // @src 0x1405f8660
    else if (s == u"HeatTransCoef") B(p).ihtc = toString(v);       // @src 0x1405f8730
    else if (s == u"ConvectionCoef") B(p).convection = toString(v);       // @src 0x1405f8800
    else if (s == u"FrictionType") B(p).frictionType = toInt(v);   // @src 0x1405f8a70
    else if (s == u"FrictionCoef") B(p).friction = toDouble(v);    // @src 0x1405f8b10
    else if (s == u"StepBy") setStepBy(p, toInt(v));               // @src 0x1405fb560
    else if (s == u"DistPerStep") B(p).dsmax = toString(v);        // @src 0x1405f88d0
    else if (s == u"TimePerStep") B(p).dtmax = toString(v);        // @src 0x1405f89a0
    else return false;
    return true;
}

}  // namespace m2m::base

namespace m2m {
// Props vt slot of the set* dispatch: the base class answers only its own slot names.
void Props::applyValue(PropNode& node, const std::vector<PropNode*>& ancestors, const qt::Variant& v)
{
    base::applyBaseValue(*this, node, ancestors, v);
}
}  // namespace m2m

// ---- moproj-ops: SfOprBaseProperties get slots (Props::getValue fallback; here because the
// standalone tests link this file) -------------------------------------------------------------
namespace m2m {

namespace base {

namespace {

qt::Variant gsInt(int v)
{
    qt::Variant r;
    r.kind = qt::Variant::Int;
    r.i = v;
    return r;
}

qt::Variant gsDouble(double v)
{
    qt::Variant r;
    r.kind = qt::Variant::Double;
    r.d = v;
    return r;
}

qt::Variant gsText(const String& v)
{
    qt::Variant r;
    r.kind = qt::Variant::Text;
    r.s = v;
    return r;
}

}  // namespace

// @src 0x1405f7b10  (get cases; B = PROPS+0x30, data::CommonBlock)
bool getValue(const Props& p, const PropNode& node, qt::Variant& out)
{
    static const data::CommonBlock none;
    const data::CommonBlock& b = p.common ? *p.common : none;
    const std::u16string& n = node.name.u16();
    if (n == u"NumSteps") out = gsInt(b.numSteps);                     // 0x1405fd130: B+0
    else if (n == u"StepIncr") out = gsInt(b.stepIncr);                // 0x1405f8610: B+4
    else if (n == u"HeatingTime") out = gsText(b.heatingTime);         // 0x1405fd200: B+0x18
    else if (n == u"EnvTemp") out = gsText(b.envTemp);                 // 0x1405f86e0: B+0x20
    else if (n == u"HeatTransCoef") out = gsText(b.ihtc);              // 0x1405f87b0: B+0x28
    else if (n == u"ConvectionCoef") out = gsText(b.convection);       // 0x1405f8880: B+0x30
    else if (n == u"FrictionType") out = gsInt(b.frictionType);        // 0x1405f8ac0: B+0xc
    else if (n == u"FrictionCoef") out = gsDouble(b.friction);         // 0x1405f8b60: B+0x10
    else if (n == u"StepBy") out = gsInt(b.dtMode);                    // 0x1405fb5b0: B+8
    else if (n == u"DistPerStep") out = gsText(b.dsmax);               // 0x1405f8950: B+0x38
    else if (n == u"TimePerStep") out = gsText(b.dtmax);               // 0x1405f8a20: B+0x40
    else return false;
    return true;
}

}  // namespace base
}  // namespace m2m
