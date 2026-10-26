Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014006D490: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014006D495: 48 83 EC 38        sub         rsp,38h
  000000014006D499: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014006D49E: 48 8B 00           mov         rax,qword ptr [rax]
  000000014006D4A1: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014006D4A6: FF 90 D8 01 00 00  call        qword ptr [rax+00000000000001D8h]
  000000014006D4AC: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  000000014006D4B0: C7 44 24 24 01 00  mov         dword ptr [rsp+24h],1
                    00 00
  000000014006D4B8: EB 0B              jmp         000000014006D4C5
  000000014006D4BA: 8B 44 24 24        mov         eax,dword ptr [rsp+24h]
  000000014006D4BE: 83 C0 01           add         eax,1
  000000014006D4C1: 89 44 24 24        mov         dword ptr [rsp+24h],eax
  000000014006D4C5: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  000000014006D4C9: 39 44 24 24        cmp         dword ptr [rsp+24h],eax
  000000014006D4CD: 7F 4C              jg          000000014006D51B
  000000014006D4CF: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014006D4D4: 48 8B 00           mov         rax,qword ptr [rax]
  000000014006D4D7: 48 8D 54 24 24     lea         rdx,[rsp+24h]
  000000014006D4DC: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014006D4E1: FF 90 C8 01 00 00  call        qword ptr [rax+00000000000001C8h]
  000000014006D4E7: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000014006D4EC: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  000000014006D4F2: 74 25              je          000000014006D519
  000000014006D4F4: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  000000014006D4F9: E8 A2 E6 FF FF     call        000000014006BBA0
  000000014006D4FE: 85 C0              test        eax,eax
  000000014006D500: 75 17              jne         000000014006D519
  000000014006D502: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014006D507: 48 8B 00           mov         rax,qword ptr [rax]
  000000014006D50A: 8B 54 24 24        mov         edx,dword ptr [rsp+24h]
  000000014006D50E: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014006D513: FF 90 A0 01 00 00  call        qword ptr [rax+00000000000001A0h]
  000000014006D519: EB 9F              jmp         000000014006D4BA
  000000014006D51B: 48 83 C4 38        add         rsp,38h
  000000014006D51F: C3                 ret

  Summary

     1E02000 .text
