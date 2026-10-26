// P06: the wizard files read at open (Boolean.BWZ, Forming.FWZ, Cycle.CWZ, DieStressBmWizard.Wzd)
// and the PROPS->vt[0xc0] post-load hooks. Decompiles: deform-rea-work/gui_mo/s1/p06/rd1.c.
#include "m2m/qt4path.hpp"
#include "m2m/xml.hpp"
#include "readers.hpp"

namespace m2m {

// @src 0x140760430  Cogging: 0x140761c90(PROPS+0x40)
void CoggingProps::postLoad(Project& p, Ctx& ctx)
{
    if (owner) datafiles::readCoggingMdt(p, *owner, *this, ctx);
}

// @src 0x1411e09d0  Boolean: 0x1411e1cd0(PROPS+0x38)
void BooleanProps::postLoad(Project& p, Ctx& ctx)
{
    if (owner) datafiles::readBooleanBwz(p, *owner, *this, ctx);
}

// @src 0x141402060  Express: if (PROPS+0x38) 0x1413f8280(PROPS+0x38)
void ExpressProps::postLoad(Project& p, Ctx& ctx)
{
    if (owner) datafiles::readExpressFwz(p, *owner, *this, ctx);
}

// @src 0x14134ec40  Cycle
void CycleProps::postLoad(Project& p, Ctx& ctx)
{
    if (owner) datafiles::readCycleCwz(p, *owner, *this, ctx);
}

// @src 0x14029eea0  DiestressBM: 0x1402a1df0(PROPS+0x38, 0x14029eef0(PROPS))
void DiestressBMProps::postLoad(Project& p, Ctx& ctx)
{
    if (owner) datafiles::readDiestressBmWzd(p, *owner, *this, ctx);
}

}  // namespace m2m

namespace m2m::datafiles {

using qt::L;

// ---- Boolean.BWZ ---------------------------------------------------------------------------------

bool readBwzFile(TextFile& f, data::BooleanData& d)
{
    TokenReader r;                         // auStack_108 (0x140906740)
    std::string word;                      // auStack_178
    for (;;) {
        int n = r.read(f, TokenReader::Upper, &word, TokenReader::kNewLine);
        if (n == -1) return false;
        if (word == "WZD_MACHDIST_WORKPIECE_OBJECT_NO") {          // 0x142cd6080
            r.read(f, TokenReader::Int, &d.workpiece, 0);
        } else if (word == "WZD_BOOLEAN_CUTTER_OBJECT_NO") {       // 0x142cd60a8
            r.read(f, TokenReader::Int, &d.cutter, 0);
        } else if (word == "WZD_BOOLEAN_REMESH") {                 // 0x142cd60c8
            r.read(f, TokenReader::Bool, &d.remesh, 0);
        } else if (word == "$END") {                               // 0x142cd60dc
            break;
        }
        if (word == "DONE") break;                                 // 0x142cd60e4
    }
    // 0x14005f770(item) afterwards only decides the return value.
    return true;
}

// @src 0x1411e1cd0 -> 0x1411e1dd0
void readBooleanBwz(Project& p, Operation& op, BooleanProps& props, Ctx& ctx)
{
    if (!props.data) props.data = std::make_shared<data::BooleanData>();   // see @src 0x1411e23f0 (P12)
    String dir = taskDirAbs(op);
    if (dir.isNull()) return;
    String name = L("Boolean.BWZ");                                // see @src 0x1411e0a00 (P12)
    String path = qt::convertSeparators(dir + qt::kSep + name);
    TextFile f;
    if (!TextFile::open(p, path, f)) return;                       // missing: defaults, no message
    if (!readBwzFile(f, *props.data))
        ctx.warn("ERROR LOADING FILE: Error occurred when file " + qt::toAscii(name) +
                 " was being read. Please check the format of the file.");
}

// ---- Forming.FWZ ---------------------------------------------------------------------------------

void readFwzFile(TextFile& f, data::ExpressData& d)
{
    TokenReader r;                         // the data object's own base (+0x00)
    std::string word;                      // auStack_168
    std::string text;                      // auStack_388
    std::int32_t iv = 0;                   // auStack_118
    double revAngle = 0.0;                 // uStack_f8 (dropped)
    std::int32_t key = 0, value = 0;       // auStack_e4, uStack_e8
    do {
        r.read(f, TokenReader::Upper, &word, TokenReader::kNewLine);
        if (word == "WZD_FORM_2D_GEOMETRY_TYPE") {
            r.read(f, TokenReader::Int, &iv, 0);
            d.geometryType2DSet = 1;
            d.geometryType2D = iv;
        } else if (word == "WZD_FORM_FORMING_TYPE") {
            r.read(f, TokenReader::Int, &iv, 0);
            d.formingType = iv;
        } else if (word == "WZD_FORM_HEATTRANS_TYPE") {
            r.read(f, TokenReader::Int, &iv, 0);
            d.heatTransType = iv;
        } else if (word == "WZD_FORM_SHAPE_COMPLEXITY") {
            r.read(f, TokenReader::Int, &iv, 0);
            d.shapeComplexity = iv;
        } else if (word == "WZD_FORM_DIE_TEMPERATURE_TYPE") {
            r.read(f, TokenReader::Int, &iv, 0);
            d.dieTemperatureType = iv;
        } else if (word == "WZD_FORM_SIMULATION_ACCURACY") {
            r.read(f, TokenReader::Int, &iv, 0);
            d.simulationAccuracy = iv;
        } else if (word == "WZD_FORM_BUILT_IN_FLOWNET") {
            r.read(f, TokenReader::Double, &d.builtInFlownet, 0);
        } else if (word == "WZD_FORM_CONTACT_TOLRANCE") {
            r.read(f, TokenReader::Double, &d.contactTolerance, 0);
        } else if (word == "WZD_FORM_WORKPIECE_SHAPE") {
            r.read(f, TokenReader::Int, &iv, 0);
            d.workpieceShape = iv;
        } else if (word == "WZD_FORM_REV_ANGLE") {
            r.read(f, TokenReader::Double, &revAngle, 0);
        } else if (word == "WZD_FORM_APPROXIMATE_DIE_STROKE") {
            r.read(f, TokenReader::Double, &d.approxDieStroke, 0);
        } else if (word == "WZD_FORM_DIE_STROKE_USE_EXACT") {
            r.read(f, TokenReader::Bool, &d.dieStrokeUseExact, 0);
        } else if (word == "WZD_FORM_WORKPIECE_GEO_CHANGED") {
            r.read(f, TokenReader::Bool, &d.workpieceGeoChanged, 0);
        } else if (word == "WZD_FORM_IMPORT_MAT_CAT_WORKPIECE") {
            r.read(f, TokenReader::Text, &text, 0);
            d.importMatCatWorkpiece = String::latin1(text);    // QString = const char*
        } else if (word == "WZD_FORM_IMPORT_MAT_CAT_DIE") {
            r.read(f, TokenReader::Text, &text, 0);
            d.importMatCatDie = String::latin1(text);
        } else if (word == "WZD_FORM_STEP_DEFINITION") {
            r.read(f, TokenReader::Int, &iv, 0);
            d.stepDefinition = iv;
        } else if (word == "WZD_MESH_TYPE_DEFINITION") {
            std::int32_t n = 0;
            r.read(f, TokenReader::Int, &n, 0);
            for (int i = 0; i < n; i = i + 1) {
                r.read(f, TokenReader::Int, &key, 0);
                r.read(f, TokenReader::Int, &value, 0);
                d.meshType[key] = value;   // 0x140066fb0 QMap<int,int>::operator[]
            }
        }
    } while (word != "DONE");
    d.loaded = 1;
}

// @src 0x1413f8280 -> 0x1413f83e0
void readExpressFwz(Project& p, Operation& op, ExpressProps& props, Ctx& ctx)
{
    (void)ctx;
    if (!props.data) props.data = std::make_shared<data::ExpressData>();
    String dir = taskDirAbs(op);
    if (dir.isNull()) return;
    data::ExpressData& d = *props.data;
    d.meshType.clear();                    // 0x1400b5090(+0xc8)
    // 0x1413f7230 also names "Forming.FWZ"; that copy of the path is unused.
    String path = qt::convertSeparators(dir + qt::kSep + L("Forming.FWZ"));
    d.path = path;                         // +0xe0
    TextFile f;                            // fopen(toLocal8Bit(path), "rt")
    if (!TextFile::open(p, path, f)) return;
    readFwzFile(f, d);
}

// ---- Cycle.CWZ -----------------------------------------------------------------------------------

void readCwzFile(TextFile& f, data::CycleData& d)
{
    TokenReader r;                         // auStack_60
    std::string word;                      // auStack_b8
    std::int32_t v = 0;                    // uStack_58
    do {
        int n = r.read(f, TokenReader::Upper, &word, TokenReader::kNewLine);
        if (n == -1) return;
        if (word == "WZD_CYCLE_COUNT") {                           // 0x142de2490
            r.read(f, TokenReader::Int, &v, 0);
            d.count = v;                                           // 0x14134ee90
        }
    } while (word != "DONE");                                      // 0x142de24a0
}

// @src 0x14134ec40
void readCycleCwz(Project& p, Operation& op, CycleProps& props, Ctx& ctx)
{
    (void)ctx;
    if (!props.data) props.data = std::make_shared<data::CycleData>();   // see @src 0x14134ef10 (P13)
    String dir = taskDirAbs(op);
    if (dir.isNull()) return;
    String path = dir + qt::kSep + L("Cycle.CWZ");                 // QString + const char*, no convertSeparators
    TextFile f;
    if (!TextFile::open(p, path, f)) return;
    readCwzFile(f, *props.data);
}

// ---- DieStressBmWizard.Wzd -----------------------------------------------------------------------

void readWzdBytes(const std::string& bytes, data::DiestressBMData& d)
{
    // QIODevice::Text drops '\r' on read (Qt 4.8 QIODevice::read).
    std::string text;
    text.reserve(bytes.size());
    for (char c : bytes)
        if (c != '\r') text += c;
    xml::Document doc;                     // QDomDocument("DieStressBmWizard")
    xml::setContent(doc, text, nullptr, nullptr, nullptr);
    const xml::Node* root = nullptr;       // firstChildElement("DieStressBmWizard") of the document
    if (doc.root && doc.root->kind == xml::Node::Element && doc.root->tag == L("DieStressBmWizard"))
        root = doc.root.get();
    auto attr = [](const xml::Node* e, const char* name) {
        return e ? e->attribute(L(name), String()) : String();
    };
    d.objTotalNum = qt::toInt(attr(root, "nObjTotalNum"));
    d.newDbName = attr(root, "qsNewDbName");
    d.useNewDb = qt::toInt(attr(root, "bUseNewDb")) != 0 ? 1 : 0;
    if (d.objTotalNum > 15)
        throw Error(M2M_E_INPUT, "DieStressBmWizard.Wzd: more than 15 objects (the original "
                                 "writes past its arrays)");
    for (int k = 0; k < d.objTotalNum; k = k + 1) {
        // "ObjId00" with bytes 5 and 6 replaced: '0' + k/10, '1' + k%10 (k = 9 gives ':')
        char name[8] = {'O', 'b', 'j', 'I', 'd', static_cast<char>(k / 10 + '0'),
                        static_cast<char>(k % 10 + '1'), '\0'};
        const xml::Node* e = nullptr;
        if (root) {
            auto list = root->childElements(L(name));
            if (!list.empty()) e = list.front();
        }
        const auto i = static_cast<std::size_t>(k);
        d.objNo[i] = qt::toInt(attr(e, "nObjNo"));
        d.refNo[i] = qt::toInt(attr(e, "nRefNo"));
        d.interpolate[i] = qt::toInt(attr(e, "bIntepolate")) != 0 ? 1 : 0;
        d.tolerance[i] = qt::toDouble(attr(e, "dTolerance"));
    }
}

// @src 0x14029eea0 -> 0x1402a1df0
void readDiestressBmWzd(Project& p, Operation& op, DiestressBMProps& props, Ctx& ctx)
{
    (void)ctx;
    if (!props.data) props.data = std::make_shared<data::DiestressBMData>();
    String dir = taskDirAbs(op);           // a null dir still gives a path in the original
    // see @src 0x14029eef0 (P16): convertSeparators(taskDirAbs + sep + "DieStressBmWizard.Wzd")
    String path = qt::convertSeparators(dir + qt::kSep + L("DieStressBmWizard.Wzd"));
    readDiestressBmWzdFile(p, path, *props.data);
}

bool readDiestressBmWzdFile(const Project& p, const String& path, data::DiestressBMData& d)
{
    std::string bytes;                     // QFile: any path the file system accepts
    if (!readFileBytes(p, path, bytes)) return false;   // "Error: Cannot read file ..." on qDebug only
    readWzdBytes(bytes, d);
    return true;
}

}  // namespace m2m::datafiles
