// ctest-labels: m2m-mdt mdt-l6 m2m-mdt-l7
#include <testkit/check.hpp>

#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "m2m/common.hpp"
#include "m2m/doe_output_data.hpp"
#include "m2m/doe_session.hpp"
#include "m2m/doe_task_runtime.hpp"
#include "m2m/doe_task_factory.hpp"
#include "m2m/doe_stepdata.hpp"
#include "m2m/doe_study.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine_scene.hpp"
#include "m2m/mst/writer.hpp" //complete OpClass for local Project destruction

using namespace m2m;
using qt::L;
namespace {
bool refuses(const auto& action, const char* contains)
{
    try { action(); }
    catch (const Error& error) {
        return error.code == M2M_E_UNSUPPORTED && std::string(error.what()).find(contains) != std::string::npos;
    }
    return false;
}
struct Fixture {
    std::shared_ptr<doe::TaskAllocationStore> allocations = std::make_shared<doe::TaskAllocationStore>();
    std::shared_ptr<doe::TaskFactoryRuntime> taskProjection;
    Project project;
    Operation operation;
    doe::OutputClassState data;
    std::uint8_t flags[6]{1, 2, 3, 4, 5, 6};
    std::vector<std::int32_t> numbers{77};
    std::vector<std::uint64_t> pointers{99};
    Fixture()
    {
        project.path = L("C:/PROBLEM/DOEUnit"); //recording-only declared unit path
        project.doeIo = doe::makeRecordingStudyIo();
        project.doeIo->bindTaskAllocationStore(allocations);
        taskProjection = doe::bindBoundedParsedDoeProjection(*project.doeIo, allocations);
        project.subs.push_back(makeSubProject(project, 2, 1));
        makeRegistry(project);
        operation.project = &project;
        operation.id = 1;
        operation.classKey = L("SfOprDoeOutput");
        operation.opClass = findClass(project, operation.classKey);
        REQUIRE(operation.opClass != nullptr);
        operation.opClass->current = &operation;
        study().objNo = 25;
        study().oprNo = 9;
        study().roiType = 7;
        study().commonMeshPath = L("prior.key");
        study().commonMeshOrigName = L("prior-name");
    }
    doe::Study& study() { return *project.subs[0]->ownedDoeStudy; }
    doe::OutputClassRefs refs() { return {data, flags, numbers, pointers}; }
    void document(const std::string& task = "<SfTask><ObjList><Object No='-2'/><Object No='3'/></ObjList>"
        "<OprList><Opr No='17'/><Opr No='18'/></OprList></SfTask>")
    {
        study().outputDocument = std::make_shared<doe::SessionDocument>();
        auto& doc = *study().outputDocument;
        qt::String error;
        int line = 0, column = 0;
        REQUIRE(xml::setContent(doc.dom, "<PartRoot><SessionRoot><Session TaskType='SfTaskDoe'>" +
            task + "</Session></SessionRoot></PartRoot>", &error, &line, &column));
        doc.partRoot = doc.dom.root;
        doe::readSessionRecords(doc, {[] {}, {}, {}, project.doeIo.get()});
    }
    engine::GuiModel* model()
    {
        auto* model = engine::opModel(project, operation);
        REQUIRE(model != nullptr);
        return model;
    }
};
} // namespace

TEST_CASE(doe_output_empty_regions_constraints_finish_first_scalar_copies_and_exact_clears)
{
    Fixture f;
    ProjectScope scope(f.project);
    f.document();
    auto* model = f.model();
    model->state100 = 77;
    model->doeStepData = std::make_shared<doe::StepDataState>();
    const auto table = model->doeStepData;
    table->propertyTable = std::make_unique<doe::PropertyTableState>(*model);
    const auto dirs = f.project.doeIo->directories().size();
    doe::finishCurrentOutputClassData(f.project, f.operation, f.refs());
    CHECK(f.study().objNo == -2 && f.study().oprNo == 17); //first selections, no clamp
    CHECK(f.data.objectNo && *f.data.objectNo == -2);
    CHECK(f.data.roiType && *f.data.roiType == 7); //copied before Study reset to0
    CHECK(f.data.meshPath && *f.data.meshPath == L("prior.key"));
    CHECK(f.study().roiType == 0 && f.study().commonMeshPath.isEmpty());
    CHECK(f.study().commonMeshOrigName.isEmpty());
    CHECK(model->state100 == 0 && model->doeStepData == table && !table->propertyTableCreated());
    REQUIRE(f.data.regionNames);
    CHECK(f.data.regionNames->empty() && f.numbers.empty() && f.pointers.empty());
    for (int index = 0; index < 4; ++index) CHECK(f.flags[index] == 0);
    CHECK(f.flags[4] == 5 && f.flags[5] == 6);
    CHECK(f.project.doeIo->directories().size() == dirs && f.project.doeIo->textWrites().empty());
}

TEST_CASE(doe_output_roi_one_rewrites_study_paths_after_class_copy_and_resets_actual_roi_model)
{
    Fixture f;
    ProjectScope scope(f.project);
    f.document(""); //fresh Task's recovered default ObjList contains0
    auto* main = f.model();
    engine::displayShow(f.project, main);
    auto* roi = engine::displayNamedModel(*main->display, L("ROIs"));
    REQUIRE(roi != nullptr && roi != main);
    main->state100 = 33;
    roi->state100 = 88;
    roi->doeStepData = std::make_shared<doe::StepDataState>();
    roi->doeStepData->propertyTable = std::make_unique<doe::PropertyTableState>(*roi);
    f.study().roiType = 1;
    doe::finishCurrentOutputClassData(f.project, f.operation, f.refs());
    CHECK(f.study().objNo == 0 && f.study().oprNo == 9);
    CHECK(f.data.roiType && *f.data.roiType == 1);
    CHECK(f.data.meshPath && *f.data.meshPath == L("prior.key"));
    CHECK(f.study().roiType == 1 && f.study().commonMeshPath == L("Regions/Common_Mesh.Key"));
    CHECK(f.study().commonMeshOrigName.isEmpty());
    CHECK(main->state100 == 33 && roi->state100 == 0 && roi->f25c == 2);
    CHECK(!roi->doeStepData->propertyTableCreated());
}

TEST_CASE(doe_output_empty_selection_lists_retain_prior_class_and_study_scalar_values)
{
    Fixture f;
    ProjectScope scope(f.project);
    f.document("<SfTask><ObjList/><OprList/></SfTask>");
    f.model();
    f.data.objectNo = 42;
    f.data.roiType = 3;
    f.data.meshPath = L("cached.key");
    doe::finishCurrentOutputClassData(f.project, f.operation, f.refs());
    CHECK(f.study().objNo == 25 && f.study().oprNo == 9);
    CHECK(f.data.objectNo && *f.data.objectNo == 42);
    CHECK(f.data.roiType && *f.data.roiType == 3);
    CHECK(f.data.meshPath && *f.data.meshPath == L("cached.key"));
    CHECK(f.study().roiType == 0 && f.data.regionNames && f.data.regionNames->empty());
}

TEST_CASE(doe_output_missing_type_guards_before_default_creation_but_matching_null_task_returns)
{
    for (const bool matchedType : {false, true}) {
        Fixture f;
        ProjectScope scope(f.project);
        f.document();
        auto& doc = *f.study().outputDocument;
        REQUIRE(doc.parts[0].sessions[0] != nullptr);
        auto& record = *doc.parts[0].sessions[0];
        if (!matchedType) record.taskType = L("Unregistered");
        record.task.reset(); //declared source helper-entry state, not allocation-fault parity
        auto* model = f.model();
        model->state100 = 77;
        if (matchedType) doe::finishCurrentOutputClassData(f.project, f.operation, f.refs());
        else CHECK(refuses([&] { doe::finishCurrentOutputClassData(f.project, f.operation, f.refs()); },
            "missing SfTaskDoe default construction"));
        CHECK(doc.parts[0].sessions.size() == 1 && model->state100 == 77);
        CHECK(f.study().objNo == 25 && f.study().roiType == 7);
        CHECK(!f.data.objectNo && !f.data.regionNames);
        CHECK(f.flags[0] == 1 && f.numbers.size() == 1 && f.pointers.size() == 1);
    }
}

TEST_CASE(doe_output_first_region_guard_retains_name_and_reset_before_constraint_clears)
{
    Fixture f;
    ProjectScope scope(f.project);
    f.document();
    auto* model = f.model();
    model->state100 = 77;
    f.study().regions = {L("first"), L("second")};
    CHECK(refuses([&] { doe::finishCurrentOutputClassData(f.project, f.operation, f.refs()); },
        "region geometry payload"));
    CHECK(model->state100 == 0 && f.study().objNo == -2 && f.study().oprNo == 17);
    REQUIRE(f.data.regionNames && f.data.regionNames->size() == 1);
    CHECK(f.data.regionNames->front() == L("first"));
    CHECK(f.study().roiType == 7 && f.study().commonMeshPath == L("prior.key"));
    CHECK(f.flags[0] == 1 && f.flags[3] == 4 && f.numbers.size() == 1 && f.pointers.size() == 1);
    CHECK(f.project.doeIo->textWrites().empty());
}

TEST_CASE(doe_output_constraint_guard_retains_zero_region_rewrite_and_only_four_flag_clears)
{
    Fixture f;
    ProjectScope scope(f.project);
    f.document();
    f.model();
    f.study().roiType = 1;
    f.study().constraints = 1; //payload itself remains unavailable
    CHECK(refuses([&] { doe::finishCurrentOutputClassData(f.project, f.operation, f.refs()); },
        "constraint payload"));
    CHECK(f.study().commonMeshPath == L("Regions/Common_Mesh.Key"));
    REQUIRE(f.data.regionNames);
    CHECK(f.data.regionNames->empty() && f.numbers.empty() && f.pointers.empty());
    for (int index = 0; index < 4; ++index) CHECK(f.flags[index] == 0);
    CHECK(f.flags[4] == 5 && f.flags[5] == 6);
}

int main() { return testkit::run_all(); }
