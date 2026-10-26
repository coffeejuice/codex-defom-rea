// Isolated045780 null-current Project leaf; native wrapper remains guarded.
#pragma once

#include <memory>
#include <string>

#include "m2m/engine/close_empty_manager_tail.hpp"

namespace m2m {
struct Project;
namespace engine {
inline constexpr const char* closePostClearProjectSourceOccurrence =
    "0x140029a83:0x140045780(null-current-leaf)";

enum class ClosePostClearProjectStage { AwaitNullCurrentLeaf, NullProjectLeafReached };
enum class ClosePostClearProjectStop { NativeWrapperAndMainWindowEffectsUnavailable };
struct ClosePostClearProjectResult {
    ClosePostClearProjectStop stop;
    bool lookupCommittedThisAttempt;
    Project* project; // null from the inner current-null leaf, never Session storage
    const char* nextSourceSite;
    static constexpr bool nativeWrapperQualified = false;
    static constexpr bool completeForPublicClose = false;
};

// Admit only an already completed CP75 empty clear on this same actual manager.
// Construction revalidates that boundary without advancing/replaying clear.
// It retains no reference to the old progress object: this occurrence has its
// own Session/manager binding and guarded, one-shot inner lookup state.
// Provenance text authenticates neither original execution nor wrapper casts.
class ClosePostClearProjectProgress final {
public:
    ClosePostClearProjectProgress(const ConversionSession&, CloseEmptyManagerTailProgress&,
                                 std::string sourceOccurrence, std::string qualification);
    ClosePostClearProjectProgress(const ClosePostClearProjectProgress&) = delete;
    ClosePostClearProjectProgress& operator=(const ClosePostClearProjectProgress&) = delete;
    ClosePostClearProjectProgress(ClosePostClearProjectProgress&&) = delete;
    ClosePostClearProjectProgress& operator=(ClosePostClearProjectProgress&&) = delete;
    ClosePostClearProjectStage stage() const noexcept { return stage_; }
    const std::string& qualification() const noexcept { return qualification_; }
private:
    friend ClosePostClearProjectResult attemptClosePostClearProjectLookup(
        const ConversionSession&, ClosePostClearProjectProgress&);
    const ConversionSession* session_;
    std::weak_ptr<ModelManager> manager_;
    std::string qualification_;
    ClosePostClearProjectStage stage_ = ClosePostClearProjectStage::AwaitNullCurrentLeaf;
};

// Reads only the same actual manager's null current, then latches null Project.
// It never reads/releases Session::liveProject(), reconstructs a lazy Project,
// accesses an active subproject, or repeats clear. Arrival of any model or
// connection metadata refuses before lookup/retry. Native01d510/0133a0's
// caller-main+50/cast binding and029a93/02a0e0 remain unresolved; no busy,
// main-window/status/notification/Qt/destructor/public Close effects execute.
ClosePostClearProjectResult attemptClosePostClearProjectLookup(
    const ConversionSession&, ClosePostClearProjectProgress&);
} // namespace engine
} // namespace m2m
