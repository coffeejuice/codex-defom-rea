// DB_STEP_DATA: the model root (packet p1-03). Layout: defmodel/layout/db_step_data.hpp.
#include <defmodel/step/db_step.hpp>

#include <cstdlib>
#include <cstring>

#include "ext.hpp"

namespace def::model {

// ---- ACTION_DB_STEP --------------------------------------------------------------------------

// @src 1401d8380  ctor: text[0] = 0
ACTION_DB_STEP::ACTION_DB_STEP() { text[0] = '\0'; }

// @src 1401d83a0  dtor (empty)
ACTION_DB_STEP::~ACTION_DB_STEP() = default;

// ---- ctor, dtor, slot 0 ----------------------------------------------------------------------

// @src 1401c8950  ctor. Bases and members are constructed by C++ in the binary's order:
// 1401bae10, 1401d8380, vptr, 1401e7b10 (sim_ctrl), 14029b4c0(0), 140339c90(0), 14017a6c0(0) x2,
// 1401fa020(0), 14032f100.
DB_STEP_DATA::DB_STEP_DATA(std::int32_t dim)
{
    sim_ctrl_set_owner(sim_ctrl, this);
    objects_set_owner(objects, this);
    objects_set_owner(objects2, this);
    inter_obj_set_owner(inter_obj, this);
    materials_set_owner(materials, this);
    inter_matl_set_owner(phase_pairs, this);
    db = nullptr;
    u1b58 = 1;
    u1b59 = 1;
    u1b5a = 0;
    u1b5b = 0;
    std::strcpy(u1748, "DEFORM");
    std::strcpy(u340, "DEFORM");
    u1b5c = 0; // 140158fa0 (p1-04 setter of 0x1b5c, inlined), called with 0
    u1740 = 0;
    u1744 = 0;
    step_init(*this, dim);
    u1748[0] = '\0';
    step_clear_u2f89(*this);
    step_clear_u2f88(*this);
    step_clear_partner(*this);
    step_clear_u33f8(*this);
}

// @src 1401cb2c0  dtor: free the contents (or, when u173c is set, only reset the lists), then
// C++ destroys members and bases in reverse order: 14032f140, 1401fa060, 14017a740 x2,
// 140339d10, 14029b550, 1401e7cb0, 1401d83a0, 1401baf10.
DB_STEP_DATA::~DB_STEP_DATA()
{
    if (u173c == 0) {
        step_free_contents(*this);
    } else {
        ext::fun_14029b500(&materials);
        ext::fun_140339cd0(&phase_pairs);
        ext::fun_14017a700(&objects);
        ext::fun_14017a700(&objects2);
        ext::fun_1401fa080(&inter_obj);
    }
}

// @src 1401c8ba0  slot 0: a new dimension frees everything and re-inits
void DB_STEP_DATA::set_dimension(const std::int32_t* dim)
{
    const std::int32_t cur = dimension; // 140158e90 (getter of 0x1c, inlined)
    if (cur != *dim) {
        DB_WRITE_VAR::set_dimension(dim);
        step_free_contents(*this);
        step_init(*this, *dim);
    }
}

// ---- init and free ---------------------------------------------------------------------------

// @src 1401c8c00  init for a dimension
void step_init(DB_STEP_DATA& s, std::int32_t dim)
{
    s.DB_WRITE_VAR::set_dimension(&dim); // 1401c5700, direct call
    s.u740 = 0;
    s.u744 = -1;
    s.u748 = 0;
    sim_ctrl_init(s.sim_ctrl);
    s.n_materials = 0;
    ext::fun_14029b500(&s.materials);
    ext::fun_140339cd0(&s.phase_pairs);
    s.u1378 = 0;
    ext::fun_14017a700(&s.objects);
    ext::fun_14017a700(&s.objects2);
    ext::fun_1401fa080(&s.inter_obj);
    step_clear_u1b48(s);
    s.u13e8 = 0;
    s.u13ec = 0;
    s.u13f0 = 0;
    s.n_phase_on = 0;
    s.u13f8 = 0;
    s.u3398 = 0;
    s.u33a0 = nullptr;
    s.u33a8 = 0;
    s.u33b0 = nullptr;
    s.u33d8 = 1;
    s.u33c8 = 1;
    s.u1400 = 0;
    s.u1401 = 0;
    if (dim == 3) {
        step_setup_array_sizes(s, 3);
    } else {
        step_setup_array_sizes(s, 2);
    }
    s.u1734 = 0;
    s.u1738 = 1;
    s.u173c = 0;
    s.u1b5d = 1;
    s.u13fc = 0;
    s.u1b60 = 0;
    s.step_version = 0;
    s.u1b74 = 0;
    s.u1b78 = nullptr;
    s.u1b80 = nullptr;
    s.u1b88 = nullptr;
    s.u1b6c = 0;
    s.u1b68 = 0;
    s.u1b70 = 0;
    step_clear_address_tables(s);
    step_clear_gendb_tables(s);
    s.u2220 = nullptr;
    s.u2228 = nullptr;
    s.u2230 = nullptr;
    s.u2238 = nullptr;
    s.u2240 = nullptr;
    s.u2248 = nullptr;
    s.u2250 = nullptr;
    s.u2258 = nullptr;
}

// @src 1401ca960
void step_clear_u1b48(DB_STEP_DATA& s)
{
    s.u1b48 = 0;
    s.u1b4c = 0;
    s.u1b50 = 0;
}

// @src 1401cb170  MAR and A address tables = 0
void step_clear_address_tables(DB_STEP_DATA& s)
{
    for (std::int32_t i = 0; i < 0x8c; ++i) {
        s.u1b90[i] = 0;
    }
    for (std::int32_t i = 0; i < 0x118; ++i) {
        s.u1dc0[i] = 0;
    }
}

// @src 1401cb1f0  GENDB tables = 0
void step_clear_gendb_tables(DB_STEP_DATA& s)
{
    for (std::int32_t i = 0; i < 0x8c; ++i) {
        s.u2260[i] = 0;
        s.u2494[i] = 0;
    }
    s.u2490 = 0;
    for (std::int32_t i = 0; i < 0x118; ++i) {
        s.u26c4[i] = 0;
        s.u2b28[i] = 0;
    }
    s.u2b24 = 0;
}

namespace {

void free_and_null(void*& p)
{
    if (p != nullptr) {
        std::free(p);
    }
    p = nullptr;
}

} // namespace

// @src 1401cb440  free arrays and list contents
void step_free_contents(DB_STEP_DATA& s)
{
    step_free_strings_33a0(s);
    step_free_strings_33b0(s);
    if (s.u1b80 != nullptr) {
        std::free(s.u1b80);
    }
    s.u1b80 = nullptr;
    free_and_null(s.u1b78);
    free_and_null(s.u1b88);
    step_clear_gendb_tables(s);
    free_and_null(s.u2240);
    free_and_null(s.u2248);
    free_and_null(s.u2250);
    free_and_null(s.u2258);
    sim_ctrl_clear(s.sim_ctrl);
    ext::fun_14029c310(&s.materials);
    ext::fun_14033a120(&s.phase_pairs);
    ext::fun_14017ab70(&s.objects);
    ext::fun_14017ab70(&s.objects2);
    s.u1378 = 0;
    ext::fun_1401fade0(&s.inter_obj);
    s.u740 = 0;
    s.u744 = 0;
}

// @src 1401cb660  free the string array at 0x33b0 (u33a8 entries)
void step_free_strings_33b0(DB_STEP_DATA& s)
{
    if (s.u33b0 != nullptr) {
        for (std::int32_t i = 0; i < s.u33a8; ++i) {
            if (s.u33b0[i] != nullptr) {
                std::free(s.u33b0[i]);
            }
            s.u33b0[i] = nullptr;
        }
        if (s.u33b0 != nullptr) {
            std::free(s.u33b0);
        }
        s.u33b0 = nullptr;
    }
}

// @src 1401cb910  free the string array at 0x33a0 (u3398 entries)
void step_free_strings_33a0(DB_STEP_DATA& s)
{
    if (s.u33a0 != nullptr) {
        for (std::int32_t i = 0; i < s.u3398; ++i) {
            if (s.u33a0[i] != nullptr) {
                std::free(s.u33a0[i]);
            }
            s.u33a0[i] = nullptr;
        }
        if (s.u33a0 != nullptr) {
            std::free(s.u33a0);
        }
        s.u33a0 = nullptr;
    }
}

// ---- array size table u15f8 ------------------------------------------------------------------
// u15f8[k] is the int at 0x15f8 + 4k; the index is the array number step_has_array() takes.
// Simulation-control ints are read by their DB_STEP_DATA offset as the decompile does
// (0x750 + member offset).

// @src 1401d09c0  fill the array size table for a dimension
void step_setup_array_sizes(DB_STEP_DATA& s, std::int32_t dim)
{
    std::int32_t* t = s.u15f8;
    for (std::int32_t i = 0; i < 0x4f; ++i) {
        t[i] = 1;
    }
    if (dim == 2) {
        t[37] = 4; // 0x168c
        t[62] = 2; // 0x16f0
    } else {
        t[37] = 0;
        t[62] = 3;
        t[46] = 0; // 0x16b0
    }
    t[0] = dim;  // 0x15f8
    t[1] = dim;
    t[2] = dim;
    t[3] = dim;
    t[4] = dim;
    t[5] = dim;
    t[6] = dim;  // 0x1610
    t[18] = dim; // 0x1640
    t[19] = dim; // 0x1644
    t[25] = dim; // 0x165c
    if (!step_has_array(s, 0x18, nullptr)) {
        t[24] = 0; // 0x1658
    } else {
        t[24] = 5;
    }
    t[38] = dim << 1; // 0x1690
    t[26] = dim << 1; // 0x1660
    t[39] = dim << 1; // 0x1694
    std::int32_t sum = 0;
    for (std::int32_t i = 0; i < 5; ++i) {
        sum = sum + field<std::int32_t>(&s, 0x86c + static_cast<std::size_t>(i) * 4); // stnout[0..4]
    }
    std::int32_t n;
    if (sum < 2) {
        n = 1;
    } else {
        n = sum;
    }
    sum = n;
    const std::int32_t extra = field<std::int32_t>(&s, 0x880) + field<std::int32_t>(&s, 0x884); // stnout[5], [6]
    t[40] = extra + n * dim * 2; // 0x1698
    t[57] = dim << 1;            // 0x16dc
    t[47] = 2;                   // 0x16b4
    if (!step_has_array(s, 0x31, nullptr)) {
        t[49] = 0; // 0x16bc
        t[50] = 0;
        t[51] = 0;
        t[52] = 0; // 0x16c8
    } else {
        t[49] = s.n_materials;
        t[50] = s.n_materials;
        t[51] = s.n_phase_on;
        t[52] = s.n_phase_on;
    }
    std::int32_t size = 0;
    if (!step_has_array(s, 0x35, &size)) {
        t[53] = 0; // 0x16cc
    } else {
        t[53] = size;
    }
    if (!step_has_array(s, 0x1d, &size)) {
        t[29] = 0; // 0x166c
    } else {
        t[29] = size;
    }
    if (!step_has_array(s, 0x36, &size)) {
        t[54] = 0; // 0x16d0
    } else {
        t[54] = size;
    }
    t[56] = 2; // 0x16d8
    t[58] = 2; // 0x16e0
    if (dim == 3) {
        t[59] = 3; // 0x16e4
    } else {
        t[59] = 2;
    }
    if (!step_has_array(s, 0x37, &size)) {
        t[55] = 0; // 0x16d4
    } else {
        t[55] = size;
    }
    if (dim == 3) {
        if (!step_has_array(s, 0x2e, nullptr)) {
            t[46] = 0; // 0x16b0
        } else {
            t[46] = 9;
        }
    }
    t[17] = s.u3398; // 0x163c
    t[60] = s.u33a8; // 0x16e8
    if (dim == 2 && field<std::int32_t>(&s, 0x850) == 3) { // sim_ctrl.geotyp
        t[2] = 3;
        t[3] = 3;
        t[4] = 3;
        t[18] = 3;
        t[19] = 3;
        t[38] = 6;
        t[26] = 6;
        t[39] = 6;
        t[40] = sum * 6 + extra;
        t[57] = 6;
    }
    if (field<std::int32_t>(&s, 0x888) == 0 || field<std::int32_t>(&s, 0x888) == 3) { // elmnod[0]
        t[28] = 0; // 0x1668
    }
    if (field<std::int32_t>(&s, 0x88c) == 0 || field<std::int32_t>(&s, 0x88c) == 3) { // elmnod[1]
        t[27] = 0; // 0x1664
    }
    if (field<std::int32_t>(&s, 0x890) == 0 || field<std::int32_t>(&s, 0x890) == 3) { // elmnod[2]
        t[26] = 0; // 0x1660
    }
}

// @src 1401d64f0  is array idx present in this model; *size (when non-null) gets its width
bool step_has_array(DB_STEP_DATA& s, std::int32_t idx, std::int32_t* size)
{
    std::int32_t dim = s.dimension; // 140158e90
    select_index_tables(&dim);
    MaterialList* ml = &s.materials;
    bool r;
    switch (idx - 0x18) {
    case 0:
        r = true;
        break;
    default:
        r = 0 < s.u15f8[idx];
        break;
    case 5: // 0x1d
        if (field<std::int32_t>(&s, 0x864) == 0) { // trans[4]
            r = false;
        } else {
            bool found = false;
            for (std::int32_t i = 0; i < ml->count; ++i) { // 140199c50: count at +0x10
                Material* m = ext::fun_14029c260(ml, i + 1);
                if (m != nullptr && field<std::int32_t>(m, 0x17e8) == 2) {
                    found = true;
                    break;
                }
            }
            r = found;
            if (!found) {
                if (size != nullptr) {
                    *size = 0;
                }
            } else {
                if (field<std::int32_t>(&s, 0x898) != 1) { // elmnod[4]
                    field<std::int32_t>(&s, 0x898) = 1;
                }
                if (size != nullptr) {
                    std::int32_t k = 2;
                    *size = step_fun_1401d70c0(s, &k);
                    if (*size == 0 && 0 < s.u1dc0[g_a_index[101]]) { // DAT_1406e8464
                        *size = s.u1dc0[g_a_index[101]];
                    }
                }
            }
        }
        break;
    case 0x16: // 0x2e
        if (s.dimension == 2) {
            r = true;
        } else {
            for (std::int32_t i = 0; i < ml->count; ++i) {
                Material* m = ext::fun_14029c260(ml, i + 1);
                if (m != nullptr) {
                    const std::int32_t t = m->aniso.t; // +0x1e40
                    if (t == 1 || t == 2 || t == 4 || t == 5) {
                        return true;
                    }
                }
            }
            r = false;
        }
        break;
    case 0x19:
    case 0x1a:
    case 0x1b:
    case 0x1c: // 0x31..0x34
        if (field<std::int32_t>(&s, 0x85c) == 0) { // trans[2]
            for (std::int32_t i = 0; i < ml->count; ++i) {
                Material* m = ext::fun_14029c260(ml, i + 1);
                if (m != nullptr && m->mstmtr_flag == 1) {
                    return true;
                }
            }
            r = false;
        } else {
            r = true;
        }
        break;
    case 0x1d: // 0x35
        if (field<std::int32_t>(&s, 0x864) == 0) {
            r = false;
        } else {
            r = false;
            for (std::int32_t i = 0; i < ml->count; ++i) {
                Material* m = ext::fun_14029c260(ml, i + 1);
                if (m != nullptr) {
                    const std::int32_t v = field<std::int32_t>(m, 0x17e8);
                    if (v == 1 || v == 4 || v == 5) {
                        if (size != nullptr) {
                            std::int32_t k = v;
                            *size = step_fun_1401d70c0(s, &k);
                        }
                        r = true;
                        break;
                    }
                }
            }
            if (r && field<std::int32_t>(&s, 0x898) != 0) {
                field<std::int32_t>(&s, 0x898) = 0;
            }
        }
        break;
    case 0x1e: { // 0x36
        const std::int32_t n = step_sum_1401d6c60(s);
        if (n < 1) {
            r = false;
        } else {
            if (size != nullptr) {
                *size = n;
            }
            r = true;
        }
        break;
    }
    case 0x1f: { // 0x37
        const std::int32_t n = step_sum_1401d6b30(s);
        if (n < 1) {
            r = false;
        } else {
            if (size != nullptr) {
                *size = n;
            }
            r = true;
        }
        break;
    }
    }
    return r;
}

// @src 1401d6b30  sum of the material counts (a mixture adds its components' counts)
std::int32_t step_sum_1401d6b30(DB_STEP_DATA& s)
{
    std::int32_t sum = 0;
    for (std::int32_t i = 0; i < s.n_materials; ++i) {
        Material* m = ext::fun_14029c260(&s.materials, i + 1);
        if (m != nullptr) {
            if (m->mstmtr_flag == 0) {
                sum = sum + ext::fun_14029b300(m);
            } else {
                for (std::int32_t j = 0; j < m->mstmtr_count; ++j) {
                    Material* c = step_material_by_id(s, m->mstmtr_list[j]);
                    if (c != nullptr) {
                        sum = sum + ext::fun_14029b300(c);
                    }
                }
            }
        }
    }
    return sum;
}

// @src 1401d6c60  sum over materials of a width per SIZEMD mode
std::int32_t step_sum_1401d6c60(DB_STEP_DATA& s)
{
    std::int32_t sum = 0;
    for (std::int32_t i = 0; i < s.n_materials; ++i) {
        Material* m = ext::fun_14029c260(&s.materials, i + 1);
        if (m != nullptr) {
            switch (m->sizemd) {
            case 1: sum = sum + 1; break;
            case 2: sum = sum + 4; break;
            case 3: sum = sum + 3; break;
            case 4: sum = sum + 2; break;
            case 5: sum = sum + 4; break;
            case 6: sum = sum + 1; break;
            default: break;
            }
        }
    }
    return sum;
}

// @src 1401d6ed0  object n (1-based) of the first object list
void* step_object_at(DB_STEP_DATA& s, std::int32_t n) { return ext::fun_14017aa80(&s.objects, n); }

// @src 1401d7060  material with number id
Material* step_material_by_id(DB_STEP_DATA& s, std::int32_t id) { return ext::fun_14029c2c0(&s.materials, id); }

// @src 1401d70c0  width for a material kind: 2 -> 13 * n + 2 * m from 1401d71b0; -1, 1, 4, 5 -> 16
std::int32_t step_fun_1401d70c0(DB_STEP_DATA& s, const std::int32_t* kind)
{
    std::int32_t m = 0;
    const std::int32_t k = *kind;
    if (k != -1 && k != 1) {
        if (k == 2) {
            const std::int32_t n = step_fun_1401d71b0(s, &m);
            return n * 0xd + m * 2;
        }
        if (k < 4) {
            return 0;
        }
        if (5 < k) {
            return 0;
        }
    }
    return 0x10;
}

// @src 1401d71b0  first nonzero 1401d7250 over the objects 1..u1378
std::int32_t step_fun_1401d71b0(DB_STEP_DATA& s, std::int32_t* out)
{
    std::int32_t r;
    std::int32_t i = 1;
    while (true) {
        if (s.u1378 < i) {
            return 0;
        }
        void* obj = step_object_at(s, i);
        if (obj != nullptr && (r = step_fun_1401d7250(s, obj, out)) != 0) {
            break;
        }
        i = i + 1;
    }
    return r;
}

// @src 1401d7250  query the material list for the object's material
std::int32_t step_fun_1401d7250(DB_STEP_DATA& s, void* obj, std::int32_t* out)
{
    const std::int32_t id = ext::fun_1401fc040(obj);
    return step_fun_1401d72a0(s, &id, out);
}

// @src 1401d72a0  14029bcd0 on the material list
std::int32_t step_fun_1401d72a0(DB_STEP_DATA& s, const std::int32_t* id, std::int32_t* out)
{
    return ext::fun_14029bcd0(&s.materials, id, out);
}

// ---- small setters ---------------------------------------------------------------------------

// @src 1401d7ef0
void step_clear_u2f89(DB_STEP_DATA& s) { s.u2f89 = 0; }

// @src 1401d7f10
void step_clear_u2f88(DB_STEP_DATA& s)
{
    s.u2f88 = 0;
    s.u2f89 = 0;
}

// @src 1401d7fd0  no companion model
void step_clear_partner(DB_STEP_DATA& s)
{
    s.companion = nullptr;
    s.use_companion = 0;
}

// @src 1401d8200
void step_clear_u33f8(DB_STEP_DATA& s)
{
    s.u33f8 = 0;
    s.u3400 = 0;
}

// @src 1401d8300  material list owner (+0x28)
void materials_set_owner(MaterialList& l, DB_STEP_DATA* owner) { l.owner = owner; }
// @src 1401d8320  phase-pair list owner (+0x18)
void inter_matl_set_owner(PhasePairList& l, DB_STEP_DATA* owner) { l.owner = owner; }
// @src 1401d8340  object list owner (+0x18)
void objects_set_owner(Cls_14017a6c0& l, DB_STEP_DATA* owner) { l.owner = owner; }
// @src 1401d8360  inter-object list owner (+0x18)
void inter_obj_set_owner(Cls_1401fa020& l, DB_STEP_DATA* owner) { l.owner = owner; }

} // namespace def::model
