// Minimal JSON reader for the oracle cache's result.json (objects, arrays,
// strings, numbers, true/false/null). Not a general-purpose parser: \u escapes
// outside ASCII are kept as '?'.
#pragma once

#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace defcmp::json {

struct Value {
    enum Type { Null, Bool, Number, String, Array, Object } type = Null;
    bool b = false;
    double num = 0;
    std::string str;
    std::vector<Value> arr;
    std::map<std::string, Value> obj;

    const Value* get(const std::string& key) const {
        auto it = obj.find(key);
        return it == obj.end() ? nullptr : &it->second;
    }
};

// Throws std::runtime_error on malformed input.
Value parse(std::string_view text);

}  // namespace defcmp::json
