#include "m2m/doe_session_records.hpp"

#include <cstdint>
#include <limits>
#include <string>
#include <utility>

#include "m2m/common.hpp"
#include "m2m/blob.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/doe_session.hpp"
#include "m2m/doe_session_notifications.hpp"
#include "m2m/doe_task_factory.hpp"
#include "m2m/doe_stepdata.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine_scene.hpp"
#include "m2m/engine/model_manager.hpp"
#include <defmodel/layout/cls_1401e7b10.hpp>

namespace m2m::doe {
namespace {
using qt::L;

int shortAttribute(const xml::Node& node, const char* name)
{
    bool ok = false;
    const int value = qt::toInt(node.attribute(L(name)), &ok);
    return ok && value >= -32768 && value <= 32767 ? value : 0;
}

TaskObserver createTask(const qt::String& type, StudyIo* io)
{
    //650e60 invokes2cf740 with key2signalCreateTaskBase(), Q_ARG(QString,
    // parsed TaskType) and a null Q_RETURN_ARG(SfTaskBase *). The shared+141
    // gate returns before receiver/factory lookup and leaves that result null;
    // unlike2cf3c0, this return-value notifier does not reset flag90.
    if (!io)
        throw Error(M2M_E_UNSUPPORTED, "DOE Task return notifier needs actual shared I/O context");
    return dispatchCreateTaskBase(*io, type);
}

std::vector<int> userDefinedSteps(const xml::Node& session)
{
    const auto lists = session.childElements(L("UserDefinedStepList"));
    if (lists.empty()) return {};
    const auto text = lists.front()->text().u16();
    std::vector<int> values;
    std::size_t start = 0;
    for (;;) {                                           //6536b0 comma KeepEmptyParts
        const auto end = text.find(u',', start);
        values.push_back(qt::toInt(qt::String(text.substr(start, end - start))));
        if (end == std::u16string::npos) break;
        start = end + 1;
    }
    return values;
}

void importSession(SessionPart& part, const xml::Node& node, const SessionImportContext& context)
{
    //653930 runs before list parsing/record creation and uses the first Image.
    // Reuse the existing original-byte codec seam; preserve its diagnostic before
    // guarding at the unrecovered QImage call, including empty decoded bytes.
    const auto images = node.childElements(L("Image"));
    if (!images.empty()) {
        std::string warning;
        const auto bytes = qt::qUncompress(qt::fromHex(qt::toAscii(images.front()->text())), &warning);
        if (!warning.empty()) {
            if (!context.reportQtWarning)
                throw Error(M2M_E_UNSUPPORTED, "DOE session Image qUncompress needs Qt diagnostic observation callback");
            context.reportQtWarning(warning);
        }
        //QImage::loadFromData is called even with empty bytes. Do not assume its
        // reader/PNG diagnostic behavior or silently infer a decoded null image.
        (void)bytes;
        throw Error(M2M_E_UNSUPPORTED, "DOE session Image qUncompress complete; QImage PNG decoding remains unsupported");
    }
    SessionRecordMetadata metadata;
    metadata.title = node.attribute(L("Title"));
    metadata.type = shortAttribute(node, "Type");
    metadata.subtype = shortAttribute(node, "Subtype");
    metadata.databaseId = shortAttribute(node, "DB_ID");
    metadata.steps = shortAttribute(node, "Steps");
    metadata.stepNumber = qt::toInt(node.attribute(L("StepNumber")));
    const int objectCount = qt::toInt(node.attribute(L("Objects")));
    //650e60 converts Objects to int, then stores its low16 bits in record+2a.
    // This differs from toShort overflow->0 used by the attributes above.
    const unsigned low = static_cast<unsigned>(objectCount) & 0xffffu;
    metadata.objects = low < 0x8000u ? static_cast<int>(low) : static_cast<int>(low) - 0x10000;
    metadata.taskType = node.attribute(L("TaskType"));
    metadata.entityNo = qt::toInt(node.attribute(L("EntityNo")));
    metadata.userDefinedSteps = userDefinedSteps(node);    //before658130/factory
    const auto store = context.io ? context.io->taskAllocationStoreOwner() : nullptr;
    if (!store)
        throw Error(M2M_E_UNSUPPORTED,
            "DOE Session allocation retention owner is unavailable before native record allocation");
    auto& record = store->createSessionRecord();           //actual658130 allocation
    static_cast<SessionRecordMetadata&>(record) = std::move(metadata);
    bindSessionTask(record, createTask(record.taskType, context.io));
    //Type2 document flag28=true; fresh SfTaskDoe vt168+a8=true. Both this
    // Task and the missing-key null Task are appended before653be0 is entered.
    part.sessions.push_back(&record);
    part.currentSession = static_cast<int>(part.sessions.size()) - 1;
    auto& current = *part.sessions.back();
    if (!context.clearNamedModels)
        throw Error(M2M_E_UNSUPPORTED, "DOE session 0x140650e60 payload needs 0x140cfa790 named-model clear");
    context.clearNamedModels();
    for (const auto* child : node.childElements()) {
        if (child->tag == L("StepData")) {
            if (child->hasAttribute(L("LayerName")))
                throw Error(M2M_E_UNSUPPORTED, "DOE session layered StepData model import remains unsupported");
            if (current.task) {
                if (!context.readStepData)
                    throw Error(M2M_E_UNSUPPORTED, "DOE session StepData model import needs display-main callback");
                context.readStepData(*child);
            }
            //No LayerName and no Task:653be0 skips vt248.
        } else if (child->tag == L("SfTask") && current.task) {
            readTaskLists(*child, current.task->lists);    //vt218 at actual encounter
        }
        //Actual644440(...,1,0,0) passes false to653be0. SfDispView,
        //GraphicsItems, Image, SfPtrk/SfFNet/ROIs/Coupons imports require true.
    }
}

void importPart(SessionDocument& document, const xml::NodePtr& root,
                const SessionImportContext& context)
{
    document.parts.emplace_back();                       //644440 append before659d90
    auto& part = document.parts.back();
    if (root->hasAttribute(L("OprEndNo"))) part.operationEnd = shortAttribute(*root, "OprEndNo");
    if (root->hasAttribute(L("OprStartNo"))) part.operationStart = shortAttribute(*root, "OprStartNo");
    if (root->hasAttribute(L("PartEntityNo"))) part.entityNo = shortAttribute(*root, "PartEntityNo");
    if (root->hasAttribute(L("DisplayName"))) part.displayName = root->attribute(L("DisplayName"));
    //010330 identifies Type2;643100/658690/65ae80 set inner flag28 true.
    if (root->hasAttribute(L("ExportSiesta"))) part.exportSiesta = shortAttribute(*root, "ExportSiesta") != 0;
    if (root->hasAttribute(L("ExportAnsys"))) part.exportAnsys = shortAttribute(*root, "ExportAnsys") != 0;
    part.root = root;                                    //650e60 ->657150, before children
    for (const auto* child : root->childElements()) {
        if (child->tag == L("Session")) importSession(part, *child, context);
        if (child->tag == L("DBLIST") || child->tag == L("ReportList"))
            throw Error(M2M_E_UNSUPPORTED, "DOE session 0x140650e60 DBLIST/ReportList import remains unsupported");
        // Other elements and non-element children are skipped by650e60.
    }
}

void descendants(SessionDocument& document, const xml::NodePtr& parent,
                 const SessionImportContext& context)
{
    for (const auto& child : parent->children) {
        if (!child || child->kind != xml::Node::Element) continue;
        if (child->tag == L("SessionRoot")) importPart(document, child, context);
        descendants(document, child, context); //QDom elementsByTagName includes nested matches
    }
}
}  // namespace

void readSessionRecords(SessionDocument& document, const SessionImportContext& context)
{
    //646e90 ->6589a0 ->64f8e0: even an empty session list clears all raw
    // children of its retained root before the part record is destroyed. These
    // references can still point into the current DOM when no new file replaced it.
    //ActualType2 b5ad0->643100 sets+29=false:64f8e0 skips Task deletion.
    //20db90 clears raw Session pointers, not record allocations. Their actual
    //store owns them from658130; list/document destruction does not transfer or
    //destroy them, so raw Task backlinks remain valid across this operation.
    for (auto& part : document.parts) {
        part.currentSession = -1;
        part.sessions.clear();
        if (part.root) part.root->children.clear();
    }
    document.parts.clear();                             //644440 ->646e90
    document.currentPart.reset(); //deleted part QPointer becomes null
    if (document.partRoot && document.partRoot->kind == xml::Node::Element) {
        if (document.partRoot->tag == L("SessionRoot"))
            importPart(document, document.partRoot, context); //root excludes descendants
        else
            descendants(document, document.partRoot, context);
    }
    if (document.parts.empty())
        throw Error(M2M_E_UNSUPPORTED, "DOE session 0x1406469d0 empty-document default-part construction remains unsupported");
    //6469d0(doc,0) is a lookup for the existing-part branch.647170(doc,0)
    //then clears current and selects the FIRST entity0, before its notification.
    document.currentPart.reset();
    for (std::size_t index = 0; index < document.parts.size(); ++index) {
        if (document.parts[index].entityNo == 0) {
            document.currentPart = index;
            break;
        }
    }
    if (!context.io)
        throw Error(M2M_E_UNSUPPORTED, "DOE session signalChangeStepRange needs per-conversion notifier state");
    const auto notify = [&](const char* signal, std::optional<int> argument = {}) {
        const auto selected = document.currentPart;
        const std::optional<int> current = selected
            ? std::optional<int>(document.parts[*selected].currentSession) : std::nullopt;
        observeEmptySessionNotification(*context.io, signal, selected, current, argument);
    };
    notify("2signalChangeStepRange()");
    //647560 ->647110: ordinal0 fallback occurs AFTER ChangeStepRange returned.
    if (!document.currentPart) document.currentPart = 0;
    auto& selected = document.parts[*document.currentPart];
    selected.currentSession = -1; //656220, after fallback and before final emits
    notify("2signalOpenSession()");
    notify("2signalChapterChanged()", selected.currentSession); //656200
    if (selected.sessions.size() > static_cast<std::size_t>(std::numeric_limits<int>::max()))
        throw Error(M2M_E_UNSUPPORTED, "DOE session count exceeds original QList count");
    notify("2signalNumChaptersChanged()", static_cast<int>(selected.sessions.size())); //656240
}

SessionRecord& constructDefaultDoeSession(Project& project, SessionDocument& document, StudyIo& io)
{
    if (!document.currentPart || *document.currentPart >= document.parts.size())
        throw Error(M2M_E_UNSUPPORTED, "DOE default session needs recovered current-part ownership");
    auto* manager = io.modelManager();
    if (!manager)
        throw Error(M2M_E_UNSUPPORTED, "DOE output missing SfTaskDoe default construction needs live model manager");
    //6477a0 reads this before allocation. The native int is truncated to short,
    // unlike QString::toShort overflow handling in imported Session attributes.
    const auto id = static_cast<std::uint32_t>(manager->currentModelId()) & 0xffffu;
    const auto store = io.taskAllocationStoreOwner();
    if (!store)
        throw Error(M2M_E_UNSUPPORTED,
            "DOE default Session allocation retention owner is unavailable before native record allocation");
    auto allocation = std::make_unique<AdapterDefaultSessionAllocation>();
    allocation->part = *document.currentPart;
    allocation->record = &store->createSessionRecord();
    auto& record = *allocation->record;
    record.databaseId = id < 0x8000u ? static_cast<int>(id) : static_cast<int>(id) - 0x10000;
    record.stepNumber = -1; //658130; title remains null, Type/Subtype/Steps/Objects/EntityNo=0
    record.taskType = L("SfTaskDoe"); //7cb7b0 absent title;7cb730 absent enum→0
    auto& staged = *allocation;
    document.adapterDefaultAllocations.push_back(std::move(allocation));

    //2cf740 has the shared suppression gate but DOES NOT reset flag90.
    // The known main-window receiver uses the same fresh factory projection as
    // Session import. Constructor preferences/Qt registry lifetime stay opaque.
    if (!io.sessionNotificationState().suppressed) {
        staged.factoryInvoked = true;
        bindSessionTask(record, createTask(record.taskType, &io));
    }
    auto& part = document.parts[staged.part];
    observeEmptySessionNotification(io, "2signalGetCurrentStepViewInfo()", staged.part, part.currentSession);
    //The empty receiver list cannot replace the freshly assigned null QImage.
    if (part.operationStart == 0 && part.operationEnd == 0) {
        //Fresh native MO startup0111c0->02a0e0(null)->013d40 sets procType5
        //before project reading.64c280 uses two registered return-value signals,
        //not the other-process-mode current-GuiModel/DB path. Both receivers
        //resolve01d540 through the ACTUAL manager current Project; a pending,
        //registered-but-unselected Project must not supply operation defaults.
        const auto queryOperations = [&](const char* signal) -> std::vector<Operation*> {
            auto& state = io.sessionNotificationState();
            state.attempts.push_back({signal, state.suppressed, false,
                staged.part, part.currentSession, {}});
            //2cf740 does NOT clear flag90. Known populated receiver keys must
            //not enter emptyKeys. Resolve the actual current again per receiver.
            if (state.suppressed) return {};
            auto* current = manager->currentModel();
            if (!current || current->kind != engine::DbHandle::Kind::Mo) return {};
            if (!current->moProject)
                throw Error(M2M_E_UNSUPPORTED, "DOE operation-list receiver lazy Project construction remains unsupported");
            auto* subproject = activeSubProject(*current->moProject); //01d540
            return subproject ? flowOperations(*subproject) : std::vector<Operation*>{};
        };
        std::vector<qt::String> names;
        for (const auto* operation : queryOperations("2signalGetOprStringList()")) {
            if (!operation)
                throw Error(M2M_E_UNSUPPORTED, "DOE operation name receiver original null operation dereference remains unsupported");
            names.push_back(operation->name); //0b9a20/050ac0
        }
        std::vector<int> numbers;
        for (const auto* operation : queryOperations("2signalGetOprNoList()")) {
            if (!operation)
                throw Error(M2M_E_UNSUPPORTED, "DOE operation number receiver original null operation dereference remains unsupported");
            numbers.push_back(flowNumber(*operation)); //0b9bf0/050b20, not operation.id
        }
        std::vector<int> retainedNumbers;
        for (std::size_t index = 0; index < names.size(); ++index) {
            //64c280(true) skips matching DISPLAY NAMES, irrespective of class.
            if (names[index].u16().find(u"2D to 3D") != std::u16string::npos) continue;
            if (index >= numbers.size())
                throw Error(M2M_E_UNSUPPORTED, "DOE operation-list original mismatched ID-list access remains unsupported");
            //The temporary local8Bit/std::string name is unconsumed by this
            //range-only caller; no codec/temporary allocator parity is claimed.
            retainedNumbers.push_back(numbers[index]);
        }
        if (!retainedNumbers.empty()) {
            if (part.operationStart == 0) part.operationStart = retainedNumbers.front();
            if (part.operationEnd == 0) part.operationEnd = retainedNumbers.back();
        } //No retained rows: original leaves0/0, never synthesizes operation1.
    }

    //64fb80 first calls64fc30 on a DETACHED Session. It appends to retained DOM
    // and the session list only after all of the following payload work returns.
    staged.detachedSession = xml::createElement(L("Session"));
    auto& node = *staged.detachedSession;
    xml::setAttribute(node, L("Title"), record.title);
    xml::setAttribute(node, L("Type"), qt::number(record.type));
    xml::setAttribute(node, L("Subtype"), qt::number(record.subtype));
    xml::setAttribute(node, L("DB_ID"), qt::number(record.databaseId));
    xml::setAttribute(node, L("Steps"), qt::number(record.steps));
    xml::setAttribute(node, L("Objects"), qt::number(record.objects));
    if (!project.display)
        throw Error(M2M_E_UNSUPPORTED, "DOE default session needs actual display-main binding");
    auto* main = engine::displayMain(*project.display);
    if (main) {
        //SfTaskDoe RTTI has TaskBase/QObject only: the TaskReportBase cast in
        //64fc30 fails. c7b2d0 is simCtl(), and native+160 is NSTART.
        auto* control = main->simCtl();
        if (!control)
            throw Error(M2M_E_UNSUPPORTED, "DOE default session original null simCtl dereference remains unsupported");
        record.stepNumber = control->nstart;
        xml::setAttribute(node, L("StepNumber"), qt::number(record.stepNumber));
    }
    xml::setAttribute(node, L("TaskType"), record.taskType);
    xml::setAttribute(node, L("EntityNo"), qt::number(record.entityNo));
    auto image = xml::createElement(L("Image"));
    //Shipped QtGui650af620 returnsfalse on null image BEFORE touching device or
    //format. QBuffer bytes stay empty; qCompress(empty)/toHex→eight zero digits.
    image->children.push_back(xml::createTextNode(L("00000000")));
    node.children.push_back(std::move(image));
    if (main && record.task) {
        staged.detachedStepData = xml::createElement(L("StepData")); //Task vt98→141403b40
        writeStepData(*main, *staged.detachedStepData,
            project.originalSettings.doeStateVarConstructor.get(), io);
        node.children.push_back(staged.detachedStepData); //only after c90640 returns
    }
    throw Error(M2M_E_UNSUPPORTED, "DOE default session main-window data-view snapshot hook remains unsupported");
}

}  // namespace m2m::doe
