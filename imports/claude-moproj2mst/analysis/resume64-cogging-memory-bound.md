# Checkpoint 64: Cogging record allocation and two-Reheat bound

## Finding

The immutable PE does establish the logical Cogging Reheat record allocation extent and confirms the two-Reheat writer's first record read lies outside it. The Reheat array setup at `0x14076c100` passes the signed count and stride `0x190` to the allocation wrapper. That wrapper passes `count * stride` as the allocation size; it then initializes exactly `count` records, each at `base + i*0x190`. For `nReheats = 2`, the valid logical record interval is `[base, base+0x320)`. The writer's second-loop iteration `i=1` passes `base+0x320` to `0x14076cb50`; after its `Reheat 2` banner call, instruction `0x14076cb80` reads four bytes from `[r11]`.

This proves the load is one-past the allocated record array, but not that it faults. The allocator may provide granularity or neighboring readable storage beyond the requested payload. Static code does not identify the runtime address-space mapping or contents there. Do not infer a crash, read value, output bytes, process status, or Save continuation from this bound proof.

## New bounded evidence

[resume64-cogging-memory-bound-capture.txt](resume64-cogging-memory-bound-capture.txt) is the unique raw capture for this checkpoint. It contains bounded disassembly slices tied to original PE SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`:

- `0x14076c100` through its RET: count/stride arguments, allocation call, and the `i < count` initialization loop using `i*0x190`.
- `0x141473368` through RET: forwards count and stride to checked allocation sizing.
- `0x141486264` through RET: checks size overflow, computes `count*stride`, and calls the allocator with that requested payload size.
- Retained writer caller loop from `evidence/gui_mo/s1/mdt/a8c0.asm` and callee's first dereference from `analysis/resume62-cogging-cb50-bounded.asm`.

The arithmetic is direct: with `n=2`, the SIMLST pass finishes at element index 1. The serialization loop's accumulated offsets are `+0` then `+0x190`; therefore its calls receive element 1 then `base+0x320`. The callee calls the `Reheat %d` banner formatter before the four-byte load from the passed pointer. The exact caller arithmetic and callee instruction were already described in prior receipts; this checkpoint adds the native allocation-size proof.

## Boundary of the conclusion

The allocation wrapper's request is `n * 0x190` bytes. This is the logical record array size, not a guarantee about the allocator's usable block size, page protection, or adjacent object placement. A GUI/runtime capture would be required to determine whether the one-past load faults in a particular process state and what it observes. Even such an observation would be specific to that run; it would not define deterministic two-Reheat serialization bytes.

No source, build, test, GUI, or mutable REA state was accessed or changed. No prior Cogging failure-order analysis was repeated beyond the caller/callee excerpts necessary to bind the allocation bound to the two-Reheat read.
