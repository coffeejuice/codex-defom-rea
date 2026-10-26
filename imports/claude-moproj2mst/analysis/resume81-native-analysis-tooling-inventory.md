# CP81 native analysis tooling and snapshot inventory

## Available local disassembler

Microsoft DUMPBIN is installed and has been used for the retained converter PE
work:

`C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\dumpbin.exe`

Its executable hash is recorded in multiple converter receipts as
`12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`.
It is suitable for bounded byte disassembly, exports, and PE metadata. It does
not provide a complete analyzed incoming-xref database, so it cannot by itself
answer the requested cross-reference questions for Cogging, DBLIST, or Report.

No `ghidraRun`, `analyzeHeadless`, IDA, radare2, or rizin command resolved from
PATH. Common Ghidra installation roots under Program Files and Local Programs
were not found. A directory exists at
`C:\Users\alext\AppData\Roaming\ghidra`, but filesystem access was denied;
its contents and any installed distribution path therefore remain unverified.
No install or launch was attempted.

## Candidate imported Ghidra project

One project-shaped copy was found outside the converter checkout:

`C:\Users\alext\AppData\Local\Temp\claude\C--Users-alext-Projects-claude-deform-rea--claude-worktrees-nice-curie-c9319d\e70ebe89-d5c7-4765-877d-0d6e399b479e\scratchpad\gp\guimo.gpr`

The `.gpr` file is zero bytes. Its sibling `guimo.rep` contains `project.prp`
(164 bytes) and the `idata`, `user`, and `versioned` folders. This proves a
Ghidra project-store-shaped residue exists, but does not prove it is complete,
opens successfully, or contains the imported `DEF_GUI_MO_64.exe` program. It is
in a Claude temporary worktree scratchpad, not an authoritative converter
snapshot. Do not open or modify this copy as a next step without first
establishing owner, target identity, program metadata, and a disposable copy.

The converter checkout does have extensive retained static evidence under
`imports/claude-moproj2mst/analysis/`, including Cogging, DBLIST and Report
bounded DUMPBIN captures, plus source/binary identity receipts. The focused
search found no checked-in `.gpr`, `.rep`, `.gzf`, `.i64`, or `.idb` analysis
database. The retained `analysis/resume48-distortion-vt198-rea-session/` copy
contains an executable, not a native analysis project. Thus there is no
verified imported converter Ghidra snapshot path to reuse at this point.

## Safe serialized xref route

For native incoming xrefs, use an already installed Ghidra headless distribution
only after its executable path and version have been verified. Preserve the
original project store untouched: make one disposable copy of a verified
project, ensure it contains the hash-identified original converter PE, and run
one analyst at a time against that copy. Export the specific function/data
reference results and the analysis log to a new uniquely named evidence file;
do not concurrently open or analyze the same Ghidra project database. If no
complete project exists, import the immutable PE into a fresh disposable
project, with no writes to the retained original or converter source tree. Keep
the target SHA-256 and Ghidra version/profile in the receipt. The immediate
functions of interest should be selected explicitly from the existing native
receipts for Cogging, DBLIST/report-list, and Report viewer/constructor paths,
then query incoming references for each address in the same serialized
analysis pass. DUMPBIN remains the bounded instruction fallback, not an xref
substitute.

The REA tool surface advertises `open_binary`, `xrefs`, native instruction and
procedure inspection, evidence export, and `close_binary`. These tools were
not called for this inventory. A future REA pass should likewise use one
hash-verified target and one active analysis session at a time, export the
needed evidence, and close the target before switching. Do not claim this is
non-mutating with respect to REA's backing analysis store without an explicit
provider guarantee; the current task did not touch it.

Inventory actions were filesystem reads and command discovery only, plus the
required `python tools/recovery_status.py` start check. No analysis database,
source, build, application, or GUI was changed or launched.
