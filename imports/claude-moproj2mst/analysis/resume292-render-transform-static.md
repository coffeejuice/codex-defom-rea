# CP292 original 2D render displacement contract

Read-only recovery establishes the ordinary finite arithmetic of `1417d0850`, `1417d0b90`, and their geometry callees. It does not establish deterministic behavior on two paths that consume indeterminate memory. Public rendering remains HOLD pending width/color/render ownership.

## Qualification and scope

Target: immutable `imports/claude-moproj2mst/evidence/original-binaries/DEF_GUI_MO_64.exe`, retained qualified original v11.0.2, independently verified SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Windows FileVersion/ProductVersion metadata is absent; v11.0.2 is the retained target qualification, not a newly observed resource version. CP287 `imports/claude-moproj2mst/analysis/resume287-palette-render-readiness.md` supplies prior caller qualification. Requested bodies were absent from the searched retained C exports; all new instruction-level claims below use bounded immutable PE disassembly with DUMPBIN 14.44, not an unqualified pseudocode export. No original execution, GUI, Frida, mutable REA database, source change, build, or test occurred. This report is the sole write.

Addresses below are absolute VAs. Allocator `141473368` calls `141486264`, which uses HeapAlloc flag 8: its successful buffers are zero-filled. Caller scratch allocator `14146c25c` forwards through `14146eabc` to `14147489c`, which uses HeapAlloc flag 0. Neither geometry wrapper checks its internal allocation results.

## Wrapper 1417d0850(N,xy,width,out,relative)

Admission is evaluated in this order: signed N<=0; ordered `1e-12 >= abs(width)`; null xy; null out. Rejection returns -1 before allocations or output stores. The comparison is COMISD/JAE: unordered width passes; do not replace it with `!(abs(width)>1e-12)`.

Allocate in order: 2*N doubles retained XY, 2*N doubles transformed XY, N ints mapping. Doubling occurs in signed-width 32-bit machine arithmetic before sign extension. No allocation failure guard or local cleanup/rollback is present. The finite implementation domain must exclude arithmetic overflow and allocation failure rather than inventing their native outcome.

Compute D=bboxDiagonal(xy), threshold=D*0.0001. Start retained count K=0. For each i, retain xy[i] when i==0 or ordered distance(xy[i],retained[K-1])>threshold; set map[i]=K-1 afterward. Equality and unordered distance comparisons coalesce. Unconditionally overwrite retained[K-1] with the original final XY. Call `1417d0b90(K,retained,width,transformed,relative)` and save its return. Unconditionally expand every out[i]=transformed[map[i]], even on -1. Free retained, transformed, mapping in that order; return saved status. A subordinate rejection expands zero-filled transformed data, unlike top-level rejection that never writes caller output.

`1417d1640` computes the diagonal of the XY component bounding box: initialize minima/maxima from first XY; update only on ordered strict comparisons; return sqrt((maxX-minX)^2+(maxY-minY)^2). `140a27500` computes Euclidean point distance, skipping sqrt only for exact zero squared distance.

## Core 1417d0b90

Repeat the same admission. Compute D=bboxDiagonal. Closed is ordered distance(first,last)<D*1e-5. When relative is nonzero, set width=width*D after admission, with no revalidation. Allocate N records of 24 bytes for normals, then N doubles for scales, both zero-filled and unchecked. Set M=N-1 when closed, otherwise N. Call `1080(normals,M,xy,closed)`, then `17b0(xy,M,normals,scales,width,closed)`; no result check.

For closed input copy normals[0].XY and scales[0] into final slot. Write all N output components as `input + ((width*scale[i])*normal[i].component)` in that operation order. Then invoke `23b0(&xy[i],&xy[i+1],&out[i+1],&out[i],N,xy)` for i=0..N-2, sequentially. Free normals then scales. If closed, calculate first and last displacement distances; if first>last, copy last output XY to first; otherwise copy first to last. Return 0. Only admission returns -1; there is no later reported helper-failure status.

## Initial normals 1417d1080

Use cyclic prev/next indices even for open input. Let u=normalize(P[i]-P[prev]), v=normalize(P[next]-P[i]). Open first chooses rotateLeft(v), open last chooses rotateLeft(u), all other points choose rotateLeft(u+v). Normalize chosen and store XY in 24-byte records. Only XY is written.

Normalization `140a276e0` calculates length=sqrt(x*x+y*y) and divides XY only when ordered abs(length)>1e-12; otherwise writes positive zero XY. Thus NaN normalizes to zero. Vector temporaries copy an unused indeterminate third qword, which does not enter these XY calculations.

## Smoothing/scales 1417d17b0

Allocate an unchecked zero-filled count*24 temporary. Every iteration reads OLD normals; copy back only after all iterations. Open first/last copy the full old normal record and set scale=1. Other points use the following source-ready XY operations; cross(a,b)=a.x*b.y-b.x*a.y, and arithmetic order is significant:

```cpp
e1=P[i]-P[prev]; e2=P[next]-P[i];
l1=length(e1); l2=length(e2);
a=width/l1; b=width/l2;
a=(10.0*a)*a; b=(10.0*b)*b;
if (cross(nprev,n)<0.0) a=0.0;
if (cross(n,nnext)<0.0) b=0.0;
candidate=(n+a*normalize(n+nprev))+b*normalize(n+nnext);
u=normalize(e1); v=normalize(e2);
c=-u+0.2*v; d=v-0.2*u;
x=cross(candidate,c); y=cross(d,candidate);
if (x<0.0 && y<0.0) candidate=x>y ? c : d;
else if (x<0.0) candidate=c;
else if (y<0.0) candidate=d;
candidate=normalize(candidate);
updated[i].xy=candidate;
s1=abs(cross(e1,candidate))/l1;
s2=abs(cross(e2,candidate))/l2;
minimum=s2>s1 ? s1 : s2;
denominator=0.2>minimum ? 0.2 : minimum;
scale[i]=1.0/denominator;
```

Copy all updated 24-byte records back, then free temporary. Interior third qwords stay zero; endpoints copy old third qwords. Preserve ordered comparison and ternary operand bias; generic min/max substitutions require separate NaN qualification.

## Segment correction 1417d23b0

Arguments are `(a,b,displacedB,displacedA,count,original)`. Snapshot quad `{a,b,displacedB,displacedA}`. `1417d2800` tests each cyclic corner cross `(q[i]-q[prev]) cross (q[next]-q[i])`; first strictly negative returns false, zero/unordered continue.

If false, intersect lines a->displacedA and b->displacedB in infinite-line mode into an UNINITIALIZED stack candidate. Ignore returned success. First replace displacedB when candidate is strictly between b/displacedB, then replace displacedA when strictly between a/displacedA. Predicate `1417d2730` selects Y iff ordered abs(a.x-b.x)<1e-12, else X; tests `(candidateAxis-aAxis)*(candidateAxis-bAxis)<0` with no collinearity test. Endpoints are excluded.

For j=0..count-2, intersect a->displacedA with original[j]->original[j+1] in segment mode. On success replace displacedA only if BOTH abs(candidate.x-a.x)>1e-6 AND abs(candidate.y-a.y)>1e-6. Then perform the equivalent b/displacedB operation. Every next intersection sees prior writes. No allocation, rollback, or failure guard.

Intersection `140a288b0(A,B,C,D,P,mode)` returns 0 without writing for a null argument, both exact vertical lines, or exact equal nonvertical slopes. Mode 0 first rejects disjoint axis boxes. First vertical writes Px=A.x and Py=((Cy-Dy)/(Cx-Dx))*(Px-Dx)+Dy. Second vertical writes Px=D.x and Py=((Ay-By)/(Ax-Bx))*(Px-Bx)+By. Otherwise m1=(Ay-By)/(Ax-Bx), m2=(Dy-Cy)/(Dx-Cx), Px=(((Cy-By)+m1*Bx)-m2*Cx)/(m1-m2), Py=m1*(Px-Bx)+By. Mode 1 returns 1 after writing. Mode 0 checks containment in both segments by choosing X when abs(dx)>=abs(dy), otherwise Y, and rejecting positive interval product. It can return 0 AFTER writing P. No epsilon/parallel tolerance.

Ignored mode-1 failure in the nonconvex branch consumes uninitialized candidate. Initializing it or skipping its uses is a behavioral change, not recovered native parity.

## Actual caller and implementation boundary

`14126a190` admits signed count!=0, converts float secondary+0x48 to double, allocates two raw scratch arrays through the nonzeroing allocator, fills original XY, calls 0850 at `14126a319`, and ignores EAX. It then writes stride4 and N-1 segments, constructs indices, resizes the separate render vector, and packs original/displaced XY into float slots. Displaced reads occur at `14126a589`; color callback `14126a5cb` precedes scratch frees. There is no local rollback. Top-level 0850 rejection therefore leaves caller displaced scratch indeterminate and consumed. Neither zero-fill, fallback duplicate vertices, status early-return, nor moved cleanup is equivalent.

A minimal deterministic subset exists for explicitly supplied distinct finite TWO-point input, positive width>1e-12, edge length>1e-12, representable intermediate arithmetic and successful allocation. Both normals are normalized left normals and both scales are one. Relative=true displaces by width*edgeLength; positive width gives a convex correction quad and original-segment intersections at endpoints fail the displacement predicates. Negative width can reach ignored parallel-intersection failure and must not inherit this subset's deterministic result.

Current `imports/claude-moproj2mst/source/apps/moproj2mst/src/engine/viewer2d.hpp::CurveItemState` owns only vertex rows/count; its separate index/render/color arrays are explicitly unresolved. `viewer2d.cpp::bind` and `viewer.cpp::bind2D` supply curve and tolerance, not owned widthFloat or color/render state. Do not derive width from tolerance. A parameterized internal transform core is source-ready; public render success remains HOLD. Prospective owned implementation files: viewer2d.hpp/.cpp; tests/mdt/test_viewer_box.cpp. Tests should exercise supplied deterministic widths, grouping equality/forced-last behavior, ordered threshold semantics and explicitly classified indeterminate branches; do not claim a new adapter guard is the native caller's early return.

Remaining evidence: production width/color/render ownership and actual occurrence values; heap/stack bytes on indeterminate paths; allocator failure occurrence; CRT exceptional floating behavior and environment. Ordinary finite geometry formulas in the audited chain have no unresolved geometry callees. No runtime equivalence is claimed.

## Immutable mapped-byte receipts

Ranges are half-open absolute VAs, hashed from PE-section-mapped original bytes.

| Range | SHA-256 |
|---|---|
| 1417d0850..1417d0b81 | fe9be504ea1fdeaed0e831797dace46b9bac838308ab6a3fe57a88c8c1912ddd |
| 1417d0b90..1417d106f | 051b192460f60f355473c0a890ea146349d6ca0a65ee1f8ef7767f177cf6a64c |
| 1417d1080..1417d162e | 5cf304f6cf62d84c1ded431aa0caf645b42029611d696ba82dae89dea0fdb092 |
| 1417d17b0..1417d23a4 | 3660be5fce5658142907d03a0efdc675f8301df360c7d54d10c8ac7986a481e8 |
| 1417d23b0..1417d2724 | 0555af500a3d825e64b56f8d738c92c8a8ac8d36ddb736ef509df56b3b99e6a1 |
| 1417d2730..1417d27ed | 681831e96615d08349534ca5fa66f102c57c6e63fbe7999b526818b8234f585d |
| 1417d2800..1417d294b | 6339e2357dac9f344c2ab6195a0fa764ce2a8454f21a3ac96b9a28af735df80f |
| 140a288b0..140a29039 | 95d856d2fa167ca2e6405d7c41e75bbaca8f37488aed0315af80328b33df2237 |
| 140a276e0..140a2779c | a0ccf312028d85daac8eaa0673acf68cf2c13ddc926426c45a89fe249974ab16 |

Mapped constants: 142248190=1e-12; 1421ce788=0.0001; 1422395f8=1e-5; 1421249e0=1; 142142078=10; 1421d3cf8=0.2; 1421d3d30=sign-bit mask.
