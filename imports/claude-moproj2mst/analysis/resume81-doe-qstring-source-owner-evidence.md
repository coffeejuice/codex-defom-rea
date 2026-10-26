# CP81: actual resource QString slot and data owner

The resource QString constructor and its conditional fresh-data owner are now recovered from the original PE. The source call constructs the actual temporary at `rsp+0x58`; its returned slot contains the helper's data pointer. With a null `QString::codecForCStrings`, the nonempty resource gets a fresh allocation with refcount **1**, rather than an alias of shared-null. The remaining native edge is the invocation-time codec state and, when non-null, the concrete virtual decoder's returned data owner. No production receiver invocation, native address, allocation generation or shared-null count has been observed or bound.

This report supersedes the CP81 binding plan's description of the constructor edge as wholly missing. It does not admit QFileInfo, custom engines, QIcon addFile, receiver delivery or public Type2 Save. No source files, build/test state, GUI, running target or analysis database were changed.

## Evidence and tooling

The retained original `DEF_GUI_MO_64.exe` SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; `QtCore4_64.dll` is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. New captures use the existing read-only `admin/capture_pe_ranges.py` and local DUMPBIN, with separate `resume81-doe-qstring-*` receipts. These are preferred image addresses, not loaded-process addresses.

`evidence/rea/rea-ghidra-1b551070dca4010f1dea92c881268ec0/target-bcf0074c7e9f.dll` has exactly the retained QtCore hash. Its sibling project-store files and logs are imported analysis residue; they were neither opened nor mutated. Their presence corrects the tooling inventory's broader statement that no project-shaped imported residue exists. Database completeness and usability remain unverified. Existing text decompilation `evidence/gui_mo/out_p05/qt_paths.c` supports the downstream QDir/QFileInfo context but does not recover this constructor's allocation branch. The evidence below comes from hash-verified PE bytes, imports and bounded disassembly.

`resume81-doe-qstring-source-fields.json` retains the exact imports, literal, module identities and codec global section metadata. `resume81-doe-qstring-codec-init-fields.json` retains codec setter imports and selected codec names. Capture receipts record byte ranges and capture hashes. Adjacent instructions/padding in explicit leaf spans are not treated as part of the selected function; `.pdata` subdivisions are joined only by the observed fallthrough/branch edges below.

## Actual source object and lifetime

Existing `resume51-doe-variable-notify-hasdoe.asm` shows the matched true receiver `0x140086ca0`:

1. `0x140086d5c` passes the literal at `0x14213afd8`; `0x140086d63` passes `rcx = rsp+0x58` to imported QString(char const*) at `0x140086d68` / IAT `0x141e04480`.
2. Import parsing resolves that slot to `QtCore4_64.dll!??0QString@@QEAA@PEBD@Z`, export RVA `0x174bd0`. The source literal is exactly `:/SfTool/icon/tool/iconDoe_24_24.png`, **36 bytes** before NUL. It contains no backslash.
3. The constructor `0x67174bd0` preserves `this`, calls `fromAscii_helper(chars,-1)` at `0x67174be2`, stores its returned data pointer into `[this]` at `0x67174be7`, then returns `this` at `0x67174bea`. A helper exception leaves no completed QString slot or returned slot receipt.
4. The caller saves the returned **object pointer**, not a second data owner, in `rsp+0x70` then `rsp+0x78`. It passes that pointer as addFile's path at `0x140086d8b/0x140086d95`. Temporary destruction uses the original `rsp+0x58` slot at `0x140086d9c/0x140086da1`.

The caller's state-1 cleanup already qualified in CP78 destroys the completed temporary when addFile throws. QString destruction decrements its data ref and frees on the zero transition, as qualified in CP80. The heap QIcon has separate ownership; it is not made exception-safe by the QString cleanup. Actual Session/model receiver identity must still come from the real delivery path; a fixed path value in `DoeIconShellPrefix` is not this constructor's returned object.

## Fresh owner when C-string codec is null

`resume81-doe-qstring-source-67074120.asm` shows `QString::fromAscii_helper` testing the mutable global `0x672f3e10` first. The null branch tail-calls `QString::fromLatin1_helper` at `0x6706bf40`. The resource is non-null and nonempty and length is passed as -1, so its path scans to NUL and falls through to `0x6706bfa0`.

The logical procedure spans the retained `6706bf40`, `6706bfa0`, and `6706bff6` captures. It:

- Calls `0x670203a0` with `2*length+0x20` bytes; its thunk IAT `0x671a8610` is `MSVCR90.dll!malloc`. For this literal, the request is **104 bytes (`0x68`)**.
- If malloc returns null, calls `0x6701bfb0` before publishing data. That function constructs a `std::exception` and enters the C++ throw path. A successful constructor return or data owner must not be emitted for this failure.
- Sets the data pointer at allocation `+0x10` to allocation `+0x1a`, writes refcount **1** at `+0`, writes length/allocation fields **36** at `+4/+8`, clears the low five flag bits at `+0x18`, and writes a UTF-16 NUL at `+0x1a+2*length`. The flag mask preserves other bits; it is not evidence that the complete flag word or padding is initialized to zero.
- Widens the Latin-1 bytes into UTF-16, using two 16-byte vector chunks for the first 32 bytes and a four-byte scalar remainder for this resource, then returns the fresh allocation at `0x6706c063`.

There is no shared-null increment on this nonempty null-codec branch. This closes the source initialization count for that newly created input data: its initial refcount is 1, before downstream aliases. It does not close the independent shared-null mutable count which QFileInfo's later file-entry construction needs. Nor does a fresh allocation's proven initialization supply its actual invocation identity until that source constructor is executed in the reconstructed path.

## Non-null codec and real native setter

With non-null codec, `0x67074120` measures the nonempty text and calls `QTextCodec::toUnicode(chars,length,nullptr)` at `0x6707419f`. The wrapper `0x67072460` dispatches virtual `+0x18` on the actual codec and supplies an output QString slot. On normal return, fromAscii increments that slot's returned data at `0x670741a9`, then destroys the temporary reference at `0x670741b2` (free-on-zero branch retained). It returns the retained data pointer. The decoder's concrete returned data, initial count and aliasing cannot be obtained from the ASCII resource value alone. Generic codec assumptions, host locale conversions and a fabricated refcount are outside this evidence.

The global is in the zero-filled virtual tail of QtCore `.data`; this establishes initial image state only. There is an actual imported setter in the original GUI:

- IAT `0x141e04350` resolves to `QTextCodec::setCodecForCStrings`; QtCore `0x67177590` directly stores its argument into `0x672f3e10`.
- The only matching direct `ff 15` RIP call pattern in GUI executable sections is confirmed at `0x1400026ee`, in the `0x1400024f0` capture. This bounded scan is not an exhaustive analyzed xref database.
- The routine reads language state at `0x143d79718`, selects Shift-JIS for 1, Big5 for 2, KOI8-R for 3, EUC-KR for 4, or GBK for 7. A selected name is passed to `QTextCodec::codecForName` at `0x1400026c0`. If it returns non-null, the routine sets the locale codec, translation codec and C-string codec in that order. Other values leave the name null and skip this setter section.

This proves a source route that invalidates an unconditional null-codec assumption. It does not prove this initialization ran for a particular B121 invocation, identify the returned codec implementation, or exclude later writes from other modules.

## Smallest next dependency and test contract

The next safe integration action is to bind the actual resource constructor to a canonical QtCore process owner only once its **invocation-time C-string codec state** is represented. A null observation can admit the recovered Latin-1 fresh allocation branch and publish the actual source slot/data generation with refcount 1 after success. A non-null or unknown observation must guard before the decoder call until its actual codec owner/vtable and return ownership are qualified. No additional unknown-state Session holder alone advances this native edge.

An implementation-ready focused test must distinguish: null codec + successful fresh resource (36 UTF-16 code units, ref 1, unique allocation generation, actual slot return); allocation failure (no completed slot/data publication); unknown/non-null codec (refusal before allocation/decoder); and source temporary cleanup (one ref release, with zero-transition free still requiring the canonical owner's free boundary). Identical resource text on a second source call must not reuse a fresh allocation merely by value. Public receiver/Type2 guards stay in place.

The independent shared-null identity remains the QtCore symbol at RVA `0x2f0b30`; its current mutable count is still unqualified. The constructor result above can close the input's initial count conditionally, but cannot authorize QFileInfo's later shared-null alias increments or native custom-handler selection.

No build or runtime test was run in this read-only lane. All production/source ownership remains released to root.
