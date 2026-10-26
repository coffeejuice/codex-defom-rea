// MDT L4: SfOprShapeOptimProperties' class link. vt+0x98 0x14013be10 makes the props with the class
// at +0x40 (0x140142620); props vt+0x60 0x140142d00 copies data +8 / +0x10 into the class's +0xcc /
// +0xe0, and props vt+0x58 0x140142ca0 copies them back (expected values: the copies by hand).
// ctest-labels: m2m-mdt m2m-mdt-l4
#include <testkit/check.hpp>

#include <memory>

#include "m2m/data/heattransfer.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/props.hpp"

using namespace m2m;

TEST_CASE(shapeoptim_props_class_link)
{
    std::unique_ptr<mst::OpClass> ob = mst::makeShapeOptim();
    Project p;
    auto a = std::dynamic_pointer_cast<ShapeOptimProps>(ob->makeProps(1));
    REQUIRE(a != nullptr);
    CHECK(a->cls == ob.get());
    CHECK_EQ(a->data->maxSims, 200);                    // 0x140142d90
    a->data->maxSims = 7;
    a->data->minObjFuncVal = 0.25;
    Operation opA;
    opA.props = a;
    CHECK(ob->propsToEngine(p, opA));                   // 0x140142d00: class +0xcc = 7, +0xe0 = 0.25

    auto b = std::dynamic_pointer_cast<ShapeOptimProps>(ob->makeProps(1));
    REQUIRE(b != nullptr);
    CHECK_EQ(b->data->maxSims, 200);
    Operation opB;
    opB.props = b;
    ob->propsLoad(p, opB);                              // 0x140142ca0
    CHECK_EQ(b->data->maxSims, 7);
    CHECK(b->data->minObjFuncVal == 0.25);

    // props without the class link (made outside vt+0x98): both copies do nothing
    auto c = std::make_shared<ShapeOptimProps>();
    c->data = std::make_shared<data::ShapeOptimData>();
    Operation opC;
    opC.props = c;
    ob->propsLoad(p, opC);
    CHECK_EQ(c->data->maxSims, 200);
}

int main()
{
    return testkit::run_all();
}
