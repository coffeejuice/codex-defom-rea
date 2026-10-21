#include "frcmod_properties.hpp"
#include "relation_properties.hpp"

#include <cmath>
#include <limits>
#include <stdexcept>

namespace deform::recovered {
namespace {
int count(double value) {
    const auto integral = std::trunc(value);
    if (!std::isfinite(value) || integral < std::numeric_limits<int>::min() ||
        integral > std::numeric_limits<int>::max())
        throw std::invalid_argument("FRCMOD table count outside finite signed32 domain");
    return static_cast<int>(integral);
}
}

int read_frcmod(FrcmodProperties& p, KeywordReader& reader) {
    const auto header = read_relation_header(reader, RelationHeaderKind::integer_five_reals);
    if (header.status != 0) return 2;
    const int selector = header.integers[0];
    const auto& v = header.reals;
    p.scalars_0x1ad8[5] = v[0];
    p.scalar_0x1b88 = v[4]; //Both common stores precede selector validation.
    if (selector >= 0 && selector <= 10) {
        p.mode_0x1ad0 = selector;
        if (selector == 2) p.scalars_0x1ad8[0] = v[1];
        else if (selector == 4) p.scalars_0x1ad8[1] = v[1];
        else if (selector == 5) p.scalars_0x1ad8[2] = v[1];
        else if (selector == 7) {
            p.scalars_0x1ad8[3] = v[1];
            p.scalars_0x1ad8[4] = v[2];
        }
    } else if (selector == 11) {
        p.mode_0x1ad0 = 12;
        const auto read = read_relation_header(reader, RelationHeaderKind::nine_reals);
        constexpr std::array<std::size_t, 9> destinations{0, 1, 4, 5, 6, 7, 8, 2, 3};
        for (std::size_t i = 0; i < destinations.size(); ++i)
            p.coefficients_0x1c80[destinations[i]] = read.reals[i];
        return read.status == 0 ? 0 : 2;
    } else if (selector == 12) {
        p.mode_0x1ad0 = 13;
        p.scalar_0x1b88 = 100.0;
        auto& table = p.tables[1];
        table.counts_0x1c[0] = count(v[1]);
        table.rank_0x04 = 1;
        table.selectors_0x10[0] = 31;
        return read_property_values(reader, table) == 0 ? 0 : 2;
    } else if (selector == 15) {
        p.mode_0x1ad0 = 15;
        p.scalars_0x1ad8[2] = v[1];
    } else if (selector == 16) p.mode_0x1ad0 = 16;
    else if (selector == 17) {
        p.mode_0x1ad0 = 17;
        auto& table = p.tables[2];
        table.counts_0x1c[0] = count(v[1]);
        return read_property_values(reader, table) == 0 ? 0 : 2;
    } else if (selector < 0) {
        p.mode_0x1ad0 = 11;
        if (selector == std::numeric_limits<int>::min())
            throw std::invalid_argument("FRCMOD INT_MIN negation remains outside recovered domain");
        p.reference_0x1b08 = -selector;
    }
    return 0; //Unknown positive selectors keep the prior mode and inline fields.
}
}
