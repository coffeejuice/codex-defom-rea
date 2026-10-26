# Resume 57: PPV file-line input contract

## Finding

The current PPV point-list parser accepts decoded `qt::String` lines; it does
not open files. Native `loadSimCtrl` opens `SimCtrl.PPV` as `QFile` with
`QIODevice::ReadOnly` (mode value 1), constructs a default `QTextStream` over
that device, and calls `readLine()` for the count and each requested row. The
native path therefore has a different input contract from the existing
`readTextLines` helper in `src/engine/l5_slots.cpp`.

The helper is explicitly documented as `QFile(ReadOnly | Text)` plus
`QTextStream` line semantics, but its implementation reads raw bytes, rejects
UTF-8/UTF-16 BOMs as unsupported, strips every `\r`, and decodes with
`MultiByteToWideChar(CP_ACP)`. Reusing it for PPV would introduce an unsupported
failure on valid BOM-marked input and would assume the PPV stream uses that
specific byte-to-Unicode path. The native reader's default `QTextStream` codec
detection/decoder is not reproduced there. This is a demonstrated source
difference; it does not establish the exact native decoded result for every
codec or malformed byte sequence.

## Source qualifications

- `analysis/resume48-ppv-reader-receipt.txt` preserves the bounded complete
  decompile of original `FUN_140d4b400` from
  `evidence/gui_mo/s1/mdt/L0/A/b0.c` (source SHA-256
  `cb28de6852d5b6279719d2576759a9dcc0f640e436bcdf846caa691622c58698`). It
  shows the QFile mode value `1`, QTextStream construction on the QFile, and
  successive `readLine()` calls. It contains no explicit `setCodec` call.
- `analysis/resume57-ppv-open-binding.md` records the qualified loader order:
  the file is checked for existence and read only on the branch after
  `SimCtrl.KEY` handling. It also records the unresolved model effects that
  prevent enabling the public path.
- `src/engine/l5_slots.cpp::readTextLines` is current source evidence for the
  alternate helper's exact BOM rejection, CR removal, and CP_ACP decode.
  The comment above it describes the helper's intended text-mode semantics;
  the implementation does not instantiate QFile/QTextStream.
- `include/m2m/engine/simctrl_ppv_reader.hpp::readPpvLines` documents that its
  `lines` span contains strings returned by successive native
  `QTextStream::readLine()` calls, including the count line. It intentionally
  remains an isolated parser seam.

## Highest-value discriminating input

Use a temporary `SimCtrl.PPV` containing a small valid ASCII PPV payload encoded
as **UTF-16LE with BOM**, with CRLF line endings, for example the logical lines
`1` and `0 true 2 3 4.5 6.5` (plus final CRLF). Ensure the test converts those
logical lines into UTF-16LE code units and writes the leading `FF FE` bytes.
This single fixture separates the implementations at the file boundary:

- A native default QTextStream should detect the UTF-16LE BOM, consume it as
  encoding metadata, and provide the count and row as Unicode lines. CRLF
  handling is then exercised by `readLine()` itself; the row parser treats
  whitespace separators, including a retained terminal carriage return, as
  whitespace.
- The current `readTextLines` returns its explicit unsupported-BOM result
  before line splitting or parsing. It therefore cannot stand in for this
  native input operation.

The row uses ASCII parser fields so this case does not depend on the machine's
numeric ACP or on non-ASCII conversion behavior. If the test adds a non-ASCII
character, put it only in the ignored first field and treat that as a secondary
codec-detection observation, not a required parser assertion. No claim is made
here about the precise BOM handling of every QTextStream version or malformed
encoding; this input should be checked against the retained Qt 4.8 runtime
contract or original runtime evidence before describing it as behaviorally
settled.

## Smallest isolated file-reader seam

Add a dedicated internal PPV file-lines function alongside the PPV reader
integration, separate from `readTextLines`. It should take a path (or an
injected QFile-like opener), open with exactly `QIODevice::ReadOnly`, create a
default QTextStream, and return the successive `readLine()` strings plus an
explicit open-success bit. A filesystem-free unit seam can instead inject a
reader callback that produces decoded lines and independently reports open
failure; the UTF-16LE file case then belongs in a small reader-adapter test.
Keep `readPpvLines` unchanged and feed it only the decoded result. Do not call
this adapter from public `loadSimCtrl` yet: the loader's per-slot `ppvCount`
mutation and predecessor/root-list Save handling remain separate blockers in
`resume57-ppv-open-binding.md`.

No source or test files were edited, no build was run, and the public PPV path
was not enabled.
