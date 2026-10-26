# CP185 R300 repeated accessor implementation

Frozen CP184 working sources were hashed before edits:

- `source/apps/moproj2mst/src/engine/save_current.cpp`: SHA-256 `36b3b12693fd5c2b3d4f4bde9ec486a9b673f7575da4b6d94903de19ceb0cf8b`.
- `source/apps/moproj2mst/tests/s/test_save_current.cpp`: SHA-256 `aec6e922352e31123545270266576f7c9c1e9cd16e9cdf7ae30290b4fb530c37`.

Implementation follows `resume185-r300-native-accessor-plan.md` and the independent
`resume185-r300-accessor-plan-review.md`. Build, focused/full validation and source
freeze belong to root. No build, test execution, GUI, original execution or REA
database operation is authorized in this implementation lane.

`saveMaterials` now snapshots the signed material count through one fresh
operation-model resolution. Each saved position resolves its current step,
filters null/type-2 materials, and copies the material number before ending that
borrow. After default-path generation, every eligible position resolves the
model/step again and calls the existing writer synchronously. Writer status stays
ignored. No traversal model, step or material pointer survives path generation or
writer resolution. Existing `withOpStep` supplies the production lookup; no test
hook was added.

This follows the qualified retained native assembly at count call
`0x140058cf8/0x140058d00`, nth lookup `0x140058d34/0x140058d40`, number copy
`0x140058d6d/0x140058d6f`, and writer resolution/call
`0x140058dc6/0x140058dce/0x140058df3`. The runtime-unwind extraction is static
evidence of the included entry/loop/return, not a whole-procedure or runtime-trace
claim. Packet S `source/docs/re/moproj2mst/s/spec.md:3–10` and L0
`evidence/gui_mo/s1/mdt/L0/spec_A.md:3–10` qualifications were read. The retained
count body supplies the initial present-model/null-step zero-count branch;
operation-model call counts do not count its one-or-two holder reads.

Five synthetic direct-helper Testkit cases use the existing virtual
`OpClass::classModel` seam with model A and B owned throughout each case:

- Stable list with four positions, three eligible writes, duplicate number and
  type-2 exclusion: expects eight lookups and exact ordered nonempty KEY effects.
- Detached initial step: expects one count-time lookup and no effects. CP184's
  existing loaded-operation detached-step test is also preserved unchanged.
- A count/traversal and B writer with the same number but different material
  name: expects B bytes. B's type 2 does not add writer-time filtering.
- Consistent A count/list of two and B count/list of three: expects only the two
  saved positions, with B's second number/path/payload proving nth reacquisition.
- Native writer-time B missing A's first selected number: preserves the target
  sentinel and overlay, invokes no callback/open for that iteration, then writes
  B's later material. This checks continued traversal after ignored writer status.

All native test paths are validated beneath `M2M_TEST_SCRATCH`. The new material
constructor/list-helper include exists at
`source/port/defmodel/include/defmodel/material/material_ctor.hpp`; the test target
links `m2m_core` and inherits its dependency includes. The file retains its Testkit
entry point and uses supported `CHECK`, `CHECK_EQ` and `REQUIRE`, with no C `assert`
or unsupported `REQUIRE_EQ`. No build or test was executed by this owner; root must
run focused `m2m.s.save_current`, then its grouped/full gates and freeze.

Initial null-wrapper and later null-step guards deliberately remain qualified
adapter divergences. Repeated lookup and lexical pointer scope do not establish
public C-ABI replacement reachability, native failure equivalence or safety if a
writer callback destroys its active step/material. The synthetic fixtures never
perform such destruction and are not native/oracle byte-parity evidence.

Released implementation hashes:

- `source/apps/moproj2mst/src/engine/save_current.cpp`: SHA-256 `dfc90650f638389644301fca786b447a581f75ac749de444653eb0b8e5bedd54`.
- `source/apps/moproj2mst/tests/s/test_save_current.cpp`: SHA-256 `4bebfa62f75f939d3940f2da3627269ac11e53f475239b1944a7fa9995a62acf`.

Source files are released to root and held unchanged pending integration checks.
