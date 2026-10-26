// Helpers for the kw-g3 tier-1 tests: a stand-in model, the KEY loop hooks for group 3 and a
// read-then-write round trip.
#pragma once

#include "key_test_util.hpp"

#include <cstddef>
#include <cstring>
#include <fstream>
#include <memory>
#include <new>
#include <sstream>
#include <string>

#include <defkey/key_reader.hpp>
#include <defkey/key_writer.hpp>
#include <defkey/kw_g3/kw_g3.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <testkit/check.hpp>

namespace g3test {

using def::model::DB_STEP_DATA;

// DB_STEP_DATA's ctor belongs to p1-03 and is not ported yet. The group 3 code only touches the
// dimension (DB_WRITE_VAR +0x1c) and the inter-object list (+0x13c8), so the tests use zeroed
// storage of the right size with just those two members set up.
class FakeModel {
public:
    explicit FakeModel(int dim) : buf_(new std::max_align_t[sizeof(DB_STEP_DATA) / sizeof(std::max_align_t) + 1])
    {
        std::memset(buf_.get(), 0, sizeof(DB_STEP_DATA));
        m_ = reinterpret_cast<DB_STEP_DATA*>(buf_.get());
        m_->dimension = dim;
        ::new (&m_->inter_obj) def::model::Cls_1401fa020(m_); // 1401fa020 (p1-06)
    }
    ~FakeModel() { m_->inter_obj.clear(); }
    DB_STEP_DATA* get() const { return m_; }

private:
    std::unique_ptr<std::max_align_t[]> buf_;
    DB_STEP_DATA* m_ = nullptr;
};

// Loop hooks: the dimension comment sets the model dimension; group 3 goes to the port's parser.
struct Hooks : def::key::KeyReaderHooks {
    DB_STEP_DATA* m;
    explicit Hooks(DB_STEP_DATA* model) : m(model) {}
    bool has_sub_model() override { return false; }
    void set_model_dimension(int d) override { m->dimension = d; }
    void set_sub_model_dimension(int) override {}
    int parse_keyword(int group, std::FILE* f, int id) override
    {
        if (group == 3) return def::key::g3::read_keyword(m, f, id);
        char rest[0x400];
        std::fgets(rest, sizeof rest, f); // other groups are not part of these tests
        return 0;
    }
};

inline std::string strip_cr(const std::string& s)
{
    std::string o;
    for (char c : s)
        if (c != '\r') o += c;
    return o;
}

inline std::string read_file(const std::string& path)
{
    std::ifstream in(path, std::ios::binary);
    std::ostringstream s;
    s << in.rdbuf();
    return s.str();
}

inline std::string data_path(const char* name)
{
    return (testkit::repo_root() / "port" / "defkey" / "tests" / "data" / "kw_g3" / name).string();
}

struct RoundTrip {
    int read_status = 0;
    int write_status = 0;
    std::string messages; // what the KEY loop printed
    std::string written;  // "*\n*  Inter-Object Data\n*\n" + the group 3 records
};

// KFREAD of `key_text` into a fresh model (dimension from the file's banner comment, default 3
// like DB_STEP_DATA's), then the Inter-Object section of KFWRIT.
inline RoundTrip round_trip(const std::string& key_text, const char* tag)
{
    keytest::legacy_printf();
    RoundTrip r;
    FakeModel model(3);
    {
        keytest::InFile in((std::string(tag) + "_in.txt").c_str(), key_text);
        keytest::OutFile msg((std::string(tag) + "_msg.txt").c_str());
        Hooks hooks(model.get());
        r.read_status = def::key::read_key_stream(in.get(), hooks, msg.get());
        r.messages = msg.text();
    }
    keytest::OutFile out((std::string(tag) + "_out.txt").c_str());
    def::key::write_section_banner(out.get(), "Inter-Object Data");
    r.write_status = def::key::g3::write_inter_objects(model.get(), out.get());
    r.written = out.text();
    return r;
}

} // namespace g3test
