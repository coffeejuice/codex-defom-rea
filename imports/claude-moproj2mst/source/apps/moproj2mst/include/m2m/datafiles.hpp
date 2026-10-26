// m2m/datafiles.hpp: data files read at open through PROPS->vt[0xc0] (seam; owner P06).
// s1-01 6.4, s1-03 5, s1-04 1.1 and 2, s1-05 3. Each reader fills the payload declared in
// include/m2m/data/<name>.hpp (P06 creates those headers). Paths are
// convertSeparators(taskDirAbs + sep + name); a file in Project::overlay is read from there.
// A missing file keeps the defaults (the original shows a dialog for Cogging; here: Ctx::warn).
#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

#include "m2m/model.hpp"
#include "m2m/props.hpp"

namespace m2m::datafiles {

// Read a project file: overlay first, then the disk (read-only). False if neither has it.
bool readFileBytes(const Project& p, const String& path, std::string& bytes);

// ---- C stdio in text mode ("rt") as the original's static MSVC CRT reads it -----------------------
// Memory-backed form translates CR LF/Ctrl-Z up front. A borrowed live line reader
// supplies its own text-mode translation and supports only fgets.
class TextFile {
public:
    TextFile() = default;
    explicit TextFile(const std::string& rawBytes);

    // fopen(name, "rt") through a char path: open fails when the path has characters the 8-bit
    // conversion (QString::latin1 / toLocal8Bit) cannot carry, or the file is missing.
    static bool open(const Project& p, const String& path, TextFile& f);
    using LineReader = std::function<bool(std::string&, int)>;
    // Callback/underlying stream must outlive every use of the returned object.
    static TextFile fromLineReader(LineReader);

    bool eof() const;
    int getc();                                   // EOF = -1
    void ungetc();
    // fgets(buf, n, f): at most n-1 bytes, stops after '\n'. False (buf untouched) at EOF.
    bool fgets(std::string& buf, int n);

    std::size_t pos() const;
    const std::string& data() const;

private:
    void requireMemory() const;
    LineReader lineReader_;
    std::string data_;
    std::size_t pos_ = 0;
};

// ---- KEY token reader 0x1409068f0 (twin of DEF_PRE 0x140411ad0) ----------------------------------
// The original keeps the line buffer (2000 bytes), the token table and both counters in process
// globals (0x143dd8660, 0x143dd8250, 0x143dd824c, 0x14386f868); here they are per reader. Every
// caller starts each file with a kNewLine read, so the state of an earlier file never shows.
class TokenReader {
public:
    enum Type : int {
        Bool = 1,      // atol != 0 -> 1 byte (bool / std::uint8_t)
        Upper = 2,     // upper-cased word -> char buffer (std::string here); EOF stores "DONE"
        Text = 3,      // word -> std::string
        Raw = 4,       // the line cut only at CR/LF -> std::string
        Int = 5,       // atol -> std::int32_t
        Int6 = 6,      // atol -> std::int32_t
        Int7 = 7,      // atol -> std::int32_t
        UInt = 8,      // strtol(., 10) -> std::int32_t (integer store)
        Float = 9,     // D -> E, atof -> float
        Double = 10,   // D -> E, strtod -> double
        DToE = 11,     // D -> E in place, nothing stored (out may be null)
        Line = 12,     // the whole next physical line -> std::string; EOF stores "DONE"
    };
    static constexpr int kNewLine = -5000;

    // @src 0x140906740  ctor: mode (+0x00) = 0
    TokenReader();
    // @src 0x140906760  dtor (empty)
    ~TokenReader() = default;

    // @src 0x1409068f0  read(f, type, out, pos). pos kNewLine: first token of the next line that
    // is not empty and does not start with '*' (returns the token count of that line); 0: next
    // token, a new line when used up; k > 0: token k of the current line. Returns 0 / count,
    // -1 (no token k), -2 (end of file). `out` points to the type named above.
    int read(TextFile& f, int type, void* out, int pos = 0);

    std::int32_t mode = 0;             // +0x00: 1 cuts every line at column 63

private:
    char line_[2000] = {};             // 0x143dd8660
    char* tok_[260] = {};              // 0x143dd8250 + 8k (slot 1 = first token)
    int count_ = 0;                    // 0x143dd824c tokens on the current line
    int next_ = 1;                     // 0x14386f868 index of the next token
};

// @src 0x140906880  upper-case s in place (toupper per byte)
char* upperInPlace(char* s);
// @src 0x140906790  replace `from` by `to` in s (exact == 1: that byte only; else both cases)
char* replaceChar(char* s, char from, char to, int exact);

// ---- the readers ------------------------------------------------------------------------------
// @src 0x1407613b0  CogModel defaults (0x1407614e0, 0x1407656d0 -> SfOprCogData ctor 0x140767a20).
// unit = the GUI unit the ctor reads (mainWindow vt+0x260): 1 SI, 2 English. The Cogging class's
// makeProps calls this for CoggingProps::model; readCoggingMdt does it when the model is missing.
void cogModelDefaults(data::CogModel& m, int unit);
// @src 0x140761c90 -> 0x140761d90  Cogging.MDT into the cogging model (defaults 0x1407613b0,
// 0x1407656d0, 0x1407614e0; SfOprCogData ctor 0x140767a20; sections 0x14076a460)
void readCoggingMdt(Project& p, Operation& op, CoggingProps& props, Ctx& ctx);
// @src 0x1407603c0  load Cogging.MDT only when the model exists; returns whether it did
// (not on the open path; kept for completeness)
bool loadCoggingMdtIfModel(Project& p, Operation& op, CoggingProps& props, Ctx& ctx);
// ---- the Cogging.MDT writer of File > Save (packet MDT, docs/re/moproj2mst/mdt/spec.md) ----------
// Text after "* " on line 3 of every MDT: "DEFORM-2D/3D " + 0x1407fb240("11.0"), whose suffix comes
// from the VERSION line of <deformDir>/PACK/pack.dat. deformDir empty: the v11.0.2 installation's
// pack.dat ("VERSION 2"), i.e. "DEFORM-2D/3D 11.0 (Service Pack 2)  ".
std::string mdtHeaderText(const std::u16string& deformDir);
struct SideFile {
    String path;                   // absolute, as the original builds it
    std::string bytes;
};
// @src 0x1400982e0 (cogging part): properties vt+0xb8 of every operation in OprManager row order;
// for SfOprCoggingProperties 0x140760400 -> 0x1407616d0 -> 0x1407617e0. produce: append each MDT
// to *out (throws where the original's output is undefined); otherwise only the writer's effect on
// the process globals (NoDies) is applied, as the MST written next sees it.
void saveCoggingMdts(Project& p, const std::string& header, bool produce, std::vector<SideFile>* out);
// One properties vt+b8 call. Native Save opens the actual CRT text file before
// writer-side global changes; open/write failures are ignored by the original
// property loop. Native mode accepts null out and never stages a replay.
bool saveCoggingMdt(Project& p, Operation& op, const std::string& header,
                    bool produce, bool native, std::vector<SideFile>* out);
// @src 0x14003a3c0 / 0x140052ec0 (cogging part): an active cogging operation is loaded into its
// engine model as File > Open does, then saved as step 2 of File > Save does, so its Cogging.MDT
// carries the geometry step 0x14075c260. Nothing for any other active item. Before saveCoggingMdts.
void saveActiveCogging(Project& p, Ctx& ctx);

// @src 0x1411e1cd0 -> 0x1411e1dd0  Boolean.BWZ
void readBooleanBwz(Project& p, Operation& op, BooleanProps& props, Ctx& ctx);
// @src 0x1413f8280 -> 0x1413f83e0  Forming.FWZ
void readExpressFwz(Project& p, Operation& op, ExpressProps& props, Ctx& ctx);
// @src 0x14134ec40 / 0x14134ee90  Cycle.CWZ (WZD_CYCLE_COUNT, KEY tokenizer 0x1409068f0)
void readCycleCwz(Project& p, Operation& op, CycleProps& props, Ctx& ctx);
// @src 0x14029eea0  DieStressBmWizard.Wzd
void readDiestressBmWzd(Project& p, Operation& op, DiestressBMProps& props, Ctx& ctx);
// @src 0x1402a1df0  read a DieStressBmWizard.Wzd into d; false when QFile::open fails (added by
// MDT L6 for the loader's vt+0x78 0x140299a90)
bool readDiestressBmWzdFile(const Project& p, const String& path, data::DiestressBMData& d);

}  // namespace m2m::datafiles
