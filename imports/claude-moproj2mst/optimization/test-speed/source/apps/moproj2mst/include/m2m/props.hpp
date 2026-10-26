// m2m/props.hpp: operation properties (SfOpr*Properties, "PROPS" = OPI+0x10) (seam).
//
// Ownership: the base (tree, owner link, factory of default trees) is P02's; post_load() bodies
// (vtable +0xc0, the data files read at open) are P06's; each class's data payload is declared by
// the packet that reads or writes it in include/m2m/data/<name>.hpp and held here through a
// shared_ptr to an incomplete type, so this header does not change when a payload grows.
//   data::CogModel ......... P06 (Cogging.MDT reader, s1-03 3/5), read by P11
//   data::BooleanData ...... P06 (Boolean.BWZ, s1-04 1.1), read by P12
//   data::ExpressData ...... P06 (Forming.FWZ, +0x38), read by P10/P15; P15 derives FormingExpressProps
//   data::CycleData ........ P06 (Cycle.CWZ count), read by P13
//   data::DiestressBMData .. P06 (DieStressBmWizard.Wzd), read by P16
//   data::MultiBlowData .... P14 (s1-04 7.3)
//   data::DistortionData ... P13 (s1-04 6)
//   data::HeatTreatData .... P16 (s1-05 4)
//   data::DiestressData .... P16 (s1-05 2)
//   data::HeatTransferStepData, data::HTFurnaceData .. MDT L2 (include/m2m/data/heattransfer.hpp)
//   data::CommonBlock ...... header by P15 (include/m2m/data/common.hpp, PROPS+0x30: FRCFAC, IHTCOF,
//                            DSMAX, DTMAX; s1-05 5.5); defaults and setters P02
#pragma once

#include <memory>
#include <vector>

#include "m2m/common.hpp"
#include "m2m/data/common.hpp"
#include "m2m/data/l6_blocks.hpp"
#include "m2m/qt4.hpp"
#include "m2m/xml.hpp"

namespace m2m {
using qt::String;
struct Operation;
struct Project;

namespace data {
struct CogModel;
struct BooleanData;
struct ExpressData;
struct CycleData;
struct DiestressBMData;
struct MultiBlowData;
struct DistortionData;
struct HeatTreatData;
struct DiestressData;
struct CommonBlock;
struct HeatTransferStepData;
struct HTFurnaceData;
struct PreData;
struct DoeOutputData;
struct ShapeOptimData;
}  // namespace data
}  // namespace m2m

namespace def::model {
class Cls_1403753f0;               // DEF_PRE furnace model (port/defmodel)
}

namespace m2m::mst {
class OpClass;
}

namespace m2m {

struct PropNode {                  // QtProperty in the class's property tree
    String name;
    String typeName;               // double, int, doubleUnits, enum, bool, group, ...
    qt::Variant value;
    int index = -1;                // SfProperty+0x48 (index of a Heat/Blow child; -1 = none)
    std::vector<PropNode> children;
};

struct Props {                     // SfOprBaseProperties
    virtual ~Props() = default;
    std::vector<PropNode> tree;    // +0x10 (top-level nodes; 0x140353770)
    Operation* owner = nullptr;    // +0x20 QPointer to the OPI (0x1405f8d90)
    // +0x30, made by the base ctor 0x1405f8bb0 -> 0x1405f8ca0 (new 0x68, 0x1405fd250) for every class,
    // with the SI defaults; base::initCommonBlock(*common, 2) gives the English ones.
    std::shared_ptr<data::CommonBlock> common = std::make_shared<data::CommonBlock>();

    // vtable +0xc0: post-load hook (0x140cac130 = nothing for the base class).
    virtual void postLoad(Project& p, Ctx& ctx);

    // Value of a property node set by P02's applyProperty (0x1400952c0 -> 0x140370600): classes
    // whose Qt slots copy property values into their data (MultiBlow P14, FormingExpress
    // 0x1414269a0 P15, ...) override this; the writer packet of the class owns the override.
    // `node` is the matched node (its index field selects the heat/blow for indexed setters),
    // `ancestors` the chain from the top-level node down to node's parent. A class may add or
    // remove children of node's parent, as setNumHeats/setNumBlows do; P02's applyProperty takes
    // its candidate list for the next level only when the original does (0x140370f60). Base: nothing.
    // Default (out of line, src/ops/props_base.cpp, P15): the SfOprBaseProperties set* slots
    // (base::applyBaseValue); subclasses fall back to it for names they do not own.
    virtual void applyValue(PropNode& node, const std::vector<PropNode*>& ancestors, const qt::Variant& v);

    // (props+0x10)->vt[+0x8], called by ParseProperties 0x140094f70 before the saved Properties
    // apply. Base @src 0x1403537f0: nothing; every class keeps it except MultiBlow (0x14060b950).
    virtual void beforeParse() {}

    // The value the .moproj writer puts in <Value> (0x14009fb40 -> 0x140370200): when the node's
    // template names a getter ("get<Name>", made by the template makers 0x1403738e0 & co.), the
    // GUI calls QMetaObject::invokeMethod(props, "get<Name>(SfProperty*)", Q_RETURN_ARG(QVariant,
    // node value)) and writes what the slot returns; without such a slot the stored value stays.
    // Returns true and sets `out` when the class has the slot for `node` (its name, and the
    // SfProperty+0x48 indices of the node and of its `ancestors`, top level first, as applyValue
    // gets them), false when it has none. Each class overrides it with its
    // qt_metacall get slots and falls back to base::getValue (SfOprBaseProperties 0x1405f84b0 ->
    // 0x1405f7b10, src/ops/props_base.cpp). The default throws M2M_E_UNSUPPORTED: a class whose
    // get slots are not ported does not write stored values in their place. (moproj-ops)
    // (inline so that the standalone test builds without src/model link)
    virtual bool getValue(const PropNode&, const std::vector<PropNode*>&, qt::Variant&)
    {
        throw Error(M2M_E_UNSUPPORTED, "project file: the get slots of this properties class are not ported "
                                       "(0x140370200)");
    }
};

namespace base {
// @src 0x1405f7b10  SfOprBaseProperties get slots (getNumSteps .. getTimePerStep); false for
// other names (src/ops/props_base.cpp, moproj-ops)
bool getValue(const Props& p, const PropNode& node, qt::Variant& out);
}  // namespace base

// dynamic_cast targets of the writers. One class per SfOpr*Properties the specs name.
struct CoggingProps : Props {      // SfOprCoggingProperties; +0x40 model
    std::shared_ptr<data::CogModel> model;
    void postLoad(Project& p, Ctx& ctx) override;   // @src 0x140760430 (P06)
};
struct BooleanProps : Props {      // SfOprBooleanProperties; +0x38 BooleanData
    std::shared_ptr<data::BooleanData> data;
    void postLoad(Project& p, Ctx& ctx) override;   // @src 0x1411e09d0 (P06)
    // the metaobject of this class has no own methods (qt_metacall 0x1411e4e90): the base get slots
    bool getValue(const PropNode& node, const std::vector<PropNode*>&, qt::Variant& out) override
    {
        return base::getValue(*this, node, out);
    }
};
struct ExpressProps : Props {      // SfOprExpressProperties (HeatTransferExpress, FormingExpress)
    std::shared_ptr<data::ExpressData> data;   // +0x38 Forming.FWZ values (P06); the FE data block
                                               // at +0x40 belongs to P15's FormingExpressProps
    void postLoad(Project& p, Ctx& ctx) override;   // @src 0x141402060 (P06)
    // the metaobject of this class has no own methods (qt_metacall 0x1414031c0): the base get slots
    bool getValue(const PropNode& node, const std::vector<PropNode*>&, qt::Variant& out) override
    {
        return base::getValue(*this, node, out);
    }
};
struct CycleProps : Props {        // SfOprCycleProperties; +0x38 count
    std::shared_ptr<data::CycleData> data;
    void postLoad(Project& p, Ctx& ctx) override;   // @src 0x14134ec40 (P06)
    // the metaobject of this class has no own methods (qt_metacall 0x14134f2c0): the base get slots
    bool getValue(const PropNode& node, const std::vector<PropNode*>&, qt::Variant& out) override
    {
        return base::getValue(*this, node, out);
    }
};
struct DiestressBMProps : Props {  // SfOprDiestressBMProperties; +0x38..
    std::shared_ptr<data::DiestressBMData> data;
    void postLoad(Project& p, Ctx& ctx) override;   // @src 0x14029eea0 (P06)
    // the metaobject of this class has no own methods (qt_metacall 0x1405f84b0, the base's): the base get slots
    bool getValue(const PropNode& node, const std::vector<PropNode*>&, qt::Variant& out) override
    {
        return base::getValue(*this, node, out);
    }
};
struct DiestressProps : Props {    // SfOprDiestressProperties
    std::shared_ptr<data::DiestressData> data;
    void postLoad(Project& p, Ctx& ctx) override;   // @src 0x1402b95a0 (P16, not followed in s1-01)
    // the metaobject of this class has no own methods (qt_metacall 0x1405f84b0, the base's): the base get slots
    bool getValue(const PropNode& node, const std::vector<PropNode*>&, qt::Variant& out) override
    {
        return base::getValue(*this, node, out);
    }
};
struct HeatTreatProps : Props {    // SfOprHeatTreatProperties (+0x58 sim-ctrl block, +0x68 schedule)
    std::shared_ptr<data::HeatTreatData> data;
    void postLoad(Project& p, Ctx& ctx) override;   // @src 0x1411c0940 (P16)
    // +0x58 sim-ctrl / property block (MDT L6, m2m/data/l6_blocks.hpp)
    std::shared_ptr<data::HTPropsBlock> block = std::make_shared<data::HTPropsBlock>();
    // qt_metacall 0x1411c4f80 -> qt_static_metacall 0x1411c41b0, then the base (L2,
    // src/model/props_heattreat_slots.cpp)
    void applyValue(PropNode& node, const std::vector<PropNode*>& ancestors, const qt::Variant& v) override;
    bool getValue(const PropNode& node, const std::vector<PropNode*>& ancestors, qt::Variant& out) override;
};
struct MultiBlowProps : Props {    // MultiBlow props; +0x38 MultiBlowData
    std::shared_ptr<data::MultiBlowData> data;
    // @src 0x14061ab80 (qt_metacall: the set* slots) (P14, src/ops/multiblow_props.cpp)
    void applyValue(PropNode& node, const std::vector<PropNode*>& ancestors, const qt::Variant& v) override;
    // @src 0x14060b950  UseRelativeEnergy (+0x88) and UseAdaptiveReheat (+0xe8) = false (P14)
    void beforeParse() override;
    // the get<Name> slots of qt_metacall 0x14061ab80, then the base ones (MDT L4, src/ops/multiblow_get.cpp)
    bool getValue(const PropNode& node, const std::vector<PropNode*>& ancestors, qt::Variant& out) override;
};
struct DistortionProps : Props {   // Distortion props
    std::shared_ptr<data::DistortionData> data;
    void postLoad(Project& p, Ctx& ctx) override;   // @src 0x141365c60 (P13)
    // +0x38 counts block, ctor 0x141369120 (MDT L6, m2m/data/l6_blocks.hpp)
    std::shared_ptr<data::DistortionCounts> counts = std::make_shared<data::DistortionCounts>();
    // qt_metacall 0x141371d60 -> qt_static_metacall 0x141371ad0 (NumFixtures, NumPasses), then the
    // base (L2, src/model/props_rest_slots.cpp)
    void applyValue(PropNode& node, const std::vector<PropNode*>& ancestors, const qt::Variant& v) override;
    bool getValue(const PropNode& node, const std::vector<PropNode*>& ancestors, qt::Variant& out) override;
};
// MDT L2: payloads the operation load fills (src/engine/loader_l2.cpp)
struct HeatTransferProps : Props { // SfOprHeatTransferProperties (ctor 0x141355f00)
    std::shared_ptr<data::HeatTransferStepData> step;     // +0x58 (0x141359a60)
    // qt_metacall 0x14135a7f0 -> qt_static_metacall 0x14135a240 set slots, then the base (L2)
    void applyValue(PropNode& node, const std::vector<PropNode*>& ancestors, const qt::Variant& v) override;
    // the get slots of 0x14135a240, then base::getValue (L2)
    bool getValue(const PropNode& node, const std::vector<PropNode*>& ancestors, qt::Variant& out) override;
};
// MDT L3: SfOprHeatTransferExpressProperties (ctor 0x1413eb5d0); the step data at +0x60 is made
// by 0x1413eb740 with the HeatTransfer data ctor 0x141359a60 and filled by the load's vt+0x68
struct HeatTransferExpressProps : ExpressProps {
    std::shared_ptr<data::HeatTransferStepData> step;     // +0x60
    // qt_metacall 0x1413f6510 -> qt_static_metacall 0x1413f5f40, then SfOprExpressProperties
    // 0x1414031c0 = the base (L2)
    void applyValue(PropNode& node, const std::vector<PropNode*>& ancestors, const qt::Variant& v) override;
    bool getValue(const PropNode& node, const std::vector<PropNode*>& ancestors, qt::Variant& out) override;
};
struct HTFurnaceProps : Props {    // SfOprHTFurnaceProperties (ctor 0x140c67820)
    std::shared_ptr<def::model::Cls_1403753f0> furnace;   // +0x38 (0x1409f00d0 = DEF_PRE 1403753f0)
    std::shared_ptr<data::HTFurnaceData> data;            // +0xd0 (new(0x40), 0x140c6a190)
    // qt_metacall 0x140c6bd20 -> qt_static_metacall 0x140c6b550, then the base (L2)
    void applyValue(PropNode& node, const std::vector<PropNode*>& ancestors, const qt::Variant& v) override;
    bool getValue(const PropNode& node, const std::vector<PropNode*>& ancestors, qt::Variant& out) override;
};
struct DoeOutputProps : Props {     // SfOprDoeOutputProperties (ctor 0x140e35ec0 -> 0x140e35f90)
    std::shared_ptr<data::DoeOutputData> data;            // +0x38
    // qt_metacall 0x140e3a830 -> qt_static_metacall 0x140e3a5a0, then the base (L2)
    void applyValue(PropNode& node, const std::vector<PropNode*>& ancestors, const qt::Variant& v) override;
    bool getValue(const PropNode& node, const std::vector<PropNode*>& ancestors, qt::Variant& out) override;
};
struct ShapeOptimProps : Props {   // SfOprShapeOptimProperties (ctor 0x14013be10 -> 0x140142720)
    std::shared_ptr<data::ShapeOptimData> data;           // +0x38
    mst::OpClass* cls = nullptr;   // +0x40 the SfOprShapeOptim whose vt+0x98 made it (0x140142620)
    // qt_metacall 0x14014b890 -> qt_static_metacall 0x14014b600, then the base (L2)
    void applyValue(PropNode& node, const std::vector<PropNode*>& ancestors, const qt::Variant& v) override;
    bool getValue(const PropNode& node, const std::vector<PropNode*>& ancestors, qt::Variant& out) override;
};
// SfOprGroupProperties (qt_metacall 0x141352fb0 = the base only) and SfOprReportProperties
// (0x1412b2260: one method, ButtonTestAction, no get slot): the base get slots (L2)
struct BaseSlotsProps : Props {
    bool getValue(const PropNode& node, const std::vector<PropNode*>&, qt::Variant& out) override
    {
        return base::getValue(*this, node, out);
    }
};
struct PreProps : Props {          // SfOprPreProperties (ctor 0x141349c50 / 0x141348810 -> 0x14134a880)
    std::shared_ptr<data::PreData> data;                  // +0x38 (new(0x48), 0x14134c570)
    // qt_metacall 0x14134d190 -> qt_static_metacall 0x14134c910, then the base (L2)
    void applyValue(PropNode& node, const std::vector<PropNode*>& ancestors, const qt::Variant& v) override;
    bool getValue(const PropNode& node, const std::vector<PropNode*>& ancestors, qt::Variant& out) override;
};

// @src OB->vt[0x98] per class: the default property tree of a class key (s1-01 Q10) (P02)
std::vector<PropNode> defaultPropTree(const String& classKey);
// @src 0x1400952c0  ApplyProperty: match by name (legacy rule), recurse, set the value through
// props.applyValue (0x140370600). The candidates are the children of the node at `path` (child
// indices from props.tree; empty = the top level). Nodes are re-resolved from the path after every
// slot call, so a slot may grow or shrink any level at its end. (P08, P02 split)
void applyProperty(Props& props, const xml::Node& e, const std::vector<int>& path);
// @src 0x140353770  top-level nodes of the property tree (P08)
std::vector<PropNode>& propRoots(Props& props);
// Find a node by name path ("Group/Name") in a tree; null if absent (helper, P02).
const PropNode* findProp(const std::vector<PropNode>& tree, const String& path);

}  // namespace m2m
