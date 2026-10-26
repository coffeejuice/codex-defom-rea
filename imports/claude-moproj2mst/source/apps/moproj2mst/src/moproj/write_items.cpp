// The .moproj writer: the four item lists (0x1400a1110 Object_List, 0x1400a16b0 Material_List,
// 0x1400a12f0 Lubricant_List, 0x1400a14d0 Equipment_List) through 0x1400a0f60 / 0x1400a0210.
// Owner L2 (coordinator 2026-10-22); written with the writer's first version.
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

namespace {

// @src 0x1400a0210  <MoItem> of a manager row
NodePtr moItem(const Project& p, const std::vector<const engine::FileEffects*>& earlier, const Item& it,
               bool withBlobs)
{
    // without blobs every item gets its Path; with them, an item holding inline data (+0x30)
    // gets <Data> instead (not reached on Save, which passes withBlobs = false)
    if (withBlobs) throw Error(M2M_E_UNSUPPORTED, "project file: items with inline data (0x1400a0210 withBlobs)");
    NodePtr e = element("MoItem");
    append(*e, textElement("Name", itemName(it)));                    // vt+0x48 (objects: the record's)
    append(*e, textElement("Id", qt::number(it.id)));                 // 0x14006b8a0
    const String path = relativeToProject(p, qt::cleanPath(it.keyPath));   // 0x14006ba60, 0x1400a42c0
    append(*e, textElement("Path", path));
    (void)earlier;
    if (it.kind == ItemKind::Object) {                                // dynamic_cast SfMoObjItem
        NodePtr oi = element("ObjectInfo");                           // record 0x140068b40 (+0x40)
        attrInt(*oi, "ObjectType", it.objectType);                    // 0x14004d5a0
        attrInt(*oi, "HasGeo", it.hasGeo ? 1 : 0);                    // 0x14004d7b0
        attrInt(*oi, "HasMesh", it.hasMesh ? 1 : 0);                  // 0x14004d800
        attr(*oi, "MatId", joinNumbers(it.matIds));                   // 0x14004db50 / 0x14004db80
        append(*e, oi);
    }
    if (it.kind == ItemKind::Material) {                              // dynamic_cast SfMoMatItem
        // @src 0x140041cd0
        const char* t = it.matType == 0 ? "Single" : it.matType == 1 ? "Mixture" : "Phase";
        append(*e, textElement("MatType", L(t)));
        if (it.matType == 1) append(*e, textElement("PhaseList", joinNumbers(it.phases)));   // 0x140042290
    }
    return e;
}

// @src 0x1400a0f60  Number = rows (vt+0x1b8), one MoItem per row (vt+0x1c0)
void itemList(const Project& p, const std::vector<const engine::FileEffects*>& earlier, const ItemManager& m,
              Node& list, bool withBlobs)
{
    attr(list, "Number", qt::number(static_cast<int>(m.rows.size())));
    for (const auto& it : m.rows) append(list, moItem(p, earlier, *it, withBlobs));
}

}  // namespace

// @src 0x1400a1110 / 0x1400a16b0 / 0x1400a12f0 / 0x1400a14d0
void managerList(xml::Document& doc, const Project& p, const std::vector<const engine::FileEffects*>& earlier,
                 const char* tag, const ItemManager& m, bool withBlobs)
{
    NodePtr list = rootChild(doc, tag, true);
    clearChildren(*list);
    itemList(p, earlier, m, *list, withBlobs);
}

// The four lists in the order 0x1400983c0 builds them (exported for the lists test)
void itemLists(xml::Document& doc, const Project& p, const std::vector<const engine::FileEffects*>& earlier,
               bool withBlobs)
{
    managerList(doc, p, earlier, "Object_List", p.objects, withBlobs);       // 0x1400a1110 (0x1400a58f0)
    managerList(doc, p, earlier, "Material_List", p.materials, withBlobs);   // 0x1400a16b0 (0x1400a5930)
    managerList(doc, p, earlier, "Lubricant_List", p.lubricants, withBlobs); // 0x1400a12f0 (0x1400a59b0)
    managerList(doc, p, earlier, "Equipment_List", p.equipment, withBlobs);  // 0x1400a14d0 (0x1400a5970)
}

}  // namespace m2m::moproj
