# CP476 independent QtCore writer review

This review corrects preferred-base arithmetic and establishes static writer decisions in the copied Qt DLL. Actual codec choice, caller state, OS bytes, and converter fidelity remain unproved by these static bytes.

## Artifact and method

Artifact: `evidence/original-binaries/QtCore4_64.dll`; SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. Its PE32+ optional header has preferred base **0x67000000**, not 0x180000000. Addresses below are preferred VAs, not observed runtime addresses.

Exports and exception-directory bounds were independently parsed from the copied PE. Tool: `C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\dumpbin.exe`, reported version 14.44.35228.0, bounded `/DISASM /RANGE`. All twelve ranges in the hash table were re-disassembled and every instruction byte compared to PE section bytes; coverage was complete and contiguous, with zero mismatches. No DLL/original/reconstruction execution, GUI, REA, build, or source edit occurred.

## QString insertion

Export `??6QTextStream@@QEAAAEAV0@AEBVQString@@@Z` is RVA C4460, preferred VA **670C4460**; exception-directory range `[670C4460,670C44A0)`, 64 bytes, 20 instructions. Complete bytes:

```text
40 53 48 83 ec 20 48 8b d9 48 8b 49 08 48 83 79 28
00 75 1b 48 83 39 00 75 15 48 8d 0d 47 7f 16 00 e8
5a 8a f5 ff 48 8b c3 48 83 c4 20 5b c3 45 33 c0 e8
99 f0 ff ff 48 8b c3 48 83 c4 20 5b c3
```

Raw signed rel32 calculation independently gives:

| Call VA | Encoding | Target VA | Binding |
|---|---|---|---|
| 670C4481 | E8 5A 8A F5 FF | 6701CEE0 | exported qWarning(char const*,...) |
| 670C4492 | E8 99 F0 FF FF | 670C3530 | substantive private string-format/write helper |

Both bodies are nonempty. qWarning has range `[6701CEE0,6701CF0F)` and calls 6701CD90. Insertion loads private state from public stream+8; when both private+28 and private+0 are null it warns with PE string 6722C3C8 (`QTextStream: No device`). Otherwise it calls 670C3530 with R8D=0, then returns the original stream reference.

Helper `[670C3530,670C37E5)` copies/refcounts the input QString and handles padding/field width. At 670C3785, private+28 determines the destination. A nonnull target receives exported `QString::append(QString const&)` (RVA 70410), called at 670C3793. Otherwise call 670C37A6 appends to private+90; 670C37B2 compares its UTF-16 length to 4000h, and only length **greater than** 4000h calls 670C2F50 at 670C37BF. Thus insertion can cause codec/device activity through this threshold branch; small insertion alone does not force a device write.

## endl and flush

Export `?endl@@YAAEAVQTextStream@@AEAV1@@Z` is RVA C6D60, range `[670C6D60,670C6D85)`, 37 bytes, 11 instructions:

```text
40 53 48 83 ec 20 ba 0a 00 00 00 e8 c0 d2 ff ff 48
8b d8 48 8b 48 08 e8 d4 c1 ff ff 48 8b c3 48 83 c4
20 5b c3
```

670C6D66 sets EDX=0A; 670C6D6B calls exported QChar insertion at 670C4030. That body constructs `QString(QChar)` through export 6706B720 and calls 670C3530 with R8D=0 at 670C407D. Then endl loads returned stream+8 at 670C6D73 and calls 670C2F50 at 670C6D77. This is newline insertion followed by an explicit buffer-flush attempt, subject to the helper's guards. Public `QTextStream::flush` export C37F0 is a leaf wrapper with bytes `48 8B 49 08 E9 57 F7 FF FF`: load private state and tail-jump to the same helper.

Flush helper `[670C2F50,670C30DE)` establishes this order:

1. 670C2F67..670C2F96 return when private+28 is nonnull, device+0 is null, status+CC is nonzero, or QString buffer is empty.
2. 670C2F9C queries exported `QIODevice::isTextModeEnabled`. Leaf BA3D0 reads device-private open-mode+88 bit 10h. If set, 670C2FAE calls exported `setTextModeEnabled(false)` (BC240), then 670C2FE2 calls exported `QString::replace` (73C30) replacing QChar LF with QLatin1String at 6722AE90. The literal begins **0D 0A 00**. This is Unicode-level CRLF substitution before encoding.
3. If codec private+38 is null, 670C2FEE calls exported `QTextCodec::codecForLocale()` (14DF70) and stores it. 670C3025 calls exported `QTextCodec::fromUnicode(QChar const*,int,ConverterState*)` (6D410), using converter state at private+60.
4. 670C3032 calls helper 67021650 on the QString write buffer before the device write. That helper's complete internals were not audited here; call and order are established.
5. 670C3047 invokes exported `QIODevice::write(char const*,__int64)` (BB750) with the converted QByteArray. Nonpositive result reaches private status+CC=3 at 670C3054. Text-mode restoration at 670C307F occurs only after the positive-result branch. Later short-write or QFile-flush failure can reach status+CC=3 at 670C30B9.
6. 670C308E dynamically checks for QFile; nonnull result calls exported `QFile::flush()` at 670C309B. This is an additional file-buffer flush attempt, not proof of OS durability.

`QIODevice::write` has its own Text path. Its complete code envelope `[670BB750,670BBA2D)` spans six contiguous runtime-function fragments with RVA bounds BB750:BB7C3, BB7C3:BB827, BB827:BB950, BB950:BB964, BB964:BB9AC, BB9AC:BBA2D. It tests open-mode bit 10h at 670BB81A, scans for byte LF, calls device virtual slot+D0 for preceding spans at 670BB861, then writes the two-byte CRLF literal at 670BB8A1. The non-Text branch calls that slot at 670BB9B8. QTextStream temporarily disables Text mode for its encoded write, preventing ordinary double substitution on this path. An existing CR before LF is not removed by either shown substitution. Device virtual dispatch and OS effects were not recovered here.

## Destruction and file close

Exported `QTextStream::~QTextStream` `[670C4F20,670C4F92)` checks buffer QString length at 670C4F50 and calls 670C2F50 at 670C4F56 when nonempty; it then destroys private state through 670C2030, deallocates, and clears public+8. This establishes conditional flush-before-destruction. No direct QFile::close call appears in this bounded destructor.

Exported `QFile::close` `[670B5F30,670B6003)` calls exported isOpen at 670B5F50, returns if false, otherwise calls QFile::flush at 670B5F60, retains its Boolean result, calls exported QIODevice::close at 670B5F6B, resets internal fields, then calls engine virtual slot+10 at 670B5F8D. Error paths depend on both retained flush result and engine-close result. It returns void, with no transactional guarantee established.

Exported `QFile::flush` `[670B5E00,670B5F2D)` checks the engine, conditionally drains its write buffer through 670B5D80, and calls engine virtual slot+18 at 670B5EBF. Mismatch/failure paths obtain engine error and return false. Engine/native write/flush/close implementations and actual filesystem outcomes are outside this review.

## Raw code hashes

Bounds are RVAs, ends exclusive; add 67000000h. Hashes are SHA-256 of exact raw code, not text. Text-mode leaf methods lack exception-directory entries; bounds end at their return instructions. QIODevice::write uses the six fragments above rather than only its first exception entry.

| RVA range | Bytes / instructions | SHA-256 |
|---|---:|---|
| C4460:C44A0 | 64 / 20 | 0e2c8574dbfe2913f988d39591d4d75ce0391b84988eebb77cc66058839ff4af |
| 1CEE0:1CF0F | 47 / 11 | ce65b021f7e790f47ae7a473f3bb24070f4ff3c17216ab44eb178a1be6226beb |
| C3530:C37E5 | 693 / 176 | 12765f7f7f6c4cc4e3282f64a8d763d7c49038251851d6eaf9fde603980409af |
| C2F50:C30DE | 398 / 99 | 055b1cc507e8a003f8383a3974a309510ce0718e45edf0105a1a747408dce21c |
| C6D60:C6D85 | 37 / 11 | 29d5f3a787227b99e38637fc17e1759a350285ce87ba0e885582176c3db0da98 |
| C4030:C40A6 | 118 / 32 | 755b8b77f03f5cd18ed0f7594e67b1f25c826a8ef77e1ea4d8f02f67186b4efd |
| BB750:BBA2D | 733 / 192 | 38353a37a820db573aef9d6a68c18911cbfe6417520a310120a08feaba7a6aec |
| BA3D0:BA3E0 | 16 / 5 | 0a45096ebf138034b8bade88e799759273236e214ee9bca81c2f404dcb07b797 |
| BC240:BC26D | 45 / 11 | 069188c594fa8ea285523accdf52ba8a31af154d911574da046e4a49c2ff7918 |
| C4F20:C4F92 | 114 / 28 | b8956d3da4c5a08ecd9f7c71a5843118d07d2238bcec9193c02586e0c7b013b5 |
| B5F30:B6003 | 211 / 51 | b9f60c84838c1783537474028b7e5699ca1723fdb9a65cd31b74d325c3dd83f0 |
| B5E00:B5F2D | 301 / 75 | 3505626dca8617f12335b6f456583c8ec59407438768642d4299d867d303f211 |

## Independent qualification

The base, exports, PE bounds, signed displacements and instruction bytes were recomputed from the copied DLL, independently of the prior agent. The prior RVA C4460 is corroborated; the prior base 180000000h is contradicted by this artifact. Supplied EXE IAT slot 141E03DF0 was contextual and was not independently revalidated in this Qt-only lane.

This establishes conditional static decision order. No particular runtime codec, caller order, complete byte output, successful OS persistence, error propagation, or complete converter fidelity follows without additional caller/runtime evidence. No behavior was inferred from modern Qt source. No app-control session was acquired.
