#pragma once
#include <array>
#include <string>
#include <vector>

namespace m2m::mst::detail {

// Prepared insertion phase order only; recovered getters are not executed here.
// Native formatting and physical bytes are not modeled. Each collection
// supplies its separately prepared first-member insertions only when nonempty.
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
