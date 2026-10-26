// defmodel/cvname/cvname.hpp: the CVNAME collection at DB_STEP_DATA+0x33b8 (Cls_14032f100: a
// copy-on-write list<CustomVarDef>, p1-10's members/custom_state_var.hpp), and its entries'
// accessors. Packet kw-g1b.
//
// Entries are keyed by id and kept in insertion order. The list handles are not copyable in the
// port, so a representation is never shared; the detach-on-write branches of the original
// (1403300b0, 14032ffa0) are therefore unreachable and stop with DEF_NOT_PORTED.
#pragma once

#include <cstdint>
#include <string>

#include <defmodel/layout/cls_14032f100.hpp>
#include <defmodel/members/custom_state_var.hpp>

namespace def::model {

// 14032f240: the entry with this id; appended (id, empty name, no items) when missing.
CustomVarDef* cv_get_or_add(Cls_14032f100* c, std::int32_t id);
// 14032f350: set the name of entry `id` (created when missing).
void cv_set_name(Cls_14032f100* c, std::int32_t id, const std::string& name);
// 14032f1a0: clear the item list of entry `id` (created when missing).
void cv_clear_items(Cls_14032f100* c, std::int32_t id);
// 14032f5f0: append an item to entry `id` (created when missing).
void cv_append_item(Cls_14032f100* c, std::int32_t id, const std::string& item);
// 14032f1e0: number of entries.
std::int32_t cv_count(Cls_14032f100* c);
// 14032f210: the i-th entry (0-based, no range check, like the original).
CustomVarDef* cv_entry(Cls_14032f100* c, std::int32_t i);

// CustomVarDef accessors used by the KEY writer.
std::int32_t cv_def_id(const CustomVarDef* d);                   // 14032ef00
void cv_def_append(CustomVarDef* d, const std::string& item);    // 14032efd0
std::int32_t cv_def_item_count(const CustomVarDef* d);           // 14032efa0
std::string cv_def_name(const CustomVarDef* d);                  // 14032ef50
std::string cv_def_item(CustomVarDef* d, std::int32_t i);        // 14032f070 ("" out of range)

}  // namespace def::model
