# CP478 safe-boundary efficiency review

The last three source-stable grouped builds took CP476r 31.657 seconds, CP477 127.891 seconds and CP478 47.281 seconds. Their complete bound launcher times were 19.953, 19.359 and 19.531 seconds. CP477 changed a shared Stream/OutputDevice header and rebuilt many more targets, so these builds are not a controlled speed comparison. The complete launcher duration stayed around 19–20 seconds in these runs. Token usage was unavailable; no token-saving claim is made.

The grouped preflight caught a CP476 test enum-printing compile error before the full launcher. Independent reviews caught test expectation/order gaps before CP477 and CP478 full builds. CP478 grouped two disjoint production increments in one build. No validation gates were weakened, and the next implementation action remains CP479 typed NUM_CON/row evidence, with public Type3 and original-session acceptance still guarded.
