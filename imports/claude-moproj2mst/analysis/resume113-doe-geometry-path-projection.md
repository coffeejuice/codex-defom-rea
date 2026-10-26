# CP113 Geometry path-value projection contract

A narrow **isolated path-value** projection is source-ready for an explicitly named absolute Windows drive/UNC InputTable filename, conditional on the normal native construction path. It can return the parent directory and exact upper/lower/nominal attribute strings destined for the owned Variable's Geometry row. It cannot certify that QFileInfo construction/handlers/native references completed, append a complete geometry record from paths alone, or release the public pre-QFileInfo guard.

Root is validating CP112 and explicitly held all source writes until its freeze/next CP113 assignment. This lane therefore completed the contract/evidence only. No source/test/shared header/CMake/plan, build/CTest, GUI/native process or mutable REA operation was used. New bounded disassembly receipts came solely from the already-retained immutable QtCore DLL. The proposed new helper/test files remain unwritten.

## Coordination and corrected owner chain

`resume113-geometry-owner-and-path.md` from graph_audit reconciles the actual owner: SfMoDoeManager's +0x178 count/+0x1a0 at-index select the **last Var**, stored at reader rsp+0xa8; it is that Variable, not the manager, that owns Geometry rows at+0x28. CP91/93 independently bind the manager vtable/constructor and Variable creation/deletion. The corrected geometry-row destruction chain releases row members through the Var's geometry clear before deleting the Variable. No manager-owned side list or new Study ownership cycle is needed.

Path values alone do not replace that record's full contract. Native append0x1408175c0 additionally consumes GeoOperation/GeoObject and compares upper/lower strings plus those IDs for an existing row. It may return without appending. The reader subsequently obtains count-1 for flag updates, and separately sets Variable morphology. These owner/key/ordering effects must be represented if a later source lane binds actual rows; they must not be guessed by always pushing a legacy GeoFile or copying a retained B122 fixture.

## QFileInfo::path value selector is now qualified

QtCore DLL SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. New `resume113-doe-fileinfo-path-static.json` SHA-256 `908ECEBA433483EAF862CFEC293FAA279CAFD138549EF213BEBAE6EAA55C5C2C` captures the complete normal getter0x670b7a70..b7ab9:

- It loads the actual private pointer and tests private+0xb4 bit0.
- Bit set takes a separate default-construction result branch; do not conflate it with QFileInfo(QString).
- Bit clear passes private+8 directly to file-entry parent getter0x670ef4b0. The getter itself does **not** ask an engine for its PathName or use an engine-name cache.

Retained private constructor0x670b8030 calls file-engine resolution first, stores its engine at+0x50, constructs caches, then at0x670b80e9 explicitly clears+0xb4.bit0 before returning. Therefore a normally returned QFileInfo(QString) takes the file-entry parent route. This corrects the earlier provisional concern that the path getter itself needed an engine-name/cache decision. It does not omit the earlier constructor/handler effects or prove that this actual occurrence returned.

New `resume113-doe-fileentry-parent-static.json` SHA-256 `A07D32F5EE42BF6203B065818B15EBD00F9F0196A960021560650201FC9B3974` captures parent getter0x670ef4b0..ef568 and the first split separator fragment. `resume113-doe-fileentry-separator-static.json` SHA-256 `8A9118F2FFB488E6FEEB234920B9B3F32063041704BEB8C5861D7C48FD64BA38` completes fragments0x670ef1f0..ef25e. The separator scan uses a signed16-bit cached position, scans backward for '/', and may detach QString storage before inspecting characters; detachment does not change the path **value** on a normal return. Native COW/ref/allocation/failure behavior is not projected by a host string helper.

The parent rule is the same value logic already present in `qt4/path.cpp::Entry::path`: no slash -> drive prefix if character1 is ':', otherwise '.'; slash0 -> '/'; slash2 with character1 ':' -> retain drive-root slash; otherwise left substring before the final slash. The existing `qt::path` is a lexical value helper and is reusable only under a source-qualified file-entry input profile, not as proof that a native QFileInfo exists.

## Constructor resolver can rewrite the entry: bound the filename profile

Retained0x670f0080 copies the entry, invokes resolver0x670efbc0 and, on a true result, copies the possibly changed filename/native-path/cache fields back to the actual entry. It is therefore **incorrect** to infer lexical parent values for every original QString merely from the final getter.

The relevant already-retained resolver branches close a useful smaller domain:

- Custom-handler return at0x670efc05..efc71 does not rewrite its copied entry when the no-existence-check flag is false, as passed by the QFileInfo constructor.
- With no custom engine, a leading slash at0x670efc93..efc97 jumps to the native branch0x670f000c.
- A colon at index1 takes0x670efd51..efd54 to that same native branch.
- With existence checking false,0x670f0014 jumps to0x670f0056 and returns without rewriting the entry.
- Other colon prefixes enter search-path resolution and can replace the filename. They are outside the first projection.

The isolated helper should therefore admit only normalized absolute ASCII drive-letter paths (`C:/...`, after native-separator replacement) and UNC paths (`//server/share/...`), with a nonempty named-source value and length below32768 UTF-16 code units so no signed16 separator overflow needs an invented rule. These are reconstruction bounds for an isolated value projection, not original input validation. Retain exact character case, repeated separators, dots/`..`, and filename text; do not clean/canonicalize or resolve against the current directory. Other names, search prefixes and generic in-memory bytes callers remain outside this projection.

Even in the admitted value domain, the custom-handler traversal, actual data/ref/shared-null owners, native allocation/engine lifetime and constructor normal return remain unimplemented process effects. The helper's result is the **conditional normal-return value**, not an admission receipt for the public reader.

## Exact geometry attribute joins

Original GUI SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; complete reader code SHA-256 `270b855af32d5c5f3fca905489895021d41d376f5e24f05d8941daf161e25f08` retained in CP50. UpperGeoFile is copied and concatenated at0x14091181d..11897; lower follows at0x1409118e7 onward; nominal follows0x140911a2b..11b1f. In all three the source uses a literal **'/'**, not QDir separator or an absolute-path join:

`projected = parent + "/" + attribute`

Consequences to preserve in focused tests: missing/empty attributes produce parent+'/' rather than an empty path; a rooted or drive-qualified right-hand attribute is still appended, not substituted; backslashes in that attribute remain raw; `../` and repeated slashes are not normalized. Only the input filename's native-separator conversion feeds the parent calculation. Attribute ordering is upper, lower, nominal in the native locals; named return fields should prevent swapping them.

The values map to the actual row roles as follows: nominal local rsp+0x230 -> third argument register r9 at0x140911f7a; upper local rsp+0x250 -> outgoing stack+0x20 at0x140911f75; lower local rsp+0x248 -> outgoing stack+0x28 at0x140911f68. These are passed to0x1408175c0 on the reader's last Variable. The later flag/correspondence/payload phases are separate and remain unconsumed by a path-only projection.

## Proposed new files and deliberately limited API

After root's CP112 freeze and explicit CP113 source assignment, the smallest independent implementation could own NEW files only:

- `src/doe/geometry_path_projection.hpp`
- `src/doe/geometry_path_projection.cpp`
- `tests/doe/test_geometry_path_projection.cpp`

Proposed value result contains the supplied `InputTableSource` filename, projected parent and upper/lower/nominal path Strings. A function accepts an actual named-source context plus exact Geometry node, validates the narrow filename/node profile, and returns those values using the already-qualified lexical path rule and literal joins. It performs no I/O, no native QFileInfo/ref/engine operation, no numeric/flag/correspondence parse and no Study/Var mutation. It is not called from the public parser, so the current Geometry guard and three-state distinction (source-less bytes, named value, native owner unavailable) remain intact.

An actual owned-row append should **not** be hidden inside this first helper. The current legacy `Var::files` GeoFile projection has path/upper/lower/flag fields but does not represent the operation/object key or the0x38 row's typed correspondence lists. Mutating it from only path values would conceal the remaining row/duplicate/flag interface rather than close it. A later source assignment must choose a coherent concrete row owner/alias representation and bind the already-recovered selected Variable to it; that is a separate integration cut, not a reason to fabricate those fields now.

Focused tests for the proposed value helper should cover absolute drive/UNC parents, drive-root edge, repeated/dot components unchanged, exact slash joining with relative/absolute/backslash/empty/missing attributes, upper/lower/nominal distinction, source-less/relative/search-prefix/overlength refusal and unchanged public Geometry refusal/no file effects. Tests must call the helper an isolated conditional value projection; no fixture string, profile enum or constructor-return Boolean can establish native owner completion. Existing B122 pure metadata/binder tests do not supply that evidence.

## Next integration dependency

The path **decision** is closed for the bounded conditional normal-return domain; actual native constructor/data/ref/handler occurrence remains guarded. Public Geometry still requires that occurrence-bound owner and a complete row key/append/flag/correspondence representation before consuming these values. A later native GEO payload consumer remains unbound by the reader captures and must not be replaced by a copied fixture. Root's CP112 build proceeds independently; no source changed in this evidence lane.
