Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014172D7E0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014172D7E5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014172D7EA: 48 83 EC 58        sub         rsp,58h
  000000014172D7EE: 48 C7 44 24 40 FE  mov         qword ptr [rsp+40h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000014172D7F7: BA 01 00 00 00     mov         edx,1
  000000014172D7FC: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000014172D801: 48 8B 09           mov         rcx,qword ptr [rcx]
  000000014172D804: FF 15 CE 6A 6D 00  call        qword ptr [0000000141E042D8h]
  000000014172D80A: 0F B6 C0           movzx       eax,al
  000000014172D80D: 85 C0              test        eax,eax
  000000014172D80F: 74 31              je          000000014172D842
  000000014172D811: 41 B8 01 00 00 00  mov         r8d,1
  000000014172D817: BA FF FF FF 7F     mov         edx,7FFFFFFFh
  000000014172D81C: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000014172D821: E8 8A 00 00 00     call        000000014172D8B0
  000000014172D826: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014172D82B: 4C 8B 44 24 68     mov         r8,qword ptr [rsp+68h]
  000000014172D830: 48 8B 54 24 20     mov         rdx,qword ptr [rsp+20h]
  000000014172D835: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000014172D83A: E8 01 C3 6B FF     call        0000000140DE9B40
  000000014172D83F: 90                 nop
  000000014172D840: EB 69              jmp         000000014172D8AB
  000000014172D842: 33 C0              xor         eax,eax
  000000014172D844: 85 C0              test        eax,eax
  000000014172D846: 74 27              je          000000014172D86F
  000000014172D848: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000014172D84D: FF 15 AD 69 6D 00  call        qword ptr [0000000141E04200h]
  000000014172D853: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000014172D858: 4C 8B 44 24 68     mov         r8,qword ptr [rsp+68h]
  000000014172D85D: 48 8B 54 24 28     mov         rdx,qword ptr [rsp+28h]
  000000014172D862: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000014172D867: E8 D4 C2 6B FF     call        0000000140DE9B40
  000000014172D86C: 90                 nop
  000000014172D86D: EB 3C              jmp         000000014172D8AB
  000000014172D86F: 4C 8B 44 24 68     mov         r8,qword ptr [rsp+68h]
  000000014172D874: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  000000014172D879: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000014172D87E: E8 BD C2 6B FF     call        0000000140DE9B40
  000000014172D883: 90                 nop
  000000014172D884: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000014172D889: FF 15 71 69 6D 00  call        qword ptr [0000000141E04200h]
  000000014172D88F: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  000000014172D894: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000014172D899: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014172D89E: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014172D8A3: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  000000014172D8A8: 48 89 01           mov         qword ptr [rcx],rax
  000000014172D8AB: 48 83 C4 58        add         rsp,58h
  000000014172D8AF: C3                 ret

  Summary

     1E02000 .text
