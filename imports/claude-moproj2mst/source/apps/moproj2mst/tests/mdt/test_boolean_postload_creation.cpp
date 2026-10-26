#include "../support/test_paths.hpp"
// ctest-labels: m2m-mdt
// Synthetic admitted operation state through actual Open/load lifecycle calls.
// This is postLoad11df0e9 creation, not Apply2955/2bc6 or a timer observation.
#include <testkit/check.hpp>
#include <process.h>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <defmodel/layout/llobj_data.hpp>
#include "m2m/data/boolean.hpp"
#include "m2m/engine.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/props.hpp"
#include "m2m/qt4path.hpp"
#include "../../src/engine/engine_internal.hpp"

namespace fs = std::filesystem;
using namespace m2m;
namespace {
struct Fixture {
    qt::CurrentDirScope keepDirectory{qt::currentPath()};
    Project project;
    ProjectScope scope{project};
    Ctx context;
    engine::FileEffects effects;
    fs::path root, input;
    Operation* operation = nullptr;
    engine::GuiModel* model = nullptr;
    std::shared_ptr<BooleanProps> properties;

    explicit Fixture(bool nonzeroSlot)
    {
        // Distinguish reruns when Windows reuses a PID; sequence separates
        // fixtures within this run without removing prior run artifacts.
        static const auto runTick = std::chrono::steady_clock::now().time_since_epoch().count();
        static unsigned sequence = 0;
        root = m2m_test::scratchOutput(m2m_test::scratchRoot() /
            ("b102_" + std::to_string(_getpid()) + "_" + std::to_string(runTick) +
             "_" + std::to_string(++sequence)));
        REQUIRE(!fs::exists(root));
        fs::create_directories(root);
        input = root / "test.moproj";
        {
            std::ofstream file(input, std::ios::binary);
            REQUIRE(file.is_open());
            file << "<Project><Unit Name=\"SI\"/></Project>";
        }
        initProject(project, qt::String(root.u16string()), qt::L("test"));
        makeRegistry(project);
        REQUIRE(openProject(project, qt::String(input.u16string()), context));
        REQUIRE(project.openedOk);
        activateSubProject(project, 0, context);

        // Declare the selected operation after the real XML-read prefix. No
        // fixture corpus is mutated and no Qt/current-editor state is invented.
        operation = createOperation(project, qt::L("Boolean"), qt::L("SfOprBoolean2D"), {}, 1);
        REQUIRE(operation != nullptr);
        operation->oprFlag = 1; // declared batch-load fixture, no setup dialog
        properties = std::dynamic_pointer_cast<BooleanProps>(operation->props);
        REQUIRE(properties && properties->data);
        properties->data->workpiece = 77;
        properties->data->cutter = 88;
        properties->data->remesh = 1;
        operation->engineModel = std::make_shared<engine::GuiModel>(nullptr, 2);
        model = operation->engineModel.get();
        REQUIRE(model->objectCount() == 0);

        auto* sub = activeSubProject(project);
        REQUIRE(sub != nullptr);
        auto item = std::make_unique<FlowItem>();
        item->opId = operation->id;
        sub->flow.items.push_back(std::move(item));
        sub->flow.activeItem = 0;
        sub->flow.numberCacheBuilt = false;
        if (nonzeroSlot) {
            // -99 is a real skip-slot code in loadFromKey. There is one OPI
            // record but no engine object:061220 and model count are distinct.
            operation->objIds.push_back(-99);
            auto entry = std::make_unique<ObjEntry>();
            entry->number = 1;
            operation->objs.push_back(std::move(entry));
        }
    }

    void openOperation()
    {
        // Actual active-flow lookup -> actual class+58/baseLoad -> real Boolean
        // prologue/loadData/postLoad -> propsLoad/views. No direct postLoad call,
        // callback driver, declared due list or synthetic queue is substituted.
        CHECK(engine::openActiveOperation(project, context, effects) == operation);
        CHECK(opBase(*operation)->current == operation);
        CHECK(operation->engineModel.get() == model);
    }
    void unchangedBooleanValues() const
    {
        CHECK_EQ(properties->data->workpiece, 77);
        CHECK_EQ(properties->data->cutter, 88);
        CHECK_EQ(properties->data->remesh, 1);
    }
};
}

TEST_CASE(boolean_open_lifecycle_zero_opi_slots_creates_one_kind5_without_apply)
{
    Fixture f(false);
    REQUIRE(f.operation->objs.empty() && f.operation->objIds.empty());
    f.openOperation();
    REQUIRE(f.model->objectCount() == 1);
    REQUIRE(f.operation->objs.size() == 1u && f.operation->objIds.size() == 1u);
    auto* object = f.model->object(1);
    REQUIRE(object != nullptr);
    CHECK_EQ(*engine::at<std::int32_t>(object, 0xc), 5);
    CHECK_EQ(f.operation->objs[0]->objectType, 5);
    f.unchangedBooleanValues();
    // A Boolean Apply of this filled missing77/88 selection would create more
    // objects/update data. This lifecycle prefix performs only postLoad's add.
    CHECK(f.project.openedOk);
}

TEST_CASE(boolean_open_lifecycle_nonzero_opi_slot_with_zero_engine_objects_does_not_fallback)
{
    Fixture f(true);
    REQUIRE(f.operation->objs.size() == 1u && f.operation->objIds.size() == 1u);
    REQUIRE(f.model->objectCount() == 0);
    f.openOperation();
    CHECK_EQ(f.model->objectCount(), 0);
    CHECK(f.model->object(1) == nullptr);
    CHECK(f.operation->objs.size() == 1u && f.operation->objIds.size() == 1u);
    CHECK_EQ(f.operation->objIds[0], -99);
    f.unchangedBooleanValues();
}

int main() { return testkit::run_all(); }
