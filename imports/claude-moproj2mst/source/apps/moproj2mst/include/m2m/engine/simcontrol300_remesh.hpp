#pragma once
#include "m2m/engine/simcontrol300.hpp"
#include "m2m/datafiles.hpp"
#include <string_view>

namespace m2m::engine::simcontrol300 {

// Original TokenReader4 copies text into the double at+10, not a number.
// Preserve the proven span +10..29, including raw flags and intervening bytes.
// <=25 bytes plus NUL fit; wider corruption remains an explicit unknown.
ApplyResult applyRemeshUniformEdgeToken(RemeshState&, std::string_view);

class RemeshReadFile {
public:
    virtual ~RemeshReadFile() = default;
    // Live or memory fgets input. readRemesh validates each reached visible
    // C-string prefix before tokenization; bytes after NUL still get consumed.
    virtual datafiles::TextFile& tokens() = 0;
    virtual void rewind() = 0;
    virtual void close() = 0;
};
class RemeshIo {
public:
    virtual ~RemeshIo() = default;
    // Original fixed narrow names opened in"rt". Null means original openfail.
    virtual std::unique_ptr<RemeshReadFile> openRead(std::string_view name) = 0;
};
// Qualified ordinary ASCII drive-absolute directory and fixed original names.
// Uses a live narrow CRT text stream; does not read or cache file data at open.
// Resource-exhaustion buffering and nonregular/device paths remain unrecovered.
std::unique_ptr<RemeshIo> makeNativeRemeshIo(std::string absoluteDirectory);
struct RemeshReadReport {
    ApplyResult outcome = ApplyResult::Completed;
    // Engaged only when original reader return is established. False=openfail;
    // true includes readable empty files. Unsupported preserves earlier stores.
    std::optional<bool> loaded;
};
RemeshReadReport readRemesh(RemeshState&, RemeshIo&, NumericLocale);
// ProjectOpen assigns enabled only after a supported original reader return.
RemeshReadReport loadRemeshAtProjectOpen(RemeshState&, RemeshIo&, NumericLocale);

struct RemeshSaveReport {
    ApplyResult outcome = ApplyResult::Completed;
    std::optional<bool> opened;
    std::optional<bool> removalSucceeded;
    // Original unchecked fopen -> fprintf(NULL) reaches the CRT invalid-
    // parameter dispatcher. Fresh startup initializes its default terminating
    // handler; the public terminal-outcome representation remains unrecovered.
    bool invalidParameterReached = false;
};
// Original09823e tail AFTER project/properties/MST, even when their file-status
// byte isfalse. directory is the actual original-equivalent CWD at that point.
// Successful writes/close and disabled QFile::remove failures do not stop Save.
RemeshSaveReport saveRemeshAtProjectSave(const RemeshState&, const std::string& absoluteDirectory,
                                       NumericLocale);

} // namespace m2m::engine::simcontrol300
