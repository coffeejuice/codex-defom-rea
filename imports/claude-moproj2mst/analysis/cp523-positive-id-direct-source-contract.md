# CP523 positive-ID distinct direct-target row-height cut

CP522's immutable-PE proof gives an ordinary nonnegative-ID Forming2D
graphic without a matching Project Operation height at least189. The native
row rebuild retains the maximum graphic height plus25, so later distinct
row0 targets may use the strict finite `y<189` lower bound. The first item
still sees the empty row table and uses `y<67.5`.

Select only 2–7 initially unlinked ordinary nongroup Forming2D items in an
initially empty chart. Conservatively require every ID nonnegative and
unbound to a Project Operation. Each RectF component is finite, each x is
below1332.5, the first y is below67.5, later y below189. Final CellPos is
explicit valid row0 col0..6 or missing/OR sentinel. Snapshot target columns
must be distinct; sentinel derivation clamps x<-60, then half-up rounds
x/205. Parsed IDs, model order and signed dimension bits survive, with
cell `(0,c)` and RectF origin `(205*c,+0)`. Adjacent grid columns link;
gaps disconnect; MST follows the lowest contiguous occupied run.

Independent DUMPBIN14.44.35228.0 check of original GUI SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`
covered 15 complete `.pdata` spans, 19,242 contiguous bytes, 3,544
instructions, zero gaps and PE-byte mismatches. CP522's four template/base/
expansion/init hashes were reconfirmed. Additional bodies: row rebuild
`140157fe0..1401583ce` SHA-256
`4800f06d904dfa13555f738873e8cf3bb6aa5f3bd976ab56a089f096d417d65c`,
row lookup `14014dfa0..14014e179` SHA-256
`6101966ca147a3f060f7e92a6857f669a22898b3deb6e01157b4e0d478e43fd8`,
placement decision `140158dc0..140158ec9` SHA-256
`095b8b657e4eb80e809821e0ec4e3be5e93a58ce07ba219e0230f9fe79171883`,
installer `14015b610..14015b7d2` SHA-256
`31e7ce04f32ab20a292f48db5fb4b8865b65fa20a9da835de3462c493df9d0c0`.
Native maximum-only store at `1401582f9..14015831d` and populated-row
strict comparison at `14014e089..14014e0a2` establish the bound.

Associated-Operation profiles, collisions, negative or mixed IDs, exact
positive graphic height and fresh Open/Save/Close parity remain outside this
cut. Boundary exclusions check no new route effects; other older explicit
CellPos fallthrough may still parse without selected relayout.
