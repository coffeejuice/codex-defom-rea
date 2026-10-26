#pragma once

#include "m2m/engine/simcontrol300_remesh.hpp"
#include <cstdarg>

namespace m2m::engine::simcontrol300::detail {

class RemeshSaveFile {
public:
    virtual ~RemeshSaveFile() = default;
    virtual int writeV(const char* format, std::va_list args) = 0;
    virtual int close() = 0;
};

// Ordered output body after successful Open. The native Save path uses a stack
// adapter over its stack CoggingFile; focused tests can inject write/close errors.
void writeRemeshSaveBody(const RemeshState&, RemeshSaveFile&);

} // namespace m2m::engine::simcontrol300::detail
