// PathScale Fortran I/O runtime subset (defrt/ftn/fio.hpp).
#include <defrt/ftn/fio.hpp>

#include <cctype>
#include <charconv>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

namespace def::rt::ftn::fio {

namespace {

struct Unit {
    std::FILE* f = nullptr;
    std::string name;
    bool written = false; // last operation was a WRITE
};

// sequential ENDFILE at the current position (used by close after a WRITE)
void truncate_here(std::FILE* f)
{
    std::fflush(f);
#ifdef _WIN32
    long long pos = _ftelli64(f);
    if (pos >= 0) {
        (void)_chsize_s(_fileno(f), pos);
    }
#else
    long pos = std::ftell(f);
    if (pos >= 0) {
        (void)::ftruncate(fileno(f), pos);
    }
#endif
}

std::map<int, Unit>& units()
{
    static std::map<int, Unit> u;
    return u;
}

void raw_stdout(const std::string& s)
{
    const char* p = s.data();
    std::size_t n = s.size();
    while (n > 0) {
#ifdef _WIN32
        int k = _write(1, p, static_cast<unsigned>(n));
#else
        long k = ::write(1, p, n);
#endif
        if (k <= 0) {
            return;
        }
        p += k;
        n -= static_cast<std::size_t>(k);
    }
}

// ---- format parsing --------------------------------------------------------------------------

struct Node {
    char t = 0; // X I F E A L(literal) / G(group)
    int rep = 1;
    int w = 0;
    int d = 0;
    bool has_w = false;
    std::string lit;
    std::vector<Node> kids;
};

struct Parser {
    const char* s;
    std::size_t i = 0;
    std::size_t n;

    void ws()
    {
        while (i < n && (s[i] == ' ' || s[i] == ',')) {
            ++i;
        }
    }
    int num(bool* got)
    {
        int v = 0;
        *got = false;
        while (i < n && std::isdigit(static_cast<unsigned char>(s[i]))) {
            v = v * 10 + (s[i] - '0');
            ++i;
            *got = true;
        }
        return v;
    }
    std::vector<Node> list() // after '('
    {
        std::vector<Node> out;
        for (;;) {
            ws();
            if (i >= n) {
                return out;
            }
            char c = s[i];
            if (c == ')') {
                ++i;
                return out;
            }
            if (c == '/') {
                Node nd;
                nd.t = '/';
                out.push_back(nd);
                ++i;
                continue;
            }
            if (c == '\'' || c == '"') {
                Node nd;
                nd.t = 'L';
                char q = c;
                ++i;
                while (i < n) {
                    if (s[i] == q) {
                        if (i + 1 < n && s[i + 1] == q) {
                            nd.lit += q;
                            i += 2;
                            continue;
                        }
                        ++i;
                        break;
                    }
                    nd.lit += s[i++];
                }
                out.push_back(nd);
                continue;
            }
            bool got = false;
            int r = num(&got);
            ws();
            if (i >= n) {
                return out;
            }
            c = static_cast<char>(std::toupper(static_cast<unsigned char>(s[i])));
            Node nd;
            nd.rep = got ? r : 1;
            if (c == '(') {
                ++i;
                nd.t = 'G';
                nd.kids = list();
                out.push_back(nd);
                continue;
            }
            if (c == 'X') {
                ++i;
                nd.t = 'X';
                nd.w = got ? r : 1;
                nd.rep = 1;
                out.push_back(nd);
                continue;
            }
            if (c == 'I' || c == 'F' || c == 'E' || c == 'A' || c == 'G' || c == 'D') {
                ++i;
                nd.t = (c == 'D' || c == 'G') ? 'E' : c;
                bool gw = false;
                nd.w = num(&gw);
                nd.has_w = gw;
                if (i < n && s[i] == '.') {
                    ++i;
                    bool gd = false;
                    nd.d = num(&gd);
                }
                out.push_back(nd);
                continue;
            }
            // unknown descriptor: skip it
            ++i;
        }
    }
};

std::vector<Node> parse(const std::string& f)
{
    Parser p{f.c_str(), 0, f.size()};
    p.ws();
    if (p.i < p.n && p.s[p.i] == '(') {
        ++p.i;
    }
    return p.list();
}

// ---- formatted output ------------------------------------------------------------------------

struct Out {
    std::vector<std::string> recs{std::string()};
    const std::vector<std::string>* dummy = nullptr;
};

} // namespace

std::string format_i(std::int64_t v, int w)
{
    std::string s = std::to_string(v < 0 ? -v : v);
    if (v < 0) {
        s = "-" + s;
    }
    if (w <= 0) {
        return s;
    }
    if (static_cast<int>(s.size()) > w) {
        return std::string(static_cast<std::size_t>(w), '*');
    }
    return std::string(static_cast<std::size_t>(w) - s.size(), ' ') + s;
}

std::string format_f(double v, int w, int dd)
{
    if (std::isnan(v) || std::isinf(v)) {
        std::string s = std::isnan(v) ? "NaN" : (v < 0 ? "-Infinity" : "Infinity");
        if (static_cast<int>(s.size()) > w) {
            s = std::isnan(v) ? "NaN" : (v < 0 ? "-Inf" : "Inf");
        }
        if (static_cast<int>(s.size()) > w) {
            return std::string(static_cast<std::size_t>(w), '*');
        }
        return std::string(static_cast<std::size_t>(w) - s.size(), ' ') + s;
    }
    char buf[512];
    auto r = std::to_chars(buf, buf + sizeof(buf), std::fabs(v), std::chars_format::fixed, dd);
    std::string s(buf, r.ptr);
    bool neg = std::signbit(v);
    if (neg) {
        // a negative value that rounds to zero keeps its sign
        s = "-" + s;
    }
    if (static_cast<int>(s.size()) > w) {
        // drop the optional leading zero first
        std::size_t z = neg ? 1 : 0;
        if (s.size() > z + 1 && s[z] == '0' && s[z + 1] == '.') {
            s.erase(z, 1);
        }
    }
    if (static_cast<int>(s.size()) > w) {
        return std::string(static_cast<std::size_t>(w), '*');
    }
    return std::string(static_cast<std::size_t>(w) - s.size(), ' ') + s;
}

std::string format_e(double v, int w, int dd)
{
    char buf[512];
    // 0.dddE+xx: mantissa digits from a correctly rounded scientific conversion
    if (v == 0.0) {
        std::string s = (std::signbit(v) ? "-0." : "0.") + std::string(static_cast<std::size_t>(dd), '0') + "E+00";
        if (static_cast<int>(s.size()) > w) {
            return std::string(static_cast<std::size_t>(w), '*');
        }
        return std::string(static_cast<std::size_t>(w) - s.size(), ' ') + s;
    }
    auto r = std::to_chars(buf, buf + sizeof(buf), std::fabs(v), std::chars_format::scientific,
                           dd > 0 ? dd - 1 : 0);
    std::string sci(buf, r.ptr); // d.ddde+xx
    std::size_t epos = sci.find('e');
    int ex = std::atoi(sci.c_str() + epos + 1) + 1;
    std::string digits;
    for (std::size_t k = 0; k < epos; ++k) {
        if (std::isdigit(static_cast<unsigned char>(sci[k]))) {
            digits += sci[k];
        }
    }
    std::string s = (v < 0 ? "-0." : "0.") + digits + "E";
    s += ex < 0 ? "-" : "+";
    int ae = ex < 0 ? -ex : ex;
    if (ae < 10) {
        s += "0";
    }
    s += std::to_string(ae);
    if (static_cast<int>(s.size()) > w) {
        return std::string(static_cast<std::size_t>(w), '*');
    }
    return std::string(static_cast<std::size_t>(w) - s.size(), ' ') + s;
}

Write::Write(int unit) : unit_(unit), list_(true) {}

Write::Write(int unit, const char* format) : unit_(unit), format_(format), list_(false) {}

Write& Write::i(std::int32_t v)
{
    items_.push_back({'i', v, 0.0, {}});
    return *this;
}

Write& Write::d(double v)
{
    items_.push_back({'d', 0, v, {}});
    return *this;
}

Write& Write::r(float v)
{
    items_.push_back({'r', 0, static_cast<double>(v), {}});
    return *this;
}

Write& Write::a(const char* s)
{
    items_.push_back({'a', 0, 0.0, std::string(s)});
    return *this;
}

Write& Write::a(const char* s, std::size_t len)
{
    items_.push_back({'a', 0, 0.0, std::string(s, len)});
    return *this;
}

namespace {

struct Exec {
    const std::vector<Write*>* unused = nullptr;
};

} // namespace

std::string Write::render() const
{
    std::vector<std::string> recs(1);
    if (list_) {
        std::string& r = recs[0];
        r = " ";
        for (const Item& it : items_) {
            if (it.kind == 'a') {
                r += it.sv;
            } else if (it.kind == 'i') {
                r += ' ';
                r += it.iv < 0 ? "-" : " ";
                r += std::to_string(it.iv < 0 ? -it.iv : it.iv);
            } else {
                // REAL list-directed layout not checked against the runtime yet
                char buf[64];
                std::snprintf(buf, sizeof(buf), "  %.16G", it.dv);
                r += buf;
            }
        }
    } else {
        std::vector<Node> fmt = parse(format_);
        std::size_t next = 0;
        bool stop = false;
        // returns false when a data descriptor needs an item and none is left
        auto emit = [&](auto&& self, const std::vector<Node>& nodes) -> bool {
            for (const Node& nd : nodes) {
                for (int k = 0; k < (nd.t == 'X' ? 1 : nd.rep); ++k) {
                    std::string& r = recs.back();
                    switch (nd.t) {
                    case 'L':
                        r += nd.lit;
                        break;
                    case 'X':
                        r.append(static_cast<std::size_t>(nd.w), ' ');
                        break;
                    case '/':
                        recs.emplace_back();
                        break;
                    case 'G':
                        if (!self(self, nd.kids)) {
                            return false;
                        }
                        break;
                    default: {
                        if (next >= items_.size()) {
                            return false;
                        }
                        const Item& it = items_[next++];
                        if (nd.t == 'I') {
                            r += format_i(it.kind == 'i' ? it.iv : static_cast<std::int64_t>(it.dv), nd.w);
                        } else if (nd.t == 'F') {
                            r += format_f(it.kind == 'i' ? static_cast<double>(it.iv) : it.dv, nd.w, nd.d);
                        } else if (nd.t == 'E') {
                            r += format_e(it.kind == 'i' ? static_cast<double>(it.iv) : it.dv, nd.w, nd.d);
                        } else if (nd.t == 'A') {
                            std::string sv = it.sv;
                            if (nd.has_w) {
                                if (static_cast<int>(sv.size()) >= nd.w) {
                                    sv = sv.substr(0, static_cast<std::size_t>(nd.w));
                                } else {
                                    sv = std::string(static_cast<std::size_t>(nd.w) - sv.size(), ' ') + sv;
                                }
                            }
                            r += sv;
                        }
                        break;
                    }
                    }
                }
            }
            return true;
        };
        if (emit(emit, fmt)) {
            // format reversion: items left -> new record from the last top-level group
            while (next < items_.size() && !stop) {
                std::size_t before = next;
                recs.emplace_back();
                const Node* last = nullptr;
                for (const Node& nd : fmt) {
                    if (nd.t == 'G') {
                        last = &nd;
                    }
                }
                bool ok = last ? emit(emit, std::vector<Node>{*last}) : emit(emit, fmt);
                if (!ok || next == before) {
                    break;
                }
            }
        }
    }
    std::string all;
    for (std::size_t k = 0; k < recs.size(); ++k) {
        if (k) {
            all += '\n';
        }
        all += recs[k];
    }
    return all;
}

std::string Write::text() const
{
    return render();
}

void Write::end()
{
    std::string t = render();
    if (unit_ == 6) {
        raw_stdout(t + "\n");
        return;
    }
    // records on files end with CR LF
    std::string out;
    for (char c : t) {
        if (c == '\n') {
            out += "\r\n";
        } else {
            out += c;
        }
    }
    out += "\r\n";
    auto& u = units();
    auto itu = u.find(unit_);
    if (itu != u.end() && itu->second.f) {
        std::fwrite(out.data(), 1, out.size(), itu->second.f);
        itu->second.written = true;
        return;
    }
    if (unit_ == 1) {
        if (std::FILE* f = std::fopen("NEWEQ.MSG", "ab")) {
            std::fwrite(out.data(), 1, out.size(), f);
            std::fclose(f);
        }
        return;
    }
    // unit never opened: the runtime would create fort.<unit>
    std::string name = "fort." + std::to_string(unit_);
    if (std::FILE* f = std::fopen(name.c_str(), "ab")) {
        std::fwrite(out.data(), 1, out.size(), f);
        std::fclose(f);
    }
}

bool open_old(int unit, const char* name)
{
    std::FILE* f = std::fopen(name, "r+b");
    if (!f) {
        f = std::fopen(name, "rb");
    }
    if (!f) {
        return false;
    }
    close(unit);
    units()[unit] = Unit{f, name};
    return true;
}

bool open_unknown(int unit, const char* name)
{
    close(unit);
    std::FILE* f = std::fopen(name, "r+b");
    if (!f) {
        f = std::fopen(name, "w+b");
    }
    if (!f) {
        return false;
    }
    units()[unit] = Unit{f, name};
    return true;
}

void close(int unit)
{
    auto& u = units();
    auto it = u.find(unit);
    if (it != u.end()) {
        if (it->second.f) {
            if (it->second.written) {
                truncate_here(it->second.f);
            }
            std::fclose(it->second.f);
        }
        u.erase(it);
    }
}

void rewind(int unit)
{
    auto& u = units();
    auto it = u.find(unit);
    if (it != u.end() && it->second.f) {
        std::rewind(it->second.f);
        it->second.written = false;
    }
}

int read_list_i32(int unit, std::int32_t* out, int n)
{
    auto& u = units();
    auto it = u.find(unit);
    if (it == u.end() || !it->second.f) {
        return 2;
    }
    std::FILE* f = it->second.f;
    it->second.written = false;
    for (int k = 0; k < n; ++k) {
        int c;
        do {
            c = std::fgetc(f);
        } while (c != EOF && (c == ' ' || c == ',' || c == '\t' || c == '\r' || c == '\n'));
        if (c == EOF) {
            return 1;
        }
        std::string tok;
        while (c != EOF && c != ' ' && c != ',' && c != '\t' && c != '\r' && c != '\n') {
            tok += static_cast<char>(c);
            c = std::fgetc(f);
        }
        char* e = nullptr;
        long v = std::strtol(tok.c_str(), &e, 10);
        if (!e || *e) {
            return 2;
        }
        out[k] = static_cast<std::int32_t>(v);
    }
    return 0;
}

void reset()
{
    auto& u = units();
    for (auto& kv : u) {
        if (kv.second.f) {
            if (kv.second.written) {
                truncate_here(kv.second.f);
            }
            std::fclose(kv.second.f);
        }
    }
    u.clear();
}

void stop()
{
    reset();
    std::fflush(nullptr);
    std::exit(0);
}

void stop(const char* code, std::size_t len)
{
    if (len > 1 || (len == 1 && code[0] != ' ')) {
        raw_stdout(std::string(code, len) + "\n"); // CR LF through the text-mode descriptor, as unit 6
    }
    reset();
    std::exit(0);
}

} // namespace def::rt::ftn::fio
