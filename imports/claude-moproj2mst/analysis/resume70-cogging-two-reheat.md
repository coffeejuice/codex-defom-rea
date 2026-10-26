# Checkpoint 70: two-Reheat allocator uncertainty and minimum non-GUI probe

## Static bound

The original allocator path is already captured in [resume64-cogging-memory-bound-capture.txt](resume64-cogging-memory-bound-capture.txt), SHA-256 `CCDC314F6B13475076214E7525C68D16507F1C067321B31EE4D8D07AADE11226`. For positive count `n`, `0x14076c100` passes `(n, 0x190)` to the allocator wrapper, which computes the product and requests `n*0x190` bytes from an indirect allocator. It initializes `n` records at 0x190-byte strides. The writer has only the model's count at `+0xcc` and base pointer at `+0xd0`; its second loop never reads a capacity field.

For `n=2`, the nominal record allocation is 800 bytes, while the invalid second writer call reaches `base+800` and reads four bytes. No static code rounds the request up to a documented array capacity, places a guard page, queries the usable allocation size, or checks the read address. The allocator may internally round its block or place it beside other committed heap blocks, but those outcomes are runtime-specific. This does not support either “always faults” or “always readable” for ordinary two-Reheat input.

The allocation helper's indirect target is shown as IAT slot `0x141e03380` in the retained disassembly. The current bounded evidence does not resolve that slot's exact imported API identity or a stable allocator usable-size policy. That identification would still not reveal a particular process's heap state, so it is not the missing fact needed to claim a deterministic fault.

## Minimum non-GUI runtime observation

A one-run probe can resolve whether the exact address is readable for one concrete allocation without opening or controlling the GUI:

1. Start the original PE under a headless native debugger and stop after its allocator/runtime globals are initialized.
2. Invoke the original allocation wrapper `0x141473368` with count 2 and element size `0x190` in that process. Record the returned base and the allocator request/return.
3. Query the allocation and address range containing `base+0x320` in the same process (`HeapSize`/heap inspection if the slot resolves to HeapAlloc, plus `VirtualQuery`), then perform exactly one guarded 32-bit read at that address. Record success and the value, or the exception code/address.

This is the smallest no-GUI probe for the allocator/readability question, but it is only a sample of that process's heap state. It cannot establish that the actual Cogging writer's normal parser allocation in a fully initialized project has the same adjacent allocation layout. To answer that exact path in one run, the debugger must instead stop at `0x14076cb80` after an ordinary two-Reheat project has been loaded and observe `r11` plus the guarded load. That requires arranging the native model/stream state; no existing CLI/runtime receipt shows a supported no-GUI route to do so. Do not substitute a fresh allocator probe result for the actual writer's outcome.

A single successful read disproves a universal guaranteed-fault claim for that observed allocator state; a single fault proves only that run's allocation faulted. Neither result defines a portable output contract. A no-GUI in-context exception experiment should capture the actual pointer, committed/protection state, exception status, handler disposition, and whether the writer returns/closes, without changing the production binary.

## Conclusion

The smallest sufficient static invariant remains the logical out-of-range pointer arithmetic. The actual fault is not determinable from the PE for ordinary inputs. Keep the candidate two-Reheat refusal guard; do not map it to an original crash status or serialize a guessed adjacent-memory value.

Evidence used: immutable `DEF_GUI_MO_64.exe` SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; bounded allocation/read slices in CP64; fault-handler chain in [resume66-cogging-fault-containment.md](resume66-cogging-fault-containment.md). No source, build, GUI, mutable REA, or runtime experiment was performed for this checkpoint.

