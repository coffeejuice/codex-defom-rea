#pragma once

#include <cstddef>
#include <memory>
#include <optional>
#include <string>

namespace m2m::engine {

class WidgetAreaWindowOwner;
class WidgetAreaStackOwner;
class WidgetAreaScrollOwner;
class WidgetAreaImplOwner;
class WidgetAreaChildOwner;
class WidgetAreaLazyParent;
class WidgetAreaLazyGetter;

// Typed diagnostic storage only. Creating a record neither constructs a native
// QObject/QWidget nor establishes native liveness, callbacks or parenthood.
class WidgetAreaLifetimeOwner {
public:
    WidgetAreaLifetimeOwner(const WidgetAreaLifetimeOwner&) = delete;
    WidgetAreaLifetimeOwner& operator=(const WidgetAreaLifetimeOwner&) = delete;
    const void* allocationIdentity() const noexcept { return identity_; }
    std::shared_ptr<const void> allocationLease() const noexcept { return allocation_.lock(); }
    std::optional<bool> nativeAlive() const noexcept { return alive_; }
    bool observationScopeAvailable() const noexcept;
    bool usableNativeOwner() const noexcept;
    bool matchesCanonicalLease(const std::shared_ptr<WidgetAreaLifetimeOwner>&) const noexcept;
    void observeNativeAlive(bool, const std::string& qualification);
    // Diagnostic revocation, never an assertion that native destruction ran.
    void releaseAdmissionScope() noexcept { admitted_ = false; }
protected:
    WidgetAreaLifetimeOwner(const std::shared_ptr<const void>&, const std::string&);
    void bindCanonical(const std::shared_ptr<WidgetAreaLifetimeOwner>&);
    void requireObservation(const std::string&) const;
private:
    const void* identity_;
    std::weak_ptr<const void> allocation_;
    std::weak_ptr<WidgetAreaLifetimeOwner> canonical_;
    std::optional<bool> alive_;
    bool admitted_ = true;
};

// Unavailable, observed null, and a nonnull pointer whose owner is now unavailable
// are different states. Expiry/revocation does not fabricate a native null store.
template<class T> struct WidgetAreaReferenceSnapshot {
    bool known = false, nonNull = false;
    std::shared_ptr<T> target;
    bool usableNativeTarget() const noexcept {
        return known && nonNull && target && target->usableNativeOwner();
    }
};

template<class T> class WidgetAreaObservedReference {
public:
    WidgetAreaReferenceSnapshot<T> snapshot() const noexcept {
        return {known_, nonNull_, target_.lock()};
    }
private:
    friend class WidgetAreaWindowOwner;
    friend class WidgetAreaScrollOwner;
    friend class WidgetAreaImplOwner;
    bool known_ = false, nonNull_ = false;
    std::weak_ptr<T> target_;
    void observeInitial(const std::shared_ptr<T>&);
    void assignObserved(const std::shared_ptr<T>&);
};

// One raw main-window+3c8 cell. Neither storage nor a historical constructor
// return establishes a current value. Typed binding and native liveness differ.
struct WidgetAreaFieldSnapshot {
    bool known = false, nonNull = false;
    const void* pointerIdentity = nullptr;
    std::shared_ptr<const void> allocation;
    std::size_t revision = 0;
};
class WidgetAreaCanonicalCell final {
    friend class WidgetAreaWindowOwner;
    friend class WidgetAreaLazyParent;
    friend class WidgetAreaLazyGetter;
    explicit WidgetAreaCanonicalCell(const std::shared_ptr<const void>& p) : parent_(p) {}
    void requireScope() const;
    void observeInitial(const std::shared_ptr<const void>&);
    void assign(const std::shared_ptr<const void>&);
    WidgetAreaFieldSnapshot snapshot() const noexcept {
        return {known_, nonNull_, pointerIdentity_, allocation_.lock(), revision_};
    }
    std::weak_ptr<const void> parent_, allocation_;
    std::weak_ptr<WidgetAreaWindowOwner> window_;
    std::weak_ptr<WidgetAreaScrollOwner> typed_;
    std::weak_ptr<WidgetAreaLazyParent> lazy_;
    const void* pointerIdentity_ = nullptr;
    const WidgetAreaLazyGetter* active_ = nullptr;
    std::size_t revision_ = 0, commits_ = 0, lastStoreOccurrence_ = 0;
    bool known_ = false, nonNull_ = false, typedBound_ = false;
    bool windowBound_ = false, admitted_ = true;
};

enum class WidgetAreaChildKind { Unknown, Placeholder, BooleanEditor, Other };
class WidgetAreaChildOwner final : public WidgetAreaLifetimeOwner {
public:
    static std::shared_ptr<WidgetAreaChildOwner> create(const std::shared_ptr<const void>&,
                                                     const std::string& qualification);
    WidgetAreaChildKind kind() const noexcept { return kind_; }
    void observeKind(WidgetAreaChildKind, const std::string& qualification);
private:
    WidgetAreaChildOwner(const std::shared_ptr<const void>& a, const std::string& q)
        : WidgetAreaLifetimeOwner(a, q) {}
    WidgetAreaChildKind kind_ = WidgetAreaChildKind::Unknown;
};

class WidgetAreaStackOwner final : public WidgetAreaLifetimeOwner {
public:
    static std::shared_ptr<WidgetAreaStackOwner> create(const std::shared_ptr<const void>&,
                                                     const std::string& qualification);
    std::optional<int> currentIndex() const noexcept { return index_; }
    void observeInitialCurrentIndex(int, const std::string& qualification);
private:
    WidgetAreaStackOwner(const std::shared_ptr<const void>& a, const std::string& q)
        : WidgetAreaLifetimeOwner(a, q) {}
    std::optional<int> index_;
};

class WidgetAreaWindowOwner final : public WidgetAreaLifetimeOwner {
public:
    static std::shared_ptr<WidgetAreaWindowOwner> create(const std::shared_ptr<const void>&,
                                                      const std::string& qualification);
    WidgetAreaReferenceSnapshot<WidgetAreaScrollOwner> area3c8() const noexcept {
        return {area_->known_, area_->nonNull_, area_->typed_.lock()};
    }
    WidgetAreaFieldSnapshot area3c8Field() const noexcept { return area_->snapshot(); }
    const void* canonicalAreaFieldIdentity() const noexcept { return area_.get(); }
    WidgetAreaReferenceSnapshot<WidgetAreaStackOwner> stack480() const noexcept { return stack_.snapshot(); }
    void observeInitialArea3c8(const std::shared_ptr<WidgetAreaScrollOwner>&, const std::string&);
    void observeInitialStack480(const std::shared_ptr<WidgetAreaStackOwner>&, const std::string&);
private:
    friend class WidgetAreaLazyParent;
    WidgetAreaWindowOwner(const std::shared_ptr<const void>& a, const std::string& q)
        : WidgetAreaLifetimeOwner(a, q), area_(new WidgetAreaCanonicalCell(a)) {}
    std::shared_ptr<WidgetAreaCanonicalCell> area_;
    WidgetAreaObservedReference<WidgetAreaStackOwner> stack_;
};

class WidgetAreaScrollOwner final : public WidgetAreaLifetimeOwner {
public:
    static std::shared_ptr<WidgetAreaScrollOwner> create(const std::shared_ptr<const void>&,
                                                      const std::string& qualification);
    WidgetAreaReferenceSnapshot<WidgetAreaImplOwner> impl28() const noexcept { return impl_.snapshot(); }
    WidgetAreaReferenceSnapshot<WidgetAreaChildOwner> scrollContent() const noexcept { return content_.snapshot(); }
    void observeInitialImpl28(const std::shared_ptr<WidgetAreaImplOwner>&, const std::string&);
    void observeInitialScrollContent(const std::shared_ptr<WidgetAreaChildOwner>&, const std::string&);
private:
    WidgetAreaScrollOwner(const std::shared_ptr<const void>& a, const std::string& q)
        : WidgetAreaLifetimeOwner(a, q) {}
    WidgetAreaObservedReference<WidgetAreaImplOwner> impl_;
    WidgetAreaObservedReference<WidgetAreaChildOwner> content_;
};

// This mirrors qualified source-point field observations, not a replacement
// executor. No method runs/marks complete layout, parenting, show, Enter, timer,
// notifier, deleteLater, scroll-area or first-Open effects.
class WidgetAreaImplOwner final : public WidgetAreaLifetimeOwner {
public:
    static std::shared_ptr<WidgetAreaImplOwner> create(const std::shared_ptr<const void>&,
                                                    const std::string& qualification);
    WidgetAreaReferenceSnapshot<WidgetAreaScrollOwner> area28() const noexcept { return area_.snapshot(); }
    WidgetAreaReferenceSnapshot<WidgetAreaChildOwner> current18() const noexcept { return current_.snapshot(); }
    WidgetAreaReferenceSnapshot<WidgetAreaChildOwner> saved20() const noexcept { return saved_.snapshot(); }
    WidgetAreaReferenceSnapshot<WidgetAreaChildOwner> pending38() const noexcept { return pending_.snapshot(); }
    std::optional<bool> deleteMode10() const noexcept { return mode_; }
    std::optional<bool> placeholderEnabled11() const noexcept { return placeholder_; }
    void observeInitialArea28(const std::shared_ptr<WidgetAreaScrollOwner>&, const std::string&);
    void observeInitialCurrent18(const std::shared_ptr<WidgetAreaChildOwner>&, const std::string&);
    void observeInitialSaved20(const std::shared_ptr<WidgetAreaChildOwner>&, const std::string&);
    void observeInitialPending38(const std::shared_ptr<WidgetAreaChildOwner>&, const std::string&);
    void observeInitialFlags(std::optional<bool> mode10, std::optional<bool> placeholder11,
                             const std::string& qualification);
    // These calls require witnesses of these actual native writes. They do not
    // infer that the intervening Qt callbacks were implemented or neutral.
    void observePendingCopyAtDadcc3(const std::shared_ptr<WidgetAreaChildOwner>& old, const std::string&);
    void observeCurrentClearAtDadfec(const std::shared_ptr<WidgetAreaChildOwner>& old, const std::string&);
    void observeCurrentStoreAtDae213(const std::shared_ptr<WidgetAreaChildOwner>& incoming,
        const std::shared_ptr<WidgetAreaChildOwner>& oldAtEntry, const std::string&);
    [[noreturn]] void requireAdoptionEffects() const;
private:
    WidgetAreaImplOwner(const std::shared_ptr<const void>& a, const std::string& q)
        : WidgetAreaLifetimeOwner(a, q) {}
    void requireLiveObservedOld(const std::shared_ptr<WidgetAreaChildOwner>&) const;
    WidgetAreaObservedReference<WidgetAreaScrollOwner> area_;
    WidgetAreaObservedReference<WidgetAreaChildOwner> current_, saved_, pending_;
    std::optional<bool> mode_, placeholder_;
};

} // namespace m2m::engine
