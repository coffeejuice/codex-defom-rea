#pragma once

#include "m2m/qt4.hpp"

namespace m2m::xml { struct Node; }
namespace m2m::doe {
struct Study;
struct InputTableSource;

// Exact retained scalar/TLOC profiles only. With a source notifier, operation1
// reaches its bounded notifier entry and stops before an unqualified return;
// parsed-empty operations skip notifier, commit scalar text and stop at
// 0x140910abf. Neither branch implies successful parse or Qt data/ref parity.
[[noreturn]] void readBoundedKeywordConstructorPrefix(
    Study&, const qt::String& originalVariableRowName, int readerLocalDimension,
    const xml::Node& keyword, const InputTableSource* source = nullptr);
} // namespace m2m::doe
