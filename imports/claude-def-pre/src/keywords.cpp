#include "deform/keywords.hpp"

#include <algorithm>
#include <array>
#include <string>
#include <unordered_map>

namespace dk {
namespace {

struct RawKeyword {
    int id;
    const char *name;
    int group;
    int flag;
};

constexpr RawKeyword kRaw[] = {
#include "keyword_table.inc"
};

struct Registry {
    std::array<KeywordInfo, std::size(kRaw)> items{};
    std::unordered_map<std::string_view, const KeywordInfo *> byName;

    Registry()
    {
        for (std::size_t i = 0; i < std::size(kRaw); ++i) {
            const auto &r = kRaw[i];
            items[i] = KeywordInfo{static_cast<std::int16_t>(r.id), r.name,
                                   static_cast<KeywordGroup>(r.group),
                                   static_cast<std::int16_t>(r.flag)};
        }
        std::sort(items.begin(), items.end(),
                  [](const KeywordInfo &a, const KeywordInfo &b) { return a.id < b.id; });
        byName.reserve(items.size() * 2);
        for (const auto &k : items) {
            byName.emplace(k.name, &k);
        }
    }
};

const Registry &registry()
{
    static const Registry r;
    return r;
}

} // namespace

std::span<const KeywordInfo> keywords() noexcept
{
    return registry().items;
}

const KeywordInfo *find_keyword(std::string_view name) noexcept
{
    const auto &m = registry().byName;
    const auto it = m.find(name);
    return it == m.end() ? nullptr : it->second;
}

std::string_view group_name(KeywordGroup group) noexcept
{
    switch (group) {
    case KeywordGroup::SimulationControl: return "simulation control";
    case KeywordGroup::Object: return "object";
    case KeywordGroup::InterObject: return "inter-object";
    case KeywordGroup::Material: return "material";
    case KeywordGroup::PhaseTransformation: return "phase transformation";
    case KeywordGroup::Command: return "command";
    case KeywordGroup::Dimension: return "dimension";
    default: return "unknown";
    }
}

} // namespace dk
