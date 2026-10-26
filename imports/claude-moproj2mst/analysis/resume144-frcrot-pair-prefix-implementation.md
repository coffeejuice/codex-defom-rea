# CP144 FRCROT pair-prefix implementation receipt

## Write set

Only these three new files were added:

- `src/doe/frcrot_pair_prefix.hpp` — SHA-256 `5A3E55FA729455374D24A58EEB14D871E658793865E7E8FAD68CFDB73146CA81`
- `src/doe/frcrot_pair_prefix.cpp` — SHA-256 `C21923EE8CBD01968955C31BC32CABBD9F19B700E7F93E6F52ECF384272FA449`
- `tests/doe/test_frcrot_pair_prefix.cpp` — SHA-256 `02901F6EC2CAC2FD3A899FEF8D558383C11D72C1647C402BD21692BE6609DFD1`

No existing file, CMake registration, shared header, public conversion path, parser, registry, or guard was changed.

## Implemented contract

`applyFrcrotPairPrefix` takes ordered pair identities and prepared mode/table-reader outcomes. It preserves prior order, matches exact ordered `(master, slave)` identity, and appends the forward tuple before branching on mode-line success. Early mode-line failure returns after that forward identity effect and has no table writes or common reverse-tail observations.

On mode-line success, it records that the template reset was reached, then captures only source-qualified writes: mode 0 writes dimension 0 and the prepared double constant; mode 1/2 writes dimension 1, code 1/4, and a caller-prepared signed integer value. Unknown modes retain the reset-only observation. It then records the later table-reader call. This is a mutation-prefix projection, not a snapshot of opaque pre-existing native table storage.

The helper reaches the common tail for either table-reader result. Self-pairs skip reverse work. Other pairs find or append the reversed tuple, then record the direction-field write and mirror-copy call boundary. Table-reader failure remains the final status without rolling back forward table writes or pair-list changes.

## Focused cases

The six tests cover early failure with new and existing forward pairs; mode-0 fields retained after table-reader failure and reverse append; mode 1/2 distinct codes; unknown-mode reset-only behavior; self-pair skip; and existing reverse-pair/order retention. Assertions are on explicit prepared values and event order only.

## Limits and validation

No build or tests were run, per assignment. Static preflight was limited to reviewing the new interface/test includes, test identifiers, macro expressions, status/order branches, and the three-file write set. No public FRCROT admission is implied. Native text parsing, native allocation/default construction, table-row grammar/partial effects, reverse payload-copy results, selector bounds, and the three active input text owners remain outside this helper.

Evidence basis: `analysis/resume144-frcrot-pair-prefix-plan.md` and its review amendment `analysis/resume144-frcrot-pair-prefix-review.md`, grounded in immutable original image SHA-256 `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`.

