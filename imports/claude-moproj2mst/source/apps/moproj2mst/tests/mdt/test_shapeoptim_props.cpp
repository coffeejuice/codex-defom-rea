// MDT L4: SfOprShapeOptimProperties' class link. vt+0x98 0x14013be10 makes the props with the class
// at +0x40 (0x140142620); props vt+0x60 0x140142d00 copies data +8 / +0x10 into the class's +0xcc /
// +0xe0, and props vt+0x58 0x140142ca0 copies them back (expected values: the copies by hand).
// ctest-labels: m2m-mdt m2m-mdt-l4
#include <testkit/check.hpp>

#include <memory>

#include "m2m/data/heattransfer.hpp"
#include "m2m/engine.hpp"
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

TEST_CASE(shapeoptim_save_current_copies_nondefault_properties_to_class)
{
    Project p;
    p.subs.push_back(makeSubProject(p, 0, 0));
    p.activeSub = 0;

    // ShapeOptim's lazy model calls dim(), which selects the predecessor of the
    // current operation in the active subproject's flow. Give that lookup a
    // real, numbered 3D predecessor, as the editor does for an optimization
    // step attached to an existing operation.
    auto predecessorClass = mst::makePre(false);
    const auto predecessorKey = predecessorClass->key();
    p.registry.emplace(predecessorKey, std::move(predecessorClass));
    auto owned = mst::makeShapeOptim();
    auto* cls = owned.get();
    const auto key = cls->key();
    p.registry.emplace(key, std::move(owned));

    auto predecessor = std::make_unique<Operation>();
    predecessor->project = &p;
    predecessor->id = 1;
    predecessor->classKey = predecessorKey;
    predecessor->opClass = p.registry.at(predecessorKey).get();
    auto opOwner = std::make_unique<Operation>();
    Operation& op = *opOwner;
    op.project = &p;
    op.id = 2;
    op.classKey = key;
    op.opClass = cls;

    auto predecessorItem = std::make_unique<FlowItem>();
    predecessorItem->opId = predecessor->id;
    predecessorItem->col = 0;
    auto currentItem = std::make_unique<FlowItem>();
    currentItem->opId = op.id;
    currentItem->col = 1;
    predecessorItem->next = currentItem.get();
    currentItem->prev = predecessorItem.get();
    p.subs[0]->flow.items.push_back(std::move(predecessorItem));
    p.subs[0]->flow.items.push_back(std::move(currentItem));
    p.subs[0]->flow.activeItem = 1;

    p.operations.push_back(std::move(predecessor));
    p.operations.push_back(std::move(opOwner));
    cls->current = &op;
    auto source = std::dynamic_pointer_cast<ShapeOptimProps>(cls->makeProps(1));
    REQUIRE(source != nullptr);
    REQUIRE(source->data != nullptr);
    source->data->maxSims = 37;
    source->data->minObjFuncVal = 0.125;
    op.props = source;

    Ctx ctx;
    engine::FileEffects effects;
    engine::saveCurrentOperation(p, op, ctx, effects);

    // Read the real class fields back through the reverse property slot. This
    // proves Save-current reached vt+0x60, rather than only exercising the
    // class copy helper directly.
    auto roundTrip = std::dynamic_pointer_cast<ShapeOptimProps>(cls->makeProps(1));
    REQUIRE(roundTrip != nullptr);
    REQUIRE(roundTrip->data != nullptr);
    op.props = roundTrip;
    cls->propsLoad(p, op);
    CHECK_EQ(roundTrip->data->maxSims, 37);              // class +0xcc
    CHECK(roundTrip->data->minObjFuncVal == 0.125);     // class +0xe0

    cls->current = nullptr;
}

int main()
{
    return testkit::run_all();
}
