// ctest-labels: p67-5 commands
// GEOTYP 3 after the mesh is read widens the 2D BCC widths (state table m+0x15f8, entries 0x12
// BCCDEF and 0x13 BCCDFN at m+0x1640 / m+0x1644) from 2 to 3. FUN_1401d0fc0 (GEOTYP's layout
// rebuild) -> 1401d0320 -> 140199650 -> 140180150 grows the node set's int arrays by realloc
// without re-layout: the first n*2 ints stay as read, the grown tail (rows past 2n/3) must read
// as zero, as the original writes it (GENBCC/v_torsion_ctl, GENBCC/torsion: OUT.KEY BCCDEF 1 38 /
// 52 records). Before the fix the port's tail was reused heap (freed KEY text); this test sprays
// freed blocks of the grown size with a pattern before GEOTYP 3 so that a missing zero fill shows.
#include "p67_5_support.hpp"

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>

#include <testkit/check.hpp>

namespace {

using p675::at;

constexpr int kNodes = 100;

p675::fs::path write_text(const char* tag, const std::string& text)
{
    p675::fs::path p = p675::tmp_name(tag);
    std::FILE* f = std::fopen(p.string().c_str(), "wb");
    std::fwrite(text.data(), 1, text.size(), f);
    std::fclose(f);
    return p;
}

std::string mesh_key()
{
    std::string s = "*\n*  DEFORM-2D V10.2 KEYWORD FILE (Qt)\n*\nGEOTYP       1\nOBJTYP       1       2       0\n";
    char line[128];
    std::snprintf(line, sizeof line, "RZ           1 %7d\n", kNodes);
    s += line;
    for (int i = 1; i <= kNodes; ++i) {
        std::snprintf(line, sizeof line, "%8d    %.10E    %.10E\n", i, 0.5 * i, 1.0);
        s += line;
    }
    s += "BCCDEF       1       3       0\n";
    s += "       1       1      -3\n";
    s += "      50       0       1\n";
    s += std::string("") + "     " + std::to_string(kNodes) + "      -2       1\n";
    return s;
}

int run_key(p675::DB_STEP_DATA& m, const std::string& text, const char* tag)
{
    p675::fs::path p = write_text(tag, text);
    p675::Case c;
    c.label = "p675/widen";
    c.inputs["CMD.KEY"] = p;
    int st;
    {
        p675::CmdRunner run(m, c);
        p675::StdoutCapture cap;  // the model's messages
        st = run.run();
    }
    std::error_code ec;
    p675::fs::remove(p, ec);
    return st;
}

// Frees many blocks of `bytes` filled with 0x5a, so that a realloc to that size gets one back.
void spray_heap(std::size_t bytes)
{
    std::vector<void*> blocks;
    for (int i = 0; i < 512; ++i) {
        void* b = std::malloc(bytes);
        std::memset(b, 0x5a, bytes);
        blocks.push_back(b);
    }
    for (void* b : blocks) std::free(b);
}

} // namespace

TEST_CASE(geotyp3_widens_bccdef_with_zero_tail)
{
    auto& m = *new p675::DB_STEP_DATA(3);
    run_key(m, mesh_key(), "p675w_mesh.KEY");
    p675::LLOBJ_DATA* o = at<p675::LLOBJ_DATA*>(m, 0x1380);
    CHECK(o != nullptr);
    if (o == nullptr) return;
    CHECK_EQ(at<std::int32_t>(*o, 0x880), kNodes);
    CHECK_EQ(at<std::int32_t>(m, 0x1640), 2);  // BCCDEF width (entry 0x12)
    CHECK_EQ(at<std::int32_t>(m, 0x1644), 2);  // BCCDFN width (entry 0x13)

    // node set +0x8a0: BCCDEF at +0xf0, BCCDFN at +0xf8 (obj+0x990 / obj+0x998)
    std::vector<std::int32_t> before(at<std::int32_t*>(*o, 0x990), at<std::int32_t*>(*o, 0x990) + 2 * kNodes);
    CHECK_EQ(before[0], 1);
    CHECK_EQ(before[1], -3);
    CHECK_EQ(before[2 * 49 + 1], 1);
    CHECK_EQ(before[2 * (kNodes - 1)], -2);

    spray_heap(static_cast<std::size_t>(kNodes) * 3 * 4);
    run_key(m, "GEOTYP       3\n", "p675w_geotyp.KEY");  // 1401d0fc0(M, 0)

    CHECK_EQ(at<std::int32_t>(m, 0x1640), 3);
    CHECK_EQ(at<std::int32_t>(m, 0x1644), 3);
    for (std::size_t off : {std::size_t{0x990}, std::size_t{0x998}}) {
        const std::int32_t* p = at<std::int32_t*>(*o, off);
        CHECK(p != nullptr);
        if (p == nullptr) continue;
        if (off == 0x990) {
            int kept = 0;
            for (int k = 0; k < 2 * kNodes; ++k) kept += p[k] == before[static_cast<std::size_t>(k)];
            CHECK_EQ(kept, 2 * kNodes);  // realloc keeps the old n*2 ints, no re-layout
        }
        int nonzero = 0;
        for (int k = 2 * kNodes; k < 3 * kNodes; ++k) nonzero += p[k] != 0;
        if (nonzero != 0) std::printf("obj+0x%zx: %d of %d tail ints not zero\n", off, nonzero, kNodes);
        CHECK_EQ(nonzero, 0);
    }
}

int main() { return testkit::run_all(); }
