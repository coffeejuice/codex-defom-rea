#pragma once

#include "object_model.hpp"

namespace deform::recovered {
//384a50 case0xE: import state only; scientific creep evaluation is separate.
[[nodiscard]] int read_creep(CreepProperties& properties, KeywordReader& reader);
}
