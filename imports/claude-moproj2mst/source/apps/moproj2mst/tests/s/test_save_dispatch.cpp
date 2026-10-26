// ctest-labels: m2m-s
// Source-derived dispatch ordering, independently observable through class
// callbacks. See original 1405ef510,1405f1e20 and ShapeOptim14013d6f0.
#include <testkit/check.hpp>
#include "m2m/engine.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/props.hpp"
#include "m2m/data/heattransfer.hpp"

using namespace m2m;
namespace {
struct ObservedClass final : mst::OpClass {
    explicit ObservedClass(bool shown, const char16_t* key = u"SfOprBoolean3D")
        : OpClass(String(key)), shown(shown) {}
    bool shown;
    int properties = 0, model = 0;
    int shapeMaxSims = -1;
    double shapeMinObjFuncVal = -1.0;
    std::vector<std::string> calls;
    bool updateDataInWidgetArea(Project&, Operation&, engine::FileEffects&) override {
        calls.push_back("editor-index");
        if (shown) { calls.push_back("apply"); properties = 7; }
        return shown;
    }
    void propsLoad(Project&, Operation&) override {
        calls.push_back("props-load");
        ++properties;
    }
    bool propsToEngine(Project&, Operation& op) override {
        calls.push_back("props-to-model");
        model = properties;
        if (const auto* shape = dynamic_cast<const ShapeOptimProps*>(op.props.get());
            shape && shape->data) {
            shapeMaxSims = shape->data->maxSims;
            shapeMinObjFuncVal = shape->data->minObjFuncVal;
        }
        return true;
    }
    void saveNames(Project&, Operation&, engine::FileEffects&) override { calls.push_back("names"); }
    bool saveOperationFiles(Project&, Operation&, engine::FileEffects&) override {
        calls.push_back("files"); return true;
    }
    bool saveAfterFiles(Project&, Operation&, engine::FileEffects&) override {
        calls.push_back("after-files"); return true;
    }
};
struct SaveObservation {
    std::vector<std::string> calls;
    int model;
    int shapeMaxSims;
    double shapeMinObjFuncVal;
};
SaveObservation save(bool shown, int properties = 0, const char16_t* key = u"SfOprBoolean3D") {
    Project p;
    Operation op;
    auto owner = std::make_unique<ObservedClass>(shown, key);
    auto& cls = *owner;
    cls.properties = properties;
    op.classKey = cls.key(); op.project = &p; op.opClass = &cls;
    p.registry.emplace(cls.key(), std::move(owner));
    if (key == u"SfOprShapeOptim") {
        auto props = std::make_shared<ShapeOptimProps>();
        props->data = std::make_shared<data::ShapeOptimData>();
        props->data->maxSims = 37;
        props->data->minObjFuncVal = 0.125;
        op.props = std::move(props);
    } else {
        op.props = std::make_shared<BooleanProps>();
    }
    cls.current = &op;
    Ctx ctx;
    engine::FileEffects effects;
    engine::saveCurrentOperation(p, op, ctx, effects);
    cls.current = nullptr;
    return {cls.calls, cls.model, cls.shapeMaxSims, cls.shapeMinObjFuncVal};
}
}

TEST_CASE(save_shown_editor_refresh_is_copied_to_model_before_names_and_files)
{
    const auto cls = save(true);
    CHECK(cls.calls == std::vector<std::string>({"editor-index", "apply", "props-load",
        "props-to-model", "names", "files", "after-files"}));
    CHECK_EQ(cls.model, 8);
}

TEST_CASE(save_index_zero_preserves_two_property_to_model_calls)
{
    const auto cls = save(false, 11);
    CHECK(cls.calls == std::vector<std::string>({"editor-index", "props-to-model",
        "props-to-model", "names", "files", "after-files"}));
    CHECK_EQ(cls.model, 11);
}

TEST_CASE(save_shapeoptim_override_does_not_inherit_outer_base_copy)
{
    const auto cls = save(true, 0, u"SfOprShapeOptim");
    CHECK(cls.calls == std::vector<std::string>({"editor-index", "apply", "props-load",
        "after-files", "files"}));
    CHECK_EQ(cls.model, 0);
}

TEST_CASE(save_shapeoptim_without_editor_applies_shape_properties_before_save_slots)
{
    const auto cls = save(false, 0, u"SfOprShapeOptim");
    CHECK(cls.calls == std::vector<std::string>({"editor-index", "props-to-model",
        "after-files", "files"}));
    CHECK_EQ(cls.shapeMaxSims, 37);
    CHECK_EQ(cls.shapeMinObjFuncVal, 0.125);
}

TEST_CASE(save_shapeoptim_with_editor_loads_properties_and_skips_direct_model_copy)
{
    const auto cls = save(true, 0, u"SfOprShapeOptim");
    CHECK(cls.calls == std::vector<std::string>({"editor-index", "apply", "props-load",
        "after-files", "files"}));
    CHECK_EQ(cls.shapeMaxSims, -1);
    CHECK_EQ(cls.shapeMinObjFuncVal, -1.0);
}

int main() { return testkit::run_all(); }
