# CP531–533 efficiency review

The three source-bound checkpoints used one focused preflight, one full build,
one 388-or-387-test launcher and one freeze each. CP532's first preflight
rejected a missing test main in 0.55 seconds, before any expensive build;
the corrected `cp532r` run passed. The shared `model.hpp` edit at CP531
forced a 153.36-second full build after a 35.50-second focused compile.
CP532r and CP533 avoided that shared-header edit and each full build took
53.70 and 53.97 seconds; focused compiles took 2.41 and 2.44 seconds.
The bound launchers took 22.546, 22.641 and 22.844 seconds respectively.

This supports keeping private graph diagnostics in a separate header/source
and using targeted preflight before the full build. The measured mean
launcher time here is 22.677 seconds; no 10× end-to-end improvement is
claimed. Source-bound freezes, raw differences and historical 93/98 matches
remain required. The next safe unit is CP534 backend/receiver provenance,
while independent editor300, DOE, failure and format investigations continue.
