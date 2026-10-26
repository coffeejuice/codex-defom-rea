Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400DA270: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  00000001400DA275: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  00000001400DA27A: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  00000001400DA27E: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400DA283: 48 83 EC 38        sub         rsp,38h
  00000001400DA287: 83 7C 24 48 00     cmp         dword ptr [rsp+48h],0
  00000001400DA28C: 0F 85 B9 00 00 00  jne         00000001400DA34B
  00000001400DA292: E8 E9 A8 F2 FF     call        0000000140004B80
  00000001400DA297: 4C 8B 5C 24 40     mov         r11,qword ptr [rsp+40h]
  00000001400DA29C: 4C 89 5C 24 20     mov         qword ptr [rsp+20h],r11
  00000001400DA2A1: 8B 44 24 50        mov         eax,dword ptr [rsp+50h]
  00000001400DA2A5: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  00000001400DA2A9: 83 7C 24 28 09     cmp         dword ptr [rsp+28h],9
  00000001400DA2AE: 0F 87 97 00 00 00  ja          00000001400DA34B
  00000001400DA2B4: 48 63 44 24 28     movsxd      rax,dword ptr [rsp+28h]
  00000001400DA2B9: 48 8D 0D 40 5D F2  lea         rcx,[0000000140000000h]
                    FF
  00000001400DA2C0: 8B 84 81 50 A3 0D  mov         eax,dword ptr [rcx+rax*4+00000000000DA350h]
                    00
  00000001400DA2C7: 48 03 C1           add         rax,rcx
  00000001400DA2CA: FF E0              jmp         rax
  00000001400DA2CC: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001400DA2D1: E8 FA 01 00 00     call        00000001400DA4D0
  00000001400DA2D6: EB 73              jmp         00000001400DA34B
  00000001400DA2D8: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001400DA2DD: E8 1E A5 02 00     call        0000000140104800
  00000001400DA2E2: EB 67              jmp         00000001400DA34B
  00000001400DA2E4: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  00000001400DA2E9: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  00000001400DA2ED: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001400DA2F2: E8 E9 DD 02 00     call        00000001401080E0
  00000001400DA2F7: EB 52              jmp         00000001400DA34B
  00000001400DA2F9: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001400DA2FE: E8 FD 00 03 00     call        000000014010A400
  00000001400DA303: EB 46              jmp         00000001400DA34B
  00000001400DA305: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001400DA30A: E8 01 DF 02 00     call        0000000140108210
  00000001400DA30F: EB 3A              jmp         00000001400DA34B
  00000001400DA311: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001400DA316: E8 25 E3 02 00     call        0000000140108640
  00000001400DA31B: EB 2E              jmp         00000001400DA34B
  00000001400DA31D: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001400DA322: E8 A9 E4 02 00     call        00000001401087D0
  00000001400DA327: EB 22              jmp         00000001400DA34B
  00000001400DA329: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001400DA32E: E8 CD E4 02 00     call        0000000140108800
  00000001400DA333: EB 16              jmp         00000001400DA34B
  00000001400DA335: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001400DA33A: E8 E1 E4 02 00     call        0000000140108820
  00000001400DA33F: EB 0A              jmp         00000001400DA34B
  00000001400DA341: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001400DA346: E8 65 F5 02 00     call        00000001401098B0
  00000001400DA34B: 48 83 C4 38        add         rsp,38h
  00000001400DA34F: C3                 ret
  00000001400DA350: CC                 int         3
  00000001400DA351: A2 0D 00 D8 A2 0D  mov         byte ptr [A2E4000DA2D8000Dh],al
                    00 E4 A2
  00000001400DA35A: 0D 00 F9 A2 0D     or          eax,0DA2F900h
  00000001400DA35F: 00 05 A3 0D 00 11  add         byte ptr [00000001510DB108h],al
  00000001400DA365: A3 0D 00 1D A3 0D  mov         dword ptr [A329000DA31D000Dh],eax
                    00 29 A3
  00000001400DA36E: 0D 00 35 A3 0D     or          eax,0DA33500h
  00000001400DA373: 00 41 A3           add         byte ptr [rcx-5Dh],al
  00000001400DA376: 0D
  00000001400DA377: 00

  Summary

     1E02000 .text
