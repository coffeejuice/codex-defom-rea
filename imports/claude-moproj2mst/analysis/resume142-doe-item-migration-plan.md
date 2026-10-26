# CP142: one canonical Item owner with explicit borrowed snapshots

Keep `Var::items` as the only keyword collection. Change each Item into a move-only envelope that distinguishes the existing value-only text projection from one owned, stable keyword constructor record. Replace the emitter's implicit Item copy with an explicit snapshot: copy the legacy text values, borrow the typed record pointer. This preserves current value-only output and native pointer identity without extending the keyword's lifetime.

This is a design report only. No source/test/build, GUI, original process or mutable REA action occurred. CP141's normal owner/destructor evidence is the basis; no new original-binary claim is made here. This migration does not admit public Keyword parsing, reconcile nonempty Save, create DYNARY data or complete a notification.

## Current surface and impact

The relevant production uses are bounded:

| Current location | Use / migration impact |
| --- | --- |
| `include/m2m/mst/doe_model.hpp:35` | Item holds only `before` and `after`; Var+30 is `vector<Item>` at83. Replace the element's ownership contract, retaining that single vector. |
| `src/mst/emit_b_doe.cpp:78` | Copies `vector<Item>` before reading Variable index/link/name. Replace at this exact point with the explicit snapshot. |
| `src/mst/emit_b_doe.cpp:96..103` | Emits before/after text with unchanged line counts, separators and numbering. Resolve snapshot entry to the same two Strings here. |
| `src/doe/variable_dimension_prefix.cpp:50` | Checks only nonempty; keep its existing boundary. A typed owner must not accidentally relax CP138. |
| `src/doe/doe_study.cpp:352` | Private prune checks items/files emptiness; envelope count retains the current semantics. A native-null entry still counts as an entry. |
| `src/doe/doe_study.cpp:715`, `src/doe/geometry_variable_writer.cpp:26` | B121/B122 isolated guards require empty items; unchanged. |
| `tests/p09/test_emit_b.cpp:473` | The only discovered DOE Item initializer-list copy: `base.items = {{before, after}}`. Move-only Item requires a narrow fixture syntax change. |

DOE count-only tests use `items.emplace_back()` and check empty/size, particularly `test_empty_variable_prune.cpp:130` and `test_variable_dimension_prefix.cpp:123..190`. Preserve default Item construction as legacy value-only/unknown metadata so these remain valid. Other `m2m::Item`, FlowItem and Spring item types are unrelated and must not be edited.

The scoped search found no production copy of a whole DOE Var or HeaderVariableRecord. Making Item move-only will make an attempted vector/Var ownership copy fail when instantiated; do not claim that a standard copyability trait on `vector<Item>` proves otherwise. No explicit Var copy/delete API change is needed for this first slice. Root's grouped compile must resolve any additional actual copy use rather than silently deep-copy an owner or add shared ownership.

## Exact canonical type contract

Use one forward-declared `KeywordConstructorRecord` with concrete, non-polymorphic deletion. Keep its definition in a worker-owned private DOE header. It holds only the source-qualified normal constructor projection, including upper/lower text and the distinct +80/+84 selectors. Its three data slots are explicitly constructor-null state; no method can adopt or manufacture a nonnull DYNARY in this increment. It is a constructor-prefix value projection, not a literal0xa8 allocation or complete native record.

The shared header declares the following interface shape; names are proposed and must be frozen before root/worker edits:

```cpp
struct KeywordConstructorRecord; // defined privately; concrete owner
struct LegacyItemText { String before, after; };
enum class ItemKind { LegacyValue, NativeKeyword };

class Item {
public:
    Item();                         // LegacyValue with default Strings
    Item(String before, String after); // LegacyValue, no native metadata
    ~Item();                        // out of line where record is complete
    Item(Item&&) noexcept;
    Item& operator=(Item&&) noexcept;
    Item(const Item&) = delete;
    Item& operator=(const Item&) = delete;

    static Item ownKeyword(std::unique_ptr<KeywordConstructorRecord>);
    ItemKind kind() const noexcept;
    LegacyItemText* legacyText() noexcept;
    const LegacyItemText* legacyText() const noexcept;
    KeywordConstructorRecord* keyword() noexcept;
    const KeywordConstructorRecord* keyword() const noexcept;
    // private tagged storage: legacy text OR unique record owner
};

using ItemSnapshotEntry = std::variant<LegacyItemText,
                                      const KeywordConstructorRecord*>;
using ItemSnapshot = std::vector<ItemSnapshotEntry>;
ItemSnapshot snapshotItems(const Var&);
LegacyItemText itemSnapshotText(const ItemSnapshotEntry&);
```

`Var` needs a forward declaration before the free-function declaration if these declarations precede it. The shared header adds memory/variant includes; it must not include private DOE record implementation headers or defmodel/DYNARY layouts. Put destructor, move operations, factories, accessors and snapshot functions out of line in the worker's new source. Out-of-line special members avoid requiring the opaque record's complete definition in every include consumer. The tagged storage can itself be private variant storage; no public tag/payload combination should be independently writable.

`ownKeyword(nullptr)` denotes an explicitly represented native-null pointer entry, distinct from LegacyValue whose Strings are default/null. The proposed fresh normal append helper always creates a nonnull record; it does not use nullptr to imitate an allocation failure. Legacy values do not gain keyword ID, entities or native-pointer validity from having text or being counted.

Heap ownership gives the record a stable address across vector growth and Item moves. Copying/moving a pointer snapshot does not copy/move a record. No shared_ptr, Study back edge, Session back edge, reference-count owner or native-layout reinterpret cast belongs in Item. No second `HeaderVariableRecord::keywords` collection is introduced.

## Snapshot and emitter semantics

`snapshotItems` visits the vector once in its current order. LegacyValue becomes a copy of its two Strings, preserving null versus empty and current requested text bytes. NativeKeyword becomes its raw `const KeywordConstructorRecord*`, including nullptr for the explicitly represented null case. The snapshot holds no Item envelope pointer, so subsequent Item-vector reallocation cannot invalidate a typed record identity. It holds no owning handle, so clearing the owner invalidates that borrowed record pointer immediately.

The snapshot is valid for typed dereference only while the actual owning Variable/record remains live and unchanged in ownership. It must not escape as a Session-independent observation, keep a Study alive, or be dereferenced after record destruction. There is no concurrent owner mutation or callback delivery in the currently modeled synchronous emitter. This contract does not grant safety under arbitrary reentrant deletion.

In `emitDoeVariables`, take this snapshot where the old vector copy occurs, then retain all following code order. At each entry, `itemSnapshotText` returns the copied legacy text, or copies typed record upper/lower text at that encounter. Legacy snapshot text therefore remains the entry-time value; typed snapshots preserve the native record identity and observe its field values when consumed. The two meanings are intentional, explicit and testable.

Do not replace the old copy with `const auto& items = v->items`: that discards legacy snapshot semantics and does not define typed borrowing. Do not copy full typed records to preserve the old line of code. Do not introduce a duplicated before/after text cache for typed entries; the typed record is their sole source.

The native emitter's null-pointer behavior has not been qualified in this task. `itemSnapshotText` must explicitly refuse a NativeKeyword/null entry before dereferencing it, retaining already-emitted Study/header text at that encounter. It must not silently skip, emit a blank keyword or treat it as a legacy empty text entry. This is a reconstruction guard, not a claim that the original threw. Existing legacy empty text continues to emit exactly as before.

## Study and Header lifetime

Study continues to own HeaderVariableRecord through `headerVariableOwners`; borrowed `Study::vars` aliases remain unchanged. HeaderVariableRecord inherits Var, and concrete deletion runs the Item vector destructor automatically. Item then deletes its one concrete keyword constructor record exactly once. Var still has no virtual destructor, so ownership must never be moved into `unique_ptr<Var>` for a derived HeaderVariableRecord.

Existing prune/removal destroys the matching Header owner and removes its borrowed alias using the already-frozen code. It must not gain a filesystem effect, sample payload change or a snapshot keepalive. Retained B121 ScalarVariablePrefixRecord and its private list remain separate diagnostics and are not adopted into this collection. This migration does not fix or broaden any legacy scalar/geometry owner API.

## Minimum coordinated write set

Root/shared edits:

1. `source/apps/moproj2mst/include/m2m/mst/doe_model.hpp`: add opaque record/tagged Item/snapshot declarations and required standard includes; retain `vector<Item>` and every unrelated model field.
2. `source/apps/moproj2mst/src/mst/emit_b_doe.cpp`: replace the copied vector with `snapshotItems(*v)` at78, then use `itemSnapshotText(entry)` at96; preserve all surrounding emission order/format.
3. `source/apps/moproj2mst/tests/p09/test_emit_b.cpp:473`: replace initializer-list assignment with `base.items.emplace_back(String(...), String(...))`, retaining the same golden bytes. Expand a focused emitter assertion there only if root prefers that to a separate worker test.

Worker-exclusive new files after assignment:

- `src/doe/keyword_constructor_record.hpp`: complete normal constructor-prefix record and source-qualified fields/defaults; no data-output constructor or notifier.
- `src/doe/item_owner.cpp`: Item special members/factory/snapshot/text projection. Existing source globs should include it; root verifies build binding.
- `tests/doe/test_item_owner.cpp`: owner/snapshot semantic tests. Test target `m2m_doe_item_owner`, CTest `m2m.doe.item_owner` under existing naming.

The later fresh append increment should be a separate small worker file/test, such as `keyword_constructor_prefix.hpp/.cpp` and `test_keyword_constructor_prefix.cpp`, after this canonical owner API is frozen. It uses the actual Header receiver, explicit decoded inputs, dimension-before-append/operation-union order and constructor-null data state from CP141. Do not implement public parser admission as part of owner migration. No Study/shared Session, C ABI, CMake, manager, notification, geometry or Type3 file is needed for the proposed migration.

## Focused validation and guards

Tests for this migration should establish:

- default/count-only and two-String legacy construction preserve empty/size behavior; legacy snapshot retains String nullness/content after the original legacy entry changes;
- Item is noncopyable and movable; a live typed record pointer remains identical through Item moves and Item-vector growth; a snapshot copies only that borrowed pointer;
- the typed record remains owned by the Item, has no owner back edge and is not duplicated by a snapshot. After owner reset, do not dereference the dangling snapshot merely to test lifetime; use pointer/type properties or a narrow destruction witness only if it does not become production state;
- typed upper/lower projection uses the record's fields, while an explicit native-null entry refuses at encounter and remains distinct from legacy empty values;
- existing P09 full emitter bytes, line counts, linked numbering and null/empty legacy text remain unchanged; no new DOEVAR row is emitted by merely creating an empty owner list;
- actual Header owner cleanup remains concrete; CP137 count-based prune and CP138 nonempty-item refusal remain unchanged. Public Keyword child still stops at its existing entry, and public active Save still refuses before reconciliation/prune/XML.

Root runs the grouped compile/focused tests/full required checks. Header copyability and the opaque-type destructor/linkage are material risks to catch in that compile, even though the scoped search found only the one copied Item list and one initializer-list fixture. No test or build was run for this report.
