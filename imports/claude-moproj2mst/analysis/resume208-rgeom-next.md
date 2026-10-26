# CP208 geometry owner follow-up

## Result

No geometry-owned positive mesh profile is admitted. The available static disassembly does not close the required producer-to-consumer chain for `GEO2D_AND_MESH2D::mesh` and its nested `OBJ_MESH2D` node set. `viewer.cpp` remains unchanged; its existing refusal is still the evidence-consistent behavior.

## Static evidence checked

The prior cut in `resume171-rgeom-next.md` remains accurate: `analysis/wave2-geometry-14126d0c0-disassembly.txt` binds the `SfDisp2DCrossSection` consumer to readiness, positive node/element counts, coordinate and connectivity getters, and rectangle builder `0x14124bec0`. That cut proves consumer order, not a native producer that initializes the nested optional owner.

There is a tempting adjacent PE function in `analysis/wave3-editor-mds-native-owner-chain-1408531e0.asm`. At `0x1408531f8`, it writes `rax` to `[rcx+0x130]`; it then conditionally invokes `0x140100980` and `0x140c4d8d0`. The file is only an unannotated native code dump, without an xref/call-chain qualification tying its `rcx` object to the geometry cross-section's `OBJ_MESH2D + 0x008` node set. The matching offset alone cannot identify the object type or establish that this is the needed owner writer. `analysis/wave3-editor-mds-node-registration-140853510.asm` likewise contains editor/MDS registration evidence but no qualified link to the geometry mesh producer.

The previous qualification names the getter chain (`0x140514f00` to `0x140854af0` to `0x140854200`) and null-owner behavior at `0x140853240`; none of the retained geometry evidence binds the adjacent MDS `+0x130` write to that getter's expected owner type or proves the required dimension setter (`u138`) for this geometry profile. Therefore it would be unsound to use the adjacent write as authority to enable positive counts/arrays.

## Exact unresolved observation

Still required: a hash-bound static xref/call-chain from a geometry cross-section mesh create/copy path into a writer of `OBJ_MESH2D + 0x008 + 0x130`, with the source/value type and matching `u138` state setter identified; then a same-profile guarded path showing non-null coordinate and connectivity results reaching the rectangle builder. If static evidence cannot bind those identities, the remaining needed observation is a controlled original-instance inspection of the populated geometry cross-section and nested node-set owner/dimension, coupled with the getter results for that same instance. No runtime or GUI interaction was performed here.

## Scope and changes

This was a read-only static evidence pass. No converter source, tests, build, shared plan/state, original executable, or GUI was changed or run. The sole artifact is this report.
