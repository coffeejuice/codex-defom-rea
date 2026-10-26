# CP92 direct variable Description on the existing owner

The smallest independent advance is direct Description assignment on CP91's actual Study-owned HeaderVariableRecord. The description field already exists with the native default-null state. Its original setter is a QString value assignment with no callback, QWidget, QFileInfo, codec, parameter parser or extra record owner. Unique-name multiple-variable import would additionally require proving all existing name lookup/last-variable, selected state and repeated helper decisions; it is a separate larger cut.

This report is read-only preparation while CP91 validation/archive is pending. No production source, build, CTest, GUI or mutable database changed for this investigation. The separately assigned CP91 stale notifier test correction has its own receipt; it is not part of the CP92 proposal.

## Exact original order and identities

Original GUI PE SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The complete0x14090ee20 body is qualified by `resume50-doe-variable-body-pe-receipt.json`,15345 bytes/code SHA-256 `270b855af32d5c5f3fca905489895021d41d376f5e24f05d8941daf161e25f08`. This lane reread the retained instruction stream `resume50-doe-variable-body-pe-compact.asm`, SHA-256 `1816C5454A6D81C4BADD706592CA854D7FABB548D9B6DC2AEB38B286EC6C5D02`.

The row sequence remains CP91's constructor/pointer append, name, selected index, invalidation, then varId/dimension and LinkTo. Only after those effects,0x14090f425 obtains the variable's first child. For each child node:

1. Convert that immediate child to an element at0x14090f455. A null conversion skips its payload tests; this does not recursively search for Description beneath an unknown wrapper.
2. Compare that element's exact tag with literal0x1427bb568 `Description` at0x14090f4b2. Case variants do not match.
3. On exact match, call QDomElement::text at0x14090f4e9, then QString::isEmpty at0x14090f4f8.
4. An empty string jumps directly to temporary cleanup0x14090f549. It does not overwrite the variable description. Thus an initially empty Description preserves constructor-null; a later empty Description preserves an earlier nonempty assignment.
5. A nonempty string is copied as the setter argument, then0x14090f543 calls0x14010f200 on the selected record. That setter assigns QString at record+8 and destroys its by-value argument. It does not trim, normalize case, convert numeric values, remove CR or replace LF with CRLF.
6. The same child then reaches the independently tested Keyword and Geometry discriminators. An actual Description element cannot equal either exact tag. Following siblings are visited in order. Normal loop completion later reaches variable reindex; deferred LinkTo remains empty for the bounded profile.

The bounded complete setter capture `resume91-doe-variable-owner-14010f200.asm`, SHA-256 `B0C942CA2234B0BABAC2AE2D7F5C028A6F4F4358C97C262A5B4F28D2DF2AA536`, covers0x14010f200..0x14010f23b through RET. It is bound to the same original by `resume91-doe-variable-owner-static.json`, SHA-256 `E0296EC39577B9AD55C376D8090DCBFA0AE084D0944A39069EC07DE77CA539B3`. No new capture or identity inference is required.

Repeated direct nonempty descriptions therefore overwrite in encounter order; the last nonempty text wins. Space-only text is nonempty and must be assigned. This is separate from the scalar nominal-data setter0x140921980, which performs newline replacement. Do not reuse that setter for Description.

## Existing source seam and admission boundary

`HeaderVariableRecord::description` is already default-null and concretely owned by Study; the borrowed Model.vars identity, correct concrete deletion and noncyclic lifetime remain unchanged. `appendBoundedHeaderVariable` currently refuses every child node. Replace only this child admission with the following bounded rule: raw non-element children may be skipped, and every direct element child must be exact Description. Keep Keyword, Geometry, unknown/case-variant element payloads outside this smallest profile. The latter are a reconstruction bound even though the original skips unrecognized tags; this cut need not widen unrelated child admission.

Perform the structural admission check before allocation, preserving CP91's existing refusal for any mixed unqualified profile. For example Description followed by direct Keyword still hits the existing pre-append matched-row guard. This is **not** a claim that the native reader refuses before Description: the native program would first commit its preceding mutations and then enter the Keyword branch. That mixed row remains unimplemented, so do not present its current reconstruction guard as native failure ordering or silently parse it as completed Description-only data.

For the admitted profile, collect/assign Description text only at the actual child encounter **after** record append and headers. Do not precompute a final description string or move all assignment before append. Use existing xml::Node::text, which concatenates descendant Text/CData, recursively visits element content and ignores comments/PI (`src/xml/dom.cpp:487..517`; unchanged inspected SHA-256 `ACCBBA68CBC0C1E49BB67DF405A1DEE0F74FFDCDBC76DE47A56FEEDDEC6167C8`). Nested tags inside an admitted Description contribute text; they are not separately dispatched as variable Keyword/Geometry payloads. Empty text must skip the setter so default-null is preserved.

Same fresh mode0, canonical positive headers, exact dimension3, empty LinkTo, single-record/fresh-owner bounds remain. No new owner, observer/token, selected-state rule, reset or load/success flag is needed. Description text/String allocations may fail after append; preserve the reached concrete record and earlier description assignments through unwinding, without claiming original Qt allocation/ref timing. No artificial fault API or fake native traversal is justified.

## Exclusive source and focused tests after CP91 freeze

Proposed write set:

- `src/doe/variable_record.cpp`: narrow child admission and exact ordered nonempty Description assignments after ID/dimension headers.
- `include/m2m/doe_variable_record.hpp`: update the helper profile comment only; keep signature/type/layout unchanged.
- New `tests/doe/test_variable_description.cpp`: focused source-value/owner/order coverage.
- `tests/doe/test_variable_record.cpp`: remove only the now-obsolete Description-only entry from its unqualified-row vector. Keep direct Keyword/Geometry/link/mode/header/duplicate/preexisting cases.

No doe_study.hpp/cpp, shared parser/Session/writer/CMake, C ABI, notification or QFileInfo edit is required. Expected target `m2m_doe_variable_description`, CTest `m2m.doe.variable_description`; root owns builds.

Focused checks should cover default-null preserved by empty/comment-only Description; nonempty/space-only value and Unicode/entity/CDATA/descendant text under the existing XML value seam; repeated nonempty/empty encounter order with LF retained; direct-only recognition, exact-case admission and mixed Keyword/Geometry pre-append guard; description retained before a second matched-row refusal with index-1; description/index0 retained before a later top-level guard; and actual Session-owned lazy read/sample continuation. Same-owner Save must still refuse nonempty-vars reconciliation **before XML open**, keeping old DOM, same record and no notifier/refresh/output claim. The corrected CP91 notifier fixture contains a direct Keyword and remains guarded; no further change there is required.

This proposal closes a real field mutation on the actual public-reader owner. It does not enable scalar/geometry/link parsing, unique-name multiple rows, native temporary/ref allocation equivalence, public output geometry, Save reconciliation or actual refresh delivery.
