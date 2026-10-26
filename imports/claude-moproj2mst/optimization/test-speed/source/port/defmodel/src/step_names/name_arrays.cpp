// step_names/name_arrays.cpp: DB_STEP_DATA user-variable name arrays (packet kw-g1b).
// Header: defmodel/step/name_arrays.hpp.
#include <defmodel/step/name_arrays.hpp>

#include <cstdlib>

#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/port.hpp>
#include <defmodel/material/state_remap.hpp>
#include <defmodel/step/db_step.hpp>
#include <defmodel/node_access.hpp>

namespace def::model {

namespace {

void realloc_names(DB_STEP_DATA* m, std::int32_t count, char*** arr, const std::int32_t* old_n,
                   const std::int32_t* new_n, void (*free_all)(DB_STEP_DATA*))
{
    (void)count;
    if (*old_n != *new_n) {
        if (*new_n == 0) {
            free_all(m);
        } else {
            for (std::int32_t i = *new_n; i < *old_n; i = i + 1) {
                if ((*arr)[i] != nullptr)
                    std::free((*arr)[i]);
                (*arr)[i] = nullptr;
            }
            *arr = static_cast<char**>(std::realloc(*arr, static_cast<std::size_t>(*new_n) << 3));
            for (std::int32_t i = *old_n; i < *new_n; i = i + 1)
                (*arr)[i] = static_cast<char*>(std::calloc(0x40, 1));
        }
    }
}

}  // namespace

void call_1401d0fc0(DB_STEP_DATA* m, std::int32_t flag)
{
    if (g_hook_1401d0fc0 != nullptr)
        g_hook_1401d0fc0(m, flag);
    else
        step_remap_material_state(*m, flag != 0);  // 1401d0fc0 (kw-g4b)
}

// FUN_1401cb910 (p1-03's step_free_strings_33a0): free the UNNAME names
void free_node_names(DB_STEP_DATA* m) { step_free_strings_33a0(*m); }

// FUN_1401cb660 (p1-03's step_free_strings_33b0): free the UENAME names
void free_element_names(DB_STEP_DATA* m) { step_free_strings_33b0(*m); }

// @src 1401cba80  resize the UNNAME array
void realloc_node_names(DB_STEP_DATA* m, const std::int32_t* old_n, const std::int32_t* new_n)
{
    realloc_names(m, m->u3398, &m->u33a0, old_n, new_n, &free_node_names);
}

// @src 1401cb7d0  resize the UENAME array
void realloc_element_names(DB_STEP_DATA* m, const std::int32_t* old_n, const std::int32_t* new_n)
{
    realloc_names(m, m->u33a8, &m->u33b0, old_n, new_n, &free_element_names);
}

// @src 1401d21c0  UNNAME count
void resize_node_names(DB_STEP_DATA* m, std::int32_t n)
{
    if (m->u3398 != n) {
        std::int32_t old_n = m->u3398;
        std::int32_t new_n = n;
        realloc_node_names(m, &old_n, &new_n);
        m->u3398 = new_n;
        call_1401d0fc0(m, 0);
    }
}

// @src 1401d2090  UENAME count
void resize_element_names(DB_STEP_DATA* m, std::int32_t n)
{
    if (m->u33a8 != n) {
        std::int32_t old_n = m->u33a8;
        std::int32_t new_n = n;
        realloc_element_names(m, &old_n, &new_n);
        m->u33a8 = new_n;
        call_1401d0fc0(m, 0);
    }
}

// @src 1401d2b70  always 1; the 3D test (1400015f0) it makes when +0x1b58 is 0 has no effect
bool names_up_to_date(DB_STEP_DATA* m)
{
    (void)m;
    return true;
}

// 1401d48d0: owner p67-3 (defmodel/node_access.hpp fun_1401d48d0)
void mark_state_var_outputs(DB_STEP_DATA* m, const std::int32_t* id) { fun_1401d48d0(m, id); }

// @src 1401cbbc0  UNNAME count for the writer
std::int32_t node_name_count(DB_STEP_DATA* m)
{
    if (m->dimension == 3) {  // 1400015f0
        // +0x2724: u26c4[24]; the rebuild 1401cf930 is unreachable (1401d2b70 returns 1)
        if (!names_up_to_date(m) && m->u26c4[(0x2724 - 0x26c4) / 4] == 0)
            DEF_NOT_PORTED(0x1401cf930);
    }
    return m->u3398;
}

// @src 1401cbc80  UENAME count for the writer
std::int32_t element_name_count(DB_STEP_DATA* m)
{
    if (m->dimension == 3) {  // 1400015f0
        // +0x2764: u26c4[40]; the rebuild 1401cfc30 is unreachable (1401d2b70 returns 1)
        if (!names_up_to_date(m) && m->u26c4[(0x2764 - 0x26c4) / 4] == 0)
            DEF_NOT_PORTED(0x1401cfc30);
    }
    return m->u33a8;
}

}  // namespace def::model
