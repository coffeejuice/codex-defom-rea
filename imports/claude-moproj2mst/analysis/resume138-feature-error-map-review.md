# CP138 independent retained error-map review

No blocking discrepancy found. The helper accurately projects the retained initial 45-row table with first-match priority and unsigned32 fallback arithmetic. It remains a pure explicit-input function with no live runtime/error-owner or Close integration.

## Independent source/data comparison

Read the original PE through section mapping and decoded `143b5e160..143b5e2c8` as 45 little-endian uint32/int32 pairs. The ordered source array matches every pair exactly. Duplicate key6 occurs at zero-based rows5 and30 with outputs9 then22; the source linear loop returns9. The golden test contains exactly the44 unique first-match entries in retained encounter order. The adjacent fallback errno word is excluded by the native/source 45-row bound; error0 remains22.

Original DEF_GUI_MO_64 v11.0.2 SHA-256 independently matches `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The extracted table SHA-256 is `dc5aaf854d47b0fa4deddacb6ff02e9a7d99cb1ac06c16e03e587d4599067fbe`. Mapper `1414740ec..141474133` hashes to `f44e86936fa3b398df977c4dbe968eff2c5fec42fd3c07a7dc4adc02d61bce17`, matching both receipts. Its bytes use the45-entry first-match loop, 32-bit LEA(error-19)/unsigned comparison17, then 32-bit ADD(error-188)/unsigned comparison14. The source uses explicit uint32 subtraction, preserves wraparound, and looks up the table before both fallback ranges. Returned numeric codes match without host errno constants.

## Tests, ownership and guard boundary

All44 source keys are covered. Other checks distinguish key6 priority, table-over-range18/33, input0, access-range transitions18/19/36/37, executable-range187/188/202/203, table-specific206/1816 and large/wrapping uint32 values. The noexcept assertion and host-only errno probe are appropriate for purity; the probe restores entry errno before Testkit assertions. It does not observe LastError or qualify native PTD.

Production code contains only fixed constexpr data, a bounded lookup and integer arithmetic. It has no allocation, OS/TLS/error accessor, external state write or default/current-error argument. All references remain in its private header/implementation and focused test; the helper is not wired to FeatureCrtRuntime, paused free failure, FLS/PTD, Project, Session or Close. The writable-native-data versus retained-initial-image distinction is explicit in API documentation and receipt. No runtime table-writer closure or live errno store is claimed. No required regression addition identified.

## Frozen binding

All source/test hashes match implementation receipt `analysis/resume138-feature-error-map.md`, SHA-256 `f27be6f28f406fc50618403c96f63597a8643e430e23cbca6e06a0e328b78269`:

- Header: `ec689dd8845c8def3bbb6e3abdcabd663cb4506f83842f596013e067f17b0f92`.
- Implementation: `128c9e07d02e589fc1f14d0e7de0a6a1656c9b120ed408135cc034f921f71cd5`.
- Test: `c9ea0e7c83446932d2afd47efa00a62081885bca49d9f0616a91c65ad2f4e5d0`.

Controlling proposal hash `6eea1fc004796fc8c534f69b6966835fcc2078914a1c4bb117ed3a0ab3eac7e5` also matches. Root owns compilation/execution of `m2m_model_feature_win32_error_map` / `m2m.model.feature_win32_error_map`. This review ran only static source/PE data comparison; no C++ build/test, GUI/original execution or mutable REA action occurred. Only this report was written.
