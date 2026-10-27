# CP510–512 efficiency review

The three selected parser/model increments used source-stable focused checks,
one complete build and one bound 387-test launcher each. Measured grouped
full-build steps were 35.281, 54.469 and 50.875 seconds for CP510, CP511 and
CP512. The corresponding bound launchers were 22.688, 45.516 and 22.093
seconds. CP511's longer launcher is an observed outlier, not an established
regression cause; registered test count and result categories remained the
same. Focused subproject cases increased from 46 to 47 to 48, all passing.

The source audit work ran in parallel without competing source ownership,
builds, original-app sessions or mutable REA. CP510/511/512 PE contracts were
available before each mutation, while independent read-only reviewers checked
each implementation. Other parallel audits found witness-gated editor300,
DOE, graph, format and operation branches and avoided speculative source
changes. These observations support retaining the grouped build and bounded
source-audit workflow; they do not measure a tenfold time or token reduction.
Model token usage was unavailable from the local receipts and is not inferred
from output size. Next review follows three more checkpoints or one hour at a
safe boundary.
