// Cube attribute lists (packet p67-7, group "cube"; GENCTC_3d_search.md sec. 3.1, 3.3): per
// entity (node or polygon) an integer and a real singly linked list of fields; a node is
// cube::ListNode with +0x00 field id, +0x04 count, +0x08 data, +0x10 next (0x18 bytes, malloc).
#include <defcmd/ctc3d/cube.hpp>

#include <cstdlib>
#include <cstring>

namespace def::cmd::cube {

namespace {

inline std::size_t bytes(i32 count, std::size_t elem) { return static_cast<std::size_t>(static_cast<std::int64_t>(count)) * elem; }

template <class T>
ListNode* attr_new(i32 field, const T* data, i32 count)
{
    auto* p = static_cast<ListNode*>(std::malloc(0x18));
    if (p == nullptr) {
        if constexpr (sizeof(T) == 4) {
            fun_14039d3a0("%s Error when allocating %s memory of size %d\n", "ERROR    ", "Integer Data for Polygon/Element", 1); // str 1406272d0
        } else {
            fun_14039d3a0("%s Error when allocating %s memory of size %d\n", "ERROR    ", "Real Data for Polygon/Element", 1); // str 140627330
        }
    }
    node_field(p) = field;
    node_count(p) = count;
    if constexpr (sizeof(T) == 4) {
        p->data = fun_140256dc0(count);
    } else {
        p->data = fun_140257150(count);
    }
    std::memcpy(p->data, data, bytes(count, sizeof(T)));
    p->next = nullptr;
    return p;
}

template <class T>
ListNode* attr_replace(ListNode* head, ListNode* node, ListNode* prev, const T* data, i32 count)
{
    ListNode* next = node->next;
    if (node_count(node) == count) {
        std::memcpy(node->data, data, bytes(count, sizeof(T)));
        return head;
    }
    if constexpr (sizeof(T) == 4) {
        node->data = fun_140256e20(static_cast<i32*>(node->data), count);
    } else {
        node->data = fun_1402571b0(node->data, count);
    }
    std::memcpy(node->data, data, bytes(count, sizeof(T)));
    node_count(node) = count;
    ListNode* r = node;
    if (prev != nullptr) {
        prev->next = node;
        r = head;
    }
    node->next = next;
    return r;
}

template <class T, ListNode* (*New)(i32, const T*, i32), ListNode* (*Replace)(ListNode*, ListNode*, ListNode*, const T*, i32)>
ListNode* attr_set(ListNode* head, i32 field, const T* data, i32 count)
{
    if (head == nullptr) {
        return New(field, data, count);
    }
    ListNode* prev = nullptr;
    for (ListNode* p = head; p != nullptr; p = p->next) {
        if (node_field(p) == field) {
            return Replace(head, p, prev, data, count);
        }
        prev = p;
    }
    prev->next = New(field, data, count);
    return head;
}

template <class T>
void attr_get(ListNode* head, i32 field, T* buf, i32* count)
{
    ListNode* p = head;
    while (true) {
        if (p == nullptr) {
            *count = 0;
            return;
        }
        if (node_field(p) == field) {
            break;
        }
        p = p->next;
    }
    *count = node_count(p);
    if (buf != nullptr) {
        std::memcpy(buf, p->data, bytes(*count, sizeof(T)));
    }
    *count = node_count(p);
}

} // namespace

// @src 140404a00  malloc(0x18) node, count ints copied (140256dc0)
ListNode* fun_140404a00(i32 field, const i32* data, i32 count) { return attr_new<i32>(field, data, count); }

// @src 140404ab0  same count: copy; else realloc (140256e20), copy, relink after prev; returns the
// head, or the node itself when prev is 0
ListNode* fun_140404ab0(ListNode* head, ListNode* node, ListNode* prev, const i32* data, i32 count)
{
    return attr_replace<i32>(head, node, prev, data, count);
}

// @src 140404900  replace the field's data (140404ab0) or append a node (140404a00); new head
ListNode* fun_140404900(ListNode* head, i32 field, const i32* data, i32 count)
{
    return attr_set<i32, fun_140404a00, fun_140404ab0>(head, field, data, count);
}

// @src 140404b90  find the field: *count and (buf != 0) the ints; *count = 0 when missing
void fun_140404b90(ListNode* head, i32 field, i32* buf, i32* count) { attr_get<i32>(head, field, buf, count); }

// @src 140404de0  real counterpart of 140404a00 (140257150)
ListNode* fun_140404de0(i32 field, const double* data, i32 count) { return attr_new<double>(field, data, count); }

// @src 140404e90  real counterpart of 140404ab0 (1402571b0)
ListNode* fun_140404e90(ListNode* head, ListNode* node, ListNode* prev, const double* data, i32 count)
{
    return attr_replace<double>(head, node, prev, data, count);
}

// @src 140404ce0  real counterpart of 140404900
ListNode* fun_140404ce0(ListNode* head, i32 field, const double* data, i32 count)
{
    return attr_set<double, fun_140404de0, fun_140404e90>(head, field, data, count);
}

// @src 140404f70  real counterpart of 140404b90 (the second store of *count is not repeated)
void fun_140404f70(ListNode* head, i32 field, double* buf, i32* count) { attr_get<double>(head, field, buf, count); }

// @src 140404560  integer field of entity (1-based); 0 when the entity is out of range
i32 fun_140404560(ElemArray* a, i32 entity, i32 field, const void* data, i32 count)
{
    if (entity < 1 || a->count < entity) {
        return 0;
    }
    ElemLists* s = a->data + (entity - 1);
    s->a = fun_140404900(s->a, field, static_cast<const i32*>(data), count);
    return 1;
}

// @src 1404045e0  get an integer field (*count = 0 when the entity is out of range)
i32 fun_1404045e0(ElemArray* a, i32 entity, i32 field, void* buf, i32* count)
{
    if (entity < 1 || a->count < entity) {
        *count = 0;
        return 0;
    }
    fun_140404b90(a->data[entity - 1].a, field, static_cast<i32*>(buf), count);
    return 1;
}

// @src 140404660  real field of entity
i32 fun_140404660(ElemArray* a, i32 entity, i32 field, const void* data, i32 count)
{
    if (entity < 1 || a->count < entity) {
        return 0;
    }
    ElemLists* s = a->data + (entity - 1);
    s->b = fun_140404ce0(s->b, field, static_cast<const double*>(data), count);
    return 1;
}

// @src 1404046f0  get a real field
i32 fun_1404046f0(ElemArray* a, i32 entity, i32 field, void* buf, i32* count)
{
    if (entity < 1 || a->count < entity) {
        *count = 0;
        return 0;
    }
    fun_140404f70(a->data[entity - 1].b, field, static_cast<double*>(buf), count);
    return 1;
}

} // namespace def::cmd::cube
