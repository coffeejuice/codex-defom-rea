// DYNARY unit conversion (14012e400, 14012e6d0); declarations and summary in
// defmodel/dynary_init.hpp. Owned by key-common.
#include <defmodel/dynary_init.hpp>
#include <defrt/rdata.hpp>

namespace def::model {

namespace {

// rdata 140509c90  1.0
constexpr double kOne = def::rt::rdata::d_140509c90;

// part of @src 14012e400 / 14012e6d0: the identical axis loop of both
void convert_axes_of(DYNARY* t, int mode, const UnitFunctions& units)
{
    for (int d = 0; d < t->ndim; ++d) {
        const int k = kVarUnitKind[t->var[d]];
        if (k == 9) {
            for (int j = 0; j < t->n[d]; ++j) t->axis[d][j] = units.temperature(mode, t->axis[d][j]);
        } else {
            for (int j = 0; j < t->n[d]; ++j) {
                const double* a = t->axis[d];
                const double f = units.factor(k, mode); // called per element, as in the original
                t->axis[d][j] = a[j] * f;
            }
        }
    }
}

} // namespace

// @src 14012e400
void dynary_convert_units(DYNARY* t, int kind, int mode, bool convert_axes, const UnitFunctions& units)
{
    if (mode == 0 || !dynary_counts_positive(t) || t->ncomp != 1) return;
    if (convert_axes) convert_axes_of(t, mode, units);
    const std::int32_t cnt = dynary_value_count(t);
    if (kind == 9) {
        t->value = units.temperature(mode, t->value);
        for (int i = 0; i < cnt && t->values != nullptr; ++i) t->values[i] = units.temperature(mode, t->values[i]);
    } else {
        double f = units.factor(kind, mode);
        if (t->type == 0x2a) f = kOne / f;
        t->value = t->value * f;
        for (int i = 0; i < cnt && t->values != nullptr; ++i) t->values[i] = t->values[i] * f;
    }
}

// @src 14012e6d0
void dynary_convert_units_inverse(DYNARY* t, int kind, int mode, bool convert_axes, const UnitFunctions& units)
{
    if (mode == 0 || !dynary_counts_positive(t) || t->ncomp != 1) return;
    if (convert_axes) convert_axes_of(t, mode, units);
    const std::int32_t cnt = dynary_value_count(t);
    if (kind == 9) {
        t->value = kOne / units.temperature(mode, t->value);
        for (int i = 0; i < cnt && t->values != nullptr; ++i)
            t->values[i] = kOne / units.temperature(mode, t->values[i]);
    } else {
        const double f = kOne / units.factor(kind, mode);
        t->value = t->value * f;
        for (int i = 0; i < cnt && t->values != nullptr; ++i) t->values[i] = t->values[i] * f;
    }
}

} // namespace def::model
