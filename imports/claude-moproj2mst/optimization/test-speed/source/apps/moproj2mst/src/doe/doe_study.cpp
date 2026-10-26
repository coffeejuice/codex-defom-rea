// SfMoDoeManager: the DOE study files of File > Save (MDT L6T). Decompiles
// gui_mo/s1/mdt/DOE/d0.c .. d2.c; spec docs/re/moproj2mst/mdt/L6/doe.md. Oracle B 120_B_DOE (a
// study without variables). Variables, regions with a reader and constraints refuse until an
// oracle case has them.
#include "m2m/doe_study.hpp"

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

bool writeSampleTable(int mode, const SampleTable& rows, const String& path, engine::FileEffects& fx)
{
    std::string bytes = qt::formatInt(mode) + "\r\n";
    int rowNo = 0;
    for (const auto& row : rows) {
        bytes += qt::formatInt(++rowNo) + '\t';
        for (double value : row) bytes += qt::formatDouble(value) + '\t';
        bytes += "\r\n";
    }
    fx.writes.push_back({path, bytes});
    return true;
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

void removeVarFiles(Study& s, int id, const String& a, const String& b, engine::FileEffects& fx)
{
    // vt+0x100 0x14091a090: the variable named b leaves the study
    for (auto it = s.vars.begin(); it != s.vars.end(); ++it)
        if (*it && (*it)->name == b) {
            s.vars.erase(it);
            break;
        }
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

bool readInputTable(const std::string& bytes, Study& s)
{
    xml::Document doc;
    String err;
    int line = 0, col = 0;
    if (!xml::setContent(doc, bytes, &err, &line, &col) || !doc.root) return false;
    bool status = false;                                  // 0x14090e080, overwritten by child readers
    for (const xml::Node* e : doc.root->childElements()) {
        const String& t = e->tag;
        if (t == L("DoeVariables")) {
            if (!e->childElements().empty()) unsupported("DoeVariables reader 0x14090ee20 has no oracle case yet");
            if (!s.vars.empty()) unsupported("empty DoeVariables with existing variables needs index finalization");
            // 0x14090ee20 returns true; vt+1e0 reindexes existing vars (none here).
            status = true;
        } else if (t == L("Operations")) {
            if (!e->childElements().empty()) unsupported("Operations reader 0x140912a20 has no oracle case yet");
            status = true;
        }
        if (t == L("RegionsOfInterest")) {
            s.regions.clear();                             // 0x140912b00 vt+b8 before children
            if (!e->childElements().empty()) unsupported("RegionsOfInterest reader 0x140912b00 has no oracle case yet");
            status = true;
        }
        if (t == L("Constraints")) {
            s.constraints = 0;                             // 0x140912d10 vt+20 before children
            if (!e->childElements().empty()) unsupported("Constraints reader 0x140912d10 has no oracle case yet");
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
    return status;                                        // headers alone do not make this true
}

bool loadInputTable(Study& s, const String& dir)
{
    const String sep = String(std::u16string(1, qt::kSep));
    const String path = dir + sep + s.name + sep + L("Control") + sep + L("DoeInputTable.XML");   // @0x142166bd0
    std::string bytes;
    if (!readAll(path, bytes)) return false;            // "Failed to open file, %1"
    return readInputTable(bytes, s);
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
    if (s.constraints > 0)
        unsupported("DEF_DOE.INI constraints (0x140913270 NUM_CON part) have no oracle case yet");
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
