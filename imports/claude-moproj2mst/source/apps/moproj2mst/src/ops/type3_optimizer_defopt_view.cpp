#include "type3_optimizer_defopt_view.hpp"

#include <limits>

namespace m2m::mst::detail {
namespace {

template <typename Rows>
std::int32_t count(const Rows& rows) noexcept
{
    return rows.size() > static_cast<std::size_t>((std::numeric_limits<std::int32_t>::max)())
        ? (std::numeric_limits<std::int32_t>::max)()
        : static_cast<std::int32_t>(rows.size());
}

}  // namespace

Type3OptimizerDefOptView::Type3OptimizerDefOptView(
    const Type3OptimizerScalarState& state) noexcept : state_(state) {}

std::int32_t Type3OptimizerDefOptView::value0c() const noexcept { return state_.value0c; }
std::int32_t Type3OptimizerDefOptView::value10() const noexcept { return state_.value10; }
std::int32_t Type3OptimizerDefOptView::value30() const noexcept { return state_.value30; }
std::int32_t Type3OptimizerDefOptView::value34() const noexcept { return state_.value34; }
std::int32_t Type3OptimizerDefOptView::value38() const noexcept { return state_.value38; }
double Type3OptimizerDefOptView::value40() const noexcept { return state_.value40; }
bool Type3OptimizerDefOptView::value48() const noexcept { return state_.value48; }

std::int32_t Type3OptimizerDefOptView::numConCount() const noexcept { return count(state_.numConRows); }

const Type3NumConRowValues* Type3OptimizerDefOptView::numConAt(std::int32_t index) const noexcept
{
    if (index < 0 || index >= numConCount()) return nullptr;
    return &state_.numConRows[static_cast<std::size_t>(index)];
}

std::int32_t Type3OptimizerDefOptView::numConField1At(std::int32_t index) const noexcept
{
    const auto* row = numConAt(index);
    return row ? row->value04 : -1;
}

double Type3OptimizerDefOptView::numConField3At(std::int32_t index) const noexcept
{
    const auto* row = numConAt(index);
    return row ? row->value18 : 0.0;
}

std::int32_t Type3OptimizerDefOptView::numConField4At(std::int32_t index) const noexcept
{
    const auto* row = numConAt(index);
    return row ? row->value0c : -1;
}

std::int32_t Type3OptimizerDefOptView::numConField2At(std::int32_t index) const noexcept
{
    const auto* row = numConAt(index);
    return row ? row->value08 : -1;
}

std::int32_t Type3OptimizerDefOptView::regionCount() const noexcept { return count(state_.regionRows); }

qt::String Type3OptimizerDefOptView::regionNameAt(std::int32_t index) const
{
    if (index < 0 || index >= regionCount()) return qt::L("Selected Object");
    return state_.regionRows[static_cast<std::size_t>(index)].value08;
}

}  // namespace m2m::mst::detail
