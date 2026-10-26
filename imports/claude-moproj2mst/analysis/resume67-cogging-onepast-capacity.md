# Checkpoint 67: Cogging one-past pointer versus allocator capacity

## Finding

The original PE establishes an exact logical record count and nominal allocation request, but not the allocator's readable extent beyond that request. The native setup at `0x14076c100` passes the signed Reheat count and stride `0x190` to its allocation wrapper; the wrapper computes `count * 0x190` and passes that requested size to an indirect allocator call. It then initializes exactly `count` records at `base + i*0x190`. The writer reads the model count at `[model+0xcc]` and record base at `[model+0xd0]`; no separate capacity/reserve value or capacity check participates in its loop.

For two records, nominal request is `0x320` (800) bytes, and the logical element interval is `[base, base+0x320)`. The writer's accumulated pointer reaches `base+0x320` and the callee executes `mov r9d,dword ptr [r11]` there. The four-byte load is logically outside the records. This is statically true for any run where this count/base pair reaches the writer.

The PE does not show how much extra addressable heap storage is available after the requested block. The allocation wrapper calls through an allocator IAT slot with the computed size; its returned pointer and `count` are retained, but no later `_msize`/usable-size query or guard-page setup is present in the captured helper. Heap block rounding, segment placement, page commitment/protection, neighboring allocations, and runtime heap state are not encoded by the requested-size arithmetic. Consequently, an access violation is not statically predictable for ordinary positive two-Reheat input. The OOB read may fault or may read adjacent/slack bytes; the bytes are not a defined record value either way.

## Evidence

[resume67-cogging-onepast-capacity-evidence.txt](resume67-cogging-onepast-capacity-evidence.txt) is a focused bounded excerpt from the original-PE capture, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. It retains:

- `0x14076c100`: count/stride allocation call and initialization loop with `i < count` and `i*0x190`.
- `0x141473368` and `0x141486264`: size multiplication and indirect allocator request.
- `0x14076a8c0` loop arithmetic and the `0x14076cb50` first read at `0x14076cb80`.

The source-level memory/read comparison is also qualified in [resume64-cogging-memory-bound.md](resume64-cogging-memory-bound.md); that receipt establishes the logical bound, not actual page accessibility. The continued-search handler boundary is recorded in [resume66-cogging-fault-containment.md](resume66-cogging-fault-containment.md). Neither changes this capacity conclusion.

## Outcome

Retain the candidate two-Reheat refusal guard. Static evidence supports a deterministic guard based on `nReheats >= 2` and the invalid native pointer arithmetic, but it does not support claiming that the original application always crashes, nor a stable output/status for the invalid read. Establishing per-run readability requires a runtime capture of the actual allocation address, heap block/segment state, and exception outcome; such a result would remain specific to that run.

No source, build, GUI, or mutable REA state was changed.
