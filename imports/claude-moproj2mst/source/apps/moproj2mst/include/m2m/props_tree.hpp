// m2m/props_tree.hpp: default property trees of the SfOpr*Properties classes (owner: P11 framework,
// src/model/props_tree.cpp; per-class builders by their owners, see the hooks below).
//
// The original has no generic tree. Each class's props ctor calls its own builder, which adds
// nodes made from static templates (QtProperty prototypes built once by a static initializer,
// e.g. 0x14075e170 for Cogging). A template keeps the property Name, its TypeName and the default
// value; 0x140375ba0 makes a node from a template and appends it to a parent's child list. The
// builders' calls to the base adders (props_base.hpp, 0x1405f9eb0 ...) need the Props object (they
// also fill CommonBlock), so they stay in each class's makeProps, after defaultPropTree().
#pragma once

#include <vector>

#include "m2m/props.hpp"

namespace m2m::tree {
using qt::String;

// A property template (the 8-byte global the initializer fills; QtProperty prototype 0x14036e860).
struct Template {
    String name;                   // property Name (what the .moproj <Name> matches)
    String typeName;               // "group", "int", "double", ...
    qt::Variant value;             // default value (template +0x28, copied into every node)
};

// @src 0x140375640  group template: TypeName "group", value QVariant(0)
Template group(const char* name);
// @src 0x140373400  int template: TypeName "int", value QVariant(v)
Template intProp(const char* name, int v);
// @src 0x1403738e0  double template: TypeName "double", value QVariant(v)
Template doubleProp(const char* name, double v);
// Template of another TypeName (doubleUnits, enum, bool ...) whose maker is not one of the three
// above; `v` is the default as the maker stores it.
Template other(const char* name, const char* typeName, qt::Variant v);

// @src 0x140375ba0 (node 0x14036f810, append 0x140376320)  node from template `t`, appended to
// `parent`; returns the new node (valid until `parent` changes size).
PropNode& add(std::vector<PropNode>& parent, const Template& t);

qt::Variant vInt(int i);
qt::Variant vDouble(double d);
qt::Variant vText(const String& s);

// ---- dispatcher ------------------------------------------------------------------------------
// @src OB->vt[0x98] of every class that does not override makeProps (the base OpClass::makeProps
// calls this): the class's props object with its default tree and, where the class's builder calls
// them, the base groups (props_base.hpp). Unknown keys: plain Props with defaultPropTree(key).
std::shared_ptr<Props> defaultProps(const String& classKey, int guiUnit);

// ---- per-class hooks (owners in brackets) ------------------------------------------------------
// Until an owner ports its builder, its hook is a stub in src/stubs/props_tree_hooks.cpp (empty
// tree, plain Props): the converter then applies none of that class's saved properties.
// Props-level hooks (the class's builder also calls the base adders):
// [P17] formingProps / preProps / htFurnaceProps / shapeOptimProps / reportProps (m2m/p17.hpp)
// Every props-level hook makes its Props and calls data::initCommonBlock(*p->common, guiUnit) first
// (base ctor 0x1405f8ca0 -> 0x1405fd250), then its tree and adders; the plain fallback does too.
// [P11] SfOprHeatTransfer / 2D / Common: props ctor 0x141355f00
std::shared_ptr<Props> heatTransferProps(const String& classKey, int guiUnit);
// [P01] SfOprHeatTransferExpress / 2D: props ctor 0x1413eb5d0 (SfOprExpressProperties)
std::shared_ptr<Props> heatTransferExpressProps(const String& classKey, int guiUnit);
// Tree-level hooks (defaultPropTree), for classes whose makeProps builds the props object itself:
// [P11] @src 0x14075eb30  SfOprCogging
std::vector<PropNode> treeCogging();
// SfOprCycle (builder 0x14134b9c0, P07): no class nodes, only the base groups (CycleClass makeProps).
// SfOprDistortion2D / 3D (0x141366100, P07): trees::distortionPropTree (m2m/p07_trees.hpp).
// [P16] @src 0x1402bb080  SfOprDiestress;  @src 0x14029f0a0  SfOprDiestressBM2D / 3D
std::vector<PropNode> treeDiestress();
std::vector<PropNode> treeDiestressBM(const String& classKey);
// [P15] props ctor 0x1411bd570  SfOprHeatTreat / 2D
std::vector<PropNode> treeHeatTreat(const String& classKey);
// [P13] props ctors: SfOprGroup 0x141352b80, SfOprBoolean 0x1411e0780
std::vector<PropNode> treeGroup();
std::vector<PropNode> treeBoolean(const String& classKey);
// [P07] props ctors: SfOprConvert2Dto3D 0x1401cd390, SfOprCopyMirror 0x140e2c030
std::vector<PropNode> treeConvert2Dto3D();
std::vector<PropNode> treeCopyMirror();
// [P17] props ctors: SfOprDoeOutput 0x140e35ec0, SfOprReport 0x1412b1a90
std::vector<PropNode> treeDoeOutput();
std::vector<PropNode> treeReport();

// How to find a class's builder and templates: OB vt+0x98 (s1/vtables.tsv) is "new N + props
// ctor"; the ctor (or a callee) calls the builder, which instantiates templates through
// 0x140375ba0 (s1/callees.pkl: callers of 0x140375ba0 and of the base adders 0x1405f9eb0 ...).
// The template globals are filled by the class's static initializer: it calls the template makers
// 0x140375640 / 0x140373400 / 0x1403738e0 with the global's address as an immediate argument (LEA
// or MOV of 0x143da65a0 for Cogging), not a MOV store, so look for callers of the three makers
// (callees.pkl) or for xrefs to the global from a function with no other callers (Cogging:
// 0x14075e170, referenced only from the CRT initializer table).

}  // namespace m2m::tree
