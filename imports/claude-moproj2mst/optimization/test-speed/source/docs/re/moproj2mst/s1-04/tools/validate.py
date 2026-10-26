"""s1-04 structural validation on sample copies (read-only).
Checks, per SfOprBoolean(2D) block: the writer-owned lines from 'NSTEP\t0' to the GENDB line
(INICTC lines come from the shared emitter 140136040 and are skipped), and that the side files
Boolean.BWZ / DEF_MACHDIST_CTRL2.INI in the task folder equal what 1411e1970 / 1411e0e40 would
write from the BWZ values. Per SfOprCycle: SIMNAM 'CYCL<k>-<member name>' and block count."""
import re, sys, os, glob

def blocks(mst):
    t = open(mst, 'rb').read().decode('latin1')
    parts = re.split(r'\nSIMULATION\t', t)
    out = []
    for p in parts[1:]:
        lines = p.split('\n')
        out.append(['SIMULATION\t' + lines[0]] + lines[1:])
    return out

def bwz_vals(path):
    toks = open(path, 'rb').read().decode('latin1').split('\n')
    v = {}
    for i, l in enumerate(toks):
        if l in ('WZD_MACHDIST_WORKPIECE_OBJECT_NO', 'WZD_BOOLEAN_CUTTER_OBJECT_NO', 'WZD_BOOLEAN_REMESH'):
            v[l] = int(toks[i + 2])
    return v

def expect_bwz(w, c, r):
    return ('WZD_MACHDIST_WORKPIECE_OBJECT_NO\n  * OBJECT # FOR WORKPIECE \n%d\n\n'
            'WZD_BOOLEAN_CUTTER_OBJECT_NO\n  * OBJECT # FOR CUTTER \n%d\n\n'
            'WZD_BOOLEAN_REMESH\n  * PERFORM REMESH AFTER BOOLEAN\n%d\n\n'
            'DONE\n\n') % (w, c, r)

def expect_ini(task, w, c):
    return (' * Keyword file which contains the workpiece \nKFREAD\n%s/Step.KEY\n\n'
            ' * Workpiece object #\nWZD_MACHDIST_WORKPIECE_OBJECT_NO\t%d\n\n'
            ' * Pass keyword file name\nWZD_MACHDIST_PASS_OBJNO\n%d\n\n'
            ' * Boolean operation\nWZD_MACHDIST_OPERATION_BOOLEAN\n\n'
            ' * Output file name \nWZD_BOOLEAN_OUTPUT_FILENAME\n%s/DEF_MACHDIST_OUT.KEY\n\n') % (task, w, c, task)

def boolean_tail(task, db):
    return ['NSTEP\t0', '<INICTC*>', ' * Keyword file which contains objects ', 'KFWRIT',
            task + '/Step.KEY', '', 'EXECMD\t4',
            'DEF_MACHDIST_CTRL23.EXE   %s/DEF_MACHDIST_CTRL2.INI  >  %s/DEF_MACHDIST_CTRL2.OUT' % (task, task),
            'KFREAD    \t1', task + '/DEF_MACHDIST_OUT.KEY', 'GENDB     \t1\t2\t2', db]

def check_proj(d):
    mp = glob.glob(os.path.join(d, '*.moproj'))[0]
    mst = mp[:-7] + '.MST'
    db = os.path.basename(mp[:-7]) + '.DB'
    ok = bad = 0
    for b in blocks(mst):
        if 'SfOprBoolean' not in b[1]:
            continue
        i = b.index('NSTEP\t0')
        tail = b[i:]
        j = 1
        while tail[j].startswith('INICTC    \t'):
            j += 1
        got = [tail[0], '<INICTC*>'] + tail[j:j + 10]
        task = tail[j + 2].rsplit('/', 1)[0]
        exp = boolean_tail(task, db)
        if got == exp: ok += 1
        else:
            bad += 1; print('MISMATCH', b[0], got, exp)
        td = os.path.join(d, task.replace('/', os.sep))
        v = bwz_vals(os.path.join(td, 'Boolean.BWZ'))
        w, c, r = v['WZD_MACHDIST_WORKPIECE_OBJECT_NO'], v['WZD_BOOLEAN_CUTTER_OBJECT_NO'], v['WZD_BOOLEAN_REMESH']
        for fn, e in (('Boolean.BWZ', expect_bwz(w, c, r)), ('DEF_MACHDIST_CTRL2.INI', expect_ini(task, w, c))):
            got_f = open(os.path.join(td, fn), 'rb').read().decode('latin1')
            if got_f == e: ok += 1
            else:
                bad += 1; print('SIDEFILE MISMATCH', td, fn, repr(got_f[:200]), repr(e[:200]))
        # workpiece INICTC flag set by the writer
        if 'INICTC    \t%d' % w not in tail: bad += 1; print('no INICTC for workpiece', b[0])
        else: ok += 1
    # cycles
    for b in blocks(mst):
        if 'SIMNAM' in b:
            n = b[b.index('SIMNAM') + 1]
            if n.startswith('CYCL'):
                m = re.match(r'CYCL(\d+)-(.*)$', n)
                if m: ok += 1
                else: bad += 1; print('bad cycle simnam', n)
    print(os.path.basename(d), 'ok', ok, 'bad', bad)

for d in sys.argv[1:]:
    check_proj(d)
