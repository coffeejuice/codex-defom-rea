# CP105 project-Open split-DB test: static review

**Result: no blocking issue found.** The new test adds a caller-level check
beyond CP103's synthetic direct-`dbOpen` test, while preserving the distinction
between the candidate's synchronous default-answer branch and native modal
event delivery.

## Route and assertions

`openProject` is the actual candidate `.moproj` Open routine. In
`src/moproj/read.cpp` it sets the project path, invokes
`engine::openProjectDb`, and only then reads the project XML. The test prepares
the Project the same way the full-save path does (`initProject` with the
`.DB`-derived absolute path, then `makeRegistry`) and calls `openProject`.
Thus the split database is opened through the project caller before the XML
walk, rather than by directly seeding a reader or invoking only `dbOpen`.
`openedOk` and `dataDb` assertions confirm the outer Open and parsed project
state in addition to reaching the DB branch.

The header-only `COG_P1_B125.DB`/`COG_P1_B1.DB` fixture is produced with the
format writer from the retained COG DB's header/step data. The test labels it
synthetic and does not claim it is a naturally captured 2-GB split. Assertions
cover selected primary path, loaded/cache state, two-part order and directory
ranges, the 175-pixel DB-open result, and the candidate warning that records
the configured default Yes. These align with CP102's native predicate/default
qualification and extend CP103's direct `dbOpen` test to the real caller path.

The warning text is an observation of the candidate's modeled answer; the test
does not dispatch a `QMessageBox`, drive timer callbacks inside nested
`QDialog::exec`, or establish full GUI Open equivalence. It also stops before
subproject activation, Save, and Close, as documented.

## Path and cleanup

`ProblemScratch` creates a fresh child under `C:\\PROBLEM`, canonicalizes it,
and refuses cleanup unless the resolved parent is the canonical problem root.
It copies only the retained project file and writes the two synthetic DBs in
that child. The three direct generated paths are checked against the 100
character limit. `DbCoreReset`, `ProjectScope`, and local declaration order
release model/DB owners before the scratch directory destructor removes the
owned child. No preexisting leaf is removed or overwritten.

Static review only: this does not establish runtime cleanup success. Root still
needs the assigned build/test receipt. No source, build, fixture, GUI, or
original application was changed or run for this review.
