#pragma once

#include "keyword_reader.hpp"
#include "object_model.hpp"

#include <array>

namespace deform::recovered {

enum class RelationHeaderKind {
    integer_two_reals, two_integers_two_reals, four_reals, five_reals, six_reals, seven_reals
};

struct RelationHeaderRead {
    std::array<int, 2> integers{};
    std::array<double, 7> reals{};
    int integer_count = 0;
    int real_count = 0;
    int conversions = -1;
    int status = 1;
};

// 415680/415ff0/414e40/414f50/415080/4151c0: one bounded line, comma normalization,
// zeroed destinations, and failure only for fgets EOF or aggregate scanf -1.
[[nodiscard]] RelationHeaderRead read_relation_header(
    KeywordReader& reader, RelationHeaderKind kind);

// 383120 cases135..143 after exact ordered-pair selection/creation. PHASTF
// selection and layout refresh remain in KeywordImporter/ObjectModel.
[[nodiscard]] int read_relation_property(int keyword, RelationRecord& relation,
    KeywordReader& reader);

} // namespace deform::recovered
