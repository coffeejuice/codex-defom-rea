# Resume 60: isolated PPV text-mode device writer

## Implementation

Added `simctrl_ppv_text_device.hpp/.cpp` with two isolated entry points:

- `writePpvTextFile` constructs the existing `mst::FileDevice` for a Unicode
  path and attempts the write-only open;
- `writePpvTextSegments` accepts an already-attempted `mst::OutputDevice`,
  writes each supplied ASCII-compatible line as one `QTextStream` string plus
  `endl`, and closes the device before the stream is destroyed.

The internal `TextModeDevice` wraps `mst::writeTextMode`; each `endl` therefore
passes text bytes and CRLF through separate underlying `write` calls, followed
by a device `flush`. Before `file.close()`, the sink installs the close
callback that flushes the still-live stream, matching the existing
`FileDevice` close notification path. A failed open returns false/no writer
return. After a successful open, `writerReturned` is true regardless of sticky
stream failure; no post-open write, flush, file-error, or close result is
promoted to a new failure result.

The sink accepts already formatted line segments. Its supported payload must
remain ASCII-compatible for the caller-supplied locale codepage; this does not
qualify the original process's exact locale codepage. It neither builds the PPV
records nor enters public Save.

## Tests and fault limits

Added `tests/mdt/test_simctrl_ppv_text_device.cpp` with Testkit assertions
active in Release and no C `assert`. Tests check byte output and per-line
CRLF/flush/close order through a fake output device, failed open, physical
`FileDevice` truncation/write in owned CTest scratch, and injected write,
CRLF-write, and flush failures. Fault cases check sticky later-write
suppression, close still being attempted, and the native true return after
successful open. They do **not** assert any particular on-disk residue after a
fault.

The fake `OutputDevice` records individual write/flush/close events; it is not
evidence about the actual bytes a locked, short-writing, or failing Windows
handle leaves on disk. `analysis/resume59-ppv-save-next.md` remains controlling
for that limit. The production `FileDevice` + `writeTextMode` pair is the
project's existing source-qualified QFile/Text-mode substrate; this increment
does not alter their global behavior or add a PPV-specific partial-file
qualification.

No public `loader.cpp`/`save_current.cpp`, model/session/shared interface, or
CMake file was changed. Existing CMake source/test globs are expected to find
the new files, but root owns configure/build/test validation. No build or tests
were run here.

## Freeze hashes

SHA-256 at freeze:

| File | SHA-256 |
|---|---|
| `simctrl_ppv_text_device.hpp` | `dc828ff183842718b0ba4eeb113e18d931a5f3991b071fc4de1918fbba013f3f` |
| `simctrl_ppv_text_device.cpp` | `0016b5c80b829c943562499f185c7bac292ffa048a03a4d5fe76d983086e4da2` |
| `test_simctrl_ppv_text_device.cpp` | `7cc91e21b66335cfc14dfaffe2b10fa33f962a1537b1dc32e2dc43dffe609b19` |
