// Step-model utilities the DEF_GUI_MO_64 loader calls as byte-identical twins (header:
// defmodel/gui_twins.hpp). Decompile: deform-rea-work/out/mdt3/f.c.
#include <defmodel/gui_twins.hpp>

#include <defmodel/layout/cls_14017a6c0.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj/object_list.hpp>
#include <defmodel/step/db_step.hpp>

namespace def::model {

// @src 1401cb400  DB_STEP_DATA clear (GUI twin 0x1408246a0)
void step_clear(DB_STEP_DATA& s)
{
    const std::int32_t dim = s.dimension; // 140158e90: *(int*)(s + 0x1c)
    step_free_contents(s);                // 1401cb440
    step_init(s, dim);                    // 1401c8c00
}

// @src 1401cabc0  first object whose kind is not 7 (GUI twin 0x140823e60)
std::int32_t first_real_object_number(DB_STEP_DATA& s)
{
    LLOBJ_DATA* found = nullptr;
    LLOBJ_DATA* o = nullptr;
    std::int32_t i = 1;
    // the loop leaves `o` = found (0) when it runs past u1378
    while ((o = found, i <= s.u1378) &&
           ((o = static_cast<LLOBJ_DATA*>(step_object_at(s, i))) == nullptr || // 1401d6ed0
            !llobj_not_kind7(o))) {                                            // 140160f10
        i = i + 1;
    }
    found = o;
    if (found == nullptr) return 0;
    return field<std::int32_t>(found, 8);
}

// @src 14017ac20  first free object number (GUI twin 0x14084ee40)
std::int32_t first_free_object_number(const Cls_14017a6c0* list)
{
    std::int32_t n = 1;
    while (object_list_find(list, n) != nullptr) n = n + 1; // 14017aaf0
    return n;
}

// @src 1401d6df0  number of the object list's tail (GUI twin 0x1408301c0)
// @src 14017ab50  (Cls_14017a6c0 tail getter: return *(list + 8), inlined here)
std::int32_t last_object_number(const DB_STEP_DATA& s)
{
    if (s.u1378 < 1) return 0;
    const LLOBJ_DATA* t = s.objects.tail; // 14017ab50: *(list + 8)
    if (t == nullptr) return 0;
    return field<std::int32_t>(t, 8);
}

} // namespace def::model
