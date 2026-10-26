# Native Save device and Boolean failure behavior

The native MST and Boolean writers now have a source-bound QFile device instead of relying on atomic replacement. This worker implemented only `mst/file_device.hpp/.cpp`, the Boolean native branch, and `tests/mdt/test_file_device.cpp`. The coordinator owns the shared Stream boundary, MST-open gate, driver, adapter status mapping, integration tests and builds.

The [caller/Qt flush receipt](wave3-boolean-save-io-contract.json) retains ten exact disassembly selections and the original failure matrix. The [buffering supplement](wave3-boolean-save-buffer-contract.json) retains thirteen additional selections, original DLL identity, native imports, candidate hashes and focused test scope. Both use shipped QtCore SHA256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. No original process, function, DLL or UI was executed by this worker.

## Native open and write contract

| Boundary | Recovered behavior |
|---|---|
| QFile::open `0x670b4ae0` | Keeps caller mode 2 on QFile; passes mode OR `0x20` to its engine. Thus QFile itself remains buffered. |
| QFS open `0x670ea5c0` | WriteOnly without Read/Append adds truncation bit `0x8`; calls native open `0x670f8150`. |
| CreateFileW at `0x670f81f7` | `GENERIC_WRITE=0x40000000`, share `3` (read/write), `OPEN_ALWAYS=4`, normal attributes `0x80`, null template; security attributes size 24, null descriptor, inheritance false. No delete sharing, temporary name, parent creation or rename. |
| Post-open truncation | Engine slot `+0x70` resolves through QFS vtable `0x672346b8` to `setSize` at `0x670f9420`, called with zero. Native open ignores its result and returns true. On the fresh native handle at offset zero, FileDevice uses `SetEndOfFile` and preserves that ignored result. |
| QFilePrivate constructor `0x670b5b30` | Initializes the ring buffer with basic block size `0x4000`. |
| QFile::writeData `0x670b6010` | If pending plus incoming exceeds `0x4000`, flush first; a failed flush returns -1 before appending new bytes. Incoming larger than `0x4000` then goes directly to the engine; smaller/equal input is buffered and reported accepted. |
| Native write `0x670f8690` | Uses WriteFile chunks up to `0x2000000`, loops after positive successful writes, stops on zero, returns -1 if failure occurs before progress, otherwise returns accumulated positive progress. Its remaining counter starts at the input length's low 32 bits, preserved in FileDevice. |
| Native flush `0x670f7ee0` | For this path-open handle, stdio pointer `+0x70` is null: returns true without `FlushFileBuffers`. |

The segmented buffer is ported from reserve `0x670b5340`, consume `0x670b5a20` and clear `0x670b58b0`. It appends within the logical backing length, grows the existing block when the tail is below half that length, or shrinks the old block to its used tail and appends another block. Flush helper `0x670b5d80` writes only the first contiguous segment and consumes a positive returned prefix. QFile::flush compares that count with the total pending count before the write; it does not drain all segments in a loop.

Each QTextStream `endl` (`0x670c6d60`) inserts LF and runs private flush `0x670c2f50`. That clears the source text before writing to the device, then calls QFile::flush. A nonpositive/short write or failed QFile flush sets private stream status `+0xcc` to 3. Later text flushes return early while that status is nonzero.

QFile::close `0x670b5f30` independently retries its remaining byte buffer once, then clears that buffer and closes the engine. This retry still happens after the separate QTextStream has failed. Native CloseHandle failure updates the Qt file's internal error state; the Boolean helpers never inspect that state. The implementation preserves the byte-buffer retry/discard sequence without inventing GUI messages or exposing new ABI fields.

## Caller status and Boolean application

Original Boolean helper `0x1411e1820` always attempts BWZ then INI when its owning operation is available. It returns the BWZ result; the MST caller ignores it. A successful-open Boolean writer returns true after close even when the stream records a later failure. Failed opens return false without serializing that file and do not suppress the second attempt.

The new native Boolean branch opens FileDevice before constructing its Stream, skips that file if open failed, closes before Stream destruction, and attempts the second file independently. Native attempts do not append staged `fx.writes` or `Ctx.pendingWrites`. Recording mode keeps its existing planning path.

MST writer `0x1401313c0` returns false on failed open, but top Save `0x1400980d0` returns its earlier subproject-check result rather than the combined project/MST success Boolean. With successful checks, an MST open failure therefore still returns `0x10000`. The mode-checked wrapper `0x140028520` tests that status and logs `Current project has been saved.`. Auto-save wrapper `0x140010b80` ignores the result and always returns 1 after logging `Auto saving...` and the saved message. These are recovered internal/GUI contracts; they do not automatically dictate the compatibility adapter's error codes. The coordinator explicitly keeps the existing `m2m_*` ABI mapping qualified separately.

## Focused validation and remaining limits

Four FileDevice tests are ready under target `m2m_mdt_file_device`, CTest `m2m.mdt.file_device`. They verify immediate truncation, deferred small writes, the exact 16KiB boundary, the larger direct-write branch, close flushing, failed opens without parent creation, and deterministic lock failure/recovery.

The lock test acquires an exclusive LockFileEx range through a second share-compatible handle. The first line must remain buffered after the blocked flush, and later stream text must be suppressed by sticky failure. After unlocking, FileDevice.close must write exactly the first line, leaving later text pending in Stream. This tests candidate native I/O only; the coordinator owns execution and review of its result.

Actual positive short writes, CloseHandle failure and device/plugin/CRT-descriptor branches have not been dynamically injected. Existing `win32Path` long-path support is retained as an adapter extension, not claimed equivalent to original Qt path limits. Earlier Save stages still use inherited deferred effects, so complete project-wide file ordering and failure-state equivalence remain open. Earlier Boolean Open-time editor creation is likewise unresolved; none of this package justifies an extra Open mutation.
