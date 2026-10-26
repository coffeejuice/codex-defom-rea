// P11: SfOprHeatTransfer(2D/Common) default props (0x141355f00 -> 0x141356060, builder 0x141356c60)
// and the dispatcher routes for Report / ShapeOptim.
#include <string>

#include <testkit/check.hpp>

#include "m2m/p17.hpp"
#include "m2m/props_tree.hpp"

using m2m::PropNode;
using m2m::qt::String;

static std::string dump(const std::vector<PropNode>& l, int depth = 0)
{
    std::string s;
    for (const PropNode& n : l) {
        s += std::string(static_cast<size_t>(depth) * 2, ' ') + n.name.utf8() + ":" + n.typeName.utf8() + "\n";
        s += dump(n.children, depth + 1);
    }
    return s;
}

TEST_CASE(heattransfer_tree)
{
    auto p = m2m::tree::defaultProps(String(u"SfOprHeatTransfer2D"), 1);
    const std::string got = dump(p->tree);
    CHECK_EQ(got.substr(0, std::string("HeatTrasnfer2D:group\n  HeatingTime:doubleUnits\n").size()),
             std::string("HeatTrasnfer2D:group\n  HeatingTime:doubleUnits\n"));
    CHECK(got.find("TestGroup:group\n  EnvTemp:doubleUnits\n  ConvectionCoef:doubleUnits\n") != std::string::npos);
    CHECK(got.find("StepDefinition:group\n  STEPDEF:enum\n  DTMAX:double\n  DTEMPMAX:double\n"
                   "  DTEMPLMT0:double\n  DTEMPLMT1:double\n  STPINC:int\n") != std::string::npos);
    CHECK(p->common->heatingTime.utf8() == "15.0 sec");
    CHECK(p->common->ihtc.utf8() == "1");

    auto q = m2m::tree::defaultProps(String(u"SfOprHeatTransfer"), 2);
    CHECK_EQ(q->tree[0].name.utf8(), std::string("HeatTrasnfer3D"));
    CHECK(q->common->ihtc.utf8() == "0.0003");
}

TEST_CASE(dispatch_report_shapeoptim)
{
    CHECK_EQ(dump(m2m::tree::defaultProps(String(u"SfOprReport"), 2)->tree), dump(m2m::reportProps(2)->tree));
    CHECK_EQ(dump(m2m::tree::defaultProps(String(u"SfOprShapeOptim"), 2)->tree),
             dump(m2m::shapeOptimProps(2)->tree));
}

int main() { return testkit::run_all(); }
