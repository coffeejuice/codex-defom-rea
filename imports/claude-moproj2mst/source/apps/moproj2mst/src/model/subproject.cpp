// P03: sub-projects, the flow chart parsed on activation, and the MST operation order
// (s1-01 8; s1-02 4.2; s1-04 2 for the cycle members).
//
// The flow chart model (SfMoDiagEditorModel, interface at model+0x10) is reduced to what the order
// functions read: the item list model+0x20 in creation order, each item's cell, group rectangle and
// group flag, and one connection per connector. Successor (+0x130, 0x14014d540) is the item at the
// other end of the first connection of the item's out connector, predecessor (+0x138,
// 0x14014d620) the same for the in connector. Connecting (+0x90, 0x140075050) first disconnects
// both connectors (+0x88 on each) after admission. During parsing each source output is fresh;
// admission rejects incompatible types and occupied inputs, preserving the first accepted edge.
#include <algorithm>
#include <cmath>
#include <set>
#include <string>

#include "m2m/model.hpp"
#include "m2m/doe_study.hpp"
#include "m2m/doe_activation.hpp"
#include "m2m/mst/writer.hpp"   // complete OpClass for Project's registry
#include "m2m/qt4path.hpp"
#include "../doe/flow_receiver_witness.hpp"
#include "../doe/input_table_reader.hpp"

namespace m2m {
using qt::L;

namespace {

// Group flag 0x1000 of item->vt[8]()->vt[0x58]() (s1-01 8.3): the Cycle and Group diagram items.
// The flag word comes from the item's class (factory 0x14014cea0 via a global, not followed); the
// two classes are the spec's reading, and the corpus cycles confirm SfOprCycle.
bool isGroupClass(const String& cls)
{
    return cls == L("SfOprCycle") || cls == L("SfOprGroup");
}

// @src 0x140084a50  a < b by cell (+0xc8): row, then column
bool lessByCell(const FlowItem* a, const FlowItem* b)
{
    if (a->row < b->row) return true;
    return a->row == b->row && a->col < b->col;
}

// @src 0x140085110  a < b by group begin (+0xd8): row, then column
bool lessByGroupBegin(const FlowItem* a, const FlowItem* b)
{
    if (a->beginRow < b->beginRow) return true;
    return a->beginRow == b->beginRow && a->beginCol < b->beginCol;
}

using LessThan = bool (*)(const FlowItem*, const FlowItem*);

// @src 0x140088f90  Qt 4.8 qSortHelper (not stable): median of three, then partition
void qSortHelper(FlowItem** start, FlowItem** end, LessThan lessThan)
{
top:
    const long long span = end - start;
    if (span < 2) return;
    --end;
    FlowItem** low = start;
    FlowItem** high = end - 1;
    FlowItem** pivot = start + span / 2;
    if (lessThan(*end, *start)) std::swap(*end, *start);
    if (span == 2) return;
    if (lessThan(*pivot, *start)) std::swap(*pivot, *start);
    if (lessThan(*end, *pivot)) std::swap(*end, *pivot);
    if (span == 3) return;
    std::swap(*pivot, *end);
    while (low < high) {
        while (low < high && lessThan(*low, *end)) ++low;
        while (high > low && lessThan(*end, *high)) --high;
        if (low < high) {
            std::swap(*low, *high);
            ++low;
            --high;
        } else {
            break;
        }
    }
    if (lessThan(*low, *end)) ++low;
    std::swap(*end, *low);
    qSortHelper(start, low, lessThan);
    start = low + 1;
    ++end;
    goto top;
}

// @src 0x140088d40  qSort(begin, end, lessThan)
void qSort(std::vector<FlowItem*>& v, LessThan lessThan)
{
    if (!v.empty()) qSortHelper(v.data(), v.data() + v.size(), lessThan);
}

// @src 0x140085190  the group items (flag 0x1000) of model+0x20, sorted by group begin
std::vector<FlowItem*> groupItems(const FlowChart& flow)
{
    std::vector<FlowItem*> out;
    for (const auto& it : flow.items)
        if (it->group) out.push_back(it.get());
    qSort(out, lessByGroupBegin);
    return out;
}

// @src 0x140085470  non-group items whose cell lies in [begin, end] of g (row-major, inclusive),
// sorted by cell
std::vector<FlowItem*> itemsInGroup(const FlowChart& flow, const FlowItem& g)
{
    std::vector<FlowItem*> out;
    for (const auto& it : flow.items) {
        if (it->group) continue;
        const int r = it->row, c = it->col;
        if ((g.beginRow < r || (r == g.beginRow && g.beginCol <= c)) &&
            (r < g.endRow || (r == g.endRow && c <= g.endCol)))
            out.push_back(it.get());
    }
    qSort(out, lessByCell);
    return out;
}

// The original has no guard against a closed connection loop and would spin; the converter stops.
[[noreturn]] void chainLoop()
{
    throw Error(M2M_E_PARSE, "flow chart connections form a loop");
}

}  // namespace

// @src 0x140084ad0  the connector chain: the non-group items sorted by cell, the first one walked
// back with +0x138 to the head, then forward with +0x130. Empty when there is no non-group item.
std::vector<FlowItem*> flowChain(const FlowChart& flow)
{
    std::vector<FlowItem*> chain;
    if (flow.items.empty()) return chain;
    std::vector<FlowItem*> plain;
    for (const auto& it : flow.items)
        if (!it->group) plain.push_back(it.get());
    if (plain.empty()) return chain;
    qSort(plain, lessByCell);
    FlowItem* h = plain[0];
    std::set<const FlowItem*> seen{h};
    for (FlowItem* p = h->prev; p; p = p->prev) {
        if (!seen.insert(p).second) chainLoop();
        h = p;
    }
    seen = {h};
    chain.push_back(h);
    for (FlowItem* n = h->next; n; n = n->next) {
        if (!seen.insert(n).second) chainLoop();
        chain.push_back(n);
    }
    return chain;
}

namespace {

// @src 0x140084f00  first item of model+0x20 whose id (+0x28) equals id, or null
FlowItem* findItemById(const FlowChart& flow, int id)
{
    for (const auto& it : flow.items)
        if (it->opId == id) return it.get();
    return nullptr;
}

// @src 0x140086820  item at the first index of model+0x20 whose id equals id, or null
FlowItem* itemById(const FlowChart& flow, int id)
{
    for (const auto& it : flow.items)
        if (it->opId == id) return it.get();
    return nullptr;
}

// groupOf of 0x1400b6630 and 0x140085800: member id -> group id; later groups overwrite.
std::map<int, int> groupMap(const FlowChart& flow)
{
    std::map<int, int> groupOf;
    for (FlowItem* g : groupItems(flow))
        for (FlowItem* it : itemsInGroup(flow, *g))
            groupOf[it->opId] = g->opId;
    return groupOf;
}

// Model +0x90 (0x140075050) on the first out connector of `from` and the first in connector of
// `to`: +0x88 clears both connectors (the far ends lose that connection too), then +0x78 links
// them. The caller must first apply the model's admission check (+0x88 on the model).
// Object rows can also reconnect object-row ports and emit ordered
// notifications. FlowItem prev/next represent only this main Control2d edge;
// associated profiles requiring row ports remain outside selected routes.
void connectItems(FlowItem* from, FlowItem* to)
{
    if (to->prev) {
        to->prev->next = nullptr;
        to->prev = nullptr;
    }
    if (from->next) {
        from->next->prev = nullptr;
        from->next = nullptr;
    }
    from->next = to;
    to->prev = from;
}

// @src 0x14015ef80: connector names from class info +0x58. The asymmetric
// converter flag takes precedence over the ordinary dimension bits.
String connectorType(Project& p, const FlowItem& item, bool output)
{
    const auto* cls = findClass(p, item.className);
    if (!cls) return L("Control3d");  // constructor's null-info default only
    const auto flags = cls->dimFlags;
    if (flags & 0x10000000) return output ? L("Control3d") : L("Control2d");
    if (!(flags & 2)) return L("Control2d");
    return (flags & 1) ? L("Control2d3d") : L("Control3d");
}

// @src 0x140161470: equality or the combined connector paired with 2D/3D.
bool connectorAccepts(const String& input, const String& output)
{
    return input == output ||
           (input == L("Control2d3d") && (output == L("Control2d") || output == L("Control3d"))) ||
           (output == L("Control2d3d") && (input == L("Control2d") || input == L("Control3d")));
}

// Legacy class names of the flow chart (as 0x140046cc0 maps operation types).
String mapLegacyClass(const String& name)
{
    String cls = name;
    if (qt::contains(name, L("SfOprHeatExpress")) || qt::contains(name, L("SfOprAirConvect")) ||
        qt::contains(name, L("SfOprHeatResting")) || qt::contains(name, L("SfOprHeatDwelling")))
        cls = qt::contains(name, L("2D")) ? L("SfOprHeatTransferExpress2D") : L("SfOprHeatTransferExpress");
    if (name == L("SfOprMultiBlow")) cls = L("SfOprMultiBlow2D");
    return cls;
}

[[noreturn]] void nullItem(const char* tag)
{
    throw Error(M2M_E_PARSE, std::string("DiagramItem: <") + tag + "> before <ClassName>");
}

// @src 0x140093140  Connections: each ItemIndex sets next[k] (the last one wins)
void parseConnections(const xml::Node& e, int k, std::map<int, int>& next)
{
    for (const xml::Node* c : e.childElements())
        if (c->tag == L("ItemIndex")) next[k] = qt::toInt(c->text());
}

// @src 0x140092ff0  Connectors: each Connections child
void parseConnectors(const xml::Node& e, int k, std::map<int, int>& next)
{
    for (const xml::Node* c : e.childElements())
        if (c->tag == L("Connections")) parseConnections(*c, k, next);
}

// @src 0x140091db0  ItemData: every attribute -> item +0xf8(name, value.toInt()). The original
// walks QDomNamedNodeMap (Qt 4 hash order); here document order. Only CycleCount occurs.
void parseItemData(const xml::Node& e, FlowItem& it)
{
    for (const xml::Attr& a : e.attrs) it.itemData.emplace_back(a.name, qt::toInt(a.value));
}

}  // namespace

// @src 0x1400b5470  SUB ctor: own flow chart, type, id. DOE study (+0x20, types 2 and 3) and
// optimisation data (+0x28, type 3) remain unsupported. Type2 owns a manager while study
// contents still load lazily; the constructor's directory failure is ignored.
// SubProject::flow projects the model; constructor witnesses do not certify its
// complete QObject or current diagram backend state.
void Project::bindFlowReceiverWitnessRegistry(
    const std::shared_ptr<doe::FlowReceiverWitnessRegistry>& registry)
{
    if (!registry || !registry->owner())
        throw Error(M2M_E_UNSUPPORTED, "Project flow receiver registry requires an existing notifier owner");
    if (flowReceiverRegistryBound_) {
        const auto current = flowReceiverRegistry_.lock();
        if (!current || current.get() != registry.get() ||
            current.owner_before(registry) || registry.owner_before(current))
            throw Error(M2M_E_UNSUPPORTED, "Project flow receiver registry cannot replace its actual owner");
        return;
    }
    if (flowReceiverConstructionStarted_ || !subs.empty())
        throw Error(M2M_E_UNSUPPORTED, "Project flow receiver registry must bind before SUB construction");
    flowReceiverRegistry_ = registry;
    flowReceiverRegistryBound_ = true;
}

std::shared_ptr<doe::FlowReceiverWitnessRegistry> Project::flowReceiverWitnessRegistry() const
{
    const auto registry = flowReceiverRegistry_.lock();
    if (flowReceiverRegistryBound_ && !registry)
        throw Error(M2M_E_UNSUPPORTED, "Project flow receiver registry owner has expired");
    return registry;
}

std::unique_ptr<SubProject> makeSubProject(Project& p, int type, int id)
{
    auto s = std::make_unique<SubProject>();
    p.flowReceiverConstructionStarted_ = true;
    const auto registry = p.flowReceiverWitnessRegistry();
    if (registry) {
        s->flowReceiverWitness_ = registry->recordNormalSubConstruction(&s->flow);
        s->flowReceiverRegistry_ = registry;
    }
    s->loaded = false;
    s->project = &p;
    if (registry) registry->bindModelContext(s->flowReceiverWitness_, p, *s);
    s->type = type;
    s->id = id;
    s->doe = nullptr;
    if (type == 2) {
        s->ownedDoeStudy = doe::makeStudy(*s, p.doeIo.get());
        s->doe = s->ownedDoeStudy.get();
    }
    s->dbName = String();
    return s;
}

// @src 0x14008f200  sub-projects >= 1 at open: Active is read (unused), Subproject_DbName DbName
void parseSubProjectDbName(Project& p, const xml::Node& e, int idx)
{
    SubProject& sub = *p.subs.at(static_cast<size_t>(idx));
    (void)qt::toInt(e.attribute(L("Active")));
    for (const xml::Node* c : e.childElements())
        if (c->tag == L("Subproject_DbName")) sub.dbName = c->attribute(L("DbName"));   // 0x1400b9f10
}

// @src 0x1400a78e0  idx-th Sub_Project child of the document element (0-based), null if none
const xml::Node* subProjectElement(const Project& p, int idx)
{
    if (!p.dom || !p.dom->root) return nullptr;
    int n = 0;
    for (const xml::Node* c : p.dom->root->childElements()) {
        if (c->tag == L("Sub_Project")) {
            if (idx == n) return c;
            ++n;
        }
    }
    return nullptr;
}

// @src 0x1400a75a0
String subProjectName(const Project& p, int idx)
{
    const xml::Node* e = subProjectElement(p, idx);
    return e ? e->attribute(L("Name")) : String();
}

// @src 0x1400a7250 -> 0x1400a7b90. The rest of both (engine unit 0x140917d60, current operation
// 0x140047af0, view and DOE table signals) is GUI state with no MST effect.
void activateSubProject(Project& p, int idx, Ctx& ctx)
{
    if (idx == -1) idx = p.activeSub;
    const xml::Node* e = subProjectElement(p, idx);
    p.activeSub = idx;
    if (idx < 0 || idx >= static_cast<int>(p.subs.size()))
        throw Error(M2M_E_PARSE, "sub-project " + std::to_string(idx) + " does not exist");
    SubProject& sub = *p.subs[static_cast<size_t>(idx)];
    if (!sub.loaded) {
        p.loadingSub = true;
        if (e) parseSubProject(p, *e, ctx);       // a null QDomElement has no children
        sub.loaded = true;
        p.loadingSub = false;
    }
    // the main window's view update after the activation, 0x140014260 -> 0x140015bf0 (current
    // operation 0x140047af0, then 0x1400bc580 -> 0x140087b10 on the sub-project's model)
    refreshDiagramItems(p);
}

void placeCelllessItems(Project& project, FlowChart& flow, const xml::Node& e, bool initiallyEmpty);

// @src 0x14008f530  ParseSubProject into the active sub-project
void parseSubProject(Project& p, const xml::Node& e, Ctx& ctx)
{
    SubProject& sub = *p.subs.at(static_cast<size_t>(p.activeSub));
    FlowChart& flow = sub.flow;
    const bool initiallyEmpty = flow.items.empty();
    const int active = qt::toInt(e.attribute(L("Active")));
    std::vector<FlowItem*> items;
    std::map<int, int> next;
    int k = 0;
    for (const xml::Node* c : e.childElements()) {
        if (c->tag == L("DiagramItem")) {
            next[k] = -1;
            FlowItem* it = parseDiagramItem(p, flow, *c, k, next);
            if (k == active) {
                if (!it) nullItem("Active");
                flow.activeItem = k;              // model +0xf8(item id): the GUI's current item
            }
            items.push_back(it);
            ++k;
        } else if (c->tag == L("SfDoeStudy") && sub.type == 2) {
            if (!sub.ownedDoeStudy || sub.doe != sub.ownedDoeStudy.get() || !p.doeIo)
                throw Error(M2M_E_UNSUPPORTED, "DOE lazy parsing requires owned study and configured I/O");
            const doe::InputTableNotifierContext notifierContext{
                ctx.session, &p, &sub, sub.ownedDoeStudy.get(), p.doeIo.get(), p.doeIo};
            doe::readLazyStudy(*sub.ownedDoeStudy, *c,
                              projectDir(p) + qt::kSep + L("DoeStudies"), *p.doeIo,
                              ctx.session ? &notifierContext : nullptr);
        } else if (c->tag == L("SfDoeStudy") && sub.type == 3) {
            throw Error(M2M_E_UNSUPPORTED, "optimisation sub-project study");
        } else if (c->tag == L("Subproject_DbName")) {
            sub.dbName = c->attribute(L("DbName"));   // 0x1400b9f10
        }
    }
    for (int j = 0; j < k; ++j) {
        const int t = next[j];
        if (t >= 0 && t < k && items[static_cast<size_t>(j)] && items[static_cast<size_t>(t)]) {
            FlowItem* from = items[static_cast<size_t>(j)];
            FlowItem* to = items[static_cast<size_t>(t)];
            // @src 0x140074f50: check types before link occupancy. This loop
            // attempts each newly created source output exactly once (sub0.c
            // 925-948), so the original both-linked owner-index branch cannot
            // apply here. This is not the general interactive reconnect rule.
            if (connectorAccepts(connectorType(p, *to, false), connectorType(p, *from, true)) &&
                !to->prev)
                connectItems(from, to);
        }
    }
    placeCelllessItems(p, flow, e, initiallyEmpty);
}

// @src 0x1401574f0 -> 0x14015a070 -> 0x140158dc0  the cell editor takes the model's items in order;
// an item with either native cell coordinate -1 (including a file without CellPos) gets the cell under its
// rectangle's top-left (x < -60 -> 0, y < -25 -> 0; 0x14014e210: column 0x14014db20 = round half
// up of x / 205, row 0x14014dfa0 from the row list), which the editor stores back in the item.
void placeCelllessItems(Project& project, FlowChart& flow, const xml::Node& e, bool initiallyEmpty)
{
    struct CellInput { bool missing = true; bool selected = true; int row = -1; int col = -1; };
    std::vector<CellInput> cellInputs;
    for (const xml::Node* c : e.childElements()) {
        if (c->tag != L("DiagramItem")) continue;
        bool given = false;
        int inputRow = -1, inputCol = -1;
        for (const xml::Node* g : c->childElements())
            if (g->tag == L("CellPos")) {
                given = true;
                inputRow = qt::toInt(g->attribute(L("Row")));
                inputCol = qt::toInt(g->attribute(L("Col")));
            }
        cellInputs.push_back({!given, !given || (inputRow == -1 && inputCol == -1), inputRow, inputCol});
    }
    std::set<std::pair<int, int>> used;
    bool row0 = false;
    // Snapshot the parsed graph and eligibility before any rectangle or link mutation.
    const bool initiallyUnlinked = std::all_of(flow.items.begin(), flow.items.end(), [](const auto& item) {
        return item && item->next == nullptr && item->prev == nullptr;
    });
    const bool singletonRelayoutCandidate = initiallyEmpty && flow.items.size() == 1 &&
        cellInputs.size() == 1 &&
        (cellInputs[0].missing || cellInputs[0].row == -1 || cellInputs[0].col == -1) && initiallyUnlinked && flow.items[0] &&
        !flow.items[0]->group && flow.items[0]->className == L("SfOprForming2D") &&
        std::isfinite(flow.items[0]->rect[0]) && std::isfinite(flow.items[0]->rect[1]) &&
        std::isfinite(flow.items[0]->rect[2]) && std::isfinite(flow.items[0]->rect[3]) &&
        flow.items[0]->rect[0] < 1332.5 && flow.items[0]->rect[1] < 67.5;
    const bool singletonExplicitCellCandidate = initiallyEmpty && flow.items.size() == 1 &&
        cellInputs.size() == 1 && !cellInputs[0].missing && cellInputs[0].row == 0 &&
        cellInputs[0].col >= 0 && cellInputs[0].col <= 6 && initiallyUnlinked && flow.items[0] &&
        !flow.items[0]->group && flow.items[0]->className == L("SfOprForming2D") &&
        std::isfinite(flow.items[0]->rect[0]) && std::isfinite(flow.items[0]->rect[1]) &&
        std::isfinite(flow.items[0]->rect[2]) && std::isfinite(flow.items[0]->rect[3]) &&
        flow.items[0]->rect[0] < 1332.5 && flow.items[0]->rect[1] < 67.5;
    const bool explicitPairCandidate = initiallyEmpty && initiallyUnlinked && flow.items.size() == 2 &&
        cellInputs.size() == 2 && flow.items[0] && flow.items[1] &&
        !flow.items[0]->group && !flow.items[1]->group &&
        flow.items[0]->className == L("SfOprForming2D") && flow.items[1]->className == L("SfOprForming2D") &&
        flow.items[0]->rect[0] == 0.0 && flow.items[0]->rect[1] == 0.0 &&
        std::isfinite(flow.items[0]->rect[2]) && std::isfinite(flow.items[0]->rect[3]) &&
        std::isfinite(flow.items[1]->rect[0]) && std::isfinite(flow.items[1]->rect[1]) &&
        std::isfinite(flow.items[1]->rect[2]) && std::isfinite(flow.items[1]->rect[3]) &&
        flow.items[1]->rect[0] < 1332.5 && flow.items[1]->rect[1] < 67.5 &&
        !cellInputs[0].missing && !cellInputs[1].missing &&
        cellInputs[0].row == 0 && cellInputs[1].row == 0 &&
        cellInputs[0].col >= 0 && cellInputs[0].col <= 6 &&
        cellInputs[1].col >= 0 && cellInputs[1].col <= 6 &&
        cellInputs[0].col != cellInputs[1].col;
    const auto validExplicitCell = [](const CellInput& input) {
        return !input.missing && input.row == 0 && input.col >= 0 && input.col <= 6;
    };
    const auto sentinelCell = [](const CellInput& input) {
        return input.missing || input.row == -1 || input.col == -1;
    };
    const bool mixedCellForms = cellInputs.size() == 2 &&
        ((validExplicitCell(cellInputs[0]) && sentinelCell(cellInputs[1])) ||
         (sentinelCell(cellInputs[0]) && validExplicitCell(cellInputs[1])));
    int mixedACol = -1, mixedBCol = -1;
    if (mixedCellForms && flow.items.size() == 2 && flow.items[1] &&
        std::isfinite(flow.items[1]->rect[0]) && flow.items[1]->rect[0] < 1332.5) {
        mixedACol = validExplicitCell(cellInputs[0]) ? cellInputs[0].col : 0;
        double x = flow.items[1]->rect[0];
        if (x < -60.0) x = 0.0;
        const double cx = x / 205.0;
        int derivedCol = static_cast<int>(std::floor(cx));
        if (cx - std::floor(cx) >= 0.5) derivedCol = static_cast<int>(std::ceil(cx));
        mixedBCol = validExplicitCell(cellInputs[1]) ? cellInputs[1].col : derivedCol;
    }
    const bool mixedPairCandidate = initiallyEmpty && initiallyUnlinked && flow.items.size() == 2 &&
        mixedCellForms && flow.items[0] && flow.items[1] &&
        !flow.items[0]->group && !flow.items[1]->group &&
        flow.items[0]->className == L("SfOprForming2D") && flow.items[1]->className == L("SfOprForming2D") &&
        flow.items[0]->rect[0] == 0.0 && flow.items[0]->rect[1] == 0.0 &&
        std::isfinite(flow.items[0]->rect[2]) && std::isfinite(flow.items[0]->rect[3]) &&
        std::isfinite(flow.items[1]->rect[0]) && std::isfinite(flow.items[1]->rect[1]) &&
        std::isfinite(flow.items[1]->rect[2]) && std::isfinite(flow.items[1]->rect[3]) &&
        flow.items[1]->rect[0] < 1332.5 && flow.items[1]->rect[1] < 67.5 &&
        mixedACol >= 0 && mixedACol <= 6 && mixedBCol >= 0 && mixedBCol <= 6 &&
        mixedACol != mixedBCol;
    const bool initialForwardPair = flow.items.size() == 2 && flow.items[0] && flow.items[1] &&
        flow.items[0]->prev == nullptr && flow.items[0]->next == flow.items[1].get() &&
        flow.items[1]->prev == flow.items[0].get() && flow.items[1]->next == nullptr;
    const bool recurrenceCandidate = initiallyEmpty && flow.items.size() >= 2 && flow.items.size() <= 7 &&
        cellInputs.size() == flow.items.size() &&
        std::all_of(flow.items.begin(), flow.items.end(), [](const auto& item) {
            return item && !item->group && item->className == L("SfOprForming2D") &&
                   item->rect[0] == 0.0 && item->rect[1] == 0.0 &&
                   item->rect[2] == 0.0 && item->rect[3] == 0.0;
        }) && std::all_of(cellInputs.begin(), cellInputs.end(), [](const CellInput& input) {
            return input.selected;
        }) && (initiallyUnlinked || initialForwardPair);
    // Above-common-height routes stay unbound until associated object-row
    // connector effects have their own public contract.
    const bool allItemsUnbound = std::all_of(flow.items.begin(), flow.items.end(),
        [&project](const auto& item) {
            return item && operationById(project, item->opId) == nullptr;
        });
    // Negative operation IDs expand the first item's graphic to height 190.
    // Rebuild therefore establishes row height 215 before the second item.
    const bool negativePairGraphicsHeight = flow.items.size() == 2 &&
        flow.items[0] && flow.items[1] &&
        flow.items[0]->opId < 0 && allItemsUnbound;
    // With no associated project Operation, an ordinary positive-ID Forming2D
    // graphic's expanded template supplies a conservative height of 189.
    const bool positivePairGraphicsHeight = flow.items.size() == 2 &&
        flow.items[0] && flow.items[1] && flow.items[0]->opId >= 0 &&
        allItemsUnbound;
    // CP495-497: source-qualified finite positive-left intervals select free columns
    // either by collision insertion or directly in a free column. Keep the
    // original geometry snapshot before B moves.
    const bool nonzeroPairCandidate = flow.items.size() == 2 && cellInputs.size() == 2 &&
        std::all_of(cellInputs.begin(), cellInputs.end(), [](const CellInput& input) {
            return input.missing || input.row == -1 || input.col == -1;
        }) &&
        initiallyEmpty && initiallyUnlinked && flow.items[0] && flow.items[1] &&
        !flow.items[0]->group && !flow.items[1]->group &&
        flow.items[0]->className == L("SfOprForming2D") && flow.items[1]->className == L("SfOprForming2D") &&
        flow.items[0]->rect[0] == 0.0 && flow.items[0]->rect[1] == 0.0 &&
        std::isfinite(flow.items[0]->rect[2]) && std::isfinite(flow.items[0]->rect[3]) &&
        std::isfinite(flow.items[1]->rect[0]) && flow.items[1]->rect[0] > 0.0 && flow.items[1]->rect[0] < 1332.5 &&
        std::isfinite(flow.items[1]->rect[1]) &&
        flow.items[1]->rect[1] < (negativePairGraphicsHeight ? 190.0 :
                                  positivePairGraphicsHeight ? 189.0 : 160.0) &&
        std::isfinite(flow.items[1]->rect[2]) && std::isfinite(flow.items[1]->rect[3]);
    const bool freeColumnPairCandidate = initiallyEmpty && initiallyUnlinked && cellInputs.size() == 2 &&
        flow.items.size() == 2 && nonzeroPairCandidate && flow.items[1]->rect[0] >= 102.5;
    const bool threeCollisionCandidate = initiallyEmpty && initiallyUnlinked &&
        flow.items.size() == 3 && cellInputs.size() == 3 &&
        std::all_of(cellInputs.begin(), cellInputs.end(), sentinelCell) &&
        std::all_of(flow.items.begin(), flow.items.end(), [](const auto& item) {
            return item && !item->group && item->className == L("SfOprForming2D");
        }) &&
        std::all_of(flow.items.begin(), flow.items.begin() + 2, [](const auto& item) {
            return item->rect[0] == 0.0 && item->rect[1] == 0.0 &&
                item->rect[2] == 0.0 && item->rect[3] == 0.0;
        }) &&
        std::isfinite(flow.items[2]->rect[0]) && flow.items[2]->rect[0] > 0.0 &&
        flow.items[2]->rect[0] < 1332.5 &&
        std::isfinite(flow.items[2]->rect[1]) && flow.items[2]->rect[1] < 160.0 &&
        std::isfinite(flow.items[2]->rect[2]) && std::isfinite(flow.items[2]->rect[3]);
    const bool threeNegativeCandidate = initiallyEmpty && initiallyUnlinked &&
        flow.items.size() == 3 && cellInputs.size() == 3 &&
        std::all_of(cellInputs.begin(), cellInputs.end(), sentinelCell) &&
        std::all_of(flow.items.begin(), flow.items.end(), [](const auto& item) {
            return item && !item->group && item->className == L("SfOprForming2D");
        }) &&
        std::all_of(flow.items.begin(), flow.items.begin() + 2, [](const auto& item) {
            return item->rect[0] == 0.0 && item->rect[1] == 0.0 &&
                item->rect[2] == 0.0 && item->rect[3] == 0.0;
        }) &&
        std::isfinite(flow.items[2]->rect[0]) && flow.items[2]->rect[0] < 0.0 &&
        std::isfinite(flow.items[2]->rect[1]) && flow.items[2]->rect[1] < 160.0 &&
        std::isfinite(flow.items[2]->rect[2]) && std::isfinite(flow.items[2]->rect[3]);
    const bool negativeMultiCandidate = initiallyEmpty && initiallyUnlinked &&
        flow.items.size() >= 4 && flow.items.size() <= 7 &&
        cellInputs.size() == flow.items.size() &&
        std::all_of(cellInputs.begin(), cellInputs.end(), sentinelCell) &&
        std::all_of(flow.items.begin(), flow.items.end(), [](const auto& item) {
            return item && !item->group && item->className == L("SfOprForming2D");
        }) &&
        std::all_of(flow.items.begin(), flow.items.end() - 1, [](const auto& item) {
            return item->rect[0] == 0.0 && item->rect[1] == 0.0 &&
                item->rect[2] == 0.0 && item->rect[3] == 0.0;
        }) &&
        std::isfinite(flow.items.back()->rect[0]) && std::isfinite(flow.items.back()->rect[1]) &&
        std::isfinite(flow.items.back()->rect[2]) && std::isfinite(flow.items.back()->rect[3]) &&
        flow.items.back()->rect[0] < 0.0 && flow.items.back()->rect[1] < 160.0;
    const bool fourPositiveCandidate = initiallyEmpty && initiallyUnlinked &&
        flow.items.size() == 4 && cellInputs.size() == 4 &&
        std::all_of(cellInputs.begin(), cellInputs.end(), sentinelCell) &&
        std::all_of(flow.items.begin(), flow.items.end(), [](const auto& item) {
            return item && !item->group && item->className == L("SfOprForming2D");
        }) &&
        std::all_of(flow.items.begin(), flow.items.begin() + 3, [](const auto& item) {
            return item->rect[0] == 0.0 && item->rect[1] == 0.0 &&
                item->rect[2] == 0.0 && item->rect[3] == 0.0;
        }) &&
        std::isfinite(flow.items[3]->rect[0]) && flow.items[3]->rect[0] > 0.0 &&
        flow.items[3]->rect[0] < 1332.5 &&
        std::isfinite(flow.items[3]->rect[1]) && flow.items[3]->rect[1] < 160.0 &&
        std::isfinite(flow.items[3]->rect[2]) && std::isfinite(flow.items[3]->rect[3]);
    const bool fivePositiveCandidate = initiallyEmpty && initiallyUnlinked &&
        flow.items.size() == 5 && cellInputs.size() == 5 &&
        std::all_of(cellInputs.begin(), cellInputs.end(), sentinelCell) &&
        std::all_of(flow.items.begin(), flow.items.end(), [](const auto& item) {
            return item && !item->group && item->className == L("SfOprForming2D");
        }) &&
        std::all_of(flow.items.begin(), flow.items.begin() + 4, [](const auto& item) {
            return item->rect[0] == 0.0 && item->rect[1] == 0.0 &&
                item->rect[2] == 0.0 && item->rect[3] == 0.0;
        }) &&
        std::isfinite(flow.items[4]->rect[0]) && flow.items[4]->rect[0] > 0.0 &&
        flow.items[4]->rect[0] < 1332.5 &&
        std::isfinite(flow.items[4]->rect[1]) && flow.items[4]->rect[1] < 160.0 &&
        std::isfinite(flow.items[4]->rect[2]) && std::isfinite(flow.items[4]->rect[3]);
    const bool sixPositiveCandidate = initiallyEmpty && initiallyUnlinked &&
        flow.items.size() == 6 && cellInputs.size() == 6 &&
        std::all_of(cellInputs.begin(), cellInputs.end(), sentinelCell) &&
        std::all_of(flow.items.begin(), flow.items.end(), [](const auto& item) {
            return item && !item->group && item->className == L("SfOprForming2D");
        }) &&
        std::all_of(flow.items.begin(), flow.items.begin() + 5, [](const auto& item) {
            return item->rect[0] == 0.0 && item->rect[1] == 0.0 &&
                item->rect[2] == 0.0 && item->rect[3] == 0.0;
        }) &&
        std::isfinite(flow.items[5]->rect[0]) && flow.items[5]->rect[0] > 0.0 &&
        flow.items[5]->rect[0] < 1332.5 &&
        std::isfinite(flow.items[5]->rect[1]) && flow.items[5]->rect[1] < 160.0 &&
        std::isfinite(flow.items[5]->rect[2]) && std::isfinite(flow.items[5]->rect[3]);
    const bool sevenPositiveCandidate = initiallyEmpty && initiallyUnlinked &&
        flow.items.size() == 7 && cellInputs.size() == 7 &&
        std::all_of(cellInputs.begin(), cellInputs.end(), sentinelCell) &&
        std::all_of(flow.items.begin(), flow.items.end(), [](const auto& item) {
            return item && !item->group && item->className == L("SfOprForming2D");
        }) &&
        std::all_of(flow.items.begin(), flow.items.begin() + 6, [](const auto& item) {
            return item->rect[0] == 0.0 && item->rect[1] == 0.0 &&
                item->rect[2] == 0.0 && item->rect[3] == 0.0;
        }) &&
        std::isfinite(flow.items[6]->rect[0]) && flow.items[6]->rect[0] > 0.0 &&
        flow.items[6]->rect[0] < 1332.5 &&
        std::isfinite(flow.items[6]->rect[1]) && flow.items[6]->rect[1] < 160.0 &&
        std::isfinite(flow.items[6]->rect[2]) && std::isfinite(flow.items[6]->rect[3]);
    bool threeDirectCandidate = initiallyEmpty && initiallyUnlinked &&
        flow.items.size() == 3 && cellInputs.size() == 3;
    if (threeDirectCandidate) {
        std::set<int> targets;
        for (std::size_t i = 0; i < 3; ++i) {
            const auto& item = flow.items[i];
            const CellInput& input = cellInputs[i];
            if (!item || item->group || item->className != L("SfOprForming2D") ||
                !std::isfinite(item->rect[0]) || !std::isfinite(item->rect[1]) ||
                !std::isfinite(item->rect[2]) || !std::isfinite(item->rect[3]) ||
                item->rect[0] >= 1332.5 || item->rect[1] >= 67.5 ||
                !(validExplicitCell(input) || sentinelCell(input))) {
                threeDirectCandidate = false;
                break;
            }
            int col = input.col;
            if (sentinelCell(input)) {
                double x = item->rect[0];
                if (x < -60.0) x = 0.0;
                const double cx = x / 205.0;
                col = static_cast<int>(std::floor(cx));
                if (cx - std::floor(cx) >= 0.5) col = static_cast<int>(std::ceil(cx));
            }
            if (col < 0 || col >= 7 || !targets.insert(col).second) {
                threeDirectCandidate = false;
                break;
            }
        }
    }
    // CP510: every requested token is empty when four to seven distinct
    // row-zero targets are installed. The first item sees the empty row table;
    // subsequent items see a populated row with a minimum height of 185.
    bool multiDirectCandidate = initiallyEmpty && initiallyUnlinked &&
        flow.items.size() >= 4 && flow.items.size() <= 7 &&
        cellInputs.size() == flow.items.size();
    if (multiDirectCandidate) {
        std::set<int> targets;
        for (std::size_t i = 0; i < flow.items.size(); ++i) {
            const auto& item = flow.items[i];
            const CellInput& input = cellInputs[i];
            if (!item || item->group || item->className != L("SfOprForming2D") ||
                !std::isfinite(item->rect[0]) || !std::isfinite(item->rect[1]) ||
                !std::isfinite(item->rect[2]) || !std::isfinite(item->rect[3]) ||
                item->rect[0] >= 1332.5 || item->rect[1] >= (i == 0 ? 67.5 : 160.0) ||
                !(validExplicitCell(input) || sentinelCell(input))) {
                multiDirectCandidate = false;
                break;
            }
            int col = input.col;
            if (sentinelCell(input)) {
                double x = item->rect[0];
                if (x < -60.0) x = 0.0;
                const double cx = x / 205.0;
                col = static_cast<int>(std::floor(cx));
                if (cx - std::floor(cx) >= 0.5) col = static_cast<int>(std::ceil(cx));
            }
            if (col < 0 || col >= 7 || !targets.insert(col).second) {
                multiDirectCandidate = false;
                break;
            }
        }
    }
    // CP520: the first negative-ID graphic expands row0 to height 215.
    // Distinct targets avoid every direction-dependent occupied-token path.
    bool negativeTallDirectCandidate = initiallyEmpty && initiallyUnlinked &&
        flow.items.size() >= 2 && flow.items.size() <= 7 &&
        cellInputs.size() == flow.items.size() && flow.items[0] &&
        flow.items[0]->opId < 0 && allItemsUnbound &&
        std::all_of(flow.items.begin(), flow.items.end(), [](const auto& item) {
            return item && !item->group &&
                item->className == L("SfOprForming2D");
        });
    if (negativeTallDirectCandidate) {
        std::set<int> targets;
        for (std::size_t i = 0; i < flow.items.size(); ++i) {
            const auto& item = flow.items[i];
            const CellInput& input = cellInputs[i];
            if (!std::isfinite(item->rect[0]) || !std::isfinite(item->rect[1]) ||
                !std::isfinite(item->rect[2]) || !std::isfinite(item->rect[3]) ||
                item->rect[0] >= 1332.5 || item->rect[1] >= (i == 0 ? 67.5 : 190.0) ||
                !(validExplicitCell(input) || sentinelCell(input))) {
                negativeTallDirectCandidate = false;
                break;
            }
            int col = input.col;
            if (sentinelCell(input)) {
                double x = item->rect[0];
                if (x < -60.0) x = 0.0;
                const double cx = x / 205.0;
                col = static_cast<int>(std::floor(cx));
                if (cx - std::floor(cx) >= 0.5) col = static_cast<int>(std::ceil(cx));
            }
            if (col < 0 || col >= 7 || !targets.insert(col).second) {
                negativeTallDirectCandidate = false;
                break;
            }
        }
    }
    // CP523-526: the first nonnegative-ID Forming2D template proves graphics
    // height at least 189. A matching first Operation with only Undefined
    // slots and no restart retains this conservative lower bound. Later items
    // stay unbound to avoid
    // association-dependent object-row graph effects.
    bool positiveTallDirectCandidate = initiallyEmpty && initiallyUnlinked &&
        flow.items.size() >= 2 && flow.items.size() <= 7 &&
        cellInputs.size() == flow.items.size() && flow.items[0] &&
        flow.items[0]->opId >= 0 &&
        std::all_of(flow.items.begin(), flow.items.end(), [](const auto& item) {
            return item && !item->group && item->className == L("SfOprForming2D");
        });
    if (positiveTallDirectCandidate) {
        Operation* firstOp = operationById(project, flow.items[0]->opId);
        positiveTallDirectCandidate = !firstOp ||
            (firstOp->restartGiven == 0 &&
             std::all_of(firstOp->objIds.begin(), firstOp->objIds.end(),
                         [](int id) { return id == -1; }));
        for (std::size_t i = 1; i < flow.items.size() && positiveTallDirectCandidate; ++i)
            positiveTallDirectCandidate = operationById(project, flow.items[i]->opId) == nullptr;
    }
    if (positiveTallDirectCandidate) {
        std::set<int> targets;
        for (std::size_t i = 0; i < flow.items.size(); ++i) {
            const auto& item = flow.items[i];
            const CellInput& input = cellInputs[i];
            if (!std::isfinite(item->rect[0]) || !std::isfinite(item->rect[1]) ||
                !std::isfinite(item->rect[2]) || !std::isfinite(item->rect[3]) ||
                item->rect[0] >= 1332.5 || item->rect[1] >= (i == 0 ? 67.5 : 189.0) ||
                !(validExplicitCell(input) || sentinelCell(input))) {
                positiveTallDirectCandidate = false;
                break;
            }
            int col = input.col;
            if (sentinelCell(input)) {
                double x = item->rect[0];
                if (x < -60.0) x = 0.0;
                const double cx = x / 205.0;
                col = static_cast<int>(std::floor(cx));
                if (cx - std::floor(cx) >= 0.5) col = static_cast<int>(std::ceil(cx));
            }
            if (col < 0 || col >= 7 || !targets.insert(col).second) {
                positiveTallDirectCandidate = false;
                break;
            }
        }
    }
    const bool negativeEightCandidate = initiallyEmpty && flow.items.size() == 8 &&
        cellInputs.size() == 8 &&
        std::all_of(flow.items.begin(), flow.items.end(), [](const auto& item) {
            return item && !item->group && item->className == L("SfOprForming2D") &&
                   item->rect[0] == 0.0 && item->rect[1] == 0.0 && item->rect[2] == 0.0 && item->rect[3] == 0.0 &&
                   item->opId < 0;
        }) && initiallyEmpty && initiallyUnlinked &&
        std::all_of(cellInputs.begin(), cellInputs.end(), [](const CellInput& input) { return input.selected; });
    // Native graph relayout can reconnect separate object-row
    // ports after the main Control2d edge. This model represents only the
    // latter; do not claim a selected multi-item graph result with such rows.
    const bool selectedMultiItemGraph = flow.items.size() >= 2 &&
        (recurrenceCandidate || nonzeroPairCandidate || freeColumnPairCandidate ||
         explicitPairCandidate || mixedPairCandidate || threeCollisionCandidate ||
         threeNegativeCandidate || fourPositiveCandidate || fivePositiveCandidate ||
         sixPositiveCandidate || sevenPositiveCandidate || negativeMultiCandidate ||
         threeDirectCandidate || multiDirectCandidate || negativeTallDirectCandidate ||
         positiveTallDirectCandidate || negativeEightCandidate);
    if (selectedMultiItemGraph) {
        const bool hasObjectPortRow = std::any_of(flow.items.begin(), flow.items.end(),
            [&project](const auto& item) {
                if (!item) return false;
                const Operation* op = operationById(project, item->opId);
                return op && std::any_of(op->objIds.begin(), op->objIds.end(),
                    [](int id) { return id != -1; });
            });
        if (hasObjectPortRow)
            throw Error(M2M_E_UNSUPPORTED,
                        "project file: associated object-row ports require graph recovery");
    }
    for (std::size_t k = 0; k < flow.items.size() && k < cellInputs.size(); ++k) {
        FlowItem& it = *flow.items[k];
        if (it.group) continue;
        if (cellInputs[k].missing || it.row == -1 || it.col == -1) {
            double x = it.rect[0], y = it.rect[1];
            if (x < -60.0) x = 0;
            if (y < -25.0) y = 0;
            const double cx = x / 205.0;
            int col = static_cast<int>(std::floor(cx));
            if (cx - std::floor(cx) >= 0.5) col = static_cast<int>(std::ceil(cx));
            // row 0: the empty row list gives round((y + 25) / 185), a row 0 present y - 0 < h - 25
            const bool inRow0 = row0 ?
                y < (((negativePairGraphicsHeight && nonzeroPairCandidate) ||
                      negativeTallDirectCandidate) ? 190.0 :
                     ((positivePairGraphicsHeight && nonzeroPairCandidate) ||
                      positiveTallDirectCandidate) ? 189.0 : 160.0) :
                (y + 25.0) / 185.0 < 0.5;
            if (!inRow0 || col < 0 || col >= 7)
                throw Error(M2M_E_UNSUPPORTED, "project file: diagram item without CellPos outside row 0 / the 7 columns "
                                               "(0x14014dfa0 row list, 0x1401583e0 relayout)");
            if (used.count({0, col})) {
                if (negativeEightCandidate && k == 7 && col == 0 && flow.items[0]->row == 0 && flow.items[0]->col == 0) {
                    flow.items[1]->row = 1; flow.items[1]->col = 0;
                    flow.items[1]->rect[0] = 0.0; flow.items[1]->rect[1] = 215.0;
                    for (std::size_t prior = 2; prior < k; ++prior) {
                        flow.items[prior]->row = 0;
                        flow.items[prior]->col = static_cast<int>(8 - prior);
                        flow.items[prior]->rect[0] = 205.0 * flow.items[prior]->col;
                        flow.items[prior]->rect[1] = 0.0;
                    }
                    it.row = 0; it.col = 1; it.rect[0] = 205.0; it.rect[1] = 0.0;
                    used.clear();
                    for (const auto& placed : flow.items) used.insert({placed->row, placed->col});
                    row0 = true;
                    continue;
                }
                // The selected row-0 recurrence shifts an occupied token run
                // to the adjacent free cells; other collision patterns stay guarded.
                if (negativeMultiCandidate && k + 1 == flow.items.size() && row0 &&
                    col == 0 && cx < 0.0 &&
                    flow.items[0]->row == 0 && flow.items[0]->col == 0) {
                    // Negative direction inserts before A and shifts the full
                    // occupied prefix. Underflow to -0 follows the +1 branch.
                    for (std::size_t prior = 0; prior < k; ++prior) {
                        ++flow.items[prior]->col;
                        flow.items[prior]->rect[0] = 205.0 * flow.items[prior]->col;
                    }
                    it.row = 0; it.col = 0;
                    it.rect[0] = 0.0; it.rect[1] = 0.0;
                } else if (sevenPositiveCandidate && k == 6 && row0 && col >= 0 && col <= 5 &&
                    flow.items[0]->col == 0 && flow.items[5]->col == 1 &&
                    flow.items[4]->col == 2 && flow.items[3]->col == 3 &&
                    flow.items[2]->col == 4 && flow.items[1]->col == 5) {
                    const double left = it.rect[0];
                    const int insertion = left < 205.0 ? 1 : left < 410.0 ? 2 :
                        left < 615.0 ? 3 : left < 820.0 ? 4 :
                        left < 1025.0 ? 5 : 6;
                    for (std::size_t prior = 1; prior < 6; ++prior) {
                        if (flow.items[prior]->col < insertion) continue;
                        ++flow.items[prior]->col;
                        flow.items[prior]->rect[0] = 205.0 * flow.items[prior]->col;
                    }
                    it.row = 0; it.col = insertion;
                    it.rect[0] = 205.0 * insertion;
                    it.rect[1] = 0.0;
                } else if (sixPositiveCandidate && k == 5 && row0 && col >= 1 && col <= 4 &&
                    flow.items[0]->col == 0 && flow.items[4]->col == 1 &&
                    flow.items[3]->col == 2 && flow.items[2]->col == 3 &&
                    flow.items[1]->col == 4) {
                    if (it.rect[0] < 205.0) {
                        flow.items[4]->col = 2; flow.items[4]->rect[0] = 410.0;
                        flow.items[3]->col = 3; flow.items[3]->rect[0] = 615.0;
                        flow.items[2]->col = 4; flow.items[2]->rect[0] = 820.0;
                        flow.items[1]->col = 5; flow.items[1]->rect[0] = 1025.0;
                        it.col = 1;
                    } else if (it.rect[0] < 410.0) {
                        flow.items[3]->col = 3; flow.items[3]->rect[0] = 615.0;
                        flow.items[2]->col = 4; flow.items[2]->rect[0] = 820.0;
                        flow.items[1]->col = 5; flow.items[1]->rect[0] = 1025.0;
                        it.col = 2;
                    } else if (it.rect[0] < 615.0) {
                        flow.items[2]->col = 4; flow.items[2]->rect[0] = 820.0;
                        flow.items[1]->col = 5; flow.items[1]->rect[0] = 1025.0;
                        it.col = 3;
                    } else if (it.rect[0] < 820.0) {
                        flow.items[1]->col = 5; flow.items[1]->rect[0] = 1025.0;
                        it.col = 4;
                    } else {
                        it.col = 5;
                    }
                    it.row = 0;
                    it.rect[0] = 205.0 * it.col;
                    it.rect[1] = 0.0;
                } else if (fivePositiveCandidate && k == 4 && row0 && col >= 1 && col <= 3 &&
                    flow.items[0]->col == 0 && flow.items[3]->col == 1 &&
                    flow.items[2]->col == 2 && flow.items[1]->col == 3) {
                    // Native direction selects insertion before the occupied
                    // suffix or advances to the next free token at 205/410/615.
                    if (it.rect[0] < 205.0) {
                        flow.items[3]->col = 2; flow.items[3]->rect[0] = 410.0;
                        flow.items[2]->col = 3; flow.items[2]->rect[0] = 615.0;
                        flow.items[1]->col = 4; flow.items[1]->rect[0] = 820.0;
                        it.col = 1;
                    } else if (it.rect[0] < 410.0) {
                        flow.items[2]->col = 3; flow.items[2]->rect[0] = 615.0;
                        flow.items[1]->col = 4; flow.items[1]->rect[0] = 820.0;
                        it.col = 2;
                    } else if (it.rect[0] < 615.0) {
                        flow.items[1]->col = 4; flow.items[1]->rect[0] = 820.0;
                        it.col = 3;
                    } else {
                        it.col = 4;
                    }
                    it.row = 0;
                    it.rect[0] = 205.0 * it.col;
                    it.rect[1] = 0.0;
                } else if (fourPositiveCandidate && k == 3 && row0 && (col == 1 || col == 2) &&
                    flow.items[0]->col == 0 && flow.items[2]->col == 1 &&
                    flow.items[1]->col == 2) {
                    // Native installer direction changes at x=205 and x=410.
                    // Before each boundary it shifts the occupied suffix;
                    // afterwards it advances to the next free token.
                    const bool insertAtOne = it.rect[0] < 205.0;
                    const bool insertAtTwo = it.rect[0] < 410.0;
                    if (insertAtOne) {
                        flow.items[2]->col = 2; flow.items[2]->rect[0] = 410.0;
                        flow.items[1]->col = 3; flow.items[1]->rect[0] = 615.0;
                        it.col = 1;
                    } else if (insertAtTwo) {
                        flow.items[1]->col = 3; flow.items[1]->rect[0] = 615.0;
                        it.col = 2;
                    } else {
                        it.col = 3;
                    }
                    it.row = 0;
                    it.rect[0] = 205.0 * it.col;
                    it.rect[1] = 0.0;
                } else if (threeNegativeCandidate && k == 2 && col == 0 && row0 && cx < 0.0 &&
                    flow.items[0]->row == 0 && flow.items[0]->col == 0 &&
                    flow.items[1]->row == 0 && flow.items[1]->col == 1) {
                    // A genuinely negative quotient requests direction -1:
                    // insert C before A and shift the occupied suffix right.
                    flow.items[0]->col = 1;
                    flow.items[0]->rect[0] = 205.0;
                    flow.items[1]->col = 2;
                    flow.items[1]->rect[0] = 410.0;
                    it.row = 0; it.col = 0;
                    it.rect[0] = 0.0; it.rect[1] = 0.0;
                } else if (threeCollisionCandidate && k == 2 && col == 1 && row0 &&
                    flow.items[1]->row == 0 && flow.items[1]->col == 1) {
                    // Native direction changes at x=205. Before it, C inserts
                    // at occupied column 1 and shifts B right. Afterwards it
                    // advances past B to the empty column 2.
                    if (it.rect[0] < 205.0) {
                        flow.items[1]->col = 2;
                        flow.items[1]->rect[0] = 410.0;
                        it.col = 1;
                    } else {
                        it.col = 2;
                    }
                    it.row = 0;
                    it.rect[0] = 205.0 * it.col;
                    it.rect[1] = 0.0;
                } else if ((recurrenceCandidate || nonzeroPairCandidate || threeCollisionCandidate ||
                            threeNegativeCandidate || fourPositiveCandidate || fivePositiveCandidate ||
                            sixPositiveCandidate || sevenPositiveCandidate || negativeMultiCandidate ||
                            (negativeEightCandidate && k < 7)) &&
                           k > 0 && row0 && col == 0 &&
                    flow.items[0]->row == 0 && flow.items[0]->col == 0) {
                    // Insertion at column 1 recursively shifts the occupied
                    // type-1 run; the model vector remains in source order.
                    for (std::size_t prior = k; prior-- > 1;) {
                        flow.items[prior]->col = static_cast<int>(k + 1 - prior);
                        flow.items[prior]->rect[0] = 205.0 * flow.items[prior]->col;
                    }
                    it.row = 0;
                    it.col = 1;
                    it.rect[0] = 205.0;
                    it.rect[1] = 0.0;
                } else {
                    throw Error(M2M_E_UNSUPPORTED, "project file: diagram item without CellPos on an occupied cell (0x14015b610)");
                }
            } else {
                it.row = 0;
                it.col = col;
                if (singletonRelayoutCandidate || mixedPairCandidate || threeDirectCandidate || multiDirectCandidate ||
                    negativeTallDirectCandidate || positiveTallDirectCandidate ||
                    (fourPositiveCandidate && k == 3) || (fivePositiveCandidate && k == 4) ||
                    (sixPositiveCandidate && k == 5) || (sevenPositiveCandidate && k == 6) ||
                    (threeCollisionCandidate && k == 2) ||
                    ((nonzeroPairCandidate || threeCollisionCandidate || threeNegativeCandidate ||
                      fourPositiveCandidate || fivePositiveCandidate || sixPositiveCandidate ||
                      sevenPositiveCandidate || negativeMultiCandidate) && k == 0)) {
                    it.rect[0] = 205.0 * col;
                    it.rect[1] = 0.0;
                }
                if (freeColumnPairCandidate && k == 1 && col >= 1) {
                    it.rect[0] = 205.0 * col;
                    it.rect[1] = 0.0;
                }
            }
        } else if (singletonExplicitCellCandidate || explicitPairCandidate || mixedPairCandidate ||
                   threeDirectCandidate || multiDirectCandidate || negativeTallDirectCandidate ||
                   positiveTallDirectCandidate) {
            it.rect[0] = 205.0 * it.col;
            it.rect[1] = 0.0;
        }
        used.insert({it.row, it.col});
        if ((fourPositiveCandidate || fivePositiveCandidate || sixPositiveCandidate ||
             sevenPositiveCandidate) && k == 2)
            used.insert({0, 2});
        if ((fivePositiveCandidate || sixPositiveCandidate || sevenPositiveCandidate) && k == 3)
            used.insert({0, 3});
        if ((sixPositiveCandidate || sevenPositiveCandidate) && k == 4) used.insert({0, 4});
        if (sevenPositiveCandidate && k == 5) used.insert({0, 5});
        if (negativeMultiCandidate && k >= 2 && k + 1 < flow.items.size())
            used.insert({0, static_cast<int>(k)});
        if (it.row == 0) row0 = true;
    }
    if (recurrenceCandidate || nonzeroPairCandidate || freeColumnPairCandidate || negativeEightCandidate ||
        singletonRelayoutCandidate || singletonExplicitCellCandidate || explicitPairCandidate || mixedPairCandidate ||
        threeCollisionCandidate || threeNegativeCandidate || fourPositiveCandidate || fivePositiveCandidate ||
        sixPositiveCandidate || sevenPositiveCandidate || negativeMultiCandidate ||
        threeDirectCandidate || multiDirectCandidate || negativeTallDirectCandidate ||
        positiveTallDirectCandidate) {
        // @src 0x140158cc0: relayout clears links, then reconnects compatible
        // occupied graphics in grid order, including the selected row boundary.
        // These selected Forming2D items have compatible Control2d connectors.
        std::vector<FlowItem*> grid;
        for (const auto& item : flow.items) {
            item->prev = nullptr;
            item->next = nullptr;
            grid.push_back(item.get());
        }
        std::sort(grid.begin(), grid.end(), lessByCell);
        for (std::size_t k = 1; k < grid.size(); ++k) {
            // The native grid scan visits empty cells too and clears its previous
            // item there. In this selected row-0 pair, an unoccupied column gap
            // therefore prevents a synthesized connector edge.
            if ((freeColumnPairCandidate || explicitPairCandidate || mixedPairCandidate || threeDirectCandidate ||
                 threeCollisionCandidate || fourPositiveCandidate || fivePositiveCandidate ||
                 sixPositiveCandidate || sevenPositiveCandidate ||
                 multiDirectCandidate || negativeTallDirectCandidate || positiveTallDirectCandidate) &&
                grid[k]->row == grid[k - 1]->row &&
                grid[k]->col > grid[k - 1]->col + 1) continue;
            connectItems(grid[k - 1], grid[k]);
        }
    }
}

namespace {

// The classes whose init (vt+0x180) sets the item info's mime list (+0x90, 0x1405fdfc0) to one
// starting with "application/sftc.text.obj" (gui_mo/s1/moproj/v30c.c); other classes leave it
// empty. The process list (+0x88, setter 0x1405fdf70) is never set, so it is always empty.
bool acceptsObjects(const String& cls)
{
    static const char16_t* const kClasses[] = {
        u"SfOprConvert2Dto3D",          // @src 0x1401c24d0
        u"SfOprDiestressBM2D",          // @src 0x14029a880
        u"SfOprDiestressBM3D",          // @src 0x14029c6c0
        u"SfOprDiestress",              // @src 0x1402b4fb0
        u"SfOprMultiBlow2D",            // @src 0x140600a50
        u"SfOprMultiBlow3D",            // @src 0x140601480
        u"SfOprCogging",                // @src 0x140750140
        u"SfOprForming2D",              // @src 0x140c043e0
        u"SfOprForming",                // @src 0x140c05bc0
        u"SfOprHTFurnace",              // @src 0x140c65420
        u"SfOprCopyMirror",             // @src 0x140e22470
        u"SfOprHeatTreat2D",            // @src 0x1411ae690
        u"SfOprHeatTreat",              // @src 0x1411b0ca0
        u"SfOprBoolean3D",              // @src 0x1411de9b0 (both dimensions)
        u"SfOprBoolean2D",
        u"SfOprPre2D",                  // @src 0x1413474d0
        u"SfOprPre3D",                  // @src 0x1413489d0
        u"SfOprCycle",                  // @src 0x14134d2e0
        u"SfOprGroup",                  // @src 0x141352020
        u"SfOprHeatTransfer2D",         // @src 0x1413530d0
        u"SfOprHeatTransfer",           // @src 0x141353ec0
        u"SfOprDistortion3D",           // @src 0x141362050 (both dimensions)
        u"SfOprDistortion2D",
        u"SfOprHeatTransferExpress2D",  // @src 0x1413e57a0
        u"SfOprHeatTransferExpress",    // @src 0x1413e6060
        u"SfOprFormingExpress2D",       // @src 0x1414126f0
        u"SfOprFormingExpress",         // @src 0x141413000
    };
    for (const char16_t* c : kClasses)
        if (cls.u16() == c) return true;
    return false;
}

// SfDiagItemExpansion rows: Text through vt+0x58 (0x140160e70, indent 2), Line through vt+0x60
// (0x140160f20), Space through vt+0x68 (0x140160f10)
void addText(FlowItem& it, const String& text)
{
    ExpansionRow r;
    r.type = 0;
    r.label = L("  ") + text;                        // 0x140188730: indent spaces + text
    it.expansion.push_back(r);
}

// an object row: a Text row with its data (0x140160e70 with the "application/sftc.text.obj..."
// mime data) whose accepted mime types (vt+0x38) are eqp and mat (0x140073900, 0x14007b630)
void addObjectRow(FlowItem& it, const String& text)
{
    addText(it, text);
    it.expansion.back().accepts = {L("application/sftc.text.eqp"), L("application/sftc.text.mat")};
}

void addLine(FlowItem& it, int style)
{
    ExpansionRow r;
    r.type = 1;
    r.lineStyle = style;                             // colour QColor(0, 0, 0, 255)
    it.expansion.push_back(r);
}

void addSpace(FlowItem& it)
{
    ExpansionRow r;
    r.type = 2;
    it.expansion.push_back(r);
}

// @src 0x140071d40  the class template: Process, its process items (none), Objects
void expansionTemplate(FlowItem& it)
{
    addText(it, L("Process"));                       // tr(): no translator is installed
    addLine(it, 1);
    for (int i = 0; i < 2; ++i) addSpace(it);        // process list (+0x88) empty: 2 Spaces
    addLine(it, 1);
    if (acceptsObjects(it.className)) {
        addText(it, L("Objects"));
        addLine(it, 2);
    }
}

}  // namespace

// @src 0x140073900  (expansion part) the template, then for an operation whose class accepts
// objects one Text row per object slot: "Undefined" (code -1), "Read From DB" (-2), else the
// slot's object name (0x140061250, which appends missing entries, and 0x14004d730). The
// lubricant row goes to an expansion row that accepts "application/sftc.text.lub" (vt+0x40); no
// row does (process items are never set; object rows accept mat and eqp), so none is added.
// 0x140086fe0(model, id, 1) for an operation with a restart (0x140064740) is not ported: it does
// not touch the expansion list.
void buildExpansion(Project& p, FlowItem& it)
{
    expansionTemplate(it);
    Operation* op = operationById(p, it.opId);       // 0x14001d7d0 -> 0x140047950 -> 0x140045f90
    if (!op) return;
    it.itemName = op->name;                           // 0x140050ac0 before later XML ItemName
    if (!acceptsObjects(it.className)) return;
    const int ns = objSlots(*op);                    // 0x14005fa80
    for (int s = 0; s < ns; ++s) {
        const int code = s < static_cast<int>(op->objIds.size()) ? op->objIds[static_cast<std::size_t>(s)] : -99;   // 0x14005fb30
        if (code == -1) {
            addText(it, L("Undefined"));             // accepted-type list remains empty
        } else if (code == -2) {
            addObjectRow(it, L("Read From DB"));
        } else if (ObjEntry* e = objEntry(*op, s)) {
            addObjectRow(it, e->name);
        }
    }
}

// ---- SfMoDiagEditorModel slots the engine's messages reach -----------------------------------------
namespace {

// DAT_14358133c (.data, 7): the rows of the template before the first object row
constexpr int kObjectRowBase = 7;

// the text of a Text row without its indent (vt+0x10(.., false): QString::mid(indent))
String rowText(const ExpansionRow& r)
{
    const std::u16string& l = r.label.u16();
    return String(l.size() >= 2 ? l.substr(2) : std::u16string());
}

// vt+0x8: indent spaces + text (0x140188730)
void setRowText(ExpansionRow& r, const String& text)
{
    r.label = L("  ") + text;
}

// QStringList::filter(type).size() of the row's accepted types (vt+0x40, 0x140088970)
bool rowAccepts(const ExpansionRow& r, const char* type)
{
    const String t = L(type);
    for (const String& a : r.accepts)
        if (a.u16().find(t.u16()) != std::u16string::npos) return true;
    return false;
}

// "<text before the first ':'>: <name>" (QString::split(":")[0] + ": " + name)
String labelWith(const ExpansionRow& r, const String& name)
{
    const std::u16string t = rowText(r).u16();
    const std::size_t c = t.find(u':');
    return String(c == std::u16string::npos ? t : t.substr(0, c)) + L(": ") + name;
}

// the diagram model's items (+0x20): the active sub-project's flow chart; null when the model has
// no editor (+0x18)
FlowChart* diagramItems(Project& p)
{
    SubProject* sp = activeSubProject(p);
    return sp ? &sp->flow : nullptr;
}

}  // namespace

void diagramObjDataAdded(Project& p, int opId, int index, const String& name)
{
    FlowChart* flow = diagramItems(p);
    if (!flow) return;
    for (auto& it : flow->items) {
        if (it->opId != opId) continue;                                   // vt+0x28
        std::vector<ExpansionRow>& rows = it->expansion;                  // vt+0x50
        const int n = static_cast<int>(rows.size());
        if (index > -1 && index == n - kObjectRowBase) {
            addObjectRow(*it, name);                                      // vt+0x58 "Object", then vt+0x8 name
        } else if (index > -1 && index < n - kObjectRowBase) {
            setRowText(rows[static_cast<std::size_t>(index + kObjectRowBase)], name);   // vt+0x8
        }
        // 0x140082760 (ports) and signalDiagEditorUpdateItem: display
        return;
    }
}

void diagramObjDataChanged(Project& p, int opId, int index, const String& name)
{
    FlowChart* flow = diagramItems(p);
    if (!flow) return;
    for (auto& it : flow->items) {
        if (it->opId != opId) continue;
        const int row = index + kObjectRowBase;
        std::vector<ExpansionRow>& rows = it->expansion;
        if (row < 0 || row >= static_cast<int>(rows.size())) continue;   // qDebug "Error in ...ObjDataChanged()"
        setRowText(rows[static_cast<std::size_t>(row)], name);
    }
}

void diagramEqpDataAdded(Project& p, int opId, int index, const String& name)
{
    FlowChart* flow = diagramItems(p);
    if (!flow || index != -1) return;                                     // only -1 is handled
    for (auto& it : flow->items) {
        if (it->opId != opId) continue;
        for (ExpansionRow& r : it->expansion)
            if (rowAccepts(r, "application/sftc.text.eqp")) {             // the first such row
                setRowText(r, labelWith(r, name));
                break;
            }
    }
}

void refreshDiagramItems(Project& p)
{
    FlowChart* flow = diagramItems(p);
    if (!flow) return;
    for (std::size_t k = 0; k < flow->items.size(); ++k) {
        FlowItem& it = *flow->items[k];
        Operation* op = operationById(p, it.opId);   // 0x140047dc0
        if (!op) continue;
        it.itemName = op->name;                       // item vt+0x10(0x140050ac0)
        const int n = objSlots(*op);                  // 0x14005fa80
        for (int s = 0; s < n; ++s) {
            ObjEntry* e = objEntry(*op, s);           // 0x140061250 (0x14005fe70, 0x14005fdd0)
            String name = e ? e->name : String();
            if (objId(*op, s) == -2)                  // 0x14005fb30
                name = L("Read From DB");
            else if ((e ? e->objectType : 7) == 7)
                name = L("Undefined");
            diagramObjDataAdded(p, op->id, s, name);  // 0x14007b630 (model +0x54 = 1: no update signal)
        }
    }
}

void diagramLubDataAdded(Project& p, int opId, const String& name)
{
    FlowChart* flow = diagramItems(p);
    if (!flow) return;
    for (auto& it : flow->items) {
        if (it->opId != opId) continue;
        for (ExpansionRow& r : it->expansion)
            if (rowAccepts(r, "application/sftc.text.lub")) {
                setRowText(r, labelWith(r, name));
                break;
            }
    }
}

// @src 0x140097d00  RectF children into a QRectF (x, y, width, height): x and y go through
// QRectF::setX / setY, which move the left / top edge and change the width / height
bool readRectF(const xml::Node& e, double* r)
{
    for (const xml::Node* c : e.childElements()) {
        const double v = qt::toDouble(c->text());
        if (c->tag == L("x")) {
            r[2] -= v - r[0];
            r[0] = v;
        } else if (c->tag == L("y")) {
            r[3] -= v - r[1];
            r[1] = v;
        } else if (c->tag == L("width")) {
            r[2] = v;
        } else if (c->tag == L("height")) {
            r[3] = v;
        }
    }
    return true;
}

// @src 0x140091f00  ParseDiagramItem
FlowItem* parseDiagramItem(Project& p, FlowChart& flow, const xml::Node& e, int k,
                           std::map<int, int>& next)
{
    FlowItem* it = nullptr;
    double rect[4] = {};                                     // QRectF(), one per call
    (void)(e.attribute(L("Expanded")) == L("yes"));   // compared, result unused
    const int opId = qt::toInt(e.attribute(L("Operation_Id")));
    for (const xml::Node* c : e.childElements()) {
        if (c->tag == L("ClassName")) {
            const String cls = mapLegacyClass(c->text());
            auto item = std::make_unique<FlowItem>();      // model +0x58 (0x140073900)
            item->opId = opId;
            item->className = cls;
            item->group = isGroupClass(cls);
            buildExpansion(p, *item);                         // 0x140073900
            it = item.get();
            flow.items.push_back(std::move(item));          // model +0x68 (0x14014cf20)
        } else if (c->tag == L("ItemName")) {
            const String name = c->text();
            if (it) it->itemName = name;                     // item +0x10
        } else if (c->tag == L("RectF")) {
            if (readRectF(*c, rect) && it)                   // 0x140097d00
                std::copy(rect, rect + 4, it->rect);         // item vt+0x90
        } else if (c->tag == L("CellPos")) {
            const int col = qt::toInt(c->attribute(L("Col")));
            const int row = qt::toInt(c->attribute(L("Row")));
            if (!it) nullItem("CellPos");
            it->row = row;                                   // item +0xc0(row, col)
            it->col = col;
        } else if (c->tag == L("GroupInfo")) {
            const int bc = qt::toInt(c->attribute(L("BeginCol")));
            const int br = qt::toInt(c->attribute(L("BeginRow")));
            if (!it) nullItem("GroupInfo");
            it->beginRow = br;                               // item +0xd0
            it->beginCol = bc;
            const int ec = qt::toInt(c->attribute(L("EndCol")));
            const int er = qt::toInt(c->attribute(L("EndRow")));
            it->endRow = er;                                 // item +0xe0
            it->endCol = ec;
        } else if (c->tag == L("ItemData")) {
            if (!it) {
                if (!c->attrs.empty()) nullItem("ItemData");
            } else {
                parseItemData(*c, *it);
            }
        } else if (c->tag == L("Connectors")) {
            parseConnectors(*c, k, next);
        }
    }
    return it;
}

// @src 0x1400b6630  operation ids of the chain of the sub-project's flow chart; members of a group
// give the group id once per run of consecutive members
std::vector<int> mstOrder(const SubProject& sub)
{
    std::vector<int> ids;
    const FlowChart& flow = sub.flow;
    const std::map<int, int> groupOf = groupMap(flow);
    int last = -1;
    for (FlowItem* it : flowChain(flow)) {
        auto g = groupOf.find(it->opId);
        if (g == groupOf.end()) {
            ids.push_back(it->opId);
            last = -1;
        } else if (g->second != last) {
            ids.push_back(g->second);
            last = g->second;
        }
    }
    return ids;
}

// @src 0x1400b7470  ids of the chain items that lie inside the group item with groupId, chain order
std::vector<int> groupMembers(const SubProject& sub, int groupId)
{
    std::vector<int> ids;
    const FlowChart& flow = sub.flow;
    const FlowItem* g = findItemById(flow, groupId);
    if (!g) return ids;
    const std::vector<FlowItem*> set = itemsInGroup(flow, *g);
    for (FlowItem* it : flowChain(flow))
        if (std::find(set.begin(), set.end(), it) != set.end()) ids.push_back(it->opId);
    return ids;
}

namespace {

// @src 0x140047d70  op class of the operation with an id: 0x140047dc0, then 0x140054e90
mst::OpClass* opClassById(const Project& p, int id)
{
    const Operation* op = operationById(p, id);
    return op ? opBase(*op) : nullptr;
}

// @src 0x1405f5b90  OB->vt[0x188]() (class info) ->vt[0x38]() name: the registry key the class was
// filed under (0x14006f940 files OB and its info under the same init name). The original
// dereferences a null OB; the converter fails instead.
const String& classInfoName(const mst::OpClass* ob)
{
    if (!ob) throw Error(M2M_E_INPUT, "flow chart operation without a registered class");
    return ob->key();
}

}  // namespace

// @src 0x1400b94f0
std::vector<Operation*> flowOperations(const SubProject& sub)
{
    std::vector<Operation*> list;
    const Project* p = sub.project;                    // 0x1400b5c40
    if (!p) return list;                                // 0x1400a58b0: the OprManager always exists here
                                                        // (created on demand by 0x140044c80: Project::operations)
    for (FlowItem* it : flowChain(sub.flow)) {
        const int id = it->opId;
        Operation* op = operationById(*p, id);
        if (!op) continue;
        const String& name = classInfoName(opClassById(*p, id));
        if (name == L("SfOprReport") || name == L("SfOprDoeOutput") || name == L("SfOprShapeOptim"))
            continue;
        const int n = flowNumber(*op);
        if (static_cast<int>(list.size()) < n) {
            list.push_back(op);
        } else {
            // n = 0 (op not numbered in the active sub-project) indexes list[-1] in the original
            if (n <= 0) throw Error(M2M_E_INPUT, "operation " + std::to_string(id) + " has no flow number");
            list[static_cast<size_t>(n - 1)] = op;
        }
    }
    return list;
}

// @src 0x140085800  1-based number of an id along the chain; a group counts once and its members
// share its number. Built into model+0x40 while that map is empty; 0 for an unknown id.
int flowChainNumber(FlowChart& flow, int opId)
{
    std::map<int, int>& num = flow.numberCache;
    if (num.empty()) {
        const std::map<int, int> groupOf = groupMap(flow);
        int last = -1;
        int n = 1;
        for (FlowItem* it : flowChain(flow)) {
            auto g = groupOf.find(it->opId);
            if (g == groupOf.end()) {
                num[it->opId] = n++;
                last = -1;
            } else {
                int gid = g->second;
                if (gid != last) {
                    num[gid] = n++;
                    last = gid;
                }
                num[it->opId] = num[gid];
            }
        }
        flow.numberCacheBuilt = true;
    }
    auto f = num.find(opId);
    return f == num.end() ? 0 : f->second;
}

// @src 0x140086720  id of the predecessor (+0x138) of the item with opId, -1 if none
int flowPredecessor(const FlowChart& flow, int opId)
{
    const FlowItem* it = itemById(flow, opId);
    if (!it || !it->prev) return -1;
    return it->prev->opId;
}

}  // namespace m2m
