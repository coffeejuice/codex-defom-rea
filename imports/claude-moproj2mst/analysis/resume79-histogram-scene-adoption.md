# CP79 histogram scene-adoption trace

## Result

The histogram's constructor chain does not insert it into a `QGraphicsScene`. Its base path explicitly constructs the `QGraphicsItem` subobject with null parent and null scene. A later caller could still insert the returned item, but the retained PE evidence does not recover that caller or bind an `addItem` call to the factory result.

The static flow is `c36bd0` → `SfGraphHistogramItem` constructor `c2b6b0` → base `c38ca0` → `c1c430`. In `c1c430`, `xor r8d,r8d` and `xor edx,edx` immediately precede the call through IAT `0x141e07e40`, mapped in retained import contracts to `??0QGraphicsItem@@QEAA@PEAV0@PEAVQGraphicsScene@@@Z`. The register arguments at that call are `rcx=this+0x10`, `rdx=nullptr` (parent), and `r8=nullptr` (scene). Thus it does not rely on caller-provided scene state and cannot establish scene membership during construction.

## AddItem search and nearby candidates

A complete `.text` scan for `FF 15 disp32` calls targeting the verified IAT slot `0x141e08260` (`QGraphicsScene::addItem`) finds 132 sites. Two in graph-adjacent code are `0x140c2c560` and `0x140c30fd7`; captures are `analysis/resume79-additem-candidate-c2c560.asm` and `analysis/resume79-additem-candidate-c30fd7.asm`. Both pass an item pointer staged as another object plus `0x10` in `rdx`, with the scene in `rcx` from a stack local. Neither bounded callsite contains a def-use edge to the pointer returned by `c15910`/`c36b80`, and neither capture binds its item to `SfGraphHistogramItem` RTTI. Their proximity is insufficient to assign either call to the histogram.

Earlier immutable-PE scans (receipts `resume60-graph-owner-xref.md`, `resume64-histogram-caller-raw.txt`, `resume67-histogram-offline-raw.txt`) found no direct or common-form incoming reference to manager wrapper `c15910`, no literal factory slot xref, and no aligned relative-data entry for `c15910`, `c36b80`, or `c36bd0`. The manager wrapper itself forwards the factory result in `RAX` unchanged. There is therefore no recovered post-return consumer.

## Exact unresolved edge

The missing edge is a caller/dispatch into `0x140c15910` (or another proven caller of the graph factory), followed through its returned `RAX` to the first owner action. That action could be scene `addItem`, registry insertion, another storage location, or pass-through. Until it is recovered, scene adoption after construction and native scene ownership remain unresolved. No conclusion is drawn from unrelated addItem sites.

## Scope

Target is immutable `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. New bounded captures: `resume79-histogram-base-constructor.asm` (base path), `resume79-qgraphicsitem-ctor-args.asm` (null parent/scene arguments), and the two addItem sites above. No source, build, GUI, or mutable REA state was accessed or changed.
