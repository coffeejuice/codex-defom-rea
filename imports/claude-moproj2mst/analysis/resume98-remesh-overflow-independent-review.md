# CP98 independent remesh decimal-overflow review

No blocker was found in the frozen source or tests. The signed decimal overflow correction follows the original kind5 converter: saturate to signed32 MIN/MAX, set range errno, then let the remesh reader clamp the method or convert the saturated integer to a boolean and continue. It removes a premature reconstruction refusal without adding floating-point or editor-palette support.

## Independent original-binary check

I independently read the imported PE and recomputed its SHA-256 as `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. I checked retained remesh reader/token-dispatch captures, decoded the kind5 table entry from PE bytes, and used bounded offline DUMPBIN views for the signed wrapper, conversion core and errno getter. The original was never executed; no REA database was opened or mutated.

All six half-open preferred-VA byte ranges in `resume98-editor300-remesh-overflow-contract.md` matched independently:

| Range | Bytes | SHA-256 |
| --- | ---: | --- |
| `140906e10..140906e14` | 4 | `b6a2dabf9aebe63e803e3179fa462f00d959570b0a54ff0b7b6dd7bc97932a96` |
| `140906cbd..140906cda` | 29 | `89df24e96ccf55e1cbc9950efdfe86bef45c2e78e9781925830d1648c2f027cf` |
| `1414737ac..1414737b7` | 11 | `063981f545db857689bf874438c79672d79e2876797be335f9c0917d9ca9fe85` |
| `141475c44..141475c74` | 48 | `629a19022f00de3f87d45e062fdc5b6ceeb4115cbe5d8280730d08526b585aa5` |
| `1414759c0..141475c42` | 642 | `f120a6e2141b2a448c558e2925a9e4277deb58e94add5d82f259ab9903dd0192` |
| `1400ab328..1400ab395` | 109 | `d8a8ddbdfb13d2be07c9db3f3a5520f86f737971cb22dfafcdd326977266a30b` |

The table entry is RVA `906cbd`. The remesh caller passes kind5 at `1400ab30d` and calls token reader `1409068f0` at `1400ab323`. Dispatch `140906cc2` calls `1414737ac` and stores returned EAX. That wrapper passes null endptr and base10 to `141475c44`; the latter passes signed flags0 to `1414759c0`. This is neither Qt integer extraction nor the adjacent unsigned conversion path.

The core tracks digit accumulation overflow, then checks positive magnitude against `7fffffff` or negative magnitude against `80000000`. At `141475bba` it calls `141474134`; `141475bbf` writes `22h`. The signed saturation selection is `141475bd0..5bdb`, followed by sign negation at `141475bec` and EAX return. Thus positive overflow returns INT_MAX and negative overflow returns INT_MIN; exact INT_MIN remains in range. The independently checked errno getter bytes `141474134..141474154` hash to `82fd11be9f0076f075cafa1b9e5c1466415a7f017d285cda92bb723d53800482`: it returns CRT thread-data+10, or fallback address `143b5e2c8`. No unconditional errno clear is present in this ordinary conversion path.

The original method caller stores the result, maps negative values to0 and values above5 to5, then jumps back to its record loop. Boolean callers compare the same shared integer against zero. The whole token was already obtained by TokenReader and endptr is null, so early candidate return after detecting overflow does not expose unconsumed digits to the next record. These findings support continuation, not merely a saturated numerical result.

## Frozen source and regression checks

| Reviewed file | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/src/engine/simcontrol300_remesh.cpp` | `620e2064ce565316cde6adbcf283d6aec5d6bbcdfcce2aaadeaa4799b572ef6a` |
| `source/apps/moproj2mst/tests/mdt/test_simcontrol300_remesh_overflow.cpp` | `0383902c09cdb3717a8a445495d15a158be4a33c9094d8bfb40372438fd62c28` |
| Root-amended `source/apps/moproj2mst/tests/mdt/test_simcontrol300_remesh.cpp` | `e138aec6ea2dd5c7e3370b1664612837e4c2201978b432e176fde0ff6e2b86ce` |

I compared the production file against its member in `build/wave3-checkpoint97-verified/source.zip`. Its sole delta replaces the overflow nullopt with `errno = ERANGE` and the sign-selected limit. The existing digit-bound check prevents accumulator overflow; no signed-min negation or out-of-range cast is introduced. Whitespace, sign, decimal prefix, no-digit zero, leading zeros, trailing text and ordinary in-range behavior remain unchanged. In-range conversion does not clear prior errno. The test explicitly checks target ERANGE equals native `0x22`.

`originalInt` still feeds the same last-int optional. Method clamping changes the state field, not that shared local; a later failed token read reuses the saturated integer. No reader loop, raw-byte spill, real parser, native file-open/rewind/close path, locale guard or public interface changed. Hex/nonfinite/range real inputs and unknown numeric locale remain refusals. The amended existing test removes only the stale integer-overflow refusal and retains those other guards.

## Test assessment and public continuation

The new file has five Testkit cases, supported release-active CHECK/CHECK_EQ/REQUIRE macros and an explicit run_all entry point. Its quoted includes resolve through the source include directory or its existing test-support relative path. Scratch writes are beneath M2M_TEST_SCRATCH; the minimal project and fallback sidecar are synthetic and no fixture corpus is changed. Root owns CMake discovery/build registration.

The integer-only memory-file case checks both boundaries, both overflows, explicit plus, leading zeros, trailing text and much larger magnitudes. It captures errno before any close-callback CHECK, proving adapter errno34 for overflow and retention of preexisting EDOM for in-range endpoints. This observes candidate conversion state at a fake close boundary; it does not test original TLS identity or physical fclose errno behavior.

The remaining memory cases require boolean true for either saturated sign, later finite scalar effects, one close, missing-integer reuse of INT_MIN rather than method-clamped0, and preservation of method5 when a later hexadecimal real still refuses. The latter correctly leaves enabled assignment pending. There is no optional success or fixture-output bypass.

The disk case calls actual `openProject` with a ConversionSession and checks openedOk, the same Session-owned RemeshState, enabled, method5/0 and later filterDistance7.5. Current `moproj/read.cpp` selects the directory and calls `ctx.session->loadProjectRemesh`; that method calls `loadRemeshAtProjectOpen` and refuses only a non-completed report. The test therefore exercises the public C++ project-open continuation rather than merely calling the parser helper. Its null editor-factory assertion also keeps the claim before editor creation.

This establishes a bounded source-qualified Open branch. It does not establish public C ABI errno visibility, complete editor Fill/palette behavior, original-process fault behavior, alternate locale/CRT TLS failures, floating overflow recovery, or full Open/Save parity.

## Review boundary

Evidence contract SHA-256: `a60f05f86b215ea82dd567218ba777c6793dd42d1b6a36307d439f562d1439da`. Offline tool: `C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/VC/Tools/MSVC/14.44.35207/bin/Hostx64/x64/dumpbin.exe`, version14.44.35228.0. Commands used `/disasm:bytes /range:<start>,<inclusive-end>` against the imported original; byte hashes above came from direct PE-section mapping. No build, test execution, source/test edit, GUI action, original execution or mutable REA action occurred. This report is the sole write. Final executable/source binding, focused tests, full launcher and frozen acceptance remain root-owned.
