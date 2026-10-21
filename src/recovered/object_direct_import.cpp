#include "object_direct_import.hpp"

#include <cmath>
#include <limits>
#include <stdexcept>

namespace deform::recovered {
int read_object_direct(int keyword, ObjectDirectImports& fields, KeywordReader& reader) {
    if (keyword == 148) {
        if (!fields.planar) return 0; //3D consumes no payload line.
        const auto read = reader.integer_real();
        fields.planar->zstr_mode_0x1f30 = read.integer_value;
        fields.planar->zstr_0x1f28 = read.real_value;
        return read.status == 0 ? 0 : 2;
    }
    const std::size_t index = keyword == 146 ? 1 : keyword == 147 ? 0 :
        keyword == 149 ? 2 : keyword == 150 ? 3 : 4;
    if (index == 4) throw std::logic_error("unexpected object direct keyword");
    if (keyword == 149 && fields.planar) {
        const auto read = reader.real_pair();
        fields.scalars_0x1ea0[index] = read.values[0];
        const double integral = std::trunc(read.values[1]);
        if (!std::isfinite(read.values[1]) || integral < std::numeric_limits<int>::min() ||
            integral > std::numeric_limits<int>::max())
            throw std::invalid_argument("PENVOL mode outside finite signed32 conversion domain");
        fields.planar->penvol_mode_0x1f20 = static_cast<int>(integral);
        return read.status == 0 ? 0 : 2;
    }
    const auto read = reader.real();
    fields.scalars_0x1ea0[index] = read.values[0]; //Live zeroing survives blank/EOF.
    return read.status == 0 ? 0 : 2;
}
}
