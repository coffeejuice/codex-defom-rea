#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "m2m/doe_study.hpp"

namespace m2m::doe {

// Typed prefix of the native 0xa8 keyword record. The existing Item projection
// does not carry this scalar metadata. Text/selector setters after the pending
// notification have not run, even when their constructor values happen to match.
struct ScalarKeywordPrefixRecord {
    int keywordId = 8;                  // +0x2c, PE metadata ID for TLOC
    int unitType = 0;                   // +0x28
    String parameterName;              // +0x00
    String nominalText, upperText, lowerText; // +0x10/+0x18/+0x20
    double lowerValue = 0.0, upperValue = 0.0; // +0x38/+0x40
    double nominalValue = 0.0;          // +0x48
    int parameter = -1;                // +0x80 constructor value
    bool function = false, enumeration = false; // +0x31/+0x32
    bool lowerDefined = false, upperDefined = false; // +0x33/+0x34
    std::vector<int> entities, operations; // +0x50/+0x60
};

struct ScalarVariablePrefixRecord : Var {
    explicit ScalarVariablePrefixRecord(int samples);
    int numSamples;                    // +0x10
    int f14 = 0;                       // +0x14
    int dimension = 3;                 // +0x24
    String description;                // +0x08
    std::vector<ScalarKeywordPrefixRecord> keywords; // typed native +0x30 list
};

// Native locals retained at the unresolved delivery boundary, not a delivered
// signal or setter effects. The B121 scalar path never invokes 0x140916530.
struct PendingScalarVariableSignal {
    String key = qt::L("2signalSetOprHasDoe()");
    bool value = true;
    int operation = 0;
    int parsedParameter = -1;
    String collectedNominalText;
};

// Value/allocation diagnostics only: no Study, Session, I/O or callback back edge.
// A shared observer may outlive those owners without implying native liveness.
class B121ScalarPrefixState final {
public:
    const ScalarVariablePrefixRecord* variable() const noexcept { return variable_.get(); }
    const std::optional<PendingScalarVariableSignal>& pendingSignal() const noexcept { return pending_; }
    const std::optional<int>& selectedIndex() const noexcept { return selectedIndex_; }
    bool started() const noexcept { return started_; }

private:
    friend class B121ScalarVariablePrefix;
    std::unique_ptr<ScalarVariablePrefixRecord> variable_;
    std::optional<PendingScalarVariableSignal> pending_;
    std::optional<int> selectedIndex_;
    bool started_ = false;
};

// Isolated actual-owner entry, not wired to the public InputTable reader. The
// same pending notification guard throws, but concrete prefix data survives on
// Study. Re-entry refuses; there is no reset, dispatch or successful-parse token.
[[noreturn]] void readOwnedB121ScalarPrefix(engine::ConversionSession& session,
    Project& project, SubProject& sub, const xml::Node& doeVariables);

// Detached callers retain their original cleanup behavior. The explicit Session
// entry below instead leaves data on Study when this stack driver unwinds.
class B121ScalarVariablePrefix {
public:
    explicit B121ScalarVariablePrefix(std::shared_ptr<Study> study);
    ~B121ScalarVariablePrefix();
    B121ScalarVariablePrefix(const B121ScalarVariablePrefix&) = delete;
    B121ScalarVariablePrefix& operator=(const B121ScalarVariablePrefix&) = delete;

    // Bounded fresh mode0/TLOC/double prefix only. Stops before the unresolved
    // messenger delivery, retaining earlier mutations. Never reports parse success.
    [[noreturn]] void read(const xml::Node& doeVariables);
    const ScalarVariablePrefixRecord* variable() const noexcept { return state_->variable(); }
    const std::optional<PendingScalarVariableSignal>& pendingSignal() const noexcept { return state_->pendingSignal(); }
    const std::optional<int>& selectedIndex() const noexcept { return state_->selectedIndex(); } // model+0x74

private:
    friend void readOwnedB121ScalarPrefix(engine::ConversionSession&, Project&,
                                         SubProject&, const xml::Node&);
    void retainOnStudy();
    void requireFresh(const xml::Node& doeVariables) const;
    void requireBinding() const;
    [[noreturn]] void readKeyword(const xml::Node& keyword);
    std::shared_ptr<Study> study_;
    std::shared_ptr<StudyIo> io_;
    Project* project_ = nullptr;
    SubProject* sub_ = nullptr;
    std::shared_ptr<B121ScalarPrefixState> state_;
    bool retained_ = false;
};

} // namespace m2m::doe
