"""Size the DEF_PRE_64 batch-mode port from the Ghidra function list.

Inputs (in the Ghidra work dir, deform-rea-work/out):
  pre64_funcs.tsv         FuncList.java: entry, size, name, C:callers, E:callees, S:strings
  bm_cmd.c                Decomp.java output for FUN_140378e00 (command keyword handler)
  ledger/ptrtables.tsv    IndirectRefs.java: runs of function pointers in data (vtables, tables)
  ledger/addrtaken.tsv    IndirectRefs.java: per function, non-call references (F: function,
                          T: pointer run, D: data item, R: other data) and indirect calls (V:slot)
  ledger/dataitems.tsv    IndirectRefs.java: data items holding function pointers
and keyword_registry.tsv next to this script.
Output: pre64_batch_ledger.tsv next to this script, plus a per-phase summary on stdout.

Edges followed from a function:
  direct    call instructions (FuncList callees)
  fnptr     the address of a function taken in code (callbacks, Fortran EXTERNAL arguments)
  table     a referenced pointer table that is not a vtable, including arrays of records with
            function pointers (DXF entity table)
  vtable    slot k of every live vtable when the function contains "call [reg+8k]"; a vtable is
            live once a function in this or an earlier phase references it (constructors and
            destructors do)
Roots besides the phase entry points: the C++ static initialisers (_initterm tables), and the
keyword registry handler columns 2 (KEY write) and 3 (table labels), which have no caller.
Run with --direct for the old direct-call-only numbers (no ledger is written then).
"""
import bisect, os, re, sys
from collections import Counter

WORK = os.environ.get('DEFORM_RE_WORK', 'C:/Users/alext/Projects/deform-rea-work/out/')
HERE = os.path.dirname(os.path.abspath(__file__))
DIRECT_ONLY = '--direct' in sys.argv

F = {}
for line in open(WORK + 'pre64_funcs.tsv', encoding='latin1'):
    p = line.rstrip('\n').split('\t')
    if len(p) < 6 or not p[1].isdigit():
        continue
    F[p[0]] = dict(name=p[2], size=int(p[1]),
                   callers=[x for x in p[3][2:].split(',') if x],
                   callees=[x for x in p[4][2:].split(',') if x])

# ---- indirect-reference sweep ---------------------------------------------------------------
RUNS = {}       # start -> (name, locator, [targets])
for line in open(WORK + 'ledger/ptrtables.tsv'):
    p = line.rstrip('\n').split('\t')
    RUNS[p[0]] = (p[2], p[3], p[4].split(','))
for line in open(WORK + 'ledger/dataitems.tsv'):
    p = line.rstrip('\n').split('\t')
    RUNS[p[0]] = (p[2], '', p[3].split(','))
REFS = {}
for line in open(WORK + 'ledger/addrtaken.tsv'):
    f, r = line.rstrip('\n').split('\t')
    REFS[f] = r.split(',')

# Classes that belong to the licence, socket, job-file and process-monitor code (skipped).
SKIP_CLASS = ('CLManager', 'CDEFORMLMObject', 'CDFMBaseSocket', 'CJobInformationFile', 'sdspm',
              'Sdspm', 'SfLicenseLogData')


def is_vtable(r):
    return 'vftable' in RUNS[r][0] or 'vftable' in RUNS[r][1]


def skipped(r):
    return any(s in RUNS[r][0] or s in RUNS[r][1] for s in SKIP_CLASS)


# C++ dynamic initialisers and terminators, called by _cinit/doexit through the _initterm tables.
# They sit after the CRT (0x140501000-0x140504fff) but are application code.
STARTUP_RUNS = {r for r in RUNS if 0x1405056c0 <= int(r, 16) < 0x140505930}
STATIC_INIT = {t for r in STARTUP_RUNS for t in RUNS[r][2]}

REGISTRY = 0x140544400          # 304 records of 0x30 bytes: id, name, group, flag, h1, h2, h3
REG_RUNS = {r for r in RUNS if REGISTRY <= int(r, 16) < REGISTRY + 0x130 * 0x30}


def islib(a):
    if a in STATIC_INIT:
        return False
    v = int(a, 16)
    return (0x140090000 <= v < 0x140130000      # licence, Sentinel, sockets, XML config
            or 0x140470000 <= v < 0x1404c0000   # process monitor, MSVC CRT/STL, Fortran runtime
            or v >= 0x1404e8000)                # import thunks, SEH unwind funclets


RUN_STARTS = sorted(int(r, 16) for r in RUNS)


def record_array(t):
    """Runs of an array of records referenced at t: the first run within 0x20 bytes, then
    every following run at the same stride (e.g. DXF name/handler table at 0x14062bdb0)."""
    v = int(t, 16)
    i = bisect.bisect_left(RUN_STARTS, v)
    if i >= len(RUN_STARTS) or RUN_STARTS[i] - v > 0x20:
        return []
    out = [RUN_STARTS[i]]
    if i + 1 < len(RUN_STARTS):
        stride = RUN_STARTS[i + 1] - RUN_STARTS[i]
        j = i + 1
        while j < len(RUN_STARTS) and RUN_STARTS[j] - RUN_STARTS[j - 1] == stride and stride <= 0x100:
            out.append(RUN_STARTS[j])
            j += 1
    return ['%x' % x for x in out]


# Per function: indirect targets (kind, target), vtables it instantiates, slots it calls.
IND, INST, SLOTS = {}, {}, {}
for f, refs in REFS.items():
    ind, inst, slots = [], set(), set()
    for x in refs:
        k, t = x[0], x[2:]
        if k == 'F':
            ind.append(('fnptr', t))
        elif k in 'TD' and t in RUNS:
            if skipped(t) or t in STARTUP_RUNS:
                continue
            if is_vtable(t):
                inst.add(t)
            elif t not in REG_RUNS:
                ind += [('table', g) for g in RUNS[t][2]]
        elif k == 'R' and int(t, 16) not in (REGISTRY, REGISTRY + 8):
            for r in record_array(t):
                if not is_vtable(r) and not skipped(r) and r not in REG_RUNS and r not in STARTUP_RUNS:
                    ind += [('table', g) for g in RUNS[r][2]]
        elif k == 'V' and t != 'r':
            slots.add(int(t, 16) // 8)
    IND[f], INST[f], SLOTS[f] = ind, inst, slots

# The keyword reader and command dispatcher re-enter each other (KFREAD, EXECMD...);
# they are not expanded when reached from elsewhere, so one command's closure does not swallow
# the whole program. They are still members of the phase that reaches them, and are expanded
# when given as roots.
STOP = {'1401b5f00', '1401b6520', '140378e00', '14037e530'}
# The command dispatcher itself is a P3 member, but its cases belong to the command phases.
NOEXPAND = {'140378e00'}


def closure(roots, live=None, kinds=()):
    """Functions reachable from roots by direct calls plus the indirect edge kinds given
    ('fnptr', 'table'); with live (a set of vtables) all indirect kinds and vtable slots."""
    roots = [r.lower() for r in roots]
    if live is not None:
        kinds = ('fnptr', 'table')
    seen, tried, st = set(), set(), list(roots)
    slots = set()
    while True:
        while st:
            x = st.pop()
            if x in tried:
                continue
            tried.add(x)
            if x not in F or islib(x):
                continue
            seen.add(x)
            if x in NOEXPAND or (x in STOP and x not in roots):
                continue
            st += [c for c in F[x]['callees'] if c not in tried]
            st += [t for k, t in IND.get(x, []) if k in kinds and t not in tried]
            if live is not None:
                slots |= SLOTS.get(x, set())
        if live is None:
            return seen
        for v in live:
            tg = RUNS[v][2]
            st += [tg[s] for s in slots if s < len(tg) and tg[s] not in tried]
        if not st:
            return seen


names, regcol = {}, {2: [], 3: []}
DEFAULT_HANDLERS = {'1402f7590', '1402f75d0', '1402f7610'}
for line in open(os.path.join(HERE, 'keyword_registry.tsv')):
    p = line.rstrip('\n').split('\t')
    if p[0].isdigit():
        names[int(p[0])] = p[1]
        for c in (2, 3):
            h = p[4 + c][2:]
            if h not in DEFAULT_HANDLERS and h != '0':  # ZSTR has no column 3 handler
                regcol[c].append(h)

# Split FUN_140378e00 into its switch cases: ids 0..0x11e at 6-space indent, ids 0x11f+ at 4.
L = open(WORK + 'bm_cmd.c').read().split('\n')
cases, cur = {}, None
for line in L:
    m = re.match(r'^(      |    )(case (\w+)|default):', line)
    if m:
        if m.group(3) is None:
            cur = None
        else:
            k = int(m.group(3), 0)
            cur = k if len(m.group(1)) == 6 else 0x11f + k
            cases[cur] = []
        continue
    if line.startswith('    switch(local_70)'):
        cur = None
    if cur is not None:
        cases[cur] += re.findall(r'FUN_(14[0-9a-f]{7})', line)
CASES = {names.get(k, str(k)): v for k, v in cases.items()}


def K(*ks):
    return [a for k in ks for a in CASES[k]]


MAIN = '14001c9f0'
MENUS = ['140014ba0', '140017540', '1400157c0', '14001a050', '140019c20', '14000afd0', '14000a370', '14000ac60']
STARTUP = sorted(a for a in STATIC_INIT if a in F)

# Phase -> (title, roots, roots whose closure is cut out of main's). Order decides first_phase.
PHASES = [
    ('P1', '-m CLI, model skeleton, static initialisers', ['14001b7b0', '14001c500'] + STARTUP, []),
    ('P2', 'DBREAD + KEY write (KFWRIT), registry write handlers',
     K('DBREAD', 'KFWRIT') + ['1401c5810'] + regcol[2], []),
    ('P3', 'KEY read, all keyword groups',
     ['1401b5f00', '1401b6520', '14039aa90', '14038b8a0', '140388fb0', '140384a50', '140383120',
      '14037e530', '1402f76c0', '140378e00']
     + K('PROBID', 'RUNTYP', 'KFREAD', 'TLOC', 'DSTROKE', 'EXECMD', 'ARMCTL'), []),
    ('P4', 'GENDB / DB write', K('GENDB') + ['1401b9940', '14000a370'], []),
    ('P5', 'menu shell, init, DEF_MULTI.INI, KEY save',
     [MAIN, '14000ac60', '140009c50', '140007d30'], ['14001b7b0', '14001c500'] + MENUS),
    ('P6', 'object commands', K('OBJPOS', 'CPYOBJ', 'SVSCAL', 'DOESCL', 'WRTOBJ', 'GENAXS', 'GENBCC',
                                'GENSUB', 'TCYCLE'), []),
    ('P7', 'contact generation', K('INICTC', 'GENCTC'), []),
    ('P8', 'external-tool commands', K('DEFAMG', 'REMESH', 'BRDEXT', 'DEFINT', 'DEFFRZ', 'MORP',
                                       'GFREAD', 'GENINT', 'IMPLUB', 'IMPEQP'), []),
    ('P9', '2D->3D and file-input menu', K('CNVT3D') + ['14000afd0', '140014ba0', '14000b170'], []),
    ('P10', 'interactive edit menus 3-6, registry table labels',
     ['140017540', '1400157c0', '14001a050', '140019c20'] + regcol[3], []),
]


def phase_set(roots, excl, live):
    if not excl:
        return closure(roots, live)
    # P5 is main minus what main reaches only through the -m CLI and the menu entries.
    s = closure([MAIN], live) - closure(excl, live)
    return s | closure([r for r in roots if r != MAIN], live)


# Live vtables grow phase by phase: a phase's virtual calls reach the vtables referenced in that
# phase or an earlier one (objects are built before they are used), so virtual methods are
# counted in the first phase that both builds the object and calls the slot.
live = None if DIRECT_ONLY else set()


def phase_live(roots, excl):
    while True:
        s = phase_set(roots, excl, live)
        nl = {v for a in s for v in INST.get(a, ())}
        if nl <= live:
            return s
        live.update(nl)


AREAS = [(0x140001000, 0x140020000, 'text_pre_ui'), (0x140020000, 0x140090000, 'fortran_pre_kernels'),
         (0x140130000, 0x140310000, 'cpp_data_model'), (0x140310000, 0x140380000, 'fortran_contact_cmd'),
         (0x140380000, 0x1403b0000, 'kw_group_handlers_dds'), (0x1403b0000, 0x1403c0000, 'datarm23_dbops'),
         (0x1403c0000, 0x140470000, 'utils_io_units_iges'), (0x1404c0000, 0x1404e8000, 'fortran_fem_geom'),
         (0x140501000, 0x140505000, 'static_init')]


def area(a):
    v = int(a, 16)
    return next((n for lo, hi, n in AREAS if lo <= v < hi), 'other')


direct_union = set()
for ph, title, roots, excl in PHASES:
    direct_union |= phase_set(roots, excl, None)

first, done, via = {}, set(), {}
print('%-4s %-52s %6s %6s %6s %6s' % ('', '', 'reach', 'new', 'newKB', '+indir'))
for ph, title, roots, excl in PHASES:
    s = phase_set(roots, excl, None) if live is None else phase_live(roots, excl)
    new = s - done
    done |= s
    for a in new:
        first[a] = ph
    print('%-4s %-52s %6d %6d %6d %6d' % (ph, title, len(s), len(new),
                                         sum(F[a]['size'] for a in new) // 1024,
                                         sum(1 for a in new if a not in direct_union)))
# via: the cheapest edge set that reaches the function from the batch roots: direct calls, then
# + function pointers, + pointer tables, + vtable slots.
allroots = [r for _, _, rs, _ in PHASES for r in rs]
reach_by = [('direct', direct_union)]
if live is not None:
    reach_by += [('fnptr', closure(allroots, None, ('fnptr',))),
                 ('table', closure(allroots, None, ('fnptr', 'table')))]
for a in first:
    via[a] = next((k for k, u in reach_by if a in u), 'vtable')
napp = sum(1 for a in F if not islib(a))
print('batch scope %d of %d application functions (%d KB); direct calls alone: %d'
      % (len(done), napp, sum(F[a]['size'] for a in done) // 1024, len(direct_union)))
if live is not None:
    print('reached via:', dict(Counter(via.values())))
    print('live vtables: %d of %d (skipped classes: %d)'
          % (len(live), sum(1 for r in RUNS if is_vtable(r)), sum(1 for r in RUNS if is_vtable(r) and skipped(r))))
    regs = set(regcol[2] + regcol[3])
    others = closure([r for _, _, rs, _ in PHASES for r in rs if r not in regs])
    print('registry handlers: column 2 %d, column 3 %d; not reached by direct calls from the '
          'other roots: %d' % (len(regcol[2]), len(regcol[3]), len(regs - others)))
    regcalls = sorted(a for a in done if 'V:r' in REFS.get(a, []))
    print('in-scope functions with "call reg" sites (targets not resolved statically): %d' % len(regcalls))
    for a in regcalls:
        print('   ', a, F[a]['name'])
    referenced = set()
    for a, refs in REFS.items():
        referenced |= {x[2:] for x in refs if x[0] == 'F'}
    for r in RUNS.values():
        referenced |= set(r[2])
    orphans = [a for a in F if not islib(a) and not F[a]['callers'] and a not in referenced]
    print('application functions with no caller and no reference (dead or unrecognised): %d' % len(orphans))
    print('application functions outside the batch scope: %d' % (napp - len(done)))
for k, v in sorted(CASES.items(), key=lambda kv: -len(closure(kv[1]))):
    print('  %-8s %5d' % (k, len(closure(v))))

if not DIRECT_ONLY:
    with open(os.path.join(HERE, 'pre64_batch_ledger.tsv'), 'w', newline='\n') as out:
        out.write('address\tsize\tarea\tfirst_phase\tvia\tstatus\n')
        for a in sorted(first):
            out.write('0x%s\t%d\t%s\t%s\t%s\ttodo\n' % (a, F[a]['size'], area(a), first[a], via[a]))
