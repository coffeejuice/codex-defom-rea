// Cls_140329c70: collection of owned StateVarBase pointers (packet p1-10).
// See defmodel/members/statevar_collection.hpp.
#include <defmodel/members/statevar_collection.hpp>

#include <defmodel/layout/state_var_base.hpp>
#include <defmodel/members/ext_deps.hpp>

namespace def::model {

// @src 140329c70  ctor
Cls_140329c70::Cls_140329c70()
{
    ext::fun_14032a430(this);
    reset_all();
}

// @src 140329cb0  dtor
Cls_140329c70::~Cls_140329c70()
{
    delete_all();
    ext::fun_14032a4a0(this);
}

// @src 140329cf0  reset all (calls 140329dc0)
// @src 140329dc0  (140300e30 on every element)
void Cls_140329c70::reset_all()
{
    std::int32_t n = ext::fun_14032a140(this);
    for (std::int32_t i = 0; i < n; i = i + 1) {
        StateVarBase** p = ext::fun_14019a860(this, static_cast<std::int64_t>(i));
        ext::fun_140300e30(*p);
    }
}

// @src 140329d10  delete every element (virtual slot 0 with flag 1), then clear
void Cls_140329c70::delete_all()
{
    std::int32_t n = ext::fun_14032a140(this);
    for (std::int32_t i = 0; i < n; i = i + 1) {
        StateVarBase** p = ext::fun_14019a860(this, static_cast<std::int64_t>(i));
        StateVarBase* v = *p;
        if (v != nullptr)
            delete v;
    }
    ext::fun_14032a680(this);
}

// @src 140329e30  append
void Cls_140329c70::add(StateVarBase* v)
{
    unsigned char it[24];
    StateVarBase* item = v;
    ext::fun_14032a4e0(this, it, &item);
}

// @src 140329e60  virtual slot 2 on every element
void Cls_140329c70::set_all(std::int32_t a, const std::int32_t* arr, std::int32_t idx)
{
    std::int32_t n = ext::fun_14032a140(this);
    for (std::int32_t i = 0; i < n; i = i + 1) {
        StateVarBase** p = ext::fun_14019a860(this, static_cast<std::int64_t>(i));
        (*p)->vslot2_140301690(a, arr, idx);
    }
}

// @src 140329ef0  copy element-wise from `other` when both have the same count; an element pair
// with different ids, or a null element, makes the result false but the loop goes on; elements
// whose id is found in `excluded` are not copied.
bool Cls_140329c70::copy_from(Cls_140329c70* other, void* excluded)
{
    bool result;
    std::int32_t n = ext::fun_14032a140(this);
    std::int32_t m = ext::fun_14032a140(other);
    if (n == m) {
        bool ok = true;
        for (std::int32_t i = 0; i < n; i = i + 1) {
            StateVarBase* a = ext::fun_14032a160(this, i);
            StateVarBase* b = ext::fun_14032a160(other, i);
            if (a == nullptr || b == nullptr) {
                ok = false;
            } else if (a->id == b->id) { // 140300ea0 on both
                std::int32_t key = a->id;
                if (ext::fun_14032a570(excluded, &key) == 0)
                    a->copy_from(b); // slot 3, result ignored
            } else {
                ok = false;
            }
        }
        result = ok;
        ext::fun_14019acf0(excluded);
    } else {
        ext::fun_14019acf0(excluded);
        result = false;
    }
    return result;
}

} // namespace def::model
