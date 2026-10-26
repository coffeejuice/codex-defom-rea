#pragma once

#include <filesystem>
#include <cstdint>
#include <string>
#include "../engine/terminal_harness_supervisor.hpp"

namespace m2m::cli {

struct IsolatedJob {
    std::u16string project;
    std::u16string output;
    std::u16string deformDir;
    std::u16string workingDirectory; // caller CWD, restored after reading the owned job
    std::uint64_t nonce = 0; // cryptographic supervisor nonce, equal to child argv
    unsigned flags = 0;
    unsigned codepage = 1252;
};

// The parent writes an exclusive bounded job inside the supervisor leaf;
// the child reads only that leaf. Both reject malformed paths and lengths.
void writeIsolatedJob(const std::filesystem::path& leaf, const IsolatedJob&);
IsolatedJob readIsolatedJob(const std::filesystem::path& leaf);

// CREATE_NEW, complete write, FlushFileBuffers and close before the caller
// relays a validated terminal status. Never replaces an existing receipt.
void commitIsolatedTerminalReceipt(const std::filesystem::path& absoluteTarget,
                                   const engine::terminal_harness::Observation&,
                                   const IsolatedJob&);

} // namespace m2m::cli
