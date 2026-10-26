// State-variable accessors and small object getters of packet p4-07.
// Declarations: defmodel/hdrstats/object_stats.hpp.
#include <defmodel/hdrstats/object_stats.hpp>

#include <cstddef>

#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/state_var_base.hpp>

namespace def::model::p407 {

namespace {

template <class T, class B>
T& at(B* base, std::size_t off)
{
    return *reinterpret_cast<T*>(reinterpret_cast<std::byte*>(const_cast<std::remove_const_t<B>*>(base)) + off);
}

// The accessor pattern of the node set (Cls_14017ef20): allocate variable `id` for the set's n
// (+0x12c) rows with the owner's dims (14017f020, 14017ffe0), mark it in the owner model
// (1401d48d0 with the owner +0x130), return the data slot.
double* node_var(void* set, std::int32_t id, std::size_t slot)
{
    std::int32_t* dims = ext::fun_14017f020(set);
    ext::fun_14017ffe0(set, at<std::int32_t>(set, 300), dims, id);
    if (at<DB_STEP_DATA*>(set, 0x130) != nullptr) {
        const std::int32_t v = id;
        ext::fun_1401d48d0(at<DB_STEP_DATA*>(set, 0x130), &v);
    }
    return at<double*>(set, slot);
}

// The same for the element set (Cls_140182880): n at +0xe8, m at +0xec, owner at +0xf0.
double* elem_var(void* set, std::int32_t id, std::size_t slot)
{
    std::int32_t* dims = ext::fun_140182980(set);
    ext::fun_1401836c0(set, at<std::int32_t>(set, 0xe8), dims, at<std::int32_t>(set, 0xec), id);
    if (at<DB_STEP_DATA*>(set, 0xf0) != nullptr) {
        const std::int32_t v = id;
        ext::fun_1401d48d0(at<DB_STEP_DATA*>(set, 0xf0), &v);
    }
    return at<double*>(set, slot);
}

} // namespace

// @src 140158d00
bool obj_kind_is_2(const LLOBJ_DATA* obj) { return ext::fun_140158ce0(obj) == 2; }

// @src 140161100
bool obj_flag_3612(const LLOBJ_DATA* obj) { return at<std::uint8_t>(obj, 0x3612) != 0; }

// @src 140220520  (forwards to 140158ce0)
std::int32_t obj_kind_140220520(const LLOBJ_DATA* obj) { return ext::fun_140158ce0(obj); }

// @src 140166200
std::int32_t strain_block_offset(LLOBJ_DATA* obj, const std::int32_t* code)
{
    std::int32_t* counts = &at<std::int32_t>(obj->owner, 0x86c);
    bool any = false;
    for (std::int32_t i = 0; i <= 4; i = i + 1) {
        if (counts[i] != 0) {
            any = true;
            break;
        }
    }
    if (!any) return 0;
    std::int32_t c;
    if (*code == 3000) {
        c = 0;
    } else if (*code == 0xc1c) {
        c = 1;
    } else if (*code == 0xc80) {
        c = 2;
    } else if (*code == 0xce4) {
        c = 3;
    } else if (*code == 0xd48) {
        c = 4;
    } else if (*code == 0xdac) {
        c = 5;
    } else {
        c = -1;
    }
    std::int32_t off;
    if (c == -1) {
        off = -1;
    } else if (*code == 0xdac) {
        off = 0;
    } else if (counts[c] == 1) {
        off = 0;
    } else {
        off = -1;
    }
    if (off != -1) {
        for (std::int32_t i = 0; i < c; i = i + 1) off = off + counts[i];
    }
    return off;
}

// @src 140180940
double* node_var_01(void* set) { return node_var(set, 0x01, 0x10); }
// @src 1401809c0
double* node_var_02(void* set) { return node_var(set, 0x02, 0x18); }
// @src 140180c40
double* node_var_08(void* set) { return node_var(set, 0x08, 0x40); }
// @src 140180f90
double* node_var_0d(void* set) { return node_var(set, 0x0d, 0x68); }
// @src 140181090
double* node_var_0f(void* set) { return node_var(set, 0x0f, 0x78); }
// @src 140181320
double* node_var_1b(void* set) { return node_var(set, 0x1b, 0x98); }

// @src 140183d50
double* elem_var_26(void* set) { return elem_var(set, 0x26, 0x20); }
// @src 140183dd0
double* elem_var_27(void* set) { return elem_var(set, 0x27, 0x28); }
// @src 140183e50
double* elem_var_28(void* set) { return elem_var(set, 0x28, 0x30); }
// @src 140183ed0
double* elem_var_29(void* set) { return elem_var(set, 0x29, 0x38); }
// @src 140183f50
double* elem_var_2a(void* set) { return elem_var(set, 0x2a, 0x40); }
// @src 140183fd0
double* elem_var_2b(void* set) { return elem_var(set, 0x2b, 0x48); }
// @src 1401846d0
double* elem_var_38(void* set) { return elem_var(set, 0x38, 0xb0); }

// @src 140300e80
void sv_set_id(StateVarBase* sv, std::int32_t id) { sv->id = id; }

// @src 140301280  (dims through 140300f60 / 140300fc0 with previous = 0, slot via 140005e70)
void sv_set_value(StateVarBase* sv, std::int32_t row, std::int32_t col, std::int32_t k, double v)
{
    const std::int32_t d1 = sv->dim1(false);
    const std::int32_t d2 = sv->dim2(false);
    const std::int32_t d1b = sv->dim1(false);
    sv->values.mutable_at(static_cast<std::size_t>(
        static_cast<std::int64_t>(col * d1b + k + row * d1 * d2))) = v;
}

} // namespace def::model::p407
