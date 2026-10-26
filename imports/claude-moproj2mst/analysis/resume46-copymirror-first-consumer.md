# CopyMirror property application: callers and first consumer

## Result

The retained project evidence supports a **Save-time property-to-engine call**
and supports **no call from the recovered native Open loader**. It does not
establish a missing native Open-time call. The first concrete consumer in the
current port is operation/model Save processing before MST emission; the
property mutation's impact on CopyMirror's final files or serialized model is
still unknown. Do not insert the slot into `cmLoad` without native caller
evidence.

## Caller separation

| Phase | Original-qualified evidence | Current reconstruction | Finding |
|---|---|---|---|
| Open operation load | CopyMirror vt+0x58 is `0x140e24b80`; its property load vt+0x58 is `0x140e2d080`; property-to-engine vt+0x60 is `0x140e2d230`. Retained native body for `0x140e24b80` is represented in the imported class/lifecycle evidence as base load plus display-model setup; no `0x140e2d230` call is identified there. | `CopyMirrorClass::loadOperation` → `cmLoad` calls `engine::baseLoad`, then scene/display setup; `cmPostLoad` calls `basePostLoadBody` plus `loadMirrorPlanes`. Neither invokes `propsToEngine`. `openActiveOperation` dispatches `loadOperation` before Save. | **No Open call established.** `cmPropsLoad` is a distinct reader: it copies model fields into the property data block; it is not the reverse application. |
| Save-current step 2 | Common GUI Save caller is `0x140052ec0` (identified in `convert.cpp` lifecycle notes). The generic native dispatcher `0x1400543a0` is identified in retained evidence as the property-to-engine virtual dispatch reached through operation selection `0x1405f36d0`. The decompiled `0x1405f36d0` evidence exists at `evidence/gui_mo/L5d/f36d0.c`; however, its large Qt property-panel body must be read with bounded call-site context, and the exact CopyMirror case/caller chain is not extracted into a dedicated evidence artifact. | `engine::save_current.cpp::propsToEngine` dispatches `opBase(op)->propsToEngine(p, op)` (`:147–152`). It is called from native Save-current branches at `:734`, `:755`, and `:759`; `saveCurrent` documents duplicate calls for index zero. CopyMirror override dispatches to `cmPropsToEngine` (`ops/copymirror.cpp:92–98`). | **Save-time virtual dispatch is present in current source.** Exact which Save branch fires for a selected CopyMirror item is determined by active operation/editor state; preserve ordering and multiplicity. |
| MST writer | Writer vt+0x280 is `0x140e24e80`. The retained inventory marks `0x140e2d230` “not called by writer”; no direct writer-local application is established. | `CopyMirrorClass::writeMst` calls `writeBlock` and does not invoke `propsToEngine` (`ops/copymirror.cpp:114 onward`). | **No direct writer call.** This does not cancel a preceding Save-current mutation. |

## Mutation and first downstream consumer

The reconstructed slot `copyMirrorPropsToEngine` at
`source/apps/moproj2mst/src/engine/l5_slots.cpp:1106–1110` delegates to
`convertPropsToEngine` (`:1083–1104`). It writes the property data to every
non-kind-7 model object: conversion kind at `+0x2be8`, WPAXIS and axis at
`+0x20a0`/`+0x2c38`, and the relevant revolution or extrusion values at
`+0x2bf0`, `+0x2bf8`, `+0x2c10`, and `+0x2c18`.

The first concrete consumer established in the current lifecycle is therefore
the subsequent Save/MST pipeline operating on that mutated model. The retained
CopyMirror writer uses model/DB content (`copymirror.cpp` `writeBlock` emits
simulation header, DBREAD, controls, materials, mesh settings and objects), but
the current evidence does not show which of the above fields it reads or whether
any emitted bytes differ for a nondefault CopyMirror property payload. The
CopyMirror object-provider/mirror-plane path is separately bounded by
`0x140e25980`, `0x140e25b20`, and viewer-plane handling (`0x140c82d50`); its
mirror geometry construction can refuse. That refusal does not prove ordinary
MST output ignores the mutated fields.

### Status

- **Settled — Save caller exists in the reconstructed flow:** the generic
  Save-current dispatcher has virtual property application, and CopyMirror's
  override is wired.
- **Settled — no current Open caller:** `cmLoad` and `cmPostLoad` do not call
  the slot; property parsing (`0x140e2d080`) is not application (`0x140e2d230`).
- **Unknown — exact native `0x140e2d230` xref inventory:** the imported evidence
  does not include a bounded xref report proving all native callers, including
  whether any indirect/GUI-only caller exists beyond Save-current.
- **Unknown — first field-sensitive external effect:** no current crosswalk
  identifies an emitted or side-file byte whose value depends on these fields
  for a normal CopyMirror project.
- **Unsupported conclusion — “must call during Open”:** no evidence supports
  adding an Open-time call; current native load structure argues against it.
- **Unsupported conclusion — “safe to omit entirely”:** Save-current has a
  reachable slot, and the body mutates persistent model fields. The comment
  “no MST byte depends” is not enough to establish full Save equivalence.

## Smallest static query if further evidence is required

Use the preserved original image and its existing Ghidra/REA database only in a
separately authorized read-only session. Request one bounded static query:

1. Resolve the CopyMirror Properties vtable entry at `0x140e2d230`, list all
   direct code references to that target and all virtual-call sites using the
   CopyMirror properties vtable/class discriminator. Include containing
   function addresses and the immediately surrounding conditional/call block.
2. For each caller, trace only the enclosing branch to classify it as Open
   (`0x140e24b80`/operation load), Save-current (`0x140052ec0` →
   `0x1400543a0`), editor/widget, or writer (`0x140e24e80`). Do not request a
   full dossier or broad function dump.
3. If the only lifecycle call is Save-current, query reads of the written
   object fields (`+0x2be8`, `+0x2bf0/+0x2bf8`, `+0x2c10/+0x2c18`, `+0x2c38`)
   along CopyMirror's `0x140e24e80` writer and its immediate model serialization
   callees. Return only field offsets, consuming function addresses, and branch
   conditions. That decides whether property application has a direct output
   effect without requiring a speculative source edit.

The query is smaller and more discriminating than launching the GUI or capturing
whole-project outputs. If a consumed field is found, the next validation should
be one original-first CopyMirror fixture with a nondefault value for that field,
checking output and side files. If no writer or reachable callee reads the
fields, the “no MST byte depends” qualification can be source-bound to this
writer; it still does not waive effects outside MST.

## Evidence qualifications

`evidence/gui_mo/L5d/f36d0.c` is retained decompilation, but no new extraction or
REA call was made here. Current source call order comes from `save_current.cpp`,
`loader.cpp`, `copymirror.cpp`, and `l5_slots.cpp`. Checkpoint 45 has no fresh
original CopyMirror parity run. No build, source edit, app control, or REA DB
mutation was performed.
