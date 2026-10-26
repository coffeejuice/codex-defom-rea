// Material, MaterialList and Material sub-object destructors and helpers (packet p1-08).
// Member destructors (DYNARY 14012f410) run in reverse declaration order, which is the order
// the original destructors call them in; each body below holds only what the original does
// before that.
#include <defmodel/mesh2d/material_ops.hpp>

#include <cstdio>
#include <cstdlib>

#include <defmodel/layout/cls_140288ae0.hpp>
#include <defmodel/layout/cls_140289930.hpp>
#include <defmodel/layout/cls_140289b80.hpp>
#include <defmodel/layout/cls_140289fe0.hpp>
#include <defmodel/layout/cls_14028a4c0.hpp>
#include <defmodel/layout/cls_14028bcc0.hpp>
#include <defmodel/layout/cls_14028c040.hpp>

namespace def::model {

// ---- destructors -------------------------------------------------------------------------------

// @src 14028cb70  Material dtor: free the MSTMTR list, then the members (0x2160 .. 0x50)
// @src 14029e210  Material scalar deleting dtor (the list calls it with 1: delete)
Material::~Material()
{
    if (mstmtr_list != nullptr) std::free(mstmtr_list);
    mstmtr_list = nullptr;
}

// @src 14029cdc0  dtor: DYNARYs 0x598 .. 0x28
Cls_140288ae0::~Cls_140288ae0() = default;

// @src 14029ce90  dtor: DYNARYs 0xf8, 0x80, 0x8
Cls_140289930::~Cls_140289930() = default;

// @src 14029cee0  dtor: DYNARYs 0x3d0 .. 0x8
Cls_140289b80::~Cls_140289b80() = default;

// @src 14029cff0  dtor: DYNARY at 0x8
Cls_140289fe0::~Cls_140289fe0() = default;

// @src 14028a320  dtor: DYNARYs d[8] .. d[0] (0x3d0 .. 0x10)
Cls_14028a140::~Cls_14028a140() = default;

// @src 14028a530  dtor: DYNARYs 0x168, 0xf0, 0x78
// @src 14029cd60  scalar deleting dtor (record lists delete their nodes with it)
Cls_14028a4c0::~Cls_14028a4c0() = default;

// @src 14028b6f0  dtor: clear the five record lists (0x258, 0x240, 0x228, 0x210, 0x1f8), then
//                 the main record (14028a530)
Cls_14028b640::~Cls_14028b640()
{
    grndat_record_list_clear(&l5);
    grndat_record_list_clear(&l4);
    grndat_record_list_clear(&l1);
    grndat_record_list_clear(&l2);
    grndat_record_list_clear(&l3);
}

// @src 14028b8e0  dtor: p[6] (14028a4b0, empty), table4 (14028a420), type2, type1
// @src 14028a420  (DYNARY dtor of table4 at +0x6c8; the member dtor call)
// @src 14028a4b0  (empty dtor of the double[6] at +0x740; nothing to do)
Cls_14028b850::~Cls_14028b850() = default;

// @src 14028bd20  dtor: DYNARYs 0x168, 0xf0, 0x78, 0x0
Cls_14028bcc0::~Cls_14028bcc0() = default;

// @src 14029cfa0  dtor: DYNARYs 0x138, 0xc0, 0x40
Cls_14028c040::~Cls_14028c040() = default;

// @src 14028c540  dtor (empty)
Cls_14028c500::~Cls_14028c500() = default;

// ---- MaterialList ------------------------------------------------------------------------------

// @src 14029b4c0  ctor
MaterialList::MaterialList(DB_STEP_DATA* owner_)
{
    owner = owner_;
    material_list_reset(this);
}

// @src 14029b550  dtor
MaterialList::~MaterialList()
{
    material_list_clear(this);
    if (numbers != nullptr) std::free(numbers);
    numbers = nullptr;
    if (index_of != nullptr) std::free(index_of);
    index_of = nullptr;
}

// @src 14029b500  reset
void material_list_reset(MaterialList* list)
{
    list->tail = nullptr;
    list->head = nullptr;
    list->count = 0;
    list->numbers = nullptr;
    list->index_of = nullptr;
}

// @src 14029c310  clear
void material_list_clear(MaterialList* list)
{
    Material* n = list->head;
    while (n != nullptr) {
        Material* next = n->next;
        delete n; // 14029e210(n, 1)
        n = next;
    }
    list->tail = nullptr;
    list->head = nullptr;
    list->count = 0;
}

// @src 14029c2c0  find by material number
Material* material_list_find(const MaterialList* list, std::int32_t number)
{
    Material* n = list->head;
    while (n != nullptr && n->number != number) n = n->next;
    return n;
}

// @src 14029c260  1-based nth node
Material* material_list_nth(const MaterialList* list, std::int32_t i)
{
    std::int32_t k = 0;
    Material* n = list->head;
    while (n != nullptr && (k = k + 1, k < i)) n = n->next;
    return n;
}

// @src 14029cd50  count (+0x10)
std::int32_t material_list_count(const MaterialList* list) { return list->count; }

// @src 14029bcd0  GRNDAT record count of a material, summed over mixture components
std::int32_t material_list_grain_record_count(const MaterialList* list, const std::int32_t* number,
                                              std::int32_t* n_out)
{
    std::int32_t sum = 0;
    const Material* m = material_list_find(list, *number);
    if (m == nullptr) return 0;
    if (!material_is_mixture(m)) {
        *n_out = 1;
        return material_grain_record_count(m);
    }
    for (std::int32_t i = 0; i < m->mstmtr_count; i = i + 1) {
        const Material* c = material_list_find(list, m->mstmtr_list[i]);
        std::int32_t n = 0;
        if (c != nullptr) n = material_grain_record_count(c);
        sum = sum + n;
    }
    *n_out = m->mstmtr_count;
    return sum;
}

// ---- Material ----------------------------------------------------------------------------------

// @src 14028d0a0  is mixture (MSTMTR)
bool material_is_mixture(const Material* m) { return m->mstmtr_flag == 1; }

// @src 14029b2c0  GRNDAT type-2 record count
std::int32_t material_grain_record_count(const Material* m)
{
    if (m->grndat.type == 2) return grndat_record_count(&m->grndat.type2);
    return 0;
}

// @src 14029b300  number of ODFs of the material's texture
std::int32_t material_num_odfs(const Material* m)
{
    if (m->txture.a == 0) return 0;
    return texture_count_num_odfs(&m->txture);
}

// @src 14028c5a0  TEXTURE_DATA::countNumODFs
std::int32_t texture_count_num_odfs(const Cls_14028c500* t)
{
    if (t->a == 0) return 0;
    if (t->a == 1 || t->a == 2) {
        if (t->b != 1) return 0;
        if (t->c == 1) return 10;
        if (t->c == 2) return 0x4c;
        if (t->c == 3) return 600;
        if (t->c == 4) return 0x12b0;
        return 0;
    }
    if (t->a == 3) {
        if (t->b != 1) return 0;
        if (t->c == 1) return 7;
        if (t->c == 2) return 0x32;
        if (t->c == 3) return 0x184;
        if (t->c == 4) return 0xc08;
        return 0;
    }
    // __iob_func()+0x30 is stdout
    std::fprintf(stdout, "Error in TEXTURE_DATA::countNumODFs()\n");
    return 0;
}

// ---- GRNDAT type-2 part ------------------------------------------------------------------------

// @src 14028b780  record count: main record + five lists (summed in the original's order)
std::int32_t grndat_record_count(const Cls_14028b640* g)
{
    std::int32_t c1 = grndat_record_list_count(&g->l1);
    std::int32_t c2 = grndat_record_list_count(&g->l2);
    std::int32_t c3 = grndat_record_list_count(&g->l3);
    std::int32_t c4 = grndat_record_list_count(&g->l4);
    std::int32_t c5 = grndat_record_list_count(&g->l5);
    return c1 + 1 + c2 + c3 + c4 + c5;
}

// part of @src 14029cd50  (count at +0x10, shared with the material list)
std::int32_t grndat_record_list_count(const Cls_14028b640::RecordList* l) { return l->count; }

// @src 14028af40  record list dtor -> clear
// @src 14028b240  record list clear
void grndat_record_list_clear(Cls_14028b640::RecordList* l)
{
    Cls_14028a4c0* n = l->head;
    while (n != nullptr) {
        Cls_14028a4c0* next = n->next;
        delete n; // 14029cd60(n, 1)
        n = next;
    }
    l->tail = nullptr;
    l->head = nullptr;
    l->count = 0;
}

} // namespace def::model
