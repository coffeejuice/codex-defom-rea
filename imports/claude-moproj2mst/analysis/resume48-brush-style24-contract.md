# Texture brush opacity and shared cache ownership

Style 24 opacity can be decided from texture representation metadata only after
the native texture getter's copy and cache effects are represented. The pixmap
and image paths have different owners and dependencies. Keep the style24 editor
guard for candidate48; this report does not enable public mode300.

Owner: `gradient_state`. Exclusive outputs are this report and the uniquely named
`analysis/resume48-brush-style24-*.asm` captures below. No source edits, builds,
original application execution, GUI control, or mutable REA. This follows the
reverse-engineer-anything skill's shipped-binary evidence requirements. Token
measurement unavailable; no performance claim.

## Evidence identity and bounds

Original: `evidence/original-binaries/QtGui4_64.dll`, SHA-256
`c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`.
Disassembler: Visual Studio dumpbin
`C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/VC/Tools/MSVC/14.44.35207/bin/Hostx64/x64/dumpbin.exe`,
SHA-256 `12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`.
Commands use `/disasm:bytes /range:0xSTART,0x(END-1)` against the absolute DLL
path. Each retained filename is `resume48-brush-style24-START.asm`.

The table records independently computed **native code byte** hashes, not stdout
hashes. U means PE unwind region; L means explicit bounded leaf candidate; C
means a separately captured continuation. U alone is not a complete logical
function: the two marked continuations were necessary to reach their actual RETs.
The L captures contain adjacent code beyond the relevant RET; only the indicated
function instructions are claimed. Exception/allocation-failure paths were not
validated.

| Start | End exclusive | Bound | Native byte SHA-256 |
|---|---|---|---|
| `6511a560` | `6511a60c` | U | `878d25fb17342603981aa39f2a4067dc621f0fd7a2b80d6166691ef8d19f8c56` |
| `6511a610` | `6511a68d` | U | `f5704cb3df184a1e9874b7e005bf62cf4021e9b0d86c55447bc4a8a77c5798cd` |
| `6511a1a0` | `6511a20a` | U | `cd444b3f6d0ce65a4591a7824621db2c19a6d06460794227fa808544122a40ec` |
| `650cec30` | `650cec55` | U | `769e0c200f317e86e53a8a0166570e4a3c2fcf9e6cedf3d92ebcb442c8970ff5` |
| `650ce980` | `650ce9e0` | L | `f738d2020b15672e0c33be82acceae4c3b50d5898045ddeae99f6edc7a8599f8` |
| `650cf590` | `650cf709` | U | `ea51646518db59aea75847336494e4c87b9da0366263114f4d02d406505676ef` |
| `650b9a70` | `650b9b4a` | U | `5ded55d6db93d288f41fbb9c4e819b62fcb25f335e43662f5e98d0c28d30be07` |
| `650ce920` | `650ce978` | U | `f43bcb564074983c72c9a30652ab9fe1693a44c3f721dea20cf4c9c7ece08e21` |
| `650b3300` | `650b3364` | U | `5784a5e783c8f86a7dd0967e92f75e3a590ba85c95d1f147a83f9d3a14d55e64` |
| `650aa340` | `650aa3a0` | L | `84fb087f125885e74a594cb4f69cbee39ec0f08976db41d953c98c7ca785d74f` |
| `650ce0a0` | `650ce0b8` | U prefix | `f18bfab311117b009236f6f4f312711a54f762ef3fcdb561bbdb1b24b22bc61a` |
| `650ce0b8` | `650ce116` | C | `960aa0d30ca046ad1fa783635fe6dd56dae50ba7ab31c66a23cb5889b1fcbfc5` |
| `650cdf00` | `650cdf23` | U prefix | `aa083ea7618a705738347f64e63ca4e486607c88fcfb782537314126d67159ae` |
| `650cdf23` | `650cdf8b` | C | `32f3476eae003aa58e687d60c6e9fad9183a30a0470f930a4cd65bad9490ff57` |
| `650a8f90` | `650a8fd0` | L | `80907a8ed26804f064379d4541fab8104b3692517ecd2d90de820414d1801d4f` |

Read the existing `analysis/wave3-editor-parentless-opacity-static.json`, its
`wave3-editor-parentless-opacity-6511c090.asm`, the alpha leaf qualification in
`wave3-editor-probe004-palette-binding-contract.json`, and the bounded
style-not-16 exit in `resume46-editor300.json`. Those establish the surrounding
order. Probe004 observed a solid brush, not a texture; its values cannot seed
texture metadata.

Export names were resolved from the retained export map, and the application
helper imports were independently resolved by PE IAT address. In particular,
`650cec30` exports both `QPixmap::hasAlpha()` and `hasAlphaChannel()`;
`650ce980` is `QPixmap::isQBitmap()`, **not** a second alpha predicate.

## Reached branch and native state

Let B be QBrush data, P a QPixmap wrapper, PD its shared platform data, I a
QImage wrapper, and ID its shared image data.

| Field | Qualified use |
|---|---|
| `B+4` int32 style | `6511c102..c110`, style24 dispatch |
| `B+0d` brush alpha byte | unconditional entered isOpaque prefix; alpha leaf reads QColor+5 |
| `B+90` byte | `6511c112`, selects pixmap path when nonzero |
| `B+70` pointer to cached QPixmap wrapper | texture getter `6511a587..a5c1`; image getter helper `6511a1c4` |
| `B+78` embedded QImage wrapper | `6511a5a3`, `6511a1b7`, `6511a1ef` |
| QPaintDevice wrapper `+8` word | `650cf675`, `650b9aa3`: painting-active copy branch |
| `P+10` -> PD | `650cec34`, `650ce980`, copy/destructor |
| `PD+0` vtable | has-alpha calls slot `+68` at `650cec40` |
| `PD+18` atomic ref count | retain/release helper and QPixmap destructor |
| `PD+20` int32 kind | isQBitmap leaf compares exactly 1 at `650ce984` |
| `I+10` -> ID; therefore `B+88` -> cached ID | QImage::isNull leaf `650a8f90..8f99` and image predicate |
| `ID+0` atomic ref count | ordinary QImage copy and destructor |
| `ID+30` int32 format | QImage::hasAlphaChannel leaf `650aa349..a37d` |
| `ID+58` bit mask4 | same leaf, read for every format outside5/6/8/10/12/15 |

All entered nonzero-style window brushes consume brush alpha before dispatch.
Style24 then takes `6511aba0`'s style-not-16 constant-false exit without radial
geometry reads. The prior alpha comparison is not the style24 result.

### Pixmap-selected path: B+90 nonzero

At `6511c11a..c137`, `QBrush::texture(6511a560)` returns first temporary P1,
then `QPixmap::hasAlphaChannel(650cec30)` reads its PD pointer. Null PD returns
false; nonnull PD calls its virtual method at vtable+68 and normalizes the
result. No generic pixel-alpha scan is performed here; the platform method's
implementation is an additional dependency.

If that predicate is true, opacity is false and the second texture getter is
skipped. If false, `6511c140..c15d` creates P2 with a **second** texture call and
reads `QPixmap::isQBitmap(650ce980..ce98b)`: PD kind==1 means opacity false,
otherwise true. This leaf unconditionally dereferences PD. If P2 has null PD,
the original has no local null guard; do not manufacture a successful opaque
result or a safe false result for that invalid route.

Cleanup order is P2, then P1, through `650ce920`. P2 exists only on the reached
second-call path. The outer return is `!hasAlphaChannel(P1) && !isQBitmap(P2)`
subject to getter and owner preconditions, not a single preobserved texture flag.

`QBrush::texture` first checks B+70. If null, `6511a58d..a5b9` allocates a
24-byte QPixmap wrapper, calls exported `QPixmap::fromImage(650cf410)` on B+78
with conversion flags0, and stores the new wrapper pointer into B+70 **before**
copying it to the returned temporary. A const getter can therefore mutate the
shared brush cache, visible to every alias. If the pointer already exists, it
uses that wrapper. No brush-data detach appears on this getter's reached path.

Each return copy goes through `QPixmap` copy constructor `650cf590`. It installs
QPaintDevice/QPixmap state, initializes output PD=null and calls `650cdf00`.
The latter requires QCoreApplication::instance (`IAT65696820`), otherwise calls
qFatal (`65697328`) with the retained literal at `656b5680`. When present, it
compares QObject::thread (`656976c8`) to QThread::currentThread (`65697810`).
Different threads are admitted only when the global QString at `659c9748` equals
QLatin1String("raster") (`QString::operator!=`, `65697790`); other mismatches
warn and return false. The warning path returns a null output PD; its later
isQBitmap route is therefore not made safe by this constructor.

After the helper returns true, painting-active word!=0 at source wrapper+8
takes a deep-copy route (`650ceee0` and assignment `650cefe0`) that is not fully
recovered by this report. Word0 uses `650ce0a0` plus its captured continuation:
retain source PD at PD+18, assign output PD, release prior output PD, invoking
the platform deleting destructor if its ref reaches zero. This copy does not
collapse the P1/P2 wrappers into the cached wrapper.

### Image-selected path: B+90 zero

At `6511c170..c194`, `QBrush::textureImage(6511a610)` creates one temporary I1,
calls exported `QImage::hasAlphaChannel(650aa340)`, and negates the result.
It then destroys I1 before returning. There is no isQBitmap check on this path.

The getter invokes helper `6511a1a0` on B. If B+78's image is null and B+70's
pixmap pointer exists, the helper calls `QPixmap::toImage(650cf0b0)`, assigns
that temporary into B+78 through `QImage::operator=(650b8f20)`, and releases
the intermediate image. The source cache mutation occurs before the returned
copy. If the cached image is nonnull, or both cached representations are null,
the helper simply returns the embedded B+78 wrapper. toImage/assignment details
and their platform effects remain separate unqualified dependencies here.

The returned image is copied through `650b9a70`. A source painting-active word0
shares ID and increments ID's atomic ref if nonnull (`650b9b1e..b9b37`). An
active source takes the `650b5dc0` rectangle-copy/assignment route, beyond this
report. The destructor `650b3300` drops ID's ref; zero invokes `650b2840` then
operator delete. With an ordinary copied live cached ID, source ownership keeps
that data alive when the temporary is released. Explicit modeled native-value
references must not be inferred from host shared_ptr use_count.

The complete leaf `650aa340..650aa384` returns false for null ID. For nonnull ID,
it returns true when format is one of **5,6,8,10,12,15**, or when format is one
of **1,2,3** and byte(ID+58)&4 is nonzero. All other formats return false. These
are native numeric formats; this report does not assign names from an unrelated
Qt release. It reads no image pixel buffer or color-table entries. Thus an
alpha-capable format produces nonopaque brush even when its actual pixels are
all fully opaque. Null image returns opaque after the native getter/copy path.
Read-order refinement found while implementing the helper: `650aa36a` reads
ID+58 mask4 for every format outside5/6/8/10/12/15 **before** comparing formats
1/2/3. The flag affects the result only for1/2/3, but other formats must still
consume that reached read and retain its unknown-state guard.

## Shared state and next executable increment

Extend the shared runtime-interned brush owner with a texture alternative,
separate from the gradient alternative: known/unknown B+90 selection, optional
cached pixmap-wrapper owner (distinguish unknown from known null), and embedded
image value. Wrapper values need their own painting-active word and a shared
platform/image data reference. Platform data and image data aliases are distinct
from brush aliases and from each other.

For an initial bounded internal image branch, source can support the following
subset once same-owner observations and shared state exist: known image-selection,
known nonnull cached ID (or both representations known null), source painting
word0, known ID format and only the conditional bit when read. This avoids
unrecovered conversion/painting branches while preserving copy/retain/release.
Its metadata can remain sparse; for formats5/6/8/10/12/15, bit(ID+58)&4 is never
read. No input field named opaque/hasAlphaResult should replace this decision.

Pixmap admission additionally needs the application/thread/raster state, cached
wrapper and painting word, shared platform data, recovered virtual-alpha method
identity/implementation, and conditional kind read. A captured callback result
alone does not reconstruct a platform owner or its method. Cache misses require
recovery of fromImage/toImage and assignment effects first. Keep local guards
at those dependencies rather than pretending every texture is immutable.

Do not mutate or duplicate actual shared brush owners when adding observations.
Use the existing live runtime qualification and alias checks; missing fields stay
unknown. Preserve consumed alpha and partial cache writes at exact retry cursors;
a later guard must not roll back a cache entry installed before a failed copy.
No scientific runtime observation of a texture is supplied by this static report.

## Focused test prerequisites

- Image-selection branch, both-null representations: null image copy and release
  complete, predicate false, resulting opacity true; unknown nullness stays guarded.
- Known cached image with painting word0: same ID retained then released, source
  owner unchanged; all numeric format classes above, conditional bit0/1, brush
  alpha0/255. Verify pixel data is not consumed by this metadata predicate.
- Missing cached image plus existing pixmap blocks at conversion until recovered;
  active image/pixmap source blocks at its reached deep-copy dependency.
- Pixmap has-alpha true skips the second getter; false invokes a second copy and
  kind==1 returns false, kind!=1 returns true. Release P2 before P1. Null PD at
  the isQBitmap route must not be converted into a successful result.
- Same brush aliases observe any reached B+70/B+78 cache mutation. A retry does
  not allocate/copy twice or reread the already-consumed alpha prefix.
- Application absent, different-thread nonraster, same-thread and raster exceptions
  preserve the original distinct outcomes; recover/qualify fatal semantics before
  admitting that path into the converter, rather than inventing an error return.
- Shared alias lifetime and field conflicts fail observation binding without
  partially overwriting existing owners. No public factory or mode300 admission
  follows from helper tests. Native opacity/window and wrapper gates stay separate.

Implementation successor: newly created internal `brush_texture_image.hpp/.cpp`
and `test_brush_texture_image.cpp` implement the bounded image copy/metadata
call frame and its focused cases. They are not yet bound into WidgetRuntimeState,
EditorBaseConstruction, or public mode300. Actual runtime owner interning and
qualification remain the shared integration owner's responsibility. The helper
guards pixmap selection, cache conversion and painting-active copies, and keeps
temporary native-value refs alive across metadata guards. Thirteen focused
test cases were authored; no build/test executable was run by this lane because
the orchestrator owns the source/build freeze. Style24 remains guarded in the
editor until that shared binding is integrated and validated.
