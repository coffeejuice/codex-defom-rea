#pragma once

#include <functional>
#include <memory>

#include "m2m/doe_task_runtime.hpp"
#include "m2m/xml.hpp"

namespace m2m::doe {
struct SessionDocument;
class StudyIo;

//650e60 parses these stack values before allocating658130. This contains no
//record/Task ownership or identity; parsed values overwrite constructor fields.
struct SessionRecordMetadata {
    qt::String title;
    qt::String taskType = qt::L("Custom"); //658130 literal1424e82b8
    int type = 0, subtype = 0, databaseId = 0, steps = 0;
    int stepNumber = -1, objects = 0, entityNo = 0;
    std::vector<int> userDefinedSteps;
};

struct SessionRecord : SessionRecordMetadata {
    SessionRecord();
    ~SessionRecord();
    SessionRecord(SessionRecord&&) = delete;
    SessionRecord& operator=(SessionRecord&&) = delete;
    SessionRecord(const SessionRecord&) = delete;
    SessionRecord& operator=(const SessionRecord&) = delete;

    TaskObserver task; //native +58 QPointer; allocation store is separate
    std::weak_ptr<SessionRecordIdentity> identity() const noexcept { return identity_; }

private:
    std::shared_ptr<SessionRecordIdentity> identity_;
};

struct SessionImportContext {
    //653be0 always calls cfa790: delete/clear the display's named models.
    // A missing callback guards at that exact point after record append.
    std::function<void()> clearNamedModels;
    // Original qWarning at Image qUncompress; never promote to ABI warnings.
    std::function<void(const std::string&)> reportQtWarning;
    //14140d100 uses display MAIN, not the operation's model. No-main is a no-op.
    std::function<void(const xml::Node&)> readStepData;
    //Four final signals have a recovered empty binding set; retain their
    //per-conversion map/flag effects through StudyIo, not a no-op callback.
    StudyIo* io = nullptr;
};

// 658690/6589d0/659020 and the Type2 inner model's initial state. This is
// a part record; it deliberately does not manufacture a Task or model payload.
struct SessionPart {
    SessionPart() = default;
    SessionPart(SessionPart&&) noexcept = default;
    SessionPart& operator=(SessionPart&&) noexcept = default;
    SessionPart(const SessionPart&) = delete;
    SessionPart& operator=(const SessionPart&) = delete;

    int operationStart = 0;
    int operationEnd = 0;
    int entityNo = 0;
    qt::String displayName = qt::L("$OPR_RANGE");
    bool exportSiesta = false;
    bool exportAnsys = false;
    int currentSession = -1;
    xml::NodePtr root; // inner model's retained QDomElement, assigned by657150
    std::vector<SessionRecord*> sessions; //ordered native nonowning pointer list
};

// Type2 prefix of644440: clear old parts, discover SessionRoot elements, append
// each constructed record BEFORE659d90 metadata import, then enter650e60.
// Allocates records in the actual I/O-bound store; part lists are nonowning.
// Constructs fresh DOE Task list state, appends before ordered Session payload
// dispatch. Missing registry keys produce null Tasks. Other registered Task
// classes guard before construction. Always guards at the first unrecovered
// Image/StepData/DBLIST/ReportList payload, or
// before646a60 empty-document construction. Existing parts complete final
// selection/reset plus four source-proven empty-binding notifications when I/O
// observation state is supplied. The higher operation lifecycle remains guarded.
void readSessionRecords(SessionDocument& document, const SessionImportContext& context = {});

}  // namespace m2m::doe
