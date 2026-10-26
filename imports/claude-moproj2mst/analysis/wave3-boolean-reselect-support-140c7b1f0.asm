Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140C7B1F0: 44 88 44 24 18     mov         byte ptr [rsp+18h],r8b
  0000000140C7B1F5: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140C7B1F9: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C7B1FE: 48 83 EC 28        sub         rsp,28h
  0000000140C7B202: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140C7B207: E8 C4 FA FF FF     call        0000000140C7ACD0
  0000000140C7B20C: 48 85 C0           test        rax,rax
  0000000140C7B20F: 74 16              je          0000000140C7B227
  0000000140C7B211: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140C7B216: E8 B5 FA FF FF     call        0000000140C7ACD0
  0000000140C7B21B: 8B 54 24 38        mov         edx,dword ptr [rsp+38h]
  0000000140C7B21F: 48 8B C8           mov         rcx,rax
  0000000140C7B222: E8 59 8A BA FF     call        0000000140823C80
  0000000140C7B227: 0F B6 44 24 40     movzx       eax,byte ptr [rsp+40h]
  0000000140C7B22C: 85 C0              test        eax,eax
  0000000140C7B22E: 74 4A              je          0000000140C7B27A
  0000000140C7B230: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140C7B235: 48 83 78 10 00     cmp         qword ptr [rax+10h],0
  0000000140C7B23A: 74 3E              je          0000000140C7B27A
  0000000140C7B23C: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140C7B241: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  0000000140C7B245: 83 B8 04 02 00 00  cmp         dword ptr [rax+0000000000000204h],1
                    01
  0000000140C7B24C: 75 0E              jne         0000000140C7B25C
  0000000140C7B24E: 33 D2              xor         edx,edx
  0000000140C7B250: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140C7B255: E8 C6 57 00 00     call        0000000140C80A20
  0000000140C7B25A: EB 1E              jmp         0000000140C7B27A
  0000000140C7B25C: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140C7B261: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  0000000140C7B265: 83 B8 04 02 00 00  cmp         dword ptr [rax+0000000000000204h],2
                    02
  0000000140C7B26C: 75 0C              jne         0000000140C7B27A
  0000000140C7B26E: 33 D2              xor         edx,edx
  0000000140C7B270: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140C7B275: E8 86 58 00 00     call        0000000140C80B00
  0000000140C7B27A: 48 83 C4 28        add         rsp,28h
  0000000140C7B27E: C3                 ret

  Summary

     1E02000 .text
