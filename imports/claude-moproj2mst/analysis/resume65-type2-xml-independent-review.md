# Checkpoint 65: Type2 InputTable XML device review

## Review result

The proposed `StudyXmlFile` has the right separation for the native InputTable writer's principal I/O contract: it can preserve WriteOnly/Text output, flush the stream when QFile closes, and report QFile error after close independently from QTextStream status. The concrete `writeType2InputTableXmlPrefix` remains deliberately incomplete at the subsequent `DoeTableRefresh` boundary. The outstanding problems are integration obligations: the recording sink is success-only, the recorded XML payload is not yet part of the public Save-effects result, and the public caller must still issue refresh even when XML writing returns false.

Native `0x1400d3200` is the wrapper, not just `0x14090a550`: after dimension reconciliation and XML writing it unconditionally emits `2signalDoeTableRefresh()` before returning the saved writer result. The CP64 native report `analysis/resume64-doe-save-effects.md` records that ordering and its original PE/call receipts. Thus a separate helper that returns after XML is a valid prefix, but the integration cannot gate refresh on `writerSucceeded` or treat this helper's return as completed InputTable Save.

## Device and text behavior

The implementation inspected is `source/apps/moproj2mst/include/m2m/doe_xml_output_device.hpp`, `src/doe/xml_output_device.cpp`, `src/doe/io.cpp`, and `src/doe/type2_xml_prefix.cpp`.

- The native path constructs the existing `mst::FileDevice` at `openInputTableXmlFile`, so open/truncate occurs before XML DOM serialization. `NativeBinary::close` delegates to FileDevice close; the nested `aboutToClose` callbacks forward the QFile close notification to `mst::Stream`, allowing pending stream text to flush before the QFile error query.
- `mst::Stream(codepage, file)` encodes the QString stream using `qt::encodeLocale`; codepage 0 reaches Windows `CP_ACP`. The outer `TextFile::write` passes the encoded bytes through `mst::writeTextMode`, which translates newline LF to CRLF. The InputTable XML serializer supplies ordinary LF-separated `xml::toString` output, so this preserves the intended locale-codec plus QFile Text transformation. This does not depend on PAR's already-encoded byte path.
- The helper closes before reading `file->error()` and sets `writerSucceeded` from that post-close QFile error only. `streamFailed` is kept diagnostic and does not decide the native writer Boolean. This matches the retained native distinction: `0x14090a550` checks QFile error after close; `0x140915360` for PAR returns false on open failure and true after a successful open/write/close without checking post-close QFile error.
- Open failure sets `Failed to open file, %1`; nonzero post-close error sets `Error writing file, %1`. Success does not clear the retained global last-error value, consistent with the documented sticky global semantics.

These observations qualify the architecture, not fresh original execution or byte-fault parity. Concrete native partial-file residue still depends on the existing FileDevice behavior and failure state; do not infer durable bytes from stream-request receipts alone.

## Recording visibility and status gaps

Recording `openInputTableXmlFile` currently clears the normalized simulated file at open, then the RecordingBinary append callback updates `writes_[key]` for each byte accepted below the text-mode wrapper. A later `StudyIo::readFile`/`openReadFile` therefore observes the bytes already written, including partial prefixes if the recording device were to accept only a prefix. `XmlWriteAttempt` also records pre-Text requested stream bytes, accepted counts, phase order, close and error observations.

However, the built-in `RecordingBinary` always reports opened, accepts all bytes, returns successful flush, and reports `error() == 0`. It cannot represent failed-open, write/flush failure, or nonzero post-close QFile error. This is suitable only as a declared successful recording simulation; it cannot satisfy the integration report's XML-failure fixtures or be cited as native error parity. Add an explicit injected recording device/state if those cases are needed. Otherwise keep the failure cases guarded and clearly mark the recorder's success profile.

The recording bytes are also not yet exported through `SaveEffects.project` or another public side-output channel: they live in RecordingStudyIo's private `writes_` map, while `XmlWriteAttempt::requestedStreamBytes` is pre-Text data rather than the actual file bytes. Immediate visibility to later StudyIo readers is real within that in-memory adapter, but it does not by itself deliver DoeInputTable.XML in `convertToBytes`/the caller-visible save result. Root integration needs a single ordered, caller-owned effect/result surface or an explicit contract that this output is intentionally internal to this prefix; do not create two executors that both write it.

## Refresh-after-false gate

`writeType2InputTableXmlPrefix` returns on failed open after storing the error; after successful open it returns the post-close result, including false QFile error. It contains no DoeTableRefresh dispatch. That is correctly scoped if the next stage is guarded at refresh, as the source comments say. The public `0x1400d3200` wrapper nevertheless sends the signal on both Boolean outcomes, then its caller ignores the ordinary false and proceeds to current PAR plus modes 0/1/2. Therefore:

- Treat both open-false and post-close-error results as reached XML completion states that proceed to the refresh phase.
- Do not use the XML Boolean to skip refresh or later PAR attempts.
- If receiver ownership remains unavailable, stop at the refresh delivery boundary while preserving the XML/error effects and retaining that the native notification is next. Do not claim the native arm itself stops there.
- Once refresh is integrated, its failure/lifetime semantics must be qualified separately; do not let a failed ordinary XML result suppress it.

## Evidence boundary

Native order and result distinctions are taken from `analysis/resume64-doe-save-effects.md`, which cites the CP63 bounded public wrapper/entry evidence and original image hash `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Candidate behavior was inspected in the source paths listed above. This was a read-only review: no files other than this note were written; no build, test, GUI, runtime, or mutable REA was used.
