// ctest-labels: m2m-mdt mdt-l6 m2m-mdt-l7
#include <testkit/check.hpp>
#include "../support/test_paths.hpp"

#include <fstream>
#include <functional>
#include <map>

#include "m2m/doe_session.hpp"
#include "m2m/doe_session_notifications.hpp"
#include "m2m/doe_task_factory.hpp"
#include "m2m/doe_study.hpp"
#include "m2m/doe_stepdata.hpp"
#include "m2m/doe_statevar.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine_scene.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/qt4path.hpp"

using namespace m2m;
using qt::L;

namespace {
const std::string document = "<!DOCTYPE SessionFile><PartRoot><SessionRoot OprStartNo='1' "
    "OprEndNo='5'><Session TaskType='SfTaskDoe'><StepData/><SfTask><OprList/>"
    "<ObjList/><OutputList/></SfTask></Session></SessionRoot></PartRoot>";

class ReadIo final : public doe::StudyIo {
public:
    ReadIo() : StudyIo(doe::IoMode::Recording)
    {
        bindTaskAllocationStore(taskAllocations);
        taskFactory = doe::bindBoundedParsedDoeProjection(*this, taskAllocations);
    }
    //Explicit test scope owner; production uses ConversionSession's same store.
    std::shared_ptr<doe::TaskAllocationStore> taskAllocations = std::make_shared<doe::TaskAllocationStore>();
    std::shared_ptr<doe::TaskFactoryRuntime> taskFactory;
    std::map<String, std::string> files;
    std::map<String, std::optional<std::uint64_t>> sizes;
    bool readSucceeded = true;
    std::function<void()> beforeRead;
    std::vector<std::string> order;
    bool ensureDirectory(const String& path) override
    { return recordDirectory(path, false, true, true); }
    bool readFile(const String& path, std::string& bytes) override
    {
        const auto found = files.find(qt::cleanPath(path));
        bytes = found == files.end() ? "" : found->second;
        return recordRead(path, bytes, found != files.end(), found != files.end());
    }
    class Handle final : public doe::StudyReadFile {
    public:
        Handle(ReadIo& owner, std::size_t index, std::string bytes,
               std::optional<std::uint64_t> size)
            : owner_(owner), index_(index), bytes_(std::move(bytes)), size_(size) {}
        std::optional<std::uint64_t> size() override
        {
            owner_.order.push_back("size");
            owner_.recordReadSize(index_, size_);
            return size_;
        }
        std::string readAll() override
        {
            owner_.order.push_back("read");
            if (owner_.beforeRead) owner_.beforeRead();
            owner_.recordReadData(index_, bytes_, owner_.readSucceeded);
            return bytes_;
        }
    private:
        ReadIo& owner_;
        std::size_t index_;
        std::string bytes_;
        std::optional<std::uint64_t> size_;
    };
    std::unique_ptr<doe::StudyReadFile> openReadFile(const String& path) override
    {
        order.push_back("open");
        const auto key = qt::cleanPath(path);
        const auto found = files.find(key);
        const auto index = recordReadOpen(path, found != files.end());
        if (found == files.end()) return {};
        const auto overrideSize = sizes.find(key);
        const auto size = overrideSize == sizes.end()
            ? std::optional<std::uint64_t>(found->second.size()) : overrideSize->second;
        return std::make_unique<Handle>(*this, index, found->second, size);
    }
};

bool refuses(const auto& action, const char* contains)
{
    try { action(); }
    catch (const Error& error) {
        return error.code == M2M_E_UNSUPPORTED &&
               std::string(error.what()).find(contains) != std::string::npos;
    }
    return false;
}

struct Fixture {
    Project project;
    std::shared_ptr<ReadIo> io = std::make_shared<ReadIo>();
    Operation operation;
    mst::OpClass* cls = nullptr;
    const String path = L("C:/doe-unit-only/DoeStudies/Existing/Control/Case.DS");

    Fixture()
    {
        project.path = L("C:/doe-unit-only/Case");
        project.doeIo = io;
        project.subs.push_back(makeSubProject(project, 2, 1));
        project.subs[0]->ownedDoeStudy->name = L("Existing");
        makeRegistry(project);
        operation.project = &project;
        operation.id = 1;
        operation.classKey = L("SfOprDoeOutput");
        cls = findClass(project, operation.classKey);
        REQUIRE(cls != nullptr);
        operation.opClass = cls;
        cls->current = &operation;
    }

    doe::Study& study() { return *project.subs[0]->ownedDoeStudy; }
};

doe::SessionDocument parsedSession(const std::string& bytes)
{
    ReadIo io;
    io.files[L("records.DS")] = bytes;
    doe::SessionDocument state;
    doe::readSessionDocument(state, L("records.DS"), io);
    return state;
}
}  // namespace

TEST_CASE(doe_session_reset_replaces_partroot_without_replacing_document_type)
{
    doe::SessionDocument state;
    CHECK(state.filename.isNull());
    REQUIRE(state.dom.root != nullptr);
    CHECK(state.dom.doctypeName == L("SessionFile"));
    CHECK(state.partRoot == state.dom.root);
    const auto old = state.partRoot;
    old->children.push_back(xml::createElement(L("SessionRoot")));
    state.filename = L("prior.DS");
    state.dom.doctypeName = L("PreservedType");
    doe::resetSessionDocument(state);
    CHECK(state.filename.isNull());
    CHECK(state.partRoot != old && state.partRoot == state.dom.root);
    CHECK(state.partRoot->tag == L("PartRoot") && state.partRoot->children.empty());
    CHECK(state.dom.doctypeName == L("PreservedType"));
}

TEST_CASE(doe_session_failed_open_and_empty_file_keep_prior_dom_but_remember_path)
{
    ReadIo io;
    doe::SessionDocument state;
    const auto prior = state.dom.root;
    prior->children.push_back(xml::createElement(L("Prior")));
    doe::readSessionDocument(state, L("missing.DS"), io);
    CHECK(state.filename == L("missing.DS") && state.dom.root == prior);
    CHECK(state.dom.root->children.size() == 1);
    io.files[L("empty.DS")] = "";
    doe::readSessionDocument(state, L("empty.DS"), io);
    CHECK(state.filename == L("empty.DS") && state.dom.root == prior);
    REQUIRE(io.reads().size() == 2);
    CHECK(!io.reads()[0].opened && io.reads()[1].opened);
    CHECK(!io.reads()[0].sizeObserved && !io.reads()[0].readAttempted);
    CHECK(io.reads()[1].sizeObserved && io.reads()[1].sizeBeforeRead == 0);
    CHECK(!io.reads()[1].readAttempted);
}

TEST_CASE(doe_session_known_size_controls_discard_before_read_not_returned_bytes)
{
    ReadIo io;
    io.files[L("short.DS")] = ""; // injected failure after a nonzero open-handle size
    io.sizes[L("short.DS")] = 17;
    io.readSucceeded = false;
    doe::SessionDocument state;
    const auto prior = state.dom.root;
    prior->children.push_back(xml::createElement(L("Prior")));
    io.beforeRead = [&] {
        CHECK(state.dom.root != prior);
        CHECK(state.partRoot == state.dom.root && state.partRoot->children.empty());
    };
    CHECK(refuses([&] { doe::readSessionDocument(state, L("short.DS"), io); }, "malformed XML"));
    CHECK(io.order == std::vector<std::string>({"open", "size", "read"}));
    REQUIRE(io.reads().size() == 1);
    CHECK(io.reads()[0].sizeBeforeRead == 17 && io.reads()[0].readAttempted);
    CHECK(!io.reads()[0].readSucceeded && io.reads()[0].bytes.empty());

    ReadIo zero;
    zero.files[L("growing.DS")] = document;
    zero.sizes[L("growing.DS")] = 0; // no read even if later bytes would be available
    const auto afterFailure = state.dom.root;
    doe::readSessionDocument(state, L("growing.DS"), zero);
    CHECK(state.dom.root == afterFailure);
    CHECK(zero.order == std::vector<std::string>({"open", "size"}));
    CHECK(!zero.reads()[0].readAttempted);
}

TEST_CASE(doe_session_unknown_handle_size_guards_without_guessing_from_bytes)
{
    ReadIo io;
    io.files[L("unknown.DS")] = document;
    io.sizes[L("unknown.DS")] = std::nullopt;
    doe::SessionDocument state;
    const auto prior = state.dom.root;
    CHECK(refuses([&] { doe::readSessionDocument(state, L("unknown.DS"), io); }, "size is not available"));
    CHECK(state.filename == L("unknown.DS") && state.dom.root == prior);
    CHECK(io.order == std::vector<std::string>({"open", "size"}));
    CHECK(io.reads()[0].sizeObserved && !io.reads()[0].sizeBeforeRead);
    CHECK(!io.reads()[0].readAttempted);
}

TEST_CASE(doe_session_native_and_recording_handles_observe_size_and_read_separately)
{
    namespace fs = std::filesystem;
    const auto root = m2m_test::scratchOutput(m2m_test::scratchRoot() /
        ("doe-session-handle-" + std::to_string(GetCurrentProcessId()) + "-" +
         std::to_string(GetTickCount64())));
    REQUIRE(!fs::exists(root));
    fs::create_directories(root);
    const auto path = root / "existing.DS";
    { std::ofstream file(path, std::ios::binary); file << document; }
    auto io = doe::makeNativeStudyIo();
    auto file = io->openReadFile(String(path.u16string()));
    REQUIRE(file != nullptr);
    CHECK(!io->reads()[0].sizeObserved && !io->reads()[0].readAttempted);
    CHECK(file->size() == document.size());
    CHECK(io->reads()[0].sizeObserved && !io->reads()[0].readAttempted);
    // Shipped ReadOnly shares READ|WRITE but not DELETE. A conflicting exclusive
    // open must fail while this same handle supplies both size and bytes.
    const HANDLE conflicting = CreateFileW(path.c_str(), GENERIC_WRITE, 0, nullptr,
                                           OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    CHECK(conflicting == INVALID_HANDLE_VALUE);
    if (conflicting != INVALID_HANDLE_VALUE) CloseHandle(conflicting);
    CHECK(file->readAll() == document);
    CHECK(io->reads()[0].readAttempted && io->reads()[0].readSucceeded);
    file.reset();
    CHECK(!io->openReadFile(String((root / "absent" / "missing.DS").u16string())));
    CHECK(!fs::exists(root / "absent"));

    const auto emptyPath = root / "empty.DS";
    { std::ofstream empty(emptyPath, std::ios::binary); }
    doe::SessionDocument state;
    const auto prior = state.dom.root;
    doe::readSessionDocument(state, String(emptyPath.u16string()), *io);
    CHECK(state.dom.root == prior && !io->reads().back().readAttempted);

    auto recording = doe::makeRecordingStudyIo();
    const auto simulatedPath = root / "simulated.DS";
    CHECK(recording->writeTextFile(String(simulatedPath.u16string()), document));
    auto simulated = recording->openReadFile(qt::cleanPath(String(simulatedPath.u16string())));
    REQUIRE(simulated != nullptr);
    CHECK(simulated->size() == document.size());
    CHECK(simulated->readAll() == document);
    CHECK(!fs::exists(simulatedPath));
    CHECK(recording->reads()[0].sizeObserved && recording->reads()[0].readAttempted);
}

TEST_CASE(doe_session_nonempty_read_replaces_document_and_retains_input_task_children)
{
    ReadIo io;
    io.files[L("valid.DS")] = document;
    doe::SessionDocument state;
    const auto prior = state.dom.root;
    doe::readSessionDocument(state, L("valid.DS"), io);
    CHECK(state.filename == L("valid.DS") && state.dom.root != prior);
    CHECK(state.dom.root == state.partRoot && state.partRoot->tag == L("PartRoot"));
    const auto parts = state.partRoot->childElements(L("SessionRoot"));
    REQUIRE(parts.size() == 1);
    CHECK(parts[0]->attribute(L("OprEndNo")) == L("5"));
    const auto sessions = parts[0]->childElements(L("Session"));
    REQUIRE(sessions.size() == 1);
    CHECK(sessions[0]->childElements(L("StepData")).size() == 1);
    CHECK(sessions[0]->childElements(L("SfTask")).size() == 1);
    // Retention alone is not evidence that these task/model payloads were applied.
}

TEST_CASE(doe_session_malformed_and_cr_guards_follow_old_document_discard)
{
    for (const auto& bytes : {std::string("<PartRoot><broken>"), std::string("<PartRoot/>\r\n")}) {
        ReadIo io;
        io.files[L("bad.DS")] = bytes;
        doe::SessionDocument state;
        const auto prior = state.dom.root;
        CHECK(refuses([&] { doe::readSessionDocument(state, L("bad.DS"), io); }, "DOE session"));
        CHECK(state.filename == L("bad.DS") && state.dom.root != prior);
        CHECK(state.partRoot == state.dom.root);
        // Only the preceding destructive replacement is claimed. The candidate
        // guard does not invent Qt's possible partial malformed document contents.
    }
    doe::SessionDocument foreign;
    foreign.dom.root = xml::createElement(L("Foreign"));
    foreign.filename = L("before.DS");
    CHECK(refuses([&] { doe::resetSessionDocument(foreign); }, "non-PartRoot"));
    CHECK(foreign.filename.isNull() && foreign.dom.root->tag == L("Foreign"));
}

TEST_CASE(doe_session_real_class_data_bridge_finishes_supported_existing_task_data)
{
    Fixture f;
    ProjectScope scope(f.project);
    // opModel installs the real loader callbacks; this is no original application
    // execution and does not open a DB or run a study.
    REQUIRE(engine::opModel(f.project, f.operation) != nullptr);
    f.io->files[f.path] = document;
    f.cls->loadClassData(f.project, f.operation);
    REQUIRE(f.study().outputDocument != nullptr);
    const auto owned = f.study().outputDocument;
    CHECK(qt::cleanPath(owned->filename) == f.path);
    CHECK(owned->partRoot->childElements(L("SessionRoot")).size() == 1);
    REQUIRE(owned->parts.size() == 1);
    CHECK(owned->parts[0].operationEnd == 5 && owned->parts[0].operationStart == 1);
    REQUIRE(f.io->reads().size() == 1);
    CHECK(qt::cleanPath(f.io->reads()[0].path) == f.path);
    f.io->files[f.path] = "";
    CHECK(refuses([&] { f.cls->loadClassData(f.project, f.operation); }, "0x1406469d0"));
    CHECK(f.study().outputDocument == owned);
    CHECK(owned->partRoot->children.empty()); // reset precedes even an empty read
    CHECK(owned->parts.empty()); //644440 clears prior records before fallback guard
    CHECK(f.io->textWrites().empty());
}

TEST_CASE(doe_session_load_operation_reset_and_show_precede_document_guard)
{
    Fixture f;
    ProjectScope scope(f.project);
    auto* model = engine::opModel(f.project, f.operation);
    REQUIRE(model != nullptr);
    model->state100 = 77;
    CHECK(model->display == nullptr);
    f.io->files[f.path] = document;
    Ctx ctx;
    engine::FileEffects effects;
    CHECK(refuses([&] { f.cls->loadOperation(f.project, f.operation, ctx, effects); }, "selected session/global task application"));
    CHECK(model->state100 == 0);
    CHECK(model->display != nullptr && f.project.display != nullptr);
    REQUIRE(model->doeStepData != nullptr);
    CHECK(model->doeStepData->propertyTableCreated());
    REQUIRE(f.study().outputDocument != nullptr);
    CHECK(qt::cleanPath(f.study().outputDocument->filename) == f.path);
    CHECK(f.io->reads().size() == 1 && f.io->textWrites().empty());
    CHECK(effects.writes.empty() && effects.deletes.empty());
}

TEST_CASE(doe_session_part_defaults_and_present_short_attributes_precede_payload_guard)
{
    ReadIo io;
    auto state = parsedSession("<PartRoot><SessionRoot OprStartNo='-32768' OprEndNo='32767' "
        "PartEntityNo='-2' ExportSiesta='-1' ExportAnsys='32768'><Session/></SessionRoot></PartRoot>");
    CHECK(refuses([&] { doe::readSessionRecords(state, {{}, {}, {}, &io}); }, "0x140650e60"));
    REQUIRE(state.parts.size() == 1);
    const auto& part = state.parts[0];
    CHECK(part.operationStart == -32768 && part.operationEnd == 32767 && part.entityNo == -2);
    CHECK(part.displayName == L("$OPR_RANGE") && !part.displayName.isNull());
    CHECK(part.exportSiesta && !part.exportAnsys);
    CHECK(part.currentSession == 0 && part.root != nullptr);
    REQUIRE(part.sessions.size() == 1);
    CHECK(part.sessions[0]->task == nullptr); //missing TaskType is an absent registry key
    CHECK(part.root->childElements(L("Session")).size() == 1);
}

TEST_CASE(doe_session_part_invalid_shorts_zero_and_explicit_empty_display_is_preserved)
{
    auto state = parsedSession("<PartRoot><SessionRoot OprStartNo='32768' OprEndNo='-32769' "
        "PartEntityNo='garbage' DisplayName='' ExportSiesta='65535' ExportAnsys='2'/></PartRoot>");
    CHECK(refuses([&] { doe::readSessionRecords(state); }, "signalChangeStepRange"));
    REQUIRE(state.parts.size() == 1);
    const auto& part = state.parts[0];
    CHECK(part.operationStart == 0 && part.operationEnd == 0 && part.entityNo == 0);
    CHECK(part.displayName.isEmpty() && !part.displayName.isNull());
    CHECK(!part.exportSiesta && part.exportAnsys);
}

TEST_CASE(doe_session_part_descendants_follow_dom_order_and_root_branch_excludes_nested_matches)
{
    auto state = parsedSession("<PartRoot><Wrap><SessionRoot PartEntityNo='4'/>"
        "<Wrap><SessionRoot PartEntityNo='5'/></Wrap></Wrap>"
        "<SessionRoot PartEntityNo='6'><SessionRoot PartEntityNo='7'/></SessionRoot></PartRoot>");
    CHECK(refuses([&] { doe::readSessionRecords(state); }, "signalChangeStepRange"));
    REQUIRE(state.parts.size() == 4);
    for (int index = 0; index != 4; ++index) CHECK(state.parts[index].entityNo == index + 4);

    auto direct = parsedSession("<SessionRoot PartEntityNo='8'><SessionRoot PartEntityNo='9'/></SessionRoot>");
    CHECK(refuses([&] { doe::readSessionRecords(direct); }, "signalChangeStepRange"));
    REQUIRE(direct.parts.size() == 1);
    CHECK(direct.parts[0].entityNo == 8);
}

TEST_CASE(doe_session_part_clear_append_and_metadata_survive_first_encounter_guard)
{
    ReadIo io;
    auto state = parsedSession("<PartRoot><SessionRoot PartEntityNo='1'/>"
        "<SessionRoot PartEntityNo='2' DisplayName='second'><Ignored/><Session/></SessionRoot>"
        "<SessionRoot PartEntityNo='3'/></PartRoot>");
    state.parts.emplace_back();
    state.parts[0].entityNo = 99;
    CHECK(refuses([&] { doe::readSessionRecords(state, {{}, {}, {}, &io}); }, "0x140650e60"));
    REQUIRE(state.parts.size() == 2);
    CHECK(state.parts[0].entityNo == 1 && state.parts[1].entityNo == 2);
    CHECK(state.parts[1].displayName == L("second"));
    CHECK(state.parts[1].root != nullptr);
    const auto oldRoot = state.parts[1].root;
    REQUIRE(!oldRoot->children.empty());
    state.partRoot = xml::createElement(L("PartRoot"));
    CHECK(refuses([&] { doe::readSessionRecords(state, {{}, {}, {}, &io}); }, "0x1406469d0"));
    CHECK(state.parts.empty()); // no invented default part or DOM append at this guard
    CHECK(state.partRoot->children.empty());
    CHECK(oldRoot->children.empty()); //64f8e0 removes raw children before deletion
}

TEST_CASE(doe_session_part_unknown_children_skip_but_dblist_and_reportlist_are_guarded)
{
    for (const auto& tag : {std::string("DBLIST"), std::string("ReportList")}) {
        auto state = parsedSession("<PartRoot><SessionRoot PartEntityNo='3'>"
            "text<!--comment--><Unknown/><" + tag + "/></SessionRoot>"
            "<SessionRoot PartEntityNo='4'/></PartRoot>");
        CHECK(refuses([&] { doe::readSessionRecords(state); }, "DBLIST/ReportList"));
        REQUIRE(state.parts.size() == 1);
        CHECK(state.parts[0].entityNo == 3 && state.parts[0].root != nullptr);
    }
}

TEST_CASE(doe_session_task_factory_creates_distinct_doe_states_before_record_payload)
{
    auto state = parsedSession("<PartRoot><SessionRoot>"
        "<Session TaskType='SfTaskDoe'/><Session TaskType='SfTaskDoe'/><Session/>"
        "</SessionRoot></PartRoot>");
    int clears = 0;
    ReadIo io;
    doe::SessionImportContext context{[&] {
        ++clears;
        REQUIRE(state.parts.size() == 1);
        CHECK(state.parts[0].currentSession == clears - 1);
        CHECK(state.parts[0].sessions.size() == static_cast<std::size_t>(clears));
    }, {}, {}, &io};
    doe::readSessionRecords(state, context);
    REQUIRE(state.parts[0].sessions.size() == 3);
    const auto& records = state.parts[0].sessions;
    REQUIRE(records[0]->task != nullptr && records[1]->task != nullptr);
    CHECK(records[0]->task.get() != records[1]->task.get());
    CHECK(records[0]->task->sessionBacklink() == records[0]);
    CHECK(records[1]->task->sessionBacklink() == records[1]);
    CHECK(io.taskAllocations->retainedTaskCount() == 2);
    CHECK(records[0]->task->lists.operations.empty());
    CHECK(records[0]->task->lists.objects == std::vector<int>({0}));
    CHECK(!records[0]->task->executionPreferencesRecovered);
    CHECK(records[2]->task == nullptr && records[2]->taskType.isNull());
    CHECK(clears == 3);
}

TEST_CASE(doe_session_record_metadata_conversions_follow_int_to_short_storage)
{
    auto state = parsedSession("<PartRoot><SessionRoot><Session TaskType='SfTaskDoe' Title='name' "
        "Type='32768' Subtype='-2' DB_ID='-32769' Steps='3' StepNumber='2147483647' "
        "Objects='40000' EntityNo='40000'/><Session/></SessionRoot></PartRoot>");
    ReadIo io;
    doe::readSessionRecords(state, {[] {}, {}, {}, &io});
    REQUIRE(state.parts[0].sessions.size() == 2);
    const auto& first = state.parts[0].sessions[0];
    CHECK(first->title == L("name") && first->type == 0 && first->subtype == -2);
    CHECK(first->databaseId == 0 && first->steps == 3 && first->stepNumber == 2147483647);
    CHECK(first->objects == -25536 && first->entityNo == 40000);
    const auto& absent = state.parts[0].sessions[1];
    CHECK(absent->title.isNull() && absent->databaseId == 0 && absent->stepNumber == 0);
    CHECK(absent->objects == 0 && absent->entityNo == 0);
}

TEST_CASE(doe_session_task_list_effects_follow_child_order_and_stop_at_stepdata)
{
    auto state = parsedSession("<PartRoot><SessionRoot><Session TaskType='SfTaskDoe'>"
        "<SfDispView><Unsupported/></SfDispView><GraphicsItems><Unsupported/></GraphicsItems>"
        "<SfTask><ObjList><Object No='4'/></ObjList></SfTask>"
        "<StepData/><SfTask><ObjList><Object No='9'/></ObjList></SfTask>"
        "</Session><Session TaskType='SfTaskDoe'/></SessionRoot></PartRoot>");
    int clears = 0;
    ReadIo io;
    CHECK(refuses([&] { doe::readSessionRecords(state, {[&] { ++clears; }, {}, {}, &io}); }, "StepData"));
    REQUIRE(state.parts[0].sessions.size() == 1);
    REQUIRE(state.parts[0].sessions[0]->task != nullptr);
    CHECK(state.parts[0].sessions[0]->task->lists.objects == std::vector<int>({4}));
    CHECK(state.parts[0].currentSession == 0 && clears == 1);
}

TEST_CASE(doe_session_missing_task_skips_task_payload_but_known_other_class_guards)
{
    ReadIo io;
    auto state = parsedSession("<PartRoot><SessionRoot><Session TaskType='NotRegistered'>"
        "<StepData><Unknown/></StepData><SfTask><OutputList><Unknown/></OutputList></SfTask>"
        "</Session><Session TaskType='Custom'/><Session TaskType='SfTaskDoe'/></SessionRoot></PartRoot>");
    int clears = 0;
    CHECK(refuses([&] { doe::readSessionRecords(state, {[&] { ++clears; }, {}, {}, &io}); }, "registered non-DOE"));
    REQUIRE(state.parts[0].sessions.size() == 1);
    CHECK(state.parts[0].sessions[0]->task == nullptr && clears == 1);
    auto layered = parsedSession("<PartRoot><SessionRoot><Session><StepData LayerName='x'/>"
        "</Session></SessionRoot></PartRoot>");
    CHECK(refuses([&] { doe::readSessionRecords(layered, {[] {}, {}, {}, &io}); }, "StepData"));
    REQUIRE(layered.parts[0].sessions.size() == 1);
    CHECK(layered.parts[0].sessions[0]->task == nullptr);
}

TEST_CASE(doe_session_image_guard_precedes_record_creation_and_user_steps_keep_empty_parts)
{
    for (const auto& image : {std::string(""), std::string("001122")}) {
        auto state = parsedSession("<PartRoot><SessionRoot><Session TaskType='SfTaskDoe'>"
            "<UserDefinedStepList>1,,2,1,</UserDefinedStepList>"
            "<UserDefinedStepList>99</UserDefinedStepList></Session>"
            "<Session TaskType='SfTaskDoe'><Image>" + image + "</Image></Session>"
            "</SessionRoot></PartRoot>");
        int clears = 0;
        ReadIo io;
        CHECK(refuses([&] { doe::readSessionRecords(state, {[&] { ++clears; }, {}, {}, &io}); }, "Image qUncompress"));
        REQUIRE(state.parts[0].sessions.size() == 1);
        CHECK(state.parts[0].sessions[0]->userDefinedSteps == std::vector<int>({1, 0, 2, 1, 0}));
        CHECK(state.parts[0].currentSession == 0 && clears == 1);
        CHECK(io.taskAllocations->retainedSessionCount() == 1); //later Image precedes allocation
    }
    auto empty = parsedSession("<PartRoot><SessionRoot><Session><UserDefinedStepList/>"
        "</Session></SessionRoot></PartRoot>");
    ReadIo emptyIo;
    doe::readSessionRecords(empty, {[] {}, {}, {}, &emptyIo});
    CHECK(empty.parts[0].sessions[0]->userDefinedSteps == std::vector<int>({0}));
}

TEST_CASE(doe_session_populated_output_guard_retains_appended_record_and_prior_lists)
{
    auto state = parsedSession("<PartRoot><SessionRoot><Session TaskType='SfTaskDoe'><SfTask>"
        "<OprList><Opr No='5'/></OprList><ObjList><Object No='2'/><Object No='2'/></ObjList>"
        "<OutputList><Unsupported/></OutputList></SfTask></Session></SessionRoot></PartRoot>");
    ReadIo io;
    CHECK(refuses([&] { doe::readSessionRecords(state, {[] {}, {}, {}, &io}); }, "OutputList"));
    REQUIRE(state.parts[0].sessions.size() == 1);
    const auto& lists = state.parts[0].sessions[0]->task->lists;
    REQUIRE(lists.operations.size() == 1);
    CHECK(lists.operations[0].no == 5 && lists.objects == std::vector<int>({2}));
    CHECK(state.parts[0].currentSession == 0);
}

TEST_CASE(doe_session_real_bridge_clears_named_models_then_finalizes_display_main)
{
    Fixture f;
    ProjectScope scope(f.project);
    auto* model = engine::opModel(f.project, f.operation);
    REQUIRE(model != nullptr);
    engine::displayShow(f.project, model);
    auto& display = engine::projectDisplay(f.project);
    REQUIRE(engine::displayNamedModel(display, L("ROIs")) != nullptr);
    REQUIRE(engine::displayNamedModel(display, L("Coupons")) != nullptr);
    CHECK(engine::displayModels(display).named.size() == 2);
    f.io->files[f.path] = document;
    f.cls->loadClassData(f.project, f.operation);
    REQUIRE(engine::displayModels(display).named.size() == 1);
    CHECK(engine::displayModels(display).named[0].name == L("ROIs"));
    CHECK(engine::displayMain(display) == model);
    REQUIRE(model->doeStepData != nullptr);
    CHECK(model->doeStepData->propertyTableCreated());
    REQUIRE(f.study().outputDocument->parts.size() == 1);
    REQUIRE(f.study().outputDocument->parts[0].sessions.size() == 1);
    CHECK(f.study().outputDocument->parts[0].sessions[0]->task != nullptr);
}

TEST_CASE(doe_session_stepdata_targets_display_main_and_task_lists_follow_it)
{
    for (const bool showMain : {false, true}) {
        Fixture f;
        ProjectScope scope(f.project);
        auto* model = engine::opModel(f.project, f.operation);
        REQUIRE(model != nullptr);
        if (showMain) engine::displayShow(f.project, model);
        f.io->files[f.path] = "<PartRoot><SessionRoot><Session TaskType='SfTaskDoe'>"
            "<StepData/><SfTask><ObjList><Object No='4'/></ObjList></SfTask>"
            "</Session></SessionRoot></PartRoot>";
        f.cls->loadClassData(f.project, f.operation);
        REQUIRE(f.study().outputDocument->parts.size() == 1);
        const auto& part = f.study().outputDocument->parts[0];
        REQUIRE(part.sessions.size() == 1 && part.sessions[0]->task != nullptr);
        CHECK(part.sessions[0]->task->lists.objects == std::vector<int>({4}));
        CHECK(static_cast<bool>(model->doeStepData) == showMain);
        if (showMain) CHECK(model->doeStepData->propertyTableCreated());
    }
}

TEST_CASE(doe_session_image_codec_diagnostic_precedes_png_guard_and_record_append)
{
    for (const auto& encoded : {std::string(""), std::string("001122")}) {
        auto state = parsedSession("<PartRoot><SessionRoot><Session TaskType='SfTaskDoe'>"
            "<Image>" + encoded + "</Image></Session></SessionRoot></PartRoot>");
        std::vector<std::string> observed;
        int clears = 0;
        doe::SessionImportContext context{[&] { ++clears; }, [&](const std::string& text) {
            REQUIRE(state.parts.size() == 1);
            CHECK(state.parts[0].sessions.empty() && state.parts[0].currentSession == -1);
            CHECK(clears == 0);
            observed.push_back(text);
        }};
        CHECK(refuses([&] { doe::readSessionRecords(state, context); }, "QImage PNG"));
        CHECK(observed == std::vector<std::string>({"qUncompress: Input data is corrupted"}));
        CHECK(state.parts[0].sessions.empty() && clears == 0);
    }
}

TEST_CASE(doe_session_image_codec_uses_first_image_and_does_not_guess_empty_png_result)
{
    // Four zero bytes are qCompress(empty), which qUncompress returns without a
    // warning. A standard zlib stream with length hint1 decompresses to "A".
    for (const auto& encoded : {std::string("00000000"), std::string("00000001789c73040000420042")}) {
        auto state = parsedSession("<PartRoot><SessionRoot><Session TaskType='SfTaskDoe'>"
            "<Image>" + encoded + "</Image><Image/></Session></SessionRoot></PartRoot>");
        std::vector<std::string> observed;
        CHECK(refuses([&] { doe::readSessionRecords(state,
            {[] {}, [&](const std::string& text) { observed.push_back(text); }}); }, "QImage PNG"));
        CHECK(observed.empty()); //later empty/corrupt Image is not visited
        CHECK(state.parts[0].sessions.empty());
    }
}

TEST_CASE(doe_session_image_codec_requires_diagnostic_sink_when_original_warns)
{
    auto state = parsedSession("<PartRoot><SessionRoot><Session><Image/></Session></SessionRoot></PartRoot>");
    CHECK(refuses([&] { doe::readSessionRecords(state, {[] {}}); }, "diagnostic observation callback"));
    REQUIRE(state.parts.size() == 1);
    CHECK(state.parts[0].sessions.empty() && state.parts[0].currentSession == -1);
    auto io = doe::makeRecordingStudyIo();
    bool sawRecordedFirst = false;
    io->setQtWarningSink([&](const std::string& text) {
        sawRecordedFirst = io->qtWarnings() == std::vector<std::string>({text});
    });
    io->noteQtWarning("original Qt diagnostic");
    CHECK(sawRecordedFirst);
}

TEST_CASE(doe_session_original_qt_diagnostic_does_not_become_strict_or_abi_warning)
{
    Fixture f;
    ProjectScope scope(f.project);
    REQUIRE(engine::opModel(f.project, f.operation) != nullptr);
    Ctx ctx;
    ctx.flags = M2M_FLAG_STRICT;
    f.io->setQtWarningSink([&ctx](const std::string& text) { ctx.originalDiagnostics.push_back(text); });
    f.io->files[f.path] = "<PartRoot><SessionRoot><Session TaskType='SfTaskDoe'>"
        "<Image/></Session></SessionRoot></PartRoot>";
    CHECK(refuses([&] { f.cls->loadClassData(f.project, f.operation); }, "QImage PNG"));
    CHECK(ctx.strict() && ctx.warnings.empty());
    CHECK(ctx.originalDiagnostics == std::vector<std::string>({"qUncompress: Input data is corrupted"}));
    CHECK(f.io->qtWarnings() == ctx.originalDiagnostics);
    CHECK(f.io->textWrites().empty());
    // This asserts the diagnostic/ABI separation; the later unsupported PNG
    // guard still prevents any claim of a successful full conversion here.
}

TEST_CASE(doe_session_statevar_uses_explicit_project_profile_and_retains_ordered_prefix)
{
    for (const bool supplied : {false, true}) {
        Fixture f;
        ProjectScope scope(f.project);
        auto* model = engine::opModel(f.project, f.operation);
        REQUIRE(model != nullptr);
        engine::displayShow(f.project, model);
        auto& display = engine::projectDisplay(f.project);
        REQUIRE(engine::displayNamedModel(display, L("ROIs")) != nullptr);
        if (supplied) {
            auto context = std::make_shared<doe::StateVarConstructorContext>();
            context->profileIdentity = "declared-Session-unit-profile; not original capture";
            context->globals.emplace();
            context->globals->colorbarNo = 7;
            context->globals->palette.codes[0] = 42;
            context->deflectionScaleType.observation = doe::SettingObservation::Absent;
            context->deflectionScale.observation = doe::SettingObservation::Absent;
            context->deflectionLineThickness.observation = doe::SettingObservation::Absent;
            context->showDeflectionReference.observation = doe::SettingObservation::Absent;
            context->transparentDeflectionReference.observation = doe::SettingObservation::Absent;
            f.project.originalSettings.doeStateVarConstructor = context;
        }
        f.io->files[f.path] = "<PartRoot><SessionRoot><Session TaskType='SfTaskDoe'>"
            "<StepData><StateVar><SVPlotVar>1000</SVPlotVar></StateVar></StepData>"
            "<SfTask><ObjList><Object No='4'/></ObjList></SfTask>"
            "</Session></SessionRoot></PartRoot>";
        CHECK(refuses([&] { f.cls->loadClassData(f.project, f.operation); },
            supplied ? "d15b40 postparse" : "explicit resolved profile"));
        CHECK(engine::displayModels(display).named.empty());
        REQUIRE(f.study().outputDocument->parts.size() == 1);
        const auto& part = f.study().outputDocument->parts[0];
        REQUIRE(part.sessions.size() == 1 && part.sessions[0]->task != nullptr);
        // Task construction/append and named clear precede StepData; later SfTask
        // payload has not executed when the StateVar prefix guards.
        CHECK(part.sessions[0]->task->lists.objects == std::vector<int>({0}));
        if (supplied) {
            REQUIRE(model->doeStepData != nullptr);
            REQUIRE(model->doeStepData->stateVar != nullptr);
            CHECK(model->doeStepData->stateVar->plotVar == 1000);
            CHECK(!model->doeStepData->propertyTableCreated());
            REQUIRE(f.io->observedStateVarRuntime() != nullptr);
            CHECK(f.io->observedStateVarRuntime()->palette->codes[0] == 42);
        } else {
            CHECK(model->doeStepData == nullptr);
            CHECK(f.io->observedStateVarRuntime() == nullptr);
        }
        CHECK(f.io->qtWarnings().empty() && f.io->textWrites().empty());
    }
}

TEST_CASE(doe_session_colorbar_snapshot_reaches_ordered_clone_before_later_task_payload)
{
    Fixture f;
    ProjectScope scope(f.project);
    auto* model = engine::opModel(f.project, f.operation);
    REQUIRE(model != nullptr);
    engine::displayShow(f.project, model);
    auto context = std::make_shared<doe::StateVarConstructorContext>();
    context->profileIdentity = "declared-colorbar-Session-unit-profile; not original capture";
    context->globals.emplace();
    context->globals->colorbarNo = 7;
    context->deflectionScaleType.observation = doe::SettingObservation::Absent;
    context->deflectionScale.observation = doe::SettingObservation::Absent;
    context->deflectionLineThickness.observation = doe::SettingObservation::Absent;
    context->showDeflectionReference.observation = doe::SettingObservation::Absent;
    context->transparentDeflectionReference.observation = doe::SettingObservation::Absent;
    context->colorbars.emplace(2);
    auto& colorbar = (*context->colorbars)[1];
    colorbar.field00 = 42;
    colorbar.title = L("Declared Session colorbar");
    colorbar.first.push_back({{1, 2, 3}, 4, 5});
    colorbar.second.push_back({{6, 7, 8}, 9, 10});
    f.project.originalSettings.doeStateVarConstructor = context;
    f.io->files[f.path] = "<PartRoot><SessionRoot><Session TaskType='SfTaskDoe'>"
        "<StepData><StateVar><SVColorBarNo>1</SVColorBarNo><SVPlotVar>1000</SVPlotVar>"
        "</StateVar></StepData><SfTask><ObjList><Object No='4'/></ObjList></SfTask>"
        "</Session></SessionRoot></PartRoot>";
    CHECK(refuses([&] { f.cls->loadClassData(f.project, f.operation); }, "d15b40 postparse"));
    REQUIRE(model->doeStepData != nullptr && model->doeStepData->stateVar != nullptr);
    const auto& current = *model->doeStepData->stateVar;
    REQUIRE(current.auxiliaryColorbar != nullptr);
    CHECK(current.auxiliaryColorbar->field00 == 42 && current.auxiliaryColorbar->title == colorbar.title);
    CHECK(current.colorbarNo == 1 && current.plotVar == 1000 && !current.auxiliaryColorbarPending);
    CHECK(!model->doeStepData->propertyTableCreated());
    const auto& allocations = f.io->stateVarRuntime().colorbarPointAllocations;
    REQUIRE(allocations.size() == 2);
    CHECK(allocations[0].value == colorbar.first[0] && allocations[1].value == colorbar.second[0]);
    REQUIRE(f.study().outputDocument->parts.size() == 1);
    const auto& part = f.study().outputDocument->parts[0];
    REQUIRE(part.sessions.size() == 1 && part.sessions[0]->task != nullptr);
    CHECK(part.sessions[0]->task->lists.objects == std::vector<int>({0}));
    CHECK(f.io->reads().size() == 1 && f.io->textWrites().empty() && f.io->qtWarnings().empty());
}

TEST_CASE(doe_session_zero_var_scene_legend_and_existing_task_class_data_finish)
{
    Fixture f;
    ProjectScope scope(f.project);
    auto* model = engine::opModel(f.project, f.operation);
    REQUIRE(model != nullptr);
    engine::displayShow(f.project, model);
    auto context = std::make_shared<doe::StateVarConstructorContext>();
    context->profileIdentity = "declared-zero-var-Scene-Session-profile; not original capture";
    context->globals.emplace();
    context->deflectionScaleType.observation = doe::SettingObservation::Absent;
    context->deflectionScale.observation = doe::SettingObservation::Absent;
    context->deflectionLineThickness.observation = doe::SettingObservation::Absent;
    context->showDeflectionReference.observation = doe::SettingObservation::Absent;
    context->transparentDeflectionReference.observation = doe::SettingObservation::Absent;
    context->translatedNoneTitle = L("Explicit translated Session None");
    context->displayContext.emplace();
    auto& displayInput = *context->displayContext;
    displayInput.boundDisplay = model->display;
    displayInput.mainWindowAvailable = true;
    displayInput.backend = doe::StateVarDisplayBackend::Scene;
    displayInput.legendSlot = doe::StateVarPresence::Absent;
    displayInput.propertyTable = doe::StateVarPresence::Absent;
    displayInput.sceneSize = std::array<double, 2>{121, 101};
    displayInput.legendGlobals = doe::StateVarLegendGlobals{0x81, -3, 12, 27, 0x82};
    f.project.originalSettings.doeStateVarConstructor = context;
    f.io->files[f.path] = "<PartRoot><SessionRoot><Session TaskType='SfTaskDoe'>"
        "<StepData><StateVar><SVType>19</SVType><SVPlotVar>0</SVPlotVar>"
        "</StateVar></StepData><SfTask><ObjList><Object No='4'/></ObjList></SfTask>"
        "</Session></SessionRoot></PartRoot>";
    f.cls->loadClassData(f.project, f.operation);
    REQUIRE(model->doeStepData != nullptr && model->doeStepData->stateVar != nullptr);
    const auto& current = *model->doeStepData->stateVar;
    CHECK(current.type == 2 && current.plotTitle == *context->translatedNoneTitle);
    CHECK(current.minimum == 0 && current.maximum == 1);
    CHECK(!current.plotApplyInProgress && !current.postparsePending);
    CHECK(model->doeStepData->propertyTableCreated());
    const auto& legends = f.io->stateVarRuntime().legends;
    REQUIRE(legends.size() == 1);
    CHECK(legends[0].display == model->display && legends[0].x == -4);
    CHECK(legends[0].insertedIntoScene && !legends[0].visible && legends[0].cacheInvalidations == 1);
    REQUIRE(f.study().outputDocument->parts.size() == 1);
    const auto& part = f.study().outputDocument->parts[0];
    REQUIRE(part.sessions.size() == 1 && part.sessions[0]->task != nullptr);
    CHECK(part.sessions[0]->task->lists.objects == std::vector<int>({4}));
    CHECK(f.io->reads().size() == 1 && f.io->textWrites().empty() && f.io->qtWarnings().empty());
}

TEST_CASE(doe_session_missing_store_guards_at_record_allocation_before_factory_append_payload)
{
    auto state = parsedSession("<PartRoot><SessionRoot><Session TaskType='SfTaskDoe' Title='pending'>"
        "<SfTask/></Session></SessionRoot></PartRoot>");
    auto io = doe::makeRecordingStudyIo(); //no actual allocation owner bound
    int clears = 0;
    CHECK(refuses([&] { doe::readSessionRecords(state, {[&] { ++clears; }, {}, {}, io.get()}); },
        "Session allocation retention owner is unavailable before native record allocation"));
    REQUIRE(state.parts.size() == 1);
    CHECK(state.parts[0].sessions.empty() && state.parts[0].currentSession == -1);
    CHECK(clears == 0 && !state.currentPart);
}

TEST_CASE(doe_session_expired_task_store_is_not_replaced_by_local_factory_state)
{
    auto state = parsedSession("<PartRoot><SessionRoot><Session TaskType='SfTaskDoe'/></SessionRoot></PartRoot>");
    auto io = doe::makeRecordingStudyIo();
    auto store = std::make_shared<doe::TaskAllocationStore>();
    io->bindTaskAllocationStore(store);
    store.reset();
    CHECK(!io->taskAllocationStoreOwner());
    CHECK(refuses([&] { doe::readSessionRecords(state, {[] {}, {}, {}, io.get()}); },
        "Session allocation retention owner is unavailable before native record allocation"));
    REQUIRE(state.parts.size() == 1);
    CHECK(state.parts[0].sessions.empty() && !io->taskAllocationStoreOwner());
}

TEST_CASE(doe_session_type2_clear_retains_record_task_payload_and_raw_backlink)
{
    ReadIo io;
    auto state = parsedSession("<PartRoot><SessionRoot><Session TaskType='SfTaskDoe' Title='old'>"
        "<SfTask><ObjList><Object No='7'/></ObjList></SfTask></Session></SessionRoot></PartRoot>");
    doe::readSessionRecords(state, {[] {}, {}, {}, &io});
    const auto task = state.parts[0].sessions[0]->task;
    const auto identity = state.parts[0].sessions[0]->identity();
    const auto* originalRecord = state.parts[0].sessions[0];
    const auto oldRoot = state.parts[0].root;
    state.partRoot = xml::createElement(L("PartRoot"));
    CHECK(refuses([&] { doe::readSessionRecords(state, {[] {}, {}, {}, &io}); }, "0x1406469d0"));
    CHECK(state.parts.empty() && oldRoot->children.empty());
    CHECK(task && !identity.expired());
    CHECK(io.taskAllocations->retainedTaskCount() == 1 && io.taskAllocations->retainedSessionCount() == 1);
    CHECK(task->sessionBacklink() == io.taskAllocations->retainedSessionAt(0));
    CHECK(task->sessionBacklink() == originalRecord); //no transfer/move at list clear
    CHECK(task->sessionBacklink()->title == L("old") && task->sessionBacklink()->task.get() == task.get());
    CHECK(task->lists.objects == std::vector<int>({7}));
}

TEST_CASE(doe_session_nonowning_list_clear_needs_no_new_allocation_owner_or_record_access)
{
    auto state = parsedSession("<PartRoot><SessionRoot><Session TaskType='SfTaskDoe' Title='old'/></SessionRoot></PartRoot>");
    auto io = doe::makeRecordingStudyIo();
    auto store = std::make_shared<doe::TaskAllocationStore>();
    io->bindTaskAllocationStore(store);
    const auto factory = doe::bindBoundedParsedDoeProjection(*io, store);
    doe::readSessionRecords(state, {[] {}, {}, {}, io.get()});
    const auto root = state.parts[0].root;
    const auto identity = state.parts[0].sessions[0]->identity();
    state.parts[0].currentSession = 3; //explicit helper-entry state
    store.reset();
    state.partRoot = xml::createElement(L("PartRoot"));
    CHECK(refuses([&] { doe::readSessionRecords(state, {[] {}, {}, {}, io.get()}); },
        "0x1406469d0"));
    CHECK(state.parts.empty() && root->children.empty());
    CHECK(identity.expired()); //the earlier adapter scope ended, not list clear
}

TEST_CASE(doe_session_document_destruction_drops_only_nonowning_record_references)
{
    ReadIo io;
    doe::TaskObserver task;
    const doe::SessionRecord* record = nullptr;
    {
        auto state = parsedSession("<PartRoot><SessionRoot><Session TaskType='SfTaskDoe' Title='alive'/>"
            "</SessionRoot></PartRoot>");
        doe::readSessionRecords(state, {[] {}, {}, {}, &io});
        record = state.parts[0].sessions[0];
        task = record->task;
    } //adapter document destruction is not an invented native record delete
    CHECK(task && task->sessionBacklink() == record);
    CHECK(record == io.taskAllocations->retainedSessionAt(0) && record->title == L("alive"));
    CHECK(io.taskAllocations->retainedSessionCount() == 1);
}

TEST_CASE(doe_session_unsupported_task_constructor_keeps_already_allocated_record)
{
    ReadIo io;
    auto state = parsedSession("<PartRoot><SessionRoot><Session TaskType='Custom' Title='pending' "
        "StepNumber='8'><UserDefinedStepList>1,,2</UserDefinedStepList></Session></SessionRoot></PartRoot>");
    CHECK(refuses([&] { doe::readSessionRecords(state, {[] {}, {}, {}, &io}); }, "registered non-DOE"));
    REQUIRE(state.parts.size() == 1);
    CHECK(state.parts[0].sessions.empty() && state.parts[0].currentSession == -1);
    REQUIRE(io.taskAllocations->retainedSessionCount() == 1);
    const auto* record = io.taskAllocations->retainedSessionAt(0);
    CHECK(record->title == L("pending") && record->stepNumber == 8 && record->task == nullptr);
    CHECK(record->userDefinedSteps == std::vector<int>({1, 0, 2}));
    CHECK(io.taskAllocations->retainedTaskCount() == 0);
}

TEST_CASE(doe_session_suppressed_factory_keeps_null_task_and_skips_task_only_payloads)
{
    ReadIo io;
    auto& notifier = io.sessionNotificationState();
    notifier.suppressed = true;
    notifier.flag90 = true;
    auto state = parsedSession("<PartRoot><SessionRoot>"
        "<Session TaskType='SfTaskDoe'><SfTask><OutputList><Output/></OutputList></SfTask><StepData/></Session>"
        "<Session TaskType='Custom'><StepData/><SfTask><OutputList><Output/></OutputList></SfTask></Session>"
        "</SessionRoot></PartRoot>");
    int clears = 0;
    int stepReads = 0;
    doe::readSessionRecords(state, {[&] {
        const auto& part = state.parts[0];
        CHECK(part.sessions.size() == static_cast<std::size_t>(clears + 1));
        CHECK(part.currentSession == clears && part.sessions.back()->task == nullptr);
        CHECK(notifier.flag90);
        ++clears;
    }, {}, [&](const xml::Node&) { ++stepReads; }, &io});
    REQUIRE(state.parts.size() == 1 && state.parts[0].sessions.size() == 2);
    CHECK(state.parts[0].sessions[0]->taskType == L("SfTaskDoe"));
    CHECK(state.parts[0].sessions[1]->taskType == L("Custom"));
    CHECK(clears == 2 && stepReads == 0);
    CHECK(io.taskAllocations->retainedSessionCount() == 2 && io.taskAllocations->retainedTaskCount() == 0);
    CHECK(notifier.flag90 && notifier.emptyKeys.empty());
    REQUIRE(notifier.attempts.size() == 4);
    for (const auto& attempt : notifier.attempts) CHECK(attempt.suppressed);
}

TEST_CASE(doe_session_factory_rereads_shared_suppression_after_each_named_model_clear)
{
    ReadIo io;
    auto& notifier = io.sessionNotificationState();
    notifier.suppressed = true;
    notifier.flag90 = true;
    auto state = parsedSession("<PartRoot><SessionRoot>"
        "<Session TaskType='Custom'/><Session TaskType='SfTaskDoe'>"
        "<SfTask><ObjList><Object No='9'/></ObjList></SfTask></Session>"
        "</SessionRoot></PartRoot>");
    int clears = 0;
    doe::readSessionRecords(state, {[&] {
        const auto& part = state.parts[0];
        CHECK(part.currentSession == clears && part.sessions.size() == static_cast<std::size_t>(clears + 1));
        CHECK(notifier.flag90); //neither return-value invocation clears it
        CHECK(static_cast<bool>(part.sessions.back()->task) == (clears == 1));
        notifier.suppressed = false;
        ++clears;
    }, {}, {}, &io});
    REQUIRE(state.parts[0].sessions.size() == 2);
    const auto* first = state.parts[0].sessions[0];
    const auto* second = state.parts[0].sessions[1];
    CHECK(first->task == nullptr);
    REQUIRE(second->task != nullptr);
    CHECK(second->task->sessionBacklink() == second);
    CHECK(second->task->lists.objects == std::vector<int>({9}));
    CHECK(clears == 2 && io.taskAllocations->retainedTaskCount() == 1);
    CHECK(!notifier.flag90 && notifier.emptyKeys.size() == 4);
    REQUIRE(notifier.attempts.size() == 4);
    for (const auto& attempt : notifier.attempts) CHECK(!attempt.suppressed);
}

TEST_CASE(doe_session_suppression_does_not_skip_layered_stepdata_after_record_append)
{
    ReadIo io;
    auto& notifier = io.sessionNotificationState();
    notifier.suppressed = true;
    notifier.flag90 = true;
    auto state = parsedSession("<PartRoot><SessionRoot><Session TaskType='Custom'>"
        "<SfTask><OutputList><Output/></OutputList></SfTask><StepData LayerName='Layer'/>"
        "</Session></SessionRoot></PartRoot>");
    int clears = 0;
    CHECK(refuses([&] { doe::readSessionRecords(state, {[&] { ++clears; }, {}, {}, &io}); },
        "layered StepData"));
    REQUIRE(state.parts.size() == 1 && state.parts[0].sessions.size() == 1);
    CHECK(state.parts[0].currentSession == 0 && state.parts[0].sessions[0]->task == nullptr);
    CHECK(clears == 1 && io.taskAllocations->retainedSessionCount() == 1);
    CHECK(io.taskAllocations->retainedTaskCount() == 0);
    CHECK(notifier.flag90 && notifier.attempts.empty() && notifier.emptyKeys.empty());
}

TEST_CASE(doe_session_suppression_does_not_skip_image_diagnostic_before_record_allocation)
{
    ReadIo io;
    auto& notifier = io.sessionNotificationState();
    notifier.suppressed = true;
    notifier.flag90 = true;
    auto state = parsedSession("<PartRoot><SessionRoot><Session TaskType='Custom'>"
        "<Image/></Session></SessionRoot></PartRoot>");
    int clears = 0;
    std::vector<std::string> warnings;
    CHECK(refuses([&] { doe::readSessionRecords(state,
        {[&] { ++clears; }, [&](const std::string& warning) { warnings.push_back(warning); }, {}, &io}); },
        "QImage PNG"));
    REQUIRE(state.parts.size() == 1);
    CHECK(state.parts[0].sessions.empty() && state.parts[0].currentSession == -1);
    CHECK(clears == 0 && io.taskAllocations->retainedSessionCount() == 0);
    CHECK(io.taskAllocations->retainedTaskCount() == 0);
    CHECK(warnings.size() == 1);
    CHECK(notifier.flag90 && notifier.attempts.empty() && notifier.emptyKeys.empty());
}

TEST_CASE(doe_session_native_factory_guard_retains_record_before_append_and_payload)
{
    auto io = doe::makeRecordingStudyIo();
    auto store = std::make_shared<doe::TaskAllocationStore>();
    io->bindTaskAllocationStore(store);
    auto factory = std::make_shared<doe::TaskFactoryRuntime>(store);
    io->bindTaskFactoryRuntime(factory);
    io->sessionNotificationState().createTaskBaseRegistrations = doe::TaskFactoryRegistrationList{
        {{factory, "slotCreateTaskBase"}}, "Explicit native return-receiver test profile", true};
    auto state = parsedSession("<PartRoot><SessionRoot><Session TaskType='SfTaskDoe' Title='before-append'>"
        "<SfTask><ObjList><Object No='9'/></ObjList></SfTask></Session></SessionRoot></PartRoot>");
    int clears = 0;
    CHECK(refuses([&] { doe::readSessionRecords(state, {[&] { ++clears; }, {}, {}, io.get()}); },
        "RVA 0x131185"));
    REQUIRE(state.parts.size() == 1);
    CHECK(state.parts[0].sessions.empty() && state.parts[0].currentSession == -1 && clears == 0);
    REQUIRE(store->retainedSessionCount() == 1);
    CHECK(store->retainedSessionAt(0)->title == L("before-append"));
    CHECK(store->retainedSessionAt(0)->task == nullptr);
    CHECK(store->retainedTaskCount() == 0 && store->retainedPrototypeCount() == 1);
    CHECK(factory->construction().dataCommitted && !factory->construction().nativeInitializationComplete);
}

TEST_CASE(doe_session_suppressed_native_receiver_appends_without_constructing_factory)
{
    auto io = doe::makeRecordingStudyIo();
    auto store = std::make_shared<doe::TaskAllocationStore>();
    io->bindTaskAllocationStore(store);
    auto factory = std::make_shared<doe::TaskFactoryRuntime>(store);
    io->bindTaskFactoryRuntime(factory);
    auto& notifier = io->sessionNotificationState();
    notifier.suppressed = true;
    notifier.flag90 = true;
    //Even unknown registration population is not consumed on this branch.
    auto state = parsedSession("<PartRoot><SessionRoot><Session TaskType='Custom'><StepData/>"
        "<SfTask><OutputList><Output/></OutputList></SfTask></Session></SessionRoot></PartRoot>");
    int clears = 0;
    doe::readSessionRecords(state, {[&] { ++clears; }, {}, {}, io.get()});
    REQUIRE(state.parts[0].sessions.size() == 1);
    CHECK(state.parts[0].sessions[0]->task == nullptr && clears == 1);
    CHECK(!factory->construction().factoryCommitted && store->retainedPrototypeCount() == 0);
    CHECK(store->retainedTaskCount() == 0 && store->retainedSessionCount() == 1);
    CHECK(notifier.flag90 && !notifier.createTaskBaseRegistrations);
}

int main() { return testkit::run_all(); }
