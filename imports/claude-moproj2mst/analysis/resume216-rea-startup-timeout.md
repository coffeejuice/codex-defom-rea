# CP216 read-only REA startup outcome

Opened the copied original `evidence/original-binaries/DEF_GUI_MO_64.exe`
through REA/Ghidra. The returned target SHA-256 matched
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
A focused read-only query for function `0x14008c630` then failed with
`provider_timeout`: Ghidra did not complete import/analysis within its
330,000 ms startup deadline. No function dossier, pseudocode or new behavioral
evidence was returned. `close_binary` completed without a snapshot.

Do not count this as confirmation or refutation of the retained Lubricants
creator mapping. The bounded copied-PE/DUMPBIN evidence in
`analysis/resume214-lubricants-creator.md` remains the current static basis;
its exact runtime creator attribution remains open. Do not repeat the same
Ghidra startup unchanged. A later supported analysis configuration or a
smaller provider query must be justified by a concrete missing source fact.
No original application process, GUI-control session or evidence fixture was
changed.
