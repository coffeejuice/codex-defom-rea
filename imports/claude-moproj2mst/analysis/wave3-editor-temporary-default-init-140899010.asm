Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140899010: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140899014: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140899019: 48 83 EC 38        sub         rsp,38h
  000000014089901D: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140899022: E8 C9 21 00 00     call        000000014089B1F0
  0000000140899027: 4C 8B 5C 24 40     mov         r11,qword ptr [rsp+40h]
  000000014089902C: 49 C7 43 60 00 00  mov         qword ptr [r11+60h],0
                    00 00
  0000000140899034: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140899039: 8B 44 24 48        mov         eax,dword ptr [rsp+48h]
  000000014089903D: 89 01              mov         dword ptr [rcx],eax
  000000014089903F: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899044: 66 0F 57 C0        xorpd       xmm0,xmm0
  0000000140899048: F2 0F 11 40 08     movsd       mmword ptr [rax+8],xmm0
  000000014089904D: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899052: C7 40 54 00 00 00  mov         dword ptr [rax+54h],0
                    00
  0000000140899059: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089905E: C7 40 04 00 00 00  mov         dword ptr [rax+4],0
                    00
  0000000140899065: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089906A: C7 40 40 01 00 00  mov         dword ptr [rax+40h],1
                    00
  0000000140899071: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899076: C7 40 50 00 00 00  mov         dword ptr [rax+50h],0
                    00
  000000014089907D: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899082: C7 40 58 00 00 00  mov         dword ptr [rax+58h],0
                    00
  0000000140899089: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140899091: EB 0B              jmp         000000014089909E
  0000000140899093: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  0000000140899097: 83 C0 01           add         eax,1
  000000014089909A: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  000000014089909E: 83 7C 24 20 03     cmp         dword ptr [rsp+20h],3
  00000001408990A3: 7D 26              jge         00000001408990CB
  00000001408990A5: 48 63 4C 24 20     movsxd      rcx,dword ptr [rsp+20h]
  00000001408990AA: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408990AF: C7 44 88 10 00 00  mov         dword ptr [rax+rcx*4+10h],0
                    00 00
  00000001408990B7: 48 63 4C 24 20     movsxd      rcx,dword ptr [rsp+20h]
  00000001408990BC: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408990C1: C7 44 88 1C 00 00  mov         dword ptr [rax+rcx*4+1Ch],0
                    00 00
  00000001408990C9: EB C8              jmp         0000000140899093
  00000001408990CB: 8B 44 24 48        mov         eax,dword ptr [rsp+48h]
  00000001408990CF: 89 44 24 24        mov         dword ptr [rsp+24h],eax
  00000001408990D3: 8B 44 24 24        mov         eax,dword ptr [rsp+24h]
  00000001408990D7: 83 E8 07           sub         eax,7
  00000001408990DA: 89 44 24 24        mov         dword ptr [rsp+24h],eax
  00000001408990DE: 81 7C 24 24 B4 00  cmp         dword ptr [rsp+24h],0B4h
                    00 00
  00000001408990E6: 0F 87 1A 0E 00 00  ja          0000000140899F06
  00000001408990EC: 48 63 44 24 24     movsxd      rax,dword ptr [rsp+24h]
  00000001408990F1: 48 8D 0D 08 6F 76  lea         rcx,[0000000140000000h]
                    FF
  00000001408990F8: 0F B6 84 01 AC A0  movzx       eax,byte ptr [rcx+rax+000000000089A0ACh]
                    89 00
  0000000140899100: 8B 84 81 38 9F 89  mov         eax,dword ptr [rcx+rax*4+0000000000899F38h]
                    00
  0000000140899107: 48 03 C1           add         rax,rcx
  000000014089910A: FF E0              jmp         rax
  000000014089910C: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899111: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  0000000140899118: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089911D: C7 40 10 01 00 00  mov         dword ptr [rax+10h],1
                    00
  0000000140899124: E9 DD 0D 00 00     jmp         0000000140899F06
  0000000140899129: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089912E: C7 40 04 02 00 00  mov         dword ptr [rax+4],2
                    00
  0000000140899135: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089913A: C7 40 10 0D 00 00  mov         dword ptr [rax+10h],0Dh
                    00
  0000000140899141: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899146: C7 40 14 0C 00 00  mov         dword ptr [rax+14h],0Ch
                    00
  000000014089914D: E9 B4 0D 00 00     jmp         0000000140899F06
  0000000140899152: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899157: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  000000014089915E: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899163: C7 40 10 01 00 00  mov         dword ptr [rax+10h],1
                    00
  000000014089916A: E9 97 0D 00 00     jmp         0000000140899F06
  000000014089916F: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899174: C7 40 04 03 00 00  mov         dword ptr [rax+4],3
                    00
  000000014089917B: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899180: C7 40 40 02 00 00  mov         dword ptr [rax+40h],2
                    00
  0000000140899187: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089918C: C7 40 10 0C 00 00  mov         dword ptr [rax+10h],0Ch
                    00
  0000000140899193: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899198: C7 40 14 06 00 00  mov         dword ptr [rax+14h],6
                    00
  000000014089919F: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408991A4: C7 40 18 03 00 00  mov         dword ptr [rax+18h],3
                    00
  00000001408991AB: E9 56 0D 00 00     jmp         0000000140899F06
  00000001408991B0: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408991B5: C7 40 04 03 00 00  mov         dword ptr [rax+4],3
                    00
  00000001408991BC: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408991C1: C7 40 40 02 00 00  mov         dword ptr [rax+40h],2
                    00
  00000001408991C8: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408991CD: C7 40 10 0C 00 00  mov         dword ptr [rax+10h],0Ch
                    00
  00000001408991D4: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408991D9: C7 40 14 06 00 00  mov         dword ptr [rax+14h],6
                    00
  00000001408991E0: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408991E5: C7 40 18 03 00 00  mov         dword ptr [rax+18h],3
                    00
  00000001408991EC: E9 15 0D 00 00     jmp         0000000140899F06
  00000001408991F1: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408991F6: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  00000001408991FD: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899202: C7 40 10 03 00 00  mov         dword ptr [rax+10h],3
                    00
  0000000140899209: E9 F8 0C 00 00     jmp         0000000140899F06
  000000014089920E: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899213: C7 40 04 03 00 00  mov         dword ptr [rax+4],3
                    00
  000000014089921A: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089921F: C7 40 40 02 00 00  mov         dword ptr [rax+40h],2
                    00
  0000000140899226: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089922B: C7 40 10 07 00 00  mov         dword ptr [rax+10h],7
                    00
  0000000140899232: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899237: C7 40 14 0E 00 00  mov         dword ptr [rax+14h],0Eh
                    00
  000000014089923E: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899243: C7 40 18 03 00 00  mov         dword ptr [rax+18h],3
                    00
  000000014089924A: E9 B7 0C 00 00     jmp         0000000140899F06
  000000014089924F: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899254: C7 40 04 03 00 00  mov         dword ptr [rax+4],3
                    00
  000000014089925B: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899260: C7 40 40 02 00 00  mov         dword ptr [rax+40h],2
                    00
  0000000140899267: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089926C: C7 40 10 0C 00 00  mov         dword ptr [rax+10h],0Ch
                    00
  0000000140899273: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899278: C7 40 14 06 00 00  mov         dword ptr [rax+14h],6
                    00
  000000014089927F: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899284: C7 40 18 03 00 00  mov         dword ptr [rax+18h],3
                    00
  000000014089928B: E9 76 0C 00 00     jmp         0000000140899F06
  0000000140899290: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899295: C7 40 04 03 00 00  mov         dword ptr [rax+4],3
                    00
  000000014089929C: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408992A1: C7 40 40 02 00 00  mov         dword ptr [rax+40h],2
                    00
  00000001408992A8: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408992AD: C7 40 10 07 00 00  mov         dword ptr [rax+10h],7
                    00
  00000001408992B4: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408992B9: C7 40 14 0E 00 00  mov         dword ptr [rax+14h],0Eh
                    00
  00000001408992C0: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408992C5: C7 40 18 03 00 00  mov         dword ptr [rax+18h],3
                    00
  00000001408992CC: E9 35 0C 00 00     jmp         0000000140899F06
  00000001408992D1: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408992D6: C7 40 04 03 00 00  mov         dword ptr [rax+4],3
                    00
  00000001408992DD: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408992E2: C7 40 40 02 00 00  mov         dword ptr [rax+40h],2
                    00
  00000001408992E9: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408992EE: C7 40 10 0C 00 00  mov         dword ptr [rax+10h],0Ch
                    00
  00000001408992F5: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408992FA: C7 40 14 0F 00 00  mov         dword ptr [rax+14h],0Fh
                    00
  0000000140899301: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899306: C7 40 18 10 00 00  mov         dword ptr [rax+18h],10h
                    00
  000000014089930D: E9 F4 0B 00 00     jmp         0000000140899F06
  0000000140899312: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899317: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  000000014089931E: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899323: C7 40 10 03 00 00  mov         dword ptr [rax+10h],3
                    00
  000000014089932A: E9 D7 0B 00 00     jmp         0000000140899F06
  000000014089932F: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899334: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  000000014089933B: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899340: C7 40 10 03 00 00  mov         dword ptr [rax+10h],3
                    00
  0000000140899347: E9 BA 0B 00 00     jmp         0000000140899F06
  000000014089934C: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899351: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  0000000140899358: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089935D: C7 40 10 05 00 00  mov         dword ptr [rax+10h],5
                    00
  0000000140899364: E9 9D 0B 00 00     jmp         0000000140899F06
  0000000140899369: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089936E: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  0000000140899375: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089937A: C7 40 1C 01 00 00  mov         dword ptr [rax+1Ch],1
                    00
  0000000140899381: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899386: C7 40 40 02 00 00  mov         dword ptr [rax+40h],2
                    00
  000000014089938D: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899392: C7 40 10 01 00 00  mov         dword ptr [rax+10h],1
                    00
  0000000140899399: E9 68 0B 00 00     jmp         0000000140899F06
  000000014089939E: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408993A3: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  00000001408993AA: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408993AF: C7 40 1C 01 00 00  mov         dword ptr [rax+1Ch],1
                    00
  00000001408993B6: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408993BB: C7 40 40 04 00 00  mov         dword ptr [rax+40h],4
                    00
  00000001408993C2: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408993C7: C7 40 10 01 00 00  mov         dword ptr [rax+10h],1
                    00
  00000001408993CE: E9 33 0B 00 00     jmp         0000000140899F06
  00000001408993D3: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408993D8: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  00000001408993DF: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408993E4: C7 40 1C 01 00 00  mov         dword ptr [rax+1Ch],1
                    00
  00000001408993EB: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408993F0: C7 40 40 03 00 00  mov         dword ptr [rax+40h],3
                    00
  00000001408993F7: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408993FC: C7 40 10 01 00 00  mov         dword ptr [rax+10h],1
                    00
  0000000140899403: E9 FE 0A 00 00     jmp         0000000140899F06
  0000000140899408: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089940D: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  0000000140899414: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899419: C7 40 10 03 00 00  mov         dword ptr [rax+10h],3
                    00
  0000000140899420: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899425: C7 40 40 02 00 00  mov         dword ptr [rax+40h],2
                    00
  000000014089942C: E9 D5 0A 00 00     jmp         0000000140899F06
  0000000140899431: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899436: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  000000014089943D: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899442: C7 40 10 0C 00 00  mov         dword ptr [rax+10h],0Ch
                    00
  0000000140899449: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089944E: C7 40 40 02 00 00  mov         dword ptr [rax+40h],2
                    00
  0000000140899455: E9 AC 0A 00 00     jmp         0000000140899F06
  000000014089945A: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089945F: C7 40 04 02 00 00  mov         dword ptr [rax+4],2
                    00
  0000000140899466: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089946B: C7 40 10 03 00 00  mov         dword ptr [rax+10h],3
                    00
  0000000140899472: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899477: C7 40 14 0C 00 00  mov         dword ptr [rax+14h],0Ch
                    00
  000000014089947E: E9 83 0A 00 00     jmp         0000000140899F06
  0000000140899483: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899488: C7 40 04 03 00 00  mov         dword ptr [rax+4],3
                    00
  000000014089948F: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899494: C7 40 10 01 00 00  mov         dword ptr [rax+10h],1
                    00
  000000014089949B: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408994A0: C7 40 14 06 00 00  mov         dword ptr [rax+14h],6
                    00
  00000001408994A7: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408994AC: C7 40 18 03 00 00  mov         dword ptr [rax+18h],3
                    00
  00000001408994B3: E9 4E 0A 00 00     jmp         0000000140899F06
  00000001408994B8: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408994BD: C7 40 04 03 00 00  mov         dword ptr [rax+4],3
                    00
  00000001408994C4: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408994C9: C7 40 10 07 00 00  mov         dword ptr [rax+10h],7
                    00
  00000001408994D0: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408994D5: C7 40 14 06 00 00  mov         dword ptr [rax+14h],6
                    00
  00000001408994DC: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408994E1: C7 40 18 03 00 00  mov         dword ptr [rax+18h],3
                    00
  00000001408994E8: E9 19 0A 00 00     jmp         0000000140899F06
  00000001408994ED: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408994F2: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  00000001408994F9: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408994FE: C7 40 10 03 00 00  mov         dword ptr [rax+10h],3
                    00
  0000000140899505: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089950A: C7 40 40 02 00 00  mov         dword ptr [rax+40h],2
                    00
  0000000140899511: E9 F0 09 00 00     jmp         0000000140899F06
  0000000140899516: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089951B: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  0000000140899522: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899527: C7 40 10 03 00 00  mov         dword ptr [rax+10h],3
                    00
  000000014089952E: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899533: C7 40 40 02 00 00  mov         dword ptr [rax+40h],2
                    00
  000000014089953A: E9 C7 09 00 00     jmp         0000000140899F06
  000000014089953F: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899544: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  000000014089954B: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899550: C7 40 10 03 00 00  mov         dword ptr [rax+10h],3
                    00
  0000000140899557: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089955C: C7 40 40 05 00 00  mov         dword ptr [rax+40h],5
                    00
  0000000140899563: E9 9E 09 00 00     jmp         0000000140899F06
  0000000140899568: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089956D: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  0000000140899574: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899579: C7 40 10 03 00 00  mov         dword ptr [rax+10h],3
                    00
  0000000140899580: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899585: C7 40 40 03 00 00  mov         dword ptr [rax+40h],3
                    00
  000000014089958C: E9 75 09 00 00     jmp         0000000140899F06
  0000000140899591: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899596: C7 40 04 03 00 00  mov         dword ptr [rax+4],3
                    00
  000000014089959D: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408995A2: C7 40 10 07 00 00  mov         dword ptr [rax+10h],7
                    00
  00000001408995A9: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408995AE: C7 40 14 08 00 00  mov         dword ptr [rax+14h],8
                    00
  00000001408995B5: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408995BA: C7 40 18 03 00 00  mov         dword ptr [rax+18h],3
                    00
  00000001408995C1: E9 40 09 00 00     jmp         0000000140899F06
  00000001408995C6: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408995CB: C7 40 04 03 00 00  mov         dword ptr [rax+4],3
                    00
  00000001408995D2: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408995D7: C7 40 10 07 00 00  mov         dword ptr [rax+10h],7
                    00
  00000001408995DE: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408995E3: C7 40 14 0C 00 00  mov         dword ptr [rax+14h],0Ch
                    00
  00000001408995EA: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408995EF: C7 40 18 03 00 00  mov         dword ptr [rax+18h],3
                    00
  00000001408995F6: E9 0B 09 00 00     jmp         0000000140899F06
  00000001408995FB: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899600: C7 40 04 03 00 00  mov         dword ptr [rax+4],3
                    00
  0000000140899607: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089960C: C7 40 10 07 00 00  mov         dword ptr [rax+10h],7
                    00
  0000000140899613: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899618: C7 40 14 08 00 00  mov         dword ptr [rax+14h],8
                    00
  000000014089961F: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899624: C7 40 18 03 00 00  mov         dword ptr [rax+18h],3
                    00
  000000014089962B: E9 D6 08 00 00     jmp         0000000140899F06
  0000000140899630: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899635: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  000000014089963C: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899641: C7 40 40 04 00 00  mov         dword ptr [rax+40h],4
                    00
  0000000140899648: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089964D: C7 40 10 03 00 00  mov         dword ptr [rax+10h],3
                    00
  0000000140899654: E9 AD 08 00 00     jmp         0000000140899F06
  0000000140899659: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089965E: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  0000000140899665: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089966A: C7 40 40 06 00 00  mov         dword ptr [rax+40h],6
                    00
  0000000140899671: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899676: C7 40 10 03 00 00  mov         dword ptr [rax+10h],3
                    00
  000000014089967D: E9 84 08 00 00     jmp         0000000140899F06
  0000000140899682: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899687: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  000000014089968E: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899693: C7 40 40 07 00 00  mov         dword ptr [rax+40h],7
                    00
  000000014089969A: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089969F: C7 40 10 03 00 00  mov         dword ptr [rax+10h],3
                    00
  00000001408996A6: E9 5B 08 00 00     jmp         0000000140899F06
  00000001408996AB: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408996B0: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  00000001408996B7: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408996BC: C7 40 40 06 00 00  mov         dword ptr [rax+40h],6
                    00
  00000001408996C3: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408996C8: C7 40 10 03 00 00  mov         dword ptr [rax+10h],3
                    00
  00000001408996CF: E9 32 08 00 00     jmp         0000000140899F06
  00000001408996D4: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408996D9: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  00000001408996E0: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408996E5: C7 40 40 07 00 00  mov         dword ptr [rax+40h],7
                    00
  00000001408996EC: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408996F1: C7 40 10 03 00 00  mov         dword ptr [rax+10h],3
                    00
  00000001408996F8: E9 09 08 00 00     jmp         0000000140899F06
  00000001408996FD: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899702: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  0000000140899709: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089970E: C7 40 40 06 00 00  mov         dword ptr [rax+40h],6
                    00
  0000000140899715: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089971A: C7 40 10 03 00 00  mov         dword ptr [rax+10h],3
                    00
  0000000140899721: E9 E0 07 00 00     jmp         0000000140899F06
  0000000140899726: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089972B: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  0000000140899732: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899737: C7 40 40 09 00 00  mov         dword ptr [rax+40h],9
                    00
  000000014089973E: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899743: C7 40 10 03 00 00  mov         dword ptr [rax+10h],3
                    00
  000000014089974A: E9 B7 07 00 00     jmp         0000000140899F06
  000000014089974F: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899754: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  000000014089975B: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899760: C7 40 40 06 00 00  mov         dword ptr [rax+40h],6
                    00
  0000000140899767: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089976C: C7 40 10 03 00 00  mov         dword ptr [rax+10h],3
                    00
  0000000140899773: E9 8E 07 00 00     jmp         0000000140899F06
  0000000140899778: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089977D: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  0000000140899784: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899789: C7 40 40 03 00 00  mov         dword ptr [rax+40h],3
                    00
  0000000140899790: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899795: C7 40 10 03 00 00  mov         dword ptr [rax+10h],3
                    00
  000000014089979C: E9 65 07 00 00     jmp         0000000140899F06
  00000001408997A1: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408997A6: C7 40 04 00 00 00  mov         dword ptr [rax+4],0
                    00
  00000001408997AD: E9 54 07 00 00     jmp         0000000140899F06
  00000001408997B2: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408997B7: C7 40 04 00 00 00  mov         dword ptr [rax+4],0
                    00
  00000001408997BE: E9 43 07 00 00     jmp         0000000140899F06
  00000001408997C3: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408997C8: C7 40 04 00 00 00  mov         dword ptr [rax+4],0
                    00
  00000001408997CF: E9 32 07 00 00     jmp         0000000140899F06
  00000001408997D4: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408997D9: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  00000001408997E0: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408997E5: C7 40 40 01 00 00  mov         dword ptr [rax+40h],1
                    00
  00000001408997EC: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408997F1: C7 40 10 03 00 00  mov         dword ptr [rax+10h],3
                    00
  00000001408997F8: E9 09 07 00 00     jmp         0000000140899F06
  00000001408997FD: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899802: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  0000000140899809: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089980E: C7 40 40 01 00 00  mov         dword ptr [rax+40h],1
                    00
  0000000140899815: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089981A: C7 40 10 03 00 00  mov         dword ptr [rax+10h],3
                    00
  0000000140899821: E9 E0 06 00 00     jmp         0000000140899F06
  0000000140899826: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089982B: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  0000000140899832: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899837: C7 40 40 01 00 00  mov         dword ptr [rax+40h],1
                    00
  000000014089983E: E9 C3 06 00 00     jmp         0000000140899F06
  0000000140899843: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899848: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  000000014089984F: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899854: C7 40 40 01 00 00  mov         dword ptr [rax+40h],1
                    00
  000000014089985B: E9 A6 06 00 00     jmp         0000000140899F06
  0000000140899860: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899865: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  000000014089986C: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899871: C7 40 40 01 00 00  mov         dword ptr [rax+40h],1
                    00
  0000000140899878: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089987D: C7 40 10 0A 00 00  mov         dword ptr [rax+10h],0Ah
                    00
  0000000140899884: E9 7D 06 00 00     jmp         0000000140899F06
  0000000140899889: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089988E: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  0000000140899895: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089989A: C7 40 40 01 00 00  mov         dword ptr [rax+40h],1
                    00
  00000001408998A1: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408998A6: C7 40 10 0A 00 00  mov         dword ptr [rax+10h],0Ah
                    00
  00000001408998AD: E9 54 06 00 00     jmp         0000000140899F06
  00000001408998B2: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408998B7: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  00000001408998BE: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408998C3: C7 40 1C 01 00 00  mov         dword ptr [rax+1Ch],1
                    00
  00000001408998CA: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408998CF: C7 40 40 01 00 00  mov         dword ptr [rax+40h],1
                    00
  00000001408998D6: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408998DB: C7 40 10 16 00 00  mov         dword ptr [rax+10h],16h
                    00
  00000001408998E2: E9 1F 06 00 00     jmp         0000000140899F06
  00000001408998E7: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408998EC: C7 40 04 00 00 00  mov         dword ptr [rax+4],0
                    00
  00000001408998F3: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408998F8: C7 40 1C 00 00 00  mov         dword ptr [rax+1Ch],0
                    00
  00000001408998FF: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899904: C7 40 40 00 00 00  mov         dword ptr [rax+40h],0
                    00
  000000014089990B: E9 F6 05 00 00     jmp         0000000140899F06
  0000000140899910: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899915: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  000000014089991C: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899921: C7 40 40 01 00 00  mov         dword ptr [rax+40h],1
                    00
  0000000140899928: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089992D: C7 40 10 07 00 00  mov         dword ptr [rax+10h],7
                    00
  0000000140899934: E9 CD 05 00 00     jmp         0000000140899F06
  0000000140899939: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089993E: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  0000000140899945: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089994A: C7 40 40 01 00 00  mov         dword ptr [rax+40h],1
                    00
  0000000140899951: E9 B0 05 00 00     jmp         0000000140899F06
  0000000140899956: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089995B: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  0000000140899962: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899967: C7 40 40 01 00 00  mov         dword ptr [rax+40h],1
                    00
  000000014089996E: E9 93 05 00 00     jmp         0000000140899F06
  0000000140899973: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899978: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  000000014089997F: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899984: C7 40 40 01 00 00  mov         dword ptr [rax+40h],1
                    00
  000000014089998B: E9 76 05 00 00     jmp         0000000140899F06
  0000000140899990: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899995: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  000000014089999C: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408999A1: C7 40 40 01 00 00  mov         dword ptr [rax+40h],1
                    00
  00000001408999A8: E9 59 05 00 00     jmp         0000000140899F06
  00000001408999AD: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408999B2: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  00000001408999B9: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408999BE: C7 40 40 01 00 00  mov         dword ptr [rax+40h],1
                    00
  00000001408999C5: E9 3C 05 00 00     jmp         0000000140899F06
  00000001408999CA: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408999CF: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  00000001408999D6: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408999DB: C7 40 40 01 00 00  mov         dword ptr [rax+40h],1
                    00
  00000001408999E2: E9 1F 05 00 00     jmp         0000000140899F06
  00000001408999E7: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408999EC: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  00000001408999F3: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408999F8: C7 40 40 01 00 00  mov         dword ptr [rax+40h],1
                    00
  00000001408999FF: E9 02 05 00 00     jmp         0000000140899F06
  0000000140899A04: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899A09: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  0000000140899A10: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899A15: C7 40 40 01 00 00  mov         dword ptr [rax+40h],1
                    00
  0000000140899A1C: E9 E5 04 00 00     jmp         0000000140899F06
  0000000140899A21: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899A26: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  0000000140899A2D: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899A32: C7 40 40 01 00 00  mov         dword ptr [rax+40h],1
                    00
  0000000140899A39: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899A3E: C7 40 10 03 00 00  mov         dword ptr [rax+10h],3
                    00
  0000000140899A45: E9 BC 04 00 00     jmp         0000000140899F06
  0000000140899A4A: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899A4F: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  0000000140899A56: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899A5B: C7 40 40 01 00 00  mov         dword ptr [rax+40h],1
                    00
  0000000140899A62: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899A67: C7 40 10 02 00 00  mov         dword ptr [rax+10h],2
                    00
  0000000140899A6E: E9 93 04 00 00     jmp         0000000140899F06
  0000000140899A73: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899A78: C7 40 04 03 00 00  mov         dword ptr [rax+4],3
                    00
  0000000140899A7F: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899A84: C7 40 10 02 00 00  mov         dword ptr [rax+10h],2
                    00
  0000000140899A8B: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899A90: C7 40 14 08 00 00  mov         dword ptr [rax+14h],8
                    00
  0000000140899A97: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899A9C: C7 40 18 03 00 00  mov         dword ptr [rax+18h],3
                    00
  0000000140899AA3: E9 5E 04 00 00     jmp         0000000140899F06
  0000000140899AA8: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899AAD: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  0000000140899AB4: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899AB9: C7 40 40 01 00 00  mov         dword ptr [rax+40h],1
                    00
  0000000140899AC0: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899AC5: C7 40 10 07 00 00  mov         dword ptr [rax+10h],7
                    00
  0000000140899ACC: E9 35 04 00 00     jmp         0000000140899F06
  0000000140899AD1: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899AD6: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  0000000140899ADD: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899AE2: C7 40 1C 00 00 00  mov         dword ptr [rax+1Ch],0
                    00
  0000000140899AE9: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899AEE: C7 40 40 00 00 00  mov         dword ptr [rax+40h],0
                    00
  0000000140899AF5: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899AFA: C7 40 10 00 00 00  mov         dword ptr [rax+10h],0
                    00
  0000000140899B01: E9 00 04 00 00     jmp         0000000140899F06
  0000000140899B06: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899B0B: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  0000000140899B12: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899B17: C7 40 1C 01 00 00  mov         dword ptr [rax+1Ch],1
                    00
  0000000140899B1E: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899B23: C7 40 40 01 00 00  mov         dword ptr [rax+40h],1
                    00
  0000000140899B2A: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899B2F: C7 40 10 01 00 00  mov         dword ptr [rax+10h],1
                    00
  0000000140899B36: E9 CB 03 00 00     jmp         0000000140899F06
  0000000140899B3B: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899B40: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  0000000140899B47: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899B4C: C7 40 1C 01 00 00  mov         dword ptr [rax+1Ch],1
                    00
  0000000140899B53: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899B58: C7 40 40 02 00 00  mov         dword ptr [rax+40h],2
                    00
  0000000140899B5F: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899B64: C7 40 10 00 00 00  mov         dword ptr [rax+10h],0
                    00
  0000000140899B6B: E9 96 03 00 00     jmp         0000000140899F06
  0000000140899B70: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899B75: C7 40 04 00 00 00  mov         dword ptr [rax+4],0
                    00
  0000000140899B7C: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899B81: C7 40 40 01 00 00  mov         dword ptr [rax+40h],1
                    00
  0000000140899B88: E9 79 03 00 00     jmp         0000000140899F06
  0000000140899B8D: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899B92: C7 40 04 00 00 00  mov         dword ptr [rax+4],0
                    00
  0000000140899B99: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899B9E: C7 40 40 01 00 00  mov         dword ptr [rax+40h],1
                    00
  0000000140899BA5: E9 5C 03 00 00     jmp         0000000140899F06
  0000000140899BAA: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899BAF: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  0000000140899BB6: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899BBB: C7 40 1C 01 00 00  mov         dword ptr [rax+1Ch],1
                    00
  0000000140899BC2: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899BC7: C7 40 40 02 00 00  mov         dword ptr [rax+40h],2
                    00
  0000000140899BCE: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899BD3: C7 40 10 00 00 00  mov         dword ptr [rax+10h],0
                    00
  0000000140899BDA: E9 27 03 00 00     jmp         0000000140899F06
  0000000140899BDF: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899BE4: C7 40 04 00 00 00  mov         dword ptr [rax+4],0
                    00
  0000000140899BEB: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899BF0: C7 40 40 01 00 00  mov         dword ptr [rax+40h],1
                    00
  0000000140899BF7: E9 0A 03 00 00     jmp         0000000140899F06
  0000000140899BFC: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899C01: C7 40 04 00 00 00  mov         dword ptr [rax+4],0
                    00
  0000000140899C08: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899C0D: C7 40 40 01 00 00  mov         dword ptr [rax+40h],1
                    00
  0000000140899C14: E9 ED 02 00 00     jmp         0000000140899F06
  0000000140899C19: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899C1E: C7 40 04 02 00 00  mov         dword ptr [rax+4],2
                    00
  0000000140899C25: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899C2A: C7 40 10 03 00 00  mov         dword ptr [rax+10h],3
                    00
  0000000140899C31: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899C36: C7 40 14 08 00 00  mov         dword ptr [rax+14h],8
                    00
  0000000140899C3D: E9 C4 02 00 00     jmp         0000000140899F06
  0000000140899C42: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899C47: C7 40 04 00 00 00  mov         dword ptr [rax+4],0
                    00
  0000000140899C4E: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899C53: C7 40 40 01 00 00  mov         dword ptr [rax+40h],1
                    00
  0000000140899C5A: E9 A7 02 00 00     jmp         0000000140899F06
  0000000140899C5F: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899C64: C7 40 04 00 00 00  mov         dword ptr [rax+4],0
                    00
  0000000140899C6B: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899C70: C7 40 40 01 00 00  mov         dword ptr [rax+40h],1
                    00
  0000000140899C77: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899C7C: F2 0F 10 05 5C AD  movsd       xmm0,mmword ptr [00000001421249E0h]
                    88 01
  0000000140899C84: F2 0F 11 40 08     movsd       mmword ptr [rax+8],xmm0
  0000000140899C89: E9 78 02 00 00     jmp         0000000140899F06
  0000000140899C8E: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899C93: C7 40 04 00 00 00  mov         dword ptr [rax+4],0
                    00
  0000000140899C9A: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899C9F: C7 40 40 02 00 00  mov         dword ptr [rax+40h],2
                    00
  0000000140899CA6: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899CAB: 66 0F 57 C0        xorpd       xmm0,xmm0
  0000000140899CAF: F2 0F 11 40 08     movsd       mmword ptr [rax+8],xmm0
  0000000140899CB4: E9 4D 02 00 00     jmp         0000000140899F06
  0000000140899CB9: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899CBE: C7 40 40 06 00 00  mov         dword ptr [rax+40h],6
                    00
  0000000140899CC5: E9 3C 02 00 00     jmp         0000000140899F06
  0000000140899CCA: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899CCF: C7 40 40 03 00 00  mov         dword ptr [rax+40h],3
                    00
  0000000140899CD6: E9 2B 02 00 00     jmp         0000000140899F06
  0000000140899CDB: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899CE0: C7 40 40 01 00 00  mov         dword ptr [rax+40h],1
                    00
  0000000140899CE7: E9 1A 02 00 00     jmp         0000000140899F06
  0000000140899CEC: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899CF1: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  0000000140899CF8: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899CFD: C7 40 40 06 00 00  mov         dword ptr [rax+40h],6
                    00
  0000000140899D04: E9 FD 01 00 00     jmp         0000000140899F06
  0000000140899D09: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899D0E: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  0000000140899D15: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899D1A: C7 40 40 03 00 00  mov         dword ptr [rax+40h],3
                    00
  0000000140899D21: E9 E0 01 00 00     jmp         0000000140899F06
  0000000140899D26: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899D2B: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  0000000140899D32: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899D37: C7 40 10 00 00 00  mov         dword ptr [rax+10h],0
                    00
  0000000140899D3E: E9 C3 01 00 00     jmp         0000000140899F06
  0000000140899D43: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899D48: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  0000000140899D4F: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899D54: C7 40 10 00 00 00  mov         dword ptr [rax+10h],0
                    00
  0000000140899D5B: E9 A6 01 00 00     jmp         0000000140899F06
  0000000140899D60: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899D65: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  0000000140899D6C: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899D71: C7 40 10 00 00 00  mov         dword ptr [rax+10h],0
                    00
  0000000140899D78: E9 89 01 00 00     jmp         0000000140899F06
  0000000140899D7D: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899D82: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  0000000140899D89: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899D8E: C7 40 10 03 00 00  mov         dword ptr [rax+10h],3
                    00
  0000000140899D95: E9 6C 01 00 00     jmp         0000000140899F06
  0000000140899D9A: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899D9F: C7 40 04 00 00 00  mov         dword ptr [rax+4],0
                    00
  0000000140899DA6: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899DAB: C7 40 40 01 00 00  mov         dword ptr [rax+40h],1
                    00
  0000000140899DB2: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899DB7: 66 0F 57 C0        xorpd       xmm0,xmm0
  0000000140899DBB: F2 0F 11 40 08     movsd       mmword ptr [rax+8],xmm0
  0000000140899DC0: E9 41 01 00 00     jmp         0000000140899F06
  0000000140899DC5: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899DCA: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  0000000140899DD1: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899DD6: C7 40 10 1F 00 00  mov         dword ptr [rax+10h],1Fh
                    00
  0000000140899DDD: E9 24 01 00 00     jmp         0000000140899F06
  0000000140899DE2: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899DE7: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  0000000140899DEE: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899DF3: C7 40 40 01 00 00  mov         dword ptr [rax+40h],1
                    00
  0000000140899DFA: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899DFF: C7 40 10 20 00 00  mov         dword ptr [rax+10h],20h
                    00
  0000000140899E06: E9 FB 00 00 00     jmp         0000000140899F06
  0000000140899E0B: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899E10: C7 40 04 00 00 00  mov         dword ptr [rax+4],0
                    00
  0000000140899E17: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899E1C: C7 40 40 00 00 00  mov         dword ptr [rax+40h],0
                    00
  0000000140899E23: E9 DE 00 00 00     jmp         0000000140899F06
  0000000140899E28: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899E2D: C7 40 04 00 00 00  mov         dword ptr [rax+4],0
                    00
  0000000140899E34: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899E39: 66 0F 57 C0        xorpd       xmm0,xmm0
  0000000140899E3D: F2 0F 11 40 08     movsd       mmword ptr [rax+8],xmm0
  0000000140899E42: E9 BF 00 00 00     jmp         0000000140899F06
  0000000140899E47: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899E4C: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  0000000140899E53: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899E58: C7 40 40 01 00 00  mov         dword ptr [rax+40h],1
                    00
  0000000140899E5F: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899E64: C7 40 10 07 00 00  mov         dword ptr [rax+10h],7
                    00
  0000000140899E6B: E9 96 00 00 00     jmp         0000000140899F06
  0000000140899E70: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899E75: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  0000000140899E7C: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899E81: C7 40 40 01 00 00  mov         dword ptr [rax+40h],1
                    00
  0000000140899E88: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899E8D: C7 40 10 07 00 00  mov         dword ptr [rax+10h],7
                    00
  0000000140899E94: EB 70              jmp         0000000140899F06
  0000000140899E96: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899E9B: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  0000000140899EA2: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899EA7: C7 40 40 01 00 00  mov         dword ptr [rax+40h],1
                    00
  0000000140899EAE: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899EB3: C7 40 10 01 00 00  mov         dword ptr [rax+10h],1
                    00
  0000000140899EBA: EB 4A              jmp         0000000140899F06
  0000000140899EBC: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899EC1: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  0000000140899EC8: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899ECD: C7 40 40 01 00 00  mov         dword ptr [rax+40h],1
                    00
  0000000140899ED4: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899ED9: C7 40 10 01 00 00  mov         dword ptr [rax+10h],1
                    00
  0000000140899EE0: EB 24              jmp         0000000140899F06
  0000000140899EE2: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899EE7: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  0000000140899EEE: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899EF3: C7 40 40 01 00 00  mov         dword ptr [rax+40h],1
                    00
  0000000140899EFA: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140899EFF: C7 40 10 01 00 00  mov         dword ptr [rax+10h],1
                    00
  0000000140899F06: 83 7C 24 48 65     cmp         dword ptr [rsp+48h],65h
  0000000140899F0B: 74 0E              je          0000000140899F1B
  0000000140899F0D: 83 7C 24 48 66     cmp         dword ptr [rsp+48h],66h
  0000000140899F12: 74 07              je          0000000140899F1B
  0000000140899F14: 83 7C 24 48 67     cmp         dword ptr [rsp+48h],67h
  0000000140899F19: 75 02              jne         0000000140899F1D
  0000000140899F1B: EB 13              jmp         0000000140899F30
  0000000140899F1D: 83 7C 24 48 51     cmp         dword ptr [rsp+48h],51h
  0000000140899F22: 75 02              jne         0000000140899F26
  0000000140899F24: EB 0A              jmp         0000000140899F30
  0000000140899F26: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140899F2B: E8 10 19 00 00     call        000000014089B840
  0000000140899F30: 48 83 C4 38        add         rsp,38h
  0000000140899F34: C3                 ret
  0000000140899F35: 0F 1F 00           nop         dword ptr [rax]
  0000000140899F38: 7D 9D              jge         0000000140899ED7
  0000000140899F3A: 89 00              mov         dword ptr [rax],eax
  0000000140899F3C: 4C 93              xchg        rax,rbx
  0000000140899F3E: 89 00              mov         dword ptr [rax],eax
  0000000140899F40: 0C 91              or          al,91h
  0000000140899F42: 89 00              mov         dword ptr [rax],eax
  0000000140899F44: C5
  0000000140899F45: 9D                 popfq
  0000000140899F46: 89 00              mov         dword ptr [rax],eax
  0000000140899F48: 08 94 89 00 31 94  or          byte ptr [rcx+rcx*4+FFFFFFFF89943100h],dl
                    89
  0000000140899F4F: 00 5A 94           add         byte ptr [rdx-6Ch],bl
  0000000140899F52: 89 00              mov         dword ptr [rax],eax
  0000000140899F54: 6F                 outs        dx,dword ptr [rsi]
  0000000140899F55: 91                 xchg        eax,ecx
  0000000140899F56: 89 00              mov         dword ptr [rax],eax
  0000000140899F58: 0E
  0000000140899F59: 92                 xchg        eax,edx
  0000000140899F5A: 89 00              mov         dword ptr [rax],eax
  0000000140899F5C: 4F 92              xchg        rax,r10
  0000000140899F5E: 89 00              mov         dword ptr [rax],eax
  0000000140899F60: 90                 nop
  0000000140899F61: 92                 xchg        eax,edx
  0000000140899F62: 89 00              mov         dword ptr [rax],eax
  0000000140899F64: D1 92 89 00 12 93  rcl         dword ptr [rdx+FFFFFFFF93120089h],1
  0000000140899F6A: 89 00              mov         dword ptr [rax],eax
  0000000140899F6C: 2F
  0000000140899F6D: 93                 xchg        eax,ebx
  0000000140899F6E: 89 00              mov         dword ptr [rax],eax
  0000000140899F70: B0 91              mov         al,91h
  0000000140899F72: 89 00              mov         dword ptr [rax],eax
  0000000140899F74: F1
  0000000140899F75: 91                 xchg        eax,ecx
  0000000140899F76: 89 00              mov         dword ptr [rax],eax
  0000000140899F78: 83 94 89 00 B8 94  adc         dword ptr [rcx+rcx*4+FFFFFFFF8994B800h],0
                    89 00
  0000000140899F80: ED                 in          eax,dx
  0000000140899F81: 94                 xchg        eax,esp
  0000000140899F82: 89 00              mov         dword ptr [rax],eax
  0000000140899F84: 16
  0000000140899F85: 95                 xchg        eax,ebp
  0000000140899F86: 89 00              mov         dword ptr [rax],eax
  0000000140899F88: 3F
  0000000140899F89: 95                 xchg        eax,ebp
  0000000140899F8A: 89 00              mov         dword ptr [rax],eax
  0000000140899F8C: 68 95 89 00 91     push        0FFFFFFFF91008995h
  0000000140899F91: 95                 xchg        eax,ebp
  0000000140899F92: 89 00              mov         dword ptr [rax],eax
  0000000140899F94: C6
  0000000140899F95: 95                 xchg        eax,ebp
  0000000140899F96: 89 00              mov         dword ptr [rax],eax
  0000000140899F98: FB                 sti
  0000000140899F99: 95                 xchg        eax,ebp
  0000000140899F9A: 89 00              mov         dword ptr [rax],eax
  0000000140899F9C: 29 91 89 00 52 91  sub         dword ptr [rcx+FFFFFFFF91520089h],edx
  0000000140899FA2: 89 00              mov         dword ptr [rax],eax
  0000000140899FA4: 59                 pop         rcx
  0000000140899FA5: 96                 xchg        eax,esi
  0000000140899FA6: 89 00              mov         dword ptr [rax],eax
  0000000140899FA8: 30 96 89 00 82 96  xor         byte ptr [rsi+FFFFFFFF96820089h],dl
  0000000140899FAE: 89 00              mov         dword ptr [rax],eax
  0000000140899FB0: AB                 stos        dword ptr [rdi]
  0000000140899FB1: 96                 xchg        eax,esi
  0000000140899FB2: 89 00              mov         dword ptr [rax],eax
  0000000140899FB4: D4
  0000000140899FB5: 96                 xchg        eax,esi
  0000000140899FB6: 89 00              mov         dword ptr [rax],eax
  0000000140899FB8: FD                 std
  0000000140899FB9: 96                 xchg        eax,esi
  0000000140899FBA: 89 00              mov         dword ptr [rax],eax
  0000000140899FBC: 26 97              xchg        eax,edi
  0000000140899FBE: 89 00              mov         dword ptr [rax],eax
  0000000140899FC0: 4F 97              xchg        rax,r15
  0000000140899FC2: 89 00              mov         dword ptr [rax],eax
  0000000140899FC4: 78 97              js          0000000140899F5D
  0000000140899FC6: 89 00              mov         dword ptr [rax],eax
  0000000140899FC8: A1 97 89 00 B2 97  mov         eax,dword ptr [C3008997B2008997h]
                    89 00 C3
  0000000140899FD1: 97                 xchg        eax,edi
  0000000140899FD2: 89 00              mov         dword ptr [rax],eax
  0000000140899FD4: B9 9C 89 00 CA     mov         ecx,0CA00899Ch
  0000000140899FD9: 9C                 pushfq
  0000000140899FDA: 89 00              mov         dword ptr [rax],eax
  0000000140899FDC: DB 9C 89 00 EC 9C  fistp       dword ptr [rcx+rcx*4+FFFFFFFF899CEC00h]
                    89
  0000000140899FE3: 00 09              add         byte ptr [rcx],cl
  0000000140899FE5: 9D                 popfq
  0000000140899FE6: 89 00              mov         dword ptr [rax],eax
  0000000140899FE8: D4
  0000000140899FE9: 97                 xchg        eax,edi
  0000000140899FEA: 89 00              mov         dword ptr [rax],eax
  0000000140899FEC: FD                 std
  0000000140899FED: 97                 xchg        eax,edi
  0000000140899FEE: 89 00              mov         dword ptr [rax],eax
  0000000140899FF0: 26 98              cwde
  0000000140899FF2: 89 00              mov         dword ptr [rax],eax
  0000000140899FF4: 43 98              cwde
  0000000140899FF6: 89 00              mov         dword ptr [rax],eax
  0000000140899FF8: 60
  0000000140899FF9: 98                 cwde
  0000000140899FFA: 89 00              mov         dword ptr [rax],eax
  0000000140899FFC: 89 98 89 00 69 93  mov         dword ptr [rax+FFFFFFFF93690089h],ebx
  000000014089A002: 89 00              mov         dword ptr [rax],eax
  000000014089A004: 9E                 sahf
  000000014089A005: 93                 xchg        eax,ebx
  000000014089A006: 89 00              mov         dword ptr [rax],eax
  000000014089A008: D3 93 89 00 B2 98  rcl         dword ptr [rbx+FFFFFFFF98B20089h],cl
  000000014089A00E: 89 00              mov         dword ptr [rax],eax
  000000014089A010: E7 98              out         98h,eax
  000000014089A012: 89 00              mov         dword ptr [rax],eax
  000000014089A014: 10 99 89 00 21 9A  adc         byte ptr [rcx+FFFFFFFF9A210089h],bl
  000000014089A01A: 89 00              mov         dword ptr [rax],eax
  000000014089A01C: 39 99 89 00 56 99  cmp         dword ptr [rcx+FFFFFFFF99560089h],ebx
  000000014089A022: 89 00              mov         dword ptr [rax],eax
  000000014089A024: 73 99              jae         0000000140899FBF
  000000014089A026: 89 00              mov         dword ptr [rax],eax
  000000014089A028: 90                 nop
  000000014089A029: 99                 cdq
  000000014089A02A: 89 00              mov         dword ptr [rax],eax
  000000014089A02C: AD                 lods        dword ptr [rsi]
  000000014089A02D: 99                 cdq
  000000014089A02E: 89 00              mov         dword ptr [rax],eax
  000000014089A030: CA 99 89           retf        8999h
  000000014089A033: 00 E7              add         bh,ah
  000000014089A035: 99                 cdq
  000000014089A036: 89 00              mov         dword ptr [rax],eax
  000000014089A038: 04 9A              add         al,9Ah
  000000014089A03A: 89 00              mov         dword ptr [rax],eax
  000000014089A03C: 4A
  000000014089A03D: 9A
  000000014089A03E: 89 00              mov         dword ptr [rax],eax
  000000014089A040: 73 9A              jae         0000000140899FDC
  000000014089A042: 89 00              mov         dword ptr [rax],eax
  000000014089A044: A8 9A              test        al,9Ah
  000000014089A046: 89 00              mov         dword ptr [rax],eax
  000000014089A048: D1 9A 89 00 06 9B  rcr         dword ptr [rdx+FFFFFFFF9B060089h],1
  000000014089A04E: 89 00              mov         dword ptr [rax],eax
  000000014089A050: 3B 9B 89 00 70 9B  cmp         ebx,dword ptr [rbx+FFFFFFFF9B700089h]
  000000014089A056: 89 00              mov         dword ptr [rax],eax
  000000014089A058: 8D 9B 89 00 AA 9B  lea         ebx,[rbx+FFFFFFFF9BAA0089h]
  000000014089A05E: 89 00              mov         dword ptr [rax],eax
  000000014089A060: DF 9B 89 00 FC 9B  fistp       word ptr [rbx+FFFFFFFF9BFC0089h]
  000000014089A066: 89 00              mov         dword ptr [rax],eax
  000000014089A068: 19 9C 89 00 42 9C  sbb         dword ptr [rcx+rcx*4+FFFFFFFF899C4200h],ebx
                    89
  000000014089A06F: 00 5F 9C           add         byte ptr [rdi-64h],bl
  000000014089A072: 89 00              mov         dword ptr [rax],eax
  000000014089A074: 8E 9C 89 00 26 9D  mov         ds,word ptr [rcx+rcx*4+FFFFFFFF899D2600h]
                    89
  000000014089A07B: 00 43 9D           add         byte ptr [rbx-63h],al
  000000014089A07E: 89 00              mov         dword ptr [rax],eax
  000000014089A080: 60
  000000014089A081: 9D                 popfq
  000000014089A082: 89 00              mov         dword ptr [rax],eax
  000000014089A084: 9A
  000000014089A085: 9D                 popfq
  000000014089A086: 89 00              mov         dword ptr [rax],eax
  000000014089A088: E2 9D              loop        000000014089A027
  000000014089A08A: 89 00              mov         dword ptr [rax],eax
  000000014089A08C: 0B 9E 89 00 28 9E  or          ebx,dword ptr [rsi+FFFFFFFF9E280089h]
  000000014089A092: 89 00              mov         dword ptr [rax],eax
  000000014089A094: 47 9E              sahf
  000000014089A096: 89 00              mov         dword ptr [rax],eax
  000000014089A098: 70 9E              jo          000000014089A038
  000000014089A09A: 89 00              mov         dword ptr [rax],eax
  000000014089A09C: 96                 xchg        eax,esi
  000000014089A09D: 9E                 sahf
  000000014089A09E: 89 00              mov         dword ptr [rax],eax
  000000014089A0A0: BC 9E 89 00 E2     mov         esp,0E200899Eh
  000000014089A0A5: 9E                 sahf
  000000014089A0A6: 89 00              mov         dword ptr [rax],eax
  000000014089A0A8: 06
  000000014089A0A9: 9F                 lahf
  000000014089A0AA: 89 00              mov         dword ptr [rax],eax
  000000014089A0AC: 00 5C 5C 01        add         byte ptr [rsp+rbx*2+1],bl
  000000014089A0B0: 5C                 pop         rsp
  000000014089A0B1: 5C                 pop         rsp
  000000014089A0B2: 02 02              add         al,byte ptr [rdx]
  000000014089A0B4: 02 02              add         al,byte ptr [rdx]
  000000014089A0B6: 02 02              add         al,byte ptr [rdx]
  000000014089A0B8: 02 02              add         al,byte ptr [rdx]
  000000014089A0BA: 5C                 pop         rsp
  000000014089A0BB: 5C                 pop         rsp
  000000014089A0BC: 5C                 pop         rsp
  000000014089A0BD: 5C                 pop         rsp
  000000014089A0BE: 5C                 pop         rsp
  000000014089A0BF: 5C                 pop         rsp
  000000014089A0C0: 5C                 pop         rsp
  000000014089A0C1: 03 5C 5C 5C        add         ebx,dword ptr [rsp+rbx*2+5Ch]
  000000014089A0C5: 5C                 pop         rsp
  000000014089A0C6: 5C                 pop         rsp
  000000014089A0C7: 5C                 pop         rsp
  000000014089A0C8: 5C                 pop         rsp
  000000014089A0C9: 5C                 pop         rsp
  000000014089A0CA: 5C                 pop         rsp
  000000014089A0CB: 5C                 pop         rsp
  000000014089A0CC: 04 05              add         al,5
  000000014089A0CE: 06
  000000014089A0CF: 5C                 pop         rsp
  000000014089A0D0: 5C                 pop         rsp
  000000014089A0D1: 07
  000000014089A0D2: 08 09              or          byte ptr [rcx],cl
  000000014089A0D4: 0A 0B              or          cl,byte ptr [rbx]
  000000014089A0D6: 5C                 pop         rsp
  000000014089A0D7: 0C 5C              or          al,5Ch
  000000014089A0D9: 5C                 pop         rsp
  000000014089A0DA: 5C                 pop         rsp
  000000014089A0DB: 0D 0E 0F 5C 5C     or          eax,5C5C0F0Eh
  000000014089A0E0: 5C                 pop         rsp
  000000014089A0E1: 5C                 pop         rsp
  000000014089A0E2: 5C                 pop         rsp
  000000014089A0E3: 5C                 pop         rsp
  000000014089A0E4: 5C                 pop         rsp
  000000014089A0E5: 5C                 pop         rsp
  000000014089A0E6: 5C                 pop         rsp
  000000014089A0E7: 5C                 pop         rsp
  000000014089A0E8: 10 11              adc         byte ptr [rcx],dl
  000000014089A0EA: 12 13              adc         dl,byte ptr [rbx]
  000000014089A0EC: 14 15              adc         al,15h
  000000014089A0EE: 5C                 pop         rsp
  000000014089A0EF: 5C                 pop         rsp
  000000014089A0F0: 5C                 pop         rsp
  000000014089A0F1: 5C                 pop         rsp
  000000014089A0F2: 5C                 pop         rsp
  000000014089A0F3: 5C                 pop         rsp
  000000014089A0F4: 16
  000000014089A0F5: 17
  000000014089A0F6: 18 19              sbb         byte ptr [rcx],bl
  000000014089A0F8: 1A 1B              sbb         bl,byte ptr [rbx]
  000000014089A0FA: 1C 1D              sbb         al,1Dh
  000000014089A0FC: 1E
  000000014089A0FD: 1F
  000000014089A0FE: 20 21              and         byte ptr [rcx],ah
  000000014089A100: 22 23              and         ah,byte ptr [rbx]
  000000014089A102: 24 25              and         al,25h
  000000014089A104: 25 25 26 26 26     and         eax,26262625h
  000000014089A109: 26
  000000014089A10A: 27
  000000014089A10B: 28 29              sub         byte ptr [rcx],ch
  000000014089A10D: 2A 2B              sub         ch,byte ptr [rbx]
  000000014089A10F: 5C                 pop         rsp
  000000014089A110: 5C                 pop         rsp
  000000014089A111: 5C                 pop         rsp
  000000014089A112: 5C                 pop         rsp
  000000014089A113: 5C                 pop         rsp
  000000014089A114: 5C                 pop         rsp
  000000014089A115: 2C 2D              sub         al,2Dh
  000000014089A117: 2E
  000000014089A118: 2F
  000000014089A119: 30 31              xor         byte ptr [rcx],dh
  000000014089A11B: 5C                 pop         rsp
  000000014089A11C: 5C                 pop         rsp
  000000014089A11D: 5C                 pop         rsp
  000000014089A11E: 32 33              xor         dh,byte ptr [rbx]
  000000014089A120: 34 35              xor         al,35h
  000000014089A122: 5C                 pop         rsp
  000000014089A123: 5C                 pop         rsp
  000000014089A124: 5C                 pop         rsp
  000000014089A125: 36
  000000014089A126: 37
  000000014089A127: 38 39              cmp         byte ptr [rcx],bh
  000000014089A129: 3A 3B              cmp         bh,byte ptr [rbx]
  000000014089A12B: 3C 3D              cmp         al,3Dh
  000000014089A12D: 3E
  000000014089A12E: 3F
  000000014089A12F: 40
  000000014089A130: 41
  000000014089A131: 42
  000000014089A132: 43
  000000014089A133: 44
  000000014089A134: 44
  000000014089A135: 44
  000000014089A136: 44
  000000014089A137: 44
  000000014089A138: 44
  000000014089A139: 44
  000000014089A13A: 44
  000000014089A13B: 44
  000000014089A13C: 44
  000000014089A13D: 45
  000000014089A13E: 44
  000000014089A13F: 44
  000000014089A140: 44
  000000014089A141: 44
  000000014089A142: 44
  000000014089A143: 44
  000000014089A144: 44
  000000014089A145: 44
  000000014089A146: 44
  000000014089A147: 44
  000000014089A148: 44 5C              pop         rsp
  000000014089A14A: 5C                 pop         rsp
  000000014089A14B: 46
  000000014089A14C: 47
  000000014089A14D: 48
  000000014089A14E: 49
  000000014089A14F: 4A
  000000014089A150: 4B
  000000014089A151: 4C
  000000014089A152: 4D
  000000014089A153: 4E
  000000014089A154: 4F 50              push        r8
  000000014089A156: 51                 push        rcx
  000000014089A157: 52                 push        rdx
  000000014089A158: 53                 push        rbx
  000000014089A159: 54                 push        rsp
  000000014089A15A: 55                 push        rbp
  000000014089A15B: 56                 push        rsi
  000000014089A15C: 57                 push        rdi
  000000014089A15D: 58                 pop         rax
  000000014089A15E: 59                 pop         rcx
  000000014089A15F: 5A                 pop         rdx
  000000014089A160: 5B                 pop         rbx

  Summary

     1E02000 .text
