// CP142: CopyMirror writer applies Properties+0x60 before the writer's onlySim filter.
#include <testkit/check.hpp>
#include "../support/test_paths.hpp"

#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj_ctor/llobj_ctor.hpp>

#include "m2m/engine.hpp"
#include "m2m/model.hpp"
#include "m2m/p17.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/props.hpp"
#include "m2m/props_variant.hpp"
#include "m2m/qt4path.hpp"
#include "../../src/engine/engine_internal.hpp"

namespace m2m::mst {
void installLoaderSlots();
}

namespace {
namespace fs = std::filesystem;
using namespace m2m;

void openVdie42(Project& project, Ctx& ctx)
{
    const fs::path source = m2m_test::configuredPath("M2M_CORPUS") / "42_vdie" / "vdie.moproj";
    const qt::String file(source.u16string());
    const qt::String db = app::changeSuffix(file, qt::L(".DB"));
    initProject(project, qt::absolutePath(db), qt::completeBaseName(db));
    makeRegistry(project);
    REQUIRE(openProject(project, file, ctx));
    REQUIRE(project.openedOk);
    activateSubProject(project, 0, ctx);
    mst::installLoaderSlots();
}

void retagCopyMirror(Project& project, Operation& operation)
{
    if (!project.registry.count(qt::L("SfOprCopyMirror"))) {
        auto cls = mst::makeCopyMirror();
        mst::classInit(*cls);
        project.registry[cls->key()] = std::move(cls);
    }
    operation.classKey = qt::L("SfOprCopyMirror");
    operation.props = project.registry.at(operation.classKey)->makeProps(project.unit);
}

PropNode* childNamed(PropNode& parent, const char16_t* name)
{
    for (PropNode& child : parent.children)
        if (child.name == qt::String(name)) return &child;
    return nullptr;
}

}  // namespace

TEST_CASE(copymirror_writer_applies_properties_before_only_sim_filter)
{
    Project project;
    ProjectScope projectScope(project);
    Ctx ctx;
    openVdie42(project, ctx);

    Operation* operation = operationById(project, 4);
    REQUIRE(operation != nullptr);
    retagCopyMirror(project, *operation);
    mst::OpClass* cls = opBase(*operation);
    REQUIRE(cls != nullptr);
    cls->current = operation;

    engine::GuiModel* model = engine::opModel(project, *operation);
    REQUIRE(model != nullptr);
    model->reset();
    engine::FileEffects fx;
    cls->loadOperation(project, *operation, ctx, fx);

    model = engine::opModel(project, *operation);
    REQUIRE(model != nullptr);
    def::model::LLOBJ_DATA* object = model->object(1);
    REQUIRE(object != nullptr);
    REQUIRE(*engine::at<std::int32_t>(object, 0x0c) != 7);

    Props& props = *operation->props;
    REQUIRE(props.tree.size() == 1u);
    PropNode& root = props.tree.front();
    PropNode* axis = childNamed(root, u"DTUPAXIS");
    PropNode* conversion = childNamed(root, u"DTCONTYPE");
    PropNode* length = childNamed(root, u"DTEXTLEN");
    PropNode* start = childNamed(root, u"DTEXTSTLOC");
    REQUIRE(axis != nullptr && conversion != nullptr && length != nullptr && start != nullptr);
    props.applyValue(*axis, {}, pv::vInt(2));
    props.applyValue(*conversion, {}, pv::vInt(2));
    props.applyValue(*length, {}, pv::vDouble(23.5));
    props.applyValue(*start, {}, pv::vDouble(-2.25));

    mst::Stream stream{1252};
    mst::State state;
    mst::W writer{stream, state, ctx};
    const qt::String db = app::changeSuffix(project.path, qt::L(".DB"));
    int sim = 5;
    cls->writeMst(writer, project, db, &sim, operation, 0, 99);

    // Native 0x140e24e80 calls the properties+0x60 virtual before 0x140e24f60
    // increments the count and rejects a nonmatching onlySim block.
    CHECK_EQ(sim, 6);
    CHECK(stream.bytes().empty());
    CHECK_EQ(*engine::at<std::int32_t>(object, 0x2c38), 2);
    CHECK_EQ(*engine::at<std::int32_t>(object, 0x2be8), 2);
    CHECK(*engine::at<double>(object, 0x2c10) == 23.5);
    CHECK(*engine::at<double>(object, 0x2c18) == -2.25);
}

TEST_CASE(copymirror_private_save_missing_directory_precedes_plane_scan)
{
    // Candidate-only private class/model setup; public CopyMirror admission is unchanged.
    Project project;
    ProjectScope projectScope(project);
    const fs::path dir = m2m_test::scratchOutput(m2m_test::scratchRoot() /
        ("cm443_" + std::to_string(GetCurrentProcessId()) + "_" + std::to_string(GetTickCount64())));
    fs::create_directories(dir);
    const fs::path task = m2m_test::scratchOutput(dir / "task");
    REQUIRE(!fs::exists(task));
    project.path = qt::String((dir / "private.moproj").u16string());

    Operation operation;
    operation.project = &project;
    retagCopyMirror(project, operation);
    setTaskDirRel(operation, qt::L("task"));
    mst::OpClass* cls = opBase(operation);
    REQUIRE(cls != nullptr);
    cls->current = &operation;
    mst::installLoaderSlots();
    operation.engineModel = std::make_shared<engine::GuiModel>(nullptr, 3);
    engine::GuiModel* model = engine::opModel(project, operation);
    REQUIRE(model == operation.engineModel.get());
    REQUIRE(def::model::llobj_ctor::get_or_create(model->step.get(), 2) != nullptr);
    REQUIRE(model->objectCount() == 2);
    model->viewerMirrorPlanes = {1};
    const auto planes = model->viewerMirrorPlanes;

    // Native cm1.c:131-145 branches out on QFile::open failure before objectCount.
    // A missing parent must therefore bypass even a positive unsupported plane count.
    engine::FileEffects missing;
    CHECK(cls->saveAfterFiles(project, operation, missing));
    CHECK(missing.writes.empty());
    CHECK(missing.deletes.empty());
    CHECK(missing.mkdirs.empty());
    CHECK(model->viewerMirrorPlanes == planes);
    CHECK(!fs::exists(task));

    // With no viewer owner yet, the same early return must not grow its plane vector.
    model->viewerMirrorPlanes.clear();
    CHECK(cls->saveAfterFiles(project, operation, missing));
    CHECK(model->viewerMirrorPlanes.empty());
    CHECK(missing.writes.empty());
    model->viewerMirrorPlanes = planes;

    auto refusesPositivePlanes = [&](engine::FileEffects& fx) {
        bool refused = false;
        try {
            (void)cls->saveAfterFiles(project, operation, fx);
        } catch (const Error& e) {
            refused = e.code == M2M_E_UNSUPPORTED &&
                std::string(e.what()).find("MIRROR_MERGE lines") != std::string::npos;
        }
        CHECK(refused);
        CHECK(fx.writes.empty());
        CHECK(fx.deletes.empty());
        CHECK(model->viewerMirrorPlanes == planes);
    };

    // FileEffects directories count as existing, without creating a physical task folder.
    engine::FileEffects planned;
    planned.mkdirs.push_back(qt::cleanPath(qt::String(task.u16string())));
    refusesPositivePlanes(planned);
    REQUIRE(planned.mkdirs.size() == 1u);
    CHECK(!fs::exists(task));

    // An existing parent keeps the prior unsupported positive-plane boundary.
    fs::create_directory(task);
    engine::FileEffects existing;
    refusesPositivePlanes(existing);
    CHECK(existing.mkdirs.empty());
    CHECK(!fs::exists(task / "mirrormerge.ini"));
    fs::remove(task);
    fs::remove(dir);
}

int main()
{
    return testkit::run_all();
}


