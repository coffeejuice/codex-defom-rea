#include "m2m/engine/message_area_cache.hpp"

#include <utility>

#include "m2m/common.hpp"

namespace m2m::engine {
namespace {
[[noreturn]] void unavailable(const char* detail)
{
    throw Error(M2M_E_UNSUPPORTED, std::string("MessageArea process cache: ") + detail);
}
void requireQualification(const std::string& value)
{
    if (value.find_first_not_of(" \t\r\n") == std::string::npos)
        unavailable("source qualification is unavailable");
}
}

MessageAreaProcessCache::MessageAreaProcessCache(std::string qualification)
    : processQualification_(std::move(qualification))
{
    requireQualification(processQualification_);
}

std::shared_ptr<MessageAreaProcessCache> MessageAreaProcessCache::create(std::string qualification)
{
    return std::shared_ptr<MessageAreaProcessCache>(new MessageAreaProcessCache(std::move(qualification)));
}

bool MessageAreaProcessCache::bindObservedUninitialized(std::uint32_t word, std::string qualification)
{
    requireQualification(qualification);
    if (word & 1u) unavailable("uninitialized binding requires initialization bit0 clear");
    if (knowledge_ != MessageAreaCacheKnowledge::Unknown) {
        if (knowledge_ == MessageAreaCacheKnowledge::KnownUninitialized && word_ == word) return false;
        unavailable("conflicting state rebind bypasses guarded native effects");
    }
    word_ = word;
    bindingQualification_ = std::move(qualification);
    knowledge_ = MessageAreaCacheKnowledge::KnownUninitialized;
    return true;
}

bool MessageAreaProcessCache::bindObservedInitialized(std::uint32_t word, const qt::String& last,
    std::string qualification)
{
    requireQualification(qualification);
    if (!(word & 1u)) unavailable("initialized binding requires bit0 and completed QString initialization");
    if (knowledge_ != MessageAreaCacheKnowledge::Unknown) {
        if (knowledge_ == MessageAreaCacheKnowledge::KnownInitialized && word_ == word &&
            *last_ == last && last_->isNull() == last.isNull()) return false;
        unavailable("conflicting state rebind bypasses guarded native effects");
    }
    last_ = last;
    word_ = word;
    bindingQualification_ = std::move(qualification);
    knowledge_ = MessageAreaCacheKnowledge::KnownInitialized;
    return true;
}

MessageAreaCacheProgress::MessageAreaCacheProgress(const std::shared_ptr<MessageAreaProcessCache>& process,
    qt::String message, bool clearBeforeCompare, std::string sourceOccurrence, std::string qualification)
    : process_(process), incoming_(std::move(message)), clear_(clearBeforeCompare)
{
    if (!process) unavailable("actual process owner identity is unavailable");
    requireQualification(qualification);
    if (sourceOccurrence != messageAreaCacheSourceOccurrence)
        unavailable("handler source occurrence is unavailable");
}

MessageAreaCacheResult attemptMessageAreaCache(const std::shared_ptr<MessageAreaProcessCache>& process,
    MessageAreaCacheProgress& progress)
{
    if (!process || progress.process_.lock() != process)
        unavailable("progress does not bind this process owner identity");
    if (progress.stage_ == MessageAreaCacheStage::AwaitInitialization) {
        if (process->knowledge() == MessageAreaCacheKnowledge::Unknown)
            return {MessageAreaCacheStop::InitializationKnowledgeUnavailable, false,
                    "0x140d15211:process initialization-word observation"};
        if (process->knowledge() == MessageAreaCacheKnowledge::KnownUninitialized)
            return {MessageAreaCacheStop::NativeInitializationEffectsUnavailable, false,
                    "0x140d1521e:before initialization-bit/QString/cleanup-registration effects"};
        if (progress.clear_) {
            progress.stage_ = MessageAreaCacheStage::BeforeClearVirtual;
        } else if (progress.incoming_ == *process->lastMessage()) {
            progress.stage_ = MessageAreaCacheStage::DuplicateSkip;
            return {MessageAreaCacheStop::DuplicateMessageSkipped, true,
                    "0x140d15304:duplicate-message handler epilogue"};
        } else {
            progress.stage_ = MessageAreaCacheStage::BeforeColorAppend;
            return {MessageAreaCacheStop::ColorAppendEffectsUnavailable, true,
                    "0x140d1528b:setTextColor before append/cache assignment"};
        }
    }
    if (progress.stage_ == MessageAreaCacheStage::BeforeClearVirtual)
        return {MessageAreaCacheStop::ClearVirtualUnavailable, false,
                "0x140d1525d:receiver virtual+1c0 before comparison"};
    if (progress.stage_ == MessageAreaCacheStage::BeforeColorAppend)
        return {MessageAreaCacheStop::ColorAppendEffectsUnavailable, false,
                "0x140d1528b:setTextColor before append/cache assignment"};
    return {MessageAreaCacheStop::DuplicateMessageSkipped, false,
            "0x140d15304:duplicate-message handler epilogue"};
}

MessageAreaCacheResetResult attemptMessageAreaCacheReset(
    const std::shared_ptr<MessageAreaProcessCache>& process, MessageAreaCacheResetCause cause)
{
    if (!process) unavailable("actual process owner identity is unavailable");
    switch (cause) {
    case MessageAreaCacheResetCause::NormalClose:
        return {MessageAreaCacheResetStop::NativeResetUnavailable,
                "normal Close establishes no process last-message reset"};
    case MessageAreaCacheResetCause::ReceiverTeardown:
        return {MessageAreaCacheResetStop::NativeResetUnavailable,
                "receiver teardown establishes no process last-message reset"};
    case MessageAreaCacheResetCause::ProcessExit:
        return {MessageAreaCacheResetStop::NativeResetUnavailable,
                "0x141dfb72b:native QString destructor/CRT exit effects unavailable"};
    }
    unavailable("unknown reset cause");
}
} // namespace m2m::engine
