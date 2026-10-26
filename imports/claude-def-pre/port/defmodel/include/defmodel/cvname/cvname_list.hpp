// defmodel/cvname/cvname_list.hpp: the list<CustomVarDef> / list<std::string> member helpers
// that the CVNAME collection (cvname.cpp, packet kw-g1b) calls: begin, end, push_back, back, the
// iterator's default ctor, and the CustomVarDef(id) ctor. Census follow-up C05.
//
// The lists are p1-10's hand-rolled copy-on-write lists (members/custom_state_var.hpp). Every
// helper first detaches a shared representation (14032fd60 / 14032fd30); the port never shares
// one, so that branch stops with DEF_NOT_PORTED. Iterators are plain node pointers (the
// original's 8-byte iterator object holds just the node, 14032fd90 / 14032fef0).
#pragma once

#include <cstdint>

#include <defmodel/members/custom_state_var.hpp>

namespace def::model {

// 14032fc00: default-constructed list<CustomVarDef> iterator (node = null).
VarListNode* var_list_iter_default();
// 14032f9b0: begin() (sentinel's next).
VarListNode* var_list_begin(VarListRep* rep);
// 14032f9f0: end() (the sentinel).
VarListNode* var_list_end(VarListRep* rep);
// 14032fa50: push_back(v): a copy of v inserted before end(); returns the new node.
VarListNode* var_list_push_back(VarListRep* rep, const CustomVarDef& v);
// 14032fb10: &back().
CustomVarDef* var_list_back(VarListRep* rep);
// 14032fcf0: list<std::string>::end() (the sentinel).
StrListNode* str_list_end(StrListRep* rep);

// 14032ed30: CustomVarDef(id): empty name, empty item list, then the id (14032eee0).
void cv_def_construct(CustomVarDef* d, std::int32_t id);

}  // namespace def::model
