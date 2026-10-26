import ctypes, ctypes.wintypes as W, sys, time
u = ctypes.windll.user32; u.SetProcessDPIAware()
WM_MOUSEMOVE, WM_LBUTTONDOWN, WM_LBUTTONUP, WM_LBUTTONDBLCLK = 0x200, 0x201, 0x202, 0x203
def _lp(x, y): return (y << 16) | (x & 0xffff)
def deep(hwnd, sx, sy):
    """deepest visible child of hwnd containing screen point"""
    while True:
        p = W.POINT(sx, sy); u.ScreenToClient(hwnd, ctypes.byref(p))
        c = u.ChildWindowFromPointEx(hwnd, p, 1 | 2 | 4)  # skip invisible, disabled, transparent
        if not c or c == hwnd: return hwnd
        hwnd = c
def click(hwnd, sx, sy, dbl=False):
    """sx, sy: screen coords; posted to the deepest child in its client coords"""
    hwnd = deep(hwnd, sx, sy)
    p = W.POINT(sx, sy); u.ScreenToClient(hwnd, ctypes.byref(p)); lp = _lp(p.x, p.y)
    u.PostMessageW(hwnd, WM_MOUSEMOVE, 0, lp); time.sleep(0.05)
    u.PostMessageW(hwnd, WM_LBUTTONDOWN, 1, lp); time.sleep(0.05); u.PostMessageW(hwnd, WM_LBUTTONUP, 0, lp)
    if dbl:
        time.sleep(0.05); u.PostMessageW(hwnd, WM_LBUTTONDBLCLK, 1, lp); time.sleep(0.05); u.PostMessageW(hwnd, WM_LBUTTONUP, 0, lp)
def key(hwnd, vk, ch=None):
    u.PostMessageW(hwnd, 0x100, vk, 1)
    if ch: u.PostMessageW(hwnd, 0x102, ord(ch), 1)
    u.PostMessageW(hwnd, 0x101, vk, 0xC0000001)
def text(hwnd, s):
    for c in s: u.PostMessageW(hwnd, 0x102, ord(c), 1)
def rclick(hwnd, sx, sy):
    hwnd = deep(hwnd, sx, sy)
    p = W.POINT(sx, sy); u.ScreenToClient(hwnd, ctypes.byref(p)); lp = _lp(p.x, p.y)
    u.PostMessageW(hwnd, WM_MOUSEMOVE, 0, lp); time.sleep(0.05)
    u.PostMessageW(hwnd, 0x204, 2, lp); time.sleep(0.05); u.PostMessageW(hwnd, 0x205, 0, lp)


if __name__ == "__main__":
    cmd = sys.argv[1]; h = int(sys.argv[2])
    if cmd in ("click", "dbl"): click(h, int(sys.argv[3]), int(sys.argv[4]), cmd == "dbl")
    elif cmd == "rclick": rclick(h, int(sys.argv[3]), int(sys.argv[4]))
    elif cmd == "key": key(h, int(sys.argv[3], 0), sys.argv[4] if len(sys.argv) > 4 else None)
    elif cmd == "text": text(h, sys.argv[3])
    elif cmd == "settext": u.SendMessageW(h, 0x000C, 0, ctypes.c_wchar_p(sys.argv[3]))
    elif cmd == "close": u.PostMessageW(h, 0x10, 0, 0)
