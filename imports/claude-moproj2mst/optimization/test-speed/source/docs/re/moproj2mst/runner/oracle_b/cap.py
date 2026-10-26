import ctypes, ctypes.wintypes as W, sys, zlib, struct
u = ctypes.windll.user32; g = ctypes.windll.gdi32
u.SetProcessDPIAware()
def png(path, w, h, bgra):
    raw = b"".join(b"\x00" + bytes(x for i in range(0, w*4, 4) for x in (row[i+2], row[i+1], row[i])) for row in (bgra[y*w*4:(y+1)*w*4] for y in range(h)))
    def ch(t, d): c = struct.pack(">I", len(d)) + t + d; return c + struct.pack(">I", zlib.crc32(t + d) & 0xffffffff)
    open(path, "wb").write(b"\x89PNG\r\n\x1a\n" + ch(b"IHDR", struct.pack(">IIBBBBB", w, h, 8, 2, 0, 0, 0)) + ch(b"IDAT", zlib.compress(raw, 6)) + ch(b"IEND", b""))
def cap(hwnd, path, scale=1):
    r = W.RECT(); u.GetWindowRect(hwnd, ctypes.byref(r)); w, h = r.right - r.left, r.bottom - r.top
    hdc = u.GetDC(0); mdc = g.CreateCompatibleDC(hdc); bmp = g.CreateCompatibleBitmap(hdc, w, h); g.SelectObject(mdc, bmp)
    g.BitBlt(mdc, 0, 0, w, h, hdc, r.left, r.top, 0x00CC0020)
    class BIH(ctypes.Structure): _fields_ = [("biSize", W.DWORD), ("biWidth", W.LONG), ("biHeight", W.LONG), ("biPlanes", W.WORD), ("biBitCount", W.WORD), ("biCompression", W.DWORD), ("biSizeImage", W.DWORD), ("a", W.LONG), ("b", W.LONG), ("c", W.DWORD), ("d", W.DWORD)]
    bi = BIH(ctypes.sizeof(BIH), w, -h, 1, 32, 0, 0, 0, 0, 0, 0); buf = ctypes.create_string_buffer(w*h*4)
    g.GetDIBits(mdc, bmp, 0, h, buf, ctypes.byref(bi), 0)
    g.DeleteObject(bmp); g.DeleteDC(mdc); u.ReleaseDC(0, hdc)
    data = buf.raw
    if scale > 1:
        nw, nh = w // scale, h // scale
        data = b"".join(data[(y*scale)*w*4 + x*scale*4:(y*scale)*w*4 + x*scale*4 + 4] for y in range(nh) for x in range(nw)); w, h = nw, nh
    png(path, w, h, data); return (r.left, r.top, w, h)
if __name__ == "__main__":
    print(cap(int(sys.argv[1]), sys.argv[2], int(sys.argv[3]) if len(sys.argv) > 3 else 1))
