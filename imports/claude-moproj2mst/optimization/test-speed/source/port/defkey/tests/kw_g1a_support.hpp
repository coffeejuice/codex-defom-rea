// kw-g1a tier-1 test support: a step model on zeroed storage with the group 1 defaults, hooks
// that run the shared KEY loop (read_key_stream) on it, and text capture of the writer.
//
// The DB_STEP_DATA / sim-control constructors are not linked into this branch yet, so the model
// is zeroed storage and the defaults below are set by hand. Each value is the documented default
// (docs/re/keywords/<KW>.md "Default", set by the ctor chain 1401c8950 -> 1401e7c50) and agrees
// with DEF_PRE's KFWRIT of an empty KEY file.
#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <defrt/stdlib/stdio_compat_test.hpp>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>

#include <defkey/group1.hpp>
#include <defkey/key_reader.hpp>
#include <defkey/kw_g1a.hpp>
#include <defmodel/dynary_init.hpp>
#include <defmodel/keyword_registry.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <testkit/check.hpp>

namespace kwg1a_test {

using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;

// DEF_PRE's CRT printf settings (def::rt::init_stdio_compat) run before main via
// <defrt/stdlib/stdio_compat_test.hpp>; the kw-g1a writer itself formats doubles with
// def::rt::crt::fprintf.

// Zeroed storage that is used as a DB_STEP_DATA (its ctor is another packet's).
struct Model {
    DB_STEP_DATA* m;
    LLOBJ_DATA* obj = nullptr;

    explicit Model(int dim)
    {
        m = static_cast<DB_STEP_DATA*>(std::calloc(1, sizeof(DB_STEP_DATA)));
        m->dimension = dim;
        set_defaults(dim);
    }
    ~Model()
    {
        auto& s = m->sim_ctrl;
        for (auto* t : {&s.dttype, &s.vfactr, &s.envtmp, &s.cnvcof, &s.envatm, &s.acvcof})
            def::model::dynary_free_arrays(t);
        std::free(obj);
        std::free(m);
    }
    // DB_STEP_DATA slot 0 (1401c8ba0): a new dimension re-initialises the whole model.
    void set_dimension(int dim)
    {
        if (dim == m->dimension) return;
        auto& s = m->sim_ctrl;
        for (auto* t : {&s.dttype, &s.vfactr, &s.envtmp, &s.cnvcof, &s.envatm, &s.acvcof})
            def::model::dynary_free_arrays(t);
        std::memset(static_cast<void*>(m), 0, sizeof(DB_STEP_DATA));
        m->dimension = dim;
        set_defaults(dim);
    }
    Model(const Model&) = delete;
    Model& operator=(const Model&) = delete;

    void set_defaults(int dim)
    {
        auto& s = m->sim_ctrl;
        std::strcpy(s.title, "SIMULATION");
        std::strcpy(s.oprnam, "OPERATION 1");
        std::strcpy(s.simnam, "SIMULATION 1");
        s.cursim[0] = 1;
        s.meshno = 1;
        s.smode = 1;
        s.stype = 1;
        s.geotyp = 1;
        s.trans[1] = 1;
        s.stnout[4] = 1;
        s.nstart = -1;
        s.stpinc = 10;
        s.nstep = 1;
        s.stpdef = 1;
        def::model::dynary_init(&s.dttype, 0x80);
        s.dttype.u58 = 1;  // 1401ee510: DSMAX 0 -> flag 1
        s.demax = dim == 2 ? 0.1 : 0.0;
        s.dtsub = dim == 3 ? 1.0 : 0.0;
        s.dplen = 0.5;
        def::model::dynary_init(&s.vfactr, dim == 3 ? 0xb5 : 0);
        for (auto* t : {&s.envtmp, &s.cnvcof, &s.envatm, &s.acvcof}) def::model::dynary_init(t, 0);
    }

    // One object in the object list M+0x1380 with id `id` and Z stroke (+0x22e0) `stroke`.
    void add_object(std::int32_t id, double stroke)
    {
        obj = static_cast<LLOBJ_DATA*>(std::calloc(1, 0x3f28));
        std::memcpy(reinterpret_cast<unsigned char*>(obj) + 8, &id, 4);
        std::memcpy(reinterpret_cast<unsigned char*>(obj) + 0x22e0, &stroke, 8);
        m->objects.head = obj;
        m->objects.tail = obj;
        m->objects.count = 1;
    }
};

inline std::string read_file(const std::filesystem::path& p)
{
    std::ifstream in(p, std::ios::binary);
    std::ostringstream s;
    s << in.rdbuf();
    std::string t = s.str();
    std::string out;  // CRLF -> LF; a lone CR is data (a corpus SIMNAM line starts with one)
    for (std::size_t i = 0; i < t.size(); ++i)
        if (!(t[i] == '\r' && i + 1 < t.size() && t[i + 1] == '\n')) out += t[i];
    return out;
}

inline std::string slurp(std::FILE* f)
{
    std::string out;
    std::rewind(f);
    int c;
    while ((c = std::fgetc(f)) != EOF) out += static_cast<char>(c);
    return out;
}

// Hooks for the shared KEY loop (def::key::read_key_stream, FUN_1401b6520): group 1 goes to the
// group 1 parser; with `only_g1a`, group 1 keywords outside kw-g1a's ids and every other group
// are skipped with the rest of their line (their parsers are other packets'; the test inputs
// keep such keywords on one line). Dimension comments re-initialise the test model.
class Hooks : public def::key::KeyReaderHooks {
public:
    Hooks(Model& mdl, bool only_g1a) : mdl_(mdl), only_g1a_(only_g1a) {}
    bool has_sub_model() override { return false; }
    void set_model_dimension(int dim) override { mdl_.set_dimension(dim); }
    void set_sub_model_dimension(int) override {}
    int parse_keyword(int group, std::FILE* f, int id) override
    {
        bool mine = def::key::g1a::kFirstId <= id && id <= def::key::g1a::kLastId;
        if (group == 1 && (mine || !only_g1a_)) return def::key::read_group1(mdl_.m, f, id);
        char line[0x400];
        std::fgets(line, 0x400, f);
        return 0;
    }

private:
    Model& mdl_;
    bool only_g1a_;
};

// Reads the KEY stream `f` into the model; returns the loop's messages (stdout in the original).
inline std::string run_key(Model& mdl, std::FILE* f, bool only_g1a = true)
{
    Hooks hooks(mdl, only_g1a);
    std::FILE* out = std::tmpfile();
    def::key::read_key_stream(f, hooks, out);
    std::string msgs = slurp(out);
    std::fclose(out);
    return msgs;
}

// Only the "ERROR: Keyword" / "WARNING: Keyword" lines, as make_expected.py keeps them.
inline std::string message_lines(const std::string& msgs)
{
    std::istringstream in(msgs);
    std::string ln, out;
    while (std::getline(in, ln)) {
        if (ln.rfind("ERROR: Keyword", 0) == 0 || ln.rfind("WARNING: Keyword", 0) == 0) out += ln + "\n";
    }
    return out;
}

inline std::string write_sections(DB_STEP_DATA* m, int* rc = nullptr)
{
    std::FILE* f = std::tmpfile();
    int r = def::key::g1a::write_sections(f, *m);
    if (rc != nullptr) *rc = r;
    std::string s = slurp(f);
    std::fclose(f);
    return s;
}

// Reads `text` as a KEY file into the model; returns the messages.
inline std::string read_text(Model& mdl, const std::string& text)
{
    std::FILE* f = std::tmpfile();
    std::fwrite(text.data(), 1, text.size(), f);
    std::rewind(f);
    std::string msgs = run_key(mdl, f);
    std::fclose(f);
    return msgs;
}

inline std::filesystem::path data_dir()
{
    return testkit::repo_root() / "port" / "defkey" / "tests" / "data" / "kw_g1a";
}

} // namespace kwg1a_test
