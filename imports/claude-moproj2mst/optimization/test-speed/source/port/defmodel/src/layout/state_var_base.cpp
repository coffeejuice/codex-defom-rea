// StateVarBase (p1-09). Layout: defmodel/layout/state_var_base.hpp.
//
// A state variable is a dim0 x dim1 x dim2 array of doubles (row = dim0 index, row length
// dim1 * dim2). cur[] holds the current dimensions and prev[] the ones before the last set_dim*.
// u30, when not null, points at the owner's double* slot; resize_values, clear and reset publish
// the array's data pointer (or null) there.
#include <defmodel/layout/state_var_base.hpp>

#include <defmodel/port.hpp>

#include <cstdlib>
#include <cstring>
#include <new>

namespace def::model {

namespace {

double** slot(void* u30) { return static_cast<double**>(u30); }

void copy_doubles(double* dst, const double* src, std::int32_t n)
{
    if (n != 0) std::memcpy(dst, src, static_cast<std::size_t>(static_cast<std::int64_t>(n)) << 3);
}

} // namespace

// @src 140300c90  ctor (the original leaves 0x0c to reset())
StateVarBase::StateVarBase(std::int32_t id_, void* u30_)
{
    // values: the handle member is created first (14019a6b0(this + 0x28))
    id = id_;
    u30 = u30_;
    cur[0] = 0;
    prev[0] = 0;
    cur[1] = 1;
    prev[1] = 1;
    cur[2] = 1;
    prev[2] = 1;
    reset();
}

// @src 1403018d0  slot 0: scalar deleting dtor (the delete is the C++ delete expression)
// @src 140300d50  dtor body
StateVarBase::~StateVarBase()
{
    clear();                              // 140300e10 -> 140300e30
    // then the handle member's dtor releases the body (_Timevec dtor in the decompile)
}

// @src 140300da0  reset dimensions to 0 x 1 x 1, kind 0, publish null
void StateVarBase::reset()
{
    set_kind(0);
    set_dim0(0);
    set_dim1(1);
    set_dim2(1);
    if (u30) *slot(u30) = nullptr;
}

// @src 140300e30  clear: dim0 = 0, free the values, publish null
// @src 140300e10  (forwards to 140300e30)
void StateVarBase::clear()
{
    set_dim0(0);
    values.clear();   // 1402568b0(this + 0x28): detach, free the storage
    if (u30) *slot(u30) = nullptr;
}

// @src 140300ea0
std::int32_t StateVarBase::get_id() const { return id; }

// @src 140300ec0
void StateVarBase::set_kind(std::int32_t kind) { u0c = kind; }

// @src 140300f00
std::int32_t StateVarBase::dim0(bool previous) const { return previous ? prev[0] : cur[0]; }

// @src 140300f30
void StateVarBase::set_dim0(std::int32_t v)
{
    prev[0] = cur[0];
    cur[0] = v;
}

// @src 140300f60
std::int32_t StateVarBase::dim1(bool previous) const { return previous ? prev[1] : cur[1]; }

// @src 140300f90
void StateVarBase::set_dim1(std::int32_t v)
{
    prev[1] = cur[1];
    cur[1] = v;
}

// @src 140300fc0
std::int32_t StateVarBase::dim2(bool previous) const { return previous ? prev[2] : cur[2]; }

// @src 140300ff0
void StateVarBase::set_dim2(std::int32_t v)
{
    prev[2] = cur[2];
    cur[2] = v;
}

// @src 140301020  number of values (the 64-bit count, used as int by every caller)
std::int32_t StateVarBase::value_count() const
{
    return static_cast<std::int32_t>(values.size());
}

// @src 140301050
double* StateVarBase::value_data() { return values.mutable_begin(); } // 1401d9050: detach, first

// @src 140301080  pointer to value `index`, null when out of range
double* StateVarBase::value_at(std::int32_t index)
{
    double* d = values.mutable_begin();
    if (-1 < index && index < value_count()) return d + static_cast<std::int64_t>(index);
    return nullptr;
}

// @src 1403010e0  first value index of row `row`, -1 when the row is out of range
std::int32_t StateVarBase::row_offset(std::int32_t row) const
{
    if (-1 < row && row < dim0(false)) {
        const std::uint32_t n1 = static_cast<std::uint32_t>(dim1(false));
        const std::uint32_t n2 = static_cast<std::uint32_t>(dim2(false));
        return static_cast<std::int32_t>(static_cast<std::uint32_t>(row) * n1 * n2);   // int wraparound
    }
    return -1;
}

// @src 140301380  resize the values to dim0 * dim1 * dim2 (new elements 0.0); true when the size
// changed. With remap (no Phase 1 caller passes it) the old rows are copied into the new row
// layout, but only when u30 is set, as in the original.
bool StateVarBase::resize_values(bool remap)
{
    bool changed = false;
    const std::int32_t want = static_cast<std::int32_t>(static_cast<std::uint32_t>(dim0(false)) *
                                                        static_cast<std::uint32_t>(dim1(false)) *
                                                        static_cast<std::uint32_t>(dim2(false)));
    if (value_count() != want) {
        std::int32_t row = dim0(false);
        const std::int32_t new_n1 = dim1(false);
        (void)dim2(false);
        const std::int32_t old_n0 = dim0(true);
        const std::int32_t old_n1 = dim1(true);
        (void)dim2(true);
        const double* src = value_data();
        const std::int32_t n = value_count();
        // operator new(n * 8); the original's overflow check turns a wrapped size into SIZE_MAX
        const std::uint64_t un = static_cast<std::uint64_t>(static_cast<std::int64_t>(n));
        const std::uint64_t bytes = un > (~0ull >> 3) ? ~0ull : un * 8u;
        void* tmp = std::malloc(bytes ? static_cast<std::size_t>(bytes) : 1);
        if (!tmp) throw std::bad_alloc();
        copy_doubles(static_cast<double*>(tmp), src, n);
        // 14019a740(this + 0x28, (int64)want, &0.0)
        values.resize(static_cast<std::size_t>(static_cast<std::int64_t>(want)), 0.0);
        if (u30) {
            *slot(u30) = value_data();
            if (remap) {
                double* dst = value_data();
                const double* old = static_cast<const double*>(tmp);
                while (row = row - 1, -1 < row) {
                    if (row < old_n0) {
                        for (std::int32_t j = 0; j < new_n1; ++j) {
                            if (j < old_n1) dst[row * new_n1 + j] = old[row * old_n1 + j];
                        }
                    }
                }
            }
        }
        std::free(tmp);
        changed = true;
    }
    return changed;
}

// @src 140301710  slot 1: set the three dimensions (keeping the old ones in prev), resize
void StateVarBase::vslot1_140301710(std::int32_t a, std::int32_t b, std::int32_t c)
{
    set_dim0(a);
    set_dim1(b);
    set_dim2(c);
    resize_values(false);
}

// @src 140301690  slot 2: when `which` is this id or -1: dim0 = n, dim1 = per_id[id], resize
void StateVarBase::vslot2_140301690(std::int32_t n, const std::int32_t* per_id, std::int32_t which)
{
    if (which == get_id() || which == -1) {
        set_dim0(n);
        set_dim1(per_id[get_id()]);
        resize_values(false);
    }
}

// @src 140301770  slot 3: take other's dimensions (slot 1, a virtual call), then copy its values
// when the sizes agree. True when this has no values afterwards or the copy was made.
bool StateVarBase::copy_from(const StateVarBase* other_)
{
    // The original calls the same (logically const) accessors on `other`.
    auto* other = const_cast<StateVarBase*>(other_);
    const std::int32_t c = other->dim2(false);
    const std::int32_t b = other->dim1(false);
    const std::int32_t a = other->dim0(false);
    vslot1_140301710(a, b, c);
    if (value_count() == 0) return true;
    if (value_count() != other->value_count()) return false;
    double* dst = value_data();
    if (dst == nullptr) return false;
    copy_doubles(dst, other->value_data(), other->value_count());
    return true;
}

// @src 140301850  slot 4: copy all of other's values into this array from the start of row
// `row` on (no length check, as in the original).
bool StateVarBase::vslot4_140301850(std::int32_t row, const StateVarBase* other_)
{
    auto* other = const_cast<StateVarBase*>(other_);
    double* dst = value_at(row_offset(row));
    if (dst != nullptr) copy_doubles(dst, other->value_data(), other->value_count());
    return dst != nullptr;
}

} // namespace def::model
