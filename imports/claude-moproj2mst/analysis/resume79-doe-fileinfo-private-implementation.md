# Checkpoint 79: QFileInfo private ref0 prefix

Implemented only the approved admitted0xe0 allocation/ref0 prefix after the isolated QIcon request. The concrete private payload initializes its first32-bit word to0 and leaves the rest unavailable. Its next boundary is QStringPathOwnership, before path copying, shared-null references, file-engine handlers, QFileInfo publication or icon plugin/engine work.

Only three new source/test files and this report changed. No CP78 source, shared Session/parser/writer, C ABI, CMake, original app, GUI or mutable REA was modified. No build, executable test or CTest ran. Root owns grouped validation and source archive.

## API and partial value

New `include/m2m/doe_fileinfo_private_prefix.hpp` exposes:

```cpp
class DoeFileInfoPrivateArena;
const DoeFileInfoPrivatePrefix& appendDoeFileInfoPrivatePrefix(
    DoeFileInfoPrivateArena& arena, const DoeIconShellPrefix& icon);
```

The caller-owned arena retains metadata and concrete allocations. `DoeFileInfoPrivateValue` is aligned8, size0xe0 and standard-layout, with the reference as its first member. Its user-provided noexcept constructor writes reference0 only. The remaining0xdc bytes have no initializer or public field/read interface; copy operations are deleted to avoid accidental copying/interpretation of that tail. The helper uses default initialization through its concrete constructor rather than value-initializing/zero-filling the whole payload.

Read-only prefix accessors expose only the concrete value's reference word, an initialized-prefix length of4, adapter stage and optional NextBoundary::QStringPathOwnership. Metadata does not masquerade as native fields in the private payload. No QString/QByteArray, shared-null owner, cache indices, engine pointer or QFileInfo wrapper is constructed. No field represents successful private-constructor/addFile return or outer wrapper/ref1 publication.

The source is the selected original allocation `0x670b9a1f` and initial store `0x670b8052`. The exact next unexecuted native call is `0x670b8059`, QDir::fromNativeSeparators/QString path ownership. Source qualification and all binary/capture hashes remain in `analysis/resume79-doe-qicon-addfile-next.md`.

## Admission, order and guard

Before publishing any new arena metadata or allocating a private payload, the helper checks the earlier isolated icon is at AddFileBoundary, has a nonnull shell whose private-data pointer remains null, default size(-1,-1), and the exact fixed DOE resource/mode0/state1 request. Empty resource or already-populated QIcon takes other native addFile branches and refuses this scope without an invented QFileInfo object. These checks are reconstruction admission, not source-independent proof of public receiver reachability.

It then publishes metadata before payload allocation, allocates the concrete0xe0 value/ref0 and returns ReferenceInitialized. If metadata growth fails, no payload existed. A host allocation exception leaves an incomplete AllocationPending diagnostic entry, not an ordinary native false/null success result.

The normal guard text is:

```text
DOE QFileInfo private prefix: QString path/shared-null ownership remains unsupported
```

The input icon/request is borrowed only while admitting the call; no back reference or String copy is retained. Once created, the private diagnostic value can be observed independently of the icon arena's lifetime. No production Session owns this isolated arena yet, and no receiver/public caller invokes it.

## Cleanup qualification

This helper returns an **unreturned native constructor prefix**, not a live completed QFileInfo. Its arena preserves adapter diagnostic storage across a later adapter guard. That must not be confused with original C++ exception cleanup: the original outer constructor's FuncInfo/funclet frees its raw0xe0 allocation if the private constructor unwinds. This implementation does not replay that native unwind or infer that actual native allocation remains alive after it.

Ordinary arena destruction destroys concrete adapter storage only. It does not call QFileInfo/QString/engine destructors, decrement fabricated shared-null counts, alter the enclosing QIcon.d, deliver notifications or claim native Close/leak cleanup. The enclosing CP78 shell and this separate partial value remain distinct owners.

## Focused tests and static preflight

`tests/doe/test_fileinfo_private_prefix.cpp` has six Testkit cases:

- Exact0xe0 size/alignment8, ref0, only4 known bytes and QStringPathOwnership boundary; no uninitialized tail byte is read.
- Original QIcon shell/request storage stays unchanged; ref is0 rather than the outer constructor's later1.
- Two isolated admissions own separate stable allocations/records. This is not a claim that a native interrupted call retried.
- Caller-thrown guard preserves diagnostic arena state, explicitly without native construction-unwind claims.
- Declared diagnostic alterations to the prior icon's d pointer or resource reject other addFile branches before a private allocation.
- Adapter owner has no icon/runtime ownership back edge and can expire independently. No native destructor/engine is invoked.

Test-only const_casts deliberately alter objects whose underlying arena-owned values are mutable, to exercise excluded source branches; they do not simulate a real native engine or resource result. Production observers remain const.

These are authored tests, not executed results. Static checks verified Testkit main, six case declarations, supported active macros, no C assert/REQUIRE_EQ, all quoted includes against actual/local target roots and no trailing whitespace. `git diff --check` on assigned paths reported no diagnostics; the direct scan covers these new files as well. Manual source inspection confirms no QString/shared-null allocation/ref, wrapper/engine publication or receiver call.

Existing CMake glob discovery registers target `m2m_doe_fileinfo_private_prefix`, CTest `m2m.doe.fileinfo_private_prefix`. Root should include the CP78 icon shell target as a dependency regression in the grouped preflight. No CMake edit was needed.

## Frozen files

Paths are beneath `source/apps/moproj2mst/`.

| New file | SHA-256 |
| --- | --- |
| include/m2m/doe_fileinfo_private_prefix.hpp | 1FAF67C98CDE0623EF18D1961275839013C78115FDCE385CAD3220BCA243A5DC |
| src/doe/fileinfo_private_prefix.cpp | 8A2A14DF159EF4F3CD95F2074F3B4C1A08686FE7F0D42AB483A3B64F41B21954 |
| tests/doe/test_fileinfo_private_prefix.cpp | 7E7B4988C28E2B83E38874BB319869088B00AD653D2C9DAE0A1C4F5DF4E37BF7 |

All three source files are held unchanged after this receipt for root's build/test/archive. Controlling report `analysis/resume79-doe-qicon-addfile-next.md` hashes to `A49C0E11BA72D3DA09B9F1070EBFB3C00F2AC517A1D92CA350388CBBC1F5B243`.

The next dependency is actual QString input/shared-null ownership and its cleanup, before file-engine handler selection. Icon factory/engine/graphic transfer, SetOprHasDoe delivery, DoeTableRefresh and public Type2 Save remain guarded. This slice makes no runtime native parity or full-feature completion claim.
