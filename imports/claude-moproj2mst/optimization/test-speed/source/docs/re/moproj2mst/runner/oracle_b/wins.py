import ctypes, ctypes.wintypes as W, sys
u = ctypes.windll.user32; u.SetProcessDPIAware()
def wins(pid=None, visible=True):
    res = []
    @ctypes.WINFUNCTYPE(W.BOOL, W.HWND, W.LPARAM)
    def cb(h, l):
        p = W.DWORD(); u.GetWindowThreadProcessId(h, ctypes.byref(p))
        if (pid is None or p.value == pid) and (not visible or u.IsWindowVisible(h)):
            n = u.GetWindowTextLengthW(h); b = ctypes.create_unicode_buffer(n + 1); u.GetWindowTextW(h, b, n + 1)
            c = ctypes.create_unicode_buffer(256); u.GetClassNameW(h, c, 256)
            r = W.RECT(); u.GetWindowRect(h, ctypes.byref(r))
            res.append((h, p.value, b.value, c.value, (r.left, r.top, r.right, r.bottom)))
        return True
    u.EnumWindows(cb, 0); return res
if __name__ == "__main__":
    pid = int(sys.argv[1]) if len(sys.argv) > 1 else None
    for w in wins(pid): print(w)
