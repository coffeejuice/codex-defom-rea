# QFile close flushes attached stream text

The earlier assumption that pending QTextStream text is lost when the caller closes QFile before destroying the stream was wrong for the recovered native path. Shipped Qt connects the file's `aboutToClose()` signal to an embedded `flushStream()` receiver. The corrected [receipt](wave3-boolean-close-flush-contract.json) preserves that prior assumption as superseded and retains eleven bounded static captures from the fixed original QtCore DLL.

QTextStream's QIODevice constructor `0x670c4b00` connects literal `2aboutToClose()` at `0x6722c308` to `1flushStream()` at `0x6722c318`, using the embedded receiver at private state `+8`. `setDevice` at `0x670c3a70` installs the same connection. Raw connection type 3 is normalized to zero in shipped QObject::connect at `0x6713162c`–`0x67131633`. Same-thread activation follows its direct path; arbitrary cross-thread lifecycle is outside this recovery.

Private constructor `0x670c2b60` installs receiver vtable `0x6722c288`. Slot `+0x10` resolves to meta-call `0x670c6dd0`; method zero follows the receiver's owner pointer to the Stream private state and calls existing `flushWriteBuffer` at `0x670c6e04`. QIODevice::close emits `aboutToClose` at `0x670ba491` before clearing its open mode at `0x670ba498`, so that flush runs while the file remains open.

The native close sequence is therefore:

1. QFile retries its own byte buffer once, independently of stream status.
2. QIODevice emits `aboutToClose`; the attached Stream flushes pending text through its existing sticky-error guard.
3. The device is marked closed, remaining QFile buffer bytes are discarded, and its native handle is closed.

With stream status 3, step 2 still suppresses later text. This does not suppress step 1: the lock test's earlier buffered line can recover at close while its later stream text remains pending.

The Stream destructor also flushes. Exported destructor `0x670c4f20` checks the pending write-buffer length and calls `0x670c2f50` at `0x670c4f56` before destroying private state. Private destruction `0x670c2030` releases its members and tail-calls QObject destruction on the embedded receiver. The external-device constructor's reset clears ownership byte `+0x20` at `0x670c2264`, so Stream destruction does not close or delete the externally supplied QFile.

The coordinator added scoped callback binding/unbinding and destructor flushing to Stream. This worker inserted `notifyAboutToClose()` between FileDevice's initial byte-buffer flush and discard/handle-close, then added a fifth native-device test. It verifies both no-newline text flushed by FileDevice.close and no-newline text flushed by Stream scope exit while its external FileDevice stays open. All source/tests were frozen before the coordinator's build began; this worker did no build or runtime execution.

The `.moproj` writer particularly depends on this path. Original `0x1400983c0` sets UTF-8 codec, streams one QDomDocument string without an explicit endl, closes QFile, then reads **QFile::error**, not QTextStream status. Successful-close pending text therefore cannot be omitted. QFile close can also clear an earlier file error when its initial byte-buffer flush and engine close succeed, even while the separate stream retains status 3. Project-writer return recovery must preserve that distinction; it cannot be inferred solely from Stream::writeFailed().

Earlier receipts are retained as chronological evidence. This receipt supersedes their incomplete close lifecycle and any resulting pending-loss claim; it does not establish complete project-wide failure ordering, Qt filesystem-plugin support or exact cross-thread behavior.
