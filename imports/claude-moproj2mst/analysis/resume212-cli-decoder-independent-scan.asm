Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014146DF30: mov         qword ptr [rsp+8],rbx
  000000014146DF35: mov         qword ptr [rsp+10h],rbp
  000000014146DF3A: mov         qword ptr [rsp+18h],rsi
  000000014146DF3F: push        rdi
  000000014146DF40: sub         rsp,60h
  000000014146DF44: mov         rbp,rcx
  000000014146DF47: mov         rcx,rdx
  000000014146DF4A: mov         rsi,r9
  000000014146DF4D: mov         rbx,r8
  000000014146DF50: mov         rdi,rdx
  000000014146DF53: call        0000000141470B80
  000000014146DF58: test        rdi,rdi
  000000014146DF5B: jne         000000014146DF82
  000000014146DF5D: call        0000000141474134
  000000014146DF62: and         qword ptr [rsp+20h],0
  000000014146DF68: xor         r9d,r9d
  000000014146DF6B: xor         r8d,r8d
  000000014146DF6E: xor         edx,edx
  000000014146DF70: xor         ecx,ecx
  000000014146DF72: mov         dword ptr [rax],16h
  000000014146DF78: call        0000000141470568
  000000014146DF7D: or          eax,0FFFFFFFFh
  000000014146DF80: jmp         000000014146DFBD
  000000014146DF82: test        rbx,rbx
  000000014146DF85: je          000000014146DF5D
  000000014146DF87: mov         r9,qword ptr [rsp+0000000000000090h]
  000000014146DF8F: mov         ecx,7FFFFFFFh
  000000014146DF94: mov         r8,rsi
  000000014146DF97: cmp         rax,rcx
  000000014146DF9A: mov         rdx,rbx
  000000014146DF9D: mov         dword ptr [rsp+48h],49h
  000000014146DFA5: cmova       eax,ecx
  000000014146DFA8: lea         rcx,[rsp+30h]
  000000014146DFAD: mov         qword ptr [rsp+40h],rdi
  000000014146DFB2: mov         qword ptr [rsp+30h],rdi
  000000014146DFB7: mov         dword ptr [rsp+38h],eax
  000000014146DFBB: call        rbp
  000000014146DFBD: lea         r11,[rsp+60h]
  000000014146DFC2: mov         rbx,qword ptr [r11+10h]
  000000014146DFC6: mov         rbp,qword ptr [r11+18h]
  000000014146DFCA: mov         rsi,qword ptr [r11+20h]
  000000014146DFCE: mov         rsp,r11
  000000014146DFD1: pop         rdi
  000000014146DFD2: ret
  000000014146DFD3: int         3
  000000014146DFD4: mov         r11,rsp
  000000014146DFD7: mov         qword ptr [r11+10h],rdx
  000000014146DFDB: mov         qword ptr [r11+18h],r8
  000000014146DFDF: mov         qword ptr [r11+20h],r9
  000000014146DFE3: sub         rsp,38h
  000000014146DFE7: mov         r8,rdx
  000000014146DFEA: lea         rax,[r11+18h]
  000000014146DFEE: mov         rdx,rcx
  000000014146DFF1: lea         rcx,[0000000141480128h]
  000000014146DFF8: xor         r9d,r9d
  000000014146DFFB: mov         qword ptr [r11-18h],rax
  000000014146DFFF: call        000000014146DF30
  000000014146E004: add         rsp,38h
  000000014146E008: ret
  000000014146E009: int         3

  Summary

     1E02000 .text
