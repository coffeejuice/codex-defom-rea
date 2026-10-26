// Material / MaterialList helpers ported by p1-08. The Material ctor and DYNARY belong to other
// packets and are not ported yet, so the nodes here are zeroed raw storage with the fields set by
// hand; nothing below destroys a Material. Expected values come from the decompile.
// ctest-labels: p1-08 mesh2d
#include <defmodel/mesh2d/material_ops.hpp>
#include <testkit/check.hpp>

#include <cstdlib>
#include <vector>

using namespace def::model;

namespace {
struct Raw {
    std::vector<void*> blocks;
    ~Raw()
    {
        for (void* p : blocks) std::free(p);
    }
    void* alloc(std::size_t n)
    {
        void* p = std::calloc(1, n);
        blocks.push_back(p);
        return p;
    }
    Material* material(std::int32_t number)
    {
        Material* m = static_cast<Material*>(alloc(sizeof(Material)));
        m->number = number;
        return m;
    }
    MaterialList* list() { return static_cast<MaterialList*>(alloc(sizeof(MaterialList))); }
};

void link(MaterialList* l, const std::vector<Material*>& ms)
{
    l->head = ms.empty() ? nullptr : ms.front();
    for (std::size_t i = 0; i < ms.size(); ++i) ms[i]->next = i + 1 < ms.size() ? ms[i + 1] : nullptr;
    l->tail = ms.empty() ? nullptr : ms.back();
    l->count = static_cast<std::int32_t>(ms.size());
}
} // namespace

TEST_CASE(find_and_nth)
{
    Raw r;
    MaterialList* l = r.list();
    Material* m1 = r.material(5);
    Material* m2 = r.material(7);
    Material* m3 = r.material(5);
    link(l, {m1, m2, m3});
    CHECK(material_list_find(l, 7) == m2);
    CHECK(material_list_find(l, 5) == m1); // first match
    CHECK(material_list_find(l, 9) == nullptr);
    CHECK(material_list_nth(l, 1) == m1);
    CHECK(material_list_nth(l, 0) == m1); // i <= 1 -> head
    CHECK(material_list_nth(l, -3) == m1);
    CHECK(material_list_nth(l, 3) == m3);
    CHECK(material_list_nth(l, 4) == nullptr);
    CHECK_EQ(material_list_count(l), 3);
}

TEST_CASE(reset_and_clear_empty)
{
    Raw r;
    MaterialList* l = r.list();
    l->numbers = reinterpret_cast<std::int32_t*>(0x10);
    l->count = 4;
    material_list_reset(l); // pointers dropped, not freed
    CHECK(l->numbers == nullptr);
    CHECK(l->index_of == nullptr);
    CHECK_EQ(l->count, 0);
    material_list_clear(l);
    CHECK(l->head == nullptr && l->tail == nullptr);
}

TEST_CASE(texture_count_num_odfs_table)
{
    Raw r;
    Cls_14028c500* t = static_cast<Cls_14028c500*>(r.alloc(sizeof(Cls_14028c500)));
    struct Row {
        int a, b, c, n;
    };
    // values from 14028c5a0
    const Row rows[] = {{0, 1, 1, 0},  {1, 1, 1, 10}, {1, 1, 2, 76},  {1, 1, 3, 600},
                        {1, 1, 4, 4784}, {1, 1, 5, 0}, {2, 1, 2, 76},  {1, 2, 1, 0},
                        {3, 1, 1, 7},  {3, 1, 2, 50}, {3, 1, 3, 388}, {3, 1, 4, 3080},
                        {3, 0, 1, 0},  {3, 1, 0, 0},  {4, 1, 1, 0},   {-1, 1, 1, 0}};
    for (const Row& row : rows) {
        t->a = row.a;
        t->b = row.b;
        t->c = row.c;
        CHECK_EQ(texture_count_num_odfs(t), row.n);
    }
}

TEST_CASE(grain_record_counts)
{
    Raw r;
    MaterialList* l = r.list();
    Material* m1 = r.material(1);
    Material* m2 = r.material(2);
    Material* mix = r.material(3);
    link(l, {m1, m2, mix});
    m1->grndat.type = 2;
    m1->grndat.type2.l1.count = 2;
    m1->grndat.type2.l3.count = 1;
    m1->grndat.type2.l5.count = 4; // 1 + 2 + 1 + 4 = 8
    m2->grndat.type = 1;           // not type 2 -> 0
    m2->grndat.type2.l2.count = 9;
    CHECK_EQ(grndat_record_count(&m1->grndat.type2), 8);
    CHECK_EQ(material_grain_record_count(m1), 8);
    CHECK_EQ(material_grain_record_count(m2), 0);

    std::int32_t comps[3] = {1, 99, 1};
    mix->mstmtr_flag = 1;
    mix->mstmtr_count = 3;
    mix->mstmtr_list = comps;
    CHECK(material_is_mixture(mix));
    CHECK(!material_is_mixture(m1));

    std::int32_t n = -1;
    std::int32_t num = 1;
    CHECK_EQ(material_list_grain_record_count(l, &num, &n), 8);
    CHECK_EQ(n, 1);
    num = 3; // 8 + 0 (99 missing) + 8
    CHECK_EQ(material_list_grain_record_count(l, &num, &n), 16);
    CHECK_EQ(n, 3);
    n = -1;
    num = 42;
    CHECK_EQ(material_list_grain_record_count(l, &num, &n), 0);
    CHECK_EQ(n, -1);
    mix->mstmtr_list = nullptr;
}

TEST_CASE(num_odfs)
{
    Raw r;
    Material* m = r.material(1);
    CHECK_EQ(material_num_odfs(m), 0); // a == 0
    m->txture.a = 3;
    m->txture.b = 1;
    m->txture.c = 2;
    CHECK_EQ(material_num_odfs(m), 50);
}

int main() { return testkit::run_all(); }
