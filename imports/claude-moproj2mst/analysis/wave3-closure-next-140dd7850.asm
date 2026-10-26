Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140DD7850: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  0000000140DD7855: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  0000000140DD785A: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140DD785E: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140DD7863: 48 83 EC 38        sub         rsp,38h
  0000000140DD7867: 83 7C 24 48 00     cmp         dword ptr [rsp+48h],0
  0000000140DD786C: 0F 85 A4 00 00 00  jne         0000000140DD7916
  0000000140DD7872: E8 09 D3 22 FF     call        0000000140004B80
  0000000140DD7877: 4C 8B 5C 24 40     mov         r11,qword ptr [rsp+40h]
  0000000140DD787C: 4C 89 5C 24 20     mov         qword ptr [rsp+20h],r11
  0000000140DD7881: 8B 44 24 50        mov         eax,dword ptr [rsp+50h]
  0000000140DD7885: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  0000000140DD7889: 83 7C 24 28 06     cmp         dword ptr [rsp+28h],6
  0000000140DD788E: 0F 87 82 00 00 00  ja          0000000140DD7916
  0000000140DD7894: 48 63 44 24 28     movsxd      rax,dword ptr [rsp+28h]
  0000000140DD7899: 48 8D 0D 60 87 22  lea         rcx,[0000000140000000h]
                    FF
  0000000140DD78A0: 8B 84 81 1C 79 DD  mov         eax,dword ptr [rcx+rax*4+0000000000DD791Ch]
                    00
  0000000140DD78A7: 48 03 C1           add         rax,rcx
  0000000140DD78AA: FF E0              jmp         rax
  0000000140DD78AC: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  0000000140DD78B1: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  0000000140DD78B5: 48 8B 12           mov         rdx,qword ptr [rdx]
  0000000140DD78B8: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140DD78BD: E8 3E 46 FD FF     call        0000000140DABF00
  0000000140DD78C2: EB 52              jmp         0000000140DD7916
  0000000140DD78C4: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  0000000140DD78C9: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  0000000140DD78CD: 48 8B 12           mov         rdx,qword ptr [rdx]
  0000000140DD78D0: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140DD78D5: E8 66 46 FD FF     call        0000000140DABF40
  0000000140DD78DA: EB 3A              jmp         0000000140DD7916
  0000000140DD78DC: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140DD78E1: E8 9A 46 FD FF     call        0000000140DABF80
  0000000140DD78E6: EB 2E              jmp         0000000140DD7916
  0000000140DD78E8: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140DD78ED: E8 CE 46 FD FF     call        0000000140DABFC0
  0000000140DD78F2: EB 22              jmp         0000000140DD7916
  0000000140DD78F4: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140DD78F9: E8 02 47 FD FF     call        0000000140DAC000
  0000000140DD78FE: EB 16              jmp         0000000140DD7916
  0000000140DD7900: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140DD7905: E8 36 47 FD FF     call        0000000140DAC040
  0000000140DD790A: EB 0A              jmp         0000000140DD7916
  0000000140DD790C: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140DD7911: E8 6A 47 FD FF     call        0000000140DAC080
  0000000140DD7916: 48 83 C4 38        add         rsp,38h
  0000000140DD791A: C3                 ret
  0000000140DD791B: 90                 nop
  0000000140DD791C: AC                 lods        byte ptr [rsi]
  0000000140DD791D: 78 DD              js          0000000140DD78FC
  0000000140DD791F: 00 C4              add         ah,al
  0000000140DD7921: 78 DD              js          0000000140DD7900
  0000000140DD7923: 00 DC              add         ah,bl
  0000000140DD7925: 78 DD              js          0000000140DD7904
  0000000140DD7927: 00 E8              add         al,ch
  0000000140DD7929: 78 DD              js          0000000140DD7908
  0000000140DD792B: 00 F4              add         ah,dh
  0000000140DD792D: 78 DD              js          0000000140DD790C
  0000000140DD792F: 00 00              add         byte ptr [rax],al
  0000000140DD7931: 79 DD              jns         0000000140DD7910
  0000000140DD7933: 00 0C 79           add         byte ptr [rcx+rdi*2],cl
  0000000140DD7936: DD 00              fld         qword ptr [rax]

  Summary

     1E02000 .text
