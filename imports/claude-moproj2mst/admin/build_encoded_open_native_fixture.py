"""Derive only a harmless looping native fixture; no original execution."""
from pathlib import Path
import subprocess,sys
R=Path(__file__).resolve().parent
s=(R/'build_outer_loader_native_prologue_fixture.py').read_text()
s=s.replace("OUT=ROOT/'scratch/cp210p01'", "OUT=ROOT/'scratch/cp213e02'")
s=s.replace('int g_fail=5;', '''int g_fail=5;
unsigned char g_main[0x2000];
struct H {int ref,capacity,length,pad;const wchar_t *data;unsigned short flags;} g_header;
const wchar_t g_path[]=L"C:\\\\PROBLEM\\\\p213a\\\\Gear_blank.moproj";''')
s=s.replace(' if(g_fail<0||g_fail>5)return 74;', ''' if(g_fail<0||g_fail>7)return 74;
 if(g_fail==6)return 42;
 if(g_fail==7)for(;;)Sleep(100);
 g_header.ref=1;g_header.capacity=34;g_header.length=34;g_header.data=g_path;
 *(void**)(g_main+0x18d8)=&g_header;''')
s=s.replace(' *(volatile uintptr_t*)0x58=value;', ' for(;;) Sleep(100);')
s=s.replace("asm=['EXTERN g_fail:DWORD'", "asm=['EXTERN g_main:BYTE','EXTERN g_fail:DWORD'")
s=s.replace("'mov ecx,700000h','mov edx,7018d8h'", "'lea rcx,g_main','lea rdx,g_main+18d8h'")
s=s.replace("['cmp rcx,700000h','jne register_bad','cmp rdx,7018d8h','jne register_bad',", "[")
s=s.replace("asm += ['call '+name", "asm += (['mov ecx,100h'] if name=='acceptance' else []) + ['call '+name")
# Execute trusted fixture builder text with its own __file__ context. All outputs fixed under new scratch.
exec(compile(s,str(R/'build_encoded_open_native_fixture.py'),'exec'),{'__file__':str(R/'build_encoded_open_native_fixture.py'),'__name__':'__main__'})

