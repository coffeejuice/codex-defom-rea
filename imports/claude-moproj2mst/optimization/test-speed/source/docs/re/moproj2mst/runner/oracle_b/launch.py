import ctypes, subprocess, sys, os, time
SEM = 0x0001 | 0x0002 | 0x8000
ctypes.windll.kernel32.SetErrorMode(SEM)
EXE = r"C:\Program Files\SFTC\DEFORM\v11.0.2\DEF_GUI_MO_64.exe"
cwd = sys.argv[1]; args = sys.argv[2:]
p = subprocess.Popen([EXE] + args, cwd=cwd, shell=False, creationflags=0x00000008 | 0x00000200)  # DETACHED|NEW_GROUP
print(p.pid)
