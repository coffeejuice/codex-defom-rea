#pragma once

#include <cstdint>
#include <functional>
#include <vector>

#include "m2m/qt4.hpp"

namespace m2m::mst::detail {

class Type3OptimizerDefOptView;

// Typed insertion events for the private Type3 .OPT REGIONS section.
// These describe prepared values and QTextStream insertion kinds; they do not
// serialize Qt bytes or model native helper execution.
struct Type3OptRegionSectionEvent {
    enum class Kind {
        Literal,
        Integer,
        String,
        Endl,
    };

    Kind kind = Kind::Endl;
    const char* literal = nullptr;
    std::int32_t integer = 0;
    qt::String string;
};

using Type3OptRegionSectionSink =
    std::function<void(const Type3OptRegionSectionEvent&)>;
using Type3OptRegionNameGetter = std::function<qt::String(std::int32_t)>;

// Streaming lower core for explicit getter and sink ordering. Exceptions from
// either callback propagate immediately; no later region getter is invoked.
void emitType3OptRegionSectionWithCallbacks(
    std::int32_t count,
    const Type3OptRegionNameGetter& regionNameAt,
    const Type3OptRegionSectionSink& sink);

// View adapter reads count once and obtains each name only when its row is due.
void emitType3OptRegionSection(
    const Type3OptimizerDefOptView& view,
    const Type3OptRegionSectionSink& sink);

// Projects the CP465/CP473 count branch and ordered CP450 region-name values.
// The String event uses the CP465 caller operand order through the audited
// QString left-plus-right helper: `Opt/` + name + `.GEO`. This prepares a
// QString event only; QString codec/QTextStream behavior, physical file bytes,
// and public Save admission are out of scope.
std::vector<Type3OptRegionSectionEvent> planType3OptRegionSection(
    const Type3OptimizerDefOptView& view);

}  // namespace m2m::mst::detail
