"""Build the moproj2mst test corpus as copies (s1-00).

Reads sources only.  For every .moproj in the list it copies, with mtimes kept (shutil.copy2):
  * the .moproj and the <stem>.MST next to it (if present);
  * the project data folders Objects, Materials, Lubricants, Equipments, Operations, Regions,
    DoeStudies and any sub-folder that holds a .moproj (sub-project), all files except *.DB;
  * root files *.par, *.ini (incl. DEF_DOE_PARSER.INI) and any root file the MST names
    that is not a .DB.
Everything else in the project folder is recorded in skipped.tsv with its size.
Projects whose copy would exceed --max-mb are not copied and are listed in cases.tsv.

usage: python -I build_corpus.py <moproj_list.txt> <corpus_dir> [--dry-run] [--max-mb 300]
       python -I build_corpus.py <list.tsv> <corpus_dir> --append

--append (oracle-B cases, corpus_b): each list line is `moproj<TAB>case_id<TAB>provenance`. The
rows are added to the existing cases.tsv and skipped.tsv (other rows and columns kept), with
oracle = B, the case's save folder in `saved_dir`, and root *.KEY files copied too (a HeatTreat
step's user keyword file). When the MST names the save folder the case is printed with `names
saved_dir`: set waiver `project-dir` by hand only if the path comes from the project location
(Diestress KFREAD <db>.KEY), not if it is a stored string the converter copies (HeatTreat user
keyword file in HeatTreatWizard.HTWZ).
"""
import argparse, collections, hashlib, os, re, shutil, sys
import xml.etree.ElementTree as ET

DATA_DIRS = {'objects', 'materials', 'lubricants', 'equipments', 'operations', 'regions', 'doestudies'}
SKIP_EXT_IN_DATA = {'.db'}
ROOT_EXT = {'.par', '.ini'}
SEP = re.compile(r'[\\/]')


def win(p):
    m = re.match(r'^/([a-zA-Z])/(.*)$', p)
    return (m.group(1).upper() + ':/' + m.group(2)) if m else p


def has_moproj(d):
    for r, ds, fs in os.walk(d):
        if any(f.lower().endswith('.moproj') for f in fs):
            return True
    return False


def mst_root_refs(mst_bytes):
    refs = set()
    for ln in mst_bytes.decode('latin-1').splitlines():
        s = ln.strip()
        if s and ' ' not in s and '\t' not in s and not SEP.search(s) and '.' in s and not s.startswith('*'):
            refs.add(s.lower())
    return refs


def parse_moproj(path):
    raw = open(path, 'rb').read().decode('utf-8')
    root = ET.fromstring(re.sub(r'<!DOCTYPE[^>]*>', '', raw, count=1))
    types = collections.Counter((e.text or '').strip() for e in root.iter('Type'))
    unit = root.find('Unit')
    unit = unit.get('Name') if unit is not None else ''
    paths = set()
    for e in root.iter():
        t = (e.text or '').strip()
        if t and SEP.search(t) and re.search(r'\.[A-Za-z0-9]{1,6}$', t) and '\n' not in t:
            paths.add(t)
    return types, unit, paths


def plan_case(moproj, root_ext=ROOT_EXT):
    d = os.path.dirname(moproj)
    stem = os.path.splitext(os.path.basename(moproj))[0]
    mst = None
    for e in os.scandir(d):
        if e.is_file() and e.name.lower() == stem.lower() + '.mst':
            mst = e.path
    refs = mst_root_refs(open(mst, 'rb').read()) if mst else set()
    copy, skip = [], []
    for e in os.scandir(d):
        rel = e.name
        if e.is_dir():
            sub = []
            take = e.name.lower() in DATA_DIRS or has_moproj(e.path)
            for r, ds, fs in os.walk(e.path):
                for f in fs:
                    fp = os.path.join(r, f)
                    rp = os.path.relpath(fp, d)
                    sz = os.path.getsize(fp)
                    if take and os.path.splitext(f)[1].lower() not in SKIP_EXT_IN_DATA:
                        copy.append((rp, sz))
                    else:
                        skip.append((rp, sz))
                if take and not fs and not ds:
                    copy.append((os.path.relpath(r, d) + os.sep, 0))  # keep empty dir
            continue
        sz = e.stat().st_size
        low = rel.lower()
        ext = os.path.splitext(low)[1]
        if low == stem.lower() + '.moproj' or (mst and rel == os.path.basename(mst)):
            copy.append((rel, sz))
        elif low.endswith('.moproj'):
            skip.append((rel, sz))  # another project sharing the folder
        elif ext in root_ext:
            copy.append((rel, sz))
        elif low in refs and ext != '.db':
            copy.append((rel, sz))
        else:
            skip.append((rel, sz))
    return d, stem, mst, copy, skip


def sha(path):
    h = hashlib.sha256()
    with open(path, 'rb') as f:
        for b in iter(lambda: f.read(1 << 20), b''):
            h.update(b)
    return h.hexdigest()


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('listfile'); ap.add_argument('corpus')
    ap.add_argument('--dry-run', action='store_true')
    ap.add_argument('--max-mb', type=float, default=300.0)
    ap.add_argument('--append', action='store_true')
    a = ap.parse_args()
    if a.append:
        return append(a)
    lst = [win(l.strip()) for l in open(a.listfile, encoding='utf-8') if l.strip()]
    os.makedirs(a.corpus, exist_ok=True)
    rows, skiprows, seen = [], [], {}
    for i, mp in enumerate(lst, 1):
        mp = os.path.normpath(mp)
        d, stem, mst, copy, skip = plan_case(mp)
        cid = '%02d_%s' % (i, stem)
        cbytes = sum(s for _, s in copy); sbytes = sum(s for _, s in skip)
        types, unit, mppaths = parse_moproj(mp)
        mp_mt = os.stat(mp).st_mtime
        dt = '' if not mst else '%.3f' % (os.stat(mst).st_mtime - mp_mt)
        oracle = 'A' if mst and abs(os.stat(mst).st_mtime - mp_mt) <= 2.0 else ''
        status = 'copied'
        if cbytes > a.max_mb * 1e6:
            status = 'skipped_size>%dMB' % a.max_mb
        key = (sha(mp), sha(mst) if mst else '')
        dup = seen.get(key, '')
        seen.setdefault(key, cid)
        missing = sorted(p for p in mppaths if not os.path.exists(os.path.join(d, p)))
        notcopied = sorted(p for p in mppaths if os.path.exists(os.path.join(d, p))
                           and os.path.normcase(os.path.normpath(p)) not in
                           {os.path.normcase(os.path.normpath(c)) for c, _ in copy})
        mstb = open(mst, 'rb').read() if mst else b''
        rows.append(dict(
            case_id=cid, source=mp.replace('\\', '/'), moproj=os.path.basename(mp),
            mst=os.path.basename(mst) if mst else '', has_mst=int(bool(mst)),
            mst_mtime_minus_moproj_mtime=dt, oracle=oracle,
            op_types=';'.join('%s:%d' % kv for kv in sorted(types.items())), n_ops=sum(types.values()),
            unit=unit, moproj_bytes=os.path.getsize(mp), mst_bytes=len(mstb),
            mst_lines=mstb.count(b'\n'), copied_files=len(copy), copied_bytes=cbytes,
            skipped_files=len(skip), skipped_bytes=sbytes, status=status, duplicate_of=dup,
            moproj_refs=len(mppaths), moproj_refs_missing_in_source=';'.join(missing[:20]),
            moproj_refs_not_copied=';'.join(notcopied[:20])))
        for rp, s in skip:
            skiprows.append((cid, rp.replace('\\', '/'), s, 'not needed'))
        if status != 'copied':
            for rp, s in copy:
                skiprows.append((cid, rp.replace('\\', '/'), s, status))
        print('%-48s %-8s %9.1f MB copy %9.1f MB skip  %s' % (cid, status[:8], cbytes / 1e6, sbytes / 1e6, oracle))
        if a.dry_run or status != 'copied':
            continue
        dst = os.path.join(a.corpus, cid)
        for rp, s in copy:
            if rp.endswith(os.sep):
                os.makedirs(os.path.join(dst, rp), exist_ok=True); continue
            t = os.path.join(dst, rp)
            os.makedirs(os.path.dirname(t), exist_ok=True)
            shutil.copy2(os.path.join(d, rp), t)
            if os.path.getsize(t) != s:
                sys.exit('size mismatch ' + t)
    if a.dry_run:
        return
    cols = list(rows[0].keys())
    with open(os.path.join(a.corpus, 'cases.tsv'), 'w', encoding='utf-8', newline='\n') as f:
        f.write('\t'.join(cols) + '\n')
        for r in rows:
            f.write('\t'.join(str(r[c]) for c in cols) + '\n')
    with open(os.path.join(a.corpus, 'skipped.tsv'), 'w', encoding='utf-8', newline='\n') as f:
        f.write('case_id\tpath\tbytes\treason\n')
        for r in skiprows:
            f.write('%s\t%s\t%d\t%s\n' % r)


def read_tsv(path):
    with open(path, encoding='utf-8') as f:
        lines = f.read().split('\n')
    cols = lines[0].split('\t')
    return cols, [dict(zip(cols, l.split('\t'))) for l in lines[1:] if l]


def write_tsv(path, cols, rows):
    with open(path, 'w', encoding='utf-8', newline='\n') as f:
        f.write('\t'.join(cols) + '\n')
        for r in rows:
            f.write('\t'.join(str(r.get(c, '')) for c in cols) + '\n')


def append(a):
    cols, rows = read_tsv(os.path.join(a.corpus, 'cases.tsv'))
    for c in ('saved_dir', 'provenance'):
        if c not in cols:
            cols.append(c)
    have = {r['case_id'] for r in rows}
    skp = open(os.path.join(a.corpus, 'skipped.tsv'), encoding='utf-8').read()
    for l in open(a.listfile, encoding='utf-8'):
        if not l.strip():
            continue
        mp, cid, prov = l.rstrip('\n').split('\t')
        mp = os.path.normpath(win(mp))
        if cid in have:
            sys.exit('case %s is already in cases.tsv' % cid)
        d, stem, mst, copy, skip = plan_case(mp, ROOT_EXT | {'.key'})
        if not mst:
            sys.exit('no MST next to ' + mp)
        types, unit, mppaths = parse_moproj(mp)
        mstb = open(mst, 'rb').read()
        saved = d.replace('\\', '/')
        dt = os.stat(mst).st_mtime - os.stat(mp).st_mtime
        if abs(dt) > 2.0:
            sys.exit('%s: MST and moproj were not saved together (%.3f s)' % (cid, dt))
        r = dict(case_id=cid, source=mp.replace('\\', '/'), moproj=os.path.basename(mp),
                 mst=os.path.basename(mst), has_mst=1, mst_mtime_minus_moproj_mtime='%.3f' % dt, oracle='B',
                 op_types=';'.join('%s:%d' % kv for kv in sorted(types.items())), n_ops=sum(types.values()),
                 unit=unit, moproj_bytes=os.path.getsize(mp), mst_bytes=len(mstb), mst_lines=mstb.count(b'\n'),
                 copied_files=len(copy), copied_bytes=sum(s for _, s in copy), skipped_files=len(skip),
                 skipped_bytes=sum(s for _, s in skip), status='copied', duplicate_of='',
                 moproj_refs=len(mppaths),
                 moproj_refs_missing_in_source=';'.join(sorted(p for p in mppaths if not os.path.exists(os.path.join(d, p)))[:20]),
                 moproj_refs_not_copied='', exclusion='',
                 waiver='', saved_dir=saved, provenance=prov)
        rows.append(r)
        dst = os.path.join(a.corpus, cid)
        if os.path.exists(dst):
            sys.exit(dst + ' exists')
        for rp, s in copy:
            if rp.endswith(os.sep):
                os.makedirs(os.path.join(dst, rp), exist_ok=True); continue
            t = os.path.join(dst, rp)
            os.makedirs(os.path.dirname(t), exist_ok=True)
            shutil.copy2(os.path.join(d, rp), t)
            if os.path.getsize(t) != s:
                sys.exit('size mismatch ' + t)
        for rp, s in skip:
            skp += '%s\t%s\t%d\t%s\n' % (cid, rp.replace('\\', '/'), s, 'not needed')
        print('%-24s B %8.1f KB copy  %s' % (cid, r['copied_bytes'] / 1e3, 'names saved_dir' if saved.encode() in mstb else ''))
    write_tsv(os.path.join(a.corpus, 'cases.tsv'), cols, rows)
    with open(os.path.join(a.corpus, 'skipped.tsv'), 'w', encoding='utf-8', newline='\n') as f:
        f.write(skp)


if __name__ == '__main__':
    main()
