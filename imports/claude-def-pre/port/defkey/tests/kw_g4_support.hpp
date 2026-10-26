// Test support for the group 4 (kw-g4b) tier-1 tests: a model without its ctor, the KEY loop
// hooks that route group 4 to the parser, and text-mode output (the original's KEY files are
// text-mode FILEs, so every "\n" is written as CR LF).
//
// The two object functions FUN_1401d0fc0 calls (llobj_copy_from 1401613a0, llobj_remap_state
// 140199650) come from defmodel's llobj_ctor port since the KEY merge.
#pragma once

#include "key_test_util.hpp"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <new>
#include <string>

#include <defkey/key_reader.hpp>
#include <defkey/kw_g4.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/material.hpp>
#include <defmodel/port.hpp>


namespace kwg4 {

using def::model::DB_STEP_DATA;
using def::model::Material;

// A DB_STEP_DATA the KEY reader can fill: zeroed storage (its ctor needs packets that are not
// merged here), dimension 3 like the callers' DB_STEP_DATA(3), the material list owner set.
// Materials are never freed (their dtors are fine, but the model has no dtor run either).
class Model {
public:
    explicit Model(int dim = 3)
        : mem_(static_cast<unsigned char*>(::operator new(sizeof(DB_STEP_DATA), std::align_val_t{16})))
    {
        std::memset(mem_, 0, sizeof(DB_STEP_DATA));
        m().dimension = dim;
        m().materials.owner = &m();
    }
    DB_STEP_DATA& m() { return *reinterpret_cast<DB_STEP_DATA*>(mem_); }

private:
    unsigned char* mem_; // leaked on purpose (see above)
};

// The interpreter loop hooks: group 4 goes to the kw-g4b parser, any other group is a test error.
struct Hooks : def::key::KeyReaderHooks {
    explicit Hooks(DB_STEP_DATA& md) : md_(md) {}
    bool has_sub_model() override { return false; }
    void set_model_dimension(int dim) override { md_.dimension = dim; }
    void set_sub_model_dimension(int) override {}
    int parse_keyword(int group, std::FILE* f, int id) override
    {
        if (group != 4) {
            ++other_groups;
            return 3;
        }
        return def::key::g4::read_keyword(md_, f, id);
    }
    int other_groups = 0;

private:
    DB_STEP_DATA& md_;
};

// Bytes of a text-mode output file written by `fn(FILE*)`.
template <class Fn>
std::string text_output(const char* base, Fn fn)
{
    const std::string tmp = keytest::tmp_name(base);  // per process: ctest runs the kw-g4 tests side by side
    const char* name = tmp.c_str();
    std::FILE* f = std::fopen(name, "w");
    fn(f);
    std::fclose(f);
    std::string s;
    std::FILE* r = std::fopen(name, "rb");
    int c;
    while ((c = std::fgetc(r)) != EOF) s += static_cast<char>(c);
    std::fclose(r);
    std::remove(name);
    return s;
}

// Every material block of the model, in list order (as FUN_1401c5af0 calls FUN_1403cdca0).
inline int write_all_materials(std::FILE* f, DB_STEP_DATA& md)
{
    int rc = 0;
    for (Material* mat = md.materials.head; mat != nullptr && rc == 0; mat = mat->next) {
        rc = def::key::g4::write_material(md, f, mat, nullptr);
    }
    return rc;
}

// Reads `key` (LF text) through the KEY loop into `md`; returns the loop status. Messages of the
// loop go to `log` (a scratch file) so that ctest output stays readable.
inline int read_key_text(DB_STEP_DATA& md, const std::string& key, Hooks& hooks, std::FILE* log)
{
    keytest::InFile in("kw_g4_in.KEY", key);
    return def::key::read_key_stream(in.get(), hooks, log);
}

} // namespace kwg4
