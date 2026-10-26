Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006704C340: mov         r11,rsp
  000000006704C343: push        rdi
  000000006704C344: sub         rsp,160h
  000000006704C34B: mov         qword ptr [rsp+28h],0FFFFFFFFFFFFFFFEh
  000000006704C354: mov         qword ptr [r11+10h],rbx
  000000006704C358: mov         qword ptr [r11+18h],rbp
  000000006704C35C: mov         qword ptr [r11+20h],rsi
  000000006704C360: movaps      xmmword ptr [r11-18h],xmm6
  000000006704C365: mov         ebp,r9d
  000000006704C368: mov         rbx,r8
  000000006704C36B: mov         rax,rdx
  000000006704C36E: mov         rsi,rcx
  000000006704C371: xor         edi,edi
  000000006704C373: mov         dword ptr [r11+8],edi
  000000006704C377: mov         dword ptr [rsp+34h],edi
  000000006704C37B: lea         rcx,[rsp+40h]
  000000006704C380: mov         qword ptr [rsp+38h],rcx
  000000006704C385: mov         dword ptr [rsp+30h],100h
  000000006704C38D: mov         ecx,0A0h
  000000006704C392: cmp         word ptr [rsi+8],cx
  000000006704C396: jne         000000006704C3B2
  000000006704C398: lea         rdx,[rsp+20h]
  000000006704C39D: mov         rcx,rax
  000000006704C3A0: call        00000000670744A0
  000000006704C3A5: nop
  000000006704C3A6: mov         edi,1
  000000006704C3AB: mov         dword ptr [rsp+0000000000000170h],edi
  000000006704C3B2: lea         r9,[rsp+30h]
  000000006704C3B7: mov         r8d,ebp
  000000006704C3BA: mov         rdx,rax
  000000006704C3BD: mov         rcx,rsi
  000000006704C3C0: call        000000006704C190
  000000006704C3C5: test        al,al
  000000006704C3C7: sete        sil
  000000006704C3CB: test        dil,1
  000000006704C3CF: je          000000006704C3E6
  000000006704C3D1: mov         rax,qword ptr [rsp+20h]
  000000006704C3D6: lock add    dword ptr [rax],0FFFFFFFFh
  000000006704C3DA: jne         000000006704C3E6
  000000006704C3DC: mov         rcx,qword ptr [rsp+20h]
  000000006704C3E1: call        00000000670700D0
  000000006704C3E6: test        sil,sil
  000000006704C3E9: je          000000006704C40D
  000000006704C3EB: test        rbx,rbx
  000000006704C3EE: je          000000006704C3F3
  000000006704C3F0: mov         byte ptr [rbx],0
  000000006704C3F3: lea         rax,[rsp+40h]
  000000006704C3F8: mov         rcx,qword ptr [rsp+38h]
  000000006704C3FD: cmp         rcx,rax
  000000006704C400: je          000000006704C407
  000000006704C402: call        00000000670203B0
  000000006704C407: xorpd       xmm0,xmm0
  000000006704C40B: jmp         000000006704C439
  000000006704C40D: xor         r8d,r8d
  000000006704C410: mov         rdx,rbx
  000000006704C413: mov         rcx,qword ptr [rsp+38h]
  000000006704C418: call        00000000670494B0
  000000006704C41D: movapd      xmm6,xmm0
  000000006704C421: lea         rax,[rsp+40h]
  000000006704C426: mov         rcx,qword ptr [rsp+38h]
  000000006704C42B: cmp         rcx,rax
  000000006704C42E: je          000000006704C435
  000000006704C430: call        00000000670203B0
  000000006704C435: movapd      xmm0,xmm6
  000000006704C439: lea         r11,[rsp+0000000000000160h]
  000000006704C441: mov         rbx,qword ptr [r11+18h]
  000000006704C445: mov         rbp,qword ptr [r11+20h]
  000000006704C449: mov         rsi,qword ptr [r11+28h]
  000000006704C44D: movaps      xmm6,xmmword ptr [r11-10h]
  000000006704C452: mov         rsp,r11
  000000006704C455: pop         rdi
  000000006704C456: ret
  000000006704C457: int         3

  Summary

      1A7000 .text
