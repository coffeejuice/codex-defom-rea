#include "json.hpp"

#include <cctype>
#include <cstdlib>
#include <stdexcept>

namespace defcmp::json {

namespace {

struct Parser {
    std::string_view s;
    std::size_t i = 0;

    [[noreturn]] void fail(const char* what) const {
        throw std::runtime_error(std::string("JSON: ") + what + " at offset " + std::to_string(i));
    }
    void ws() {
        while (i < s.size() && (s[i] == ' ' || s[i] == '\t' || s[i] == '\n' || s[i] == '\r')) ++i;
    }
    bool lit(std::string_view w) {
        if (s.substr(i, w.size()) != w) return false;
        i += w.size();
        return true;
    }
    std::string string() {
        if (s[i] != '"') fail("expected string");
        ++i;
        std::string out;
        while (i < s.size() && s[i] != '"') {
            char c = s[i++];
            if (c != '\\') {
                out += c;
                continue;
            }
            if (i >= s.size()) fail("bad escape");
            c = s[i++];
            switch (c) {
            case 'n': out += '\n'; break;
            case 't': out += '\t'; break;
            case 'r': out += '\r'; break;
            case 'b': out += '\b'; break;
            case 'f': out += '\f'; break;
            case 'u': {
                if (i + 4 > s.size()) fail("bad \\u escape");
                const long cp = std::strtol(std::string(s.substr(i, 4)).c_str(), nullptr, 16);
                out += cp < 128 ? static_cast<char>(cp) : '?';
                i += 4;
                break;
            }
            default: out += c;
            }
        }
        if (i >= s.size()) fail("unterminated string");
        ++i;
        return out;
    }
    Value value() {
        ws();
        if (i >= s.size()) fail("unexpected end");
        Value v;
        const char c = s[i];
        if (c == '{') {
            v.type = Value::Object;
            ++i;
            ws();
            if (i < s.size() && s[i] == '}') { ++i; return v; }
            for (;;) {
                ws();
                std::string k = string();
                ws();
                if (i >= s.size() || s[i] != ':') fail("expected ':'");
                ++i;
                v.obj[k] = value();
                ws();
                if (i < s.size() && s[i] == ',') { ++i; continue; }
                if (i < s.size() && s[i] == '}') { ++i; return v; }
                fail("expected ',' or '}'");
            }
        }
        if (c == '[') {
            v.type = Value::Array;
            ++i;
            ws();
            if (i < s.size() && s[i] == ']') { ++i; return v; }
            for (;;) {
                v.arr.push_back(value());
                ws();
                if (i < s.size() && s[i] == ',') { ++i; continue; }
                if (i < s.size() && s[i] == ']') { ++i; return v; }
                fail("expected ',' or ']'");
            }
        }
        if (c == '"') {
            v.type = Value::String;
            v.str = string();
            return v;
        }
        if (lit("true")) { v.type = Value::Bool; v.b = true; return v; }
        if (lit("false")) { v.type = Value::Bool; return v; }
        if (lit("null")) return v;
        const std::size_t b = i;
        while (i < s.size() && (std::isdigit(static_cast<unsigned char>(s[i])) || s[i] == '-' || s[i] == '+' ||
                                s[i] == '.' || s[i] == 'e' || s[i] == 'E'))
            ++i;
        if (b == i) fail("unexpected character");
        v.type = Value::Number;
        v.num = std::strtod(std::string(s.substr(b, i - b)).c_str(), nullptr);
        return v;
    }
};

}  // namespace

Value parse(std::string_view text) {
    Parser p{text};
    Value v = p.value();
    p.ws();
    if (p.i != text.size()) p.fail("trailing characters");
    return v;
}

}  // namespace defcmp::json
