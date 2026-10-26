// P01: the open path (0x14008a9b0 -> 0x14008afa0 -> 0x14008b410) on corpus projects, with the
// field values s1-01 9 gives for COG_P1_B1 and HEAT1, and every corpus project opening.
//
// The walk calls into P02 (operations), P03 (project ctor, sub-projects), P04 (QString::toInt,
// split, arg) and P05 (cleanPath, suffix helpers). Until those packets are merged their stubs
// throw m2m::NotPorted; a case that hits one prints "PENDING <address>" and does not fail, so the
// checks run for real as soon as the base branch has the other packets.
#include <testkit/check.hpp>

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <string>

#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"   // Project owns OpClass instances
#include "m2m/qt4path.hpp"

using m2m::qt::String;

namespace {

String S(const char16_t* s) { return String(s); }

std::filesystem::path corpus()
{
    const char* c = std::getenv("M2M_CORPUS");
    return c ? std::filesystem::path(c) : std::filesystem::path();
}

String qpath(const std::filesystem::path& p)
{
    std::u16string u;
    for (wchar_t c : p.generic_wstring()) u += static_cast<char16_t>(c);
    return String(u);
}

// The driver's sequence up to the open (0x14001ca30): project ctor for changeSuffix(file, ".DB"),
// class registry, OpenProject.
bool open(m2m::Project& p, const String& file, m2m::Ctx& ctx)
{
    const String db = m2m::app::changeSuffix(file, m2m::qt::L(".DB"));
    m2m::initProject(p, m2m::qt::absolutePath(db), m2m::qt::completeBaseName(db));
    m2m::makeRegistry(p);
    return m2m::openProject(p, file, ctx) && p.openedOk;
}

template <class F>
void pending(const char* name, F f)
{
    try {
        f();
    } catch (const m2m::NotPorted& e) {
        std::printf("PENDING %s: %s\n", name, e.what());
    }
}

}  // namespace

TEST_CASE(cog_p1_b1_fields)
{
    pending("cog_p1_b1_fields", [] {
        const auto dir = corpus() / "01_COG_P1_B1";
        const String d = qpath(dir);
        m2m::Project p;
        m2m::Ctx ctx;
        REQUIRE(open(p, qpath(dir / "COG_P1_B1.moproj"), ctx));
        CHECK(p.openedOk);
        CHECK(p.path == d + S(u"/COG_P1_B1.moproj"));
        CHECK(p.title.isEmpty());                         // <Title Name=""/>: text() of an empty element
        CHECK_EQ(p.geotyp, 0);
        CHECK_EQ(p.unit, 1);
        CHECK(p.primaryDb == d + S(u"/COG_P1_B1.DB"));     // ctor value (no .DB in the corpus copy)
        CHECK_EQ(p.subs.size(), 1u);
        CHECK_EQ(p.operations.size(), 5u);
        REQUIRE(p.objects.rows.size() == 7u);
        const int ids[7] = {1, 11, 12, 5, 6, 7, 8};
        const char16_t* names[7] = {u"Workpiece", u"Top 650 New", u"Bot 650 New", u"Left Manipulator 1",
                                    u"Left Manipulator 2", u"Right Manipulator 1", u"Right Manipulator 2"};
        for (int i = 0; i < 7; ++i) {
            const m2m::Item& it = *p.objects.rows[static_cast<size_t>(i)];
            CHECK_EQ(it.id, ids[i]);
            CHECK(it.name == S(names[i]));
            CHECK_EQ(it.objectType, i == 0 ? 1 : 0);
            CHECK(it.hasGeo);
            CHECK(it.hasMesh);
            CHECK(it.matIds.empty());
        }
        CHECK(p.objects.rows[0]->keyPath == d + S(u"/Objects\\Object00001.KEY"));
        CHECK(p.objects.rows[1]->keyPath == d + S(u"/Objects\\Object00011.KEY"));
        REQUIRE(p.materials.rows.size() == 2u);
        CHECK(p.materials.rows[0]->name == S(u"IN718_6um[1650-2200F(900-1200C)]"));
        CHECK_EQ(p.materials.rows[0]->matType, 0);
        CHECK(p.materials.rows[1]->keyPath == d + S(u"/Materials\\Material00002.KEY"));
        REQUIRE(p.lubricants.rows.size() == 1u);
        CHECK(p.lubricants.rows[0]->keyPath == d + S(u"/Lubricants\\Lubricant00001.KEY"));
        REQUIRE(p.equipment.rows.size() == 4u);
        CHECK(p.equipment.rows[3]->name == S(u"Equipment (4)"));
        CHECK(p.equipment.rows[3]->keyPath == d + S(u"/Equipments\\Equipment00004.KEY"));
        CHECK(ctx.warnings.empty());
    });
}

TEST_CASE(heat1_fields)
{
    pending("heat1_fields", [] {
        const auto dir = corpus() / "08_HEAT1";
        const String d = qpath(dir);
        m2m::Project p;
        m2m::Ctx ctx;
        REQUIRE(open(p, qpath(dir / "HEAT1.moproj"), ctx));
        CHECK(p.title.isEmpty());
        CHECK_EQ(p.unit, 1);
        CHECK_EQ(p.operations.size(), 2u);
        CHECK_EQ(p.subs.size(), 1u);
        REQUIRE(p.objects.rows.size() == 1u);
        CHECK(p.objects.rows[0]->name == S(u"Workpiece"));
        CHECK_EQ(p.materials.rows.size(), 1u);
        CHECK_EQ(p.lubricants.rows.size(), 0u);           // <Lubricant_List Number="0"/>
        CHECK_EQ(p.equipment.rows.size(), 1u);
    });
}

TEST_CASE(english_unit)
{
    pending("english_unit", [] {
        const auto dir = corpus() / "46_MO2_Boolean_Oprations";
        m2m::Project p;
        m2m::Ctx ctx;
        REQUIRE(open(p, qpath(dir / "MO2_Boolean_Oprations.moproj"), ctx));
        CHECK_EQ(p.unit, 2);
    });
}

TEST_CASE(not_a_project_suffix)
{
    pending("not_a_project_suffix", [] {
        m2m::Project p;
        m2m::Ctx ctx;
        CHECK(!m2m::openProject(p, S(u"C:/x/project.txt"), ctx));
        CHECK(!p.openedOk);
    });
}

TEST_CASE(every_corpus_project_opens)
{
    pending("every_corpus_project_opens", [] {
        int files = 0;
        for (const auto& dir : std::filesystem::directory_iterator(corpus())) {
            if (!dir.is_directory()) continue;
            for (const auto& f : std::filesystem::directory_iterator(dir.path())) {
                if (f.path().extension() != ".moproj") continue;
                ++files;
                m2m::Project p;
                m2m::Ctx ctx;
                const bool ok = open(p, qpath(f.path()), ctx);
                if (!ok) std::fprintf(stderr, "open failed: %s\n", f.path().string().c_str());
                CHECK(ok);
                CHECK(p.unit == 1 || p.unit == 2);
                CHECK(!p.subs.empty());
            }
        }
        CHECK_EQ(files, 56);
    });
}

int main() { return testkit::run_all(); }
