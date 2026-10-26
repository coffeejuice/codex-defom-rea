# deformdb

C++20 reader and writer for DEFORM v11 `.DB` result databases. It needs no DEFORM installation and no
licence: it reads the file format described in [`docs/db-format-spec.md`](../docs/db-format-spec.md).

## Build (Windows, MSVC + Ninja)

```bat
deformdb\build.cmd            :: Release build + codec tests
```

Targets: `deformdb_core` (static C++ library), `deformdb.dll` (C ABI, `include/deformdb/deformdb.h`),
`dbinfo.exe` (inspection tool), `dbmanager.exe` (purge / merge), `test_codec`, `test_writer`, `capi_smoke`.

## Layers

| Header | What it gives |
|---|---|
| `deformdb/database.hpp` | `Database::open`, file header, step directory, `read_step` (header + MAR/A arrays + regions + dynamic arrays), parallel `read_steps` |
| `deformdb/model.hpp` | `StepModel`: objects, global node/element numbering, any keyword by name (`RZ`, `STRESS`, `NDTMP`, `BCCDEF`, …), connectivity |
| `deformdb/writer.hpp` | `Writer`: creates a database and appends steps (full or header-only), the counterpart of DEFORM's write-step operation |
| `deformdb/manager.hpp` | `purge` and `merge` with the rules of DEF_GUI23's DB Manager ([`docs/task-dbmanager-purge-merge.md`](../docs/task-dbmanager-purge-merge.md)) |
| `deformdb/deformdb.h` | C ABI over the reader, for applications such as DMO |

Design points for very large files (hundreds of GB, 10 000+ steps):

* positional reads only (`ReadFile` with an offset / `pread`), so any number of threads can read
  different steps of one open file; nothing is memory-mapped as a whole;
* the directory is read once (13 ints per step); a step is only touched when it is requested;
* each MAR/A region is a separate compressed block, so callers that need only a few keywords can
  be served by reading only those regions (planned; today a step is decoded completely);
* `refresh()` re-reads the header and directory, for databases a running solver is still appending to.

## Examples

```bat
dbinfo C:\path\run.DB                         :: header + step directory
dbinfo C:\path\run.DB --step 25 --objects     :: objects and keyword summary
dbinfo C:\path\run.DB --step 25 --regions     :: MAR/A region table
dbinfo C:\path\run.DB --all --threads 16      :: decode every step, timing
dbinfo C:\path\run.DB --step 25 --dump RZ     :: values, for scripts/compare_key.py
```

```bat
dbmanager purge run.DB run_PUR.DB -1 25 40:60   :: keep -1, 25 and every |step| in 40..60
dbmanager merge part1.DB part2.DB run_MERGE.DB  :: order found from the -(last+1) restart step
```

```cpp
deformdb::purge("run.DB", "run_PUR.DB", std::vector<std::int32_t>{25, 50, 100});
deformdb::merge("part1.DB", "part2.DB", "run_MERGE.DB");

auto db = deformdb::Database::open("run.DB");
auto step = db->read_step(*db->find_step(25));
deformdb::StepModel m(step);
for (const auto& obj : m.objects()) {
    auto rz = m.real_keyword(obj, "RZ");          // rows = nodes, width = 3
    auto sts = m.real_keyword(obj, "STRESS");     // rows = elements, width = 6
    auto conn = m.connectivity(obj);              // local node numbers
}
```

## Testing against DEFORM

`scripts/compare_key.py` compares `dbinfo --dump` output with a KEY file written by DEFORM's own
text export (`DEF_PRE`: `DBREAD <step>`, `KFWRIT <file>`), run on a copy of the database.

Writer checks done on copies of the two example databases:

* `dbmanager purge` keeping every step rewrites both files with the same record layout; the
  only differing bytes are ISTART, the A record of header-only entries (DEFORM leaves a stale
  value, we write 0) and H7/H8 in header-only step headers (the solver keeps the lengths, the
  GUI path writes 0). 9.2 MB file: 181 bytes differ; 5.0 GB file: 682 of 9.8 M records differ,
  all of them those fields. The 5 GB copy takes about 15 s.
* KEY files exported by DEF_PRE from purged and merged files are byte-identical to the ones
  exported from the original for the same step (COG 12, 25, −13; Inc718 690).
* Merging the two halves of a purged COG file gives the same bytes as the full copy.
