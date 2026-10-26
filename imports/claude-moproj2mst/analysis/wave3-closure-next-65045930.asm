Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065045930: 48 85 C9           test        rcx,rcx
  0000000065045933: 0F 84 9A 00 00 00  je          00000000650459D3
  0000000065045939: 57                 push        rdi
  000000006504593A: 48 83 EC 30        sub         rsp,30h
  000000006504593E: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000065045947: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  000000006504594C: 0F 1F 40 00        nop         dword ptr [rax]
  0000000065045950: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  0000000065045954: F6 80 9C 00 00 00  test        byte ptr [rax+000000000000009Ch],4
                    04
  000000006504595B: 74 6C              je          00000000650459C9
  000000006504595D: 83 A0 9C 00 00 00  and         dword ptr [rax+000000000000009Ch],0FFFFFFFBh
                    FB
  0000000065045964: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  0000000065045968: F6 80 9C 00 00 00  test        byte ptr [rax+000000000000009Ch],1
                    01
  000000006504596F: 75 10              jne         0000000065045981
  0000000065045971: FF 15 F1 0E 65 00  call        qword ptr [0000000065696868h]
  0000000065045977: 48 8B C8           mov         rcx,rax
  000000006504597A: 48 85 C0           test        rax,rax
  000000006504597D: 75 D1              jne         0000000065045950
  000000006504597F: EB 48              jmp         00000000650459C9
  0000000065045981: FF 15 E1 0E 65 00  call        qword ptr [0000000065696868h]
  0000000065045987: 48 8B F8           mov         rdi,rax
  000000006504598A: B9 18 00 00 00     mov         ecx,18h
  000000006504598F: E8 DE B7 59 00     call        00000000655E1172
  0000000065045994: 48 8B D8           mov         rbx,rax
  0000000065045997: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  000000006504599C: 48 85 C0           test        rax,rax
  000000006504599F: 74 1A              je          00000000650459BB
  00000000650459A1: BA 4C 00 00 00     mov         edx,4Ch
  00000000650459A6: 48 8B C8           mov         rcx,rax
  00000000650459A9: FF 15 69 0E 65 00  call        qword ptr [0000000065696818h]
  00000000650459AF: 4C 8D 1D 9A 89 65  lea         r11,[000000006569E350h]
                    00
  00000000650459B6: 4C 89 1B           mov         qword ptr [rbx],r11
  00000000650459B9: EB 02              jmp         00000000650459BD
  00000000650459BB: 33 DB              xor         ebx,ebx
  00000000650459BD: 48 8B D3           mov         rdx,rbx
  00000000650459C0: 48 8B CF           mov         rcx,rdi
  00000000650459C3: FF 15 27 12 65 00  call        qword ptr [0000000065696BF0h]
  00000000650459C9: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  00000000650459CE: 48 83 C4 30        add         rsp,30h
  00000000650459D2: 5F                 pop         rdi
  00000000650459D3: F3 C3              rep ret

  Summary

      695000 .text
