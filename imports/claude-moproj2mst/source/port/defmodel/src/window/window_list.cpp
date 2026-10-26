// Window list Cls_140269250: the methods p1-06 did not port (packet kw-g3). p1-06 has the ctor,
// reset, set_owner, set_dimension and clear in src/linked/inter_object.cpp.
// Declarations: defmodel/window/window_list.hpp.
#include <defmodel/window/window_list.hpp>

namespace def::model {

// @src 14019a590  count (+0x14)
std::int32_t winlist_count(const Cls_140269250* l) { return l->count; }

// @src 1402694a0  new window (operator new 0x920, ctor 14025c100 with the list owner), appended
LLWINDEF_DATA* winlist_new(Cls_140269250* l)
{
    LLWINDEF_DATA* w = new LLWINDEF_DATA(l->owner);
    winlist_link(l, w);
    return w;
}

// @src 1402693f0  append
// @src 14025c8e0  (w+0x890 setter, inlined)
void winlist_link(Cls_140269250* l, LLWINDEF_DATA* w)
{
    w->u890 = l->u28;
    if (l->u28 != 0) window_set_shape(w, 0xb); // 140263450
    if (l->count == 0) {
        l->tail = w;
        l->head = w;
    } else {
        l->tail->next = w;
        l->tail = w;
    }
    l->count = l->count + 1;
}

// @src 140269900  n-th window (1-based; n < 1 gives the head)
LLWINDEF_DATA* winlist_nth(const Cls_140269250* l, std::int32_t n)
{
    std::int32_t i = 0;
    LLWINDEF_DATA* w = l->head;
    while (w != nullptr && (i = i + 1, i < n)) w = w->next;
    return w;
}

// @src 140269bc0  grow to n windows (each new one gets its 1-based index), return window n
LLWINDEF_DATA* winlist_extend(Cls_140269250* l, std::int32_t n)
{
    while (winlist_count(l) < n) {
        LLWINDEF_DATA* w = winlist_new(l);
        w->index = winlist_count(l);
    }
    return winlist_nth(l, n);
}

// @src 140269340  copy: clear, take +0x18 (dimension) and +0x28, deep-copy every window
void winlist_copy(Cls_140269250* dst, const Cls_140269250* src)
{
    dst->clear(); // 1402699c0
    dst->u18 = src->u18;
    dst->u28 = src->u28;
    for (std::int32_t i = 0; i < winlist_count(src); i = i + 1) {
        LLWINDEF_DATA* w = winlist_new(dst);
        window_copy(w, winlist_nth(src, i + 1)); // 14025c9b0
        w->next = nullptr;
    }
}

} // namespace def::model
