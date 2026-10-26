// m2m/mst/doe_model.hpp: the DOE study records the DOE emitters read (owner: P09 declares the
// types, P17 builds them). Spec: s1-05 7.3, s1-02 6.5. DOE output is M2M_E_UNSUPPORTED in stage 1;
// mst::State::doeModel points to a doe::Model and doeVarList holds doe::Var pointers.
#pragma once

#include <map>
#include <memory>
#include <optional>
#include <variant>
#include <vector>

#include "m2m/qt4.hpp"

namespace m2m::doe {
using qt::String;

// A native correspondence entry is two consecutive QPointF values (0x20
// bytes); the Upper and Lower lists are owned by their GeoFile row.
struct GeometryPoint { double x = 0.0; double y = 0.0; };
struct GeometryPointPair { GeometryPoint first; GeometryPoint second; };

struct GeoFile {                   // record in Var+0x28 (accessors 0x140817a80 / 0x140817940)
    String path;                   // +0x00 (0x140817a80)
    String before;                 // +0x08 geometry file a (0x140817940)
    String after;                  // +0x10 geometry file b
    bool useBefore = false;        // +0x18 (0x140817e20: && !before.isEmpty())
    bool useAfter = false;         // +0x19 (0x140817eb0: && !after.isEmpty())
    // Native row lookup also keys on +0x1c/+0x20. Legacy projected rows leave
    // these disengaged; the bounded row-prefix receiver refuses to dedupe a
    // mixed list whose original key is unknown.
    std::optional<int> geoOperation;
    std::optional<int> geoObject;
    std::vector<GeometryPointPair> upperCorrespondence; // native +0x28
    std::vector<GeometryPointPair> lowerCorrespondence; // native +0x30
};

struct KeywordConstructorRecord;
struct LegacyItemText {
    String before;                 // +0x18 (0x1400d7fc0)
    String after;                  // +0x20 (0x1400d8000)
};
enum class ItemKind { LegacyValue, NativeKeyword };
// The sole Var+0x30 owner: legacy text remains value-only, while a recovered
// keyword record has one concrete owner and a stable borrowed pointer identity.
class Item {
public:
    Item();
    Item(String before, String after);
    ~Item();
    Item(Item&&) noexcept;
    Item& operator=(Item&&) noexcept;
    Item(const Item&) = delete;
    Item& operator=(const Item&) = delete;

    static Item ownKeyword(std::unique_ptr<KeywordConstructorRecord>);
    ItemKind kind() const noexcept;
    LegacyItemText* legacyText() noexcept;
    const LegacyItemText* legacyText() const noexcept;
    KeywordConstructorRecord* keyword() noexcept;
    const KeywordConstructorRecord* keyword() const noexcept;

private:
    std::variant<LegacyItemText, std::unique_ptr<KeywordConstructorRecord>> storage_;
};
struct Var;
using ItemSnapshotEntry = std::variant<LegacyItemText, const KeywordConstructorRecord*>;
using ItemSnapshot = std::vector<ItemSnapshotEntry>;
ItemSnapshot snapshotItems(const Var&);
LegacyItemText itemSnapshotText(const ItemSnapshotEntry&);

// B121-only scalar shape recovered from one retained TLOC DoeInputTable row.
// It is a lower-level model projection, not proof of native notification delivery.
struct ScalarKeyword {
    String name;
    String parameterType;
    int unitType = 0;
    int parameter = -1;
    double nominalValue = 0.0;
    String nominalDataText;
    std::vector<int> operationIds;
};

struct ScalarVariable {
    int id = 0;
    int dimension = 0;
    int sampleCount = 0;
    ScalarKeyword keyword;
};

// B122-only geometry metadata projection. File names are references from the
// retained InputTable row; this does not own or create GEO files.
struct GeometryVariable {
    int id = 0;
    int dimension = 0;
    int sampleCount = 0;
    int geoObject = 0;
    int geoOperation = 0;
    int morphMethod = 0;
    bool upperDefined = false;
    bool lowerDefined = false;
    String nominalGeoFile;
    String upperGeoFile;
    String lowerGeoFile;
    bool upperCorrespondencePresent = false;
    bool lowerCorrespondencePresent = false;
};

struct Var {                       // DOE variable
    String name;                   // +0x00 (0x1403047c0)
    int index = 0;                 // +0x18 (0x140010320), set by the manager's re-indexing
    int number = 0;                // +0x1c (0x140100980)
    int geoType = 0;               // +0x20 (0x140817870), 100 = XYR mode
    std::vector<GeoFile> files;    // +0x28
    std::vector<Item> items;       // +0x30 (0x1400d0e60)
    std::vector<int> ops;          // +0x38 operation ids the variable applies to (0x14091dbb0)
    Var* link = nullptr;           // +0x40 (0x1401cc6b0)
    std::optional<ScalarVariable> scalar; // B121 TLOC candidate model-prefix projection
    std::optional<GeometryVariable> geometry; // B122 isolated metadata, no GEO ownership

    // @src 0x14091dbb0  ops.contains(opId) (QList<int>, searched from the end)
    bool appliesTo(int opId) const;
};

// DefDoeModel / SfMoDoeManager (the two share these slots, vtables.tsv). P17 fills vars and
// implements geometryVars (0x14091ba60); the other queries are P09's (src/mst/emit_b_doe.cpp).
// The queries re-index the variables (v->index := position), as the original does.
class Model {
public:
    virtual ~Model() = default;
    std::vector<Var*> vars;        // +0x38 QList<Var*>

    // vt+0x178  @src 0x1400d7f00  vars.size()
    virtual int count() const;
    // vt+0x1a0  @src 0x14091b950  var at index i, null when out of [0, count())
    virtual Var* varAt(int i) const;
    // vt+0x1a8  @src 0x14091b9b0  all vars (re-indexed)
    virtual std::vector<Var*> allVars();
    // vt+0x1b0  @src 0x14091ba60  geometry variables of operation opId whose object is objId (P17)
    virtual std::vector<Var*> geometryVars(int opId, int objId) const = 0;
    // vt+0x1b8  @src 0x14091bbb0  vars that apply to operation opId (re-indexed); g_doeVarList of
    //           the DOE dispatch 0x1401321a0
    virtual std::vector<Var*> varsForOperation(int opId);
    // vt+0x1c0  @src 0x14091bcb0  vars without a link (re-indexed)
    virtual std::vector<Var*> unlinkedVars();
    // vt+0x1c8  @src 0x14091bdb0  link map: var index -> position of its (link's) var among the
    //           unlinked vars, -1 if not there (s1-05 7.3); g_doeObjMap
    virtual std::map<int, int> linkMap();
};

}  // namespace m2m::doe
