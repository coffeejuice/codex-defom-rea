Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\gui_mo\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140B6A3BB: lea         r8,[rsp+00000000000000CCh]
  0000000140B6A3C3: lea         rdx,[rsp+00000000000000C8h]
  0000000140B6A3CB: mov         rcx,qword ptr [rsp+00000000000006D8h]
  0000000140B6A3D3: call        0000000140AF0AD0
  0000000140B6A3D8: mov         dword ptr [rsp+0000000000000100h],eax
  0000000140B6A3DF: mov         rcx,qword ptr [rsp+00000000000006D0h]
  0000000140B6A3E7: add         rcx,750h
  0000000140B6A3EE: lea         rdx,[rsp+00000000000000C8h]
  0000000140B6A3F6: call        00000001408D66C0
  0000000140B6A3FB: movzx       eax,al
  0000000140B6A3FE: test        eax,eax
  0000000140B6A400: je          0000000140B6A6FE
  0000000140B6A406: mov         rcx,qword ptr [rsp+00000000000006D0h]
  0000000140B6A40E: mov         eax,dword ptr [rsp+00000000000000C8h]
  0000000140B6A415: mov         dword ptr [rcx+0000000000000A34h],eax
  0000000140B6A41B: mov         rax,qword ptr [rsp+00000000000006D0h]
  0000000140B6A423: mov         dword ptr [rax+0000000000000A38h],0
  0000000140B6A42D: mov         rax,qword ptr [rsp+00000000000006D0h]
  0000000140B6A435: cmp         dword ptr [rax+0000000000000A34h],2
  0000000140B6A43C: jne         0000000140B6A613
  0000000140B6A442: cmp         dword ptr [rsp+00000000000000CCh],4
  0000000140B6A44A: jne         0000000140B6A517
  0000000140B6A450: mov         rcx,qword ptr [rsp+00000000000006D0h]
  0000000140B6A458: mov         eax,dword ptr [rsp+00000000000000CCh]
  0000000140B6A45F: mov         dword ptr [rcx+0000000000000A38h],eax
  0000000140B6A465: lea         rax,[rsp+00000000000000DCh]
  0000000140B6A46D: lea         r9,[rsp+00000000000000D8h]
  0000000140B6A475: lea         r8,[rsp+00000000000000D4h]
  0000000140B6A47D: lea         rdx,[rsp+00000000000000D0h]
  0000000140B6A485: mov         qword ptr [rsp+20h],rax
  0000000140B6A48A: mov         rcx,qword ptr [rsp+00000000000006D8h]
  0000000140B6A492: call        0000000140AF0CF0
  0000000140B6A497: mov         dword ptr [rsp+0000000000000100h],eax
  0000000140B6A49E: movd        xmm0,dword ptr [rsp+00000000000000D0h]
  0000000140B6A4A7: cvtdq2pd    xmm0,xmm0
  0000000140B6A4AB: mov         rax,qword ptr [rsp+00000000000006D0h]
  0000000140B6A4B3: movsd       mmword ptr [rax+0000000000000A40h],xmm0
  0000000140B6A4BB: movd        xmm0,dword ptr [rsp+00000000000000D4h]
  0000000140B6A4C4: cvtdq2pd    xmm0,xmm0
  0000000140B6A4C8: mov         rax,qword ptr [rsp+00000000000006D0h]
  0000000140B6A4D0: movsd       mmword ptr [rax+0000000000000A48h],xmm0
  0000000140B6A4D8: movd        xmm0,dword ptr [rsp+00000000000000D8h]
  0000000140B6A4E1: cvtdq2pd    xmm0,xmm0
  0000000140B6A4E5: mov         rax,qword ptr [rsp+00000000000006D0h]
  0000000140B6A4ED: movsd       mmword ptr [rax+0000000000000A50h],xmm0
  0000000140B6A4F5: movd        xmm0,dword ptr [rsp+00000000000000DCh]
  0000000140B6A4FE: cvtdq2pd    xmm0,xmm0
  0000000140B6A502: mov         rax,qword ptr [rsp+00000000000006D0h]
  0000000140B6A50A: movsd       mmword ptr [rax+0000000000000A58h],xmm0
  0000000140B6A512: jmp         0000000140B6A60E
  0000000140B6A517: mov         rax,qword ptr [rsp+00000000000006D0h]
  0000000140B6A51F: mov         dword ptr [rax+0000000000000A38h],4
  0000000140B6A529: mov         rax,qword ptr [rsp+00000000000006D0h]
  0000000140B6A531: movsd       xmm0,mmword ptr [0000000142101520h]
  0000000140B6A539: movsd       mmword ptr [rax+0000000000000A40h],xmm0
  0000000140B6A541: mov         rax,qword ptr [rsp+00000000000006D0h]
  0000000140B6A549: movsd       xmm0,mmword ptr [00000001421D3C70h]
  0000000140B6A551: movsd       mmword ptr [rax+0000000000000A48h],xmm0
  0000000140B6A559: mov         rax,qword ptr [rsp+00000000000006D0h]
  0000000140B6A561: movsd       xmm0,mmword ptr [00000001421249E0h]
  0000000140B6A569: movsd       mmword ptr [rax+0000000000000A50h],xmm0
  0000000140B6A571: mov         rax,qword ptr [rsp+00000000000006D0h]
  0000000140B6A579: movsd       xmm0,mmword ptr [00000001421249E0h]
  0000000140B6A581: movsd       mmword ptr [rax+0000000000000A58h],xmm0
  0000000140B6A589: call        000000014146DB84
  0000000140B6A58E: mov         rcx,rax
  0000000140B6A591: add         rcx,30h
  0000000140B6A595: mov         r8d,2
  0000000140B6A59B: lea         rdx,[00000001427F5740h]
  0000000140B6A5A2: call        000000014146DA0C
  0000000140B6A5A7: mov         r11,qword ptr [rsp+00000000000006D0h]
  0000000140B6A5AF: cvttsd2si   ebp,mmword ptr [r11+0000000000000A60h]
  0000000140B6A5B8: mov         rax,qword ptr [rsp+00000000000006D0h]
  0000000140B6A5C0: cvttsd2si   esi,mmword ptr [rax+0000000000000A50h]
  0000000140B6A5C8: mov         rax,qword ptr [rsp+00000000000006D0h]
  0000000140B6A5D0: cvttsd2si   edi,mmword ptr [rax+0000000000000A48h]
  0000000140B6A5D8: mov         rax,qword ptr [rsp+00000000000006D0h]
  0000000140B6A5E0: cvttsd2si   ebx,mmword ptr [rax+0000000000000A40h]
  0000000140B6A5E8: call        000000014146DB84
  0000000140B6A5ED: mov         rcx,rax
  0000000140B6A5F0: add         rcx,30h
  0000000140B6A5F4: mov         dword ptr [rsp+28h],ebp
  0000000140B6A5F8: mov         dword ptr [rsp+20h],esi
  0000000140B6A5FC: mov         r9d,edi
  0000000140B6A5FF: mov         r8d,ebx
  0000000140B6A602: lea         rdx,[00000001427F5770h]
  0000000140B6A609: call        000000014146DA0C
  0000000140B6A60E: jmp         0000000140B6A6FC
  0000000140B6A613: mov         rax,qword ptr [rsp+00000000000006D0h]
  0000000140B6A61B: cmp         dword ptr [rax+0000000000000A34h],6
  0000000140B6A622: jne         0000000140B6A6FC
  0000000140B6A628: cmp         dword ptr [rsp+00000000000000CCh],4
  0000000140B6A630: jne         0000000140B6A6DE
  0000000140B6A636: mov         rcx,qword ptr [rsp+00000000000006D0h]
  0000000140B6A63E: mov         eax,dword ptr [rsp+00000000000000CCh]
  0000000140B6A645: mov         dword ptr [rcx+0000000000000A38h],eax
  0000000140B6A64B: lea         rax,[rsp+0000000000000088h]
  0000000140B6A653: lea         r9,[rsp+0000000000000080h]
  0000000140B6A65B: lea         r8,[rsp+78h]
  0000000140B6A660: mov         qword ptr [rsp+20h],rax
  0000000140B6A665: lea         rdx,[rsp+70h]
  0000000140B6A66A: mov         rcx,qword ptr [rsp+00000000000006D8h]
  0000000140B6A672: call        0000000140AF1A30
  0000000140B6A677: mov         dword ptr [rsp+0000000000000100h],eax
  0000000140B6A67E: mov         rax,qword ptr [rsp+00000000000006D0h]
  0000000140B6A686: movsd       xmm0,mmword ptr [rsp+70h]
  0000000140B6A68C: movsd       mmword ptr [rax+0000000000000A40h],xmm0
  0000000140B6A694: mov         rax,qword ptr [rsp+00000000000006D0h]
  0000000140B6A69C: movsd       xmm0,mmword ptr [rsp+78h]
  0000000140B6A6A2: movsd       mmword ptr [rax+0000000000000A48h],xmm0
  0000000140B6A6AA: mov         rax,qword ptr [rsp+00000000000006D0h]
  0000000140B6A6B2: movsd       xmm0,mmword ptr [rsp+0000000000000080h]
  0000000140B6A6BB: movsd       mmword ptr [rax+0000000000000A50h],xmm0
  0000000140B6A6C3: mov         rax,qword ptr [rsp+00000000000006D0h]
  0000000140B6A6CB: movsd       xmm0,mmword ptr [rsp+0000000000000088h]
  0000000140B6A6D4: movsd       mmword ptr [rax+0000000000000A58h],xmm0
  0000000140B6A6DC: jmp         0000000140B6A6FC
  0000000140B6A6DE: call        000000014146DB84
  0000000140B6A6E3: mov         rcx,rax
  0000000140B6A6E6: add         rcx,30h
  0000000140B6A6EA: mov         r8d,6
  0000000140B6A6F0: lea         rdx,[00000001427F5790h]
  0000000140B6A6F7: call        000000014146DA0C
  0000000140B6A6FC: jmp         0000000140B6A71E
  0000000140B6A6FE: call        000000014146DB84
  0000000140B6A703: mov         rcx,rax
  0000000140B6A706: add         rcx,30h
  0000000140B6A70A: mov         r8d,dword ptr [rsp+00000000000000C8h]
  0000000140B6A712: 48
  0000000140B6A713: 8D

  Summary

     1E02000 .text
