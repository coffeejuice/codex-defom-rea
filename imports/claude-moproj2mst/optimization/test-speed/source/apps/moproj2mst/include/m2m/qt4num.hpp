// m2m/qt4num.hpp: Qt 4.8.4 number formatting below the qt4.hpp seam (owner: P04).
//
// Source: static reading of a copy of QtCore4_64.dll 4.8.4 shipped with DEFORM v11.0.2
// (image base 0x67000000). Addresses below are VAs in that DLL, not in DEF_GUI_MO_64.exe.
#pragma once

#include <string>

#include "m2m/qt4.hpp"

namespace m2m::qt {

// QLocalePrivate::DoubleForm (Qt 4.8 order: DFExponent = 0, DFDecimal, DFSignificantDigits)
enum class DoubleForm { Exponent = 0, Decimal = 1, SignificantDigits = 2 };

// QLocalePrivate::Flags (the bits doubleToString / longLongToString test)
enum : unsigned {
    kAlternate = 0x01,
    kZeroPadded = 0x02,
    kLeftAdjusted = 0x04,
    kBlankBeforePositive = 0x08,
    kAlwaysShowSign = 0x10,
    kThousandsGroup = 0x20,
    kCapitalEorX = 0x40,
};

// Result of qdtoa: the digit string without sign or point, the decimal point position and the sign
// bit of the input (set for -0.0 too).
struct Digits {
    std::string digits;
    int decpt = 0;
    bool sign = false;
};

// @src 0x67057450 qdtoa -> @src 0x67055fd0 _qdtoa (David Gay's dtoa, old BSD version).
// Modes 2 and 3 only (the only ones doubleToString uses); other modes throw NotPorted.
Digits qdtoa(double d, int mode, int ndigits);

// @src 0x6704b550 QLocalePrivate::doubleToString(QChar zero, plus, minus, exponential, group,
// decimal, double, int precision, DoubleForm, int width, unsigned flags), with the C locale
// characters ('0', '+', '-', 'e', ',', '.'). Returns Latin-1 text.
std::string doubleToString(double d, int precision = -1, DoubleForm form = DoubleForm::SignificantDigits,
                           int width = -1, unsigned flags = 0);

// QLocalePrivate::longLongToString / unsLongLongToString in the C locale.
std::string longLongToString(long long v, int precision = -1, int base = 10, int width = -1,
                             unsigned flags = 0);
std::string unsLongLongToString(unsigned long long v, int precision = -1, int base = 10,
                                int width = -1, unsigned flags = 0);

// QString::number(double, ...) and QString::arg(double, ...) are declared in qt4.hpp.

}  // namespace m2m::qt
