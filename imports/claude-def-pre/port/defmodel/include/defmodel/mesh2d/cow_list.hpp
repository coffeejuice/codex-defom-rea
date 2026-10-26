// The copy-on-write lists of interface pointers that the 2D geometry/topology classes hold.
// They are four more instantiations of p1-05's CowList<T> (<defmodel/sdctl/cow.hpp>): same body
// (0x18: int refs, sentinel, size), same node (0x18: next, prev, value), same detach-on-write and
// clear semantics; the binary has one copy of the code per element type.
//
//   instantiation   element   GEO2D_AND_MESH2D   other holders
//   A               ILoop*    +0x10 loops        _DEF_FACE +0x08, _DEF_EDGE +0x10
//   B               IEdge*    +0x18 edges        _DEF_LOOP +0x08, _DEF_VERTEX +0x18
//   C               IFace*    +0x08 faces        _DEF_LOOP +0x10
//   D               IVertex*  +0x20 vertices
//
// Function addresses per instantiation (A / B / C / D) and the CowList member they are:
//
//   handle ctor          140284c20 140284e30 140285080 140285290  CowList()
//   handle dtor          140284c90 140284ea0 1402850f0 140285300  ~CowList (release)
//   release body         140286260 1402862c0 140286320 140286380  release
//   body deleting dtor   140286440 140286480 1402864c0 140286500  delete body
//   body ctor            140286ba0 140286ec0 1402871e0 140287500  CowListBody()
//   sentinel ctor        140287d30 140287d70 140287db0 140287df0  (std::list sentinel)
//   body dtor            140286c50 140286f70 140287290 1402875b0  ~CowListBody
//   body copy ctor       140287eb0 1402880d0 1402882f0 140288510  CowListBody(const&)
//   body insert          140286cd0 140286ff0 140287310 140287630  std::list insert
//   node ctor(value)     140287d00 140287d40 140287d80 140287dc0  (std::list node)
//   body node_at         140286e60 140287180 1402874a0 1402877c0  walk i nodes
//   body clear           140288030 140288250 140288470 140288690  std::list::clear
//   make_unique          140285df0 140285e20 140285e50 140285f50  detach (refs > 1)
//   detach (copy body)   1402866f0 140286800 140286910 140286a20  detach
//   clear                140286660 140286770 140286880 140286990  clear
//   begin / end          140284cb0 140284ec0 140285110 140285320  detach + iterator
//                        140284cf0 140284f00 140285150 140285360
//   push_back            140284d30 140284f40 140285190 1402853a0  push_back
//   at(i)                140284de0 140284ff0 140285240 140285480  mutable_at
//   at(i) const          -         140285040 -         -          at (no detach)
//   iterator()           1402856b0 1402857a0 1402855c0 140285890  std::list iterator
//   iterator(node)       140285fa0 140285fc0 140285f80 140285fe0
//   const_iterator(node) -         140286000 -         -
//   iterator copy        1402856d0 1402857c0 1402855e0 1402858b0
//   operator!=           1402856f0 1402857e0 140285600 1402858d0  (Ghidra: CFileTimeSpan::!=)
//   operator*            140285730 140285820 140285640 140285910  (&node->value)
//   const operator*      -         140285980 -         -
//   operator++(int)      140285750 140285840 140285660 140285930
//
// size() of these lists (140159c20/40/60, 1402568e0) is CowList::size.
#pragma once

#include <defmodel/sdctl/cow.hpp>

namespace def::model {

class IFace;
class ILoop;
class IEdge;
class IVertex;

// @src 140284c20  A handle ctor
// @src 140284c90  A handle dtor
// @src 140284cb0  A begin
// @src 140284cf0  A end
// @src 140284d30  A push_back
// @src 140284de0  A at
// @src 140285df0  A make_unique
// @src 140286660  A clear
// @src 1402866f0  A detach
// @src 140286260  A release
// @src 140286440  A body deleting dtor
// @src 140286ba0  A body ctor
// @src 140286c50  A body dtor
// @src 140286cd0  A body insert
// @src 140286e60  A node_at
// @src 140287eb0  A body copy
// @src 140288030  A body clear
// @src 140287d00  A node ctor
// @src 140287d30  A sentinel ctor
// @src 1402856b0  A iterator()
// @src 140285fa0  A iterator(node)
// @src 1402856d0  A iterator copy
// @src 1402856f0  A operator!=
// @src 140285730  A operator*
// @src 140285750  A operator++(int)
using LoopList = CowList<ILoop*>;

// @src 140284e30  B handle ctor
// @src 140284ea0  B handle dtor
// @src 140284ec0  B begin
// @src 140284f00  B end
// @src 140284f40  B push_back
// @src 140284ff0  B at
// @src 140285040  B at const
// @src 140285e20  B make_unique
// @src 140286770  B clear
// @src 140286800  B detach
// @src 1402862c0  B release
// @src 140286480  B body deleting dtor
// @src 140286ec0  B body ctor
// @src 140286f70  B body dtor
// @src 140286ff0  B body insert
// @src 140287180  B node_at
// @src 1402880d0  B body copy
// @src 140288250  B body clear
// @src 140287d40  B node ctor
// @src 140287d70  B sentinel ctor
// @src 1402857a0  B iterator()
// @src 140285fc0  B iterator(node)
// @src 140286000  B const_iterator(node)
// @src 1402857c0  B iterator copy
// @src 1402857e0  B operator!=
// @src 140285820  B operator*
// @src 140285980  B const operator*
// @src 140285840  B operator++(int)
using EdgeList = CowList<IEdge*>;

// @src 140285080  C handle ctor
// @src 1402850f0  C handle dtor
// @src 140285110  C begin
// @src 140285150  C end
// @src 140285190  C push_back
// @src 140285240  C at
// @src 140285e50  C make_unique
// @src 140286880  C clear
// @src 140286910  C detach
// @src 140286320  C release
// @src 1402864c0  C body deleting dtor
// @src 1402871e0  C body ctor
// @src 140287290  C body dtor
// @src 140287310  C body insert
// @src 1402874a0  C node_at
// @src 1402882f0  C body copy
// @src 140288470  C body clear
// @src 140287d80  C node ctor
// @src 140287db0  C sentinel ctor
// @src 1402855c0  C iterator()
// @src 140285f80  C iterator(node)
// @src 1402855e0  C iterator copy
// @src 140285600  C operator!=
// @src 140285640  C operator*
// @src 140285660  C operator++(int)
using FaceList = CowList<IFace*>;

// @src 140285290  D handle ctor
// @src 140285300  D handle dtor
// @src 140285320  D begin
// @src 140285360  D end
// @src 1402853a0  D push_back
// @src 140285480  D at
// @src 140285f50  D make_unique
// @src 140286990  D clear
// @src 140286a20  D detach
// @src 140286380  D release
// @src 140286500  D body deleting dtor
// @src 140287500  D body ctor
// @src 1402875b0  D body dtor
// @src 140287630  D body insert
// @src 1402877c0  D node_at
// @src 140288510  D body copy
// @src 140288690  D body clear
// @src 140287dc0  D node ctor
// @src 140287df0  D sentinel ctor
// @src 140285890  D iterator()
// @src 140285fe0  D iterator(node)
// @src 1402858b0  D iterator copy
// @src 1402858d0  D operator!=
// @src 140285910  D operator*
// @src 140285930  D operator++(int)
using VertexList = CowList<IVertex*>;

static_assert(sizeof(LoopList) == 8 && sizeof(EdgeList) == 8 && sizeof(FaceList) == 8 &&
              sizeof(VertexList) == 8);

} // namespace def::model
