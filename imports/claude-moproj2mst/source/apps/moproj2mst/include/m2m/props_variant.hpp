// m2m/props_variant.hpp: QVariant conversions the properties' set slots use and the QVariant
// constructors of their get slots (Qt 4.8 semantics), shared by the props slot files (L2).
#pragma once

#include "m2m/qt4.hpp"

namespace m2m::pv {

using qt::String;
using qt::Variant;

// QVariant::toInt (qConvertToInt: double -> qRound64, QString -> QString::toInt)
inline int toInt(const Variant& v)
{
    switch (v.kind) {
    case Variant::Int:
    case Variant::Bool: return v.i;
    case Variant::Double: {
        const double d = v.d;
        const long long r = d >= 0.0 ? static_cast<long long>(d + 0.5)
                                     : static_cast<long long>(d - static_cast<double>(static_cast<long long>(d - 1)) + 0.5) +
                                           static_cast<long long>(d - 1);
        return static_cast<int>(r);
    }
    case Variant::Text: return qt::toInt(v.s);
    default: return 0;
    }
}

// QVariant::toDouble
inline double toDouble(const Variant& v)
{
    switch (v.kind) {
    case Variant::Double: return v.d;
    case Variant::Int:
    case Variant::Bool: return static_cast<double>(v.i);
    case Variant::Text: return qt::toDouble(v.s);
    default: return 0.0;
    }
}

// QVariant::toBool (QString: false for "", "0" and "false" after toLower)
inline bool toBool(const Variant& v)
{
    switch (v.kind) {
    case Variant::Int:
    case Variant::Bool: return v.i != 0;
    case Variant::Double: return v.d != 0.0;
    case Variant::Text: {
        std::u16string s = v.s.u16();
        for (char16_t& c : s)
            if (c >= u'A' && c <= u'Z') c = static_cast<char16_t>(c + 32);
        return !(s.empty() || s == u"0" || s == u"false");
    }
    default: return false;
    }
}

// QVariant::toString
inline String toString(const Variant& v)
{
    switch (v.kind) {
    case Variant::Text: return v.s;
    case Variant::Int: return qt::number(v.i);
    case Variant::Double: return qt::number(v.d, 'g', 15);
    case Variant::Bool: return qt::L(v.i ? "true" : "false");
    default: return String();
    }
}

inline Variant vInt(int i)
{
    Variant r;
    r.kind = Variant::Int;
    r.i = i;
    return r;
}
inline Variant vDouble(double d)
{
    Variant r;
    r.kind = Variant::Double;
    r.d = d;
    return r;
}
inline Variant vBool(bool b)
{
    Variant r;
    r.kind = Variant::Bool;
    r.i = b ? 1 : 0;
    return r;
}
inline Variant vText(const String& s)
{
    Variant r;
    r.kind = Variant::Text;
    r.s = s;
    return r;
}

}  // namespace m2m::pv
