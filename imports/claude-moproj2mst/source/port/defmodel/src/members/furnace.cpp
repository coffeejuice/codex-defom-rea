// Cls_1403753f0 methods: the furnace model's curve list, its SI defaults and its unit
// conversion (packet p1-10). Curve and list primitives are other packets' (members/ext_deps.hpp).
//
// Curves are addressed 1-based (140136860). A curve value is get/set by four indices
// (140130720 / 140130800: row, j, k, column); the shape fields written here are ndim (0x04),
// var[0] (0x10), n[0] (0x1c) and ncomp (0x40) of DYNARY.
#include <defmodel/layout/cls_1403753f0.hpp>
#include <defmodel/members/ext_deps.hpp>
#include <defmodel/members/furnace.hpp>

#include <cstdio>
#include <cstring>

namespace def::model {

namespace {

using ext::fun_140130720; // get
using ext::fun_140130800; // set

double get(DYNARY* c, std::int32_t i, std::int32_t j, std::int32_t k, std::int32_t l)
{
    return fun_140130720(c, i, j, k, l);
}

void set(DYNARY* c, double v, std::int32_t i, std::int32_t j, std::int32_t k, std::int32_t l)
{
    fun_140130800(c, v, i, j, k, l);
}

double factor(std::int32_t kind, std::int32_t mode) { return ext::fun_140235d00(kind, mode); }

double temp(std::int32_t mode, double t) { return ext::fun_140235d80(mode, t); }

} // namespace

// @src 140375470  zero bytes 0x58..0x6f, then sprintf(this, "Furnace model")
void Cls_1403753f0::init_name()
{
    for (std::uint8_t& b : u58)
        b = 0;
    std::snprintf(name, sizeof name, "Furnace model");
}

// @src 1403754e0  release the curves (calls 140375520)
void Cls_1403753f0::release_curves() { clear_curves(); }

// @src 140375520  delete every curve (140136910)
void Cls_1403753f0::clear_curves() { ext::fun_140136910(&curves); }

// @src 140375600  build the curves: types 0x8e..0xa2 (21), then one 0xa3 per extra entry
void Cls_1403753f0::setup_curves(const std::int32_t* n)
{
    clear_curves();
    for (std::int32_t type = 0x8e; type <= 0xa2; ++type) {
        DYNARY* c = ext::fun_140136670(&curves);
        ext::fun_14012d2a0(c, type);
    }
    if (0x15 < *n) {
        for (std::int32_t i = 0x15; i < *n; i = i + 1) {
            DYNARY* c = ext::fun_140136670(&curves);
            ext::fun_14012d2a0(c, 0xa3);
        }
    }
}

// @src 140375950  number of curves (14013c1b0)
std::int32_t Cls_1403753f0::curve_count() { return ext::fun_14013c1b0(&curves); }

// @src 140375980  i-th curve, 1-based (140136860)
DYNARY* Cls_1403753f0::curve(const std::int32_t* i) { return ext::fun_140136860(&curves, *i); }

// @src 140375e90  shape every curve from the table, fill the SI defaults, convert when English
void Cls_1403753f0::set_defaults()
{
    std::int32_t n = curve_count();
    for (std::int32_t i = 1; i <= n; i = i + 1) {
        DYNARY* c = curve(&i);
        if (c == nullptr || 0x14 < i) {
            if (c != nullptr) {
                c->ndim = 1;
                c->var[0] = 0;
                c->n[0] = 0;
                c->ncomp = 0;
            }
        } else {
            const std::int32_t* row = kFurnaceCurveShape[i - 1];
            c->ndim = row[1];
            c->var[0] = 0;
            c->n[0] = row[3];
            c->ncomp = row[4];
        }
        if (c == nullptr)
            continue;
        switch (i) {
        case 1:
            ext::fun_14012fad0(c);
            set(c, 2.0, 0, 0, 0, 0); // 140509fb8
            set(c, 1.0, 0, 0, 0, 1); // 140509c90
            set(c, 1.0, 0, 0, 0, 2);
            break;
        case 2:
        case 3:
            ext::fun_14012fad0(c);
            break;
        case 4:
            ext::fun_14012fad0(c);
            set(c, 1.0, 0, 0, 0, 0);    // 140509c90
            set(c, 10.0, 0, 0, 0, 1);   // 14051bee8
            set(c, 0.05, 0, 0, 0, 2);   // 140524ce0
            set(c, 2400.0, 0, 0, 0, 3); // 1405544b8
            set(c, 0.0, 0, 0, 0, 4);
            break;
        case 5:
            ext::fun_14012fad0(c);
            set(c, 850.0, 0, 0, 0, 2);                  // 1405544b0
            set(c, 0.15, 0, 0, 0, 3);                   // 1405544a8
            set(c, 0.94, 1, 0, 0, 6 - c->ncomp);          // 1405544a0
            set(c, -0.0001894, 1, 0, 0, 7 - c->ncomp);    // 140554498
            set(c, -1.605e-08, 1, 0, 0, 8 - c->ncomp);    // 140554490
            set(c, -0.01104, 1, 0, 0, 9 - c->ncomp);      // 140554488
            set(c, 0.0001816, 1, 0, 0, 10 - c->ncomp);    // 140554480
            set(c, 7.322e-09, 1, 0, 0, 0xb - c->ncomp);   // 140554478
            break;
        case 6:
            ext::fun_14012fad0(c);
            set(c, 0.6, 0, 0, 0, 1); // 140554470
            set(c, 1.0, 0, 0, 0, 2); // 140509c90
            break;
        case 7:
            ext::fun_14012fad0(c);
            break;
        case 8:
            ext::fun_14012fad0(c);
            set(c, 10.0, 0, 0, 0, 0);      // 14051bee8
            set(c, 1e-12, 0, 0, 0, 1);     // 14051f260
            set(c, 1000000000000.0, 0, 0, 0, 2); // 140554468
            set(c, 0.4, 2, 0, 0, 2);       // 140554460
            set(c, 0.036, 3, 0, 0, 0);     // 140554458
            set(c, 1.0, 3, 0, 0, 1);       // 140509c90
            set(c, 68.0, 3, 0, 0, 2);      // 14051f1f8
            set(c, 7.7e-06, 3, 0, 0, 3);   // 14052e8b8
            set(c, 0.0179, 4, 0, 0, 0);    // 140554450
            set(c, 0.01578, 4, 0, 0, 1);   // 140554448
            set(c, 0.01292, 4, 0, 0, 2);   // 140554440
            set(c, 1002.0, 4, 0, 0, 3);    // 140554438
            set(c, 460.0, 5, 0, 0, 0);     // 140554430
            set(c, 528.0, 5, 0, 0, 1);     // 140554428
            set(c, 68.0, 5, 0, 0, 2);      // 14051f1f8
            set(c, 1.8, 5, 0, 0, 3);       // 140531748
            set(c, 3.306e-15, 5, 0, 0, 4); // 14052e8c0
            set(c, 2.681, 6, 0, 0, 0);     // 140554420
            set(c, 0.003661, 6, 0, 0, 1);  // 140554418
            set(c, 20.0, 8, 0, 0, 1);      // 14051f1f0
            set(c, 68.0, 9, 0, 0, 1);      // 14051f1f8
            set(c, 68.0, 9, 0, 0, 3);      // 14051f1f8
            break;
        case 9:
        case 10:
            ext::fun_14012fad0(c);
            break;
        case 11:
            c->var[0] = 1;
            break;
        case 14:
            ext::fun_14012fad0(c);
            break;
        default: // 12, 13 and above 14: nothing more
            break;
        }
    }
    if (ext::fun_140235c10() != 2)
        convert_units(2);
}

// @src 1403769f0  convert the furnace curves between unit systems (mode as in 140235d00:
// 1 or 2; 0 does nothing). Curves 13, 10, 12, 16 and those above 17 are left alone. Curve 6
// writes its three converted values all to column 3-ncomp (as the original does), and curve 7
// computes one product it never stores.
void Cls_1403753f0::convert_units(std::int32_t mode)
{
    if (mode == 0)
        return;
    std::int32_t n = curve_count();
    for (std::int32_t i = 1; i <= n; i = i + 1) {
        DYNARY* c = curve(&i);
        if (c == nullptr)
            continue;
        double v;
        std::int32_t id;
        switch (i) {
        case 1:
            v = get(c, 0, 0, 0, 3);
            v = v * factor(0x28, mode);
            set(c, v, 0, 0, 0, 3);
            break;
        case 2:
            v = get(c, 0, 0, 0, 0);
            v = v * factor(0x29, mode);
            set(c, v, 0, 0, 0, 0);
            v = get(c, 0, 0, 0, 1);
            v = temp(mode, v);
            set(c, v, 0, 0, 0, 1);
            v = get(c, 0, 0, 0, 2);
            v = temp(mode, v);
            set(c, v, 0, 0, 0, 2);
            break;
        case 3:
            for (std::int32_t l = 0; l <= 5; ++l) {
                v = get(c, 0, 0, 0, l);
                v = v * factor(1, mode);
                set(c, v, 0, 0, 0, l);
            }
            break;
        case 4:
            v = get(c, 0, 0, 0, 3);
            v = temp(mode, v);
            set(c, v, 0, 0, 0, 3);
            break;
        case 5:
            v = get(c, 0, 0, 0, 1);
            v = v * factor(0x28, mode);
            v = v / factor(4, mode);
            set(c, v, 0, 0, 0, 1);
            v = get(c, 0, 0, 0, 2);
            v = temp(mode, v);
            set(c, v, 0, 0, 0, 2);
            break;
        case 6:
            v = get(c, 0, 0, 0, 0);
            v = v * factor(3, mode);
            set(c, v, 0, 0, 0, 0);
            v = get(c, 1, 0, 0, 3 - c->ncomp);
            v = v * factor(4, mode);
            set(c, v, 1, 0, 0, 3 - c->ncomp);
            v = get(c, 1, 0, 0, 4 - c->ncomp);
            v = temp(mode, v);
            set(c, v, 1, 0, 0, 3 - c->ncomp);
            v = get(c, 1, 0, 0, 5 - c->ncomp);
            v = temp(mode, v);
            set(c, v, 1, 0, 0, 3 - c->ncomp);
            break;
        case 7:
            v = get(c, 0, 0, 0, 3);
            v = v * factor(6, mode); // not stored
            v = get(c, 0, 0, 0, 5);
            v = v * factor(1, mode);
            set(c, v, 0, 0, 0, 5);
            v = get(c, 0, 0, 0, 6);
            v = v * factor(1, mode);
            set(c, v, 0, 0, 0, 6);
            v = get(c, 1, 0, 0, 10 - c->ncomp);
            v = v * factor(4, mode);
            set(c, v, 1, 0, 0, 10 - c->ncomp);
            v = get(c, 1, 0, 0, 0xb - c->ncomp);
            v = temp(mode, v);
            set(c, v, 1, 0, 0, 0xb - c->ncomp);
            v = get(c, 1, 0, 0, 0xc - c->ncomp);
            v = temp(mode, v);
            set(c, v, 1, 0, 0, 0xc - c->ncomp);
            v = get(c, 2, 0, 0, c->ncomp * -2 + 0x15);
            v = v * factor(0x29, mode);
            set(c, v, 2, 0, 0, c->ncomp * -2 + 0x15);
            // curve numbers stored as values: convert the referenced curves
            id = static_cast<std::int32_t>(get(c, 2, 0, 0, 2));
            ext::fun_14012e400(curve(&id), 0x17, mode, true);
            id = static_cast<std::int32_t>(get(c, 2, 0, 0, 3));
            ext::fun_14012e400(curve(&id), 0x18, mode, true);
            id = static_cast<std::int32_t>(get(c, 2, 0, 0, 4));
            ext::fun_14012e400(curve(&id), 0x19, mode, true);
            id = static_cast<std::int32_t>(get(c, 2, 0, 0, 5));
            ext::fun_14012e400(curve(&id), 0x2a, mode, true);
            id = static_cast<std::int32_t>(get(c, 3, 0, 0, 2));
            ext::fun_14012e400(curve(&id), 0x17, mode, true);
            id = static_cast<std::int32_t>(get(c, 3, 0, 0, 3));
            ext::fun_14012e400(curve(&id), 0x18, mode, true);
            id = static_cast<std::int32_t>(get(c, 3, 0, 0, 4));
            ext::fun_14012e400(curve(&id), 0x19, mode, true);
            id = static_cast<std::int32_t>(get(c, 3, 0, 0, 5));
            ext::fun_14012e400(curve(&id), 0x2a, mode, true);
            id = static_cast<std::int32_t>(get(c, 4, 0, 0, 2));
            ext::fun_14012e400(curve(&id), 5, mode, true);
            ext::fun_14012e6d0(curve(&id), 3, mode, false); // 140377945: R9D = 0
            break;
        case 8:
            v = get(c, 2, 0, 0, 2);
            v = v * factor(0x14, mode);
            set(c, v, 2, 0, 0, 2);
            v = get(c, 3, 0, 0, 0);
            v = v * factor(0x18, mode);
            set(c, v, 3, 0, 0, 0);
            v = get(c, 3, 0, 0, 2);
            v = temp(mode, v);
            set(c, v, 3, 0, 0, 2);
            v = get(c, 3, 0, 0, 3);
            v = v * factor(0x14, mode);
            set(c, v, 3, 0, 0, 3);
            v = get(c, 4, 0, 0, 3);
            v = v * factor(0x27, mode);
            v = v / factor(4, mode);
            set(c, v, 4, 0, 0, 3);
            v = get(c, 5, 0, 0, 0);
            v = temp(mode, v);
            set(c, v, 5, 0, 0, 0);
            v = get(c, 5, 0, 0, 1);
            v = temp(mode, v);
            set(c, v, 5, 0, 0, 1);
            v = get(c, 5, 0, 0, 2);
            v = temp(mode, v);
            set(c, v, 5, 0, 0, 2);
            v = get(c, 5, 0, 0, 4);
            v = v * factor(0x13, mode);
            set(c, v, 5, 0, 0, 4);
            v = get(c, 6, 0, 0, 0);
            v = v * factor(1, mode);
            set(c, v, 6, 0, 0, 0);
            v = get(c, 6, 0, 0, 1);
            v = v / factor(9, mode);
            set(c, v, 6, 0, 0, 1);
            v = get(c, 9, 0, 0, 1);
            v = temp(mode, v);
            set(c, v, 9, 0, 0, 1);
            v = get(c, 9, 0, 0, 3);
            v = temp(mode, v);
            set(c, v, 9, 0, 0, 3);
            v = get(c, 10, 0, 0, 0);
            v = v * factor(0x14, mode);
            set(c, v, 10, 0, 0, 0);
            v = get(c, 10, 0, 0, 1);
            v = v * factor(0x14, mode);
            set(c, v, 10, 0, 0, 1);
            break;
        case 9:
            for (std::int32_t j = 0; j < c->n[0]; j = j + 1) {
                v = get(c, j, 0, 0, 1);
                v = v * factor(6, mode);
                set(c, v, j, 0, 0, 1);
            }
            break;
        case 11:
            c->value = temp(mode, c->value);
            ext::fun_14012e400(c, 9, mode, true);
            break;
        case 14:
            for (std::int32_t j = 0; j < c->n[0]; j = j + 1) {
                std::int32_t k;
                for (k = 0xc; k < 0x12; k = k + 1) {
                    v = get(c, j, 0, 0, k);
                    v = v * factor(1, mode);
                    set(c, v, j, 0, 0, k);
                }
                for (std::int32_t l = k; l < c->ncomp; l = l + 1) {
                    v = get(c, j, 0, 0, l);
                    v = v * factor(3, mode);
                    set(c, v, j, 0, 0, l);
                }
            }
            break;
        case 15:
        case 17: {
            const std::int32_t kind = (i == 15) ? 0x29 : 1;
            const std::int32_t rows = c->n[0]; // read once (local_18 / local_14)
            for (std::int32_t j = 0; j < rows; j = j + 1) {
                v = get(c, j, 0, 0, 1);
                v = v * factor(kind, mode);
                set(c, v, j, 0, 0, 1);
                id = static_cast<std::int32_t>(get(c, j, 0, 0, 2));
                ext::fun_14012e400(curve(&id), 0x17, mode, true);
                id = static_cast<std::int32_t>(get(c, j, 0, 0, 3));
                ext::fun_14012e400(curve(&id), 0x18, mode, true);
                id = static_cast<std::int32_t>(get(c, j, 0, 0, 4));
                ext::fun_14012e400(curve(&id), 0x19, mode, true);
                id = static_cast<std::int32_t>(get(c, j, 0, 0, 5));
                ext::fun_14012e400(curve(&id), 0x2a, mode, true);
            }
            break;
        }
        default: // 10, 12, 13, 16 and above 17
            break;
        }
    }
}

} // namespace def::model
