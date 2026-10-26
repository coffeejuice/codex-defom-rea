#include "object_motion.hpp"

#include <stdexcept>

namespace deform::recovered {

ObjectMotion::ObjectMotion(int dimension)
    : tables{PropertyTable{0}, PropertyTable{0}, PropertyTable{0},
             PropertyTable{0}, PropertyTable{0}, PropertyTable{0x6f},
             PropertyTable{dimension == 2 ? 0x79 : 0x7a},
             PropertyTable{0}, PropertyTable{0}} {
    if (dimension == 2) direction_0x21b0 = {1.0, 0.0, 0.0};
    else if (dimension == 3) direction_0x21b0 = {0.0, 0.0, -1.0};
    else throw std::invalid_argument("motion constructor dimension outside owned profile");
}

void ObjectMotion::deactivate() noexcept {
    for (auto& table : tables) table.clear_owned_storage();
}

} // namespace deform::recovered
