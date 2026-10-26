// StateVarBase (RTTI .?AVStateVarBase@@): base of the StateVar* family (StateVarNode,
// StateVarNodePost, StateVarElem*, StateVarCustom). Holds an id, three current and three previous
// ints and a heap vector of 8-byte values. Embedded in LLOBJ_DATA at 0xb70 as a plain StateVarBase.
//
//   vtable   0x140549ce0 (5 slots)
//   ctor     140300c90(this, id, p) (vector handle at 0x28, then 140300da0 resets the values)
//   dtor     140300d50 (virtual; scalar deleting dtor 1403018d0 in slot 0; 140300e10, then
//            frees the vector handle at 0x28)
//   size     0x38: the ctor writes 8 bytes at 0x30 (param_1[6]); LLOBJ_DATA's next member starts
//            at 0xba8 = 0xb70 + 0x38.
//   fields   setters 140300ec0 (0x0c), 140300f30 (0x10, old value to 0x1c), 140300f90 (0x14 ->
//            0x20), 140300ff0 (0x18 -> 0x24); getter 140300ea0 (0x08); vector size 140301020,
//            data 140301050 (both on 0x28).
//   open     what the pointer at 0x30 points to (140300da0 writes *(*(this+0x30)) = 0).
#pragma once

#include <defmodel/sdctl/cow.hpp>

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>

namespace def::model {

class StateVarBase {
public:
    // @src 140300c90  ctor; LLOBJ_DATA passes (-1, 0)
    explicit StateVarBase(std::int32_t id = -1, void* u30 = nullptr);

    // @src 1403018d0  slot 0: scalar deleting dtor (dtor body 140300d50)
    virtual ~StateVarBase();
    // @src 140301710  slot 1: set 0x10/0x14/0x18 (keeping old values), then 140301380(this, 0)
    virtual void vslot1_140301710(std::int32_t a, std::int32_t b, std::int32_t c);
    // @src 140301690  slot 2: if idx == id or idx == -1: 0x10 = a, 0x14 = arr[id], 140301380
    virtual void vslot2_140301690(std::int32_t a, const std::int32_t* arr, std::int32_t idx);
    // @src 140301770  slot 3: copy from another state variable (slot 1, then memcpy of values)
    virtual bool copy_from(const StateVarBase* other);
    // @src 140301850  slot 4: memcpy other's values into the row chosen by 1403010e0(this, key)
    virtual bool vslot4_140301850(std::int32_t key, const StateVarBase* other);

    // Non-virtual members (p1-09, defmodel/src/layout/state_var_base.cpp). "dim" k is cur[k]; with
    // previous = true the getters return prev[k]. The value array holds dim0 * dim1 * dim2 doubles.
    std::int32_t get_id() const;                         // 140300ea0
    void set_kind(std::int32_t kind);                    // 140300ec0: 0x0c (1 node, 2 element)
    std::int32_t dim0(bool previous) const;              // 140300f00
    void set_dim0(std::int32_t v);                       // 140300f30
    std::int32_t dim1(bool previous) const;              // 140300f60
    void set_dim1(std::int32_t v);                       // 140300f90
    std::int32_t dim2(bool previous) const;              // 140300fc0
    void set_dim2(std::int32_t v);                       // 140300ff0
    std::int32_t value_count() const;                    // 140301020
    double* value_data();                                // 140301050
    double* value_at(std::int32_t index);                // 140301080
    std::int32_t row_offset(std::int32_t row) const;     // 1403010e0
    bool resize_values(bool remap);                      // 140301380
    void reset();                                        // 140300da0
    void clear();                                        // 140300e30 (140300e10 forwards here)

    // vptr at 0x00
    std::int32_t id;          // 0x08 ctor param 2; getter 140300ea0
    std::int32_t u0c;         // 0x0c 140300ec0
    std::int32_t cur[3];      // 0x10 140300f30 / 140300f90 / 140300ff0 (ctor: 0, 1, 1)
    std::int32_t prev[3];     // 0x1c previous values of cur (ctor: 0, 1, 1)
    CowVector<double> values; // 0x28 heap vector of 8-byte values (14019a6b0: new 0x20; p1-05 cow.hpp)
    void* u30;                // 0x30 ctor param 3
};

static_assert(sizeof(StateVarBase) == 0x38);
DEF_LAYOUT_OFFSET(StateVarBase, id, 0x08);
DEF_LAYOUT_OFFSET(StateVarBase, u0c, 0x0c);
DEF_LAYOUT_OFFSET(StateVarBase, cur, 0x10);
DEF_LAYOUT_OFFSET(StateVarBase, prev, 0x1c);
DEF_LAYOUT_OFFSET(StateVarBase, values, 0x28);
DEF_LAYOUT_OFFSET(StateVarBase, u30, 0x30);

} // namespace def::model
