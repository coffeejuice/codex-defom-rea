// Model functions of the group 2 state-variable keywords (packet kw-g2sv).
// Declarations and ownership: defmodel/statevar_kw/statevar_kw.hpp.
#include <defmodel/statevar_kw/statevar_kw.hpp>

#include <cstdlib>

#include <defmodel/bc/boundary_topology.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/members/ext_deps.hpp>
#include <defmodel/members/statevar_collection.hpp>
#include <defmodel/step/name_arrays.hpp>

namespace def::model {

namespace {

template <class T, class B>
T& at(B* base, std::size_t off)
{
    return *reinterpret_cast<T*>(reinterpret_cast<std::byte*>(base) + off);
}

// the custom-variable list obj+0xb58 starts with its Cls_140329c70 collection
void* custom_list(LLOBJ_DATA& obj) { return obj.sb58; }

// material list at DB_STEP_DATA+0x1328: head, ..., count at +0x10; next at material+0x2200;
// material number at +0; +4 "used" flag (1401b0620)
void* material_head(DB_STEP_DATA& m) { return at<void*>(&m, 0x1328); }
void* material_next(void* mat) { return at<void*>(mat, 0x2200); }

// 14029c2c0 (material list): material by number
void* material_by_number(DB_STEP_DATA& m, std::int32_t no)
{
    void* p = material_head(m);
    while (p != nullptr && at<std::int32_t>(p, 0) != no) p = material_next(p);
    return p;
}

// 14029c870 (material list): index of material `no` = number of materials with a smaller
// number, -1 when it does not exist
std::int32_t material_index(DB_STEP_DATA& m, std::int32_t no)
{
    if (material_by_number(m, no) == nullptr) return -1;
    std::int32_t k = 0;
    for (void* p = material_head(m); p != nullptr; p = material_next(p)) {
        if (at<std::int32_t>(p, 0) < no) k = k + 1;
    }
    return k;
}

void mark_db_var(void* owner, std::int32_t id)
{
    if (owner != nullptr) {
        const std::int32_t v = id;
        if (g_hook_1401d48d0 != nullptr)
            g_hook_1401d48d0(static_cast<DB_STEP_DATA*>(owner), &v);
        else
            mark_state_var_outputs(static_cast<DB_STEP_DATA*>(owner), &v); // 1401d48d0
    }
}

} // namespace

// @src 14032ebd0  StateVarCustom ctor
StateVarCustom::StateVarCustom(std::int32_t id_, void* p) : StateVarBase(id_, p)
{
    set_kind(0); // 140300ec0
    custom_id = 0;
}

// @src 14032ec40  StateVarCustom dtor
// @src 14032eca0  scalar deleting dtor (vtable slot 0): `delete v`
StateVarCustom::~StateVarCustom() = default;

namespace svkw {

// @src 14032a140  count of the collection (here the custom list obj+0xb58)
std::int32_t custom_count(LLOBJ_DATA& obj) { return ext::fun_14032a140(custom_list(obj)); }

// @src 140185f80  entry i as StateVarCustom, or null
StateVarCustom* custom_at(LLOBJ_DATA& obj, std::int32_t i)
{
    const std::int32_t n = custom_count(obj);
    if (i < 0 || n <= i) return nullptr;
    StateVarBase** p = ext::fun_14019a860(custom_list(obj), static_cast<std::int64_t>(i));
    return dynamic_cast<StateVarCustom*>(*p); // __RTDynamicCast(StateVarBase -> StateVarCustom)
}

// @src 140185e00  first StateVarCustom whose custom id is `id`
StateVarCustom* custom_find(LLOBJ_DATA& obj, std::int32_t id)
{
    const std::int32_t n = custom_count(obj);
    for (std::int32_t i = 0; i < n; i = i + 1) {
        StateVarBase** p = ext::fun_14019a860(custom_list(obj), static_cast<std::int64_t>(i));
        StateVarCustom* c = dynamic_cast<StateVarCustom*>(*p);
        if (c != nullptr && c->custom_id == id) return c; // 14032ec90
    }
    return nullptr;
}

// @src 140185ed0  find or create
StateVarCustom* custom_get(LLOBJ_DATA& obj, std::int32_t id)
{
    StateVarCustom* c = custom_find(obj, id);
    if (c == nullptr) c = custom_new(obj, id);
    return c;
}

// @src 140186010  new StateVarCustom(0x4e, 0) with custom id `id`, appended to the list
// @src 14032ec70  (custom id setter, inlined)
StateVarCustom* custom_new(LLOBJ_DATA& obj, std::int32_t id)
{
    StateVarCustom* c = new StateVarCustom(0x4e, nullptr); // operator new(0x40), 14032ebd0
    c->custom_id = id;
    reinterpret_cast<Cls_140329c70*>(custom_list(obj))->add(c); // 140329e30
    return c;
}

// @src 140185d20  define custom variable `id`: type 0 -> nr rows, 1 -> one per node, 2 -> one per
// element (the list's owner object, +0x10 of the list), m values per row, then resize
StateVarCustom* custom_define(LLOBJ_DATA& obj, std::int32_t id, std::int32_t type, std::int32_t m,
                              std::int32_t nr)
{
    StateVarCustom* v = custom_get(obj, id);
    v->set_kind(type); // 140300ec0
    LLOBJ_DATA* owner = at<LLOBJ_DATA*>(custom_list(obj), 0x10);
    if (type == 0) {
        v->set_dim0(nr); // 140300f30
    } else if (type == 1) {
        if (owner != nullptr) v->set_dim0(at<std::int32_t>(owner, 0x880));
    } else if (type == 2 && owner != nullptr) {
        v->set_dim0(at<std::int32_t>(owner, 0x884));
    }
    v->set_dim1(m);          // 140300f90
    v->resize_values(false); // 140301380
    return v;
}

// @src 140301620  every value = d
void custom_fill(StateVarBase* v, double d)
{
    double* p = v->value_data();         // 140301050
    const std::int32_t n = v->value_count(); // 140301020
    for (std::int32_t i = 0; i < n; i = i + 1) p[i] = d;
}

// @src 14019a4e0  the model's custom-variable registry (+0x33b8)
std::int32_t* model_custom_registry(DB_STEP_DATA& m) { return &at<std::int32_t>(&m, 0x33b8); }

// @src 14032f630  *p = v
void set_first_int(std::int32_t* p, std::int32_t v) { *p = v; }

// ---- set accessors --------------------------------------------------------------------------

// @src 14017f020  dims of set 0x8a0: owner model +0x15f8, or null
std::int32_t* set8a0_dims(void* set)
{
    void* owner = at<void*>(set, 0x130);
    return owner == nullptr ? nullptr : &at<std::int32_t>(owner, 0x15f8);
}

// @src 140182980  dims of set 0x9e0
std::int32_t* set9e0_dims(void* set)
{
    void* owner = at<void*>(set, 0xf0);
    return owner == nullptr ? nullptr : &at<std::int32_t>(owner, 0x15f8);
}

// @src 14017ffe0  (re)allocate the set's own arrays for key 0/-1/-2, then size the variables
void set8a0_alloc(void* set, std::int32_t n, std::int32_t* dims, std::int32_t key)
{
    if (0 < n) {
        if ((key == 0 || key == -2 || key == -1) && at<void*>(set, 8) == nullptr)
            at<double*>(set, 8) = bc::calloc_doubles(n * *dims); // 140257150
        if (key == -2 || key == -1) {
            for (int k = 0; k < 6; k = k + 1)
                at<std::int32_t*>(set, 0xf0 + 8 * k) = bc::calloc_ints(n * dims[0x12 + k]); // 140256dc0
            at<std::int32_t>(set, 300) = n;
        }
        reinterpret_cast<Cls_140329c70*>(set)->set_all(n, dims, key); // 140329e60
    }
}

// @src 1401836c0  the same for set 0x9e0
void set9e0_alloc(void* set, std::int32_t n, std::int32_t* dims, std::int32_t m, std::int32_t key)
{
    if (0 < n) {
        if (key == -2 || key == -1) {
            at<std::int32_t*>(set, 8) = bc::calloc_ints(n * m);
            at<std::int32_t*>(set, 0x18) = bc::calloc_ints(n * dims[0x3d]); // *(int*)(dims + 0xf4)
            at<std::int32_t>(set, 0xe8) = n;
            at<std::int32_t>(set, 0xec) = m;
        }
        reinterpret_cast<Cls_140329c70*>(set)->set_all(n, dims, key);
    }
}

// @src 140180310  resize variable `id` of set 0x8a0 (w unused besides the sign test)
void set8a0_resize(void* set, std::int32_t id, std::int32_t w)
{
    if (0 < at<std::int32_t>(set, 300) && -1 < w && at<void*>(set, 0x130) != nullptr)
        reinterpret_cast<Cls_140329c70*>(set)->set_all(at<std::int32_t>(set, 300),
                                                       &at<std::int32_t>(at<void*>(set, 0x130), 0x15f8), id);
}

// @src 140183840  resize variable `id` of set 0x9e0; SIZESH / TXTODF also keep the width
// @src 14019a410  (+0xfc setter, inlined)
// @src 14019a430  (+0x100 setter, inlined)
void set9e0_resize(void* set, std::int32_t id, std::int32_t w)
{
    if (0 < at<std::int32_t>(set, 0xe8) && -1 < w && at<void*>(set, 0xf0) != nullptr) {
        void* owner = at<void*>(set, 0xf0);
        reinterpret_cast<Cls_140329c70*>(set)->set_all(at<std::int32_t>(set, 0xe8),
                                                       &at<std::int32_t>(owner, 0x15f8), id);
        if (id == 0x36) at<std::int32_t>(set, 0xfc) = at<std::int32_t>(owner, 0x16d0);
        if (id == 0x37) at<std::int32_t>(set, 0x100) = at<std::int32_t>(owner, 0x16d4);
    }
}

namespace {
double* set8a0_var(void* set, std::int32_t id, std::size_t slot, bool mark)
{
    std::int32_t* dims = set8a0_dims(set);
    set8a0_alloc(set, at<std::int32_t>(set, 300), dims, id);
    if (mark) mark_db_var(at<void*>(set, 0x130), id);
    return at<double*>(set, slot);
}
double* set9e0_var(void* set, std::int32_t id, std::size_t slot, bool mark)
{
    std::int32_t* dims = set9e0_dims(set);
    set9e0_alloc(set, at<std::int32_t>(set, 0xe8), dims, at<std::int32_t>(set, 0xec), id);
    if (mark) mark_db_var(at<void*>(set, 0xf0), id);
    return at<double*>(set, slot);
}
} // namespace

// @src 140181190  NWEAR array (mark the DB variable only when *mark)
double* nwear_values(void* s, const bool* mark) { return set8a0_var(s, 0x18, 0x88, *mark); }
// @src 1401815b0  USRNOD array
double* usrnod_values(void* s) { return set8a0_var(s, 0x11, 0xc8, true); }
// @src 1401814b0  TIMERC array
double* timerc_values(void* s) { return set8a0_var(s, 0x22, 0xb8, true); }
// @src 140181530  NRECVL array
double* nrecvl_values(void* s) { return set8a0_var(s, 0x23, 0xc0, true); }
// @src 140181420  GRAIN2 array (mark when *no_mark is false)
double* grain2_values(void* s, const bool* no_mark) { return set8a0_var(s, 0x1d, 0xb0, !*no_mark); }
// @src 140184150  MATAXI array
double* mataxi_values(void* s, const bool* mark) { return set9e0_var(s, 0x2e, 0x60, *mark); }
// @src 140184a30  USRELM array
double* usrelm_values(void* s) { return set9e0_var(s, 0x3c, 0xe0, true); }
// @src 1401845b0  SIZESH array
double* sizesh_values(void* s) { return set9e0_var(s, 0x36, 0xa0, true); }
// @src 140184640  TXTODF array
double* txtodf_values(void* s) { return set9e0_var(s, 0x37, 0xa8, true); }
// @src 1401849a0  ERECVL array
double* erecvl_values(void* s) { return set9e0_var(s, 0x3f, 0xd8, true); }
// @src 1401840d0  DAMAGE element array
double* damage_values(void* s) { return set9e0_var(s, 0x2d, 0x58, true); }
// @src 140184520  GRAIN array
double* grain_values(void* s) { return set9e0_var(s, 0x35, 0x98, true); }
// @src 1401812a0  DAMAGE nodal array
double* damage_node_values(void* s) { return set8a0_var(s, 0x1c, 0xa0, true); }
// @src 140180eb0  DTMP array (temperature change; STEP.DAT writer 1402ac8a0)
double* dtmp_values(void* s) { return set8a0_var(s, 0x0c, 0x60, true); }
// @src 1401813a0  nodal stress array (STEP.DAT writer)
double* stress_node_values(void* s) { return set8a0_var(s, 0x1a, 0xa8, true); }
// @src 140184400  DVOLF array (STEP.DAT writer)
double* dvolf_values(void* s) { return set9e0_var(s, 0x33, 0x88, true); }
// @src 140184490  TICF array (STEP.DAT writer)
double* ticf_values(void* s) { return set9e0_var(s, 0x34, 0x90, true); }
// @src 140184760  YLDS array (STEP.DAT writer)
double* ylds_values(void* s) { return set9e0_var(s, 0x39, 0xb8, true); }

// ---- phases and grain model -------------------------------------------------------------------

// @src 140398350  phase-column map of the object's mixture materials
void phase_column_map(DB_STEP_DATA& m, LLOBJ_DATA& obj, std::int32_t* map, std::int32_t* count)
{
    if (map == nullptr || at<std::int32_t>(&obj, 0x87c) == 0) return;
    const std::int32_t nmat = at<std::int32_t>(&m, 0x1320);
    for (std::int32_t i = 0; i < nmat; i = i + 1) map[i] = -1;
    std::int32_t* mark = static_cast<std::int32_t*>(std::calloc(static_cast<std::size_t>(nmat), 4));
    std::int32_t prev = 0;
    const std::int32_t* grp = at<std::int32_t*>(&obj, 0x9f8);
    for (std::int32_t e = 0; e < at<std::int32_t>(&obj, 0x884); e = e + 1) {
        if (prev == grp[e]) continue;
        prev = grp[e];
        std::int32_t k = material_index(m, prev);
        if (-1 < k && k < nmat && mark[k] == 0) {
            void* mat = material_by_number(m, prev); // 1401d7060
            if (mat != nullptr && at<std::int32_t>(mat, 0x1cc8) == 1) {
                mark[k] = 1;
                for (std::int32_t j = 0; j < at<std::int32_t>(mat, 0x1ccc); j = j + 1) {
                    k = material_index(m, at<std::int32_t*>(mat, 0x1cd0)[j]);
                    if (-1 < k && k < nmat) mark[k] = 2;
                }
            }
        }
    }
    std::int32_t c = 0;
    for (std::int32_t i = 0; i < nmat; i = i + 1) {
        if (mark[i] == 2) {
            map[i] = c;
            c = c + 1;
        }
    }
    *count = c;
    if (mark != nullptr) std::free(mark);
}

// @src 1401d7160  grain model of the model's materials
std::int32_t grain_model(DB_STEP_DATA& m, const bool* only_used)
{
    if (at<std::int32_t>(&m, 0x864) == 0) return -9;
    return material_grain_model(m, only_used); // 14029bb50
}

// @src 14029bb50  1 / 2 / 4 / 5 / -1 by the first kind found in this order, -99 for 1 and 2
// together, 0 for none
std::int32_t material_grain_model(DB_STEP_DATA& m, const bool* only_used)
{
    bool k1 = false, k2 = false, km1 = false, k4 = false, k5 = false;
    const std::int32_t n = at<std::int32_t>(&m, 0x1338); // 140199c50 (list + 0x10)
    for (std::int32_t i = 0; i < n; i = i + 1) {
        void* mat = material_head(m); // 14029c260(list, i + 1)
        for (std::int32_t j = 1; mat != nullptr && j < i + 1; j = j + 1) mat = material_next(mat);
        if (mat == nullptr) continue;
        if (*only_used && at<std::int32_t>(mat, 4) == 0) continue; // 1401b0620
        const std::int32_t g = at<std::int32_t>(mat, 0x17e8);
        if (g == 1)
            k1 = true;
        else if (g == 2)
            k2 = true;
        else if (g == 4)
            k4 = true;
        else if (g == 5)
            k5 = true;
        else if (g == -1)
            km1 = true;
    }
    if (k1 && k2) return -99;
    if (k1) return 1;
    if (k2) return 2;
    if (k4) return 4;
    if (k5) return 5;
    if (km1) return -1;
    return 0;
}

} // namespace svkw
} // namespace def::model
