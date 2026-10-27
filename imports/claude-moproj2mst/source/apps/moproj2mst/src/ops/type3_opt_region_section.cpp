#include "type3_opt_region_section.hpp"

#include "type3_optimizer_defopt_view.hpp"

#include <utility>

namespace m2m::mst::detail {
namespace {

using Event = Type3OptRegionSectionEvent;

Event literal(const char* value)
{
    Event event;
    event.kind = Event::Kind::Literal;
    event.literal = value;
    return event;
}

Event integer(std::int32_t value)
{
    Event event;
    event.kind = Event::Kind::Integer;
    event.integer = value;
    return event;
}

Event string(qt::String value)
{
    Event event;
    event.kind = Event::Kind::String;
    event.string = std::move(value);
    return event;
}

Event endl()
{
    Event event;
    event.kind = Event::Kind::Endl;
    return event;
}

}  // namespace

void emitType3OptRegionSectionWithCallbacks(
    std::int32_t count,
    const Type3OptRegionNameGetter& regionNameAt,
    const Type3OptRegionSectionSink& sink)
{
    if (count < 1) {
        sink(literal("REGIONS\t\t0"));
        sink(endl());
        return;
    }

    sink(literal("REGIONS    \t\t"));
    sink(integer(count));
    sink(endl());
    for (std::int32_t index = 0; index < count; ++index) {
        auto preparedName = qt::L("Opt/") + regionNameAt(index) + qt::L(".GEO");
        sink(string(std::move(preparedName)));
        sink(endl());
    }
}

void emitType3OptRegionSection(
    const Type3OptimizerDefOptView& view,
    const Type3OptRegionSectionSink& sink)
{
    emitType3OptRegionSectionWithCallbacks(
        view.regionCount(),
        [&view](std::int32_t index) { return view.regionNameAt(index); },
        sink);
}

std::vector<Type3OptRegionSectionEvent> planType3OptRegionSection(
    const Type3OptimizerDefOptView& view)
{
    std::vector<Event> events;
    emitType3OptRegionSection(view, [&events](const Event& event) { events.push_back(event); });
    return events;
}

}  // namespace m2m::mst::detail
