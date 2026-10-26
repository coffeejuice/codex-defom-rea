# CP73 null-companion return and arrays-timer prefix

The source-closed next loader branch is the actual step's **null companion DB**:
native loader returns before directory/version/A mutations, then the reader
records its actual arrays-timer checkpoint. This bounded branch is now implemented
and stops before object assignment. Nonnull companion loading, directory-version
mutation and A allocation/zero-length shortcuts remain queued behind their
owner/storage qualifications. No public cache-miss success is enabled.

Root expanded this audit lane to exclusive viewer.cpp/test_viewer_box.cpp
implementation. Only those two source/test files and this report changed. No
shared model/helper/CMake edit, build, CTest, GUI or mutable REA action was
performed by this worker. Source was released frozen to root for validation;
token measurement is unavailable.

## Native qualification and source reuse

Original GUI target SHA-256:
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
CP71/70 retain exact loader/reader/outer/timer bounds, native-byte and dumpbin
hashes. Relevant complete intervals are loader140828060..1408283d5, query
140825440..140825532, allocator1409c83b0..1409c8408 and checkpoint
140832370..14083242f (ends exclusive). No new sidecar was written here.

Native loader reads actual S+3390 and takes its null return before the directory
query. Reader then calls checkpoint at14082d8d6 on global143DCDAE0 with
"Assign the variable to the object data", sub=null. CP71's constructor/name/flag
and layout qualification maps that actual global to source g_timer_read_arrays.
Checkpoint appends only when signed count<100; nonnegative count>=100 is a normal
no-op. Negative count can index before its arrays and remains an adapter guard,
without resetting/repairing the actual counter.

Additional existing helpers were located rather than reimplemented:

- `defdbio::dbread::step_dir_sizes` in w2_objarrays.cpp matches the native query's
  four local outputs, actual S.u740 indexing, ignored explicit step argument,
  low-byte success and conditional model-version mutation. Public declaration
  is in defdbio/dbread/w2.hpp.
- `defmodel::bc::calloc_doubles` in boundary_topology.cpp has the same zero-count
  special case and signed-count-to-calloc(element size8) shape. Native allocation
  diagnostic literals exactly match source ERROR/DOUBLE/format strings. Full
  failure-callback control flow is not newly qualified merely by matching text.
- The broader source `read_single_a_array_from_file` still uses the unconditional
  DEF_PRE counterpart names_up_to_date gate. Do not invoke that whole helper for
  GUI high versions: it would omit the reached native allocation/read body.

Reviewed source SHA-256: w2_objarrays.cpp
`c567ec0c3391507f438bfe66f5121aa25dab577f40592d5a84164d4133152359`;
w2.hpp `ac5ada504545294321a4a8d0446093d50d19778205f9c54ab72297fcc9f787a7`;
boundary_topology.cpp
`40aa96113753fe286c30f2d7b156be6b78596323970b69c85f65272a75862fa4`.
These are source reuse candidates, not authorization to cross missing owners.

## Implemented bounded behavior

After existing qualified COW/table selection and the actual node-reader timer's
two entry checkpoints, a nonnull S.db still returns HighVersionDbLoaderPending.
The source does not dereference it, query records or call a loader. For null S.db,
the native loader's normal no-op return is closed, so the adapter checks the
actual arrays timer count. Negative returns HighVersionPostLoaderTimerPending;
otherwise it calls the existing actual global checkpoint and returns
HighVersionObjectAssignmentPending. The normal100-entry cap/no-refill behavior
is the existing timer method, not an adapter-created count/default.

Both states keep the complete geometry contribution unknown and selected scene
unsupported, preserving COW/table/timer effects. No directory output, model
version/cache/header/body write, object enumeration/copy, timer stop/report or
rollback follows. Fresh public calls repeat bounded native entry effects while
preserving the arrays timer's prior history; that timer is not reset per mesh.
Cached normal returns and all unknown-owner/preallocation/registry guards remain.

## Nonnull directory owner and record extent remain prerequisites

Directory query uses a40-byte DbDirRecord at companion.uad8 + S.u740*0x28.
It copies hsize+8, mar_size+c, a_size+10 and version+20 into distinct caller
locals, then writes S.step_version when different. Source typed DbDirRecord and
step_dir_sizes match. The query does not populate S.u1b68/u1b6c; native checks
those existing model MAR/A header lengths afterward. Nonzero directory sizes
cannot stand in for the model's header state.

Actual companion identity must come from a live real owner, such as the actual
DbHandle.reader with its owning Project/subproject lease or an equivalent proven
borrowed-lifetime contract, and must equal existing S.db. GuiModel.db is a separate
borrowed adapter handle. Do not dereference an opaque raw companion or create a
new shared owner around it to manufacture provenance.

The directory's allocated backing extent also needs a current full-load invariant
or receipt. `load_db_info` writes uac8=nrsets before directory read/allocation
success, and a later read error can return while retaining an old uad8 array.
Thus pointer!=null plus logical uac8 alone does not prove capacity. Source
db_info_read.cpp SHA-256 is
`a40750756c612f6ae71febf6f25a63e8b40e8b26e88e5d196cb728851e99e3b2`.
A completed full-open ownership/extent proof can use the existing helper; the
current viewer lane exposes no such record extent/lifetime envelope. Keep the
nonnull guard until that proof is assigned. Header-only/partial states must not
be mistaken for a current directory allocation.

The directory version mutation precedes the loader's GUI gate recheck. A record
version below6000 may cause that recheck to return while preserving the changed
version. Missing model headers may likewise return after version mutation and
native error output. No rollback is justified. A future resumed prefix must
retain its query locals and phase rather than replaying a changed gate as if the
earlier reader completed; fresh invocations must be distinguished from resume.

## A allocation and zero-stored-length shortcut remain queued

After directory query/re-gate, valid id and nonzero existing MAR/A header lengths,
native rechecks cache. For cache0 and null A pointer it calls allocator with
wrapped32(S.u2b24+2), then immediately publishes the returned pointer at S+1b78.
Only afterward it checks S.u2b28[id]. Zero writes cache1 and returns, without
stored-data reading or rollback of the A publication. Preexisting nonnull A is
preserved without reallocating/clearing it on that shortcut.

Native allocator tests n==0, not n<=0. Zero returns null; a later zero-stored-
length shortcut can still commit cache1 with no A body. Nonzero counts are signed-
extended into allocation size; null allocation calls its diagnostic before any
normal return. Overflow/negative sizes and failure callback behavior must not
be repaired or generalized. Source plain signed +2 needs a bounded nonoverflow
profile or explicit recovered wrapped arithmetic before future integration.

Nonzero stored length enters companion file-name/Fortran conversion and actual
DB-read body140b38d57; on normal return cache1 is stored without testing a result.
That body and subsequent model-object array-copy destinations remain unclosed.
Even loader null/old-version/zero-length returns are followed by the reader's
arrays checkpoint and model-object assignment loop. They are not whole-reader
success or permission to substitute optional M for main object arrays.

## Focused tests and release

The viewer test now has32 unique Testkit cases (one new, CP72 expectations updated).
The new null-companion case covers actual arrays-timer counts-1,0,99,100,101 and
INT32_MAX with populated/null connectivity (12 combinations), including repeated
fresh calls. It observes COW before table/node-timer changes; null loader return
then actual append/cap behavior; preserved prior labels/times/subs/name/flag/pad;
negative counter untouched; and no public result. Subsequent call preserves
earlier appended timestamps/labels rather than resetting arrays history.

CP72's null/opaque companion case now expects arrays checkpoint effects only for
null, while opaque remains byte-for-byte guarded. Other timers and actual model
version/cache/header lengths/body count/stored length/object count/body pointers
and mesh arrays/owners are checked unchanged. Test-only snapshots restore actual
module history after fixtures; production retains the committed partial effects.
Clock samples stay opaque real runtime inputs, without fabricated fixed values.

Static preflight passed:32 unique names, existing Testkit entry, supported
Release-active CHECK/REQUIRE, no C assert/REQUIRE_EQ, resolvable unchanged headers,
no trailing whitespace and clean `git diff --check`. No compile/test execution
is claimed by this worker. Root target `m2m_mdt_viewer_box`, CTest
`m2m.mdt.viewer_box`.

Future directory tests need current actual owner/extent, ignored argument versus
S.u740, output order and version changes before later exits. Allocation tests
need existing A preservation, small successful/null-zero publications before
cache1, failure-diagnostic ordering and no invented rollback; they must remain
separate from unqualified object copies. No such larger source work was done.

## Frozen source SHA-256

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/engine/viewer.cpp` | `d7d6d07aa5df2f82d4e2c719005f7c63d87a392c1bf0f088e8f6fe5894defa87` |
| `source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp` | `b55b39d02c4be895fca99b733044c2428390ee1b454879ff852d31738b5ccab7` |
