// FormingExpress(2D) writer, its data block and the SimCtrl values it reads (packet P15).
// Spec: s1-05 5 (with the corrections in src/ops/express.cpp), decompiles s1-05 d1/d4/d5/d7 and
// s1/p15/m*.c. Addresses are VAs in DEF_GUI_MO_64.exe.
#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "m2m/data/common.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/loader_l3.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/props.hpp"

namespace m2m::data {

// FormingExpress data "D" = SfOprFormingExpressProperties+0x40 (0x178 bytes, ctor 0x14142fac0,
// created by 0x1414263f0). Members keep the offset; names follow their use in the writer.
struct FormingExpressData {
    std::uint8_t f10 = 0;          // +0x10 set by 0x14142c8b0 (model present); not read by the writer
    int type = 2;                  // +0x14 movement type: 3, 4 = express (0x14142bb30); 0x14142cb20
    String name;                   // +0x18 movement name (0x14142cb20 -> Movement)
    double hammerEnergy = 425.06;           // +0x28 type 3: Movement+0x1f0
    double hammerMass = 0.013;            // +0x30 type 3: Movement+0x1e8
    double hammerEfficiency = 0.85;             // +0x38 type 3: Movement+0x1f8
    double screwEnergy = 79200.0;          // +0x40 type 4: Movement+0x280
    double screwEfficiency = 0.9;              // +0x48 type 4: Movement+0x288
    double momentOfInertia = 4606.46;          // +0x50 type 4: Movement+0x290
    double leadScrewPitch = 21.09;            // +0x58 type 4: Movement+0x298
    int mechPressType = 0;                   // +0x60
    double totalStroke = 10.0;             // +0x68 type 2: Movement+0x318
    double cyclesSec = 0.6;              // +0x70 type 2: Movement+0x320
    double connectingRodLength = 0.0;              // +0x78 type 2: Movement+0x328
    double dir[3] = {0.0, 0.0, -1.0};   // +0x88 +0x90 +0x98 movement direction (tree builder sets
                                        // (0,-1,0) for SfOprFormingExpress2D, (0,0,-1) otherwise)
    std::uint8_t initStrain = 0;   // +0xa8 TO: STRAIN reset (0x14142bc40)
    double toTemp = 950.0;         // +0xc0 TO: NDTMP value (0x14142bc90); 1800 when English
    double roomTemp = 20.0;             // +0xc8 read by TO (0x14142bce0), result unused; 68 when English
    double toTime = 10.0;          // +0xd0 TO total time (0x14142bd00)
    double toStep = 1.0;           // +0xd8 TO time step (0x14142bd50)
    double heatingTime = 3600.0;           // +0xe0
    double heatingTimeStep = 20.0;             // +0xe8
    String dwStep = String(u"0.1 sec");   // +0xf8 DW time step, unit category 0 (0x14142bdb0)
    String dwIhtc;                 // +0x100 DW IHTC, category 0x20 (0x14142be90); unit default
    std::uint8_t express = 0;      // +0x110 express mode (0x14142bf50)
    int blows = 0;                 // +0x120 (0x14142c080)
    std::vector<int> steps;                        // +0x128 steps per blow (0x14142c110); NumBlows
    std::vector<double> temperature;               // +0x130 per-blow Temperature (100 when added)
    std::vector<std::vector<double>> stroke;       // +0x150 (0x14142c210): DOESCL = stroke / 100
    std::vector<std::vector<double>> l158;         // +0x158 (0x14142c3e0): read, result unused
    std::vector<std::vector<double>> gap;          // +0x160 (0x14142c490): DW when > 0
    std::vector<std::vector<std::uint8_t>> drop;   // +0x168 (0x14142c5f0): TO / die-back
    std::vector<std::vector<std::uint8_t>> flip;   // +0x170 (0x14142c750)
};

// @src 0x14142fac0  ctor defaults; unit = the GUI unit global (mainWindow vt+0x260, 2 = English)
void initFormingExpressData(FormingExpressData& d, int unit);

// ---- property setters (express_tree.cpp); the tree names are the MultiBlow ones: a "heat" here is
// a blow (b), a "blow" is a step (s). k, j are the 0-based node indices (SfProperty+0x48). ----------
// @src 0x14142d610  setNumHeats
void feSetNumHeats(FormingExpressData& d, int n);
// @src 0x14142e260  setNumBlows of heat k
void feSetNumBlows(FormingExpressData& d, int k, int n);
// @src 0x1414261c0  setTemperature of heat k
void feSetTemperature(FormingExpressData& d, int k, const qt::Variant& v);
// @src 0x14142ed40 / 0x14142eff0 / 0x14142f2a0 / 0x14142f550 / 0x14142f7f0  per-blow setters
bool feSetBlow(FormingExpressData& d, const String& name, int k, int j, const qt::Variant& v);
// scalar setters of 0x1414645a0 (Equipment .. UseBlowTable, NumHeats); false for other names
bool feSetScalar(FormingExpressData& d, const String& name, const qt::Variant& v);

// ---- property tree (express_tree.cpp) ------------------------------------------------------------
// @src 0x141429cc0  default tree for the data `d` (after the ctor's setNumHeats(1))
std::vector<PropNode> formingExpressPropTree(const FormingExpressData& d, bool is2D);
std::vector<PropNode> feEquipmentTypeNodes();                          // @src 0x14142a740
std::vector<PropNode> feEquipmentSpecNodes(const FormingExpressData& d);   // @src 0x14142a950
PropNode feHeatNode(int k);                                            // heat k (0x14142d610)
PropNode feBlowNode(int j);                                            // blow j (0x14142e260)

}  // namespace m2m::data

namespace m2m {

// SfOprFormingExpressProperties (a SfOprExpressProperties: +0x38 Forming.FWZ stays P06's).
struct FormingExpressProps : ExpressProps {
    std::shared_ptr<data::FormingExpressData> fe = std::make_shared<data::FormingExpressData>();   // +0x40
    // @src 0x141465fe0 -> 0x1414645a0  the set* slots, and the Heat/Blow nodes that setNumHeats /
    // setNumBlows add to or remove from the tree (express_tree.cpp)
    void applyValue(PropNode& node, const std::vector<PropNode*>& ancestors, const qt::Variant& v) override;
    // @src 0x1414645a0  the get slots (moproj-ops, express_tree.cpp), then the base's
    bool getValue(const PropNode& node, const std::vector<PropNode*>& ancestors, qt::Variant& out) override;
};

// @src 0x14141ce30  dynamic_cast<SfOprFormingExpressProperties*>(props of op (or of current))
FormingExpressProps* feProps(const mst::OpClass& self, Operation* op);

// ---- D accessors (1-based b, s; 0 / false when out of range) ------------------------------------
bool feIsExpressType(const FormingExpressProps& p);           // @src 0x14142bb30  type 3 or 4
bool feExpressFlag(const FormingExpressProps& p);             // @src 0x14142bf50  +0x110
int feBlowCount(const FormingExpressProps& p);                // @src 0x14142c080  +0x120
int feStepCount(const FormingExpressProps& p, int b);         // @src 0x14142c110
double feStroke(const FormingExpressProps& p, int b, int s);  // @src 0x14142c210
double feL158(const FormingExpressProps& p, int b, int s);    // @src 0x14142c3e0
double feGapAt(const FormingExpressProps& p, int b, int s);   // @src 0x14142c490
bool feDropAt(const FormingExpressProps& p, int b, int s);    // @src 0x14142c5f0
bool feFlipAt(const FormingExpressProps& p, int b, int s);    // @src 0x14142c750
double feDwIhtc(const FormingExpressProps& p);                // @src 0x14142be90
double feDwStep(const FormingExpressProps& p);                // @src 0x14142be70 -> 0x14142bdb0

// ---- SfOprBaseProperties values (owner P15: s1-05 5.5) -------------------------------------------
// @src 0x1405f8dc0  unit of the props' owner operation (0x140054ff0: project unit, 0 without one)
int propsUnit(const Props& p);
// @src 0x1405fc890  friction = B+0x10 (no conversion)
double propsFriction(const Props& p);
// @src 0x1405fc8e0  IHTC = unit value of B+0x28, category 0x20
double propsIhtc(const Props& p);

// The parts of the DEF_PRE SIM_CTRL (0x1408d3bc0) the FormingExpress BL block reads.
struct SimCtrlVals {
    int unit = 0;                  // +0xfc   (0x1408e0200)
    int i164 = 0;                  // +0x164
    int i16c = 0;                  // +0x16c
    int i1c8 = 1;                  // +0x1c8
    double dtmax = 0.0;            // +0x1d0
    double d1d8 = 0.0;             // +0x1d8 (no writer on this path)
    double dsmax = 0.0;            // +0x1e0  (0x1405fd4b0)
    int kw1ec = 0;                 // +0x1ec word 1 of the 0x80 keyword descriptor (0x140899010)
    int dtMode = 1;                // +0x240  (0x1408da6f0 / read 0x1408da6d0)
    double d3d0 = 0.0;             // +0x3d0
    double d4c8 = 0.0;             // +0x4c8
};
// @src 0x1408d3cd0 -> 0x1408d3d10 -> 0x1408d47a0  defaults of the fields above (+0x240 from
// 0x1408da660: 1 because +0x1e0 <= 0 and +0x1ec == 0)
void initSimCtrlVals(SimCtrlVals& sc);
// @src 0x1405fb6a0  props vt+0x70: vt+0x80, +0x90, +0xa0, +0xb0 with a SIM_CTRL
void propsToSimCtrl(const Props& p, SimCtrlVals& sc);
bool propsSimCtrl80(const Props& p, SimCtrlVals& sc);         // @src 0x1405fb980
bool propsSimCtrl90(const Props& p, SimCtrlVals& sc);         // @src 0x1405fbd10
bool propsSimCtrlA0(const Props& p, SimCtrlVals& sc);         // @src 0x1405fc060
bool propsSimCtrlB0(const Props& p, SimCtrlVals& sc);         // @src 0x1405fc5d0

namespace mst {

// The movement scratch object (DEF_PRE twin of 0x14085f3f0) as far as the writer reads it.
// Defaults from its init 0x14085f6d0 (via 0x14085f540): mode 1; dir[dim-1] = 0x1421d3c90, the
// others 0; feToMovement then overwrites all three of dir (and mode for types 0..4).
struct MovementVals {
    int mode = 1;                  // +0x68
    double dir[3] = {};            // +0x50 +0x58 +0x60
};
// @src 0x14142cb20  movement from D (mode by type; direction D+0x88/+0x90/+0x98)
void feToMovement(const FormingExpressProps& p, MovementVals& mv);
// @src 0x1408609b0  atan2(dir[1], dir[0]) * 57.29577951308232
double movementAngle(const MovementVals& mv);

// @src 0x1413fbd10  OB vt+0xc8 of SfOprExpressBase, SfOprFormingExpress* and SfOprHeatTransferExpress*
// (MDT DB-5, L4): step mode set to user defined for a never set-up operation, the "Keep Mesh of
// Dies?" question (answered Yes, documented deviation), then vt+0x2b8 (engine bodies: loader_l4.hpp)
void expressSyncFromModelTail(OpClass& c, Project& p, Ctx& ctx);

// SfOprFormingExpress / SfOprFormingExpress2D (ctor 0x14141bbe0 via 0x14141bb40 / 0x141412f90).
class FormingExpressClass : public OpClass {
public:
    FormingExpressClass(bool is2D);

    // @src 0x1414136b0 (3D) / 0x141412c90 (2D): base writer unless express
    void writeMst(W& w, Project& prj, const String& db, int* sim, Operation* op, int dbreadArg,
                  int onlySim) override;
    // @src 0x1413e5ef0  vt+0x200: true
    bool writesDimNstart() const override { return true; }
    // @src 0x14141cf10  vt+0x98: new SfOprFormingExpressProperties(vt+0x188)
    std::shared_ptr<Props> makeProps(int guiUnit) const override;
    // ---- MDT L3: own loader slots of SfOprFormingExpressCommon (bodies in src/engine/loader_l3.cpp);
    // vt+0x58, +0x1b8, +0x1f8 stay the base slots (0x1405ef430, 0x1405f6520, 0x1413e5ef0) ----------
    // vt+0xb8  @src 0x14141bc90
    void loadPrologue(Project& p, Operation& op) override { l3Slot(g_loaderSlotsL3.feLoadPrologue)(*this, p, op); }
    // vt+0x78  @src 0x14141d310: empty
    void loadClassData(Project&, Operation&) override {}
    // vt+0xc0  @src 0x14141bda0
    void postLoad(Project& p, Operation& op, Ctx& ctx, engine::FileEffects& fx) override
    {
        l3Slot(g_loaderSlotsL3.fePostLoad)(*this, p, op, ctx, fx);
    }
    // vt+0xc8  @src 0x1413fbd10 (MDT L4, DB-5)
    void syncFromModelTail(Project& p, Operation&, Ctx& ctx, engine::FileEffects&) override
    {
        expressSyncFromModelTail(*this, p, ctx);
    }
    // SfOprFormingExpressProperties vt+0x58  @src 0x14142a6a0
    void propsLoad(Project& p, Operation& op) override { l3Slot(g_loaderSlotsL3.fePropsLoad)(*this, p, op); }
    // @src 0x141423390  vt+0x298 (body in P14's simcount.cpp)
    void simList(int pos, int* counter, Operation* op, std::vector<SimEntry>& out) override;

    // @src 0x14141f2f0  the express driver
    void writeExpress(W& w, Project& prj, const String& db, int* sim, Operation* op, int dbreadArg,
                      int onlySim);
    // @src 0x14141fc50  block BL of (b, s)
    bool blockBL(W& w, Project& prj, const String& db, int* sim, Operation* op, int dbreadArg,
                 int onlySim, FormingExpressProps& p, int b, int s, const int* pos, bool* first,
                 const SimCtrlVals& sc, const MovementVals& mv, const String& equip, int wp, int top,
                 int bot, int nextSimNo);
    // @src 0x1414216e0  block DW of (b, s)
    bool blockDW(W& w, Project& prj, const String& db, int* sim, Operation* op, int dbreadArg,
                 int onlySim, FormingExpressProps& p, int b, int s, const int* pos, bool* first,
                 const MovementVals& mv, double angle, int wp, int top, int bot);
    // @src 0x141422320  block TO of (b, s)
    bool blockTO(W& w, Project& prj, const String& db, int* sim, Operation* op, int dbreadArg,
                 int onlySim, FormingExpressProps& p, int b, int s, const int* pos, bool* first);
    // @src 0x141423130  object section (mode 0x200010 BL, 0x100020 TO, 0x100010 / 0x100000)
    void writeObjects(W& w, Project& prj, const String& db, int n, Operation* op, int mode,
                      bool* first);

    // @src 0x141423f20 "BL%1", 0x141424130 "DW%1", 0x141423d10 "TO%1": (op name + "-") + tag<s>
    String simNameBL(Operation* op, int b, int s, bool shortName) const;
    String simNameDW(Operation* op, int b, int s, bool shortName) const;
    String simNameTO(Operation* op, int b, int s, bool shortName) const;

    // Object state (ctor 0x14141bbe0; +0x70..+0x90 written by the driver and DW).
    int wpNo = 1;                  // +0x60
    int topNo = 2;                 // +0x64
    int botNo = 3;                 // +0x68
    double moveDist = 1.0;         // +0x70
    double back[3] = {};           // +0x78 +0x80 +0x88
    bool dwWritten = false;        // +0x90
    double d98 = 0.0;              // +0x98

private:
    bool is2D_;
};

}  // namespace mst
}  // namespace m2m
