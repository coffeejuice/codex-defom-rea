// The .moproj writer: Operation_List 0x14009cfd0 with the properties 0x14009fb40, the blobs
// 0x1400a1890 and the ActionKeywords texts (0x1400620f0, 0x1400abfe0, 0x1400613b0).
// Owner L3 (coordinator 2026-10-22); written with the writer's first version.
#include <algorithm>
#include <string>
#include <vector>

#include "m2m/blob.hpp"
#include "m2m/engine.hpp"
#include "m2m/mst/emit.hpp"
#include "m2m/mst/stream.hpp"
#include "m2m/objects.hpp"
#include "write_internal.hpp"

namespace m2m::moproj {

// @src 0x1400a1890  <Data> with the hex of qCompress(file bytes, -1); a null element when the
// path is empty or the file cannot be opened
NodePtr blobElement(const Project& p, const std::vector<const engine::FileEffects*>& earlier, const String& path)
{
    if (path.isEmpty()) return nullptr;
    std::string bytes;
    if (!readProjectFile(p, earlier, path, bytes)) return nullptr;
    const std::string hex = qt::toHex(qt::qCompress(bytes));   // 0x1400ac100(ba, -1), toHex
    return textElement("Data", String::latin1(hex));
}

namespace {

std::u16string streamText(mst::Stream& ts)
{
    ts.flush();
    const std::string& b = ts.bytes();
    for (unsigned char c : b)
        if (c >= 0x80) throw Error(M2M_E_UNSUPPORTED, "project file: non-ASCII keyword text");
    return std::u16string(b.begin(), b.end());
}

// @src 0x140036090-free helper: QString::split("\n", SkipEmptyParts)
std::vector<String> splitLines(const std::u16string& s)
{
    std::vector<String> r;
    std::size_t b = 0;
    for (;;) {
        std::size_t e = s.find(u'\n', b);
        std::u16string part = s.substr(b, e == std::u16string::npos ? std::u16string::npos : e - b);
        if (!part.empty()) r.push_back(String(part));
        if (e == std::u16string::npos) break;
        b = e + 1;
    }
    return r;
}

// QVariant::toString of a property value (Qt 4.8: double QString::number(d, 'g', DBL_DIG), bool
// "true" / "false")
String variantText(const qt::Variant& v)
{
    switch (v.kind) {
    case qt::Variant::Double: return qt::number(v.d, 'g', 15);
    case qt::Variant::Int: return qt::number(v.i);
    case qt::Variant::Text: return v.s;
    case qt::Variant::Bool: return L(v.i ? "true" : "false");
    case qt::Variant::Invalid: break;
    }
    return String();
}

// @src 0x14009fb40  <Property>: Name (0x14036fcc0), TypeName (0x140370050), Value (0x140370200:
// the class's get slot through Props::getValue, else the stored value; QVariant::toString) or, for
// a group, the children. `ancestors` are the nodes above `n`, top level first.
NodePtr propertyElement(Props& props, PropNode& n, std::vector<PropNode*>& ancestors)
{
    NodePtr e = element("Property");
    String name = n.name;
    if (n.index >= 0) name = name + qt::arg(L("%1"), n.index + 1);   // 0x14036fcc0
    append(*e, textElement("Name", name));
    append(*e, textElement("TypeName", n.typeName));
    if (n.typeName != L("group")) {
        qt::Variant v;
        if (props.getValue(n, ancestors, v)) n.value = v;            // invokeMethod(.., node+0x10)
        append(*e, textElement("Value", variantText(n.value)));
    } else {
        ancestors.push_back(&n);
        for (PropNode& c : n.children) append(*e, propertyElement(props, c, ancestors));   // 0x140370f60
        ancestors.pop_back();
    }
    return e;
}

}  // namespace

// @src 0x14009cfd0  Operation_List rebuilt from the operation manager rows
void operationList(xml::Document& doc, Project& p, const std::vector<const engine::FileEffects*>& earlier,
                   bool withBlobs)
{
    NodePtr list = rootChild(doc, "Operation_List", true);
    clearChildren(*list);
    attr(*list, "Number", qt::number(static_cast<int>(p.operations.size())));   // vt+0x1b8
    for (const auto& opp : p.operations) {
        Operation& op = *opp;
        NodePtr e = element("Operation");
        append(*e, textElement("Name", op.name));                   // 0x140050ac0
        append(*e, textElement("Id", qt::number(op.id)));           // 0x14006b8a0 (row item id)
        append(*e, textElement("Type", op.classKey));               // 0x140050d00
        append(*e, textElement("SubType", op.subType));             // 0x140050da0
        append(*e, textElement("OprFlag", qt::number(op.oprFlag))); // 0x1400509b0
        NodePtr eq = element("Equipment");
        attr(*eq, "Id", qt::number(op.equipmentId));                // 0x140066760 (+0x4c)
        append(*e, eq);
        NodePtr lub = element("Lubricant");
        attr(*lub, "Id", qt::number(op.lubricantId));               // 0x1400657f0 (+0x48)
        attr(*lub, "InterObjectMethod", op.lubSystem ? L("System") : L("User"));   // 0x140065780(+0x70)
        append(*e, lub);
        const int n = objSlots(op);                                 // 0x14005fa80
        NodePtr objs = element("Objects");
        attr(*objs, "Number", qt::number(n));
        for (int s = 0; s < n; ++s) {
            NodePtr o = element("Object");
            attr(*o, "Id", qt::number(objId(op, s)));               // 0x14005fb30
            if (ObjEntry* r = objEntry(op, s)) {                    // 0x140061250
                attr(*o, "Name", r->name);                          // 0x14004d730
                attrInt(*o, "ObjectType", r->objectType);           // 0x14004d5a0
                attrInt(*o, "HasGeo", r->hasGeo ? 1 : 0);
                attrInt(*o, "HasMesh", r->hasMesh ? 1 : 0);
                attrInt(*o, "EquipmentID", r->equipmentId);         // 0x14004de90
                attr(*o, "MatId", joinNumbers(r->matIds));
                attrInt(*o, "Remesh", r->remesh ? 1 : 0);           // 0x14004df80
                attrInt(*o, "SkipDefine", r->skipDefine ? 1 : 0);   // 0x14004df40
                attrInt(*o, "MeshSetting", r->meshSetting ? 1 : 0);
                attrInt(*o, "Mesh2dAbsolute", r->mesh2dAbsolute ? 1 : 0);
                attrInt(*o, "Mesh3dFinerInternal", r->mesh3dFinerInternal ? 1 : 0);
                attrInt(*o, "DefineBCC", r->defineBCC ? 1 : 0);
                attrInt(*o, "AssignMovement", r->assignMovement ? 1 : 0);
                attrInt(*o, "DefineRotationMovement", r->defineRotationMovement ? 1 : 0);
                // @src 0x14004e180  type 5 ? +0x1c1 : 1
                attrInt(*o, "ThermalStop", r->objectType == 5 ? (r->thermalStop ? 1 : 0) : 1);
                NodePtr key = element("ObjPropertiesKey");
                append(*key, blobElement(p, earlier, objPropertiesPath(op, s)));   // 0x14005eb10
                append(*o, key);
            }
            append(*objs, o);
        }
        append(*e, objs);
        if (withBlobs) throw Error(M2M_E_UNSUPPORTED, "project file: SimCtrlKey / InterObjectKey blobs (withBlobs)");
        // ActionKeywords (each keyword is a fresh QString written through a QTextStream)
        NodePtr kw = element("ActionKeywords");
        {
            std::u16string t = u"TLOC      \t";
            const std::string d = qt::formatDouble(op.tloc);       // 0x1400ac0e0, operator<<(double)
            t.append(d.begin(), d.end());
            append(*kw, textElement("Keyword", String(t)));
        }
        {
            mst::Stream ts(1252);
            mst::State st;
            Ctx cctx;
            mst::W w{ts, st, cctx};
            for (const auto& pos : op.objpos)                       // 0x1400620f0 (OPI+0x60)
                if (pos) {
                    if (is3D(op)) mst::emitObjposLine3D(w, *pos);   // 0x140065f80
                    else mst::emitObjposLine2D(w, *pos);            // 0x140065af0
                }
            append(*kw, textElement("Keyword", String(streamText(ts))));
        }
        {
            mst::Stream ts(1252);
            mst::State st;
            Ctx cctx;
            mst::W w{ts, st, cctx};
            mst::emitContactBlock(w, op.icb);                       // 0x1400abfe0 -> 0x1400ac010
            for (const String& line : splitLines(streamText(ts))) append(*kw, textElement("Keyword", line));
        }
        {
            mst::Stream ts(1252);
            mst::State st;
            Ctx cctx;
            mst::W w{ts, st, cctx};
            mst::emitObjectStateVars(w, op);                        // 0x1400613b0
            append(*kw, textElement("Keyword", String(streamText(ts))));
        }
        append(*e, kw);
        NodePtr ns = element("NSTART");
        if (op.nstartGiven) append(*ns, textElement("StartStepNo", qt::number(op.startStepNo)));   // 0x14005a2a0/2e0
        append(*e, ns);
        NodePtr uk = element("UserDefinedKey");
        if (op.userKeyGiven) append(*uk, textElement("KeyFileName", op.userKey));   // 0x140062e40/e80
        append(*e, uk);
        NodePtr rk = element("RestartKey");
        if (op.restartGiven) append(*rk, textElement("KeyFileName", restartKeyPath(op, false)));   // 0x140064a50(op, 0)
        append(*e, rk);
        if (op.props) {                                             // 0x140051230 (+0x10 tree)
            NodePtr pr = element("Properties");
            attr(*pr, "Number", qt::number(static_cast<int>(op.props->tree.size())));
            std::vector<PropNode*> anc;
            try {
                for (PropNode& n : op.props->tree) append(*pr, propertyElement(*op.props, n, anc));
            } catch (const Error& x) {
                // name the operation class in the refusal (diagnostic only)
                if (x.code != M2M_E_UNSUPPORTED) throw;
                throw Error(x.code, std::string(x.what()) + " [" + op.classKey.utf8() + "]");
            }
            append(*e, pr);
        }
        append(*list, e);
    }
}


}  // namespace m2m::moproj
