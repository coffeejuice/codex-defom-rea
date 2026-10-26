// defmodel/members/statevar_collection.hpp: Cls_140329c70, a collection of owned StateVarBase
// pointers held through a copy-on-write handle (packet p1-10).
//
// It is the first member of the p1-04 container classes built by 14017ef20 and 140182880
// (both then set their own fields at 0x130/0xf0), and is torn down by 14017ef80, 1401828e0 and
// 140185ca0. The handle and its element access are p1-09/p1-05 code (14032a430 ..., 14019a860),
// reached through members/ext_deps.hpp until those packets land.
//
//   ctor        140329c70 (handle ctor 14032a430, then reset_all)
//   dtor        140329cb0 (delete_all, then handle dtor 14032a4a0)
//   reset_all   140329cf0 -> 140329dc0 (140300e30 on every element)
//   delete_all  140329d10 (delete every element through its virtual dtor, then clear 14032a680)
//   add         140329e30 (append at the end, 14032a4e0)
//   set_all     140329e60 (virtual slot 2 on every element)
//   copy_from   140329ef0 (element-wise copy, skipping ids found in a key set)
//   size        8: the handle; whether more members belong here is up to the p1-04 owners of
//               the containing classes (their own fields start at 0x08 or later).
#pragma once

#include <cstdint>

namespace def::model {

class StateVarBase;

class Cls_140329c70 {
public:
    Cls_140329c70();
    ~Cls_140329c70();
    Cls_140329c70(const Cls_140329c70&) = delete;
    Cls_140329c70& operator=(const Cls_140329c70&) = delete;

    void reset_all();
    void delete_all();
    void add(StateVarBase* v);
    void set_all(std::int32_t a, const std::int32_t* arr, std::int32_t idx);
    // `excluded` is the key set the caller passed by value; this function destroys it
    // (14019acf0), as the callee does in the original.
    bool copy_from(Cls_140329c70* other, void* excluded);

    void* handle; // 0x00 copy-on-write handle (14032a430)
};
static_assert(sizeof(Cls_140329c70) == 8);

} // namespace def::model
