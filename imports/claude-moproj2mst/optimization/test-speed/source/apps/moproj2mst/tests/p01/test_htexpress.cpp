// P01: SfOprHeatTransferExpress(2D) default properties (0x1413eb5d0 builder chain) and their use by
// the .moproj reader on the SfOprAirConvect2D operation of 46_MO2_Boolean_Oprations.
#include <testkit/check.hpp>

#include <cstdlib>
#include <filesystem>

#include "m2m/data/common.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/props_tree.hpp"
#include "m2m/qt4path.hpp"

using m2m::qt::String;

namespace {
const m2m::PropNode* kid(const std::vector<m2m::PropNode>& l, const char16_t* n)
{
    for (const auto& x : l)
        if (x.name.u16() == n) return &x;
    return nullptr;
}
}  // namespace

TEST_CASE(default_tree_2d)
{
    auto p = m2m::tree::heatTransferExpressProps(String(u"SfOprHeatTransferExpress2D"), 1);
    REQUIRE(p != nullptr);
    CHECK(dynamic_cast<m2m::ExpressProps*>(p.get()) != nullptr);
    REQUIRE(p->tree.size() == 3u);
    CHECK(p->tree[0].name == String(u"HeatTrasnferExpress2D"));
    CHECK(p->tree[1].name == String(u"TestGroup"));
    CHECK(p->tree[2].name == String(u"StepDefinition"));
    REQUIRE(p->tree[0].children.size() == 1u);
    CHECK(p->tree[0].children[0].name == String(u"HeatingTime"));
    CHECK(kid(p->tree[1].children, u"EnvTemp") != nullptr);
    CHECK(kid(p->tree[1].children, u"ConvectionCoef") != nullptr);
    const char16_t* order[] = {u"STEPDEF", u"DTMAX", u"DTEMPMAX", u"DTEMPLMT0", u"DTEMPLMT1", u"STPINC"};
    REQUIRE(p->tree[2].children.size() == 6u);
    for (int i = 0; i < 6; ++i) CHECK(p->tree[2].children[static_cast<size_t>(i)].name == String(order[i]));
    CHECK_EQ(p->tree[2].children[0].value.i, 1);
    CHECK_BITS(p->tree[2].children[1].value.d, 0.001);
    CHECK_BITS(p->tree[2].children[2].value.d, 2.0);
    CHECK_BITS(p->tree[2].children[4].value.d, 10.0);
    CHECK_EQ(p->tree[2].children[5].value.i, 10);
    CHECK(p->common->heatingTime == String(u"3600.0 sec"));
    auto p3 = m2m::tree::heatTransferExpressProps(String(u"SfOprHeatTransferExpress"), 1);
    CHECK(p3->tree[0].name == String(u"HeatTrasnferExpress3D"));
}

TEST_CASE(corpus_air_convect_values_applied)
{
    const char* c = std::getenv("M2M_CORPUS");
    REQUIRE(c != nullptr);
    const auto f = std::filesystem::path(c) / "46_MO2_Boolean_Oprations" / "MO2_Boolean_Oprations.moproj";
    std::u16string u;
    for (wchar_t ch : f.generic_wstring()) u += static_cast<char16_t>(ch);
    const String file(u);
    m2m::Project p;
    m2m::Ctx ctx;
    const String db = m2m::app::changeSuffix(file, m2m::qt::L(".DB"));
    m2m::initProject(p, m2m::qt::absolutePath(db), m2m::qt::completeBaseName(db));
    m2m::makeRegistry(p);
    REQUIRE(m2m::openProject(p, file, ctx) && p.openedOk);
    m2m::Operation* ac = nullptr;
    for (auto& o : p.operations)
        if (o->subType == String(u"SfOprAirConvect2D")) ac = o.get();
    REQUIRE(ac != nullptr);
    CHECK(ac->classKey == String(u"SfOprHeatTransferExpress2D"));
    REQUIRE(ac->props != nullptr);
    const auto& t = ac->props->tree;
    REQUIRE(t.size() == 3u);
    CHECK(t[0].children[0].value.s == String(u"720 sec"));             // via the legacy name rule
    CHECK(kid(t[1].children, u"EnvTemp")->value.s == String(u"2192 F"));
    CHECK_BITS(t[2].children[1].value.d, 3.6);                          // DTMAX
    CHECK_EQ(t[2].children[5].value.i, 20);                             // STPINC
}

int main() { return testkit::run_all(); }
