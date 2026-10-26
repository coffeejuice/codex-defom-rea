# CP134 Type3 `.OPT` stream owner

## Finding

The retained native parser is a stack-scoped file/stream operation whose mutation target is a separately supplied optimizer pointer. It does not establish that the candidate `SubProject` or `ModelManager` owns an equivalent optimizer, and the current source has no Type3 `.OPT` reader. The smallest safe adapter boundary is therefore a private Type3 parser context that explicitly carries the actual optimizer state plus a read-only file/line source; a generic project reader or DOE `StudyIo` cannot stand in for that owner.

## Native object and lifetime order

`analysis/resume127-type3-opt-parser.asm` is a DUMPBIN capture of the original `DEF_GUI_MO_64.exe` (binary SHA-256 `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`; capture SHA-256 `7C77136A27A1514D94D02A07263A082A9C4C2FDDF2F323CB26335DF57A5172D9`). The function begins at `0x140908630`, takes path in `rcx` and optimizer in `rdx`, spills them at `[rsp+0x220]` and `[rsp+0x228]`, and reserves stack locals.

The relevant owner sequence is visible directly in the capture:

1. Null optimizer at `0x14090864d–0x140908675` returns false before constructing the file object.
2. The path is used to construct a local `QFile` at `[rsp+0x68]` (`0x14090868b–0x140908698`). The file is opened with `ReadOnly | Text` (`0x14090869f–0x1409086cf`). Open failure follows its own message/cleanup branch and does not call the optimizer’s `+0x160` setter.
3. On open success, the parser calls optimizer vtable `+0x160` with `true` (`0x1409087e4–0x1409087f9`), then constructs a local `QTextStream` at `[rsp+0x50]` with the local QFile address as its device argument (`0x1409087ff–0x140908820`). This proves a stream-over-file association for this call, not ownership of the optimizer by the file or stream.
4. The stream reads one line into a local string, trims it and calls base-10 `toInt`; the integer is stored in a local and is not used for dispatch (`0x140908827–0x140908861`). The return local is set to false immediately afterward (`0x140908865`). The subsequent loop checks `atEnd`, reads lines, and dispatches them (`0x14090886d–0x1409088ed`).
5. `ENDDATA` branches to the common cleanup at `0x140909400`. That path closes QFile (`0x140909400–0x140909405`), then destroys the local stream (`0x140909426–0x14090942b`), then the QFile (`0x140909432–0x140909437`), then the path QString (`0x14090943e–0x140909452`). Normal function exit returns the previously initialized false byte (`0x140909458–0x140909466`). The optimizer is not destroyed by this routine; its pointer is an input and the containing Type3 SUB stores the separately constructed object at `+0x28` per `resume125-type3-optimizer-owner.md`.

On the open-failure branch, cleanup calls QFile close and destroys QFile/path locals before returning false (`0x1409087b2–0x1409087df`). Therefore file-open failure precedes the purge setter; post-open failures/termination occur after it.

## First-line, `ENDDATA`, and partial effects

The first line is a header probe, not a dispatched record: trim, decimal integer conversion, and ignored result happen before the `atEnd` line loop. For an empty opened file, this separate first `readLine` still runs before the loop’s `atEnd` check. Successful open has already set the optimizer’s purge flag true at that point. `ENDDATA` exits through close/cleanup; it does not restore the false return value to true. The parser preserves prior scalar writes and row appends made during earlier lines; there is no rollback shown in the retained path. The caller stores the parser result without branching on it, per `resume128-type3-constructor-parser.md`.

Thus the observable ordered prefix is: validate optimizer pointer → construct/open QFile → on success set purge true → associate QTextStream with QFile → consume/convert ignored header line → process subsequent lines in file order → stop on `ENDDATA` or EOF → close file and destroy locals → return false. Open failure stops earlier and leaves optimizer purge state untouched by this routine.

## Candidate source boundary

Current source keeps the relevant owner unbound:

- `src/model/subproject.cpp` documents that Type3 optimization data at `SubProject +0x28` is unsupported; `makeSubProject` does not allocate the native optimizer.
- `src/ops/report.cpp` (`ShapeOptimClass::loadClassData`) explicitly refuses Type3 because the optimizer object is not modeled. The Type3 refusal in `src/engine/shapeoptim_load.cpp` remains in the later save path as well.
- `src/engine/model_manager.cpp::emitModelSignal` still refuses Type3 model-signal dispatch; it is not a parser or optimizer owner.
- The DOE `StudyIo::openReadFile` facility is scoped to the DOE/InputTable path and does not supply the optimizer pointer, the native `QFile`/`QTextStream` lifetime, or this first-line/header protocol.

The smallest future adapter contract should therefore take an explicit optimizer-state reference/handle and a Type3-specific ordered reader capable of reporting open failure before any state mutation, then successful open, header-line conversion, line/end status, and close. It must preserve immediate per-line mutations and distinguish file/stream ownership from optimizer ownership. Until constructor defaults, mutation slots, and row conversion/append effects are all mapped to candidate state, implementing even this adapter would only be a structural seam; it would not justify removing the public Type3 refusal.

## Scope

Read-only synthesis of the retained parser assembly and the cited source/report files. No public Type3 behavior, source, build, test, GUI, original execution, or mutable REA state was changed. The capture gives exact local object/lifetime ordering and the parser’s prefix; it does not identify the full C++ class layout or establish an equivalent candidate optimizer owner.
