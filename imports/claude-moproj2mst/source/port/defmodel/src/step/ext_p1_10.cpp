// p1-03 calls into p1-10 (see ext.hpp), wired to the owner's methods at the Phase 1 gate.
#include "ext.hpp"

#include <defmodel/layout/cls_1403753f0.hpp>
#include <defmodel/layout/phase_pair_list.hpp>

namespace def::model::ext {

// part of @src 140339cd0  inter-material list reset (PhasePairList::reset)
void fun_140339cd0(PhasePairList* l) { l->reset(); }

// part of @src 14033a120  inter-material list clear (PhasePairList::clear)
void fun_14033a120(PhasePairList* l) { l->clear(); }

// part of @src 140375520  furnace: clear the curve list
void fun_140375520(Cls_1403753f0* f) { f->clear_curves(); }

// part of @src 140375600  furnace: build the curves
void fun_140375600(Cls_1403753f0* f, const std::int32_t* n) { f->setup_curves(n); }

// part of @src 140375e90  furnace: defaults
void fun_140375e90(Cls_1403753f0* f) { f->set_defaults(); }

} // namespace def::model::ext
