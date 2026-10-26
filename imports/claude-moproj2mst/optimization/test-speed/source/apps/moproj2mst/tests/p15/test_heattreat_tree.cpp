// ctest-labels: m2m-p15
// Tier-1 test of the HeatTreat default property tree (0x1411bd890 templates, 0x1411bf050 builder),
// links m2m_core.
#include <testkit/check.hpp>

#include <string>

#include "m2m/props_tree.hpp"

using namespace m2m;

TEST_CASE(heattreat_tree_shape)
{
    auto t = tree::treeHeatTreat(qt::L("SfOprHeatTreat2D"));
    REQUIRE(t.size() == 1);
    CHECK(t[0].name.utf8() == "HTWizard");
    std::string names;
    for (auto& c : t[0].children) names += c.name.utf8() + ",";
    CHECK(names == "Mode,Thermal Schedule,StepDefinition,");
    names.clear();
    for (auto& c : t[0].children[0].children) names += c.name.utf8() + ",";
    CHECK(names == "PhaseTransformation,Diffusion,Deformation,");
    const PropNode& step = t[0].children[2];
    names.clear();
    for (auto& c : step.children) names += c.name.utf8() + ",";
    CHECK(names == "STEPDEF,DTMAX,DTEMPMAX,DTEMPLMT0,DTEMPLMT1,STPINC,");
    CHECK(step.children[1].value.d == 0.001);
    CHECK(step.children[5].value.i == 10);
}

int main() { return testkit::run_all(); }
