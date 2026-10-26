Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001408F4760: 44 88 4C 24 20     mov         byte ptr [rsp+20h],r9b
  00000001408F4765: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  00000001408F476A: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001408F476F: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001408F4774: 48 83 EC 68        sub         rsp,68h
  00000001408F4778: 48 8B 05 B1 98 26  mov         rax,qword ptr [0000000143B5E030h]
                    03
  00000001408F477F: 48 33 C4           xor         rax,rsp
  00000001408F4782: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  00000001408F4787: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  00000001408F478C: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  00000001408F4791: E8 EA EB FF FF     call        00000001408F3380
  00000001408F4796: 88 44 24 50        mov         byte ptr [rsp+50h],al
  00000001408F479A: 0F B6 44 24 50     movzx       eax,byte ptr [rsp+50h]
  00000001408F479F: 85 C0              test        eax,eax
  00000001408F47A1: 75 04              jne         00000001408F47A7
  00000001408F47A3: 32 C0              xor         al,al
  00000001408F47A5: EB 7F              jmp         00000001408F4826
  00000001408F47A7: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000001408F47AC: E8 5F ED FF FF     call        00000001408F3510
  00000001408F47B1: 89 44 24 30        mov         dword ptr [rsp+30h],eax
  00000001408F47B5: 83 7C 24 30 FF     cmp         dword ptr [rsp+30h],0FFFFFFFFh
  00000001408F47BA: 75 04              jne         00000001408F47C0
  00000001408F47BC: 32 C0              xor         al,al
  00000001408F47BE: EB 66              jmp         00000001408F4826
  00000001408F47C0: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001408F47C8: E8 63 ED 7C FF     call        00000001400C3530
  00000001408F47CD: 0F B6 C0           movzx       eax,al
  00000001408F47D0: 85 C0              test        eax,eax
  00000001408F47D2: 74 2E              je          00000001408F4802
  00000001408F47D4: 0F B6 84 24 88 00  movzx       eax,byte ptr [rsp+0000000000000088h]
                    00 00
  00000001408F47DC: 88 44 24 20        mov         byte ptr [rsp+20h],al
  00000001408F47E0: 44 8B 4C 24 30     mov         r9d,dword ptr [rsp+30h]
  00000001408F47E5: 4C 8B 84 24 80 00  mov         r8,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001408F47ED: 48 8B 54 24 78     mov         rdx,qword ptr [rsp+78h]
  00000001408F47F2: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  00000001408F47F7: E8 04 01 00 00     call        00000001408F4900
  00000001408F47FC: 88 44 24 50        mov         byte ptr [rsp+50h],al
  00000001408F4800: EB 20              jmp         00000001408F4822
  00000001408F4802: 44 8B 4C 24 30     mov         r9d,dword ptr [rsp+30h]
  00000001408F4807: 4C 8B 84 24 80 00  mov         r8,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001408F480F: 48 8B 54 24 78     mov         rdx,qword ptr [rsp+78h]
  00000001408F4814: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  00000001408F4819: E8 12 04 00 00     call        00000001408F4C30
  00000001408F481E: 88 44 24 50        mov         byte ptr [rsp+50h],al
  00000001408F4822: 8A 44 24 50        mov         al,byte ptr [rsp+50h]
  00000001408F4826: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  00000001408F482B: 48 33 CC           xor         rcx,rsp
  00000001408F482E: E8 DD 96 B7 00     call        000000014146DF10
  00000001408F4833: 48 83 C4 68        add         rsp,68h
  00000001408F4837: C3                 ret

  Summary

     1E02000 .text
