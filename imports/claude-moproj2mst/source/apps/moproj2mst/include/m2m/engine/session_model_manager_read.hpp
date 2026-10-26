// Actual candidate Session owner/read receipt; not a native factory witness.
#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>

namespace m2m::engine {
class ConversionSession;
class ModelManager;
enum class SessionModelManagerReadOrigin { CandidateExistingOwnerRead };

class SessionModelManagerReadReceipt final {
public:
    SessionModelManagerReadReceipt(const SessionModelManagerReadReceipt&) = delete;
    SessionModelManagerReadReceipt& operator=(const SessionModelManagerReadReceipt&) = delete;
    SessionModelManagerReadReceipt(SessionModelManagerReadReceipt&&) = delete;
    SessionModelManagerReadReceipt& operator=(SessionModelManagerReadReceipt&&) = delete;
    SessionModelManagerReadOrigin origin() const noexcept {
        return SessionModelManagerReadOrigin::CandidateExistingOwnerRead;
    }
    // Source correspondence only. Public convert currently prepares before
    // suffix acceptance, so this does not prove native Open reached this site.
    static constexpr const char* sourceSite = "0x14001d078:0x140c9b7ca(main+0x50 read)";
    static constexpr const char* qualification =
        "actual candidate Session manager field/backlink read before child allocation; not native execution";
    static constexpr bool nativeFactoryCompleted = false;
    static constexpr bool nativeConcreteMoClassObserved = false;
    static constexpr bool nativeWrapperQualified = false;
    static constexpr bool mainWindowReady = false;
    static constexpr bool completeForPublicClose = false;

    // Historical candidate identity/counts, never current native facts.
    // Opaque historical token; not a pointer, native address or live-target API.
    std::uintptr_t observedManagerIdentityToken() const noexcept { return managerIdentity_; }
    std::size_t modelCountAtRead() const noexcept { return modelsAtRead_; }
    std::size_t connectionCountAtRead() const noexcept { return connectionsAtRead_; }
    bool sessionScopeAvailable() const noexcept { return admitted_; }
    // Revalidates this exact live Session and canonical manager; does not make
    // any promise about its current population, Project or native Qt lifetime.
    std::shared_ptr<ModelManager> requireManagerFor(const ConversionSession&) const;
private:
    friend class ConversionSession;
    SessionModelManagerReadReceipt(const ConversionSession&, const std::shared_ptr<ModelManager>&);
    void revokeSessionScope() noexcept { admitted_ = false; } // adapter revocation, not native death
    const ConversionSession* session_;
    std::weak_ptr<ModelManager> manager_;
    std::uintptr_t managerIdentity_;
    std::size_t modelsAtRead_, connectionsAtRead_;
    bool admitted_ = true;
};
} // namespace m2m::engine
