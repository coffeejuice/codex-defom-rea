# Per-packet ledger status

One file per porting packet, `<packet>.tsv` (e.g. `p1-04.tsv`), owned by that packet only:

```
address	status	note
0x1401c8950	ported	port/defmodel/src/db_step_data.cpp
```

`status` is `todo`, `ported`, `verified` or `not-needed`. `python -I docs/re/fold_status.py`
folds these files into `docs/re/pre64_batch_ledger.tsv`; see `docs/port-conventions.md` section 8.
