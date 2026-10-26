# CP78: independent offline PE recheck of project Save failure ordering

The retained original bytes confirm the narrow CP77 contract: the project writer closes QFile, then queries QFile.error; the enclosing Save continues through property writers and MST before testing the project Boolean. Its final return is the prior check/status local. Candidate M2M_E_WRITE remains an adapter mapping, and the seeded physical-device test remains candidate-only evidence.

Only this report was written. This check read PE files, retained DUMPBIN captures and receipts; it did not launch the original, load Qt, invoke REA, build, run a test or acquire GUI control. The reverse-engineer-anything skill was read; the explicitly assigned offline/no-mutable-REA boundary governed execution. Token measurements are unavailable.

All paths below are relative to `C:/Users/alext/Projects/codex-deform-rea/imports/claude-moproj2mst`.

## Independent byte and symbol binding

Python standard-library code read each DOS/PE header, required PE32+, read ImageBase and section virtual/raw mappings, and mapped each retained disassembly instruction record (including wrapped continuation bytes) to the current PE file. Every recorded byte matched. It independently decoded executable import descriptors/thunks and Qt export name/function tables; no symbol label was accepted solely from the retained prose. Direct CALL rel32 and indirect CALL RIP-relative targets were calculated from their actual bytes.

| Original file | Current SHA-256 | Preferred base |
|---|---|---|
| evidence/original-binaries/DEF_GUI_MO_64.exe | fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc | 0x140000000 |
| evidence/original-binaries/QtCore4_64.dll | bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160 | 0x67000000 |

| Retained capture | SHA-256 | Instruction records / bytes | Mismatches |
|---|---|---:|---:|
| analysis/wave3-project-close-status-1400983c0.asm | 8887d0a3a169f999298163282d28331008d2633629790a48bc307bbff36a1f83 | 541 / 3,194 | 0 |
| analysis/wave3-boolean-save-io-1400980d0.asm | 0e5a4d4bc4873a90aa00768679f5778a68aba5147809b5402db0c717b3e1c1ce | 101 / 476 | 0 |
| analysis/wave3-project-close-status-670b4840.asm | 76641e684c0fbde58c5a855f654832b48b24741741682dfbc71652490309c0d8 | 28 / 112 | 0 |
| analysis/wave3-boolean-save-io-670b5f30.asm | ad5fc40e2dc1172c5d02b24731a868ad9443f585200a14db6fe2dcc7ac269935 | 51 / 211 | 0 |

Total: 721 instruction records and 3,993 bytes. This is an independent PE-byte/target recheck of retained decoding, not a new decompilation, exhaustive control-flow recovery or runtime observation.

## Project writer: actual calls and post-close Boolean

The following RIP-relative call bytes independently resolve through executable imports:

| Call site | Raw instruction bytes | IAT slot | Imported target |
|---|---|---|---|
| 0x140098d8f | ff 15 13 ba d6 01 | 0x141e047a8 | QtCore4_64!QTextStream::setCodec |
| 0x140098dac | ff 15 5e 38 d7 01 | 0x141e0c610 | QtXml4_64!QDomDocument::toString(int) |
| 0x140098dbd | ff 15 2d b0 d6 01 | 0x141e03df0 | QtCore4_64!QTextStream::operator<<(QString const&) |
| 0x140098dc8 | ff 15 4a b2 d6 01 | 0x141e04018 | QtCore4_64!QFile::close |
| 0x140098dd3 | ff 15 d7 b9 d6 01 | 0x141e047b0 | QtCore4_64!QFile::error |

Qt's actual export table maps QFile::close to 0x670b5f30, QFile::error to 0x670b4840, QFile::flush to 0x670b5e00 and QIODevice::close to 0x670ba470. The two QFile-object loads immediately before close/error both use `[rsp+68h]`. The error result is tested by `85 c0` at 0x140098dd9 and zero branches to 0x140098fce, which stores a true result. The nonzero path later stores zero at 0x140098f6a before returning its failure local. This decision is driven by the post-close QFile error, not QTextStream status.

The getter at 0x670b4840 reads the private pointer at object+8 and returns the dword at private+0x120. In QFile::close, the call at 0x670b5f60 invokes real exported QFile::flush; its Boolean is saved into edi at 0x670b5f65 before QIODevice::close at 0x670b5f6b. After the engine-close virtual call at 0x670b5f8d, the native-success branch tests saved dil at 0x670b5f94. Only a true saved initial flush reaches unsetError at 0x670b5f9c (target 0x670b4850), whose store at 0x670b4861 clears private+0x120. A false saved flush bypasses clearing. These bytes support the retained saved-initial-flush distinction; concrete original Windows fault entry and file residue remain unobserved.

## Enclosing Save: continuation and final status are separate

Direct CALL displacement decoding gives:

| Site | Bytes | Calculated target |
|---|---|---|
| 0x1400981e1 | e8 da ac fb ff | 0x140052ec0, current-operation Save |
| 0x1400981f0 | e8 cb 01 00 00 | 0x1400983c0, project writer |
| 0x140098201 | e8 da 00 00 00 | 0x1400982e0, property dispatch |
| 0x14009820e | e8 bd 9e 00 00 | 0x1400a20d0, MST/subsequent study writer |

After the project call, `88 44 24 20` at 0x1400981f5 saves AL to `[rsp+20h]`. The intervening instructions before property and MST calls are argument loads; there is no branch on that saved Boolean. The MST AL is tested after its return at 0x140098216, and only afterward is the saved project Boolean tested at 0x14009821f. The combined result is stored via the local at `[rsp+78h]` and then `[rsp+20h]`; remesh-tail calls follow.

The final return load is `8b 44 24 24` at 0x1400982a0: EAX receives `[rsp+24h]`, the earlier check/status local, rather than either Boolean local. Consequently the native function's return must not be equated with adapter M2M_E_WRITE or with a combined writer-success Boolean. This establishes the bounded caller order on the inspected normal-return route; earlier validation, exceptions, unsupported study routes or later GUI wrappers require their own evidence.

## Comparison with CP77 physical acceptance report

Inspected report: `analysis/resume77-physical-original-acceptance.md`, SHA-256 `60e6cd7dac8c636582ba9921619ff14f58460ad1acf083b6d433cb718a72d862`. Its source-qualified ordering and qualification statements agree with the rechecked bytes. No correction to those claims is required.

The two supporting candidate logs were independently reopened:

| Case / receipt | Run folder | Actual log SHA-256 | Observed summary |
|---|---|---|---|
| CP75 project_output_physical_lock; analysis/resume75-integrated-grouped4-tests.json | build/test-runs/1792887286649966300 | e6597aa5e59273abc4561f2e1ac86498305267250290df2f7d47982178e32656 | seeded_buffer_lock case; 0 of 1 cases failed |
| CP76 s.physical_postclose; analysis/resume76-integrated-grouped-tests.json | build/test-runs/1792887553418918400 | 82e237a0ee6bf43518126bdc07b8ed5846d49e19acb5ae46c7843d7578c7a0ca | seeded_real_project_postclose case; 0 of 1 cases failed |

The corresponding grouped receipt hashes are `71e759486e83878d840f5c1ab43199fb031b292f3e3da49e4e919dad9d5c7927` and `670055b8aa86a928e024ebdf3aaf80ed7f0613d5d8943260ab8221e192b3474d`. Neither log contains the lock-setup diagnostic that would bypass its physical-error assertions. The tests have a diagnostic early-return branch if lock acquisition fails, so a pass alone would be insufficient; the retained no-diagnostic logs are part of the actual qualification.

CP77 correctly says the candidate test starts with an artificial one-byte real FileDevice buffer, then applies an actual byte-zero lock. The measured real error 2 and later Boolean/MST/remesh continuation are useful candidate coverage through private shared core services. They are not ordinary convert/C ABI execution, do not prove the original reaches the seeded state, and do not observe original partial bytes, GUI messages/actions, timer delivery or subsequent Close state. The log does not export a separate residue hash or read Stream::writeFailed; these limits also remain accurately recorded.

## Smallest remaining acceptance edge

This recheck completes CP77's proposed offline original-byte confirmation. More candidate status injection or another callback cannot supply the remaining original-runtime fact. The next original acceptance edge is a separately authorized and gated disposable-project capture of an actually reachable unmodified original failure, post-close QFile error, later writer effects and native Save/Close status. Until app-control release/status evidence permits that work and the failure condition is qualified, retain the static native order plus candidate-only seeded-error coverage. No completion claim follows from this report.
