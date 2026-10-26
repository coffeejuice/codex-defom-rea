// cvname/cvname.cpp: the CVNAME collection on Cls_14032f100 (packet kw-g1b).
// Header: defmodel/cvname/cvname.hpp. The list classes are p1-10's (members/custom_state_var.*).
#include <defmodel/cvname/cvname.hpp>
#include <defmodel/cvname/cvname_list.hpp>

#include <defmodel/port.hpp>

namespace def::model {

namespace {

// @src 14032fd60  detach the list<CustomVarDef> before a write when it is shared (refs > 1).
void detach(VarListRep* rep)
{
    if (1 < static_cast<std::uint32_t>(rep->refs))
        DEF_NOT_PORTED(0x1403300b0);  // copy the rep; unreachable: handles are never shared here
}

// @src 14032fd30  the same for a list<std::string>.
void detach(StrListRep* rep)
{
    if (1 < static_cast<std::uint32_t>(rep->refs))
        DEF_NOT_PORTED(0x14032ffa0);
}

// @src 14032f670  find the entry with this id: iterator to it, or end()
// (14032fca0 ++, 14032fc80 deref, 14032fc20 copies the result iterator)
VarListNode* find(Cls_14032f100* c, std::int32_t id)
{
    VarListRep* rep = c->u8.list;
    VarListNode* result = var_list_end(rep);   // 14032f9f0
    VarListNode* it = var_list_iter_default(); // 14032fc00
    it = var_list_begin(rep);                  // 14032f9b0
    for (;;) {
        if (it == var_list_end(rep))
            return result;
        if (cv_def_id(&it->value) == id)
            return it;
        it = it->next;
    }
}

}  // namespace

// @src 14032f240  entry with this id, appended when missing
// (14032ee00 destroys the temporary)
CustomVarDef* cv_get_or_add(Cls_14032f100* c, std::int32_t id)
{
    VarListNode* found = find(c, id);
    VarListRep* rep = c->u8.list;
    if (found != var_list_end(rep))
        return &found->value;  // 14032fc80
    CustomVarDef tmp;
    cv_def_construct(&tmp, id);    // 14032ed30
    var_list_push_back(rep, tmp);  // 14032fa50
    return var_list_back(rep);     // 14032fb10
}

// @src 14032f350  set the name of entry `id`
void cv_set_name(Cls_14032f100* c, std::int32_t id, const std::string& name)
{
    cv_get_or_add(c, id)->set_name(name);  // 14032ef20
}

// @src 14032f1a0  clear the items of entry `id`
void cv_clear_items(Cls_14032f100* c, std::int32_t id)
{
    cv_get_or_add(c, id)->clear_items();  // 14032eeb0
}

// @src 14032f5f0  append an item to entry `id`
void cv_append_item(Cls_14032f100* c, std::int32_t id, const std::string& item)
{
    cv_def_append(cv_get_or_add(c, id), item);
}

// @src 14032f1e0  number of entries (14032fa30: rep size, no detach)
std::int32_t cv_count(Cls_14032f100* c) { return static_cast<std::int32_t>(c->u8.list->size); }

// @src 14032f210  i-th entry (14032fb40: detach, then 1403306d0 walks i nodes from begin)
CustomVarDef* cv_entry(Cls_14032f100* c, std::int32_t i)
{
    VarListRep* rep = c->u8.list;
    detach(rep);
    VarListNode* p = rep->head->next;
    for (std::uint64_t k = 0; k < static_cast<std::uint64_t>(static_cast<std::int64_t>(i)); k = k + 1)
        p = p->next;
    return &p->value;
}

// @src 14032ef00  id
std::int32_t cv_def_id(const CustomVarDef* d) { return d->id; }

// @src 14032efd0  append an item (14032f880: detach, then 1403302b0 inserts before end)
// @src 14032f880  (list<std::string> push_back)
void cv_def_append(CustomVarDef* d, const std::string& item)
{
    StrListRep* rep = d->items.rep;
    detach(rep);
    StrListNode* where = str_list_end(rep);  // 14032fcf0
    StrListNode* node = new StrListNode{nullptr, nullptr, item};  // 1403307f0 copies the string
    node->next = where;
    node->prev = where->prev;
    where->prev->next = node;
    where->prev = node;
    rep->size = rep->size + 1;
}

// @src 14032fc00  list<CustomVarDef> iterator default ctor
VarListNode* var_list_iter_default() { return nullptr; }

// @src 14032f9b0  list<CustomVarDef>::begin (detach, then 14032fd90 wraps head->next)
VarListNode* var_list_begin(VarListRep* rep)
{
    detach(rep);
    return rep->head->next;
}

// @src 14032f9f0  list<CustomVarDef>::end (detach, then 14032fd90 wraps the sentinel)
VarListNode* var_list_end(VarListRep* rep)
{
    detach(rep);
    return rep->head;
}

// @src 14032fa50  list<CustomVarDef>::push_back: detach, then 140330520 allocates a node
// (0x48 bytes, value copied by 140330850) and links it before end(); size + 1
VarListNode* var_list_push_back(VarListRep* rep, const CustomVarDef& v)
{
    detach(rep);
    VarListNode* where = var_list_end(rep);
    VarListNode* node = new VarListNode();
    node->value.set_id(v.id);
    node->value.name = v.name;
    StrListRep* src = v.items.rep;
    for (StrListNode* p = src->head->next; p != src->head; p = p->next)
        cv_def_append(&node->value, p->value);
    node->next = where;
    node->prev = where->prev;
    where->prev->next = node;
    where->prev = node;
    rep->size = rep->size + 1;
    return node;
}

// @src 14032fb10  list<CustomVarDef>::back (detach, &head->prev->value)
CustomVarDef* var_list_back(VarListRep* rep)
{
    detach(rep);
    return &rep->head->prev->value;
}

// @src 14032fcf0  list<std::string>::end (detach 14032fd30, then 14032fef0 wraps the sentinel)
StrListNode* str_list_end(StrListRep* rep)
{
    detach(rep);
    return rep->head;
}

// @src 14032ed30  CustomVarDef(id): string 140005040, items 14032f7d0, then 14032eee0 sets the id
void cv_def_construct(CustomVarDef* d, std::int32_t id)
{
    d->name.clear();
    d->items.clear();
    d->set_id(id);
}

// @src 14032efa0  number of items (14032f860: rep size, no detach)
std::int32_t cv_def_item_count(const CustomVarDef* d) { return static_cast<std::int32_t>(d->items.rep->size); }

// @src 14032ef50  copy of the name
std::string cv_def_name(const CustomVarDef* d) { return d->name; }

// @src 14032f070  copy of item i, or "" (140005040) when i is out of range
// @src 14032f8e0  (item i: detach, then 1401b07b0 walks i nodes from begin)
std::string cv_def_item(CustomVarDef* d, std::int32_t i)
{
    if (-1 < i && i < cv_def_item_count(d)) {
        StrListRep* rep = d->items.rep;
        detach(rep);
        StrListNode* p = rep->head->next;
        for (std::uint64_t k = 0; k < static_cast<std::uint64_t>(static_cast<std::int64_t>(i)); k = k + 1)
            p = p->next;
        return p->value;
    }
    return std::string();
}

}  // namespace def::model
