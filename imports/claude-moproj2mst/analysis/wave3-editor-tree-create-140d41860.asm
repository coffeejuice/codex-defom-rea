Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D41860: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140D41865: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D4186A: 56                 push        rsi
  0000000140D4186B: 57                 push        rdi
  0000000140D4186C: 48 81 EC A8 00 00  sub         rsp,0A8h
                    00
  0000000140D41873: 48 C7 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  0000000140D4187F: 48 C7 44 24 40 00  mov         qword ptr [rsp+40h],0
                    00 00 00
  0000000140D41888: 48 8B 8C 24 C8 00  mov         rcx,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140D41890: FF 15 4A 9C 0C 01  call        qword ptr [0000000141E0B4E0h]
  0000000140D41896: 89 44 24 3C        mov         dword ptr [rsp+3Ch],eax
  0000000140D4189A: C7 44 24 4C 00 00  mov         dword ptr [rsp+4Ch],0
                    00 00
  0000000140D418A2: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140D418A7: FF 15 23 40 0C 01  call        qword ptr [0000000141E058D0h]
  0000000140D418AD: 90                 nop
  0000000140D418AE: C6 44 24 38 00     mov         byte ptr [rsp+38h],0
  0000000140D418B3: C6 44 24 48 00     mov         byte ptr [rsp+48h],0
  0000000140D418B8: C7 44 24 50 00 00  mov         dword ptr [rsp+50h],0
                    00 00
  0000000140D418C0: EB 0B              jmp         0000000140D418CD
  0000000140D418C2: 8B 44 24 50        mov         eax,dword ptr [rsp+50h]
  0000000140D418C6: 83 C0 01           add         eax,1
  0000000140D418C9: 89 44 24 50        mov         dword ptr [rsp+50h],eax
  0000000140D418CD: 8B 44 24 3C        mov         eax,dword ptr [rsp+3Ch]
  0000000140D418D1: 39 44 24 50        cmp         dword ptr [rsp+50h],eax
  0000000140D418D5: 0F 8D FA 00 00 00  jge         0000000140D419D5
  0000000140D418DB: 45 33 C0           xor         r8d,r8d
  0000000140D418DE: 8B 54 24 50        mov         edx,dword ptr [rsp+50h]
  0000000140D418E2: 48 8B 8C 24 C8 00  mov         rcx,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140D418EA: FF 15 F8 9B 0C 01  call        qword ptr [0000000141E0B4E8h]
  0000000140D418F0: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140D418F5: 48 83 7C 24 40 00  cmp         qword ptr [rsp+40h],0
  0000000140D418FB: 0F 84 CF 00 00 00  je          0000000140D419D0
  0000000140D41901: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  0000000140D41906: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140D4190E: E8 0D FB FF FF     call        0000000140D41420
  0000000140D41913: 88 44 24 48        mov         byte ptr [rsp+48h],al
  0000000140D41917: 0F B6 44 24 48     movzx       eax,byte ptr [rsp+48h]
  0000000140D4191C: 85 C0              test        eax,eax
  0000000140D4191E: 74 05              je          0000000140D41925
  0000000140D41920: E9 B0 00 00 00     jmp         0000000140D419D5
  0000000140D41925: 48 8B 8C 24 C8 00  mov         rcx,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140D4192D: FF 15 AD 9B 0C 01  call        qword ptr [0000000141E0B4E0h]
  0000000140D41933: 89 44 24 4C        mov         dword ptr [rsp+4Ch],eax
  0000000140D41937: 4C 8B 44 24 40     mov         r8,qword ptr [rsp+40h]
  0000000140D4193C: 48 8D 54 24 58     lea         rdx,[rsp+58h]
  0000000140D41941: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140D41949: FF 15 61 9D 0C 01  call        qword ptr [0000000141E0B6B0h]
  0000000140D4194F: 48 89 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],rax
                    00 00
  0000000140D41957: 48 8D 7C 24 78     lea         rdi,[rsp+78h]
  0000000140D4195C: 48 8B B4 24 98 00  mov         rsi,qword ptr [rsp+0000000000000098h]
                    00 00
  0000000140D41964: B9 18 00 00 00     mov         ecx,18h
  0000000140D41969: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140D4196B: 48 8D 7C 24 20     lea         rdi,[rsp+20h]
  0000000140D41970: 48 8D 74 24 78     lea         rsi,[rsp+78h]
  0000000140D41975: B9 18 00 00 00     mov         ecx,18h
  0000000140D4197A: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140D4197C: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  0000000140D41981: FF 15 59 25 0C 01  call        qword ptr [0000000141E03EE0h]
  0000000140D41987: 48 8B 84 24 C0 00  mov         rax,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140D4198F: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140D41992: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  0000000140D41997: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140D4199F: FF 50 78           call        qword ptr [rax+78h]
  0000000140D419A2: 88 44 24 38        mov         byte ptr [rsp+38h],al
  0000000140D419A6: 0F B6 44 24 38     movzx       eax,byte ptr [rsp+38h]
  0000000140D419AB: 85 C0              test        eax,eax
  0000000140D419AD: 74 21              je          0000000140D419D0
  0000000140D419AF: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  0000000140D419B4: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140D419BC: E8 9F FE FF FF     call        0000000140D41860
  0000000140D419C1: 88 44 24 48        mov         byte ptr [rsp+48h],al
  0000000140D419C5: 0F B6 44 24 48     movzx       eax,byte ptr [rsp+48h]
  0000000140D419CA: 85 C0              test        eax,eax
  0000000140D419CC: 74 02              je          0000000140D419D0
  0000000140D419CE: EB 05              jmp         0000000140D419D5
  0000000140D419D0: E9 ED FE FF FF     jmp         0000000140D418C2
  0000000140D419D5: 0F B6 44 24 48     movzx       eax,byte ptr [rsp+48h]
  0000000140D419DA: 88 44 24 70        mov         byte ptr [rsp+70h],al
  0000000140D419DE: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140D419E3: FF 15 F7 24 0C 01  call        qword ptr [0000000141E03EE0h]
  0000000140D419E9: 8A 44 24 70        mov         al,byte ptr [rsp+70h]
  0000000140D419ED: 48 81 C4 A8 00 00  add         rsp,0A8h
                    00
  0000000140D419F4: 5F                 pop         rdi
  0000000140D419F5: 5E                 pop         rsi
  0000000140D419F6: C3                 ret

  Summary

     1E02000 .text
