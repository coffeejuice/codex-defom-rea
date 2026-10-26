# Checkpoint 62: provenance of Report QObject private `+0x20` input

## Finding

The prior DWORD consumed by the private `+0x20` transform is not statically qualified. The immutable public constructor allocates `0x88` bytes and passes the returned private pointer to the initializer without any visible zero-fill. The initializer itself reads `private+0x20` before writing it. No retained runtime receipt records the allocation contents before that read.

The isolated helper is still correct as a pure transform of an explicit input. Its result cannot be derived for the Report instance without observing the old word. Do not substitute zero or characterize the output as a known QObject initial field value.

## Bounded native chain

`analysis/wave2-doe-task-base-public-qobject-constructor.asm` shows the public constructor:

1. At QtCore VA `0x671313fb`, set allocation size to `0x88`.
2. At `0x67131400`, call `0x67178a86`; store return RAX at stack `+0x58` and test it.
3. If nonnull, pass version `0x40804` and the returned private pointer to initializer `0x67131160` at `0x67131417`.
4. After initializer return, publish object/private backlinks. The null allocation branch bypasses the initializer and publishes null private state instead.

The allocator target at `0x67178a86` is a thunk (`jmp qword ptr [0x671a8578]`). The retained import map identifies slot `0x671a8578` as `??2@YAPEAX_K@Z` (`operator new`). This establishes that the call leaves QtCore through its imported allocation function; the evidence reviewed here does not include the allocator implementation or any contract about the particular returned block's prior bytes. No memset/zero-fill instruction occurs in the visible QObject caller between allocation return and initializer entry.

`analysis/wave2-doe-task-qobject-private-initializer.asm` shows the initializer makes other explicit writes, but private `+0x20` is first read at `0x67131224`, then transformed by `AND 0xfffffcf0` and `OR 0x60`, and stored at `0x6713122f`. No earlier instruction in the retained routine writes that DWORD. Thus the initializer defines a subset of its bits and preserves the others from the value supplied by the allocation state.

## What this bounds

- Static evidence qualifies the address, operation, input dependency, and output formula.
- Static evidence does not qualify the input value or its provenance inside the allocator/runtime heap. It could depend on allocator reuse or earlier process history; neither is recorded for the first Report frame.
- The probe004 live capture does not cover the Report constructor and contains no relevant private allocation.
- The existing Report capture plan already includes the necessary bounded sample: at the real DOE caller and initializer, read `private+0x20` immediately before the mask sequence and after the store, linked by the same call ordinal/private pointer. The plan is not a capture receipt and does not supply either value.

## Recommendation

No source or test change follows. Retain the isolated `isolatedQObjectPrivateFlags(previousWord)` helper and leave native Report construction guarded. If a root-serialized passive Report capture is later run, record allocation return/private identity, the old DWORD at initializer entry, and the new DWORD after `0x6713122f`; reject attribution if nested constructors or allocator ambiguity prevent binding both reads to the same call. The captured word can validate this one local field without requiring GUI event scheduling, but it cannot satisfy shared-reference, backlink, TLS/QThreadData, process-counter, or cleanup prerequisites.

No source, build, test, GUI, or mutable REA action was taken for this static audit.
