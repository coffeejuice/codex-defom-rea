Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670C5110: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000000670C5115: 57                 push        rdi
  00000000670C5116: 48 83 EC 50        sub         rsp,50h
  00000000670C511A: 48 C7 44 24 40 FE  mov         qword ptr [rsp+40h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670C5123: 48 89 5C 24 70     mov         qword ptr [rsp+70h],rbx
  00000000670C5128: 48 8B DA           mov         rbx,rdx
  00000000670C512B: C7 44 24 78 00 00  mov         dword ptr [rsp+78h],0
                    00 00
  00000000670C5133: 48 8B 79 08        mov         rdi,qword ptr [rcx+8]
  00000000670C5137: 48 83 7F 28 00     cmp         qword ptr [rdi+28h],0
  00000000670C513C: 75 2C              jne         00000000670C516A
  00000000670C513E: 48 83 3F 00        cmp         qword ptr [rdi],0
  00000000670C5142: 75 26              jne         00000000670C516A
  00000000670C5144: 48 8D 0D 7D 72 16  lea         rcx,[000000006722C3C8h]
                    00
  00000000670C514B: E8 90 7D F5 FF     call        000000006701CEE0
  00000000670C5150: 4C 8D 1D D9 B9 22  lea         r11,[00000000672F0B30h]
                    00
  00000000670C5157: 4C 89 1B           mov         qword ptr [rbx],r11
  00000000670C515A: F0 83 05 CE B9 22  lock add    dword ptr [00000000672F0B30h],1
                    00 01
  00000000670C5162: 48 8B C3           mov         rax,rbx
  00000000670C5165: E9 95 00 00 00     jmp         00000000670C51FF
  00000000670C516A: C7 44 24 20 02 00  mov         dword ptr [rsp+20h],2
                    00 00
  00000000670C5172: 45 8B C8           mov         r9d,r8d
  00000000670C5175: 4C 8D 44 24 60     lea         r8,[rsp+60h]
  00000000670C517A: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  00000000670C517F: 48 8B CF           mov         rcx,rdi
  00000000670C5182: E8 59 DF FF FF     call        00000000670C30E0
  00000000670C5187: 84 C0              test        al,al
  00000000670C5189: 75 17              jne         00000000670C51A2
  00000000670C518B: 4C 8D 1D 9E B9 22  lea         r11,[00000000672F0B30h]
                    00
  00000000670C5192: 4C 89 1B           mov         qword ptr [rbx],r11
  00000000670C5195: F0 83 05 93 B9 22  lock add    dword ptr [00000000672F0B30h],1
                    00 01
  00000000670C519D: 48 8B C3           mov         rax,rbx
  00000000670C51A0: EB 5D              jmp         00000000670C51FF
  00000000670C51A2: 44 8B 44 24 60     mov         r8d,dword ptr [rsp+60h]
  00000000670C51A7: 48 8B 54 24 38     mov         rdx,qword ptr [rsp+38h]
  00000000670C51AC: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000000670C51B1: E8 6A 61 FA FF     call        000000006706B320
  00000000670C51B6: 90                 nop
  00000000670C51B7: 8B 97 8C 00 00 00  mov         edx,dword ptr [rdi+000000000000008Ch]
  00000000670C51BD: 85 D2              test        edx,edx
  00000000670C51BF: 74 08              je          00000000670C51C9
  00000000670C51C1: 48 8B CF           mov         rcx,rdi
  00000000670C51C4: E8 B7 E1 FF FF     call        00000000670C3380
  00000000670C51C9: C7 87 8C 00 00 00  mov         dword ptr [rdi+000000000000008Ch],0
                    00 00 00 00
  00000000670C51D3: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000000670C51D8: 48 89 03           mov         qword ptr [rbx],rax
  00000000670C51DB: F0 83 00 01        lock add    dword ptr [rax],1
  00000000670C51DF: C7 44 24 78 01 00  mov         dword ptr [rsp+78h],1
                    00 00
  00000000670C51E7: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000000670C51EC: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670C51F0: 75 0A              jne         00000000670C51FC
  00000000670C51F2: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000000670C51F7: E8 D4 AE FA FF     call        00000000670700D0
  00000000670C51FC: 48 8B C3           mov         rax,rbx
  00000000670C51FF: 48 8B 5C 24 70     mov         rbx,qword ptr [rsp+70h]
  00000000670C5204: 48 83 C4 50        add         rsp,50h
  00000000670C5208: 5F                 pop         rdi
  00000000670C5209: C3                 ret
  00000000670C520A: CC                 int         3
  00000000670C520B: CC                 int         3
  00000000670C520C: CC                 int         3
  00000000670C520D: CC                 int         3
  00000000670C520E: CC                 int         3
  00000000670C520F: CC                 int         3
  00000000670C5210: 48

  Summary

      1A7000 .text
