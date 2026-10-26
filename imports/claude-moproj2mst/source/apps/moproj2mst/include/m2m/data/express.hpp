// m2m/data/express.hpp: the Forming wizard data of SfOprExpressProperties (PROPS+0x38; the
// post-load hook 0x141402060 passes *(PROPS+0x38) to 0x1413f8280). Not the FormingExpress data block
// at PROPS+0x40 (s1-05 5.1, P15). Owner: P06 (Forming.FWZ reader 0x1413f83e0 fills it).
// Offsets are relative to the wizard object; field names follow the FWZ keywords.
#pragma once

#include <cstdint>
#include <map>

#include "m2m/qt4.hpp"

namespace m2m::data {

struct ExpressData {
    // +0x00 KEY token reader base (0x140906740)
    int formingType = 0;           // +0x04 WZD_FORM_FORMING_TYPE
    int dieTemperatureType = 2;    // +0x08 WZD_FORM_DIE_TEMPERATURE_TYPE
    int shapeComplexity = 50;      // +0x0c WZD_FORM_SHAPE_COMPLEXITY
    int simulationAccuracy = 50;   // +0x10 WZD_FORM_SIMULATION_ACCURACY
    int workpieceShape = 0;        // +0x14 WZD_FORM_WORKPIECE_SHAPE
    int heatTransType = -1;        // +0x18 WZD_FORM_HEATTRANS_TYPE
    double revAngle = 0.0;         // +0x20 WZD_FORM_REV_ANGLE: written by 0x1413f74a0, never read back
    double builtInFlownet = -1.0;   // +0x28 WZD_FORM_BUILT_IN_FLOWNET (token type 10)
    double contactTolerance = 0.0; // +0x30 WZD_FORM_CONTACT_TOLRANCE
    double approxDieStroke = 0.0;  // +0x38 WZD_FORM_APPROXIMATE_DIE_STROKE
    std::uint8_t dieStrokeUseExact = 0;     // +0x40 WZD_FORM_DIE_STROKE_USE_EXACT (type 1)
    std::uint8_t workpieceGeoChanged = 0;   // +0x41 WZD_FORM_WORKPIECE_GEO_CHANGED (type 1)
    qt::String importMatCatWorkpiece;       // +0xa8 WZD_FORM_IMPORT_MAT_CAT_WORKPIECE
    qt::String importMatCatDie;             // +0xb0 WZD_FORM_IMPORT_MAT_CAT_DIE
    int geometryType2D = 1;        // +0xb8 WZD_FORM_2D_GEOMETRY_TYPE
    std::uint8_t geometryType2DSet = 0;     // +0xbc set to 1 with +0xb8
    int stepDefinition = 0;        // +0xc0 WZD_FORM_STEP_DEFINITION
    std::map<int, int> meshType;   // +0xc8 WZD_MESH_TYPE_DEFINITION pairs (QMap<int,int>, cleared first)
    std::uint8_t loaded = 0;       // +0xd8 1 once the file was read
    qt::String path;               // +0xe0 the FWZ path
    // +0xd0 owner SfOprExpressProperties* (Props::owner here)
    // WZD_FORM_REV_ANGLE is read into a local and dropped (+0x20 keeps the ctor's 0).
    // Member defaults = the ctor 0x1413f68e0 (MDT L3; +0x42..+0xa4 = 0 are not modelled); the
    // reader overwrites only the keys present in the file.
};

}  // namespace m2m::data
