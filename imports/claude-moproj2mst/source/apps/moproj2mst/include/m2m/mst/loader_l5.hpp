// m2m/mst/loader_l5.hpp: operation-loader slots of SfOprBoolean, SfOprConvert2Dto3D and
// SfOprCopyMirror (packet MDT L5, docs/re/moproj2mst/mdt/L5/spec_L5.md).
//
// The classes (src/ops/boolean.cpp, convert23.cpp, copymirror.cpp) override their OpClass slots and
// call the bodies through this table, which src/engine/l5_slots.cpp fills from installLoaderSlots,
// so the standalone writer tests that compile those files without the engine still link. Class
// members the slots use keep their offsets in the comments.
#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <vector>

#include "m2m/common.hpp"
#include "m2m/model.hpp"

namespace def::model {
class LLOBJ_DATA;
}

namespace m2m::engine {
struct FileEffects;
struct GuiModel;
}

namespace m2m::data {
struct ConvertData;
struct BooleanData;
}

namespace m2m::mst {
class OpClass;

// SfObjectSetupBool: the Object editor's filled combo selections. The first
// entry is Add New Object; other entries are "(number) name" (0x1411e25b0).
struct BooleanObjectEditor {
    bool shown = false;
    String workpieceText, cutterText;
};

// SfConv23Config (ctor 0x1401e1a80, setupUi 0x1401e4120, init 0x1401e1c10): the editor of the
// object tree's "Configuration" item, which the GUI shows after the operation is opened. Only what
// its slots and vt+0x1d0 read back is kept: the line-edit texts, the "Y is up" radio, the object and
// the GEO23 copy taken after the init (+0x50).
struct Conv23ConfigEditor {
    bool shown = false;
    int objNo = 1;                 // vt+0x1a0 0x140130d90: item data 0 -> 1
    bool hasObject = false;        // +0x48 (object of objNo, kind != 7)
    bool yup = false;              // ui+0x20 "Y is up" (setupUi checks ui+0x18 "Z is up")
    String revAngle, revStart, revTol, extLen, extStart;   // ui+0xc0, +0xb0, +0x70, +0xe8, +0xf8
    std::vector<std::uint8_t> geo23;   // +0x50: copy of the object's GEO23 block (0x128 bytes)
};

// SfOprConvert2Dto3D members of the load path
struct Convert2Dto3DState {
    Conv23ConfigEditor editor;
    bool geometryTypeShown = false;           // first tree item for a chain head (0x1401c5000)
    int geometryType = 1;                     // 2D radio selection; setupUi 0x141058f20
    std::uint8_t b60 = 0;                      // +0x60 (0x1401c23d0: 0; the M23_INPUT CNVT3D choice)
    std::uint8_t loading = 0;                  // +0x130 set around SfOprBase::load in vt+0x58
    std::shared_ptr<engine::GuiModel> work;    // +0x138 3D work model (0x1401c7370 creates it)
};

// One plane line of mirrormerge.ini (0x28 bytes, appended by 0x140e2a030)
struct MirrorPlane {
    int a = 0, b = 0, c = 0;       // +0x00 .. +0x08 (a becomes the viewer plane's +0x104)
    float v[6] = {};               // +0x0c .. +0x20 (point and normal, read as double, kept as float)
    int flag = 0;                  // +0x24
};

// One MIRROR_MERGE block (0x140e29e70: object number, then its planes)
struct MirrorEntry {
    int objNo = 0;                 // +0x00
    std::vector<MirrorPlane> planes;   // +0x08
};

// SfOprCopyMirror members of the load path
struct CopyMirrorState {
    std::map<int, double> tolerances;          // +0x138 per object number (0x140e29d30; nothing on
                                               // the load and save paths fills it)
    double tolerance = 0.0;        // +0x148 TOLERANCE
    String kfread;                 // +0x150 the line after "KFREAD n"
    std::vector<MirrorEntry> entries;   // +0x158 MIRROR_MERGE blocks
};

struct L5Slots {
    // SfOprBoolean
    void (*booleanPrologue)(OpClass&, Project&, Operation&) = nullptr;                  // 0x1411def50
    void (*booleanPostLoad)(OpClass&, Project&, Operation&, engine::FileEffects&) = nullptr;   // 0x1411df020
    bool (*booleanEditorFill)(BooleanObjectEditor&, Project&, Operation&) = nullptr;   // 0x1411e25b0
    bool (*booleanEditorApply)(BooleanObjectEditor&, Project&, Operation&,
                               engine::FileEffects&) = nullptr;                      // 0x1411e2870
    void (*booleanSideFiles)(Project&, Operation&, const data::BooleanData&,
                             Ctx&, engine::FileEffects&) = nullptr;                   // 0x1411e1820
    // SfOprConvert2Dto3D; tempStep = the class's SfOprConvert2Dto3D_TEMP_STEP.KEY path (0x1401c6f20,
    // owner P12, computed by the class)
    void (*c23Load)(OpClass&, Convert2Dto3DState&, const String& tempStep, Project&, Operation&, Ctx&,
                    engine::FileEffects&) = nullptr;                                     // 0x1401c3a60
    void (*c23Prologue)(OpClass&, Project&, Operation&) = nullptr;                      // 0x1401c2970
    void (*c23PostLoad)(OpClass&, Convert2Dto3DState&, const String& tempStep, Project&, Operation&,
                        Ctx&, engine::FileEffects&) = nullptr;                           // 0x1401c29d0
    void (*c23PropsLoad)(Project&, Operation&, data::ConvertData&) = nullptr;           // 0x1401ce3e0
    // SfOprCopyMirror
    void (*cmLoad)(OpClass&, Project&, Operation&, Ctx&, engine::FileEffects&) = nullptr;   // 0x140e24b80
    void (*cmPrologue)(OpClass&, Project&, Operation&) = nullptr;                       // 0x140e22900
    void (*cmPostLoad)(OpClass&, CopyMirrorState&, Project&, Operation&, Ctx&,
                       engine::FileEffects&) = nullptr;                                  // 0x140e24b50
    void (*cmPropsLoad)(Project&, Operation&, data::ConvertData&) = nullptr;            // 0x140e2d080
    bool (*cmProvideObject)(OpClass&, CopyMirrorState&, Project&, int,
                            def::model::LLOBJ_DATA*) = nullptr;                          // 0x140e25980
    // ---- save current operation (MDT L5S); paths computed by the classes (owner P12) ----
    void (*c23SaveFiles)(OpClass&, Convert2Dto3DState&, const String& m23, const String& tempStep,
                         const String& convStatus, Project&, Operation&, engine::FileEffects&) = nullptr;   // 0x1401c3be0
    bool (*c23PropsToEngine)(Project&, Operation&, data::ConvertData&) = nullptr;       // 0x1401ce590
    bool (*c23UpdateData)(Convert2Dto3DState&, Project&, Operation&) = nullptr;          // selected editor vt+0x1c0
    void (*c23SyncTail)(OpClass&, Convert2Dto3DState&, const String& tempStep, Project&,
                        engine::FileEffects&) = nullptr;                              // 0x1401c3570 (vt+0xc8)
    void (*cmSaveFiles)(OpClass&, const String& stepKey, Project&, Operation&, engine::FileEffects&) =
        nullptr;                                                                         // 0x140e24cf0
    void (*cmSaveAfter)(OpClass&, CopyMirrorState&, const String& stepKey, Project&, Operation&,
                        engine::FileEffects&) = nullptr;                                 // 0x140e24b30
    bool (*cmPropsToEngine)(Project&, Operation&, data::ConvertData&) = nullptr;        // 0x140e2d230
};
inline L5Slots g_l5Slots{};

// A slot body the table does not have (engine not linked)
template <class F> F& l5Slot(F& f)
{
    if (!f) throw Error(M2M_E_UNSUPPORTED, "operation loader (MDT L5) not linked");
    return f;
}

}  // namespace m2m::mst
