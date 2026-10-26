"""run.py <case dir>...: open each scratch project in the running DEF_GUI_MO_64, File > Save, File > Close (no save)."""
import os, sys, time, subprocess
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import wins, inp, dlg, cap, g
ROOT = r"C:\Users\alext\Projects\deform-rea-work\gui_mo\oracle_b"
def pid():
    out = subprocess.run(["tasklist", "/FI", "IMAGENAME eq DEF_GUI_MO_64.exe", "/FO", "CSV", "/NH"], capture_output=True, text=True).stdout
    for l in out.splitlines():
        if "DEF_GUI_MO_64" in l: return int(l.split('","')[1])
def others(p): return [w for w in wins.wins(p) if "DEFORM Multiple Operation" not in w[2] and w[3] != "SysShadow"]
def wait_quiet(p, t=60):
    for _ in range(int(t * 2)):
        o = others(p)
        if o: return o
        time.sleep(0.5)
    return []
FRAC = {("Question", 3, "no"): (0.547, 0.84), ("Question", 2, "yes"): (0.38, 0.79), ("Question", 2, "no"): (0.76, 0.79),
        ("Warning", 2, "no"): (0.83, 0.82), ("Warning", 2, "yes"): (0.53, 0.82)}
def answer(p, w, choice):
    wd, ht = w[4][2] - w[4][0], w[4][3] - w[4][1]
    nb = 3 if (w[2] == "Question" and wd / g.APP_F > 440) else 2   # Yes/No/Cancel box 506 px wide, Yes/No 371 (reference scale)
    pos = FRAC.get((w[2], nb, choice))
    if not pos:
        cap.cap(w[0], os.path.join(ROOT, "_tools", "unexpected_%d.png" % w[0])); raise SystemExit("unexpected dialog %r" % (w,))
    inp.click(w[0], w[4][0] + int(pos[0] * wd), w[4][1] + int(pos[1] * ht)); time.sleep(1.5)
def run(case):
    d = os.path.join(ROOT, case); mp = [f for f in os.listdir(d) if f.lower().endswith(".moproj")][0]
    p = pid(); assert p, "GUI not running"
    if others(p): raise SystemExit("dialog open before start: %r" % others(p))
    g.menu(p, 0, "open")  # File > Open
    o = wait_quiet(p, 10)
    if o and o[0][2] == "Question": answer(p, o[0], "yes"); o = wait_quiet(p, 10)
    if o and o[0][2] == "Question": answer(p, o[0], "no"); o = wait_quiet(p, 10)
    o = [w for w in o if w[3] == "#32770"]
    if not o: raise SystemExit("no open dialog: %r" % others(p))
    dlg.filedlg(o[0][0], os.path.join(d, mp)); time.sleep(3)
    for _ in range(120):
        t = wins.wins(p); o = others(p)
        if o: cap.cap(o[0][0], os.path.join(d, "open_dialog.png")); raise SystemExit("dialog after open: %r" % o)
        if any(w[2].startswith(mp[:-7] + " - ") for w in t): break
        time.sleep(0.5)
    else: raise SystemExit("project did not open")
    time.sleep(2)
    g.shot(p, os.path.join(d, "opened.png"), 2)
    mst = os.path.join(d, mp[:-7] + ".MST"); before = os.path.getmtime(mst) if os.path.exists(mst) else 0
    g.menu(p, 0, "save")  # File > Save
    for _ in range(120):
        o = others(p)
        if o: cap.cap(o[0][0], os.path.join(d, "save_dialog.png")); raise SystemExit("dialog after save: %r" % o)
        if os.path.exists(mst) and os.path.getmtime(mst) != before: break
        time.sleep(0.5)
    else: raise SystemExit("MST not written")
    time.sleep(2)
    g.menu(p, 0, "close")  # File > Close
    o = wait_quiet(p, 10)
    if o: answer(p, o[0], "no")
    print(case, "saved", os.path.getsize(mst))
def close():
    p = pid()
    if not any(" - DEFORM Multiple Operation" in w[2] for w in wins.wins(p)): return
    g.menu(p, 0, "close"); o = wait_quiet(p, 5)
    if o: answer(p, o[0], "no")
EXE = r"C:\Program Files\SFTC\DEFORM\v11.0.2\DEF_GUI_MO_64.exe"
def fresh(case):
    """one GUI process per case: the GUI keeps session state between opens (e.g. the MultiBlow
    root template 0x143d9ef10), so a case must not be opened after another one in the same process"""
    import ctypes
    if pid(): raise SystemExit("a DEF_GUI_MO_64 process is already running")
    ctypes.windll.kernel32.SetErrorMode(0x0001 | 0x0002 | 0x8000)
    proc = subprocess.Popen([EXE], cwd=os.path.dirname(os.path.abspath(__file__)), shell=False)
    for _ in range(120):
        time.sleep(0.5)
        if any(w[2] == "DEFORM Multiple Operation" for w in wins.wins(proc.pid)) and not others(proc.pid): break
        if any(" - DEFORM Multiple Operation" in w[2] for w in wins.wins(proc.pid)):
            raise SystemExit("the GUI opened a project by itself at start-up")
    else: raise SystemExit("GUI did not start")
    time.sleep(3)
    run(case)
    h = g.main_hwnd(proc.pid); inp.u.PostMessageW(h, 0x10, 0, 0)   # WM_CLOSE with no project open
    try: proc.wait(60)
    except subprocess.TimeoutExpired: raise SystemExit("GUI did not exit: %r" % others(proc.pid))
    print(case, "GUI exit code", proc.returncode)
if __name__ != "__main__": pass
elif sys.argv[1:2] == ["--close"]: close()
elif sys.argv[1:2] == ["--same-process"]:   # old mode, for re-saves only
    for c in sys.argv[2:]: run(c)
    close()
else:
    for c in sys.argv[1:]: fresh(c)
