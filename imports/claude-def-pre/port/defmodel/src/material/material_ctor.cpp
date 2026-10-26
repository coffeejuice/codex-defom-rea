// Material construction and material-list bookkeeping (packet kw-g4b).
// Member DYNARYs and sub-objects are constructed in declaration order, which is the offset order
// the original ctors call their member ctors in; each body below holds what follows those calls.
#include <defmodel/material/material_ctor.hpp>

#include <cstdio>
#include <initializer_list>
#include <cstdlib>
#include <cstring>
#include <new>

#include <defmodel/dynary_init.hpp>
#include <defmodel/layout/cls_140289930.hpp>
#include <defmodel/layout/cls_14028b850.hpp>
#include <defmodel/layout/cls_14028c500.hpp>
#include <defmodel/mesh2d/material_ops.hpp>

namespace def::model {

// ---- Material -----------------------------------------------------------------------------------

// @src 14028c6f0  ctor: member ctors 0x50 .. 0x2160, owner, next = 0, then 14028cdc0
Material::Material(DB_STEP_DATA* owner_)
{
    owner = owner_;
    next = nullptr;
    material_init(this);
}

// @src 14028cdc0  init
void material_init(Material* m)
{
    m->number = 0;
    std::strcpy(m->name, ""); // str 140539206
    // 14028cde9..14028ce27: reads FUN_140235c10() (DAT_1406fe778 ? 2 : 1) and, with an owner,
    // the simulation-control int at owner+0x750+0xfc (1401c8f40 -> 1401f4090) into a local that
    // nothing reads; no side effect, so not ported.
    m->expand_ref = 0.0;
    m->hdnrul = 0;
    m->mstmtr_flag = 0;
    m->mstmtr_count = 0;
    m->mstmtr_list = nullptr;
    m->frae2h = 0.9; // DAT_14051f220 = 0x3feccccccccccccd
    m->u1ed0 = 0;    // stored as the double 0.0 (all-zero bits)
    m->tmpsol = 0.0;
    m->tmpliq = 0.0;
    m->matden = 0.0;
    m->heatcp_extra = 0;
    m->coarse_flag = 0;
    m->sizemd = 0;
    dynary_init(&m->hdntim, 0x53);
    dynary_init(&m->jominy, 0x52);
    dynary_init(&m->difbnd, 0xaf);
    dynary_init(&m->fperv1, 0);
    dynary_init(&m->fperv2, 0);
    m->skip_key = 0;
    m->sinter_model = 0;
    m->sinter_p[3] = 0.0;
    m->sinter_p[2] = 0.0;
    m->sinter_p[1] = 0.0;
    m->sinter_p[0] = 0.0;
    dynary_init(&m->gbengy, 0x5d);
    m->gbmobi_p = 0.0;
    m->gbmobi_q = 0.0;
    dynary_init(&m->nucsiz, 0xb6);
    material_clear_u004(m);
}

// @src 14028d030
void material_clear_u004(Material* m)
{
    std::int32_t zero = 0;
    std::memcpy(m->pad_0004, &zero, 4);
    std::memcpy(m->pad_0004 + 4, &zero, 4);
}

// ---- DIFCOE block -------------------------------------------------------------------------------

// @src 140289930  ctor
Cls_140289930::Cls_140289930()
{
    mode = 0;
    dynary_init(&d0, 0x29);
    d0.ndim = 0; // param_1[3] = 0: the int at +0xc is d0's ndim, so d0 is a type-0x29 constant
    dynary_init(&d2, 0x27);
    dynary_init(&d3, 0x28);
    user_routine = 0;
}

// ---- GRNDAT block -------------------------------------------------------------------------------

// @src 14028a140  ctor
Cls_14028a140::Cls_14028a140()
{
    flag_d3 = 0;
    flag_d5 = 0;
    flag_d7 = 0;
    flag_d9 = 0;
    dynary_init(&d[0], 0x55);
    dynary_init(&d[1], 0x54);
    dynary_init(&d[2], 0x56);
    dynary_init(&d[3], 0x57);
    dynary_init(&d[4], 0x58);
    dynary_init(&d[5], 0x59);
    dynary_init(&d[6], 0x5a);
    dynary_init(&d[7], 0x5b);
    dynary_init(&d[8], 0x5c);
    g2 = 0.0;
    g1 = 0.0;
}

// @src 14028a4c0  record ctor (3 DYNARYs, next = 0, then 14028a580)
Cls_14028a4c0::Cls_14028a4c0()
{
    next = nullptr;
    grndat_record_init(this);
}

// @src 14028a580  record init
void grndat_record_init(Cls_14028a4c0* r)
{
    dynary_init(&r->sub1, 0x5e);
    dynary_init(&r->sub2, 0x5f);
    dynary_init(&r->sub3, 0x60);
    std::int32_t n = 0xc;
    grndat_record_set_n(r, &n);
    for (std::int32_t i = 0; i < r->n; i = i + 1) { // 14029cd20 reads n each pass
        r->values[i] = 0.0;
    }
    r->d = 0.0;
    r->i = 0;
    r->is_main = 0;
}

// @src 14029ccd0  set the record value count
void grndat_record_set_n(Cls_14028a4c0* r, std::int32_t* n)
{
    if (0xc < *n) {
        *n = 0xc;
        std::fprintf(stdout, "Error - SubbdryVarSize is incorrect.\n"); // FUN_14047903c()+0x30
    }
    r->n = *n;
}

// @src 14028b640  ctor (record, five empty lists, c = 0, record.is_main = 1 via 14029cda0)
// @src 14028af00  list ctor (head = tail = 0, count = 0), inlined for the five lists
Cls_14028b640::Cls_14028b640()
{
    for (RecordList* l : {&l3, &l2, &l1, &l4, &l5}) {
        l->head = nullptr;
        l->tail = nullptr;
        l->count = 0;
    }
    c = 0;
    main.is_main = 1; // 14029cda0(&main, &true)
}

// @src 14028b850  ctor
// @src 14028a3e0  (table4: DYNARY ctor, then init 0xac)
// @src 14028a440  (p[6] = 0)
Cls_14028b850::Cls_14028b850()
{
    dynary_init(&table4, 0xac);
    for (double& v : p) {
        v = 0.0;
    }
    type = 0;
    ref = 0;
}

// @src 14028b050  append a record to a GRNDAT record list
Cls_14028a4c0* grndat_record_list_add(Cls_14028b640::RecordList* l)
{
    Cls_14028a4c0* r = new (std::nothrow) Cls_14028a4c0(); // FUN_1404797d8(0x1f0)
    if (l->count == 0) {
        l->tail = r;
        l->head = r;
    }
    else {
        l->tail->next = r;
        l->tail = r;
    }
    l->count = l->count + 1;
    return r;
}

// ---- TXTURE -------------------------------------------------------------------------------------

// @src 14028c500  ctor
Cls_14028c500::Cls_14028c500()
{
    a = 0;
    b = 1;
    c = 1;
}

// ---- MaterialList -------------------------------------------------------------------------------

// @src 14029c060  add
Material* material_list_add(MaterialList* list)
{
    Material* m = new (std::nothrow) Material(list->owner); // FUN_1404797d8(0x2218)
    if (list->count == 0) {
        list->tail = m;
        list->head = m;
    }
    else {
        list->tail->next = m;
        list->tail = m;
    }
    list->count = list->count + 1;
    return m;
}

// @src 14029ca10  rebuild the number / index arrays
void material_list_rebuild_index(MaterialList* list, bool free_only)
{
    if (list->numbers != nullptr) {
        std::free(list->numbers);
        list->numbers = nullptr;
    }
    if (list->index_of != nullptr) {
        std::free(list->index_of);
        list->index_of = nullptr;
    }
    if (!free_only) {
        list->numbers = static_cast<std::int32_t*>(std::calloc(list->count, 4));
        list->index_of = static_cast<std::int32_t*>(std::calloc(list->count, 4));
        for (std::int32_t i = 0; i < list->count; i = i + 1) {
            const Material* m = material_list_nth(list, i + 1);
            const std::int32_t idx = material_list_index_of(list, m->number);
            list->numbers[i] = m->number;
            list->index_of[i] = idx;
        }
    }
}

// @src 14029c870  index of a material number
std::int32_t material_list_index_of(const MaterialList* list, std::int32_t number)
{
    if (material_list_find(list, number) == nullptr) {
        return -1;
    }
    std::int32_t idx = 0;
    for (const Material* m = list->head; m != nullptr; m = m->next) {
        if (m->number < number) {
            idx = idx + 1;
        }
    }
    return idx;
}

} // namespace def::model
