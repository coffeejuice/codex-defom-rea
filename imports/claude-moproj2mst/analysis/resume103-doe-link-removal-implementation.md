# CP103 DOE LinkTo removal ownership fix

## Change

`doe_study.cpp::removeVarFiles` now mirrors the source-qualified graph cleanup for a found variable: it saves the exact list pointer, clears every incoming `Var::link` equal to it, erases its matching `HeaderVariableRecord` from concrete `unique_ptr` ownership (never deletes through `Var*`), removes the borrowed `Study::vars` entry, and marks all three sample profiles invalid. Existing geometry-file effect planning remains after the manager mutation. If no matching Study variable exists, owner/sample state is left alone as before.

After independent review found that unsupported scalar/foreign owners could otherwise be erased from `Study::vars` without a concrete destructor, the helper now requires a matching `headerVariableOwners` entry before any link/sample/list mutation. Unsupported owner kinds throw `M2M_E_UNSUPPORTED` with the graph and file effects unchanged. No native removal dispatch for those separate candidate owner types was available, so they remain refused.

This is intentionally scoped to the existing removal helper. A source search found no production caller, so the new focused test invokes the helper directly and does not claim public UI-removal integration. No Save/refresh, shared interface, CMake, ABI, or unrelated DOE path changed.

## Focused regression

New `tests/doe/test_variable_link_removal.cpp` has three cases. The first imports A→B, B→A, C→B, removes B, checks both surviving incoming links are null and the concrete owner/list vectors remain aligned, verifies all sample-invalid flags, then performs two more deferred-link reader calls to ensure the owner alignment still admits new rows. The second removes a self-linked record and verifies both vectors empty with invalid flags set. The third supplies a scalar-prefix-owned and a foreign stack-owned target; each must refuse before graph mutation, sample invalidation, or file-effect planning. These cases exercise forward/cyclic, self and unsupported-owner paths without Save/refresh.

## Static source evidence

Original immutable PE SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Bounded DUMPBIN reads covered manager removal `0x14091a090..0x14091a2ce`: incoming link compare/clear at `0x14091a1b9..0x14091a1d4`, deleting wrapper call at `0x14091a244..0x14091a253`, list removal call at `0x14091a269..0x14091a279`, and sample invalidation stores at `0x14091a29a..0x14091a2ac`. CP102's retained destructor capture for `0x1408169d0` establishes that the destructor does not follow `Var+0x40`.

## Preflight and frozen hashes

`python admin/check_test_source_preflight.py source/apps/moproj2mst/tests/doe/test_variable_link_removal.cpp` passed with no issues. `git diff --check` for the two source/test paths passed. No compiler, build, CTest, GUI, original process, or mutable REA database was run by this worker; root owns validation.

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/doe/doe_study.cpp` | `a81a54f0a7bf24254ef357ac9fc3ad8324ee56ca21278e9bdfbe8b2503c87a01` |
| `source/apps/moproj2mst/tests/doe/test_variable_link_removal.cpp` | `44999a64703a2da268b0a9168b2f2ba99e303f5a62e778eaf02ea0dfd855e2dd` |

Expected target: `m2m_doe_variable_link_removal`; existing source/test GLOB registration needs no CMake edit.
