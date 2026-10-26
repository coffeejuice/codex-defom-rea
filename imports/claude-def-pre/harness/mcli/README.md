# `-m` utility path cases (packet m-cli)

`mcli_cases.py` runs 129 cases of `DEF_PRE_64 -m ...` through the oracle cache and the port, and
compares stdout, stderr, exit code and every written file with `defcmp oracle`.

```bat
port\build.cmd
python -I harness\mcli\mcli_cases.py --port port\build\def_pre.exe --defcmp port\build\defcmp\defcmp.exe
python -I harness\mcli\mcli_cases.py --list          :: case ids and arguments
python -I harness\mcli\mcli_cases.py --oracle-only   :: fill the oracle cache only
```

What the cases cover: the four modes plus `-t 1|2|3` on both corpus DBs and on three DBs made from
a copy of COG with `dbmanager purge` (fixed `--date`, into `port/build/mcli_synth`): an operation
with one stored step, gaps in the operation numbers, a single step. Help (`-h`, `-H`, after other
options), missing `-o` / `-d` (exit -1), parsed-only modes (`summary`, `reclist`, `diff`),
aliases, attached values, repeated options, quote stripping, nine kinds of bad DB file for every
mode, an output file that cannot be opened, a value missing at the end of the command line, an
empty or blank DB name, 1023/1024-character values, `NEWEQ.MSG` as input or output.

Behaviour of the original these cases pin down (all recovered from the code first):

| Case | Original |
|---|---|
| `-o`/`-d`/`-m`/`-t` as last argument | reads `argv[argc]` (NULL): exit 0xC0000005, nothing printed |
| `-o` file cannot be opened | header/stepList/lastStepNo pass NULL to fprintf: exit 0xC0000417; simStepList checks and exits 0 |
| `-d ""` or blank, header mode | Fortran runtime error "Bad file specification" on stderr, then exit 0xC0000005 |
| option value of 1024+ characters | quote stripper overruns its stack buffer: exit 0xC0000409 |
| `"x"` values | `FUN_14001b700` terminates at the old end: `"q.txt"` becomes `q.txtt` |
| simStepList | the "last step" of an operation with one stored step is the previous operation's; `-t 1` / `-t 2` (hidden) add step lists without a final newline |
| `SFTC-DEFORM-2D-DB720` | rejected by the signature check of `-m` although the reader knows it |
