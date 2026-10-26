#pragma once

#include <functional>

namespace m2m {
struct Project;
struct Operation;
struct Ctx;
namespace data { struct BooleanData; }
namespace mst {

// Selected native writer return paths, NOT durable-byte/stream-status results.
enum class BooleanFileAttemptResult : int { OpenFailed = 0, OpenedPath = 1 };
struct BooleanSideFileAttempts {
    std::function<BooleanFileAttemptResult()> bwz;
    std::function<BooleanFileAttemptResult()> ini;
};

// 1411e1820: resolve actual data/properties/OPI owner; -1 if its owner guard
// resolves null, otherwise attempt BWZ then INI and return BWZ's integer only.
// One synchronous borrowed call; callbacks must not destroy/rebind its owners.
// Unsupported/foreign ownership is unavailable evidence, not native null.
int nativeBooleanSideFileSequence(Project&, Operation&, const data::BooleanData&,
                                  const BooleanSideFileAttempts&);

// Physical native-FileDevice entry. The existing void L5 adapter discards this
// result where original callers do. Recording/staged mode has no native result.
int booleanSideFilesNativeResult(Project&, Operation&, const data::BooleanData&, Ctx&);

} // namespace mst
} // namespace m2m
