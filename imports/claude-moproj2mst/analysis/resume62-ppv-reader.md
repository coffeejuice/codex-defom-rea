# Resume 62: PPV physical reader ordering and CR qualification

## Audit result and narrow fix

The retained QtCore evidence now qualifies embedded CR as ordinary line
content in the original PPV QFile(ReadOnly)/QTextStream path. The line reader
removes a CR only when it immediately precedes LF. The current bounded decoder
had continued to reject any other CR, leaving an unnecessarily narrow profile.
Updated only the PPV file-reader header/source and its test to preserve embedded
CR in a decoded line while still stripping CRLF terminators. The PPV parser's
qualified `\\s+` separator treats that preserved CR as whitespace downstream.

The physical reader's open-state distinction is also tested: a missing path
returns `OpenFailed`; a successfully opened empty file returns `Read` with zero
lines. The original reader opens QFile before clearing the local list, so these
statuses must remain distinct for the Open sequence. A successful empty read
continues to the parser, which clears local points and then returns false for a
nonpositive/missing count; an open failure leaves lists untouched. The
caller-level count update follows either reader result.

## Retained qualifications and guards

- `analysis/resume48-ppv-reader-receipt.txt` records original
  `FUN_140d4b400`: QFile open mode 1 (`ReadOnly`), default QTextStream, then
  successive `readLine()` calls. It clears the local list only after successful
  QFile open.
- `analysis/resume58-ppv-qt-codec.md` and
  `analysis/resume59-ppv-codec-next.md` use the same QtCore 4.8 binary identity
  (SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`)
  to qualify UTF-16LE/UTF-8 BOM selection, default `codecForLocale()` for
  BOM-less data, CRLF stripping, and embedded CR preservation. These are static
  qualifications, not a runtime observation of an original project.
- The decoder still accepts only BOM-free ASCII, UTF-8 BOM with ASCII payload,
  and valid UTF-16LE with BOM. BOM-less non-ASCII, non-ASCII UTF-8, other BOMs,
  malformed UTF-16, and unsupported controls remain guarded because the
  original locale codec or malformed-byte conversion outcome is unqualified.
- The disk reader buffers the physical bytes before decoding; it is not a
  literal QFile/QTextStream implementation. A post-open physical read error is
  not exposed as a distinct status. No static evidence or fault fixture here
  establishes the original's row/list effects for that case, so this increment
  makes no claim about it.
- The existing Project-overlay reader remains unchanged. The new physical
  entry bypasses the overlay and continues to distinguish OpenFailed from
  successful empty input.

The test additions use active Testkit `REQUIRE`/`CHECK` assertions and scratch
paths under the configured disposable test root. No public loader/Save,
CMake, shared session, GUI, or mutable REA change was made. No build or tests
were run; root owns validation.

## Freeze hashes

| File | SHA-256 |
|---|---|
| `simctrl_ppv_file_reader.hpp` | `0804d2fcb99d27e729cb205aa562b0216bf038d62af02c31a6235db4300cf9fc` |
| `simctrl_ppv_file_reader.cpp` | `b6ce018d9187b8f06828f6fefe55c9ad226794481f1ec8cef4133937d607d47a` |
| `test_simctrl_ppv_file_reader.cpp` | `cc26202c1fb0ef41e12bb1bf79ee983bf1e879baa9d98c54d4cf9837725cb10e` |
