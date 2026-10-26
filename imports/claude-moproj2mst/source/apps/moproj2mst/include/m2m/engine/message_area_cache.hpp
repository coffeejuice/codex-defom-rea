// Isolated SfMessageArea process cache; no GUI or native cleanup effects.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>

#include "m2m/qt4.hpp"

namespace m2m::engine {
inline constexpr const char* messageAreaCacheSourceOccurrence =
    "0x140d151f0:slotShowMsgInArea(QString,QColor,bool)";

enum class MessageAreaCacheKnowledge { Unknown, KnownUninitialized, KnownInitialized };

// One owner per represented native process, shared across receiver invocations.
// It is not owned by Session/Project/receiver and is not a native static QString.
// Observations label existing native state; they do not execute/authenticate it.
class MessageAreaProcessCache final {
public:
    static std::shared_ptr<MessageAreaProcessCache> create(std::string processQualification);
    MessageAreaProcessCache(const MessageAreaProcessCache&) = delete;
    MessageAreaProcessCache& operator=(const MessageAreaProcessCache&) = delete;
    MessageAreaProcessCache(MessageAreaProcessCache&&) = delete;
    MessageAreaProcessCache& operator=(MessageAreaProcessCache&&) = delete;

    MessageAreaCacheKnowledge knowledge() const noexcept { return knowledge_; }
    const std::optional<std::uint32_t>& initializationWord() const noexcept { return word_; }
    const std::optional<qt::String>& lastMessage() const noexcept { return last_; }
    const std::string& processQualification() const noexcept { return processQualification_; }
    const std::string& bindingQualification() const noexcept { return bindingQualification_; }
    // One-time existing-state binding. Same exact binding retains first receipt;
    // conflicting rebinds cannot bypass guarded initialization/append effects.
    bool bindObservedUninitialized(std::uint32_t word, std::string qualification);
    bool bindObservedInitialized(std::uint32_t word, const qt::String& last,
                                 std::string qualification);
private:
    explicit MessageAreaProcessCache(std::string);
    MessageAreaCacheKnowledge knowledge_ = MessageAreaCacheKnowledge::Unknown;
    std::optional<std::uint32_t> word_;
    std::optional<qt::String> last_;
    std::string processQualification_, bindingQualification_;
};

enum class MessageAreaCacheStage { AwaitInitialization, BeforeClearVirtual, BeforeColorAppend, DuplicateSkip };
enum class MessageAreaCacheStop {
    InitializationKnowledgeUnavailable, NativeInitializationEffectsUnavailable,
    ClearVirtualUnavailable, ColorAppendEffectsUnavailable, DuplicateMessageSkipped
};
struct MessageAreaCacheResult {
    MessageAreaCacheStop stop;
    bool comparisonCommittedThisAttempt;
    const char* nextSourceSite;
};

// Binds a single already-entered handler invocation. Receiver liveness, QObject
// dispatch and QColor argument-copy effects precede this helper and are not
// implied by construction. The process cache is weakly referenced metadata.
class MessageAreaCacheProgress final {
public:
    MessageAreaCacheProgress(const std::shared_ptr<MessageAreaProcessCache>&,
        qt::String message, bool clearBeforeCompare, std::string sourceOccurrence,
        std::string handlerQualification);
    MessageAreaCacheProgress(const MessageAreaCacheProgress&) = delete;
    MessageAreaCacheProgress& operator=(const MessageAreaCacheProgress&) = delete;
    MessageAreaCacheProgress(MessageAreaCacheProgress&&) = delete;
    MessageAreaCacheProgress& operator=(MessageAreaCacheProgress&&) = delete;
    MessageAreaCacheStage stage() const noexcept { return stage_; }
    const qt::String& incomingMessage() const noexcept { return incoming_; }
private:
    friend MessageAreaCacheResult attemptMessageAreaCache(const std::shared_ptr<MessageAreaProcessCache>&,
                                                         MessageAreaCacheProgress&);
    std::weak_ptr<MessageAreaProcessCache> process_;
    qt::String incoming_;
    bool clear_;
    MessageAreaCacheStage stage_ = MessageAreaCacheStage::AwaitInitialization;
};

// Initialized + clear=false + equal content implements only d1527c's skip.
// Clear=true guards before virtual+1c0 even for equal messages. Different content
// guards before setTextColor/append and NEVER updates lastMessage early: native
// assignment d152ad follows append's return at d152a1. No bypass API is supplied.
MessageAreaCacheResult attemptMessageAreaCache(const std::shared_ptr<MessageAreaProcessCache>&,
                                              MessageAreaCacheProgress&);

enum class MessageAreaCacheResetCause { NormalClose, ReceiverTeardown, ProcessExit };
enum class MessageAreaCacheResetStop { NativeResetUnavailable };
struct MessageAreaCacheResetResult {
    MessageAreaCacheResetStop stop;
    const char* unavailableBoundary;
};
// All reset effects are unavailable. Normal Close/receiver death establish no
// reset; process cleanup callback141dfb720 still needs actual CRT/QString effects.
// Owner destruction releases adapter metadata only, never native reset/expiry.
MessageAreaCacheResetResult attemptMessageAreaCacheReset(const std::shared_ptr<MessageAreaProcessCache>&,
                                                        MessageAreaCacheResetCause);
} // namespace m2m::engine
