// Tier 1 for packet p4-06: the DB_STEP_DATA-level GENDB steps 1401c4950 .. 1401c4fa0
// (UNNAME / UENAME blocks, 2D / 3D object dispatch, region widths from the A table).
// Expected values are worked out by hand from the decompile.
// ctest-labels: p4-06 gendb
#include <defdbio/gendb/ext_p4_06.hpp>
#include <defdbio/gendb/p4_06_frame.hpp>
#include <testkit/check.hpp>

#include <cstdio>
#include <cstring>
#include <memory>
#include <string>
#include <vector>

using namespace def::model;
using namespace def::dbio::gendb;

namespace {

struct ZModel {
    std::unique_ptr<std::byte[]> buf{new std::byte[sizeof(DB_STEP_DATA) + 16]()};
    DB_STEP_DATA* get()
    {
        return reinterpret_cast<DB_STEP_DATA*>((reinterpret_cast<std::uintptr_t>(buf.get()) + 15) & ~std::uintptr_t{15});
    }
};

std::uint64_t g_last_addr = 0;
std::uint64_t record(std::uint64_t addr, const std::uint64_t*, int)
{
    g_last_addr = addr;
    return 0;
}

} // namespace

TEST_CASE(node_names_3d_after_numnp_rows)
{
    ZModel zm;
    DB_STEP_DATA* m = zm.get();
    m->dimension = 3;
    std::vector<double> a(200, 0.0);
    m->u058 = a.data();
    field<std::int32_t>(m, 0x1e20) = 11; // A slot 24 (LLND1) start
    m->u13e8 = 5;                         // NUMNP
    char n1[] = "TEMPX";
    char n2[] = "A_VERY_LONG_NAME_THAT_IS_MORE_THAN_SIXTY_FOUR_CHARACTERS_LONG_XXXXXXXX";
    char* names[2] = {n1, n2};
    m->u3398 = 2;
    m->u33a0 = names;
    write_step_node_var_names(m);
    // start = 11 - 1 + 5 * 2 = 20 doubles
    const char* out = reinterpret_cast<const char*>(a.data() + 20);
    CHECK(std::string(out, 64) == std::string("TEMPX") + std::string(59, ' '));
    CHECK(std::string(out + 64, 64) == std::string(n2, 64)); // cut at 64, no padding
    CHECK(a[19] == 0.0);
}

TEST_CASE(elem_names_2d_fixed_slot_and_null_list)
{
    ZModel zm;
    DB_STEP_DATA* m = zm.get();
    m->dimension = 2;
    std::vector<double> a(100, 0.0);
    m->u058 = a.data();
    field<std::int32_t>(m, 0x1ea0) = 4; // A slot 56 start
    m->u33a8 = 1;
    m->u33b0 = nullptr; // no names: blanks only
    write_step_elem_var_names(m);
    const char* out = reinterpret_cast<const char*>(a.data() + 3);
    CHECK(std::string(out, 64) == std::string(64, ' '));
    m->u33a8 = 0; // nothing written
    a.assign(100, 0.0);
    write_step_elem_var_names(m);
    CHECK(a[3] == 0.0);
}

TEST_CASE(objects_3d_dispatch_to_p4_05)
{
    ZModel zm;
    DB_STEP_DATA* m = zm.get();
    m->dimension = 3;
    ext::g_hook = &record;
    write_step_objects(m, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    CHECK_EQ(g_last_addr, std::uint64_t{0x14017b370});
    ext::g_hook = nullptr;
}

TEST_CASE(inter_materials_empty_list_writes_nothing)
{
    ZModel zm;
    DB_STEP_DATA* m = zm.get();
    m->dimension = 2;
    m->phase_pairs.owner = m;
    std::vector<double> lltfm(10, -1.0);
    write_step_inter_materials(m, lltfm.data(), nullptr, nullptr);
    CHECK(lltfm[0] == -1.0);
}

int main()
{
    std::setvbuf(stdout, nullptr, _IONBF, 0);
    return testkit::run_all();
}
