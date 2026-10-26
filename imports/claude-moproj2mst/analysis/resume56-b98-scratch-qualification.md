# B98 candidate-only scratch qualification

Two checkpoint56 read-only audits flagged the B98 regression's runtime path as
longer than 100 characters and outside `C:\PROBLEM`. The applicable converter
`AGENTS.md` expressly places **all takeover test scratch and analysis outputs
beneath this converter directory** and protects external original test projects.
The parent `AGENTS.md` short `C:\PROBLEM` path rule is for DEF_PRE/DEFORM GUI test
projects in the separate DMO work. This B98 test runs only the headless converter
on a copied fixture under CTest's configured converter scratch directory.

The B98 test asserts that the candidate writes its actual copied KEY path and
that replacing only that path in the candidate MST yields exact historical bytes.
It does not launch DEFORM or establish original behavior for long paths. The
path-length scope is therefore absent from this candidate-only regression,
rather than silently treated as validated. Fresh original comparisons, when the
GUI-control release gate permits them, will use their separately qualified
disposable short-path runtime trees.
