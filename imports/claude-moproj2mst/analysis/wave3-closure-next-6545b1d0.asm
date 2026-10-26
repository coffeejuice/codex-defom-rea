Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006545B1D0: 40 57              push        rdi
  000000006545B1D2: 48 83 EC 30        sub         rsp,30h
  000000006545B1D6: 48 C7 44 24 28 FE  mov         qword ptr [rsp+28h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006545B1DF: 48 89 5C 24 40     mov         qword ptr [rsp+40h],rbx
  000000006545B1E4: 48 8B DA           mov         rbx,rdx
  000000006545B1E7: 48 8B F9           mov         rdi,rcx
  000000006545B1EA: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  000000006545B1EF: 48 8B CB           mov         rcx,rbx
  000000006545B1F2: E8 49 98 BD FF     call        0000000065034A40
  000000006545B1F7: 83 38 00           cmp         dword ptr [rax],0
  000000006545B1FA: 74 30              je          000000006545B22C
  000000006545B1FC: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  000000006545B201: 48 8B CB           mov         rcx,rbx
  000000006545B204: E8 37 98 BD FF     call        0000000065034A40
  000000006545B209: 44 8B 00           mov         r8d,dword ptr [rax]
  000000006545B20C: 41 81 E0 00 00 00  and         r8d,20000000h
                    20
  000000006545B213: 74 09              je          000000006545B21E
  000000006545B215: 81 7B 28 05 00 00  cmp         dword ptr [rbx+28h],1000005h
                    01
  000000006545B21C: 74 0E              je          000000006545B22C
  000000006545B21E: 48 8B CB           mov         rcx,rbx
  000000006545B221: FF 15 71 BB 23 00  call        qword ptr [0000000065696D98h]
  000000006545B227: E9 F4 00 00 00     jmp         000000006545B320
  000000006545B22C: 8B 43 28           mov         eax,dword ptr [rbx+28h]
  000000006545B22F: 3D 00 00 00 01     cmp         eax,1000000h
  000000006545B234: 0F 84 DA 00 00 00  je          000000006545B314
  000000006545B23A: 05 FC FF FF FE     add         eax,0FEFFFFFCh
  000000006545B23F: 83 F8 01           cmp         eax,1
  000000006545B242: 0F 87 C1 00 00 00  ja          000000006545B309
  000000006545B248: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006545B24D: FF 15 15 B5 23 00  call        qword ptr [0000000065696768h]
  000000006545B253: 90                 nop
  000000006545B254: 4C 8B C0           mov         r8,rax
  000000006545B257: 48 8D 54 24 58     lea         rdx,[rsp+58h]
  000000006545B25C: 48 8B CF           mov         rcx,rdi
  000000006545B25F: E8 AC 0A F4 FF     call        000000006539BD10
  000000006545B264: 90                 nop
  000000006545B265: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006545B26A: FF 15 F0 B4 23 00  call        qword ptr [0000000065696760h]
  000000006545B270: 33 DB              xor         ebx,ebx
  000000006545B272: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  000000006545B277: FF 15 3B B6 23 00  call        qword ptr [00000000656968B8h]
  000000006545B27D: 85 C0              test        eax,eax
  000000006545B27F: 7E 6C              jle         000000006545B2ED
  000000006545B281: 8B D3              mov         edx,ebx
  000000006545B283: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  000000006545B288: FF 15 B2 B6 23 00  call        qword ptr [0000000065696940h]
  000000006545B28E: 48 8B 38           mov         rdi,qword ptr [rax]
  000000006545B291: 48 8B CF           mov         rcx,rdi
  000000006545B294: E8 27 C1 F9 FF     call        00000000653F73C0
  000000006545B299: 84 C0              test        al,al
  000000006545B29B: 74 0F              je          000000006545B2AC
  000000006545B29D: 48 8B 47 20        mov         rax,qword ptr [rdi+20h]
  000000006545B2A1: 8B 48 08           mov         ecx,dword ptr [rax+8]
  000000006545B2A4: C1 E9 0F           shr         ecx,0Fh
  000000006545B2A7: 80 E1 01           and         cl,1
  000000006545B2AA: 75 13              jne         000000006545B2BF
  000000006545B2AC: FF C3              inc         ebx
  000000006545B2AE: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  000000006545B2B3: FF 15 FF B5 23 00  call        qword ptr [00000000656968B8h]
  000000006545B2B9: 3B D8              cmp         ebx,eax
  000000006545B2BB: 7D 30              jge         000000006545B2ED
  000000006545B2BD: EB C2              jmp         000000006545B281
  000000006545B2BF: 0F B6 50 08        movzx       edx,byte ptr [rax+8]
  000000006545B2C3: 80 E2 01           and         dl,1
  000000006545B2C6: 75 09              jne         000000006545B2D1
  000000006545B2C8: 48 8B CF           mov         rcx,rdi
  000000006545B2CB: E8 10 6C F1 FF     call        0000000065371EE0
  000000006545B2D0: 90                 nop
  000000006545B2D1: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  000000006545B2D6: FF 15 9C B6 23 00  call        qword ptr [0000000065696978h]
  000000006545B2DC: 84 C0              test        al,al
  000000006545B2DE: 75 40              jne         000000006545B320
  000000006545B2E0: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  000000006545B2E5: FF 15 75 B6 23 00  call        qword ptr [0000000065696960h]
  000000006545B2EB: EB 33              jmp         000000006545B320
  000000006545B2ED: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  000000006545B2F2: FF 15 80 B6 23 00  call        qword ptr [0000000065696978h]
  000000006545B2F8: 84 C0              test        al,al
  000000006545B2FA: 75 24              jne         000000006545B320
  000000006545B2FC: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  000000006545B301: FF 15 59 B6 23 00  call        qword ptr [0000000065696960h]
  000000006545B307: EB 17              jmp         000000006545B320
  000000006545B309: 48 8B CB           mov         rcx,rbx
  000000006545B30C: FF 15 86 BA 23 00  call        qword ptr [0000000065696D98h]
  000000006545B312: EB 0C              jmp         000000006545B320
  000000006545B314: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006545B317: 48 8B CF           mov         rcx,rdi
  000000006545B31A: FF 90 A0 01 00 00  call        qword ptr [rax+00000000000001A0h]
  000000006545B320: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  000000006545B325: 48 83 C4 30        add         rsp,30h
  000000006545B329: 5F                 pop         rdi
  000000006545B32A: C3                 ret

  Summary

      695000 .text
