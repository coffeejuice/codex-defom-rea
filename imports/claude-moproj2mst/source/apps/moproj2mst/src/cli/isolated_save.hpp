#pragma once

#include <string>

namespace m2m::cli {

// Private opt-in CLI boundary. Ordinary CLI calls and the six-export C ABI
// continue to use their existing in-process path.
struct IsolatedSaveRequest {
    std::wstring project;
    std::wstring output;
    std::wstring deformDir;
    std::wstring receipt;
    unsigned flags = 0;
    unsigned codepage = 1252;
};

// Returns an ordinary adapter/supervisor code. A validated terminal Boundary
// commits the receipt and terminates only this CLI process with 0xC0000417.
int runIsolatedSave(const IsolatedSaveRequest&);

// Called only for the explicit same-image child role, before ordinary parsing.
int isolatedSaveWorkerMain(int argc, wchar_t** argv);

} // namespace m2m::cli
