#include "object_integer_import.hpp"

#include <stdexcept>

namespace deform::recovered {
namespace {
int index(int keyword) noexcept {
    switch (keyword) {
    case 234: return 0; //ELPSOL1f18, default2.
    case 267: return 1; //OBJUPD1f1c.
    case 199: return 2; //FRCSTP1f34.
    case 200: return 3; //FRCNEL1f38.
    case 201: return 4; //FRCMTH1f3c.
    case 253: return 5; //USRSUB2150.
    default: return -1;
    }
}
}
bool object_integer_keyword(int keyword) noexcept { return index(keyword) >= 0; }

int read_object_integer(int keyword, ObjectDirectImports& fields, KeywordReader& reader) {
    const int slot = index(keyword);
    if (slot < 0) throw std::logic_error("unexpected direct object integer keyword");
    const auto read = reader.integer(); //413de0 skip0; live zeroing survives failure.
    fields.integers[static_cast<std::size_t>(slot)] = read.values[0];
    return read.status == 0 ? 0 : 2;
}
}
