// StateVarElemMATAXI (p1-09). Layout: defmodel/layout/state_var_elem_mataxi.hpp.
#include <defmodel/layout/state_var_elem_mataxi.hpp>


#include <cstdio>

namespace def::model {

namespace {

// @src 14032d130  SdctlValueVector<double>::replace(index, value): the range check only prints
void replace_value(StateVarBase* sv, std::int32_t index, const double* value)
{
    const std::int32_t n = sv->value_count();   // 140003090(this + 0x28)
    if (index < 0 || n <= index) {
        std::fprintf(stdout, "DebugInfo - SdctlValueVector<T>::replace() - index out of range\n");
        // The original then writes out of bounds anyway; the port skips that write (heap
        // corruption in the original, unreachable when dim1 = 9 and dim2 >= 1).
        return;
    }
    const double v = *value;
    sv->value_data()[index] = v;
}

} // namespace

// @src 14032ccd0  ctor: StateVarElem ctor, own vptr
StateVarElemMATAXI::StateVarElemMATAXI(std::int32_t id_, void* slot_) : StateVarElem(id_, slot_) {}

// @src 14032d0b0  slot 0: scalar deleting dtor
// @src 14032cd20  dtor (vptr reset, then the StateVarElem dtor 14032c920)
StateVarElemMATAXI::~StateVarElemMATAXI() = default;

// @src 14032cd50  slot 2: like 140301690; when the array was resized and per_id[id] == 9, set
// values 0, 4 and 8 of each 9-value row to 1.0 (identity axes)
void StateVarElemMATAXI::vslot2_140301690(std::int32_t n, const std::int32_t* per_id, std::int32_t which)
{
    if (which == get_id() || which == -1) {
        set_dim0(n);
        set_dim1(per_id[get_id()]);
        if (resize_values(false)) {
            if (per_id[get_id()] == 9) {
                const std::int32_t rows = dim0(false);
                for (std::int32_t i = 0; i < rows; ++i) {
                    const double one_a = 1.0;   // DAT_140509c90
                    replace_value(this, i * 9, &one_a);
                    const double one_b = 1.0;
                    replace_value(this, i * 9 + 4, &one_b);
                    const double one_c = 1.0;
                    replace_value(this, i * 9 + 8, &one_c);
                }
            }
        }
    }
}

} // namespace def::model
