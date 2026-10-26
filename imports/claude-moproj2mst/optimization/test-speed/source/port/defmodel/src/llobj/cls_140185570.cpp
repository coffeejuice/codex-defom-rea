// Cls_140185570 (defmodel/llobj/cls_140185570.hpp).
#include <defmodel/llobj/cls_140185570.hpp>

#include <cstdlib>

namespace def::model {

// @src 140185570  ctor
Cls_140185570::Cls_140185570()
{
    u060 = nullptr;
    u068 = 3;
    init();
}

// @src 1401855b0  dtor (-> 1401856e0)
Cls_140185570::~Cls_140185570() { free_arrays(); }

// @src 14019a4b0  u068 = v
void Cls_140185570::set_u068(std::int32_t v) { u068 = v; }

// @src 140185630  zero the twelve pointers
void Cls_140185570::init()
{
    for (auto& p : arrays)
        p = nullptr;
}

// @src 1401856e0  free and zero the twelve pointers, in order
void Cls_140185570::free_arrays()
{
    for (auto& p : arrays) {
        if (p != nullptr)
            std::free(p);
        p = nullptr;
    }
}

} // namespace def::model
