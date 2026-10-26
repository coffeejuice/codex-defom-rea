# Next style24 boundary: QPaintDevice cleanup before more copy paths

The smallest responsible isolated increment is the existing-counter-zero fast
path of QPaintDevice cleanup, using the actual module-global counter owner.
It is a prerequisite of the extra temporary in the small active-null-image copy
route and is also an unrepresented tail of the already modeled ordinary image
destruction. More pixmap/conversion admission should not hide that tail behind
the current `ImageValueDestroyed` event.

Owner: `editor300`. This report is the only write. No source/build/GUI/original
execution/mutable REA changes. The 56 editor cases do not establish completion.
Token measurement unavailable; no throughput claim.

## Current state and qualifications

Reviewed `brush_texture_image.cpp` SHA-256
`8900978e4ea8b8d3a344fa71ce967f07cc58830aa824e20f7a83db7e8b7cf11b` and
`editor_base.cpp` SHA-256
`10f709a9c995eaf07095050c612e579952bdc23799a3bf5f4ff33d006ebb25ac`.
The helper retains image-data references through metadata guards, releases the
temporary ref and records image destruction before exposing opacity. Editor
uses that result at the retained `TextureImage` cursor before `OpacityDecision`.
That establishes represented image-data lifetime, not every native destructor
effect or a public mode300 constructor return.

Read `resume48-brush-style24-contract.md` (SHA-256
`16fed4dfd99f7fa8e304b6b8c69744fcf13a4c356ecc4a29117ed803f1a4d3e5`)
and its bounded asm, plus `resume51-style24-editor-hook.md`, with their original
allocation/platform/runtime-observation qualifications. Probe004 supplied only
a solid brush; it supplies no current texture/global-cleanup values.

## Comparison of the three current refusals

| Branch | Closed prefix | Remaining owner/effects |
|---|---|---|
| Pixmap-selected | Cached-wrapper lookup, two conditional getter calls, alpha then kind predicate, P2-before-P1 destruction | Actual cached QPixmap wrapper/platform data, app/thread/raster guard, virtual alpha method, ordinary versus active copy, wrapper/cache lifetime; null platform data at second isQBitmap is an original dereference, not a safe success |
| Image cache conversion | Known null cached image plus present cached pixmap invokes toImage, assigns embedded image, destroys conversion temporary before outer copy | Actual cached QPixmap owner and platform pointer/null flag; nonnull platform virtual toImage, returned image and assignment/cache mutation; extra QPaintDevice destructor tail |
| Active image copy | Outer returned image starts null, default QRect constructed, QImage::copy then assignment and inner temporary destruction | Nonnull source needs pixels/geometry/allocation; known null source has a closed no-pixel prefix, but adds another destructor/global cleanup |

`QPixmap::toImage650cf0b0` has a useful later null route: null platform data, or
its byte+14 nonzero, constructs a null QImage without the platform virtual+90
call. This can eventually close a narrow conversion case, but present-cache
boolean alone is not the actual QPixmap/platform owner required to select it.

## Small active-null prefix, and why it cannot yet complete

New bounded inspection confirms:

1. `650b9aa3` tests source painting word. Nonzero initializes outer result
   imageData=null, constructs default QRect and calls `QImage::copy650b5dc0`.
2. `650b5df7..650b5e17` tests source imageData **before reading the rectangle or
   pixels**. For null it constructs a separate null QImage and jumps directly
   to epilogue `650b66cd`. No pixel allocation/copy occurs.
3. Assignment `650b8f20` first reads the inner source painting word. Its
   QPaintDevice constructor set that word0, so it takes `650b8fb8`. Both source
   and destination imageData are null: no ref/deref/free, but the destination
   null-pointer store at `650b8ff3..8ff7` still occurs. There is no pointer-equality
   early return to invent here.
4. Copy constructor then destroys the **inner** temporary. Outer returned value
   remains null; its hasAlpha predicate would later return false. The original
   source painting word and data remain unchanged.

This could be a small helper-only prefix without new image/pixmap observations.
However, the inner destructor calls `QPaintDevice::~65138190`, and that function
tail-jumps to `65148840`. The current helper models none of that global cleanup.
Also, whether an active null image is reachable through valid original UI state
is unestablished. This prefix must not be promoted into a new real-project
acceptance claim merely because its raw-state arithmetic/control flow is closed.

## Newly identified destructor tail

`QImage::~650b3300` releases/frees image data as already modeled, then tail-jumps
to `QPaintDevice::~65138190`. That destructor installs the QPaintDevice vtable,
tests its painting word (nonzero emits a warning), and tail-jumps at `651381bf`
to `65148840`.

The cleanup body calls lazy global getter `65146e30`. Its state is separate from
QImageData references, brush metadata, QObject counts and widget counts:

| Native state | Use |
|---|---|
| global `659cb858` | pointer to one module-global QAtomicInt counter |
| global byte `659cb860` | destroyed-state branch when pointer is null |
| globals `659cb850/659cb848` | lazy callback-registration guard/storage |
| pointed-to int32 | compared with exactly0 using imported QBasicAtomicInt::operator== |

If the pointer is already nonnull, getter returns it directly. If pointer is
null and destroyed byte false, getter allocates4bytes, calls QAtomicInt(0),
CAS-installs the pointer, and registers a cleanup callback through `655e102c`.
The callback-registration routine's public CRT name was not established; do not
substitute a guessed implementation. Losing the CAS has its own deletion path.

The cleanup body returns immediately if the first getter returns null. Otherwise
it calls the getter **again**, reads the actual counter and returns when it is
exactly0. Nonzero enters additional global mutex/list work. A null or unknown
global pointer must not silently become the initialized-zero fast path: lazy
allocation/registration are real reached effects. The image-data temp-ref count
being0 proves nothing about this different module-global counter.

This qualifies an inherited omission in the ordinary helper's destruction event;
it is not evidence that the public converter currently executes an admitted
complete image constructor. The public wrapper/setup gates remain separate.

## Proposed isolated implementation boundary

Implement a shared `PaintDeviceCleanupCall` normal fast path only. Inputs are an
actual unpainted temporary's identity/profile, the same QtGui runtime/module
global pointer owner, and its current counter value. Do not accept a
`cleanupSucceeded` flag, create an editor-only counter, infer0 from image refs,
or seed a current pointer/value from the DLL's static initializer.

The owner representation must distinguish unknown global pointer, known null,
and a known nonnull counter owner. Use one runtime-local counter owner per
qualified native identity. It borrows the pointer; the two getter calls add no
native counter references and mutate neither counter nor image data on this
fast path. Actual module/runtime lifetime and alias qualification must precede
binding; diagnostic host references do not manufacture native lifetime.

The retained cursor records first getter return, second getter return and the
single reached `counter==0` read. With a known existing owner/value0 it returns;
unknown/null pointer retains a precise lazy-state guard, unknown counter value
retains the comparison guard, and every nonzero value (including negative)
retains the mutex/list continuation guard. A nonzero painting-word warning path
also remains unrecovered. Do not reread or replay getter-prefix effects on late
qualification.

Keep this isolated until reviewed. Subsequent integration must place cleanup
after image-data release but before helper `Returned`/opacity exposure. Only
after this shared prerequisite should the active-null prefix be admitted, with
separate inner and outer temporary destruction. No new pixmap, platform virtual,
pixel conversion or painting-copy behavior follows from this one helper.

Focused prerequisites: unknown/null/existing pointer distinction; existing
counter0 returns without lazy allocation/registration/mutex/list effects; late
value qualification resumes at comparison; nonzero/negative values stay guarded;
same actual owner across two getter reads and foreign/changed owner rejection;
no image/counter-ref mutation; retry and destruction do not duplicate effects;
and two actual temporary calls share the one global owner. Integration tests must
show a pending cleanup cannot expose opacity or enter the QWidget tail. These
are proposed tests, not executed checks.

## Reproducible new static evidence

Original QtGui SHA-256
`c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`;
QtCore SHA-256
`bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.
VS dumpbin tool SHA-256
`12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`.
Use `/disasm:bytes /range:START,(END-1)` against the retained original DLL.
Addresses below are preferred-base VAs; hashes cover native PE bytes, not text.

| QtGui interval, end exclusive | Qualification | Native-byte SHA-256 |
|---|---|---|
| `650b5df7..650b5e1c` | QImage copy null-source branch only | `32cdf1e001676456aa8ce147314a347bafeaa2e2e9c78332d20388687dee2e41` |
| `650b66cd..650b66e8` | selected copy epilogue | `99d6d9ea16585b096da81c9f8eeae8aadc7dbb5ad229b481c291b78fe1f90bf8` |
| `650b8f20..650b900e` | assignment normal unwind region/RET | `2cd423de4596c68fca22b5aa2934f142ec48e428a81390c465a9a12d8a73359c` |
| `65138140..65138154` | QPaintDevice constructor leaf | `b15ab774e0aff0c7501b2d7714e821ef63e5edd4b190cfd5d72764e51dfe4bfa` |
| `65138190..651381c4` | destructor through actual tail jump | `e9c60b9bad4b5bc82baf7532e5ec266e5e6a48fb6bd4f3dd7d4b18ee64552aa6` |
| `65148840..6514898e` | cleanup normal unwind region/RET | `7d5683790552c0a5baf547f49e0618d24b551b5f03920fb8cc00d770d0b03f16` |
| `65146e30..65146ee8` | lazy getter normal unwind region/RET | `e1081d23f188772b27de7ca0711c1bcf8d7ceefbf235d826a4ef6bc634f1303e` |
| `650cf0b0..650cf0f3` | QPixmap::toImage normal region/RET | `5134dd77543613db1fe2c1db902ed8ad01a7a7925039cf48c7f9cf082df39404` |

QtCore export `QRect::QRect()` is RVA b230; bounded inspection establishes its
four endpoint stores0,0,-1,-1 and RET at `6700b248`. The active-null QImage copy
does not read those endpoints. Function unwind bounds are not claims of exception
funclet coverage. No native allocation failure, callback concurrency, complete
painting registry, or new original runtime observation was established.
