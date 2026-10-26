Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014091B5D0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014091B5D5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014091B5DA: 48 83 EC 68        sub         rsp,68h
  000000014091B5DE: 48 C7 44 24 40 FE  mov         qword ptr [rsp+40h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000014091B5E7: C7 44 24 24 FF FF  mov         dword ptr [rsp+24h],0FFFFFFFFh
                    FF FF
  000000014091B5EF: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  000000014091B5F7: EB 0B              jmp         000000014091B604
  000000014091B5F9: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  000000014091B5FD: 83 C0 01           add         eax,1
  000000014091B600: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  000000014091B604: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  000000014091B609: 48 83 C1 38        add         rcx,38h
  000000014091B60D: E8 CE 94 6E FF     call        0000000140004AE0
  000000014091B612: 39 44 24 20        cmp         dword ptr [rsp+20h],eax
  000000014091B616: 7D 67              jge         000000014091B67F
  000000014091B618: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  000000014091B61D: 48 83 C1 38        add         rcx,38h
  000000014091B621: 8B 54 24 20        mov         edx,dword ptr [rsp+20h]
  000000014091B625: E8 E6 55 84 FF     call        0000000140160C10
  000000014091B62A: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  000000014091B62F: 48 8B 08           mov         rcx,qword ptr [rax]
  000000014091B632: E8 89 91 9E FF     call        00000001403047C0
  000000014091B637: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  000000014091B63C: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000014091B641: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  000000014091B646: 48 8B 54 24 78     mov         rdx,qword ptr [rsp+78h]
  000000014091B64B: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014091B650: FF 15 B2 88 4E 01  call        qword ptr [0000000141E03F08h]
  000000014091B656: 88 44 24 28        mov         byte ptr [rsp+28h],al
  000000014091B65A: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  000000014091B65F: FF 15 7B A3 4E 01  call        qword ptr [0000000141E059E0h]
  000000014091B665: 44 0F B6 5C 24 28  movzx       r11d,byte ptr [rsp+28h]
  000000014091B66B: 45 85 DB           test        r11d,r11d
  000000014091B66E: 74 0A              je          000000014091B67A
  000000014091B670: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  000000014091B674: 89 44 24 24        mov         dword ptr [rsp+24h],eax
  000000014091B678: EB 05              jmp         000000014091B67F
  000000014091B67A: E9 7A FF FF FF     jmp         000000014091B5F9
  000000014091B67F: 8B 44 24 24        mov         eax,dword ptr [rsp+24h]
  000000014091B683: 89 44 24 38        mov         dword ptr [rsp+38h],eax
  000000014091B687: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  000000014091B68C: FF 15 4E A3 4E 01  call        qword ptr [0000000141E059E0h]
  000000014091B692: 8B 44 24 38        mov         eax,dword ptr [rsp+38h]
  000000014091B696: 48 83 C4 68        add         rsp,68h
  000000014091B69A: C3                 ret

  Summary

     1E02000 .text
