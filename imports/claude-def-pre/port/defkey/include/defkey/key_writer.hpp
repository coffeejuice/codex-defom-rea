// defkey/key_writer.hpp: the pieces every KEY writer shares (FUN_1401c5af0 driver,
// FUN_1401c5a20 banner, FUN_1403d4990 and the other per-group writers).
// Spec: docs/re/keywords/_common.md section 5.
//
// Writer order is plain code in the binary: section banners, then per-keyword writers called in
// a fixed order; the first non-zero status aborts the whole KEY write and is returned. WriteChain
// gives the ported writers that rule in one place:
//
//     WriteChain w(f);
//     w.section("Process Definition");
//     w.step([&] { return write_title(f, m); });
//     ...
//     return w.status();
//
// Keyword names are printed with "%-6s" from keyword_label(id) (registry lookup, 1402f7720).
#pragma once

#include <cstdio>
#include <utility>

namespace def::key {

// "*\n*  %s\n*\n", the section banner every writer uses
void write_section_banner(std::FILE* f, const char* title);

// 1401c5a20: the file banner. dim_option == 0: "%s V%s KEYWORD FILE (Qt)" with the product name
// (FUN_140224dd0(FUN_140158e90(model))) and version (FUN_140225450); otherwise
// "DEFORM-%dD V%s KEYWORD FILE (Qt)". Printed as a section banner. The caller supplies the two
// strings until the model code that computes them is ported.
void write_file_banner(std::FILE* f, unsigned dim_option, const char* product, const char* version);

// Registry name of keyword `id` for "%-6s" (def::model::keyword_name, one static buffer).
const char* keyword_label(int id);

class WriteChain {
public:
    explicit WriteChain(std::FILE* f) : f_(f) {}
    // Section banner, skipped once a step failed.
    WriteChain& section(const char* title)
    {
        if (status_ == 0) write_section_banner(f_, title);
        return *this;
    }
    // Runs `fn` (returning int status) unless an earlier step failed.
    template <class Fn>
    WriteChain& step(Fn&& fn)
    {
        if (status_ == 0) status_ = std::forward<Fn>(fn)();
        return *this;
    }
    int status() const { return status_; }

private:
    std::FILE* f_;
    int status_ = 0;
};

} // namespace def::key
