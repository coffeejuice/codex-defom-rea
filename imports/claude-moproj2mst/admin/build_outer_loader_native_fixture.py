"""Build only the fixed disposable CP210 native Frida fixture; never DEFORM."""
from pathlib import Path
import hashlib,json,os,struct,subprocess
ROOT=Path(__file__).resolve().parent.parent
OUT=ROOT/'scratch/cp210n03'
OUT.mkdir(parents=True,exist_ok=False)
VC=Path(r'C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/VC/Auxiliary/Build/vcvars64.bat')
r=subprocess.run(f'cmd.exe /d /s /c ""{VC}" >nul && set"',capture_output=True,text=True,check=True)
env=dict(os.environ)
for line in r.stdout.splitlines():
 k,sep,v=line.partition('=')
 if sep and k:
  for old in list(env):
   if old.lower()==k.lower():del env[old]
  env[k]=v
code=r'''#include <windows.h>
#include <stdlib.h>
#include <stdint.h>
int g_fail=5;
extern uintptr_t synthetic_open(void);
int main(int argc,char **argv){
 SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX|SEM_NOOPENFILEERRORBOX);
 if(argc!=2)return 73;
 g_fail=atoi(argv[1]);
 if(g_fail<0||g_fail>5)return 74;
 volatile uintptr_t value=synthetic_open();
 if((g_fail==5&&value!=0x100)||(g_fail<5&&value!=0))return 75;
 *(volatile uintptr_t*)0x58=value;
 return 76;
}
'''
asm=['EXTERN g_fail:DWORD','PUBLIC open_cont, duplicate_cont, factory_cont, cast_cont, project_cont, acceptance_cont','.code','synthetic_open PROC','sub rsp,28h','mov ecx,700000h','mov edx,7018d8h','call outer','open_cont::','nop','add rsp,28h','ret','synthetic_open ENDP',
'outer PROC','mov [rsp+10h],rdx','mov [rsp+8h],rcx','sub rsp,1d8h']
names=['duplicate','factory','cast','project','acceptance']
for name in names:
 asm += ['call '+name,name+'_cont::','test rax,rax',('jnz failed' if name=='duplicate' else 'jz failed')]
asm += ['mov eax,100h','jmp done','failed:','xor eax,eax','done:','add rsp,1d8h','ret','outer ENDP']
for i,name in enumerate(names):
 asm += [name+' PROC']+['nop']*24+['xor eax,eax','cmp g_fail,'+str(i),'je '+name+'_fail','mov eax,'+('0' if i==0 else '101h' if i==4 else '100h'),'ret',name+'_fail:','mov eax,'+('1' if i==0 else '0'),'ret',name+' ENDP']
asm+=['END']
(OUT/'fixture.c').write_text(code);(OUT/'fixture.asm').write_text('\n'.join(asm)+'\n')
bin=Path(env['VCToolsInstallDir'])/'bin/Hostx64/x64'
commands=[[str(bin/'ml64.exe'),'/nologo','/c','/Fo'+str(OUT/'hooks.obj'),str(OUT/'fixture.asm')],
[str(bin/'cl.exe'),'/nologo','/Od','/MT',str(OUT/'fixture.c'),str(OUT/'hooks.obj'),'/Fo'+str(OUT/'main.obj'),'/Fe'+str(OUT/'fixture.exe'),'/link','/INCREMENTAL:NO',*['/EXPORT:'+n for n in ['synthetic_open','outer','open_cont']+names+[n+'_cont' for n in names]]]]
logs=[]
for c in commands:
 r=subprocess.run(c,cwd=OUT,env=env,capture_output=True,text=True);logs.append({'command':c,'exit':r.returncode,'stdout':r.stdout,'stderr':r.stderr})
 (OUT/'build.json').write_text(json.dumps(logs,indent=2))
 if r.returncode:raise SystemExit(r.stdout+r.stderr)
b=(OUT/'fixture.exe').read_bytes();pe=struct.unpack_from('<I',b,0x3c)[0];n=struct.unpack_from('<H',b,pe+6)[0];opt=pe+24;sz=struct.unpack_from('<H',b,pe+20)[0];sections=opt+sz
size=struct.unpack_from('<I',b,opt+56)[0]
def off(rva):
 for i in range(n):
  vs,va,rs,raw=struct.unpack_from('<IIII',b,sections+40*i+8)
  if va<=rva<va+max(vs,rs):return raw+rva-va
 raise ValueError(rva)
exp=struct.unpack_from('<I',b,opt+112)[0];e=off(exp);count,functions,namesr,ordinals=struct.unpack_from('<IIII',b,e+24)
exports={}
for i in range(count):
 nr=struct.unpack_from('<I',b,off(namesr)+i*4)[0];st=off(nr);name=b[st:b.index(0,st)].decode();ordinal=struct.unpack_from('<H',b,off(ordinals)+i*2)[0];exports[name]=struct.unpack_from('<I',b,off(functions)+ordinal*4)[0]
hooks=[]
for name in ['outer']+names:
 rva=exports[name];hooks.append({'name':name,'rva':rva,'bytes':b[off(rva):off(rva)+(17 if name=='outer' else 24)].hex(),'callerRva':exports['open_cont' if name=='outer' else name+'_cont']})
meta={'exe':str(OUT/'fixture.exe'),'size':size,'sha256':hashlib.sha256(b).hexdigest(),'hooks':hooks,'exports':exports,'sourceHashes':{n:hashlib.sha256((OUT/n).read_bytes()).hexdigest() for n in ['fixture.c','fixture.asm']}}
(OUT/'metadata.json').write_text(json.dumps(meta,indent=2));print(json.dumps({'outcome':'BUILT','exe':meta['exe'],'sha256':meta['sha256']}))
