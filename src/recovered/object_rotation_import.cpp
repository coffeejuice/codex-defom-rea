#include "object_rotation_import.hpp"

#include <stdexcept>

namespace deform::recovered {
int read_object_rotation(int keyword, ObjectDirectImports& fields, KeywordReader& reader) {
    if (keyword == 259) {
        const auto read = reader.integer_real();
        fields.target_volume_0x2090 = read.real_value;
        fields.target_volume_mode_0x2098 = read.integer_value;
        if (fields.target_volume_0x2090 == 0.0) fields.target_volume_mode_0x2098 = -1;
        return read.status == 0 ? 0 : 2;
    }
    if (keyword != 258) throw std::logic_error("unexpected rotation/target-volume keyword");
    if (!fields.rotation_symmetry) return 0; //2D consumes no payload.
    auto& p = *fields.rotation_symmetry;
    const auto scalar = reader.real();
    p.scalar_0x2058 = scalar.values[0];
    if (scalar.status != 0) return 2;
    const auto second = reader.real_triple();
    p.vector_0x2078 = second.values;
    if (second.status != 0) return 2;
    const auto first = reader.real_triple();
    p.vector_0x2060 = first.values;
    return first.status == 0 ? 0 : 2;
}
}
