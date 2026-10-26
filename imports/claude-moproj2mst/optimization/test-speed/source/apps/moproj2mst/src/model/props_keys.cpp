// Operation properties, NSTART / UserDefinedKey / RestartKey and ObjProperties blobs (owner: P08,
// the "P02 split" rows of docs/re/moproj2mst/owners.tsv). Spec: s1-01 5.4, 5.5, 6.3, 6.4; decompiles
// s1-01 dc/open3.c, open4.c, acc0.c, opr3.c, props0.c and p15/m10.c (0x140370600).
#include "m2m/blob.hpp"
#include "m2m/model.hpp"
#include "m2m/props.hpp"
#include "m2m/qt4path.hpp"
#include "m2m/native_directories.hpp"
#include "m2m/mst/file_device.hpp"

namespace m2m {

using qt::L;

// ---- OPI field accessors (s1-01 9.3) -------------------------------------------------------------

// @src 0x14005a2a0
bool nstartGiven(const Operation& op) { return op.nstartGiven != 0; }
// @src 0x14005a2c0
void setNstartGiven(Operation& op, bool v) { op.nstartGiven = v ? 1 : 0; }
// @src 0x14005a2e0
int startStepNo(const Operation& op) { return op.startStepNo; }
// @src 0x14005a350
void setStartStepNo(Operation& op, int v) { op.startStepNo = v; }

// @src 0x14005a370
void resetNstart(Operation& op)
{
    op.nstartGiven = 0;
    op.startStepNo = 0;
}

// @src 0x14005a300
int startStep(const Operation& op)
{
    if (!nstartGiven(op)) return 0;
    int v = startStepNo(op);
    if (v < 0) v = -1 - v;
    return v;
}

// @src 0x140062e60
void setUserKeyGiven(Operation& op, bool v) { op.userKeyGiven = v ? 1 : 0; }
// @src 0x140062ed0
void setUserKey(Operation& op, const String& s) { op.userKey = s; }

// @src 0x140064740
bool restartGiven(const Operation& op) { return op.restartGiven != 0; }

// @src 0x140064760  (emit = true sends signalSetOprInteractiveMode(); no receiver in the converter)
void setRestartGiven(Operation& op, bool v, bool /*emit*/)
{
    if ((op.restartGiven != 0) != v) op.restartGiven = v ? 1 : 0;
}

// @src 0x140064c60
void setRestartKey(Operation& op, const String& s) { op.restartKey = s; }

// @src 0x140064a50
String restartKeyPath(const Operation& op, bool raw)
{
    String s;
    String dir = taskDirAbs(op);   // 0x140050df0 (native separators)
    s = dir + qt::kSep + op.restartKey;
    if (!raw) s = qt::relativeFilePath(taskDirAbs(op), s);
    return qt::cleanPath(s);
}

// ---- NSTART, UserDefinedKey, RestartKey (s1-01 5.5) ----------------------------------------------

// @src 0x140095e40
void parseNstart(Project& /*p*/, const xml::Node& e, Operation& op)
{
    bool found = false;
    for (const xml::Node* c : e.childElements()) {
        if (c->tag == L("StartStepNo" /*142140020*/)) {
            setStartStepNo(op, qt::toInt(c->text()));
            found = true;
        }
    }
    setNstartGiven(op, found);
}

// @src 0x140096010
void parseUserDefinedKey(Project& /*p*/, const xml::Node& e, Operation& op)
{
    bool found = false;
    for (const xml::Node* c : e.childElements()) {
        if (c->tag == L("KeyFileName" /*142140030*/)) {
            setUserKey(op, c->text());
            found = true;
        }
    }
    setUserKeyGiven(op, found);
}

// @src 0x1400961d0
void parseRestartKey(Project& /*p*/, const xml::Node& e, Operation& op)
{
    bool found = false;
    for (const xml::Node* c : e.childElements()) {
        if (c->tag == L("KeyFileName" /*142140040*/)) {
            setRestartKey(op, c->text());
            found = true;
        }
    }
    setRestartGiven(op, found, false);
}

// ---- ObjProperties blobs (s1-01 5.4) -------------------------------------------------------------

// @src 0x1400ab710  qUncompress(ba.constData(), ba.size())
std::string uncompressBlob(const std::string& bytes, std::string* error)
{
    return qt::qUncompress(bytes, error);
}

// @src 0x1400937b0  WriteBlob: every Data child is decoded and written to `path` in turn, so the
// last one wins. The original mkpaths the folder and writes the file (WriteOnly, truncate); the
// recording conversion keeps bytes in the overlay. Native conversion performs
// each directory/open/write/close attempt here and ignores its result.
void storeBlob(Project& p, const xml::Node& e, const String& path, Ctx& ctx)
{
    String text;
    for (const xml::Node* c : e.childElements()) {
        if (c->tag != L("Data" /*14213fe14*/)) continue;
        text = c->text();
        std::string hex = qt::fromHex(qt::toAscii(text));
        std::string error;
        std::string bytes = uncompressBlob(hex, &error);
        if (!error.empty()) ctx.warn(error + " (" + path.utf8() + ")");
        if (ctx.nativeOperationBlobs) {
            if (ctx.beforeNativeOperationBlob) ctx.beforeNativeOperationBlob(path.u16());
            const auto directory = qt::absolutePath(path); // 140819590
            const bool made = qt::nativeMkpathAbsolute(directory);
            ctx.blobDirectoryAttempts.push_back({directory.u16(), made});
            // 14081afb0 failure dialog remains a qualified event boundary.
            // Its result does not skip the QFile attempt at14009398b.
            mst::FileDevice target(path); // QFile WriteOnly2, binary QByteArray
            if (target.opened()) {
                p.overlay.erase(qt::cleanPath(path));
                (void)target.write(bytes);
                target.close(); //1400939de: no write/close result inspected
            }
        } else {
            p.overlay[qt::cleanPath(path)] = std::move(bytes);
        }
    }
}

// @src 0x140093ab0
void parseObjPropertiesKey(Project& p, const xml::Node& obj, Operation& op, int slot, Ctx& ctx)
{
    for (const xml::Node* c : obj.childElements()) {
        if (c->tag == L("ObjPropertiesKey" /*14213fe20*/))
            storeBlob(p, *c, objPropertiesPath(op, slot) /*0x14005eb10*/, ctx);
    }
}

// ---- Properties (s1-01 6.3) ----------------------------------------------------------------------

// @src 0x140cac130  PROPS vt+0xc0 of SfOprBaseProperties and most classes: nothing
void Props::postLoad(Project&, Ctx&) {}

// @src 0x140353770  top-level nodes of the property tree (a copy of the QList at tree+8)
std::vector<PropNode>& propRoots(Props& props) { return props.tree; }

// The original holds QtProperty pointers, which never move. Here a class's applyValue may add or
// remove children anywhere above the node (setNumHeats grows the root's children, setNumBlows a
// heat's), which moves vector elements. So nodes are addressed by index paths from the top level
// and every pointer is rebuilt from props.tree after each slot call. Slots append or drop nodes at
// the end of a level, so the indices of the nodes on the path stay valid.
static std::vector<PropNode>* levelAt(Props& props, const std::vector<int>& path)
{
    std::vector<PropNode>* level = &props.tree;
    for (int k : path) {
        if (k < 0 || k >= static_cast<int>(level->size())) return nullptr;
        level = &(*level)[static_cast<size_t>(k)].children;
    }
    return level;
}

static std::vector<PropNode*> ancestorsAt(Props& props, const std::vector<int>& path)
{
    std::vector<PropNode*> chain;
    std::vector<PropNode>* level = &props.tree;
    for (int k : path) {
        PropNode* n = &(*level)[static_cast<size_t>(k)];
        chain.push_back(n);
        level = &n->children;
    }
    return chain;
}

// @src 0x140370600  setValue(node, v): the node's slot is invoked with v first (Props::applyValue),
// then the value is stored. The valueChanged signal to 0x143d76700 has no receiver here. `path` is
// the node's parent chain and i its index; both are resolved again after the slot.
static void setPropValue(Props& props, const std::vector<int>& path, int i, const qt::Variant& v)
{
    std::vector<PropNode>* level = levelAt(props, path);
    if (!level || i >= static_cast<int>(level->size())) return;
    props.applyValue((*level)[static_cast<size_t>(i)], ancestorsAt(props, path), v);
    level = levelAt(props, path);
    if (level && i < static_cast<int>(level->size())) (*level)[static_cast<size_t>(i)].value = v;
}

// @src 0x14036fcc0  SfProperty name: the template's Name, plus QString("%1").arg(index + 1) when the
// node has an index (SfProperty+0x48 >= 0), so heat k's NumBlows is "NumBlows<k+1>"
static String propName(const PropNode& n)
{
    if (n.index < 0) return n.name;
    return n.name + qt::arg(L("%1" /*1423275e4*/), n.index + 1, 0, 10, u' ');
}

static bool containsCs(const String& s, const char* what)
{
    return qt::contains(s, L(what), qt::Case::Sensitive);
}

// @src 0x1400952c0  ApplyProperty(P, e, candidates). The candidates are the children of the node at
// `path` (the top level when path is empty), looked up again on every use (see levelAt). Only the
// first `count` of them (the original's copy of the list at entry) are searched by name.
void applyProperty(Props& props, const xml::Node& e, const std::vector<int>& path)
{
    String name, typeName, value;
    std::vector<PropNode>* level = levelAt(props, path);
    if (!level) return;
    const int count = static_cast<int>(level->size());
    int matched = -1;   // null QPointer
    auto node = [&]() -> PropNode* {
        std::vector<PropNode>* l = levelAt(props, path);
        return l && matched >= 0 && matched < static_cast<int>(l->size()) ? &(*l)[matched] : nullptr;
    };
    for (const xml::Node* c : e.childElements()) {
        if (c->tag == L("Name" /*14213ff7c*/)) {
            name = c->text();
            std::vector<PropNode>& candidates = *levelAt(props, path);
            for (int i = 0; i < count && i < static_cast<int>(candidates.size()); ++i) {
                String candName = propName(candidates[static_cast<size_t>(i)]);   // 0x14036fcc0
                bool legacy = containsCs(name, "HeatingExpress" /*14213ff88*/) ||
                              containsCs(name, "AirConvection" /*14213ff98*/) ||
                              containsCs(name, "HeatResting" /*14213ffa8*/) ||
                              containsCs(name, "HeatDwelling" /*14213ffb8*/);
                if (!legacy) {
                    if (candName == name) {
                        matched = i;
                        break;
                    }
                } else if (containsCs(candName, "TrasnferExpress" /*14213ffc8*/) ||
                           containsCs(candName, "HeatingExpress" /*14213ffd8*/)) {
                    matched = i;
                    break;
                }
            }
            if (!node()) return;   // 0x140c1aec0: nothing matched, the rest is ignored
        } else if (c->tag == L("TypeName" /*14213ffe8*/)) {
            typeName = c->text();
        } else if (c->tag == L("Property" /*14213fff8*/)) {
            if (!node()) continue;   // the original dereferences a null QPointer here
            std::vector<int> sub = path;
            sub.push_back(matched);
            applyProperty(props, *c, sub);   // candidates = the matched node's children (0x140370f60)
        } else if (c->tag == L("Value" /*142140004*/)) {
            value = c->text();
            qt::Variant v;
            if (typeName == L("double" /*14214000c*/)) {
                v.kind = qt::Variant::Double;
                v.d = qt::toDouble(value);
            } else if (typeName == L("int" /*14213f44c*/)) {
                v.kind = qt::Variant::Int;
                v.i = qt::toInt(value);
            } else {
                v.kind = qt::Variant::Text;
                v.s = value;
            }
            if (!node()) continue;   // as above
            setPropValue(props, path, matched, v);
        }
    }
}

// @src 0x140094f70  ParseProperties. Without an OPI or PROPS the original logs
// "Error on reading project file!" (qCritical) and returns false; ParseOperation then fails on the
// missing PROPS itself. (props+0x10)->vt[+0x8] runs first: nothing for every class but MultiBlow,
// whose slot clears two flags the ctor set (Props::beforeParse).
void parseProperties(Project& /*p*/, const xml::Node& e, Operation& op, Ctx& /*ctx*/)
{
    if (!op.props) return;
    Props& props = *op.props;
    props.beforeParse();
    propRoots(props);   // the top level is path {} below
    for (const xml::Node* c : e.childElements()) {
        if (c->tag == L("Property" /*14213ff70*/)) applyProperty(props, *c, {});
    }
}

}  // namespace m2m
