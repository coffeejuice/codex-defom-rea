#pragma once

// Test-only paths. Fixtures are explicit inputs; every filesystem mutation belongs
// below the CTest-configured, disposable scratch directory.
#include <cstdlib>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <vector>
#include <windows.h>

namespace m2m_test {
inline std::filesystem::path configuredPath(const char* name)
{
    const char* value = std::getenv(name);
    if (!value || !*value)
        throw std::runtime_error(std::string("Required test fixture setting: ") + name);
    const std::filesystem::path path(value);
    if (!path.is_absolute())
        throw std::runtime_error(std::string("Test setting must be absolute: ") + name);
    return path;
}

inline std::filesystem::path scratchRoot()
{
    const auto root = configuredPath("M2M_TEST_SCRATCH");
    std::filesystem::create_directories(root);
    return root;
}

// FileEffects describe outputs/deletions that may not exist. Normalize the
// lexical spelling (including corpus/../../oracle_b) and match Windows path
// components without case sensitivity; do not inspect or create these files.
// This is a comparison helper, never an output-containment check.
inline std::filesystem::path effectRelative(const std::filesystem::path& value,
                                            const std::filesystem::path& base)
{
    const auto path = std::filesystem::absolute(value).lexically_normal();
    const auto parent = std::filesystem::absolute(base).lexically_normal();
    auto same = [](const std::filesystem::path& a, const std::filesystem::path& b) {
        return CompareStringOrdinal(a.c_str(), -1, b.c_str(), -1, TRUE) == CSTR_EQUAL;
    };
    if (!same(path.root_name(), parent.root_name()) ||
        path.has_root_directory() != parent.has_root_directory())
        throw std::runtime_error("Effect and comparison base have different roots");
    auto part = path.begin(), rootPart = parent.begin();
    while (part != path.end() && rootPart != parent.end() && same(*part, *rootPart)) {
        ++part;
        ++rootPart;
    }
    std::filesystem::path relative;
    for (; rootPart != parent.end(); ++rootPart)
        if (!rootPart->empty() && *rootPart != ".") relative /= "..";
    for (; part != path.end(); ++part) relative /= *part;
    return relative.empty() ? std::filesystem::path(".") : relative;
}

// Resolve existing ancestors, including junctions/symlinks, before appending a
// missing output tail. weakly_canonical on a nonexistent Windows leaf can fail
// with access denied; a dangling reparse point must still fail closed.
inline std::filesystem::path resolveOutput(const std::filesystem::path& value)
{
    auto existing = std::filesystem::absolute(value).lexically_normal();
    std::vector<std::filesystem::path> tail;
    for (;;) {
        std::error_code ec;
        const auto status = std::filesystem::symlink_status(existing, ec);
        if (ec && ec != std::errc::no_such_file_or_directory)
            throw std::filesystem::filesystem_error("Inspect test output ancestor", existing, ec);
        if (status.type() != std::filesystem::file_type::not_found) {
            if (ec) throw std::filesystem::filesystem_error("Inspect test output ancestor", existing, ec);
            break;
        }
        tail.push_back(existing.filename());
        const auto parent = existing.parent_path();
        if (parent == existing)
            throw std::runtime_error("No existing output ancestor: " + value.string());
        existing = parent;
    }
    auto resolved = std::filesystem::canonical(existing);
    for (auto it = tail.rbegin(); it != tail.rend(); ++it) resolved /= *it;
    return resolved.lexically_normal();
}

inline const char* scratchSetting(const char* name)
{
    const char* value = std::getenv(name);
    if (!value || !*value) return nullptr;
    const auto path = resolveOutput(configuredPath(name));
    const auto root = std::filesystem::canonical(scratchRoot());
    const auto relative = path.lexically_relative(root);
    if (relative.empty() || relative.is_absolute() || *relative.begin() == "..")
        throw std::runtime_error(std::string("Test output escapes scratch: ") + name);
    std::filesystem::create_directories(path);
    return value;
}

inline std::filesystem::path scratchOutput(const std::filesystem::path& value)
{
    const auto path = resolveOutput(value);
    const auto root = std::filesystem::canonical(scratchRoot());
    const auto relative = path.lexically_relative(root);
    if (relative.empty() || relative.is_absolute() || *relative.begin() == "..")
        throw std::runtime_error("Test output escapes scratch: " + path.string());
    return path;
}
}  // namespace m2m_test
