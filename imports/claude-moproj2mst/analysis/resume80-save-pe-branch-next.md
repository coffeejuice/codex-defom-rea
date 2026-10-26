# CP80: omit a project Data blob when its disk file cannot open for reading

The smallest exact missing branch was the project side-file read-open gate. Original helper 0x1400a1890 returns a null QDomElement if QFile(ReadOnly).open fails. The candidate previously treated an existing but unreadable file as readable empty bytes and generated a Data element. The authorized source increment now checks actual ifstream open success while preserving readable-empty Data, virtual-source precedence and all later Save ordering.

Only `source/apps/moproj2mst/src/moproj/write.cpp`, new `source/apps/moproj2mst/tests/moproj/test_project_blob_open.cpp`, and this report were edited. The read-only investigation became an exclusive source assignment after root confirmed the CP79 freeze. No shared header, core coordinator, C ABI, archive flag, factory, CMake or shared test changed. No build, test execution, GUI, original runtime, Qt DLL load or mutable REA action occurred.

All paths are relative to `C:/Users/alext/Projects/codex-deform-rea/imports/claude-moproj2mst`.

## Ordering recovered before choosing the branch

`analysis/resume78-project-save-pe-recheck.md` retains the 721 instruction records / 3,993 bytes with zero original-PE mismatches. Those records establish native project output open before DOM/list building, serialization followed by QFile close/error, and caller project → property dispatch → MST → remesh ordering. The caller tests project/MST booleans only after both writers have returned and returns its prior check/status local. Adapter M2M_E_WRITE is separate from that native return.

The side-file blob is read while the project DOM/list builders run, before the final project stream. Its failed input-open branch omits the Data child and continues building; it is not a failed project-output Boolean and must not abort later Save stages. The patch therefore returns false only from the internal read helper, letting existing blobElement return null. It adds no thrown error, transaction or rollback.

Archive support is a separate boundary: `write_ops.cpp` still refuses withBlobs SimCtrlKey/InterObjectKey, and `write_items.cpp` still refuses inline item archive data. Ordinary native Save passes withBlobs=false. Those broader guards were not removed to repair this small read branch. The current operation writer also uses blobElement for object-property data independently of its withBlobs branch; the exact original helper behavior is established below without claiming a newly verified complete archive caller graph.

## Additional original helper check

The earlier 721 records did not include 0x1400a1890. To avoid attributing an unseen helper branch to that receipt, this task additionally disassembled exactly 0x1400a1890 through 0x1400a1b15 with existing Microsoft DUMPBIN 14.44.35228.0 and checked every emitted byte directly against the original PE section mapping. Result: 121 instruction records, 646 bytes, zero mismatches. Raw region SHA-256: `37c79eb06a8b9a82b47b1186689845132672585942d29dae9661556e1d666449`.

The executable rehash is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Retained decompile `evidence/gui_mo/s1/moproj/w1.c` hashes to `ba5f7624f55477c54338e3998f83e518a18c3797e72f14d3c42fd32ffffe4d18`. Independently decoded PE imports identify these decisive sites:

| Site | Instruction bytes | Established action |
|---|---|---|
| 0x1400a18c7 | ff 15 f3 ac d6 01 | default QDomElement constructor |
| 0x1400a18e2 | ff 15 e0 26 d6 01 | QString::isEmpty |
| 0x1400a18ed | 0f 85 d8 01 00 00 | empty path skips to null-element return setup at 0x1400a1acb |
| 0x1400a1911 | ba 01 00 00 00 | ReadOnly mode argument 1 |
| 0x1400a1928 | ff 15 b2 26 d6 01 | QFile::open |
| 0x1400a1933 | 0f 84 87 01 00 00 | failed open skips Data creation to QFile destruction at 0x1400a1ac0 |
| 0x1400a1943 | ff 15 a7 26 d6 01 | successful-open QIODevice::readAll |
| 0x1400a194f | ff 15 c3 26 d6 01 | QFile::close after reading |
| 0x1400a1965 | e8 96 a7 00 00 | compression helper 0x1400ac100; then hex/Data construction |
| 0x1400a1ac5 | ff 15 55 25 d6 01 | QFile destructor on both open outcomes |
| 0x1400a1ad8 | ff 15 ba ab d6 01 | copy resulting QDomElement to return storage |

Reproduction command: existing `C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/VC/Tools/MSVC/14.44.35207/bin/Hostx64/x64/dumpbin.exe /disasm:bytes /range:0x1400a1890,0x1400a1b15 evidence/original-binaries/DEF_GUI_MO_64.exe`. Output was inspected in memory; no extra disassembly artifact was written. Import names and branch targets were checked against the PE rather than only the imported decompile. This is bounded static evidence, not injected original failure or full archive recovery.

## Minimal source correction

The private `readAll` in write.cpp now returns bool and accepts an output string reference. It returns false immediately if `std::ifstream::is_open()` is false; otherwise it collects the same iterator bytes and returns true. The disk fallback of readProjectFile returns that bool after its existing existence check. Earlier FileEffects deletes/writes, overlay lookup and their ordering are unchanged.

The gate tests opening, not nonempty content or stream.good after reading. A successfully opened empty file must still create Data; the original helper also does not introduce a post-read QFile error gate before compression. No new after-read failure policy was inferred. This does not exhaust subtle native QFile versus C++ stream partial-read behavior.

A diff against the archived CP79 writer has exactly the two intended hunks: read helper signature/open gate and its sole caller's bool propagation. The serializer body is text-identical. Baseline CP79 source identity: `22828eda881712488bad653fdcf0a2e05b2cebdfe32934974865b13922f1e14c`.

## Dedicated test contract

Existing CMake discovery should create target `m2m_moproj_project_blob_open`, CTest `m2m.moproj.project_blob_open`, with three Release-active Testkit cases:

1. `project_blob_missing_and_readable_empty_have_distinct_dom_results`: absent path gives no node; readable empty file gives Data text exactly 00000000; a readable binary payload with NUL/CRLF round-trips through the existing compression path.
2. `project_blob_existing_but_read_denied_omits_data_until_unlocked`: an exclusive shared-denial handle leaves qt::fileExists true, while a real ifstream probe must fail to open. Both readProjectFile and blobElement must reject that disk input. Releasing the handle restores the original readable payload. Failed fault setup is a REQUIRE failure, with no skip/diagnostic bypass.
3. `project_blob_virtual_effects_keep_precedence_over_unreadable_disk`: while the physical file remains unreadable, an empty overlay still produces readable-empty Data; an earlier virtual write supplies its bytes; a later deletion suppresses the node. This protects existing virtual filesystem semantics from a misplaced disk gate.

The test constructs only Project storage and synthetic side.KEY files beneath configured M2M_TEST_SCRATCH. It copies no original fixture, launches no app, and makes no whole-Save or native-runtime parity claim. Native handles use RAII cleanup. It uses the existing private write_internal.hpp declaration, so no shared interface edit is required.

## Static preflight and release hashes

| Owned source | Lines | SHA-256 |
|---|---:|---|
| source/apps/moproj2mst/src/moproj/write.cpp | 633 | 8af4be80324c39e1f8f452d7723bebd200e4b7372180d8e32c1e6ee32c2399a2 |
| source/apps/moproj2mst/tests/moproj/test_project_blob_open.cpp | 120 | 8b8c8772ea7a3e1de6df76730883ba9b5db8f97ec4580e141c8253520ca22d06 |

Every quoted include resolves relative to its source file or existing target include directory. The new file has three TEST_CASE entries, one Testkit main, supported REQUIRE/CHECK/CHECK_EQ, and no raw C assert, REQUIRE_EQ or skip. The only readAll caller was checked and updated. The unchanged write_ops.cpp hash is `ed789023fb419781e8402caad11e631ae30c6c912e9307c6723c999b27de9d4d`; unchanged write_items.cpp hash is `e06bbb6d428741e57da01a1227cbd56c947a68ce274431497eac830b5cd7b942`.

The two owned source files are released unchanged for root integration. Root should configure to discover the new target, group it with existing project-operation/project-writer and Save-I/O regressions, then retain the exact full source-stable suite and immutable freeze. Static preflight is not a compilation or passing test receipt. Full archive mode, original error residue and fresh single-project Open/Save/Close acceptance remain separately qualified. Tokens are unavailable.
