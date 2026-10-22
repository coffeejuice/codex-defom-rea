#include "object_output_range_import.hpp"

#include <limits>
#include <stdexcept>

namespace deform::recovered {
int read_object_output_range(ObjectOutputRange& fields, KeywordReader& reader) {
    const auto header = reader.token_header(6);
    if (header.status != 0) return 2;
    const auto token = [&header](std::size_t i) {
        return i < header.tokens.size() ? std::string_view(header.tokens[i]) : std::string_view{};
    };
    const int selector = KeywordReader::decimal_token_integer(token(0));
    fields.values_0x1ef8[0] = KeywordReader::legacy_token_real(token(1));
    fields.values_0x1ef8[1] = KeywordReader::legacy_token_real(token(2));
    fields.option_0x1f08 = header.token_count < 4 ? 0 :
        KeywordReader::decimal_token_integer(token(3));
    if (selector < 0) {
        fields.mode_0x1eec = 3;
        if (selector == std::numeric_limits<int>::min())
            throw std::invalid_argument("OTPRNG INT_MIN reference negation remains uncharacterized");
        fields.reference_0x1ef0 = -selector;
    } else fields.mode_0x1eec = selector; //Inactive reference remains untouched.
    return 0;
}
}
