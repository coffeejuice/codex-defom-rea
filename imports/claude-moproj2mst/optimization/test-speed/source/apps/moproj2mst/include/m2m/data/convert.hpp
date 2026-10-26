// m2m/data/convert.hpp: the data block of SfOprConvert2Dto3DProperties and SfOprCopyMirrorProperties
// (props+0x38, 0x40 bytes, ctor 0x140e2d400, owner P07). P07's property templates take their
// defaults from it; MDT L5's properties load (props vt+0x58, 0x1401ce3e0 / 0x140e2d080) fills it
// from the engine model.
#pragma once

namespace m2m::data {

struct ConvertData {
    int upAxis = 3;                // +0x00
    int conType = 1;               // +0x04
    double revAngle = 360.0;       // +0x08 (0x1421249c8)
    double revStartAngle = 0.0;    // +0x10
    double revTol = 0.001;         // +0x18 (0x1421b5050)
    int revType = 0;               // +0x20
    double extLen = 5.0;           // +0x28 (0x1421d3c60)
    double extStart = 0.0;         // +0x30
    // +0x38 QPointer to the root node (set by the builder)
};

}  // namespace m2m::data

#include <string>
#include <vector>

#include "m2m/props_variant.hpp"
#include "m2m/qt4.hpp"

namespace m2m {
struct PropNode;
namespace slots {
// The eight set/get slot pairs of SfOprConvert2Dto3DProperties (qt_static_metacall 0x1401e8700)
// and SfOprCopyMirrorProperties (0x140e2d500), the same bodies on the same block (L2). False when
// `name` is not one of them.
// @src 0x1401cce90 / 0x140e2bb30 setDTUPAXIS (toInt, +0), 0x1401ccf30 / 0x140e2bbd0 setDTCONTYPE
// (toInt, +4), 0x1401ccfd0 / 0x140e2bc70 setDTREVTYPE (toInt, +0x20), 0x1401cd070 / 0x140e2bd10
// setDTREVANGLE (toDouble, +8), 0x1401cd110 / 0x140e2bdb0 setDTREVSTANGLE (+0x10), 0x1401cd1b0 /
// 0x140e2be50 setDTREVTOL (+0x18), 0x1401cd250 / 0x140e2bef0 setDTEXTLEN (+0x28), 0x1401cd2f0 /
// 0x140e2bf90 setDTEXTSTLOC (+0x30)
inline bool convertSet(data::ConvertData& d, const std::u16string& n, const qt::Variant& v)
{
    if (n == u"DTUPAXIS") d.upAxis = pv::toInt(v);
    else if (n == u"DTCONTYPE") d.conType = pv::toInt(v);
    else if (n == u"DTREVTYPE") d.revType = pv::toInt(v);
    else if (n == u"DTREVANGLE") d.revAngle = pv::toDouble(v);
    else if (n == u"DTREVSTANGLE") d.revStartAngle = pv::toDouble(v);
    else if (n == u"DTREVTOL") d.revTol = pv::toDouble(v);
    else if (n == u"DTEXTLEN") d.extLen = pv::toDouble(v);
    else if (n == u"DTEXTSTLOC") d.extStart = pv::toDouble(v);
    else return false;
    return true;
}
// @src 0x1401ccee0 / 0x140e2bb80 getDTUPAXIS, 0x1401ccf80 / 0x140e2bc20 getDTCONTYPE, 0x1401cd020 /
// 0x140e2bcc0 getDTREVTYPE, 0x1401cd0c0 / 0x140e2bd60 getDTREVANGLE, 0x1401cd160 / 0x140e2be00
// getDTREVSTANGLE, 0x1401cd200 / 0x140e2bea0 getDTREVTOL, 0x1401cd2a0 / 0x140e2bf40 getDTEXTLEN,
// 0x1401cd340 / 0x140e2bfe0 getDTEXTSTLOC
inline bool convertGet(const data::ConvertData& d, const std::u16string& n, qt::Variant& out)
{
    if (n == u"DTUPAXIS") out = pv::vInt(d.upAxis);
    else if (n == u"DTCONTYPE") out = pv::vInt(d.conType);
    else if (n == u"DTREVTYPE") out = pv::vInt(d.revType);
    else if (n == u"DTREVANGLE") out = pv::vDouble(d.revAngle);
    else if (n == u"DTREVSTANGLE") out = pv::vDouble(d.revStartAngle);
    else if (n == u"DTREVTOL") out = pv::vDouble(d.revTol);
    else if (n == u"DTEXTLEN") out = pv::vDouble(d.extLen);
    else if (n == u"DTEXTSTLOC") out = pv::vDouble(d.extStart);
    else return false;
    return true;
}
}  // namespace slots
}  // namespace m2m
