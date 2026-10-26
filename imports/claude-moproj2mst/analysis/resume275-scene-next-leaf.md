# CP275 scene/display next leaf: post-bind color/node display remains the exact blocker

## Decision

There is no independently source-ready scene/display successor after the canonical 3D/null-database Slice short circuit. The next caller-local unresolved region is `viewer.cpp::bind`'s post-Slice tail: native color work (`FUN_1411f28f0`) followed by node display (`FUN_1411fc900`). The converter intentionally leaves both qualified at the current boundary. I cannot name a finite behavioral change there without the exact helper contracts and actual-caller side-effect witnesses; implementing guessed color or node behavior would exceed the retained evidence. This is an exact blocker, not a proposal to repeat CP274.

## Bounded evidence reviewed

The converter takeover instructions, `TAKEOVER.md`, `IMPLEMENTATION_PLAN.md`, `analysis/takeover-state.json`, and CP250–274 scene/display reports were consulted. CP272 completed canonical live-2D Slice rebind admission, CP274 reviewed and mapped the canonical live-3D/null-DB short circuit, and `analysis/resume274-geometry-next-leaf-review.md` gives the narrow collection-preserving caller contract. The retained original is DEF_GUI_MO_64.exe v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, as qualified by CP257/CP262/CP274; this pass did not rehash or execute it.

Current `source/apps/moproj2mst/src/engine/viewer.cpp::bind` performs partial unbind, object assignment, 2D/3D representation binding, pending-geometry return, and the guarded Slice rebind check. The function then ends with an explicit qualification that native `0x1411f28f0` color and `0x1411fc900` node-display behavior remains unresolved. The retained decompile `evidence/gui_mo/s1/mdt/V/v0.c::FUN_1411f17f0` establishes these calls occur after the Slice adaptation gate, but does not provide a complete, source-qualified result contract for their nested graphics/color behavior. The nearby CP274 Slice evidence explicitly warns that larger helper bodies contain removed-unreachable regions; it cannot be extended to certify this tail.

Scene-box and fit routes do not provide a smaller successor: CP257/259/262 recovered refresh prefix, scene-box fixed-slot contributions, and the independent radius path; the remaining named-holder/currentness and optional nested geometry items have separate missing-witness requirements. Reopening those completed leaves or inferring from cached boxes would not establish the color/node tail.

## Exact evidence needed to unblock

Recover and source-qualify the relevant native helper bodies and their direct callees for the bounded call sites `0x1411f28f0` and `0x1411fc900`, including the ordered owner/view/settings/object reads, any color or node-list writes, early-return and failure paths, and the smallest ordinary caller fixture that reaches each effect. Tie that contract to the converter's current `bind` entry and an existing actual-caller test fixture. If a required branch depends on a GUI event, editor-only owner, graphics backend, or unsupported geometry state, record that dependency rather than fabricating it. No GUI witness is required for the static, finite synchronous paths, but no such fully qualified path was found in the reviewed records.

This report changes no source or tests and makes no original-runtime, GUI, or reconstructed DEF_PRE claim. No build or mutable REA operation was run. No GUI-control session was acquired. Only this report is written.
