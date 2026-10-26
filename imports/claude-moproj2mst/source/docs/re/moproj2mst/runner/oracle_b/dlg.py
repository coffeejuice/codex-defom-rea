import ctypes, ctypes.wintypes as W, sys, time
u = ctypes.windll.user32; u.SetProcessDPIAware()
def children(h):
    res = []
    @ctypes.WINFUNCTYPE(W.BOOL, W.HWND, W.LPARAM)
    def cb(c, l):
        n = u.GetWindowTextLengthW(c); b = ctypes.create_unicode_buffer(n + 1); u.GetWindowTextW(c, b, n + 1)
        k = ctypes.create_unicode_buffer(256); u.GetClassNameW(c, k, 256)
        res.append((c, u.GetDlgCtrlID(c), k.value, b.value, bool(u.IsWindowVisible(c)))); return True
    u.EnumChildWindows(h, cb, 0); return res
def filedlg(h, path):
    """fill the file-name edit of a common file dialog and press OK"""
    ed = [c for c in children(h) if c[2] == "Edit" and c[4]]
    combo = [c for c in children(h) if c[1] == 0x47c]
    tgt = None
    if combo:
        e = [c for c in children(combo[0][0]) if c[2] == "Edit"]
        tgt = e[0][0] if e else None
    if tgt is None: tgt = ed[-1][0]
    u.SendMessageW(tgt, 0x000C, 0, ctypes.c_wchar_p(path)); time.sleep(0.3)
    ok = u.GetDlgItem(h, 1); u.PostMessageW(ok, 0x00F5, 0, 0)
if __name__ == "__main__":
    h = int(sys.argv[1])
    if len(sys.argv) > 2 and sys.argv[2] == "file": filedlg(h, sys.argv[3])
    elif len(sys.argv) > 2 and sys.argv[2] == "btn": u.PostMessageW(u.GetDlgItem(h, int(sys.argv[3])), 0x00F5, 0, 0)
    else:
        for c in children(h): print(c)
