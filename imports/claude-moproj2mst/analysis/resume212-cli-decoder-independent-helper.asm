Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400011A0: mov         qword ptr [rsp+10h],rdx
  00000001400011A5: mov         qword ptr [rsp+8],rcx
  00000001400011AA: sub         rsp,0B8h
  00000001400011B1: mov         qword ptr [rsp+0000000000000090h],0FFFFFFFFFFFFFFFEh
  00000001400011BD: mov         rax,qword ptr [0000000143B5E030h]
  00000001400011C4: xor         rax,rsp
  00000001400011C7: mov         qword ptr [rsp+00000000000000A0h],rax
  00000001400011CF: mov         dword ptr [rsp+0000000000000088h],0
  00000001400011DA: mov         rcx,qword ptr [rsp+00000000000000C8h]
  00000001400011E2: call        qword ptr [0000000141E044B0h]
  00000001400011E8: mov         dword ptr [rsp+20h],eax
  00000001400011EC: cmp         dword ptr [rsp+20h],0
  00000001400011F1: jne         0000000140001227
  00000001400011F3: lea         rdx,[0000000141E14E1Fh]
  00000001400011FA: mov         rcx,qword ptr [rsp+00000000000000C0h]
  0000000140001202: call        qword ptr [0000000141E04480h]
  0000000140001208: nop
  0000000140001209: mov         eax,dword ptr [rsp+0000000000000088h]
  0000000140001210: or          eax,1
  0000000140001213: mov         dword ptr [rsp+0000000000000088h],eax
  000000014000121A: mov         rax,qword ptr [rsp+00000000000000C0h]
  0000000140001222: jmp         00000001400013EC
  0000000140001227: mov         eax,dword ptr [rsp+20h]
  000000014000122B: cdq
  000000014000122C: and         edx,3
  000000014000122F: add         eax,edx
  0000000140001231: sar         eax,2
  0000000140001234: cdqe
  0000000140001236: mov         qword ptr [rsp+60h],rax
  000000014000123B: mov         eax,2
  0000000140001240: mul         rax,qword ptr [rsp+60h]
  0000000140001245: mov         rcx,rax
  0000000140001248: mov         rax,0FFFFFFFFFFFFFFFFh
  000000014000124F: cmovo       rcx,rax
  0000000140001253: call        000000014146C25C
  0000000140001258: mov         qword ptr [rsp+70h],rax
  000000014000125D: cmp         qword ptr [rsp+70h],0
  0000000140001263: je          000000014000128F
  0000000140001265: mov         r9,qword ptr [0000000141E04488h]
  000000014000126C: mov         r8d,dword ptr [rsp+60h]
  0000000140001271: mov         edx,2
  0000000140001276: mov         rcx,qword ptr [rsp+70h]
  000000014000127B: call        00000001400020C0
  0000000140001280: mov         r11,qword ptr [rsp+70h]
  0000000140001285: mov         qword ptr [rsp+0000000000000098h],r11
  000000014000128D: jmp         000000014000129B
  000000014000128F: mov         qword ptr [rsp+0000000000000098h],0
  000000014000129B: mov         rax,qword ptr [rsp+0000000000000098h]
  00000001400012A3: mov         qword ptr [rsp+68h],rax
  00000001400012A8: mov         rax,qword ptr [rsp+68h]
  00000001400012AD: mov         qword ptr [rsp+48h],rax
  00000001400012B2: mov         dword ptr [rsp+50h],0
  00000001400012BA: jmp         00000001400012C7
  00000001400012BC: mov         eax,dword ptr [rsp+50h]
  00000001400012C0: add         eax,1
  00000001400012C3: mov         dword ptr [rsp+50h],eax
  00000001400012C7: mov         eax,dword ptr [rsp+50h]
  00000001400012CB: shl         eax,2
  00000001400012CE: cmp         eax,dword ptr [rsp+20h]
  00000001400012D2: jge         000000014000136F
  00000001400012D8: mov         dword ptr [rsp+58h],0
  00000001400012E0: jmp         00000001400012ED
  00000001400012E2: mov         eax,dword ptr [rsp+58h]
  00000001400012E6: add         eax,1
  00000001400012E9: mov         dword ptr [rsp+58h],eax
  00000001400012ED: cmp         dword ptr [rsp+58h],4
  00000001400012F2: jge         000000014000132C
  00000001400012F4: mov         ecx,dword ptr [rsp+58h]
  00000001400012F8: mov         eax,dword ptr [rsp+50h]
  00000001400012FC: lea         r8d,[rcx+rax*4]
  0000000140001300: lea         rdx,[rsp+78h]
  0000000140001305: mov         rcx,qword ptr [rsp+00000000000000C8h]
  000000014000130D: call        qword ptr [0000000141E04490h]
  0000000140001313: mov         rcx,rax
  0000000140001316: call        qword ptr [0000000141E04498h]
  000000014000131C: movzx       r11d,al
  0000000140001320: movsxd      rax,dword ptr [rsp+58h]
  0000000140001325: mov         byte ptr [rsp+rax+30h],r11b
  000000014000132A: jmp         00000001400012E2
  000000014000132C: mov         byte ptr [rsp+34h],0
  0000000140001331: lea         r8,[rsp+54h]
  0000000140001336: lea         rdx,[0000000141E151C4h]
  000000014000133D: lea         rcx,[rsp+30h]
  0000000140001342: call        000000014146DFD4
  0000000140001347: mov         edx,dword ptr [rsp+54h]
  000000014000134B: lea         rcx,[rsp+7Ah]
  0000000140001350: call        qword ptr [0000000141E044A0h]
  0000000140001356: movsxd      r11,dword ptr [rsp+50h]
  000000014000135B: mov         rcx,qword ptr [rsp+48h]
  0000000140001360: movzx       eax,word ptr [rsp+7Ah]
  0000000140001365: mov         word ptr [rcx+r11*2],ax
  000000014000136A: jmp         00000001400012BC
  000000014000136F: lea         rcx,[rsp+40h]
  0000000140001374: call        qword ptr [0000000141E044C0h]
  000000014000137A: nop
  000000014000137B: mov         eax,dword ptr [rsp+20h]
  000000014000137F: cdq
  0000000140001380: and         edx,3
  0000000140001383: add         eax,edx
  0000000140001385: sar         eax,2
  0000000140001388: mov         r8d,eax
  000000014000138B: mov         rdx,qword ptr [rsp+48h]
  0000000140001390: lea         rcx,[rsp+40h]
  0000000140001395: call        qword ptr [0000000141E044A8h]
  000000014000139B: mov         rax,qword ptr [rsp+48h]
  00000001400013A0: mov         qword ptr [rsp+0000000000000080h],rax
  00000001400013A8: mov         rcx,qword ptr [rsp+0000000000000080h]
  00000001400013B0: call        000000014146D898
  00000001400013B5: lea         rdx,[rsp+40h]
  00000001400013BA: mov         rcx,qword ptr [rsp+00000000000000C0h]
  00000001400013C2: call        qword ptr [0000000141E059D8h]
  00000001400013C8: mov         eax,dword ptr [rsp+0000000000000088h]
  00000001400013CF: or          eax,1
  00000001400013D2: mov         dword ptr [rsp+0000000000000088h],eax
  00000001400013D9: lea         rcx,[rsp+40h]
  00000001400013DE: call        qword ptr [0000000141E059E0h]
  00000001400013E4: mov         rax,qword ptr [rsp+00000000000000C0h]
  00000001400013EC: mov         rcx,qword ptr [rsp+00000000000000A0h]
  00000001400013F4: xor         rcx,rsp
  00000001400013F7: call        000000014146DF10
  00000001400013FC: add         rsp,0B8h
  0000000140001403: ret
  0000000140001404: int         3

  Summary

     1E02000 .text
