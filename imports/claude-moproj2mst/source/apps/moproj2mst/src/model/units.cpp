// P17: unit-string evaluator 0x1402ede00 and the unit table 0x143d76368 (spec: docs/re/moproj2mst/p17/spec.md 1).
#include <cstdint>
#include <iterator>
#include <vector>
#include <defmodel/step/db_step.hpp>

#include "m2m/model.hpp"
#include "m2m/p17.hpp"

namespace m2m::units {
namespace {

struct Conv {                      // 0x18-byte entry, ctor 0x1402ee5e0 (1.0, 0, 0)
    double factor;                 // +0x00
    double offset;                 // +0x08
    std::uint8_t flag;             // +0x10
};

struct Category {                  // 0x38-byte record built by 0x1402ea840
    int kind;
    const char* name;              // +0x00
    const char* label;             // +0x08
    int n;                         // +0x20 QStringList size (= +0x28 QList<int> size)
    const char* const* units;      // +0x20 unit texts (nullptr = null QString)
    const Conv* conv;              // +0x30 n x n, conv[from * n + to]
};

// @src 0x1402eaaf0 (ctor, via static init @src 0x141ca0c60; atexit @src 0x141df4760 frees it) and
// @src 0x1402ea840 (one category; record ctor @src 0x1402ee620): generated table
#include "units_table.inc"

const Category* category(int kind)   // 0x140c43620 / 0x1402eebb0 on QMap<int, Category*>
{
    for (const Category& c : kCategories)
        if (c.kind == kind) return &c;
    return nullptr;
}

// QString(const char*) for the table texts: ASCII, null for a null pointer.
String text(const char* s) { return String::latin1(s); }

// QChar::toCaseFolded of the code units that fold to ASCII (Qt 4.8 simple folding); the table side
// is ASCII, so no other folding can make two texts equal.
char16_t fold(char16_t c)
{
    if (c >= u'A' && c <= u'Z') return static_cast<char16_t>(c + 0x20);
    if (c == 0x212a) return u'k';  // KELVIN SIGN
    if (c == 0x017f) return u's';  // LATIN SMALL LETTER LONG S
    return c;
}

// QString::compare(o, Qt::CaseInsensitive) == 0
bool equalsCi(const String& a, const String& b)
{
    if (a.size() != b.size()) return false;
    for (int i = 0; i < a.size(); ++i)
        if (fold(a.at(i)) != fold(b.at(i))) return false;
    return true;
}

// @src 0x1402ee890  flag ? (v + offset) * factor : v * factor + offset
double apply(const Conv& c, double v)
{
    if (c.flag == 0) return v * c.factor + c.offset;
    return (v + c.offset) * c.factor;
}

}  // namespace

// @src 0x140917de0
const char* unitName(int kind, int unitSys)
{
    if (kind < 0 || kind >= static_cast<int>(std::size(kUnitNameSI))) return "";
    return unitSys == 1 ? kUnitNameSI[kind] : kUnitNameEN[kind];
}

// @src 0x1402ed330
int unitIndex(int kind, const String& unit)
{
    int r = -1;
    if (const Category* c = category(kind)) {
        for (int i = 0; i < c->n; ++i) {
            if (equalsCi(text(c->units[i]), unit)) {
                r = i;
                break;
            }
        }
    }
    return r;
}

// @src 0x1402ed5c0
bool convert(int kind, const String& from, double v, const String& to, double* out)
{
    bool ok = false;
    const Category* c = category(kind);
    if (!c) {
        if (kind == kPlainNumber) *out = v;
    } else {
        int i = unitIndex(kind, from);
        int j = unitIndex(kind, to);
        if (i != -1 && j != -1) {
            *out = apply(c->conv[i * c->n + j], v);
            ok = true;
        }
    }
    return ok;
}

// @src 0x1402edb70
bool parse(int kind, const String& text_, double* out, String* unit)
{
    *out = 0.0;
    bool r = false;
    const Category* c = category(kind);
    if (!c) {
        if (kind == kPlainNumber) {
            bool ok = false;
            *out = qt::toDouble(text_, &ok);
            if (ok) r = true;
        }
    } else {
        std::vector<String> parts = qt::split(text_, String::latin1(" "), qt::SplitBehavior::KeepEmptyParts);
        // 0x1400acf20 QStringList::removeAll("")
        std::vector<String> kept;
        for (String& s : parts)
            if (!(s == String::latin1(""))) kept.push_back(std::move(s));
        parts = std::move(kept);
        if (parts.size() < 2) {
            if (parts.size() == 1) {
                bool ok = false;
                *out = qt::toDouble(parts[0], &ok);
                if (ok) r = true;
            }
        } else {
            for (int i = 0; i < c->n; ++i) {
                if (text(c->units[i]) == parts[1]) {
                    bool ok = false;
                    *out = qt::toDouble(parts[0], &ok);
                    *unit = parts[1];
                    if (ok) {
                        r = true;
                        break;
                    }
                }
            }
        }
    }
    return r;
}

// @src 0x1402ed750
bool toSystem(int kind, int unitSys, const String& unit, double v, double* out)
{
    bool r = false;
    String target = text(unitName(kind, unitSys));
    if (!target.isEmpty()) {
        r = convert(kind, unit, v, target, out);
    } else {
        *out = v;
        if (kind == kPlainNumber) r = true;
    }
    return r;
}

// @src 0x1402ede00
bool evalUnit(int kind, int unitSys, const String& text_, double* out, String* unit)
{
    bool r = false;
    if (parse(kind, text_, out, unit)) {
        r = toSystem(kind, unitSys, *unit, *out, out);
        *unit = text(unitName(kind, unitSys));
    }
    return r;
}

}  // namespace m2m::units

namespace m2m {

// @src 0x1403b5580 reads 0x143d79724, initially zero in the PE virtual tail.
// Known UI writers include New Project 14001c110, SfProjectNew::slotOkClicked
// 140127960 and the unit-combo route 1403b5f50 through setter 1403b5560.
// The adapter scopes an explicitly requested startup profile to one conversion.
int guiStartupUnit()
{
    return def::model::g_gui_engine_startup_unit;
}

GuiStartupUnitScope::GuiStartupUnitScope(int overrideUnit) : previous_(guiStartupUnit())
{
    def::model::g_gui_engine_startup_unit = overrideUnit == 1 || overrideUnit == 2 ? overrideUnit : 0;
}

GuiStartupUnitScope::~GuiStartupUnitScope()
{
    def::model::g_gui_engine_startup_unit = previous_;
}

// G0 seam: the evaluator with a fresh unit text (as the callers 0x14060c4e0 / 0x1405fb980 pass).
double evalUnitString(const String& text, int category, int unit, bool* ok)
{
    double out = 0.0;
    String u;
    bool r = units::evalUnit(category, unit, text, &out, &u);
    if (ok) *ok = r;
    return out;
}

}  // namespace m2m
