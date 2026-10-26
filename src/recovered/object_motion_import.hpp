#pragma once

#include "object_motion.hpp"

namespace deform::recovered {
[[nodiscard]] int read_object_motion(ObjectMotion& motion, int dimension,
                                     KeywordReader& reader);
}
