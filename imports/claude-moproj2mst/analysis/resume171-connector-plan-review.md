# CP171 connector independent plan review

## Decision

**Conditionally source-ready for implementation after accepted CP170 becomes source-stable and passes its focused, checkpoint, and original-comparison gates.** The proposed cut is correctly limited to [0x1311c7, 0x1311d2): the lea resolves the qualified QtCore export address, and the following mov persists it at QObjectPrivate +0x68. The cut ends before the separate atomic increment beginning at 0x1311d2. Therefore it can establish a nonowning binding and cursor advance, but must leave the reference-committed marker false and symbolic delta unchanged.

The plan accurately records its unresolved prerequisite: the present source has CP170's private58 / BeforeQVectorSharedNull frame, but no stable focused/checkpoint/original-comparison receipt is established. Current source ends at this boundary. Static bytes alone do not authorize unconditional source readiness.

## Contract assessment

A QVector shared-null record owned by the same allocation store's associated WidgetRuntimeState is the right root identity. The source already models QList and QString symbols as root-owned records qualified by QtCore hash/RVA, with symbolic deltas and unknown absolute counts. Extending that pattern for QVector at RVA 0x2f0bb8 is coherent. The record must remain pointer-identical for constructor frames and must not derive a live count from PE initialized bytes. PendingTaskPrototype::SparseQObjectPrivate should keep a weak/nonowning binding and a separate false marker until the later increment cut.

The root owner guard should mirror the existing QList/QString cuts: require the factory's live WidgetRuntimeState to be the exact root paired with TaskAllocationStore, checked by ownsWidgetRuntime (control-block and pointee identity), then validate all symbol qualification and predecessor state before changing either the binding or phase. Existing ConversionSession ownership pairing is relevant as the positive end-to-end case; a standalone or aliasing foreign root must fail. The brush-gradient helper is a separate owner with no demonstrated process-root interning, so it cannot satisfy this guard. If later evidence establishes one process scope for both, reconcile through one canonical symbol owner rather than duplicate records.

The predecessor checks are well chosen: exact retained Report and private identities, sizes and address points, BeforeQVectorSharedNull phase, four observed zero fields (+0x40/+0x48/+0x50/+0x58), committed and pointer-identical QList/QString bindings, and incomplete/unbacklinked QObject state. On admission only the +0x68 binding and phase should change. All deltas/count observations, maps, allocation identities, and completion flags remain fixed. Reentry after the stop should refuse without changing that frame.

One detail to preserve in implementation is that the QVector root record's qualification must be validated independently of its mere presence: exact QtCore digest and symbol RVA, with no malformed or substituted record accepted. An expired private binding is the expected unbound state; any live prior binding, even to an otherwise qualified object, should be rejected as premature state.

## Evidence and acceptance

The static PE evidence supports the selected instruction interpretation and export identity. It does not prove a live occurrence, exceptional cleanup, destructor release, or any reference-count value. Model identity by module hash/RVA, not preferred VA. The later atomic increment needs a separate cut and same-call live pre/post observation before any absolute-count claim.

The focused acceptance list is appropriate: begin from an accepted source-stable CP170 frame; capture root and pending identities, predecessor bindings/deltas, phase, sizes/address points, fields, maps, counts, and flags; then show one continuation changing only the binding and cursor. Exercise same-key/different-key reentry and the malformed/foreign owner and predecessor cases without mutation. The explicit ConversionSession owner-pairing positive case and isolation of the gradient symbol are especially useful. Run the named factory/root-owner focused tests and the standard checkpoint/original-comparison gates before calling the implementation accepted.

**Conclusion:** no material semantic gap found in the proposed cut or owner contract. The sole readiness blocker is the missing source-stable accepted CP170 prerequisite and its required gates. This review ran no tests, build, comparison, GUI, or live REA operation.

