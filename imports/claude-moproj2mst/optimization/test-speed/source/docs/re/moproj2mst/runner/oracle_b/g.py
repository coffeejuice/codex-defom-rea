# high-level helpers: g.py <cmd> ...
import sys, time, re, subprocess
import os; sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import wins, inp, dlg, cap
import ctypes
def F(): return ctypes.windll.user32.GetDpiForSystem() / 168.0   # constants below were measured at 168 dpi
def sc(v): return int(round(v * F()))
def main_hwnd(pid):
    for w in wins.wins(pid):
        if "DEFORM Multiple Operation" in w[2]: return w[0]
def popups(pid): return [w for w in wins.wins(pid) if w[3] in ("QPopup", "QMenu") or w[2] == "DEF_GUI_MO_64"]
VK_DOWN, VK_RETURN, VK_ESC = 0x28, 0x0D, 0x1B
APP_F = 1.0
FILE_ITEMS = {"new": 1, "open": 2, "save": 3, "close": 8}
def menu(pid, x, item):
    """open the File menu (menu-bar click at client (15, 8)) and pick an item with Down x n + Enter"""
    h = main_hwnd(pid)
    import ctypes.wintypes as W
    pt = W.POINT(15, 8); ctypes.windll.user32.ClientToScreen(h, ctypes.byref(pt))
    for _ in range(3):
        inp.click(h, pt.x, pt.y)
        for _ in range(10):
            time.sleep(0.4); pp = popups(pid)
            if pp: break
        if pp: break
    global APP_F
    APP_F = (pp[0][4][2] - pp[0][4][0]) / 290.0   # File popup is 290 px wide at the reference scale
    p = pp[0][0]
    for _ in range(FILE_ITEMS[item]): inp.key(p, VK_DOWN); time.sleep(0.1)
    inp.key(p, VK_RETURN); time.sleep(1.5)
def openp(pid, path):
    menu(pid, 0, "open"); time.sleep(1)
    d = [w for w in wins.wins(pid) if w[3] == "#32770"][0]; dlg.filedlg(d[0], path); time.sleep(5)
def shot(pid, out, scale=2):
    h = main_hwnd(pid); print(cap.cap(h, out, scale))
if __name__ == "__main__":
    c = sys.argv[1]; pid = int(sys.argv[2])
    if c == "open": openp(pid, sys.argv[3])
    elif c == "save": menu(pid, 0, "save")
    elif c == "shot": shot(pid, sys.argv[3], int(sys.argv[4]) if len(sys.argv) > 4 else 2)
    for w in wins.wins(pid): print(w)
