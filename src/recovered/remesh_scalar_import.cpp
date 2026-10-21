#include "remesh_scalar_import.hpp"

#include <stdexcept>

namespace deform::recovered {
namespace {
int index(int keyword) noexcept {
    switch (keyword) {
    case 186: return 0; //MGSIZR2d20.
    case 188: return 1; //MGWTMP2d28.
    case 189: return 2; //MGWSTN2d30.
    case 190: return 3; //MGWSTR2d38.
    case 187: return 4; //MGWCUV2d48.
    case 180: return 5; //RMDPTH2dc8.
    case 181: return 6; //RMSTRK2dd0.
    case 183: return 7; //RMSTEP2dd8.
    case 182: return 8; //RMTIME2de0.
    default: return -1;
    }
}
}
bool remesh_scalar_keyword(int keyword) noexcept { return keyword == 202 || index(keyword) >= 0; }

int read_remesh_scalar(int keyword, ObjectRecord& object, KeywordReader& reader) {
    const int slot = index(keyword);
    if (keyword != 202 && slot < 0) throw std::logic_error("unexpected remeshing scalar keyword");
    double& destination = keyword == 202 ? object.direct.temperature_limit_0x1f10 :
        object.remeshing_scalars.values[static_cast<std::size_t>(slot)];
    const auto read = reader.real();
    destination = read.values[0]; //Live zeroing is committed even on blank/EOF.
    return read.status == 0 ? 0 : 2;
}
}
