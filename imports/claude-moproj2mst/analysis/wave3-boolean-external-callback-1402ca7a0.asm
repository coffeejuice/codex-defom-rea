Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001402CA7A0: 88 54 24 10        mov         byte ptr [rsp+10h],dl
  00000001402CA7A4: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001402CA7A9: 48 83 EC 58        sub         rsp,58h
  00000001402CA7AD: 48 C7 44 24 38 FE  mov         qword ptr [rsp+38h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001402CA7B6: 0F B6 44 24 68     movzx       eax,byte ptr [rsp+68h]
  00000001402CA7BB: 85 C0              test        eax,eax
  00000001402CA7BD: 74 38              je          00000001402CA7F7
  00000001402CA7BF: BA 03 00 00 00     mov         edx,3
  00000001402CA7C4: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001402CA7C9: FF 15 D1 D4 B3 01  call        qword ptr [0000000141E07CA0h]
  00000001402CA7CF: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001402CA7D4: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001402CA7D9: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001402CA7DE: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  00000001402CA7E3: FF 15 AF D4 B3 01  call        qword ptr [0000000141E07C98h]
  00000001402CA7E9: 90                 nop
  00000001402CA7EA: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001402CA7EF: FF 15 9B D4 B3 01  call        qword ptr [0000000141E07C90h]
  00000001402CA7F5: EB 13              jmp         00000001402CA80A
  00000001402CA7F7: FF 15 A3 E1 B3 01  call        qword ptr [0000000141E089A0h]
  00000001402CA7FD: 48 85 C0           test        rax,rax
  00000001402CA800: 74 08              je          00000001402CA80A
  00000001402CA802: FF 15 68 D4 B3 01  call        qword ptr [0000000141E07C70h]
  00000001402CA808: EB ED              jmp         00000001402CA7F7
  00000001402CA80A: FF 15 40 9C B3 01  call        qword ptr [0000000141E04450h]
  00000001402CA810: 4C 8D 5C 24 28     lea         r11,[rsp+28h]
  00000001402CA815: 4C 89 5C 24 30     mov         qword ptr [rsp+30h],r11
  00000001402CA81A: BA 01 00 00 00     mov         edx,1
  00000001402CA81F: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001402CA824: E8 37 A2 D3 FF     call        0000000140004A60
  00000001402CA829: 48 8B C8           mov         rcx,rax
  00000001402CA82C: FF 15 16 9C B3 01  call        qword ptr [0000000141E04448h]
  00000001402CA832: 48 83 C4 58        add         rsp,58h
  00000001402CA836: C3                 ret

  Summary

     1E02000 .text
