# CP92 Description assignment on the actual Variable owner

The fresh mode0 variable reader now assigns exact direct Description text on its existing concrete Study-owned record, in encounter order after record append and headers. Empty text preserves the previous description, including constructor-null. Other direct element payloads remain guarded before append, and Save retains nonempty-variable reconciliation before XML open.

Root confirmed CP91 frozen before authorizing this cut. Source qualification is `resume92-doe-variable-description-plan.md`, SHA-256 `AFC8EF42A17E78FFD5A8336D35320ABF04A6006E1CAD6EA79806A3D87263D390`. It binds the original GUI PE, CP50 complete0x14090ee20 reader, and complete0x14010f200 setter captured in CP91. No new native capture, original execution, GUI or mutable database was used. Root owns compilation/tests; no worker build/CTest was run.

## Source change and limits

`appendBoundedHeaderVariable` now permits raw non-element children and exact direct Description elements in its structural admission. Any other direct element, including Keyword/Geometry, unknown wrapper or case variant, still refuses the whole matched row before allocation. This is the existing reconstruction admission boundary extended only for Description, not an assertion that native parsing fails at that earlier point. In particular a native Description-then-Keyword row would first apply Description and enter Keyword; that mixed profile remains outside this implementation.

After the unchanged constructor/owner/pointer append/name/selected-index/invalidation/ID/dimension sequence, the helper visits each admitted Description in node order. It calls the existing Node::text value seam, skips empty strings, and otherwise assigns the existing description field. It does not precompute a final value, trim, convert numbers or normalize LF to CRLF. Repeated nonempty values overwrite; a subsequent empty value does not erase the earlier assignment. Description descendant text is collected as text, so a nested Keyword tag inside Description does not become a direct variable keyword payload.

Ownership, selected state, numeric/fresh-mode/single-record admission, link rejection and caller reindex/outer ID timing are unchanged. Text/value allocation failures after append retain the concrete record and reached earlier state. Native QString/QDom refcounts, allocation timing/faults and original Close/delete behavior are not claimed. No new owner, observer, success token, notifier, output consumer or ABI is introduced.

Changed exactly four files: helper source, header profile comment, one obsolete Description-only entry in the previous guard-test vector, and a new focused test. The helper signature and record fields are unchanged. No Study header/source, Session, activation, shared writer, convert, CMake, C ABI or original fixture changed. CP91's corrected notifier fixture still has a direct Keyword and remains guarded without notifier delivery.

## Focused tests and static preflight

New target `m2m_doe_variable_description`, CTest `m2m.doe.variable_description`, has seven cases:

- Nonempty value on the same concrete record with source headers and completed reindex.
- Empty/comment-only/nested-empty descriptions preserve default-null; raw direct nodes are skipped.
- Repeated nonempty/empty descriptions preserve encounter order and LF; explicit space CDATA remains nonempty and is assigned.
- Unicode/entity/CDATA/descendant text uses the existing XML primitive; nested Keyword text creates no keyword/operation payload.
- Direct case variants/wrappers and mixed Keyword/Geometry profiles retain the pre-append matched-row guard and unchanged invalid/selected state.
- Description survives a second-row refusal with index-1, or a later top-level refusal with completed index0.
- Actual Session-owned Type2 lazy read keeps the same Study/record and description, continues ordered LHS/FF/USR writes and current PAR read, and later Save stops before XML at reconciliation with old DOM, record and notifier state retained.

The whitespace-only test intentionally uses CDATA: existing XML parsing filters ordinary whitespace-only text nodes. This slice changes neither that primitive nor its native qualification, and does not infer input-text preservation from QString::isEmpty alone.

Static checks resolved every new quoted include against source-local/target roots; Testkit main, supported CHECK/CHECK_EQ/REQUIRE, no C assert/REQUIRE_EQ and no trailing whitespace were verified. `qt4path.hpp` explicitly supplies kSep. git diff --check reported no diagnostics, with direct file scans covering new/untracked sources. Seven tests were written, not executed by this worker. Root should run the new target with existing variable_record and notifier-binding regressions before the full source-stable launcher.

## Frozen source

Paths are under `source/apps/moproj2mst/`. Source released to root immediately after preflight and held through grouped build/tests/archive.

| File | SHA-256 |
| --- | --- |
| `src/doe/variable_record.cpp` | `09E7260153A5A5F83CBE9D31DE0939F296B530E2E6C511C9269335400B642F3D` |
| `include/m2m/doe_variable_record.hpp` | `5D53655CC7391139098DB9C357BE7AF4D7AFBE7B9E4FF7323F83FD55291CAA9E` |
| `tests/doe/test_variable_record.cpp` | `03D5E4A54CAC33F7068E36435E321AAA090DEFECB48AEB7B7D3E93EBEC9F586E` |
| `tests/doe/test_variable_description.cpp` | `8E6EDE2E289C0F3EB9446E855128D96DE2DD526339D3F553FBEF7A0E55F1A7D5` |

The next larger owner/decision cut is distinct unique-name multiheader import, requiring consistency of existing concrete owner aliases and source next-number/name lookup/selected state at every append. Duplicate/prepopulated names, Keyword/Geometry/deferred links, general numeric conversion, native Qt allocation/ref parity and public Save/output remain guarded. This is an actual reader field mutation, not full scalar parsing or public Save completion.
