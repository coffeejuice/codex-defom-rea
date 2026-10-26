// m2m/doe_study.hpp: the DOE study files of File > Save (SfMoDoeManager, MDT L6T; spec
// docs/re/moproj2mst/mdt/L6/doe.md). The study is the manager's data as far as the files read and
// write it; variables and constraints refuse (M2M_E_UNSUPPORTED) until an oracle B case has them.
#pragma once

#include <memory>
#include <array>
#include <optional>
#include <vector>

#include <defmodel/layout/dynary.hpp>

#include "m2m/engine.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/doe_model.hpp"
#include "m2m/xml.hpp"

namespace m2m::doe {
struct SessionDocument;

// 0x140917630 appends a row to the manager's implicitly shared linked list. Equal rows
// remain separate, in insertion order; no sampling or duplicate elimination occurs here.
using SampleTable = std::vector<std::vector<double>>;

struct SampleState {
    int mode = 0;                                      // +0x40, 0x140919b80
    std::array<SampleTable, 3> tables;                  // +0x58
    std::array<bool, 3> invalid{{true, true, true}};     // +0x70
    // Only mode zero's count is initialized by the recovered constructor. Unwritten
    // mode 1/2 counts stay unknown here rather than acquiring invented zero defaults.
    std::array<std::optional<int>, 3> counts{{100, std::nullopt, std::nullopt}}; // +0x48
};

struct Study : Model {
    SubProject* sub = nullptr;        // +0x90 (0x1400b5c40: its project's folder)
    String name;                      // +0x98 (0x1400ab770), "DoeStudy<n>"
    bool doPositioning = false;       // +0x78 (0x1400bc690)
    int roiType = 0;                  // +0x7c (0x1408e0550)
    String commonMeshPath;            // +0x80 (0x1400bc640)
    String commonMeshOrigName;        // +0x88 (0x140917060)
    std::vector<String> regions;      // vt+0xc0 0x1400d7ee0 count, vt+0xd8 0x14091f380 name
    int constraints = 0;              // vt+0x28 0x140159d00 count
    int objNo = 0;                    // 0x14091e0f0
    int oprNo = 0;                    // 0x14091e0b0
    SampleState samples;
    std::shared_ptr<SessionDocument> outputDocument; // Type2 SP+30 document, lazily created
    // Explicit run multiplicities supplied by the caller of writeDefDoeIni. This legacy
    // seam is not evidence that the manager groups duplicate rows; the sampler is unported.
    std::vector<int> designCounts;

    // @src 0x14091ba60: the recovered empty-variable branch only.
    std::vector<Var*> geometryVars(int opId, int objId) const override;
};

// @src 0x1400d1180: name/owner/defaults, then 0x140909660; directory failure does
// not prevent manager construction. A null sink performs no I/O (legacy direct callers).
std::shared_ptr<Study> makeStudy(SubProject& sub, StudyIo* io);

// @src 0x1409157c0 after QFile open: already-decoded QTextStream text, split on
// QRegExp("\\s+"), SkipEmptyParts. Clears rows, optionally replaces mode from the first
// line; ignores row identifiers and drops lines with no value columns. Numeric failures
// become zero. No mode-range validation in this low-level parser, as in the original.
// Shipped regex whitespace excludes U+0085 (wave2-doe-regexp-space-contract.json).
// Qt numeric conversion edge-case/locale equivalence remains unverified at this seam.
void parseSampleText(const String& text, int& mode, SampleTable& rows);
// @src 0x140915360: mode header, one-based row ids, tab after each value, Text-mode CRLF.
// These FileEffects helpers plan writes; the engine applies real open/write failures.
bool writeSampleTable(int mode, const SampleTable& rows, const String& path, engine::FileEffects& fx);
// @src 0x1400d7790 / 0x1400d75a0: invalid tables are not written.
bool saveSampleTable(const Study& s, int mode, const String& path, engine::FileEffects& fx);
bool saveCurrentSamples(const Study& s, const String& doeStudiesDir, engine::FileEffects& fx);
// @src 0x1400d7a80: despite its load-like caller, writes an EMPTY table. When the
// requested table was nonempty, clears the CURRENT table and validates the REQUESTED
// mode. Keeps those original bugs; no file is read. Effects represent a successful open.
bool loadSampleTable(Study& s, int mode, const String& path, engine::FileEffects& fx);
// @src 0x1400d7820 after successful read: commit the parsed current mode/table/count.
// Out-of-range mode would index outside the original manager; that undefined branch refuses.
void loadCurrentSampleText(Study& s, const String& text);

// @src 0x140819600  file-name form of a name: '-' -> '_' everywhere when it starts with '-', then
// each of * # ! @ $ space ' " \ / < > : ; . , | ? ( ) { } [ ] % ^ = & -> '_'
String fileNameOf(const String& s);

// @src 0x1400d5970 / 0x1400d5ee0 / 0x1400d6450  <project dir>\DoeStudies\<name>\Control\
// fileNameOf("%1_%2_%3_Nominal|Upper|Lower".arg(b).arg(a).arg(id)) + ".GEO"
enum class GeoKind { Nominal, Upper, Lower };
String geoPath(const Study& s, GeoKind k, int id, const String& a, const String& b);

// @src 0x140909660  mkpath of the study folder and its Control, Solutions and Output folders;
// false at the first one that fails (here: recorded in fx.mkdirs, never fails)
bool makeStudyDirs(const String& dir, engine::FileEffects& fx);
// @src 0x140909660: actual/simulated results, stops at the first false; distinct
// from 0x14081afb0 (which always returns true). The constructor ignores this result.
bool makeStudyDirs(const String& dir, StudyIo& io);

// @src 0x1400d69c0  the variable's three geometry files and their .COR files removed, after
// vt+0x100 (0x14091a090, remove variable b)
void removeVarFiles(Study& s, int id, const String& a, const String& b, engine::FileEffects& fx);
// @src 0x1400d6dd0  the three geometry files and their .COR files renamed old -> new; the Nominal
// .COR rename renames the file to itself (as in the original); with a variable, its file names
// follow (0x1408179d0, 0x140817af0)
void renameVarFiles(Study& s, Var* var, int id, const String& a, const String& oldName, const String& newName,
                    engine::FileEffects& fx);

// @src 0x14090a550  DoeInputTable.XML (QFile WriteOnly | Text, QTextStream, toString(1))
bool writeInputTable(const String& path, Study& s, engine::FileEffects& fx);
// @src 0x1400d3200  <dir>\<name>\Control\DoeInputTable.XML (0x1400d2b60 first; the refresh
// signal 0x1402cf3c0 is UI)
bool saveInputTable(Study& s, const String& dir, engine::FileEffects& fx);
// @src 0x14090e080  read DoeInputTable.XML into the study (header elements; variables refuse)
bool readInputTable(const std::string& bytes, Study& s);
// @src 0x1400d3970  <dir>\<name>\Control\DoeInputTable.XML read
bool loadInputTable(Study& s, const String& dir);

// @src 0x140915eb0  <ctrlDir>\DEF_DOE_PARSER.INI: "1", the MST.TEMP name, the MST name, one per
// line (QFile WriteOnly | Text, QTextStream with endl)
bool writeParserIni(const String& tempName, const String& mstName, const String& ctrlDir, engine::FileEffects& fx);

// @src 0x140913270  <ctrlDir>\DEF_DOE.INI (QFile WriteOnly | Text): NOMINAL_DB, OBJ_NO, OPR_NO,
// CONV_SIESTA / CONV_ANSYS, DO_NOT_POS, ROI_TYPE, COM_MESH, DOE_DBS with one ../Solutions/Run<k>/<db>
// line per sample row, then constraints (refused) and regions
bool writeDefDoeIni(const String& ctrlDir, const String& db, Study& s, bool siesta, bool ansys,
                    engine::FileEffects& fx);

// @src 0x140916530  a DOE variable's keyword text (nominal, upper, lower) through temp.KWD in the
// project folder (QFile WriteOnly, written before each parse; the last text stays on disk) and the
// KEY group reader into a scratch model of dimension dim; when all three parse, the keyword's data
// (registry handler3 via 0x140922280) is copied out (null when the handler has none). Refuses a
// keyword outside groups 1..5 (the original then reads an uninitialised status) and a handler3
// that is not the default stub.
void keywordValues(int id, int objNo, int dim, const std::vector<int>& ids, const String& nominal, const String& upper,
                   const String& lower, const String& projectFolder, engine::FileEffects& fx,
                   std::unique_ptr<def::model::DYNARY>* out);

// @src 0x1400d29d0  <SfDoeStudy name="..."/> of the .moproj
xml::NodePtr studyElement(const Study& s);
// @src 0x1400d2aa0  name from that element
void readStudyElement(Study& s, const xml::Node& e);

}  // namespace m2m::doe
