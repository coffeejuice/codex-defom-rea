// defkey/key_reader.hpp: the KEY interpreter loop (FUN_1401b6520), with the model side behind
// hooks so that the loop can be used and tested before the model classes are ported.
// Spec: docs/re/keywords/_common.md section 1.
//
// Loop, until fscanf("%39s") hits EOF:
//   * an empty token or one starting with '*' is a comment: fgets(0x400) reads the rest of the
//     line. Until the dimension has been set once, a non-empty comment line is scanned as
//     "%s %s": "DEFORM 3D|F3", "DEFORM-3D|-F3" set model 3 / sub-model 2; "DEFORM 2D|F2",
//     "DEFORM-2D|-F2", "DEFORM-MACH2", "DEFORM-DIESTRESS2", "DEFORM-MACHDIST2",
//     "DEFORM-MICROSTRUCTURE2" set model 2 / sub-model 3 (sub-model only if it exists);
//   * an unknown name: fgets the rest of the line and print the warning (not for SRFNRG);
//   * otherwise the group parser runs (group 1..7; any other group gives 3). Its status:
//     0 next keyword; 2 print "ERROR: Keyword %s Reading Error. \n" and continue; 3 print
//     "ERROR: Keyword %s Data Error. \n" and continue; anything else is returned at once.
// At EOF it returns the last non-zero status seen (2 or 3), or 0: a file with errors reads to
// the end but still reports the error to the caller.
#pragma once

#include <cstdio>

namespace def::key {

class KeyReaderHooks {
public:
    virtual ~KeyReaderHooks() = default;
    // FUN_1401d8120(model) != NULL, asked once before the loop
    virtual bool has_sub_model() = 0;
    // vtable slot 0 of the model / of the sub-model, called with the dimension (2 or 3)
    virtual void set_model_dimension(int dim) = 0;
    virtual void set_sub_model_dimension(int dim) = 0;
    // The group parser for registry group 1..7 (FUN_14039aa90, 14038b8a0, 140388fb0, 140384a50,
    // 140383120, 140378e00 on FUN_1401d81b0(model); 14037e530 on the model). The keyword token is
    // consumed, so the parser starts on the rest of its line. Returns 0, 2, 3 or a fatal code.
    virtual int parse_keyword(int group, std::FILE* f, int id) = 0;
};

// @src 1401b6520. Messages go to `out` (the original writes to stdout).
int read_key_stream(std::FILE* f, KeyReaderHooks& hooks, std::FILE* out = stdout);

} // namespace def::key
