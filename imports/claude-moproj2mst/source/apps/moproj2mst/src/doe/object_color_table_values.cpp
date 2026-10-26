#include "object_color_table_values.hpp"

#include <cstddef>

#include "m2m/datafiles.hpp"

namespace m2m::doe {

bool readIsolatedObjectColorTableValues(datafiles::TextFile* stream,
    datafiles::TokenReader& reader, ObjectColorTableValues& values,
    std::int32_t& firstTrailingValue, std::int32_t& secondTrailingValue)
{
    if (!stream) return false;
    using Reader = datafiles::TokenReader;
    std::int32_t rowId = 0, red = 0, green = 0, blue = 0, alpha = 0;
    const auto readComponent = [&](std::int32_t& destination) {
        (void)reader.read(*stream, Reader::Int, &destination, 0);
    };
    for (int attempt = 0; attempt < 100; ++attempt) {
        const int tokenCount = reader.read(*stream, Reader::Int, &rowId, Reader::kNewLine);
        if (rowId < 1 || rowId > 100) continue;
        const auto row = static_cast<std::size_t>(rowId - 1);
        const bool rgba = tokenCount != 13;
        for (std::size_t table = 0; table < 4; ++table) {
            readComponent(red);
            readComponent(green);
            readComponent(blue);
            auto& color = values.tables[table][row];
            color[0] = static_cast<std::uint8_t>(red);
            color[1] = static_cast<std::uint8_t>(green);
            color[2] = static_cast<std::uint8_t>(blue);
            // The native first four tables store RGB before reading alpha.
            if (rgba) {
                readComponent(alpha);
                color[3] = static_cast<std::uint8_t>(alpha);
            }
        }
        if (rgba) {
            for (std::size_t table = 4; table < 6; ++table) {
                readComponent(red);
                readComponent(green);
                readComponent(blue);
                readComponent(alpha);
                // Native last two tables read all four before any store.
                values.tables[table][row] = {static_cast<std::uint8_t>(red),
                    static_cast<std::uint8_t>(green), static_cast<std::uint8_t>(blue),
                    static_cast<std::uint8_t>(alpha)};
            }
        }
        // EOF/errors do not break or reset token destinations in cdeab0.
    }
    (void)reader.read(*stream, Reader::Int, &firstTrailingValue, Reader::kNewLine);
    (void)reader.read(*stream, Reader::Int, &secondTrailingValue, 0);
    return true;
}

} // namespace m2m::doe
