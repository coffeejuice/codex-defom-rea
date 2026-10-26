Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001416BF560: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001416BF565: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001416BF56A: 57                 push        rdi
  00000001416BF56B: 48 83 EC 50        sub         rsp,50h
  00000001416BF56F: 48 C7 44 24 40 FE  mov         qword ptr [rsp+40h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001416BF578: 48 8D 44 24 20     lea         rax,[rsp+20h]
  00000001416BF57D: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001416BF582: 33 D2              xor         edx,edx
  00000001416BF584: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  00000001416BF589: E8 E2 76 E8 FE     call        0000000140546C70
  00000001416BF58E: 4C 8B C0           mov         r8,rax
  00000001416BF591: 48 8B 54 24 68     mov         rdx,qword ptr [rsp+68h]
  00000001416BF596: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001416BF59B: FF 15 DF C4 74 00  call        qword ptr [0000000141E0BA80h]
  00000001416BF5A1: 90                 nop
  00000001416BF5A2: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001416BF5A7: 48 8D 05 4A 1E 90  lea         rax,[0000000142FC13F8h]
                    01
  00000001416BF5AE: 48 89 01           mov         qword ptr [rcx],rax
  00000001416BF5B1: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001416BF5B6: 48 8D 05 03 1E 90  lea         rax,[0000000142FC13C0h]
                    01
  00000001416BF5BD: 48 89 41 10        mov         qword ptr [rcx+10h],rax
  00000001416BF5C1: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  00000001416BF5C6: 48 C7 40 30 00 00  mov         qword ptr [rax+30h],0
                    00 00
  00000001416BF5CE: 48 C7 44 24 30 70  mov         qword ptr [rsp+30h],170h
                    01 00 00
  00000001416BF5D7: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001416BF5DC: E8 DB F4 DA FF     call        000000014146EABC
  00000001416BF5E1: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001416BF5E6: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  00000001416BF5EC: 74 1A              je          00000001416BF608
  00000001416BF5EE: 48 8B 7C 24 38     mov         rdi,qword ptr [rsp+38h]
  00000001416BF5F3: 33 C0              xor         eax,eax
  00000001416BF5F5: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001416BF5FA: F3 AA              rep stos    byte ptr [rdi]
  00000001416BF5FC: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  00000001416BF601: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001416BF606: EB 09              jmp         00000001416BF611
  00000001416BF608: 48 C7 44 24 48 00  mov         qword ptr [rsp+48h],0
                    00 00 00
  00000001416BF611: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001416BF616: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  00000001416BF61B: 48 89 41 28        mov         qword ptr [rcx+28h],rax
  00000001416BF61F: 48 8B 54 24 60     mov         rdx,qword ptr [rsp+60h]
  00000001416BF624: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001416BF629: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  00000001416BF62D: E8 9E 5C 00 00     call        00000001416C52D0
  00000001416BF632: 48 8B 54 24 60     mov         rdx,qword ptr [rsp+60h]
  00000001416BF637: C7 42 38 FF FF FF  mov         dword ptr [rdx+38h],0FFFFFFFFh
                    FF
  00000001416BF63E: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001416BF643: E8 98 00 00 00     call        00000001416BF6E0
  00000001416BF648: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001416BF64D: E8 EE 01 00 00     call        00000001416BF840
  00000001416BF652: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001416BF657: E8 54 08 00 00     call        00000001416BFEB0
  00000001416BF65C: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001416BF661: E8 BA 08 00 00     call        00000001416BFF20
  00000001416BF666: 90                 nop
  00000001416BF667: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  00000001416BF66C: 48 83 C4 50        add         rsp,50h
  00000001416BF670: 5F                 pop         rdi
  00000001416BF671: C3                 ret

  Summary

     1E02000 .text
