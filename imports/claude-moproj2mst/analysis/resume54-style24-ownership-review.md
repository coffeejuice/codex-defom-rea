# Style24 ownership and PaintDevice cleanup review

Status: **accept the frozen isolated existing-counter-zero cleanup-tail helper**.
No concrete new counter/image alias bug or double cleanup was found. Current
image code still has the inherited missing destructor tail; this isolated helper
does not yet change image/editor admission. The exact integration regressions
below remain required before treating the image destruction boundary as complete.

Reviewer: `gradient_state`. Exclusive write: this report. No source changes,
builds, original execution, GUI control or mutable REA. `editor300` owns the
isolated cleanup helper and its tests. Token measurement unavailable.

## Current image ownership result

No double image-data release or runtime image-alias bug was found in the
reviewed restricted image path. `BrushTextureImageOpacityCall` copies its source
image once and adds one explicitly modeled temporary value reference to a
nonnull ID. Format/flag guards retain that reference. `dropTemporary()` decrements
only while `retained_` is true, clears that flag, and resets the temporary host
pointer. `resume()` on `Returned` is inert; later C++ helper destruction therefore
does not decrement the completed temporary again.

The runtime's alias-only observation envelope resolves ID in its dedicated map,
preflights the whole batch, and refines existing payload/data owners in place.
Map snapshots and diagnostic shared_ptr leases do not change the modeled native
temporary reference count. Same brush means the same embedded source payload;
distinct source payloads may share ID. Each constructor/helper frame owns its
own temporary reference. A second editor which resumes against a shared payload
can therefore hold an additional pending reference even without obtaining a
diagnostic receiver alias grant. The updated binding tests correctly distinguish
those two kinds of ownership/access.

This result assumes the admitted source-cache ownership profile: cached nonnull
ID remains owned by the source image while its copied value is pending. The
temporary count is not an absolute Qt atomic count and does not model source
image teardown or prove an image-data destructor/free occurred at count0. Cache
conversion, source-pointer replacement, active copy and public lifecycle remain
guarded/unclaimed as previously recorded.

Reviewed hashes:

| File under `source/apps/moproj2mst/` | SHA-256 |
|---|---|
| `include/m2m/engine/brush_texture_image.hpp` | `9f5db0d92b62c1986dc2caaca1f64d6268feb70ce7499ecc3096af363607f5da` |
| `src/engine/brush_texture_image.cpp` | `8900978e4ea8b8d3a344fa71ce967f07cc58830aa824e20f7a83db7e8b7cf11b` |
| `src/engine/editor_base.cpp` | `10f709a9c995eaf07095050c612e579952bdc23799a3bf5f4ff33d006ebb25ac` |
| `src/engine/view_manager.cpp` | `91c2a5b8048d1bc3f1b6ad5f0eff196be4fe5d91591f5a1054f22cd2d6e5bc4c` |

## Confirmed inherited destructor omission

Read the qualifications in `analysis/resume48-brush-style24-contract.md`,
`analysis/resume50-style24-runtime-binding.md`,
`analysis/resume51-style24-editor-hook.md`, and
`analysis/resume53-editor-style24-next.md`. The latest contract's native
destructor evidence was independently inspected with read-only dumpbin.

Original QtGui4_64.dll SHA-256 remains
`c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`.
`QImage::~650b3300` drops ID's reference, then tail-jumps to
`QPaintDevice::~65138190`. The latter compares **that temporary's** word+8,
installs its QPaintDevice vtable, warns for nonzero word, then tail-jumps at
`651381bf` to cleanup body `65148840`. The tail first calls global getter
`65146e30`; with nonnull result it calls the getter again, then compares the
pointed-to atomic int with exactly0 (`65148866..65148878`).

The existing image helper's `complete()` drops ID, emits `ImageValueDestroyed`
and publishes `Returned/opaque` without this inherited tail. The editor then
accepts that returned result at `TextureImage` and advances to `OpacityDecision`.
That is a known unrepresented reached effect in the internal image branch. The
isolated cleanup helper closes a dependency; its existence alone does not repair
this integration boundary or establish a full destructor.

The counter is the module-global owner addressed by pointer global `659cb858`.
It is different from ID's Qt ref count and from modeled temporary value refs,
QObject construction delta and QWidget counts. An image temporary count of0
does not qualify this global value as0. Unknown/null global pointers remain
lazy-state guards; neither the DLL initializer nor old passing image tests can
seed a current initialized global counter.

## Frozen cleanup helper result

`editor300` explicitly froze the helper, then I independently read the final
header, implementation, all14 tests and the scope report. Verified SHA-256:

| File under `source/apps/moproj2mst/` | SHA-256 |
|---|---|
| `include/m2m/engine/paint_device_cleanup.hpp` | `da96ec1b52b44f383935d34b8675292220644583e294921f4c2ae2915060c034` |
| `src/engine/paint_device_cleanup.cpp` | `b4beec44486bfed96d10ca399c511636a22bf564f2b46829b97c294dfc4fb49a` |
| `tests/mdt/test_paint_device_cleanup.cpp` | `287f9b731b5ab823dba2d777e0498545b1fdeb27b6ac0c844d963a54a851b690` |

The scope/qualification report `analysis/resume54-paint-device-cleanup.md` has
SHA-256 `6221b88c58d95491ddab81fdf48263afca54e7f3919d0fdb6e192109e38a336b`.

The implementation's boundary is precise: `65148840` cleanup-tail only,
existing actual global pointer, exact counter0. `PaintDeviceCleanupGlobals`
pins the module hash/RVA9cb858 and matches the receiver's object/control block
and nonempty receipt qualification. Alias/value/link checks finish before old
owners, registry, pointer or qualification mutate. Unknown, explicit-null and
positive owner links remain distinct; pointer replacement and nonzero->0 value
overwrite are rejected. No image-data object or image temporary-ref count is
accepted as an input to that counter cell.

`PaintDeviceCleanupCall` weakly binds the module owner and weakly borrows its
counter; diagnostic leases cannot make a released/expired module callable.
The code preserves two getter-return effects then one equality read. Missing
value holds `CounterComparison`, and late0 resumes without repeating getters.
Every nonzero value, including negative, commits one comparison then stays at
the nonzero continuation guard. Returning the old `Returned` receipt on repeat
resume is idempotence of that one frame, not a new destructor invocation.

The fast path adds no Qt counter/image references and never changes the counter.
Two cleanup call frames can share one counter without sharing continuation
state. The paint-device argument is unconsumed on this fast path; omitting an
otherwise unread temporary-address input is consistent with the retained native
body. The caller still owes the destructor prefix and its painting-word check.

All14 authored cases were inspected, including ordered effects, late pointer and
value qualification, null/lazy distinction, negative values, atomic rejection,
foreign control blocks, two frames, same numeric alias in separate owners, owner
release and diagnostic-lease lifetime. I ran no build/tests under this read-only
assignment. Getter-owner replacement is unreachable through valid monotonic
observations and is defensively guarded; concurrent/native dynamic pointer
transitions remain outside the declared snapshot profile.

## Required integration ordering

Split image completion into retained private predicate, image-data release,
PaintDevice prefix/tail cleanup, and final return. The private opacity value must
not appear as completed `report.opaque` or `Returned` while cleanup is pending.

1. Finish metadata predicate once, retaining its bool privately.
2. Drop the copied nonnull ID reference once, and commit that release cursor.
   A later guard must not reenter format/flag reads or decrement again.
3. Apply the returned image's PaintDevice destructor prefix, using the returned
   image's known constructor painting word0. `QPaintDevice::QPaintDevice65138140`
   writes word0 at `6513814c`; do not substitute the cached source image's
   painting word. The prefix has its own class/vtable projection; the new helper
   intentionally starts at the later cleanup body only.
4. Resume one retained cleanup-tail call for this actual temporary against the
   same runtime/module-global owner. Counter getters borrow the pointer; they
   do not add image/counter refs or destroy the source image.
5. Only after cleanup returns emit the completed image-destruction boundary and
   publish opacity; then reuse `OpacityDecision`. A later raw120 suspension must
   preserve this already completed cleanup cursor.

Two temporaries need two call frames even when they share ID and the same global
counter. The fast tail does not read its device argument, so it does not need an
invented native object-address input to decide its zero-counter branch. Future
active-null copying adds an inner temporary: inner cleanup must complete before
the outer copy constructor returns; outer cleanup follows its later predicate.
Keep that extra-copy branch guarded until both lifetimes are represented.

Helper abandonment is host cleanup, not evidence of completed native teardown.
If abandoned before ID release, host destruction drops its held modeled ref
once. If abandoned while PaintDevice cleanup is pending after ID release, it
must not drop ID again or silently claim the native tail completed.

## Exact focused regression additions at integration

Add the following to the existing helper/editor/runtime tests when the cleanup
helper is bound; these are proposed cases, not executed tests in this review:

| Proposed case | Exact assertions |
|---|---|
| `texture_cleanup_pending_after_single_image_release` | Known nonnull ID/direct-alpha format; unqualified cleanup globals. Copy/retain/format/release each1; temporary refs0; cleanup pending; report opacity absent, editor still TextureImage, no PaletteOpacityResolved/bit17/tail. Repeated resume changes none of those counts. |
| `texture_cleanup_late_counter_zero_completes_without_replay` | Starting at the prior guard, qualify the same actual existing counter/value0. Two getter returns and one equality read total, no allocation/registration/mutex/list or Qt-ref ops; prefix/release/format still1; return saved opacity only after cleanup. Repeat returned image/editor resume without another cleanup. |
| `texture_cleanup_null_image_still_runs_paintdevice_tail` | Known-null cached image, absent cached pixmap and painting word0: no ID retain/release/format, but one copied null image, one prefix/tail cleanup and one destruction. Unknown/null global must still block; ID null does not bypass cleanup. |
| `texture_cleanup_false_and_true_results_have_same_destruction_boundary` | Direct-alpha format gives false; non-alpha format/flag gives true. Both require completed cleanup before result exposure; brush alpha0/255 does not alter that ordering. |
| `texture_shared_id_separate_cleanup_frames` | Two calls on shared ID hold2 temp refs. Both release their own ID refs once into cleanup guards; completing A's cleanup never completes B's frame. Both borrow the one actual counter, whose value stays0. Use numerically equal image/counter aliases to prove typed owner maps are not conflated. |
| `texture_cleanup_owner_loss_and_conflict_do_not_repeat_release` | While tail comparison is pending, release native global owner or reject a foreign/replacement owner observation. Remain guarded with saved predicate, ID release1, no extra native read/ref/destruction claim. Diagnostic host leases do not keep native global owner valid. |
| `texture_cleanup_abandonment_before_and_after_release` | Abandon one call at missing format and another after release/pending global cleanup. ID temporary counts return to baseline once in both cases; no second release, automatic tail retry or claimed native cleanup success. |
| `texture_cleanup_raw120_retry_is_after_all_cleanup` | Successful qualified-zero cleanup, changed widget opacity, raw120 unknown/true: existing native-opacity guard follows single cleanup. Retry never copies, retains, releases or invokes two global getters again. |
| `texture_observation_only_does_not_run_cleanup` | Snapshot image/helper/editor ledgers, then refine ID or global metadata through qualified observation. No helper resume, counter getter, ID release or editor continuation occurs until explicit resume. |

For every case, keep pixmap/platform virtual, lazy-global creation, nonzero
counter cleanup, active-image copy and public factory/setup/Save guards explicit.
The future active-null case must separately assert inner-before-outer cleanup
and no pixel/rectangle reads; it is outside the isolated tail helper's admission.
