// Helpers shared by the comparator sources.
#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "defcmp/defcmp.hpp"

namespace defcmp::detail {

std::string read_file(const fs::path& p);   // throws std::runtime_error

// Splits into lines without their terminators ("\n" or "\r\n"); a final line
// without a terminator is kept, an empty tail is not.
std::vector<std::string_view> split_lines(std::string_view text);

// Printable copy of a line for reports (control characters escaped, long lines cut).
std::string show(std::string_view s, std::size_t max = 160);

// 1-based column of the first differing character (0 when equal).
std::size_t first_diff_column(std::string_view a, std::string_view b);

}  // namespace defcmp::detail
