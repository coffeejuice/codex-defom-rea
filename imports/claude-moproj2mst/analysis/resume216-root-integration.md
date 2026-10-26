# CP216 integration and successor

The only implementation increment is a pure five-path operation project
directory decision collector in
`source/apps/moproj2mst/src/model/operation_project_directories.hpp`.
It records `Operations`, `Objects`, `Materials`, `Equipments`, and `Lubricants`
in the copied-PE static order for qualifying operation IDs. It performs no
I/O and is not called from public Open, Save or `FileEffects::mkdirs`.
`analysis/resume216-opdir-review.md` independently passed that narrow source
contract. The first focused build reached a Testkit printing error in a
string equality assertion; the test was changed to use fixed expected paths
and Testkit's Boolean `CHECK`, then the MSVC-configured focused build and
CTest passed. Its scratch-directory check confirms no directory creation.

The source-bound complete suite at `build/test-runs/1793023252965661100`
passed: 363 registered, 356 clean, six passing with logged gaps, one passing
diagnostic difference, zero failed/skipped/incomplete outputs and zero fixture
changes. Historical comparison remains 93 SAME/five DIFF. The build receipt
is `analysis/cp216-opdir-build.json`; the source/binary freeze is
`analysis/wave3-checkpoint216-validation.json` and
`build/wave3-checkpoint216-verified/`. No old test's diagnostic fields
changed relative to checkpoint215.

REA opened the copied original PE with the expected SHA-256, but its Ghidra
provider timed out during initial import/analysis and returned no function
evidence. The session was closed; no identical retry was made. See
`analysis/resume216-rea-startup-timeout.md`. CP213's `Lubricants` side effect
and the retained static call chain still do not prove the exact runtime
creator or failures.

CP217 next needs a supported same-occurrence original operation-directory
decision/effect witness before the collector can be integrated into public
Open writes. `python admin/check_continuation.py` remains `CONTINUE`; active
DOE, diagram/view, XML parser, Save and accepted-Close work is unfinished.
