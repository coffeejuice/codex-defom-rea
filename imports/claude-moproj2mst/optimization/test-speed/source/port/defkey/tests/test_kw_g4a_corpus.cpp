// Corpus parse check of the kw-g4a readers: every FRAE2H, FPERV, FRCMOD, FSTRES, CREEP, ANISO,
// BURGRS, ALPHA, NDISFM and RECVRY block in the corpus KEY files must read without error and
// consume exactly its lines (the next token is a keyword name, not a number left over).
// ctest-labels: kw-g4a keywords corpus
//
// Corpus: DEF_CORPUS_DIR, default C:\Users\alext\Projects\DEFORM_corpus (key/ and materials/).
// Each file is copied to a temp file first (corpus files are only read through copies). The
// test passes with a note when the corpus is not on this machine.
#include <defkey/kw_g4a.hpp>

#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/material.hpp>
#include <testkit/check.hpp>

#include "key_test_util.hpp"

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <memory>
#include <new>
#include <string>
#include <vector>

using namespace def::key::g4a;
using def::model::DYNARY;
using def::model::Material;
namespace fs = std::filesystem;

namespace {

struct Mat {
    Material* m;
    Mat(std::int32_t n)
    {
        void* raw = ::operator new(sizeof(Material));
        std::memset(raw, 0, sizeof(Material));
        m = static_cast<Material*>(raw);
        new (&m->fstres) def::model::Cls_140288ae0();
        new (&m->creep) def::model::Cls_140289b80();
        new (&m->disloc) def::model::Cls_14028bcc0();
        new (&m->frcmod) def::model::Cls_14028c040();
        new (&m->aniso) def::model::Cls_140289fe0();
        new (&m->fperv1) DYNARY();
        new (&m->fperv2) DYNARY();
        m->frae2h = 0.9;
        m->number = n;
    }
    ~Mat()
    {
        m->fstres.~Cls_140288ae0();
        m->creep.~Cls_140289b80();
        m->disloc.~Cls_14028bcc0();
        m->frcmod.~Cls_14028c040();
        m->aniso.~Cls_140289fe0();
        m->fperv1.~DYNARY();
        m->fperv2.~DYNARY();
        ::operator delete(static_cast<void*>(m));
    }
};

struct Kw {
    const char* name;
    int (*read)(std::FILE*, Material&);
};
const Kw kMine[] = {
    {"FRAE2H", read_frae2h}, {"FPERV", read_fperv}, {"FRCMOD", read_frcmod}, {"FSTRES", read_fstres},
    {"CREEP", read_creep},   {"ANISO", read_aniso}, {"BURGRS", read_burgrs}, {"ALPHA", read_alpha},
    {"NDISFM", read_ndisfm}, {"RECVRY", read_recvry},
};

struct Stats {
    int files = 0, blocks = 0, errors = 0, desync = 0;
};

void scan(const fs::path& path, Stats& st)
{
    const fs::path tmp = fs::temp_directory_path() / keytest::tmp_name("kw_g4a_corpus.key");
    fs::copy_file(path, tmp, fs::copy_options::overwrite_existing);
    std::FILE* fp = std::fopen(tmp.string().c_str(), "r");
    if (fp == nullptr) return;
    ++st.files;
    std::vector<std::unique_ptr<Mat>> mats;
    char tok[40];
    char rest[0x400];
    bool after_mine = false;
    while (std::fscanf(fp, "%39s", tok) == 1) {
        if (after_mine) {
            after_mine = false;
            if (!std::isalpha(static_cast<unsigned char>(tok[0])) && tok[0] != '*') {
                ++st.desync;
                std::printf("desync in %s before token %s\n", path.string().c_str(), tok);
            }
        }
        const Kw* kw = nullptr;
        for (const Kw& k : kMine)
            if (std::strcmp(k.name, tok) == 0) kw = &k;
        // a line that starts with one of our keywords; anything else: skip the line
        if (kw == nullptr) {
            std::fgets(rest, sizeof rest, fp);
            continue;
        }
        std::int32_t n = 0;
        if (std::fscanf(fp, "%i", &n) == -1) break;
        Material* m = nullptr;
        for (auto& x : mats)
            if (x->m->number == n) m = x->m;
        if (m == nullptr) {
            mats.push_back(std::make_unique<Mat>(n));
            m = mats.back()->m;
        }
        ++st.blocks;
        if (kw->read(fp, *m) != 0) {
            ++st.errors;
            std::printf("read error %s in %s\n", tok, path.string().c_str());
        }
        after_mine = true;
    }
    std::fclose(fp);
    fs::remove(tmp);
}

} // namespace

TEST_CASE(corpus_blocks_read_exactly)
{
    const char* env = std::getenv("DEF_CORPUS_DIR");
    const fs::path root = env ? fs::path(env) : fs::path("C:/Users/alext/Projects/DEFORM_corpus");
    if (!fs::exists(root / "key")) {
        std::printf("corpus not found at %s: skipped\n", root.string().c_str());
        return;
    }
    Stats st;
    for (const char* sub : {"key", "materials"})
        for (const auto& e : fs::recursive_directory_iterator(root / sub)) {
            if (!e.is_regular_file()) continue;
            std::string ext = e.path().extension().string();
            for (char& c : ext) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
            if (ext == ".KEY") scan(e.path(), st);
        }
    std::printf("%d files, %d kw-g4a blocks, %d read errors, %d desyncs\n", st.files, st.blocks, st.errors,
                st.desync);
    CHECK_EQ(st.files, 942);
    CHECK_EQ(st.errors, 0);
    CHECK_EQ(st.desync, 0);
    CHECK(st.blocks > 6000);
}

int main()
{
    keytest::legacy_printf();  // what def::rt::init_stdio_compat() sets in def_pre
    return testkit::run_all();
}
