Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067070E30: mov         rax,rsp
  0000000067070E33: push        rdi
  0000000067070E34: sub         rsp,60h
  0000000067070E38: mov         qword ptr [rsp+48h],0FFFFFFFFFFFFFFFEh
  0000000067070E41: mov         qword ptr [rax+10h],rbx
  0000000067070E45: mov         qword ptr [rax+18h],rbp
  0000000067070E49: mov         qword ptr [rax+20h],rsi
  0000000067070E4D: movaps      xmmword ptr [rax-18h],xmm6
  0000000067070E51: mov         ebp,r9d
  0000000067070E54: movapd      xmm6,xmm1
  0000000067070E58: mov         rbx,rcx
  0000000067070E5B: mov         esi,1
  0000000067070E60: lea         eax,[r8-41h]
  0000000067070E64: xor         edi,edi
  0000000067070E66: mov         ecx,40h
  0000000067070E6B: cmp         al,19h
  0000000067070E6D: cmovbe      edi,ecx
  0000000067070E70: lea         eax,[r8-41h]
  0000000067070E74: cmp         al,19h
  0000000067070E76: ja          0000000067070E7C
  0000000067070E78: add         r8b,20h
  0000000067070E7C: movsx       ecx,r8b
  0000000067070E80: sub         ecx,65h
  0000000067070E83: je          0000000067070E94
  0000000067070E85: sub         ecx,esi
  0000000067070E87: je          0000000067070E96
  0000000067070E89: cmp         ecx,esi
  0000000067070E8B: jne         0000000067070E96
  0000000067070E8D: mov         esi,2
  0000000067070E92: jmp         0000000067070E96
  0000000067070E94: xor         esi,esi
  0000000067070E96: xor         r8d,r8d
  0000000067070E99: lea         edx,[r8+1]
  0000000067070E9D: lea         rcx,[rsp+40h]
  0000000067070EA2: call        000000006704D5A0
  0000000067070EA7: lea         rcx,[rsp+40h]
  0000000067070EAC: call        0000000067049340
  0000000067070EB1: mov         rcx,rax
  0000000067070EB4: mov         dword ptr [rsp+30h],edi
  0000000067070EB8: or          edi,0FFFFFFFFh
  0000000067070EBB: mov         dword ptr [rsp+28h],edi
  0000000067070EBF: mov         dword ptr [rsp+20h],esi
  0000000067070EC3: mov         r9d,ebp
  0000000067070EC6: movapd      xmm2,xmm6
  0000000067070ECA: lea         rdx,[rsp+70h]
  0000000067070ECF: call        000000006704EE70
  0000000067070ED4: mov         rsi,rax
  0000000067070ED7: mov         rcx,qword ptr [rax]
  0000000067070EDA: lock add    dword ptr [rcx],1
  0000000067070EDE: mov         rdx,qword ptr [rbx]
  0000000067070EE1: lock add    dword ptr [rdx],edi
  0000000067070EE4: jne         0000000067070EEE
  0000000067070EE6: mov         rcx,qword ptr [rbx]
  0000000067070EE9: call        00000000670700D0
  0000000067070EEE: mov         rax,qword ptr [rsi]
  0000000067070EF1: mov         qword ptr [rbx],rax
  0000000067070EF4: mov         rax,qword ptr [rsp+70h]
  0000000067070EF9: lock add    dword ptr [rax],0FFFFFFFFh
  0000000067070EFD: jne         0000000067070F0E
  0000000067070EFF: mov         rcx,qword ptr [rsp+70h]
  0000000067070F04: call        00000000670700D0
  0000000067070F09: mov         rax,rbx
  0000000067070F0C: jmp         0000000067070F11
  0000000067070F0E: mov         rax,rbx
  0000000067070F11: lea         r11,[rsp+60h]
  0000000067070F16: mov         rbx,qword ptr [r11+18h]
  0000000067070F1A: mov         rbp,qword ptr [r11+20h]
  0000000067070F1E: mov         rsi,qword ptr [r11+28h]
  0000000067070F22: movaps      xmm6,xmmword ptr [rsp+50h]
  0000000067070F27: mov         rsp,r11
  0000000067070F2A: pop         rdi
  0000000067070F2B: ret
  0000000067070F2C: int         3
  0000000067070F2D: int         3
  0000000067070F2E: int         3
  0000000067070F2F: int         3
  0000000067070F30: 48

  Summary

      1A7000 .text
