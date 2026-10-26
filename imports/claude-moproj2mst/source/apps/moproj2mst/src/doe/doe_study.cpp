// SfMoDoeManager: the DOE study files of File > Save (MDT L6T). Decompiles
// gui_mo/s1/mdt/DOE/d0.c .. d2.c; spec docs/re/moproj2mst/mdt/L6/doe.md. Oracle B 120_B_DOE (a
// study without variables). Unqualified variable/constraint payloads refuse;
// region names use the existing value projection without geometry ownership.
#include "m2m/doe_study.hpp"
#include "input_table_reader.hpp"
#include "empty_variable_prune.hpp"

#include <algorithm>
#include <bit>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>

#include <cstdio>
#include <memory>

#include <defcmd/dispatch.hpp>
#include <defmodel/keyword_registry.hpp>
#include <defmodel/layout/db_step_data.hpp>

#include "m2m/longpath.hpp"
#include "m2m/qt4path.hpp"

namespace m2m::engine {
std::string local8(const qt::String& s);   // src/engine/loader.cpp
}

namespace m2m::doe {
namespace {

using qt::L;

bool dirExists(const String& d)
{
    std::error_code ec;
    return std::filesystem::is_directory(std::filesystem::path(win32Path(d.u16())), ec);
}

bool readAll(const String& path, std::string& out)
{
    std::ifstream f(std::filesystem::path(win32Path(path.u16())), std::ios::binary);
    if (!f) return false;
    out.assign(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());
    return true;
}

[[noreturn]] void unsupported(const std::string& what)
{
    throw Error(M2M_E_UNSUPPORTED, "DOE study: " + what);
}

// Data-only vt+0x100 (0x14091a090) prefix. The geometry-file removal caller
// performs its separate path/probe effects only after this mutation returns.
void eraseOwnedHeaderVariable(Study& study, std::vector<Var*>::iterator variableIt)
{
    Var* const removed = *variableIt;
    const auto ownerIt = std::find_if(study.headerVariableOwners.begin(), study.headerVariableOwners.end(),
        [removed](const auto& owner) { return owner.get() == removed; });
    if (ownerIt == study.headerVariableOwners.end())
        unsupported("variable removal owner is outside the concrete header-record profile");
    for (Var* variable : study.vars)
        if (variable && variable->link == removed) variable->link = nullptr;
    // Var has no virtual destructor: release the matching concrete owner first.
    study.headerVariableOwners.erase(ownerIt);
    study.vars.erase(variableIt);
    study.samples.invalid.fill(true);
}

// @src 0x14091da70: ordered scan, recomputed for each nonpositive outer row.
// This is not lowest-free-ID; add eax,1 wraps before the next signed comparison.
int nextVariableNumber(const Study& study)
{
    static_assert(sizeof(int) == sizeof(std::int32_t));
    std::int32_t candidate = 0;
    for (const auto* variable : study.vars) {
        // The outer reader skips nulls, but this inner source getter does not.
        if (!variable)
            unsupported("variable-number finalization: original null variable dereference (0x14091dadd) remains unsupported");
        const auto number = static_cast<std::int32_t>(variable->number);
        if (number >= candidate) {
            const auto incremented = static_cast<std::uint32_t>(number) + std::uint32_t(1);
            candidate = std::bit_cast<std::int32_t>(incremented);
        }
    }
    return static_cast<int>(candidate);
}

// @src 0x1409128bc -> manager vt+1e0,0x14091c7a0. The caller's stable
// existing list is borrowed; no variable is constructed or removed here.
void reindexExistingVariables(Study& study)
{
    const int count = study.count();
    for (int index = 0; index < count; ++index) {
        auto* variable = study.varAt(index);
        if (!variable)
            unsupported("empty DoeVariables reindex: original null variable dereference (0x14091c800) remains unsupported");
        variable->index = index; // Encounter order matters for aliases/faults.
    }
}

// QFile::exists with this save's earlier removals, renames and writes taken into account
bool existsFx(const String& path, const engine::FileEffects& fx)
{
    bool exists = qt::fileExists(path);
    for (const String& d : fx.deletes)
        if (d == path) exists = false;
    for (const engine::FileEffects::Rename& r : fx.renames) {
        if (r.to == path) exists = true;
        else if (r.from == path) exists = false;
    }
    for (const engine::FileEffects::Write& w : fx.writes)
        if (w.path == path) exists = true;
    return exists;
}

// <project dir>\DoeStudies\<name>\Control\ (the common head of 0x1400d5970 / 5ee0 / 6450)
String controlDir(const Study& s)
{
    const Project* prj = s.sub ? s.sub->project : nullptr;    // 0x1400b5c40
    String dir = prj ? projectDir(*prj) : String();          // 0x1400a4220
    const String sep = String(std::u16string(1, qt::kSep));
    // ((((sep + "DoeStudies") + sep) + name) + sep) + "Control" + sep, appended to the folder
    dir += sep + L("DoeStudies") + sep + s.name + sep + L("Control") + sep;   // @0x142166c28, 0x1427baf10
    return dir;
}

}  // namespace

std::vector<Var*> Study::geometryVars(int, int) const
{
    if (!vars.empty()) unsupported("geometry variable mapping (0x14091ba60) is not recovered");
    return {};
}

std::shared_ptr<Study> makeStudy(SubProject& sub, StudyIo* io)
{
    auto study = std::make_shared<Study>();
    study->sub = &sub;
    study->name = L("DoeStudy") + qt::number(sub.id);
    if (io) {
        if (!sub.project) unsupported("study constructor without owning project (0x1400d1180)");
        const String dir = projectDir(*sub.project) + qt::kSep + L("DoeStudies") + qt::kSep + study->name;
        (void)makeStudyDirs(dir, *io);                      // original constructor ignores failure
    }
    return study;
}

void parseSampleText(const String& text, int& mode, SampleTable& rows)
{
    rows.clear();                                         // 0x1409172d0, before even QFile::open
    const auto& input = text.u16();
    // Shipped QRegExp \\s: category mask 0x380 plus code units 9..13 (0x67062ceb).
    // See wave2-doe-regexp-space-contract.json; U+0085 remains part of a token.
    auto space = [](char16_t c) {
        if ((c >= 9 && c <= 13) || c == 32 || (c >= 0x2000 && c <= 0x200a)) return true;
        switch (c) {
        case 0xa0: case 0x1680: case 0x180e: case 0x2028: case 0x2029:
        case 0x202f: case 0x205f: case 0x3000: return true;
        default: return false;
        }
    };
    std::size_t begin = 0;
    bool first = true;
    while (begin < input.size()) {
        std::size_t end = input.find(u'\n', begin);
        if (end == std::u16string::npos) end = input.size();
        std::vector<String> tokens;
        std::size_t at = begin;
        while (at < end) {
            while (at < end && space(input[at])) ++at;
            const std::size_t token = at;
            while (at < end && !space(input[at])) ++at;
            if (at != token) tokens.emplace_back(input.substr(token, at - token));
        }
        if (first) {
            if (!tokens.empty()) mode = qt::toInt(tokens.front());
            first = false;
        } else if (tokens.size() > 1) {
            std::vector<double> row;
            for (std::size_t i = 1; i < tokens.size(); ++i) row.push_back(qt::toDouble(tokens[i]));
            rows.push_back(std::move(row));                 // 0x1409171b0, append without grouping
        }
        if (end == input.size()) break;
        begin = end + 1;
    }
}

namespace {
std::string serializeSampleTable(int mode, const SampleTable& rows)
{
    std::string bytes = qt::formatInt(mode) + "\r\n";
    int rowNo = 0;
    for (const auto& row : rows) {
        bytes += qt::formatInt(++rowNo) + '\t';
        for (double value : row) bytes += qt::formatDouble(value) + '\t';
        bytes += "\r\n";
    }
    return bytes;
}
} // namespace

bool writeSampleTable(int mode, const SampleTable& rows, const String& path, engine::FileEffects& fx)
{
    const std::string bytes = serializeSampleTable(mode, rows);
    fx.writes.push_back({path, bytes});
    return true;
}

bool saveSampleTableImmediate(const Study& s, int mode, const String& path, StudyIo& io)
{
    if (mode < 0 || mode >= 3 || s.samples.invalid[mode]) return false;
    return io.writeTextFile(path, serializeSampleTable(mode, s.samples.tables[mode]));
}

ImmediateType2SampleSequence saveType2SampleSequenceImmediate(
    const Study& s, const String& doeStudiesDir, StudyIo& io)
{
    const String sep = String(std::u16string(1, qt::kSep));
    const String control = doeStudiesDir + sep + s.name + sep + L("Control") + sep;
    ImmediateType2SampleSequence result;
    const auto record = [&](std::size_t ordinal, int mode, const String& path) {
        auto& write = result.writes[ordinal];
        write.mode = mode;
        write.path = path;
        if (s.samples.invalid[mode]) {
            write.state = ImmediateSampleWriteState::SkippedInvalid;
            return;
        }
        write.state = saveSampleTableImmediate(s, mode, path, io)
            ? ImmediateSampleWriteState::Opened : ImmediateSampleWriteState::OpenFailed;
    };

    const int currentMode = s.samples.mode;
    if (currentMode < 0 || currentMode >= 3)
        unsupported("current sample mode outside manager storage (0x1400d75a0)");
    record(0, currentMode, control + L("DEF_DOE.PAR"));

    static const std::array<const char*, 3> fileNames{
        "Samples_LHS.par", "Samples_FF.par", "Samples_USR.par"};
    for (int mode = 0; mode < 3; ++mode)
        record(static_cast<std::size_t>(mode + 1), mode, control + L(fileNames[mode]));
    return result;
}

bool saveSampleTable(const Study& s, int mode, const String& path, engine::FileEffects& fx)
{
    if (mode < 0 || mode >= 3 || s.samples.invalid[mode]) return false;
    return writeSampleTable(mode, s.samples.tables[mode], path, fx);
}

bool saveCurrentSamples(const Study& s, const String& doeStudiesDir, engine::FileEffects& fx)
{
    const int mode = s.samples.mode;
    if (mode < 0 || mode >= 3) unsupported("current sample mode outside manager storage (0x1400d75a0)");
    const String sep = String(std::u16string(1, qt::kSep));
    return saveSampleTable(s, mode, doeStudiesDir + sep + s.name + sep + L("Control") + sep + L("DEF_DOE.PAR"), fx);
}

bool loadSampleTable(Study& s, int mode, const String& path, engine::FileEffects& fx)
{
    if (mode < 0 || mode >= 3) return false;
    // 0x1400d7a80 calls the WRITER 0x140915360 with a new, empty local table.
    if (!writeSampleTable(mode, {}, path, fx)) return false;
    if (!s.samples.tables[mode].empty()) {
        const int current = s.samples.mode;
        if (current < 0 || current >= 3)
            unsupported("current sample mode outside manager storage (0x1400d7a80)");
        s.samples.tables[current].clear();
        s.samples.invalid[mode] = false;
    }
    return true;
}

void loadCurrentSampleText(Study& s, const String& text)
{
    SampleTable rows;
    // The original reads directly into the manager's selected-mode field, even if invalid.
    parseSampleText(text, s.samples.mode, rows);
    const int mode = s.samples.mode;
    if (mode < 0 || mode >= 3) unsupported("loaded sample mode outside manager storage (0x1400d7820)");
    s.samples.tables[mode] = std::move(rows);
    s.samples.invalid[mode] = false;
    s.samples.counts[mode] = static_cast<int>(s.samples.tables[mode].size());
}

String fileNameOf(const String& s)
{
    std::u16string u = s.u16();
    if (!u.empty() && u[0] == u'-')
        for (char16_t& c : u)
            if (c == u'-') c = u'_';                          // @0x1427a4d2c
    static const char16_t kRepl[] = u"*#!@$ '\"\\/<>:;.,|?(){}[]%^=&";
    for (const char16_t* r = kRepl; *r; ++r)                 // one QString::replace per character
        for (char16_t& c : u)
            if (c == *r) c = u'_';
    return String(u);
}

String geoPath(const Study& s, GeoKind k, int id, const String& a, const String& b)
{
    const char* fmt = k == GeoKind::Nominal ? "%1_%2_%3_Nominal"   // @0x142166c38
                      : k == GeoKind::Upper ? "%1_%2_%3_Upper"     // @0x142166c68
                                            : "%1_%2_%3_Lower";    // @0x142166c90
    String f = qt::arg(qt::arg(qt::arg(L(fmt), b), a), id, 0, 10);
    f = fileNameOf(f);                                       // 0x140819600
    f += L(".GEO");                                          // @0x142166c4c
    return controlDir(s) + f;
}

bool makeStudyDirs(const String& dir, engine::FileEffects& fx)
{
    // QDir(dir).exists() ? : mkpath, then Control, Solutions, Output (@0x1427baf30 / 38 / 44)
    for (const char* sub : {static_cast<const char*>(nullptr), "Control", "Solutions", "Output"}) {
        const String d = sub ? dir + qt::kSep + L(sub) : dir;
        if (!dirExists(d)) fx.mkdirs.push_back(d);
    }
    return true;
}

bool makeStudyDirs(const String& dir, StudyIo& io)
{
    for (const char* sub : {static_cast<const char*>(nullptr), "Control", "Solutions", "Output"}) {
        const String path = sub ? dir + qt::kSep + L(sub) : dir;
        if (!io.ensureDirectory(path)) return false;
    }
    return true;
}

void pruneEmptyHeaderVariables(Study& study)
{
    // Admission only, before the native collect/delete phases. Foreign/null/
    // aliased owners and duplicate content names are outside this bounded cut.
    if (study.vars.size() != study.headerVariableOwners.size() ||
        std::any_of(study.headerVariableOwners.begin(), study.headerVariableOwners.end(),
            [](const auto& owner) { return !owner; }))
        unsupported("empty-variable prune needs coherent concrete header owners");
    for (std::size_t i = 0; i < study.vars.size(); ++i) {
        const auto* variable = study.vars[i];
        if (!variable || std::none_of(study.headerVariableOwners.begin(), study.headerVariableOwners.end(),
                [variable](const auto& owner) { return owner.get() == variable; }))
            unsupported("empty-variable prune needs coherent concrete header owners");
        for (std::size_t prior = 0; prior < i; ++prior)
            if (study.vars[prior] == variable || study.vars[prior]->name == variable->name)
                unsupported("empty-variable prune needs distinct aliases and content names");
    }

    std::vector<String> names;
    for (const auto* variable : study.vars)
        if (variable->items.empty() && variable->files.empty()) names.push_back(variable->name);
    // 91c951: finish collection before deleting; look up first current match.
    for (const auto& name : names) {
        const auto variableIt = std::find_if(study.vars.begin(), study.vars.end(),
            [&name](const Var* variable) { return variable->name == name; });
        if (variableIt != study.vars.end()) eraseOwnedHeaderVariable(study, variableIt);
    }
}

void removeVarFiles(Study& s, int id, const String& a, const String& b, engine::FileEffects& fx)
{
    // vt+0x100 0x14091a090: unlink every incoming +40 pointer before the
    // manager deletes the selected record and removes its list entry.
    auto variableIt = std::find_if(s.vars.begin(), s.vars.end(), [&](const Var* variable) {
        return variable && variable->name == b;
    });
    if (variableIt != s.vars.end()) eraseOwnedHeaderVariable(s, variableIt);
    for (GeoKind k : {GeoKind::Nominal, GeoKind::Upper, GeoKind::Lower}) {
        const String geo = geoPath(s, k, id, a, b);
        if (!existsFx(geo, fx)) continue;
        fx.deletes.push_back(geo);                           // QFile::remove
        const String cor = app::changeSuffix(geo, L("COR")); // 0x140818d50 (@0x142163574)
        if (existsFx(cor, fx)) fx.deletes.push_back(cor);
    }
}

void renameVarFiles(Study& s, Var* var, int id, const String& a, const String& oldName, const String& newName,
                    engine::FileEffects& fx)
{
    String newGeo[3];
    int i = 0;
    for (GeoKind k : {GeoKind::Nominal, GeoKind::Upper, GeoKind::Lower}) {
        const String from = geoPath(s, k, id, a, oldName);
        const String to = geoPath(s, k, id, a, newName);
        newGeo[i] = to;
        if (existsFx(from, fx)) {
            fx.renames.push_back({from, to});                // QFile::rename
            const String corFrom = app::changeSuffix(from, L("COR"));
            if (existsFx(corFrom, fx)) {
                // Nominal: rename(corFrom, corFrom), the new name is built and dropped (0x1400d6dd0)
                const String corTo = k == GeoKind::Nominal ? corFrom : app::changeSuffix(to, L("COR"));
                if (!(corTo == corFrom)) fx.renames.push_back({corFrom, corTo});
            }
        }
        ++i;
    }
    if (var) {
        // 0x1408179d0(var, 0, upper, lower), 0x140817af0(var, 0, nominal): file record 0
        if (var->files.empty()) unsupported("variable without a geometry file record (0x1408179d0)");
        var->files[0].before = newGeo[1];
        var->files[0].after = newGeo[2];
        var->files[0].path = newGeo[0];
    }
}

bool writeInputTable(const String& path, Study& s, engine::FileEffects& fx)
{
    // 0x14091c820: variables with no samples and no geometry files are removed first
    if (!s.vars.empty())
        unsupported("DoeInputTable.XML with variables (0x14090a550 variable part) has no oracle case yet");
    if (s.constraints > 0)
        unsupported("DoeInputTable.XML with constraints (0x14090a550 vt+0x28 part) has no oracle case yet");
    xml::Document doc;                                       // QDomDocument("DoeInputTableFile")
    doc.doctypeName = L("DoeInputTableFile");
    xml::NodePtr root = xml::createElement(L("sfDoeInputTable"));
    doc.root = root;
    xml::setAttribute(*root, L("Version"), L("1"));          // setAttribute(QString, double 1.0): "%.16g"
    auto child = [&](const char* tag) {
        xml::NodePtr e = xml::createElement(L(tag));
        root->children.push_back(e);
        return e;
    };
    xml::setAttribute(*child("DoeCommonMeshPath"), L("path"), s.commonMeshPath);
    xml::setAttribute(*child("DoeCommonMeshOrigName"), L("originalName"), s.commonMeshOrigName);
    xml::setAttribute(*child("DoeDoPositioning"), L("bool"), L(s.doPositioning ? "true" : "false"));
    xml::setAttribute(*child("DoeRoiType"), L("type"), qt::number(s.roiType));
    if (!s.regions.empty()) {
        xml::NodePtr r = child("RegionsOfInterest");
        for (const String& n : s.regions) {
            xml::NodePtr e = xml::createElement(L("Region"));
            r->children.push_back(e);
            xml::setAttribute(*e, L("name"), n);
        }
    }
    child("DoeVariables");
    child("Operations");                                     // the variables' operation map (empty)
    // QTextStream on the QFile (WriteOnly | Text): the locale codec, "\n" -> "\r\n"
    const std::string text = engine::local8(String(xml::toString(doc, 1)));
    std::string bytes;
    for (char c : text) {
        if (c == '\n') bytes += '\r';
        bytes += c;
    }
    fx.writes.push_back({path, bytes});
    return true;
}

bool saveInputTable(Study& s, const String& dir, engine::FileEffects& fx)
{
    // 0x1400d2b60: the variables' operation numbers follow the flow chart
    if (!s.vars.empty()) unsupported("0x1400d2b60 with variables has no oracle case yet");
    const String sep = String(std::u16string(1, qt::kSep));
    const String path = dir + sep + s.name + sep + L("Control") + sep + L("DoeInputTable.XML");   // @0x142166b78
    return writeInputTable(path, s, fx);
}

namespace {
bool parseInputTable(const std::string& bytes, Study& s, const InputTableSource* source)
{
    // The named-source entry owns this optional value for the entire call.
    // Geometry still stops before QFileInfo; do not interpret path/ref state.
    xml::Document doc;
    String err;
    int line = 0, col = 0;
    if (!xml::setContent(doc, bytes, &err, &line, &col) || !doc.root) return false;
    bool status = false;                                  // 0x14090e080, overwritten by child readers
    for (const xml::Node* e : doc.root->childElements()) {
        const String& t = e->tag;
        if (t == L("DoeVariables")) {
            DeferredVariableLinks deferredLinks; // Per reader invocation, not Study history.
            // 0x14090ee9f..0x14090ef0b: only exact direct DoeVariable rows
            // enter payload parsing. Nonmatching/raw children are not recursive.
            for (const auto& child : e->children) {
                if (child && child->kind == xml::Node::Element && child->tag == L("DoeVariable") &&
                    !appendBoundedHeaderVariableFromSource(s, *child, deferredLinks, source))
                    unsupported("DoeVariables reader 0x14090ee20 has no oracle case yet");
            }
            reindexExistingVariables(s); // Before later children and outer ID pass.
            resolveDeferredVariableLinks(s, deferredLinks); // 0x1409128bc..9129b0, after reindex.
            status = true;
        } else if (t == L("Operations")) {
            // 0x140912a20's normal return is true with no payload application.
            // Native QDom child/ref traversal and its internal faults are not
            // represented by this status-only branch, even for populated lists.
            status = true;
        }
        if (t == L("RegionsOfInterest")) {
            s.regions.clear();                             // 0x140912b00 vt+b8 before children
            // vt+a8(0,name) appends direct Region names in order. The native
            // record's null payload is not a geometry/readiness capability.
            for (const auto& child : e->children) {
                if (child && child->kind == xml::Node::Element && child->tag == L("Region"))
                    s.regions.push_back(child->attribute(L("name"), String()));
            }
            status = true;
        }
        if (t == L("Constraints")) {
            clearOwnedConstraintRecords(s);               // 0x140912d10 vt+20 before children
            // The native row dispatcher ignores every nonmatching direct child.
            // A recognized Constraint still needs its actual record owner.
            for (const auto& child : e->children) {
                if (child && child->kind == xml::Node::Element && child->tag == L("Constraint") &&
                    !appendBoundedConstraintRecord(s, *child))
                    unsupported("Constraints reader 0x140912d10 has no oracle case yet");
            }
            status = true;
        } else if (t == L("DoeCommonMeshPath")) {
            s.commonMeshPath = e->attribute(L("path"), String());          // 0x1409170b0
        } else if (t == L("DoeCommonMeshOrigName")) {
            s.commonMeshOrigName = e->attribute(L("originalName"), String());   // 0x1409170f0
        } else if (t == L("DoeDoPositioning")) {
            const std::u16string v = e->attribute(L("bool"), String()).u16();
            auto ieq = [&](const char16_t* w) {              // QString::compare, Qt::CaseInsensitive
                std::u16string a = v, b = w;
                if (a.size() != b.size()) return false;
                for (std::size_t i = 0; i < a.size(); ++i) {
                    char16_t x = a[i], y = b[i];
                    if (x >= u'A' && x <= u'Z') x = static_cast<char16_t>(x + 32);
                    if (y >= u'A' && y <= u'Z') y = static_cast<char16_t>(y + 32);
                    if (x != y) return false;
                }
                return true;
            };
            s.doPositioning = ieq(u"true") || ieq(u"t");    // 0x140917150
        } else if (t == L("DoeRoiType")) {
            s.roiType = qt::toInt(e->attribute(L("type"), String()));   // 0x140917130
        }
    }
    // @src 0x14090ed14..0x14090edb3: after normal child dispatch, even when the
    // retained status is false. Unsupported child exceptions never reach here.
    const int variableCount = s.count();
    for (int index = 0; index < variableCount; ++index) {
        auto* variable = s.varAt(index);                   // vt+1a0, outer null skip
        if (variable && variable->number < 1)
            variable->number = nextVariableNumber(s);    // 0x1409ff590, +1c only
    }
    return status;                                        // headers alone do not make this true
}
} // namespace

bool readInputTable(const std::string& bytes, Study& s)
{
    return parseInputTable(bytes, s, nullptr); // Explicitly source-less in-memory API.
}

bool readInputTableFromSource(const std::string& bytes, Study& s, InputTableSource source)
{
    return parseInputTable(bytes, s, &source);
}

bool readB121TlocModelPrefix(const xml::Node& inputTable, Study& s)
{
    // Candidate-only projection for the one retained B121 row. Keep the public
    // input-table reader's unsupported child guard unchanged: this helper does
    // not deliver 2signalSetOprHasDoe or claim full native parse completion.
    auto fail = [](const char* reason) -> void {
        unsupported(std::string("B121 TLOC model prefix: ") + reason);
    };
    if (inputTable.kind != xml::Node::Element || inputTable.tag != L("sfDoeInputTable"))
        fail("expected sfDoeInputTable root");
    if (!s.vars.empty() || !s.scalarPrefixVariableOwners.empty())
        fail("requires a fresh empty Study");

    const xml::Node* variables = nullptr;
    const xml::Node* operations = nullptr;
    const xml::Node* meshPath = nullptr;
    const xml::Node* meshName = nullptr;
    const xml::Node* positioning = nullptr;
    const xml::Node* roi = nullptr;
    for (const auto* child : inputTable.childElements()) {
        if (child->tag == L("DoeVariables")) {
            if (variables) fail("duplicate DoeVariables list");
            variables = child;
        } else if (child->tag == L("Operations")) {
            if (operations) fail("duplicate Operations list");
            operations = child;
        } else if (child->tag == L("DoeCommonMeshPath")) {
            if (meshPath) fail("duplicate DoeCommonMeshPath");
            meshPath = child;
        } else if (child->tag == L("DoeCommonMeshOrigName")) {
            if (meshName) fail("duplicate DoeCommonMeshOrigName");
            meshName = child;
        } else if (child->tag == L("DoeDoPositioning")) {
            if (positioning) fail("duplicate DoeDoPositioning");
            positioning = child;
        } else if (child->tag == L("DoeRoiType")) {
            if (roi) fail("duplicate DoeRoiType");
            roi = child;
        } else {
            fail("input-table child is outside the retained B121 record");
        }
    }
    if (inputTable.attribute(L("Version")) != L("1") || !variables || !operations ||
        !meshPath || !meshName || !positioning || !roi ||
        !meshPath->attribute(L("path")).isEmpty() ||
        !meshName->attribute(L("originalName")).isEmpty() ||
        positioning->attribute(L("bool")) != L("false") ||
        roi->attribute(L("type")) != L("0"))
        fail("header differs from the retained B121 defaults");
    const auto variableNodes = variables->childElements();
    if (variableNodes.size() != 1 || variableNodes.front()->tag != L("DoeVariable"))
        fail("only the single B121 DoeVariable is recovered");

    const xml::Node& source = *variableNodes.front();
    const String name = source.attribute(L("name"));
    bool idOk = false, dimensionOk = false, samplesOk = false;
    const int id = qt::toInt(source.attribute(L("varId")), &idOk);
    const int dimension = qt::toInt(source.attribute(L("dimension")), &dimensionOk);
    const int samples = qt::toInt(source.attribute(L("numSamples")), &samplesOk);
    if (name != L("Transfer Time") || !idOk || id != 1 || !dimensionOk || dimension != 3 ||
        !samplesOk || samples != 3 || !source.attribute(L("LinkTo")).isEmpty())
        fail("variable header is outside the retained Transfer Time record");

    const xml::Node* keyword = nullptr;
    for (const auto* child : source.childElements()) {
        if (child->tag != L("Keyword")) fail("non-Keyword variable child is outside this prefix");
        if (keyword) fail("multiple keywords are outside this prefix");
        keyword = child;
    }
    if (!keyword || keyword->attribute(L("name")) != L("TLOC") ||
        keyword->attribute(L("parmType")) != L("double"))
        fail("expected the retained TLOC double keyword");
    bool unitOk = false, parameterOk = false, nominalOk = false;
    const int unit = qt::toInt(keyword->attribute(L("unitType")), &unitOk);
    const int parameter = qt::toInt(keyword->attribute(L("parameter")), &parameterOk);
    const double nominal = qt::toDouble(keyword->attribute(L("nominalValue")), &nominalOk);
    if (!unitOk || unit != 0 || !parameterOk || parameter != -1 || !nominalOk || nominal != 0.1 ||
        !keyword->attribute(L("parameterName")).isEmpty())
        fail("keyword attributes are outside the retained TLOC record");

    String nominalText;
    std::vector<int> keywordOperationIds;
    bool sawNominal = false, sawKeywordOperations = false;
    for (const auto* child : keyword->childElements()) {
        if (child->tag == L("NominalData")) {
            if (sawNominal) fail("duplicate NominalData");
            sawNominal = true;
            if (child->children.size() == 1 && child->children.front() &&
                child->children.front()->kind == xml::Node::Element &&
                child->children.front()->tag == L("string"))
                nominalText = child->children.front()->text();
        } else if (child->tag == L("KwOperations")) {
            if (sawKeywordOperations) fail("duplicate KwOperations");
            sawKeywordOperations = true;
            for (const auto* operation : child->childElements()) {
                if (operation->tag != L("KwOperation")) fail("unexpected keyword-operation child");
                bool operationIdOk = false;
                const int operationId = qt::toInt(operation->attribute(L("id")), &operationIdOk);
                if (!operationIdOk || operationId != 1) fail("keyword operation is outside B121 id 1");
                keywordOperationIds.push_back(operationId);
            }
        } else {
            fail("keyword child is outside the retained TLOC record");
        }
    }
    if (!sawNominal || nominalText != L("TLOC    \t0.1") || !sawKeywordOperations ||
        keywordOperationIds != std::vector<int>{1})
        fail("nominal text or nested operation map differs from B121");

    std::vector<int> operationIds;
    for (const auto* operation : operations->childElements()) {
        if (operation->tag != L("Operation")) fail("unexpected Operations child");
        bool operationIdOk = false;
        const int operationId = qt::toInt(operation->attribute(L("id")), &operationIdOk);
        if (!operationIdOk || operationId != 1) fail("top-level operation id is outside B121 id 1");
        const auto bindings = operation->childElements();
        if (bindings.size() != 1 || bindings.front()->tag != L("DoeOprVariable") ||
            bindings.front()->attribute(L("name")) != name)
            fail("top-level operation binding differs from Transfer Time id 1");
        operationIds.push_back(operationId);
    }
    if (operationIds != std::vector<int>{1}) fail("expected one top-level operation mapping");

    auto parsed = std::make_unique<Var>();
    parsed->name = name;
    parsed->index = -1;
    parsed->number = id;
    parsed->geoType = 100;
    parsed->ops = operationIds;
    ScalarVariable scalar;
    scalar.id = id;
    scalar.dimension = dimension;
    scalar.sampleCount = samples;
    scalar.keyword.name = L("TLOC");
    scalar.keyword.parameterType = L("double");
    scalar.keyword.unitType = unit;
    scalar.keyword.parameter = parameter;
    scalar.keyword.nominalValue = nominal;
    scalar.keyword.nominalDataText = nominalText;
    scalar.keyword.operationIds = std::move(keywordOperationIds);
    parsed->scalar = std::move(scalar);
    Var* alias = parsed.get();
    s.scalarPrefixVariableOwners.push_back(std::move(parsed));
    s.vars.push_back(alias);
    return true;
}

bool writeB121TlocModelPrefix(const String& path, const Study& s, engine::FileEffects& fx)
{
    // Serialize only the retained one-variable schema. This isolated seam does
    // not relax writeInputTable/saveInputTable or claim DOE Save lifecycle parity.
    auto fail = [](const char* reason) -> void {
        unsupported(std::string("B121 TLOC model writer prefix: ") + reason);
    };
    if (s.vars.size() != 1 || s.vars[0] == nullptr || s.scalarPrefixVariableOwners.size() != 1 ||
        s.vars[0] != s.scalarPrefixVariableOwners[0].get() || !s.vars[0]->scalar)
        fail("requires exactly one scalar variable");
    const Var& variable = *s.vars[0];
    const ScalarVariable& scalar = *variable.scalar;
    const ScalarKeyword& keyword = scalar.keyword;
    if (!s.commonMeshPath.isEmpty() || !s.commonMeshOrigName.isEmpty() || s.doPositioning ||
        s.roiType != 0 || !s.regions.empty() || s.constraints != 0 ||
        !variable.files.empty() || !variable.items.empty() || variable.link != nullptr ||
        variable.name != L("Transfer Time") || variable.number != 1 || scalar.id != 1 ||
        scalar.dimension != 3 || scalar.sampleCount != 3 ||
        keyword.name != L("TLOC") || keyword.parameterType != L("double") ||
        keyword.unitType != 0 || keyword.parameter != -1 || keyword.nominalValue != 0.1 ||
        keyword.nominalDataText != L("TLOC    \t0.1") ||
        keyword.operationIds != std::vector<int>{1} || variable.ops != std::vector<int>{1})
        fail("model fields are outside the retained B121 TLOC record");

    // This one retained row has exact raw golden ordering. Do not route it
    // through the generic XML AttrOrder hash projection: that projection's
    // variable-attribute order differs from this retained 707-byte artifact.
    // This narrow writer is candidate fixture behavior, not a recovered native
    // call-order contract or a general DoeInputTable serializer.
    const std::string text =
        "<!DOCTYPE DoeInputTableFile>\n"
        "<sfDoeInputTable Version=\"1\">\n"
        " <DoeCommonMeshPath path=\"\"/>\n"
        " <DoeCommonMeshOrigName originalName=\"\"/>\n"
        " <DoeDoPositioning bool=\"false\"/>\n"
        " <DoeRoiType type=\"0\"/>\n"
        " <DoeVariables>\n"
        "  <DoeVariable numSamples=\"" + std::to_string(scalar.sampleCount) +
        "\" varId=\"" + std::to_string(scalar.id) +
        "\" dimension=\"" + std::to_string(scalar.dimension) +
        "\" name=\"" + variable.name.utf8() + "\">\n"
        "   <Keyword nominalValue=\"0.1\" unitType=\"" + std::to_string(keyword.unitType) +
        "\" name=\"" + keyword.name.utf8() + "\" parmType=\"" + keyword.parameterType.utf8() +
        "\" parameter=\"" + std::to_string(keyword.parameter) + "\">\n"
        "    <NominalData>\n"
        "     <string>" + keyword.nominalDataText.utf8() + "</string>\n"
        "    </NominalData>\n"
        "    <KwOperations>\n"
        "     <KwOperation id=\"" + std::to_string(keyword.operationIds.front()) + "\"/>\n"
        "    </KwOperations>\n"
        "   </Keyword>\n"
        "  </DoeVariable>\n"
        " </DoeVariables>\n"
        " <Operations>\n"
        "  <Operation id=\"" + std::to_string(variable.ops.front()) + "\">\n"
        "   <DoeOprVariable name=\"" + variable.name.utf8() + "\"/>\n"
        "  </Operation>\n"
        " </Operations>\n"
        "</sfDoeInputTable>\n";
    std::string bytes;
    for (char c : text) {
        if (c == '\n') bytes += '\r';
        bytes += c;
    }
    fx.writes.push_back({path, std::move(bytes)});
    return true;
}

bool loadInputTable(Study& s, const String& dir)
{
    const String sep = String(std::u16string(1, qt::kSep));
    const String path = dir + sep + s.name + sep + L("Control") + sep + L("DoeInputTable.XML");   // @0x142166bd0
    std::string bytes;
    if (!readAll(path, bytes)) return false;            // "Failed to open file, %1"
    return readInputTableFromSource(bytes, s, InputTableSource{path});
}

bool writeParserIni(const String& tempName, const String& mstName, const String& ctrlDir, engine::FileEffects& fx)
{
    const String path = ctrlDir + String(std::u16string(1, qt::kSep)) + L("DEF_DOE_PARSER.INI");   // @0x1427bba00
    // QTextStream << 1 << endl << temp << endl << mst << endl, the locale codec, Text mode
    std::string text = "1\n" + engine::local8(tempName) + "\n" + engine::local8(mstName) + "\n";
    std::string bytes;
    for (char c : text) {
        if (c == '\n') bytes += '\r';
        bytes += c;
    }
    fx.writes.push_back({path, bytes});
    return true;
}

bool writeDefDoeIni(const String& ctrlDir, const String& db, Study& s, bool siesta, bool ansys,
                    engine::FileEffects& fx)
{
    if (s.constraints < 0 || static_cast<std::size_t>(s.constraints) != s.constraintRecords.size() ||
        std::any_of(s.constraintRecords.begin(), s.constraintRecords.end(), [](const auto& record) {
            return !record || record->category < 0 || record->category > 2 || record->value != 0.0;
        }))
        unsupported("DEF_DOE.INI constraint owner/zero-value profile remains unsupported");
    const String sep = String(std::u16string(1, qt::kSep));
    const String path = ctrlDir + sep + L("DEF_DOE.INI");   // @0x1427bb800
    std::string t;                                           // QTextStream: the locale codec
    auto line = [&](const std::string& x) { t += x + "\n"; }; // << ... << endl
    auto str = [&](const String& x) { return engine::local8(x); };
    line("NOMINAL_DB");
    line(str(qt::cleanPath(L("..") + sep + L("..") + sep + L("..") + sep + db)));
    line("OBJ_NO");
    line(std::to_string(s.objNo));
    line("OPR_NO");
    line(std::to_string(s.oprNo));
    if (siesta) line("CONV_SIESTA");
    if (ansys) line("CONV_ANSYS");
    if (!s.doPositioning) line("DO_NOT_POS");
    line("ROI_TYPE");
    line(std::to_string(s.roiType));
    if (!s.commonMeshPath.isEmpty() && s.roiType != 0) {
        line("COM_MESH");
        line(str(qt::cleanPath(s.commonMeshPath)));
    }
    line("DOE_DBS");
    int run = 0;
    for (int n : s.designCounts)                             // each distinct row, count times
        for (int k = 0; k < n; ++k) {
            ++run;
            const String r = L("..") + sep + L("Solutions") + sep + L("Run") + qt::number(run) + sep + db;   // @0x1427baf20, 0x1427ba444
            line(str(qt::cleanPath(r)));
        }
    if (!s.constraintRecords.empty()) {
        t += '\n';
        line("NUM_CON\t\t" + std::to_string(s.constraints)); // 0x140913eae
        const auto row = [&](std::size_t number, const ConstraintRecord& record, const std::string& value) {
            line("  " + std::to_string(number) + ' ' + std::to_string(record.id) + "    " +
                std::to_string(record.objectNo) + "      " + value + "    " + std::to_string(record.regionNo));
        };
        std::size_t following = 1;
        for (std::size_t i = 0; i < s.constraintRecords.size(); ++i) {
            const auto& record = *s.constraintRecords[i];
            if (record.category == 2) {
                // 913fb7: this pass prints original index+1, not the counter.
                row(i + 1, record, qt::formatDouble(record.value));
                ++following;
            }
        }
        for (const auto& record : s.constraintRecords) {
            if (record->category == 0 || record->category == 1) {
                // 914318: category1 ignores Value and emits the literal "0.0".
                row(following++, *record, record->category == 1 ? "0.0" : qt::formatDouble(record->value));
            }
        }
    }
    if (!s.regions.empty()) {
        t += "\n";                                           // endl
        line("REGIONS    \t\t" + std::to_string(s.regions.size()));   // @0x1427bb8f0
        for (const String& n : s.regions)
            line(str(L("Regions") + L("/") + fileNameOf(n) + L(".GEO")));   // @0x1427bb910 / 908 / 900
    }
    std::string bytes;                                       // Text mode
    for (char c : t) {
        if (c == '\n') bytes += '\r';
        bytes += c;
    }
    fx.writes.push_back({path, bytes});
    return true;
}

void keywordValues(int id, int objNo, int dim, const std::vector<int>& ids, const String& nominal, const String& upper,
                   const String& lower, const String& projectFolder, engine::FileEffects& fx,
                   std::unique_ptr<def::model::DYNARY>* out)
{
    (void)objNo;
    (void)ids;
    const int group = def::model::keyword_group(id);        // 0x140922090 = 1402f7880
    if (group < 1 || group > 5)
        unsupported("temp.KWD keyword " + std::to_string(id) + " of group " + std::to_string(group) +
                    ": 0x140916530 parses groups 1..5 only and reads an uninitialised status otherwise");
    if (const def::model::KeywordRecord* r = def::model::find_keyword(id); !r || r->handler3 != def::model::kDefaultHandler3)
        unsupported("temp.KWD keyword " + std::to_string(id) +
                    ": its registry handler3 (0x140922280 -> 1402f7a70) is not wired here");
    const String target = projectFolder + String(std::u16string(1, qt::kSep)) + L("temp.KWD");   // @0x1427bba28
    const String* texts[3] = {&nominal, &upper, &lower};
    std::vector<std::unique_ptr<def::model::DB_STEP_DATA>> models;
    for (const String* text : texts) {
        // QFile("temp.KWD") WriteOnly (no Text), QTextStream << text: the locale codec
        const std::string bytes = engine::local8(*text);
        fx.writes.push_back({target, bytes});
        auto m = std::make_unique<def::model::DB_STEP_DATA>(dim);   // 0x140821bf0
        const std::filesystem::path tmp = std::filesystem::temp_directory_path() / L"m2m_temp_kwd.tmp";
        { std::ofstream(tmp, std::ios::binary) << bytes; }
        std::FILE* f = _wfopen(tmp.wstring().c_str(), L"r");   // fopen("temp.KWD", "r")
        if (!f) return;
        char name[40] = {};
        int rc = 0;
        if (std::fscanf(f, "%39s", name) == 1) {           // 0x141472ac8
            def::cmd::ModelKeyHooks hooks(m.get());
            rc = hooks.parse_keyword(group, f, id);          // 140b69090 .. 140b60970
        }
        std::fclose(f);                                      // 0x14146fb28
        std::error_code ec;
        std::filesystem::remove(tmp, ec);
        models.push_back(std::move(m));
        if (rc != 0) return;                                 // the next text is not written
    }
    // handler3 1402f7610 returns -1 without a DYNARY: nothing is copied (0x140898f40 not reached)
    for (int k = 0; k < 3; ++k) out[k].reset();
}

xml::NodePtr studyElement(const Study& s)
{
    xml::NodePtr e = xml::createElement(L("SfDoeStudy"));   // @0x142166b58
    xml::setAttribute(*e, L("name"), s.name);               // +0x98
    return e;
}

void readStudyElement(Study& s, const xml::Node& e)
{
    s.name = e.attribute(L("name"), String());
}

}  // namespace m2m::doe
