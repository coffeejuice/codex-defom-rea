// P03 test support: corpus cases, the OPRNAM sequence of a saved MST, and a flow chart built from a
// .moproj by a small XML reader (enough for these files: elements, attributes, text, comments,
// the five predefined entities and numeric references below 0x80).
#pragma once

#include <algorithm>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"   // complete OpClass for Project's registry

namespace p03test {

struct El {
    std::string tag;
    std::vector<std::pair<std::string, std::string>> attrs;
    std::string text;              // concatenated direct and descendant text
    std::vector<std::unique_ptr<El>> kids;

    std::string attr(const std::string& n) const
    {
        for (const auto& a : attrs)
            if (a.first == n) return a.second;
        return {};
    }
    std::vector<const El*> all(const std::string& t) const
    {
        std::vector<const El*> v;
        for (const auto& k : kids)
            if (k->tag == t) v.push_back(k.get());
        return v;
    }
    const El* first(const std::string& t) const
    {
        for (const auto& k : kids)
            if (k->tag == t) return k.get();
        return nullptr;
    }
};

inline std::string unescape(const std::string& s)
{
    std::string o;
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] != '&') {
            o += s[i];
            continue;
        }
        size_t e = s.find(';', i);
        std::string n = s.substr(i + 1, e - i - 1);
        if (n == "lt") o += '<';
        else if (n == "gt") o += '>';
        else if (n == "amp") o += '&';
        else if (n == "quot") o += '"';
        else if (n == "apos") o += '\'';
        else if (!n.empty() && n[0] == '#') o += static_cast<char>(std::stoi(n.substr(1)));
        i = e;
    }
    return o;
}

class Reader {
public:
    explicit Reader(std::string s) : s_(std::move(s)) {}
    std::unique_ptr<El> parse()
    {
        for (;;) {
            size_t lt = s_.find('<', p_);
            if (lt == std::string::npos) return nullptr;
            p_ = lt;
            if (s_.compare(p_, 2, "<?") == 0 || s_.compare(p_, 2, "<!") == 0) {
                skipMarkup();
                continue;
            }
            return element();
        }
    }

private:
    void skipMarkup()
    {
        if (s_.compare(p_, 4, "<!--") == 0) p_ = s_.find("-->", p_) + 3;
        else p_ = s_.find('>', p_) + 1;
    }
    std::unique_ptr<El> element()
    {
        auto e = std::make_unique<El>();
        ++p_;
        size_t q = s_.find_first_of(" \t\r\n/>", p_);
        e->tag = s_.substr(p_, q - p_);
        p_ = q;
        for (;;) {
            p_ = s_.find_first_not_of(" \t\r\n", p_);
            if (s_[p_] == '/') {
                p_ = s_.find('>', p_) + 1;
                return e;
            }
            if (s_[p_] == '>') {
                ++p_;
                break;
            }
            size_t eq = s_.find('=', p_);
            std::string name = s_.substr(p_, eq - p_);
            name.erase(name.find_last_not_of(" \t\r\n") + 1);
            size_t qs = s_.find_first_of("\"'", eq);
            size_t qe = s_.find(s_[qs], qs + 1);
            e->attrs.emplace_back(name, unescape(s_.substr(qs + 1, qe - qs - 1)));
            p_ = qe + 1;
        }
        for (;;) {
            size_t lt = s_.find('<', p_);
            e->text += unescape(s_.substr(p_, lt - p_));
            p_ = lt;
            if (s_.compare(p_, 2, "</") == 0) {
                p_ = s_.find('>', p_) + 1;
                return e;
            }
            if (s_.compare(p_, 2, "<!") == 0 || s_.compare(p_, 2, "<?") == 0) {
                skipMarkup();
                continue;
            }
            auto k = element();
            e->text += k->text;
            e->kids.push_back(std::move(k));
        }
    }
    std::string s_;
    size_t p_ = 0;
};

inline std::string readFile(const std::filesystem::path& p)
{
    std::ifstream f(p, std::ios::binary);
    std::ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

struct Case {
    std::string name;
    std::filesystem::path moproj, mst;
};

// Every corpus folder with a .moproj and the .MST of the same base name.
inline std::vector<Case> corpusCases(const std::filesystem::path& corpus)
{
    std::vector<Case> out;
    for (const auto& d : std::filesystem::directory_iterator(corpus)) {
        if (!d.is_directory()) continue;
        for (const auto& f : std::filesystem::directory_iterator(d.path())) {
            if (f.path().extension() != ".moproj") continue;
            auto mst = f.path();
            mst.replace_extension(".MST");
            if (std::filesystem::exists(mst)) out.push_back({d.path().filename().string(), f.path(), mst});
        }
    }
    std::sort(out.begin(), out.end(), [](const Case& a, const Case& b) { return a.name < b.name; });
    return out;
}

// OPRNAM values of a saved MST, consecutive repeats merged (a cycle writes its name per member).
inline std::vector<std::string> oprnamRuns(const std::filesystem::path& mst)
{
    std::istringstream in(readFile(mst));
    std::vector<std::string> out;
    std::string line;
    bool take = false;
    while (std::getline(in, line)) {
        if (take) {
            if (out.empty() || out.back() != line) out.push_back(line);
            take = false;
        } else if (line.rfind("OPRNAM", 0) == 0) {
            take = true;
        }
    }
    return out;
}

// Append unless it repeats the last name: the MST side merges a cycle's repeated OPRNAM, and the
// 2013 MST of case 56 has two operations with one name (no rename then, s1-01 6.2).
inline void pushRun(std::vector<std::string>& v, const std::string& s)
{
    if (v.empty() || v.back() != s) v.push_back(s);
}

inline std::string join(const std::vector<std::string>& v)
{
    std::string s;
    for (const auto& x : v) s += "[" + x + "]";
    return s;
}

struct Project {
    std::unique_ptr<El> root;
    std::map<int, std::string> names;   // operation id -> <Name> text (not uniquified)
};

inline int toInt(const std::string& s)
{
    try {
        return std::stoi(s);
    } catch (...) {
        return 0;
    }
}

inline Project load(const std::filesystem::path& moproj)
{
    Project px;
    std::string text = readFile(moproj);
    text.erase(std::remove(text.begin(), text.end(), '\r'), text.end());
    px.root = Reader(text).parse();
    if (const El* ol = px.root->first("Operation_List")) {
        int ordinal = 0;
        for (const El* op : ol->all("Operation")) {
            ++ordinal;
            const El* id = op->first("Id");
            const El* name = op->first("Name");
            int v = id ? toInt(id->text) : 0;
            if (v == 0) v = ordinal;                     // 0x14008ccf0: id 0 -> ordinal
            px.names[v] = name ? name->text : std::string();
        }
    }
    return px;
}

// Flow chart of the first Sub_Project as 0x14008f530 / 0x140091f00 build it.
inline void buildFlow(const Project& px, m2m::SubProject& sub)
{
    using m2m::qt::String;
    const El* sp = px.root->first("Sub_Project");
    if (!sp) return;
    std::vector<m2m::FlowItem*> items;
    std::map<int, int> next;
    int k = 0;
    for (const El* di : sp->all("DiagramItem")) {
        next[k] = -1;
        m2m::FlowItem* it = nullptr;
        const int opId = toInt(di->attr("Operation_Id"));
        for (const auto& c : di->kids) {
            if (c->tag == "ClassName") {
                auto n = std::make_unique<m2m::FlowItem>();
                n->opId = opId;
                n->group = c->text == "SfOprCycle" || c->text == "SfOprGroup";
                it = n.get();
                sub.flow.items.push_back(std::move(n));
            } else if (c->tag == "CellPos") {
                it->row = toInt(c->attr("Row"));
                it->col = toInt(c->attr("Col"));
            } else if (c->tag == "GroupInfo") {
                it->beginRow = toInt(c->attr("BeginRow"));
                it->beginCol = toInt(c->attr("BeginCol"));
                it->endRow = toInt(c->attr("EndRow"));
                it->endCol = toInt(c->attr("EndCol"));
            } else if (c->tag == "Connectors") {
                for (const El* cs : c->all("Connections"))
                    for (const El* ii : cs->all("ItemIndex")) next[k] = toInt(ii->text);
            }
        }
        items.push_back(it);
        ++k;
    }
    for (int j = 0; j < k; ++j) {
        const int t = next[j];
        if (t < 0 || t >= k || !items[j] || !items[t]) continue;
        m2m::FlowItem* a = items[j];
        m2m::FlowItem* b = items[t];
        if (b->prev) b->prev->next = nullptr, b->prev = nullptr;
        if (a->next) a->next->prev = nullptr, a->next = nullptr;
        a->next = b;
        b->prev = a;
    }
}

}  // namespace p03test
