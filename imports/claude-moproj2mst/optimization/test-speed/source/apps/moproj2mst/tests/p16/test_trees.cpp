// P16: default property trees of Diestress (0x1402bb080) and DiestressBM (0x14029f0a0).
#include <testkit/check.hpp>

#include "m2m/mst/writer.hpp"
#include "m2m/props_base.hpp"
#include "m2m/props_tree.hpp"

using namespace m2m;
using qt::L;

namespace {

std::vector<std::string> names(const std::vector<PropNode>& v)
{
    std::vector<std::string> out;
    for (const auto& n : v) out.push_back(n.name.utf8());
    return out;
}

void checkEquipmentTree(const std::vector<PropNode>& t, const char* root)
{
    REQUIRE(t.size() == 1u);
    CHECK(t[0].name == L(root));
    CHECK(t[0].typeName == L("group"));
    REQUIRE(t[0].children.size() == 3u);
    CHECK((names(t[0].children) == std::vector<std::string>{"EquipmentType", "EquipmentSpec", "StepNumber"}));
    const PropNode& type = t[0].children[0];
    CHECK((names(type.children) == std::vector<std::string>{"Equipment", "EquipName"}));
    CHECK(type.children[0].typeName == L("enum"));
    CHECK_EQ(type.children[0].value.i, 1);
    CHECK(type.children[1].value.s == L("Undefined"));
    const PropNode& spec = t[0].children[1];          // mechanical press, crank (block defaults)
    CHECK((names(spec.children) ==
           std::vector<std::string>{"MechPressType", "TotalStroke", "CyclesSec", "ConnectingRodLength"}));
    CHECK(spec.children[1].value.d == 10.0);
    CHECK(spec.children[2].value.d == 0.6);
    CHECK(spec.children[3].value.d == 0.9);
    const PropNode& step = t[0].children[2];
    CHECK(step.typeName == L("int"));
    CHECK_EQ(step.value.i, -1);
}

}  // namespace

TEST_CASE(diestress_tree)
{
    checkEquipmentTree(defaultPropTree(L("SfOprDiestress")), "SfOprDiestress");
}

TEST_CASE(diestress_bm_trees)
{
    checkEquipmentTree(defaultPropTree(L("SfOprDiestressBM2D")), "SfOprDiestressBM2D");
    checkEquipmentTree(defaultPropTree(L("SfOprDiestressBM3D")), "SfOprDiestressBM3D");
}

TEST_CASE(make_props_adds_base_groups)
{
    for (int which = 0; which < 2; ++which) {
        auto cls = which == 0 ? mst::makeDiestress() : mst::makeDiestressBM(true);
        auto p = cls->makeProps(1);
        REQUIRE(p != nullptr);
        CHECK(!p->tree.empty());
        CHECK(p->tree[0].name == cls->key());
        CHECK(base::testGroup(*p) != nullptr);        // base adders (0x1405f9eb0 ...)
    }
}

int main()
{
    return testkit::run_all();
}
