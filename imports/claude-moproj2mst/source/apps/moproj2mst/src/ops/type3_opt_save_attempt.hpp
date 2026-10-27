#pragma once
#include "type3_optimizer_scalar_state.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace m2m::mst::detail {

// Prepared insertion phase order only; native formatting and physical bytes are not modeled.
// CP466 treated firstMember as stream output; CP468 shows these preliminary
// result locals are never inserted, even when supplied to this generic seam.
using Type3OptFragments = std::vector<std::string>;
struct Type3OptCollection {
    bool nonempty = false;
    Type3OptFragments firstMember;
};
struct Type3OptSavePrepared {
    std::string path;
    Type3OptFragments header;                 // includes the three header lines
    Type3OptFragments scalarPrefix;
    std::array<Type3OptCollection, 3> collections;
    Type3OptFragments scalarSuffix;
    std::vector<Type3OptFragments> rows;
    Type3OptFragments managerCountAndNumbers; // count and 1..count, not values
    Type3OptFragments emptySection;
    std::vector<Type3OptFragments> sectionRows;
    Type3OptFragments finalLines;
};

// Logical prepared identities, not an implementation of Qt copy-on-write.
// Snapshots record the values in each destroyed callee temporary; each writer
// original remains empty and its preliminary +0x60 branch is skipped.
struct Type3OptGroupReceipt {
    std::array<std::vector<std::int32_t>, 3> temporarySnapshots;
    std::array<std::size_t, 3> ignoredCounts{};
    std::array<std::vector<std::int32_t>, 3> writerOriginals;
    std::array<bool, 3> preliminaryBranchTaken{};
};

struct Type3OptAttemptSink {
    virtual ~Type3OptAttemptSink() = default;
    virtual bool open(const std::string& path) = 0;
    // Return status is an observation, never a branch in the ordinary path.
    virtual bool insert(const std::string& fragment) = 0;
    virtual bool close() = 0;
    virtual void assignOpenFailureDiagnostic(const std::string& path) = 0;
};

// Both ordinary paths return false. Exceptions/nonreturns and native Qt
// buffering, formatting, physical writes, and unwind cleanup are not modeled.
bool attemptType3OptSave(const Type3OptSavePrepared& prepared,
                         Type3OptAttemptSink& sink);

// Stable normally returning group profile. Receipt is assigned only after open,
// header and scalarPrefix complete; a failed open or earlier throw leaves it alone.
// This is a private value projection, not public Type3 admission, Qt bytes or
// an assertion about the native optimizer owner's lifetime.
bool attemptType3OptSaveWithGroups(const Type3OptSavePrepared& prepared,
                                   const Type3OptimizerScalarState& state,
                                   Type3OptAttemptSink& sink,
                                   Type3OptGroupReceipt& receipt);

struct Type3OptCallerEffects {
    virtual ~Type3OptCallerEffects() = default;
    virtual void appendStudy() = 0;
    virtual void saveInputTable() = 0;
    virtual void commonContinuation() = 0;
};

// Borrowed-pointer presence is a gate only; this seam makes no lifetime claim.
void attemptType3SaveCaller(bool optimizerPresent, bool managerPresent,
                            const Type3OptSavePrepared& prepared,
                            Type3OptAttemptSink& sink,
                            Type3OptCallerEffects& effects);

} // namespace m2m::mst::detail
