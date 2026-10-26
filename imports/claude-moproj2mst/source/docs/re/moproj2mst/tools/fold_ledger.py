"""Fold the per-spec ledgers s1-0x/ledger.tsv into docs/re/moproj2mst/ledger.tsv (stage 1, G0).

Columns: address, size, role, status, packet, assign, specs
  status  spec / library / out-of-scope / pending / fenced from the spec ledgers, or `ported`
          once the owning packet is merged (kept across re-folds, or set with --ported P08 ...)
  packet  owner from the plan (section 6) and the seam headers:
          assign=seam  the address is declared in apps/moproj2mst (stub or @src line)
          assign=rule  inferred from the spec and the role text (check when porting)
          assign=-     library / out-of-scope rows (no owner)
  specs   which spec ledgers list the address

Packet ledgers (pNN/ledger.tsv, pNN-ledger.tsv; a header row with `address` and `status`) mark
their rows ported (status `ported...` or `spec+port`; an address named in apps/moproj2mst/src outside
src/stubs (`@src` or inline) also marks a row ported), give them that packet (or the row's
`packet` column) and add addresses the spec ledgers do not list. Owner decisions taken after the
fold are in owners.tsv (address or lo-hi range, packet, note) and override every other owner.

usage: python -I fold_ledger.py [--ported P08 ...] [--check]
  --check  only print counts and rows without an owner; exit 1 if any spec row has none
Stdlib only.
"""
import argparse, glob, os, re, sys

HERE = os.path.dirname(os.path.abspath(__file__))
DOCS = os.path.normpath(os.path.join(HERE, '..'))
REPO = os.path.normpath(os.path.join(DOCS, '..', '..', '..'))
APP = os.path.join(REPO, 'apps', 'moproj2mst')
OUT = os.path.join(DOCS, 'ledger.tsv')
OWNERS = os.path.join(DOCS, 'owners.tsv')

STATUS_RANK = {'ported': 0, 'spec': 1, 'pending': 2, 'fenced': 3, 'library': 4, 'out-of-scope': 5}
ADDR = re.compile(r'0x(14[0-9a-fA-F]{7})')

# Owner of a header when a line names no packet (plan section 5 and the header comments).
HEADER_OWNER = {
    'qt4.hpp': 'P04', 'qt4path.hpp': 'P05', 'xml.hpp': 'P01', 'datafiles.hpp': 'P06',
    'props.hpp': 'P02', 'stream.hpp': 'P04', 'state.hpp': 'G0', 'writer.hpp': 'P10',
    'emit.hpp': 'P07', 'model.hpp': 'P02', 'common.hpp': 'G0', 'convert.hpp': 'G0',
}

# Rules per spec: (regex on the role text, packet); first match wins, then the spec default.
RULES = {
    's1-01': [(r'Sub_?Project|sub-project|diagram|DiagramItem|flow|chain|predecessor|group|cell|'
               r'Connections|ItemData|activate|project subdir|projectDir|default DB path|'
               r'set path \+ DB path|SetPrimaryDb|OpenProject|ReadProjectFile|WalkProject', None),
              (r'MoItem|ItemList|item list|KEY path from id|material (type|phase|is mixture)', 'P01'),
              (r'OpenProject|ReadProjectFile|WalkProject|SetPrimaryDb|set unit', 'P01'),
              (r'Sub_?Project|sub-project|diagram|flow|chain|predecessor|group|cell|Connections|'
               r'ItemData|activate|project subdir|projectDir|default DB path|set path', 'P03')],
    's1-02': [(r'DOE|\.MST\.TEMP|sub-project block|SUBPRJ', 'P17'),
              (r'main \.MST|saveMst|header|main-project|trailer|walk|dispatch|mstPath|codec', 'P07'),
              (r'SIMULATION|DIM|NSTART|DBREAD|KFREAD|OPRNAM|SIMNAM|CURSIM|GENDB|KFWRIT|MDSOBJ|'
               r'material|keyword', 'P08'),
              (r'OBJTYP|MTLGRP|CNTACT|OBJPOS|TLOC|INICTC|GENCTC|state var|NRECVL|ERECVL|SVSCAL|'
               r'REMESH|MeshSettings|BccDef|IMPEQP|ObjProperties|equipment|mask', 'P09'),
              (r'base writer|slot \+0x(280|288|200|290|190)|vbase|dim flags', 'P10'),
              (r'cycle', 'P13'),
              (r'sub-project|flow', 'P03')],
    's1-03': [(r'slot \+0x(200|288|290)|base writer|self\+0x20|props of op', 'P10'),
              (r'block|writer|CHKSTOP|translation|size selector|automatic bite|number of|passes of|'
               r'bites of|index valid|pointer|direction table|object id table|die count|UseManip|'
               r'adaptive', 'P11'),
              (r'MDT|read|default|ctor|dtor|global|alloc|free|init|store|copy|load|name', 'P06')],
    's1-04': [(r'Boolean|BWZ|MACHDIST', 'P12'),
              (r'Convert|2Dto3D|CNV23|CopyMirror|mirror|rotation', 'P12'),
              (r'Cycle|CYCL|DoeOutput|Group', 'P13'),
              (r'Distortion', 'P13'),
              (r'MultiBlow|blow|sim record|nextSim|sim-control', 'P14'),
              (r'unit', 'P17')],
    's1-05': [(r'DOE|\.COR|DEF_DOE_PARSER|sub-project', 'P17'),
              (r'FormingExpress|Express|FE |FWZ|BL|DW|TO |SIMNAM builder|blow|step', 'P15'),
              (r'sim list|sim entry|position', 'P14'),
              (r'Diestress|HeatTreat|Report|ShapeOptim|MOKEY|schedule|Wzd', 'P16'),
              (r'unit', 'P17')],
}
DEFAULT = {'s1-01': 'P02', 's1-02': 'P09', 's1-03': 'P11', 's1-04': 'P13', 's1-05': 'P16'}


def seam_owners():
    """address -> packet from src/stubs/pNN.cpp and the @src lines of the headers."""
    own = {}
    for p in sorted(glob.glob(os.path.join(APP, 'src', 'stubs', 'p*.cpp'))):
        pkt = 'P' + os.path.basename(p)[1:3]
        for ln in open(p, encoding='utf-8'):
            for a in ADDR.findall(ln):
                own.setdefault(a.lower(), pkt)
    for p in sorted(glob.glob(os.path.join(APP, 'include', '**', '*.hpp'), recursive=True)):
        cur = HEADER_OWNER.get(os.path.basename(p))
        for ln in open(p, encoding='utf-8'):
            m = re.match(r'\s*// =*\s*$', ln)
            sec = re.match(r'// (P\d\d|G0):', ln)
            if sec:
                cur = sec.group(1)
            if '@src' not in ln:
                continue
            m = re.search(r'\((P\d\d)\)', ln)
            pkt = m.group(1) if m else cur
            for a in ADDR.findall(ln):
                own.setdefault(a.lower(), pkt)
    return own


# Address ranges of one class's code (checked before the role rules).
RANGES = [
    (0x1401c0000, 0x1401cffff, 'P12'),   # Convert2Dto3D
    (0x140e20000, 0x140e2ffff, 'P12'),   # CopyMirror
    (0x1411d0000, 0x1411effff, 'P12'),   # Boolean
    (0x140600000, 0x14060ffff, 'P14'),   # MultiBlow
    (0x1405fb000, 0x1405fdfff, 'P14'),   # props base sim-control fill (+0x70..+0xb0)
    (0x140e32000, 0x140e33fff, 'P13'),   # DoeOutput
    (0x14134c000, 0x14134ffff, 'P13'),   # Cycle
    (0x141360000, 0x14136ffff, 'P13'),   # Distortion
    (0x141160000, 0x14116ffff, 'P13'),   # Distortion BCC INI block
    (0x141400000, 0x14143ffff, 'P15'),   # FormingExpress
    (0x140290000, 0x1402bffff, 'P16'),   # Diestress / DiestressBM
    (0x1411a0000, 0x1411cffff, 'P16'),   # HeatTreat
    (0x1412a0000, 0x1412bffff, 'P16'),   # Report
    (0x140130000, 0x14014ffff, None),    # MST emitters / ShapeOptim: role rules decide
]


def rule_owner(spec, role, addr=None):
    if addr:
        a = int(addr, 16)
        for lo, hi, pkt in RANGES:
            if lo <= a <= hi:
                if pkt:
                    return pkt
                break
    for rx, pkt in RULES.get(spec, []):
        if pkt and re.search(rx, role, re.I):
            return pkt
    return DEFAULT.get(spec)


def implemented():
    """addresses named (`@src` tags or inline 0x14... comments) in apps/moproj2mst/src outside src/stubs."""
    out = set()
    for p in glob.glob(os.path.join(APP, 'src', '**', '*.*'), recursive=True):
        if os.sep + 'stubs' + os.sep in p or not p.endswith(('.cpp', '.hpp', '.inc')):
            continue
        for ln in open(p, encoding='utf-8', errors='replace'):
            # @src tags, and addresses the port names inline at the line that does the work
            out.update(a.lower() for a in ADDR.findall(ln))
    return out


def read_owners():
    """[(lo, hi, packet)] from owners.tsv; later lines win."""
    out = []
    if os.path.exists(OWNERS):
        for ln in open(OWNERS, encoding='utf-8').read().split('\n')[1:]:
            c = ln.split('\t')
            if len(c) < 2 or not c[0].strip():
                continue
            a = c[0].strip().lower().replace('0x', '').split('-')
            out.append((int(a[0], 16), int(a[-1], 16), c[1].strip()))
    return out


def packet_ledgers():
    """address -> (packet, ported, role) from the packet ledgers."""
    res = {}
    paths = glob.glob(os.path.join(DOCS, 'p[0-9][0-9]', 'ledger.tsv')) + \
        glob.glob(os.path.join(DOCS, 'p[0-9][0-9]-ledger.tsv')) + \
        glob.glob(os.path.join(DOCS, 'mdt', 'ledger.tsv')) + \
        glob.glob(os.path.join(DOCS, 'mdt', '*', 'ledger.tsv')) + \
        glob.glob(os.path.join(DOCS, 's', 'ledger.tsv'))
    for p in sorted(paths):
        m = re.search(r'[\\/](p\d\d|mdt|s)(?:[\\/]|-ledger)', p)
        pkt = m.group(1).upper()
        lines = open(p, encoding='utf-8').read().split('\n')
        head = [h.strip().lower() for h in lines[0].split('\t')]
        if 'address' not in head or 'status' not in head:
            continue
        ia, ist = head.index('address'), head.index('status')
        ir = head.index('role') if 'role' in head else None
        ip = head.index('packet') if 'packet' in head else None
        for ln in lines[1:]:
            c = ln.split('\t')
            if len(c) <= max(ia, ist) or not re.match(r'(0x)?14[0-9a-fA-F]{7}$', c[ia].strip()):
                continue
            a = c[ia].strip().lower().replace('0x', '')
            st = c[ist].strip().lower()
            own = pkt
            if ip is not None and len(c) > ip and re.match(r'(P\d\d|MDT)$', c[ip].strip().upper()):
                own = c[ip].strip().upper()
            done = st.startswith('ported') or st == 'spec+port'
            role = c[ir] if ir is not None and len(c) > ir else ''
            res[a] = (own, done, role)
    return res


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--ported', nargs='*', default=[], help='packets whose seam rows are ported')
    ap.add_argument('--check', action='store_true')
    args = ap.parse_args()

    prev = {}
    if os.path.exists(OUT):
        for ln in open(OUT, encoding='utf-8').read().split('\n')[1:]:
            c = ln.split('\t')
            if len(c) >= 5:
                prev[c[0]] = c

    rows = {}
    for p in sorted(glob.glob(os.path.join(DOCS, 's1-0*', 'ledger.tsv'))):
        spec = os.path.basename(os.path.dirname(p))
        for ln in open(p, encoding='utf-8').read().split('\n')[1:]:
            if not ln.strip():
                continue
            c = ln.split('\t')
            addr, size, role, status = (c + ['', '', '', ''])[:4]
            addr = addr.lower().replace('0x', '')
            r = rows.get(addr)
            if r is None:
                rows[addr] = dict(address=addr, size=size, role=role, status=status, specs=[spec],
                                  spec=spec)
            else:
                r['specs'].append(spec)
                if STATUS_RANK.get(status, 9) < STATUS_RANK.get(r['status'], 9):
                    r.update(status=status, role=role, spec=spec, size=size or r['size'])

    pled = packet_ledgers()
    for a, (pkt, done, role) in pled.items():
        if a not in rows:
            rows[a] = dict(address=a, size='', role=role, status='spec', specs=[pkt.lower()],
                           spec=pkt.lower())
    owners = read_owners()
    impl = implemented()
    seams = seam_owners()
    ported = set(args.ported)
    for a, r in rows.items():
        if a in seams:
            r['packet'], r['assign'] = seams[a] or '?', 'seam'
        elif r['status'] in ('library', 'out-of-scope'):
            r['packet'], r['assign'] = '-', '-'
        else:
            r['packet'], r['assign'] = rule_owner(r['spec'], r['role'], a) or '?', 'rule'
        if a in pled and r['status'] not in ('library', 'out-of-scope'):
            r['packet'], r['assign'] = pled[a][0], 'packet'
        for lo, hi, pkt in owners:
            if lo <= int(a, 16) <= hi:
                r['packet'], r['assign'] = pkt, 'owners'
        if r['status'] == 'out-of-scope' and r['assign'] == 'seam' and a not in impl:
            pass  # an out-of-scope seam that only a stub implements stays out of scope
        elif r['assign'] == 'seam' and r['packet'] in ported:
            r['status'] = 'ported'
        elif a in pled and pled[a][1]:
            r['status'] = 'ported'
        elif a in impl and r['status'] not in ('library', 'out-of-scope'):
            r['status'] = 'ported'
        elif a in prev and prev[a][3] == 'ported':
            r['status'] = 'ported'

    out = sorted(rows.values(), key=lambda r: int(r['address'], 16))
    cnt = {}
    for r in out:
        cnt[(r['packet'], r['status'])] = cnt.get((r['packet'], r['status']), 0) + 1
    by_pkt = {}
    for (pkt, st), n in cnt.items():
        by_pkt.setdefault(pkt, {})[st] = n
    for pkt in sorted(by_pkt):
        print('%-4s %s' % (pkt, ', '.join('%s=%d' % kv for kv in sorted(by_pkt[pkt].items()))))
    print('rows: %d' % len(out))
    orphans = [r for r in out if r['packet'] == '?']
    for r in orphans:
        print('no owner: %s %s (%s)' % (r['address'], r['role'], r['spec']))
    if args.check:
        return 1 if orphans else 0
    with open(OUT, 'w', encoding='utf-8', newline='\n') as f:
        f.write('address\tsize\trole\tstatus\tpacket\tassign\tspecs\n')
        for r in out:
            f.write('\t'.join([r['address'], r['size'], r['role'].replace('\t', ' '), r['status'],
                               r['packet'], r['assign'], ','.join(sorted(set(r['specs'])))]) + '\n')
    print('wrote', OUT)
    return 0


if __name__ == '__main__':
    sys.exit(main())
