// Native regular-file branch of the shipped CRT fopen(path,"wt").
#pragma once
#include <array>
#include <cstdio>
#include "m2m/qt4.hpp"

namespace m2m::datafiles::detail {

class CoggingFile {
    std::FILE* file_ = nullptr;
    std::array<char, 0x1000> buffer_{};
public:
    explicit CoggingFile(const qt::String& path);
    // Other original CRT writers choose their own QString codec before fopen.
    explicit CoggingFile(const std::string& pathBytes);
    ~CoggingFile();
    CoggingFile(const CoggingFile&) = delete;
    CoggingFile& operator=(const CoggingFile&) = delete;
    std::FILE* get() const { return file_; }
    void close();
};

} // namespace m2m::datafiles::detail
