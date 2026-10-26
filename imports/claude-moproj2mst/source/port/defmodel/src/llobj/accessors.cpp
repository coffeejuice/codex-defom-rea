// Small p1-04 accessors (defmodel/llobj/accessors.hpp).
#include <defmodel/llobj/accessors.hpp>

#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/llobj/ext.hpp>
#include <defmodel/sdctl/cow.hpp>

namespace def::model::llobj {

namespace {
// The list element type does not change size(); the binary's three size functions are one code.
std::size_t list_size_at(const void* self, std::size_t off)
{
    return reinterpret_cast<const CowList<void*>*>(static_cast<const std::byte*>(self) + off)->size();
}
} // namespace

// @src 140158f10  size of the list at +8 (140159c20)
std::size_t list8_size_a(const void* self) { return list_size_at(self, 8); }

// @src 140158f30  size of the list at +8 (140159c60)
std::size_t list8_size_b(const void* self) { return list_size_at(self, 8); }

// @src 140159190  size of the list at +0x18 (140159c40)
std::size_t list18_size(const void* self) { return list_size_at(self, 0x18); }

// @src 140158ec0  list at +8 not empty and its first item's list at +8 not empty
std::int32_t first_item_has_items(void* self)
{
    if (static_cast<std::int32_t>(list8_size_b(self)) < 1)
        return 0;
    std::int32_t idx = 0;
    void* item = ext::f140283f80(self, &idx);
    if (static_cast<std::int32_t>(list8_size_a(item)) < 1)
        return 0;
    return 1;
}

// @src 140158fa0  step+0x1b5c = *value
void set_step_u1b5c(DB_STEP_DATA* step, const std::uint8_t* value) { step->u1b5c = *value; }

} // namespace def::model::llobj
