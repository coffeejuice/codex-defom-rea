Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400548C0: 88 54 24 10        mov         byte ptr [rsp+10h],dl
  00000001400548C4: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400548C9: 48 83 EC 48        sub         rsp,48h
  00000001400548CD: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001400548D2: E8 B9 05 00 00     call        0000000140054E90
  00000001400548D7: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001400548DC: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  00000001400548E2: 75 02              jne         00000001400548E6
  00000001400548E4: EB 50              jmp         0000000140054936
  00000001400548E6: 48 8D 0D CB 1D D2  lea         rcx,[0000000143D766B8h]
                    03
  00000001400548ED: E8 AE 6E 27 00     call        00000001402CB7A0
  00000001400548F2: 83 F8 05           cmp         eax,5
  00000001400548F5: 75 0A              jne         0000000140054901
  00000001400548F7: C7 44 24 30 01 00  mov         dword ptr [rsp+30h],1
                    00 00
  00000001400548FF: EB 08              jmp         0000000140054909
  0000000140054901: C7 44 24 30 00 00  mov         dword ptr [rsp+30h],0
                    00 00
  0000000140054909: 0F B6 44 24 30     movzx       eax,byte ptr [rsp+30h]
  000000014005490E: 88 44 24 20        mov         byte ptr [rsp+20h],al
  0000000140054912: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  0000000140054917: 85 C0              test        eax,eax
  0000000140054919: 74 11              je          000000014005492C
  000000014005491B: 0F B6 54 24 58     movzx       edx,byte ptr [rsp+58h]
  0000000140054920: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140054925: E8 26 00 00 00     call        0000000140054950
  000000014005492A: EB 0A              jmp         0000000140054936
  000000014005492C: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140054931: E8 4A 03 00 00     call        0000000140054C80
  0000000140054936: 48 83 C4 48        add         rsp,48h
  000000014005493A: C3                 ret

  Summary

     1E02000 .text
