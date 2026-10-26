# Isolated QPaintDevice cleanup existing-counter-zero helper

Source is frozen for root review. The helper models only `65148840`'s normal
existing-global-counter==0 branch. It does not bind editor/runtime/image code,
finish the QPaintDevice destructor prefix, or remove any public guard.

Owner: `editor300`. Only the three new source/test files below and this report
were written. No CMake, build, GUI, original execution or mutable REA changes.
Token measurement unavailable. Focused tests are authored, not executed here.

## Native boundary

Read `analysis/resume53-editor-style24-next.md` and its pinned original
QtGui/QtCore/tool identities and native-byte hashes. The controlling intervals
are QPaintDevice destructor `65138190..651381c4`, cleanup body
`65148840..6514898e`, and lazy getter `65146e30..65146ee8`. Normal RET/tail-jump
paths are qualified there; exception/concurrency and broader cleanup are not.

The selected branch performs one getter call at `65148858`, requires a returned
nonnull actual global counter, calls the getter again at `65148866`, then invokes
QBasicAtomicInt::operator==(0). Exactly0 returns at `65148986` before mutex/list
work. The paint-device argument is not dereferenced on this branch, so the new
helper does not invent an otherwise unread temporary-object metadata input.
Destructor vtable/painting-word/warning effects remain separate caller work.

## API and owner model

- `PaintDeviceCleanupGlobals` is a shared, explicitly qualified module/global
  owner. An observation must identify that exact object/control block, its stable
  nonempty receipt qualification, original QtGui SHA-256 and pointer RVA9cb858.
  Future runtime integration must intern one such actual module owner; creating
  a host object is not proof of a current native global pointer.
- The envelope uses positive counter aliases and optional signed32 cell values.
  Its optional current alias distinguishes unknown pointer, explicit0/null and
  a known positive interned owner. Cell values are actual QAtomicInt values,
  never image/Qt reference counts or DLL-initializer defaults.
- All aliases, values, pointer links and qualifications are checked before an
  existing owner or registry changes. Unknown-to-known/equal refinement is
  permitted; conflicting values, pointer replacement, duplicate/zero aliases,
  foreign/rewrapped receivers and unavailable positive aliases are rejected.
  A known-null snapshot cannot be changed to an initialized pointer through an
  observation, bypassing unrecovered native lazy allocation/registration.
- `PaintDeviceCleanupCall` weakly binds the module owner and borrows the one
  actual counter. Diagnostic host leases are not native counter references and
  cannot keep an expired/released module callable.

The retained cursor emits only `FirstGetterReturned`, `SecondGetterReturned`,
then `CounterValueRead`. Missing pointer blocks before a getter return is
claimed. Known null retains `LazyGlobalUnrecovered`: the destroyed-state/lazy
creation branches are not falsely collapsed into a null-return success.
Missing value suspends after the two getter returns, and late value qualification
resumes only the comparison. Every nonzero value, including negative values,
retains `NonzeroCleanupUnrecovered` after one comparison. No modeled native
counter mutation, ref/deref, allocation, registration, warning, lock or list
effect is introduced on the admitted branch. Host observation/ledger memory is
ordinary reconstruction storage, not a native allocation-effect assertion.

`Returned` is an idempotent historical receipt for that one call frame. It does
not replay the cleanup or certify a later destructor invocation. Pending calls
stop if the module owner dies. No dynamic global-state transitions, warning
behavior, null/lazy paths, nonzero mutex/list traversal, allocation failure,
exception unwind or concurrent callbacks are admitted by this snapshot helper.

## Focused cases and verification

Fourteen new in-memory cases cover:

1. Missing/unqualified globals and unknown pointer without fabricated getter effects.
2. Known-null pointer and prevention of an observation-based lazy-init bypass.
3. Exact0 success with ordered getter/getter/comparison and unchanged source value.
4. Unknown value retained after both getters; late qualification without replay.
5. Late unknown-pointer qualification without first treating it as null.
6. Positive/negative/int32-limit nonzero guards and no value overwrite.
7. Explicit module/RVA/qualification requirements.
8. Foreign/rewrapped/expired receiver rejection.
9. Whole-envelope rejection of late invalid links with no earlier data refinement.
10. Duplicate/zero aliases and pointer replacement preserving old state.
11. Two independent call frames sharing one actual global counter.
12. Same numeric alias in distinct runtime owners remaining distinct.
13. Released module stopping a pending comparison without mutation.
14. Diagnostic counter leases failing to extend actual module lifetime.

Direct whitespace/conflict-marker and unique-test-name checks passed. Root owns
compilation, focused/full execution and acceptance. Passing these isolated tests
would not establish the full image/paint-device destructor or converter parity.

## Frozen source

Paths are under `source/apps/moproj2mst/`:

| File | SHA-256 |
|---|---|
| `include/m2m/engine/paint_device_cleanup.hpp` | `da96ec1b52b44f383935d34b8675292220644583e294921f4c2ae2915060c034` |
| `src/engine/paint_device_cleanup.cpp` | `b4beec44486bfed96d10ca399c511636a22bf564f2b46829b97c294dfc4fb49a` |
| `tests/mdt/test_paint_device_cleanup.cpp` | `287f9b731b5ab823dba2d777e0498545b1fdeb27b6ac0c844d963a54a851b690` |

Next dependency: review/build this isolated boundary, then separately bind the
actual module globals and integrate a retained cleanup cursor after image-data
release and before image helper `Returned`. Pending cleanup must withhold opacity
and QWidget-tail progression; abandoned call frames must not release image data
twice. This task intentionally changes none of those existing paths.
