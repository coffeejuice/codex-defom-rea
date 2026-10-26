// Helpers for the kw-g57 tier-1 tests: stand-in models (primary and DIM companion), the KEY loop
// hooks for groups 5 and 7, and the Inter-Material section of the KEY writer.
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
#include <defkey/kw_g57.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/phase/phase_ops.hpp>
#include <testkit/check.hpp>

namespace g57test {

using def::model::DB_STEP_DATA;

// DB_STEP_DATA's ctor belongs to p1-03 and is not merged here. Groups 5 and 7 only touch the
// dimension (+0x1c), the phase-pair list (+0x1358), the active-pair count (+0x13f4) and the DIM
// fields (+0x33e8, +0x33f0), so the tests use zeroed storage with just those set up.
class FakeModel {
public:
    explicit FakeModel(int dim)
        : buf_(new std::max_align_t[sizeof(DB_STEP_DATA) / sizeof(std::max_align_t) + 1])
    {
        std::memset(buf_.get(), 0, sizeof(DB_STEP_DATA));
        m_ = reinterpret_cast<DB_STEP_DATA*>(buf_.get());
        m_->dimension = dim;
        ::new (&m_->phase_pairs) def::model::PhasePairList(m_); // 140339c90 (p1-10)
    }
    ~FakeModel() { m_->phase_pairs.clear(); }
    DB_STEP_DATA* get() const { return m_; }

private:
    std::unique_ptr<std::max_align_t[]> buf_;
    DB_STEP_DATA* m_ = nullptr;
};

// DEF_PRE's main object: a primary model and its companion of the other dimension (14001c770).
struct ModelPair {
    FakeModel primary{3};
    FakeModel companion{2};
    ModelPair() { primary.get()->companion = companion.get(); }
};

struct Hooks : def::key::KeyReaderHooks {
    DB_STEP_DATA* m;
    explicit Hooks(DB_STEP_DATA* model) : m(model) {}
    bool has_sub_model() override { return def::model::companion_model(m) != nullptr; }
    void set_model_dimension(int d) override { m->dimension = d; }
    void set_sub_model_dimension(int d) override { m->companion->dimension = d; }
    int parse_keyword(int group, std::FILE* f, int id) override
    {
        if (group == 5) return def::key::g57::read_phase_keyword(def::model::keyword_target_model(m), f, id);
        if (group == 7) return def::key::g57::read_dim_keyword(m, f, id);
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
    return (testkit::repo_root() / "port" / "defkey" / "tests" / "data" / "kw_g57" / name).string();
}

// KFREAD into `models`, return what the loop printed.
inline int read_into(ModelPair& models, const std::string& key_text, const char* tag, std::string* messages)
{
    keytest::legacy_printf();
    keytest::InFile in((std::string(tag) + "_in.txt").c_str(), key_text);
    keytest::OutFile msg((std::string(tag) + "_msg.txt").c_str());
    Hooks hooks(models.primary.get());
    const int st = def::key::read_key_stream(in.get(), hooks, msg.get());
    if (messages != nullptr) *messages = msg.text();
    return st;
}

// The Inter-Material section of KFWRIT (FUN_1401c5af0) for model `m`.
inline std::string write_section(DB_STEP_DATA* m, const char* tag, int* status = nullptr)
{
    keytest::legacy_printf();
    keytest::OutFile out((std::string(tag) + "_out.txt").c_str());
    def::key::write_section_banner(out.get(), "Inter-Material Data");
    int r = 0;
    for (int i = 0; i < def::model::phase_pair_count(&m->phase_pairs) && r == 0; i = i + 1) {
        r = def::key::g57::write_phase_pair(m, out.get(), def::model::phase_pair_nth(&m->phase_pairs, i + 1), i + 1);
    }
    if (status != nullptr) *status = r;
    return out.text();
}

} // namespace g57test
