"""Corpus runner for the moproj2mst converter (stage 1, packet s1-00).

For each corpus case (gui_mo/s1/corpus/<case_id>/, listed in corpus/cases.tsv):
  1. copy the case to a scratch folder (the corpus itself is never handed to the CLI);
  2. hash every file of the scratch project (sha256, plus the set of files and folders);
  3. run  <cli> <scratch>/<proj>/<name>.moproj  <scratch>/out/<case_id>.MST
     (the output path is outside the project folder);
  4. byte-compare the output with the saved .MST of the corpus case;
  5. re-hash the scratch project and fail if any file was added, removed or changed;
  6. print one table row: result, first differing line number, expected and actual line.

Results: PASS, DIFF (output differs), CHANGED (project touched; wins over PASS/DIFF),
A case with a `waiver` in cases.tsv has lines of its expected MST rewritten first (see WAIVERS);
the note lists each waived line. EXIT (non-zero exit), NOOUT (no output file), TIMEOUT, NOORACLE (case has no saved MST;
the CLI still runs and the project check still applies), SKIP (case not in the corpus).

usage:
  python -I run_corpus.py --cli "C:/path/moproj2mst.exe" [--cli "extra arg" ...]
         [--corpus DIR] [--cases PATTERN ...] [--oracle-a] [--oracle-b] [--tsv results.tsv]
         [--keep] [--timeout 120] [--scratch DIR]
  python -I run_corpus.py --self-test [--all]

--cli may be given several times; the values form the command prefix (e.g. --cli python
--cli -I --cli conv.py).  A single value with spaces is split (shlex, Windows quoting kept).
Exit status: 0 when every selected case with an oracle is PASS and no case is CHANGED.
Stdlib only.
"""
import argparse, fnmatch, hashlib, os, shlex, shutil, subprocess, sys, tempfile, time

HERE = os.path.dirname(os.path.abspath(__file__))
DEFAULT_CORPUS = os.path.normpath(os.path.join(HERE, '..', 'corpus'))


# ---------------------------------------------------------------- corpus
def load_cases(corpus):
    path = os.path.join(corpus, 'cases.tsv')
    with open(path, encoding='utf-8') as f:
        lines = f.read().split('\n')
    cols = lines[0].split('\t')
    out = []
    for ln in lines[1:]:
        if ln.strip():
            out.append(dict(zip(cols, ln.split('\t'))))
    return out


def tree_state(root):
    """Map relative path -> sha256 (files) or '<dir>' (folders)."""
    st = {}
    for r, ds, fs in os.walk(root):
        for d in ds:
            st[os.path.relpath(os.path.join(r, d), root).replace('\\', '/') + '/'] = '<dir>'
        for fn in fs:
            p = os.path.join(r, fn)
            h = hashlib.sha256()
            with open(p, 'rb') as f:
                for b in iter(lambda: f.read(1 << 20), b''):
                    h.update(b)
            st[os.path.relpath(p, root).replace('\\', '/')] = h.hexdigest()
    return st


def tree_diff(a, b):
    added = sorted(set(b) - set(a))
    removed = sorted(set(a) - set(b))
    changed = sorted(k for k in set(a) & set(b) if a[k] != b[k])
    return added, removed, changed


def first_diff(exp, got):
    """1-based line number of the first differing line and both lines (bytes, no EOL)."""
    el = exp.split(b'\n')
    gl = got.split(b'\n')
    for i in range(max(len(el), len(gl))):
        e = el[i] if i < len(el) else None
        g = gl[i] if i < len(gl) else None
        if e != g:
            return i + 1, e, g
    return 0, None, None  # identical as line lists (cannot happen if bytes differ)


def diff_col(e, g):
    """0-based first differing byte column of two lines (None = EOF)."""
    if e is None or g is None:
        return 0
    n = min(len(e), len(g))
    for i in range(n):
        if e[i] != g[i]:
            return i
    return n


def show(b, col=0, width=60):
    """Line as escaped text; long lines are cut to a window around the first differing column."""
    if b is None:
        return '<EOF>'
    start = max(0, col - width // 2) if len(b) > width else 0
    piece = b[start:start + width]
    s = repr(piece)[2:-1]
    return ('...' if start else '') + s + ('...' if start + width < len(b) else '')


# ---------------------------------------------------------------- known-diff waivers
# cases.tsv column `waiver`: comma-separated waiver ids for pre-v11 saves. Each waiver rewrites
# lines of the EXPECTED MST to what v11 writes; every rewritten line is reported in the note.
#   legacy-express-simtyp: 2013 saves print the legacy express <Type> in *!SIMTYP; v11's factory
#   (0x140046cc0, s1-01 5.2) maps it to SfOprHeatTransferExpress(2D) (P01, coordinator 2026-10-22).
#   project-dir: an oracle-B MST names its save folder (column `saved_dir`) in a path built from
#   the project location (Diestress KFREAD of <db>.KEY); the run's project copy is elsewhere, so
#   the expected lines get the copy's folder (forward slashes) instead. Not for stored strings
#   the converter copies as they are (HeatTreat user keyword file from HeatTreatWizard.HTWZ).
LEGACY_EXPRESS = (b'SfOprHeatExpress', b'SfOprAirConvect', b'SfOprHeatResting', b'SfOprHeatDwelling')
SIMTYP = b'*!SIMTYP  \t'


def waive_legacy_express(lines, waived, case, proj):
    for i, ln in enumerate(lines):
        if not ln.startswith(SIMTYP):
            continue
        key = ln[len(SIMTYP):]
        if any(k in key for k in LEGACY_EXPRESS):
            new = b'SfOprHeatTransferExpress2D' if b'2D' in key else b'SfOprHeatTransferExpress'
            lines[i] = SIMTYP + new
            waived.append('L%d %s->%s' % (i + 1, key.decode('latin-1'), new.decode()))


def waive_project_dir(lines, waived, case, proj):
    old = (case.get('saved_dir') or '').encode('utf-8')
    if not old or proj is None:
        raise SystemExit('waiver project-dir needs saved_dir and a project copy (case %s)' % case.get('case_id'))
    new = os.path.abspath(proj).replace('\\', '/').encode('utf-8')
    for i, ln in enumerate(lines):
        if old in ln:
            lines[i] = ln.replace(old, new)
            waived.append('L%d saved_dir->copy' % (i + 1))


#   unopened-genctc: operations saved without reopening keep GENCTC off, while v11 open+save writes
#   "GENCTC 0" after the INICTC lines (0x1400651f0 sets GENCTC on, the ActionKeywords parser never
#   clears it): oracle B re-saves gui_mo/oracle_b/R11, R19, R46, R49, R52 (P12, coordinator 2026-10-22).
GENCTC = b'GENCTC    \t'
INICTC = b'INICTC    \t'


def waive_unopened_genctc(lines, waived, case, proj):
    start, i = 0, 0
    while i < len(lines):
        if lines[i].rstrip(b'\r') == b'OPRNAM':
            ini = [j for j in range(start, i) if lines[j].startswith(INICTC)]
            if ini and not any(lines[j].startswith(GENCTC) for j in range(start, i)):
                cr = b'\r' if lines[ini[-1]].endswith(b'\r') else b''
                lines.insert(ini[-1] + 1, GENCTC + b'0' + cr)
                waived.append('L%d +GENCTC 0' % (ini[-1] + 2))
                i += 1
            start = i + 1
        i += 1


WAIVERS = {'legacy-express-simtyp': waive_legacy_express, 'project-dir': waive_project_dir,
           'unopened-genctc': waive_unopened_genctc}


def apply_waivers(case, exp, proj=None):
    """Return (expected bytes after the case's waivers, list of waived-line descriptions)."""
    ids = [w.strip() for w in (case.get('waiver') or '').split(',') if w.strip()]
    if not ids:
        return exp, []
    lines = exp.split(b'\n')
    waived = []
    for w in ids:
        if w not in WAIVERS:
            raise SystemExit('unknown waiver %r in case %s' % (w, case.get('case_id')))
        WAIVERS[w](lines, waived, case, proj)
    return b'\n'.join(lines), waived


# ---------------------------------------------------------------- one case
def run_case(case, corpus, cli, scratch, timeout, keep):
    cid = case['case_id']
    src = os.path.join(corpus, cid)
    res = dict(case=cid, result='', line='', expected='', got='', note='', secs='')
    if not os.path.isdir(src):
        res['result'] = 'SKIP'; res['note'] = case.get('status', 'not in corpus')
        return res
    work = os.path.join(scratch, cid)
    proj = os.path.join(work, 'proj')
    outdir = os.path.join(work, 'out')
    if os.path.exists(work):
        shutil.rmtree(work)
    shutil.copytree(src, proj)  # copy2: mtimes kept
    os.makedirs(outdir)
    out_mst = os.path.join(outdir, cid + '.MST')
    moproj = os.path.join(proj, case['moproj'])
    mst_name = case.get('mst', '')
    before = tree_state(proj)
    t0 = time.time()
    try:
        cp = subprocess.run(cli + [moproj, out_mst], cwd=outdir, stdout=subprocess.PIPE,
                            stderr=subprocess.STDOUT, timeout=timeout)
        rc = cp.returncode
        tail = cp.stdout.decode('utf-8', 'replace').strip().splitlines()[-1:] if cp.stdout else []
    except subprocess.TimeoutExpired:
        rc = None; tail = ['timeout']
    res['secs'] = '%.2f' % (time.time() - t0)
    after = tree_state(proj)
    added, removed, changed = tree_diff(before, after)

    if rc is None:
        res['result'] = 'TIMEOUT'
    elif rc != 0:
        res['result'] = 'EXIT'; res['note'] = 'rc=%d %s' % (rc, ' '.join(tail))[:120]
    elif not os.path.isfile(out_mst):
        res['result'] = 'NOOUT'
    elif not mst_name:
        res['result'] = 'NOORACLE'
    else:
        exp = open(os.path.join(src, mst_name), 'rb').read()
        got = open(out_mst, 'rb').read()
        exp, waived = apply_waivers(case, exp, proj)
        if waived:
            res['note'] = 'waived ' + '; '.join(waived)
        if exp == got:
            res['result'] = 'PASS'
        else:
            res['result'] = 'DIFF'
            n, e, g = first_diff(exp, got)
            col = diff_col(e, g)
            res['line'] = str(n); res['expected'] = show(e, col); res['got'] = show(g, col)
            res['note'] = ('col %d; bytes exp=%d got=%d' % (col + 1, len(exp), len(got)) +
                           ('; ' + res['note'] if res['note'] else ''))
    if added or removed or changed:
        res['result'] = 'CHANGED'
        parts = []
        for tag, lst in (('+', added), ('-', removed), ('~', changed)):
            parts += [tag + p for p in lst[:3]]
        res['note'] = ('project changed: ' + ' '.join(parts))[:160]
    if not keep:
        shutil.rmtree(work, ignore_errors=True)
    return res


def print_table(rows, stream=sys.stdout):
    hdr = ('case', 'result', 'line', 'expected', 'got', 'note')
    w = [max(len(h), *(len(str(r[h])) for r in rows)) if rows else len(h) for h in hdr]
    w = [min(x, 48) for x in w]
    fmt = '  '.join('%%-%ds' % x for x in w)
    print(fmt % hdr, file=stream)
    print(fmt % tuple('-' * x for x in w), file=stream)
    for r in rows:
        print(fmt % tuple(str(r[h])[:48] if h in ('case', 'result', 'line') else str(r[h]) for h in hdr),
              file=stream)


def run_all(args, cli, cases_override=None):
    corpus = os.path.abspath(args.corpus)
    cases = load_cases(corpus) if cases_override is None else cases_override
    if args.cases:
        cases = [c for c in cases if any(fnmatch.fnmatch(c['case_id'], p) or p in c['case_id']
                                         for p in args.cases)]
    want = {'A'} if args.oracle_a else set()
    if getattr(args, 'oracle_b', False):
        want.add('B')
    if want:
        cases = [c for c in cases if c.get('oracle') in want]
    own_scratch = args.scratch is None
    scratch = args.scratch or tempfile.mkdtemp(prefix='m2m_corpus_')
    os.makedirs(scratch, exist_ok=True)
    rows = []
    try:
        for c in cases:
            r = run_case(c, corpus, cli, scratch, args.timeout, args.keep)
            rows.append(r)
            print('%-44s %s %s' % (r['case'], r['result'], r['line']), file=sys.stderr, flush=True)
    finally:
        if own_scratch and not args.keep:
            shutil.rmtree(scratch, ignore_errors=True)
    print_table(rows)
    cnt = {}
    for r in rows:
        cnt[r['result']] = cnt.get(r['result'], 0) + 1
    print('\nsummary: ' + ', '.join('%s=%d' % kv for kv in sorted(cnt.items())))
    if args.tsv:
        with open(args.tsv, 'w', encoding='utf-8', newline='\n') as f:
            cols = ('case', 'result', 'line', 'expected', 'got', 'note', 'secs')
            f.write('\t'.join(cols) + '\n')
            for r in rows:
                f.write('\t'.join(str(r[k]).replace('\t', ' ') for k in cols) + '\n')
    bad = [r for r in rows if r['result'] not in ('PASS', 'NOORACLE', 'SKIP')]
    return rows, (1 if bad else 0)


# ---------------------------------------------------------------- self-test
def dummy_cli(argv):
    """Dummy converter: copy <stem>.MST next to the moproj to the output path.
    --mode ok | flip (change one byte on line 3) | touch (also modify a project file)
    | none (write nothing; for cases without MST write an empty file in 'ok' mode)."""
    mode = 'ok'
    if argv and argv[0].startswith('--mode='):
        mode = argv[0].split('=', 1)[1]; argv = argv[1:]
    moproj, out = argv
    d = os.path.dirname(moproj)
    stem = os.path.splitext(os.path.basename(moproj))[0]
    src = None
    for fn in os.listdir(d):
        if fn.lower() == stem.lower() + '.mst':
            src = os.path.join(d, fn)
    data = open(src, 'rb').read() if src else b''
    if mode == 'none':
        return 0
    if mode == 'flip' and data:
        lines = data.split(b'\n')
        k = min(2, len(lines) - 1)
        lines[k] = lines[k] + b'X'
        data = b'\n'.join(lines)
    if mode == 'touch':
        with open(moproj, 'ab') as f:
            f.write(b' ')
    with open(out, 'wb') as f:
        f.write(data)
    return 0


def self_test(args):
    me = os.path.abspath(__file__)
    py = [sys.executable, '-I', me, '--dummy-cli']
    timing = os.environ.get('M2M_RUNNER_SELF_TEST_TIMING') is not None
    total_start = time.perf_counter()
    load_start = time.perf_counter()
    all_cases = load_cases(os.path.abspath(args.corpus))
    if timing:
        print('TIMING runner_self_test phase=load_cases seconds=%.6f' %
              (time.perf_counter() - load_start), file=sys.stderr, flush=True)
    ok = True

    def sub(mode, cases, oracle_a):
        a = argparse.Namespace(corpus=args.corpus, cases=cases, oracle_a=oracle_a, scratch=None,
                               keep=False, timeout=args.timeout, tsv=None)
        print('\n=== self-test: dummy CLI mode=%s ===' % mode)
        mode_start = time.perf_counter()
        result = run_all(a, py + ['--mode=' + mode], all_cases)
        if timing:
            print('TIMING runner_self_test phase=mode mode=%s seconds=%.6f' %
                  (mode, time.perf_counter() - mode_start), file=sys.stderr, flush=True)
        return result

    selection_start = time.perf_counter()
    small = [c['case_id'] for c in all_cases if c.get('status') == 'copied' and c.get('oracle') == 'A'
             and int(c.get('copied_bytes') or 0) < 3_000_000][:3]
    pos_cases = None if args.all else small + [c['case_id'] for c in all_cases if c.get('has_mst') == '0'][:1]
    if timing:
        print('TIMING runner_self_test phase=case_selection seconds=%.6f' %
              (time.perf_counter() - selection_start), file=sys.stderr, flush=True)
    rows, rc = sub('ok', pos_cases, False)
    exp_ok = all(r['result'] in ('PASS', 'NOORACLE', 'SKIP') for r in rows) and rc == 0
    print('check ok-mode: all PASS/NOORACLE ->', exp_ok); ok &= exp_ok

    rows, rc = sub('flip', small[:2], False)
    exp = all(r['result'] == 'DIFF' and r['line'] == '3' and 'X' in r['got'] for r in rows) and rc == 1
    print('check flip-mode: DIFF at line 3 ->', exp); ok &= exp

    rows, rc = sub('touch', small[:2], False)
    exp = all(r['result'] == 'CHANGED' for r in rows) and rc == 1
    print('check touch-mode: CHANGED ->', exp); ok &= exp

    rows, rc = sub('none', small[:1], False)
    exp = all(r['result'] == 'NOOUT' for r in rows) and rc == 1
    print('check none-mode: NOOUT ->', exp); ok &= exp

    # the corpus itself must be unchanged by the self-test (spot check mtimes of moproj files)
    print('\nSELF-TEST', 'PASSED' if ok else 'FAILED')
    if timing:
        print('TIMING runner_self_test phase=total seconds=%.6f' %
              (time.perf_counter() - total_start), file=sys.stderr, flush=True)
    return 0 if ok else 1


def main():
    if len(sys.argv) > 1 and sys.argv[1] == '--dummy-cli':
        sys.exit(dummy_cli(sys.argv[2:]))
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--cli', action='append', help='converter command (prefix); args <moproj> <out.mst>')
    ap.add_argument('--corpus', default=DEFAULT_CORPUS)
    ap.add_argument('--cases', nargs='*', help='case ids, substrings or glob patterns')
    ap.add_argument('--oracle-a', action='store_true', help='only cases marked oracle A')
    ap.add_argument('--oracle-b', action='store_true',
                    help='only cases marked oracle B (GUI-saved, oracle_b/); with --oracle-a: A and B')
    ap.add_argument('--tsv', help='write results TSV here')
    ap.add_argument('--keep', action='store_true', help='keep scratch copies and outputs')
    ap.add_argument('--scratch', help='scratch folder (default: new temp folder)')
    ap.add_argument('--timeout', type=float, default=300.0)
    ap.add_argument('--self-test', action='store_true')
    ap.add_argument('--all', action='store_true', help='self-test: positive run over every case')
    args = ap.parse_args()
    if args.self_test:
        sys.exit(self_test(args))
    if not args.cli:
        ap.error('--cli is required (or --self-test)')
    cli = []
    for v in args.cli:
        cli += [t.strip('"') for t in shlex.split(v, posix=False)] if (' ' in v and not os.path.exists(v)) else [v]
    _, rc = run_all(args, cli)
    sys.exit(rc)


if __name__ == '__main__':
    main()
