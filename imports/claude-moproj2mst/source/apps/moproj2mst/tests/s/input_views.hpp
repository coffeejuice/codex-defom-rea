// The view-derived parts of a .moproj taken from the input file, and the mask of what they write
// (shared by the tests that compare a saved .moproj; same as test_moproj_write.cpp's copy).
#pragma once

#include <cstdio>
#include <regex>
#include <string>
#include <vector>

#include "m2m/model.hpp"
#include "m2m/moproj_write.hpp"

namespace m2m::testviews {

inline const xml::Node* child(const xml::Node& n, const char* tag)
{
    for (const auto& c : n.children)
        if (c->kind == xml::Node::Element && c->tag == qt::L(tag)) return c.get();
    return nullptr;
}

inline double num(const xml::Node* n) { return n ? qt::toDouble(n->text()) : 0.0; }

inline xml::NodePtr deepCopy(const xml::Node& n)
{
    auto c = std::make_shared<xml::Node>();
    c->kind = n.kind;
    c->tag = n.tag;
    c->data = n.data;
    c->attrs = n.attrs;
    for (const auto& k : n.children) c->children.push_back(deepCopy(*k));
    return c;
}

// The view-derived parts as the input file has them (a stand-in for the GUI's views).
class InputViews : public moproj::DiagramViews {
public:
    explicit InputViews(const xml::Node& sub)
    {
        sub_ = deepCopy(sub);
        for (const auto& c : sub_->children)
            if (c->kind == xml::Node::Element && c->tag == qt::L("DiagramItem")) items_.push_back(c.get());
    }
    void viewCentre(const SubProject&, double& x, double& y) override
    {
        const xml::Node* vt = child(*sub_, "ViewTransform");
        x = vt ? num(child(*vt, "CenterX")) : 0;
        y = vt ? num(child(*vt, "CenterY")) : 0;
    }
    xml::NodePtr dispView(const SubProject&) override
    {
        const xml::Node* d = child(*sub_, "SfDispView");
        return d ? deepCopy(*d) : nullptr;
    }
    bool expanded(const SubProject& sub, int i) override { return moproj::savedExpanded(sub, i); }
    moproj::RectF rect(const SubProject& sub, int i) override
    {
        try {
            return moproj::sceneRect(sub, i, *this);
        } catch (const Error& e) {
            if (e.code != M2M_E_UNSUPPORTED) throw;
            rectFromInput = true;
        }
        moproj::RectF r;
        if (const xml::Node* e = child(*item(i), "RectF")) {
            r.x = num(child(*e, "x"));
            r.y = num(child(*e, "y"));
            r.h = num(child(*e, "height"));
            r.w = num(child(*e, "width"));
        }
        return r;
    }
    std::vector<moproj::ExpansionItem> expansion(const SubProject&, int i) override
    {
        std::vector<moproj::ExpansionItem> r;
        const xml::Node* x = child(*item(i), "Expansion_Items");
        if (!x) return r;
        for (const xml::Node* e : x->childElements()) {
            moproj::ExpansionItem it;
            const std::string t = e->attribute(qt::L("Type")).utf8();
            it.type = t == "Text" ? 0 : t == "Line" ? 1 : t == "Space" ? 2 : 3;
            it.label = e->attribute(qt::L("Label"));
            const std::string st = e->attribute(qt::L("LineStyle")).utf8();
            it.lineStyle = st == "NoLine" ? 0 : st == "Dash" ? 2 : st == "Dot" ? 3 : st == "DashDot" ? 4 : st == "DashDotDot" ? 5 : 1;
            std::sscanf(e->attribute(qt::L("LineColor")).utf8().c_str(), "%d, %d, %d, %d", &it.r, &it.g, &it.b, &it.a);
            r.push_back(it);
        }
        return r;
    }

    bool rectFromInput = false;

private:
    xml::NodePtr sub_;
    std::vector<const xml::Node*> items_;
    const xml::Node* item(int i)
    {
        if (i < 0 || i >= static_cast<int>(items_.size())) throw Error(M2M_E_INPUT, "input has fewer DiagramItems");
        return items_[static_cast<std::size_t>(i)];
    }
};

inline std::string masked(std::string s, bool rect)
{
    static const std::regex parts[] = {
        std::regex(R"(<ViewTransform>[\s\S]*?</ViewTransform>)"),
        std::regex(R"(<SfDispView>[\s\S]*?</SfDispView>)"),
        std::regex(R"(<Expansion_Items[\s\S]*?</Expansion_Items>|<Expansion_Items[^>]*/>)"),
        // group items: the original's flag is uninitialised memory, the port writes "yes"
        // (Aleksandr 2026-10-22); either value is accepted there
        std::regex(R"(Expanded="[a-z]*"( Operation_Id="[^"]*">\s*<ClassName>SfOpr(Cycle|Group)<))"),
    };
    static const std::regex rectF(R"(<RectF>[\s\S]*?</RectF>)");
    for (const auto& r : parts) s = std::regex_replace(s, r, "<view/>$1");
    if (rect) s = std::regex_replace(s, rectF, "<view/>");
    return s;
}

}  // namespace m2m::testviews
