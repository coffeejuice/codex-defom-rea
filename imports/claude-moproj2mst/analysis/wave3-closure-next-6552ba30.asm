Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006552BA30: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000006552BA35: 53                 push        rbx
  000000006552BA36: 48 83 EC 30        sub         rsp,30h
  000000006552BA3A: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006552BA43: 48 8B D9           mov         rbx,rcx
  000000006552BA46: 48 8B 0A           mov         rcx,qword ptr [rdx]
  000000006552BA49: 48 89 0B           mov         qword ptr [rbx],rcx
  000000006552BA4C: 48 83 C1 10        add         rcx,10h
  000000006552BA50: FF 15 6A AE 16 00  call        qword ptr [00000000656968C0h]
  000000006552BA56: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000006552BA59: F6 41 24 01        test        byte ptr [rcx+24h],1
  000000006552BA5D: 75 1C              jne         000000006552BA7B
  000000006552BA5F: 48 83 C1 10        add         rcx,10h
  000000006552BA63: BA 01 00 00 00     mov         edx,1
  000000006552BA68: FF 15 02 B0 16 00  call        qword ptr [0000000065696A70h]
  000000006552BA6E: 84 C0              test        al,al
  000000006552BA70: 74 09              je          000000006552BA7B
  000000006552BA72: 48 8B CB           mov         rcx,rbx
  000000006552BA75: E8 16 03 D3 FF     call        000000006525BD90
  000000006552BA7A: 90                 nop
  000000006552BA7B: C7 43 08 00 00 00  mov         dword ptr [rbx+8],0
                    00
  000000006552BA82: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000006552BA85: FF 15 0D B1 16 00  call        qword ptr [0000000065696B98h]
  000000006552BA8B: 48 89 43 10        mov         qword ptr [rbx+10h],rax
  000000006552BA8F: 48 8B 03           mov         rax,qword ptr [rbx]
  000000006552BA92: 48 89 43 18        mov         qword ptr [rbx+18h],rax
  000000006552BA96: 48 8B C3           mov         rax,rbx
  000000006552BA99: 48 83 C4 30        add         rsp,30h
  000000006552BA9D: 5B                 pop         rbx
  000000006552BA9E: C3                 ret

  Summary

      695000 .text
