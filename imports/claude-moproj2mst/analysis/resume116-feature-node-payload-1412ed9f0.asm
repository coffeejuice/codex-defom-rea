Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001412ED9F0: 40 57              push        rdi
  00000001412ED9F2: 48 83 EC 30        sub         rsp,30h
  00000001412ED9F6: 48 C7 44 24 28 FE  mov         qword ptr [rsp+28h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001412ED9FF: 48 89 5C 24 40     mov         qword ptr [rsp+40h],rbx
  00000001412EDA04: 48 89 74 24 48     mov         qword ptr [rsp+48h],rsi
  00000001412EDA09: 49 8B D9           mov         rbx,r9
  00000001412EDA0C: 49 8B F8           mov         rdi,r8
  00000001412EDA0F: 48 8B F2           mov         rsi,rdx
  00000001412EDA12: B9 30 00 00 00     mov         ecx,30h
  00000001412EDA17: E8 A0 10 18 00     call        000000014146EABC
  00000001412EDA1C: 4C 8B D8           mov         r11,rax
  00000001412EDA1F: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001412EDA24: 48 85 C0           test        rax,rax
  00000001412EDA27: 74 2D              je          00000001412EDA56
  00000001412EDA29: 48 89 30           mov         qword ptr [rax],rsi
  00000001412EDA2C: 48 89 78 08        mov         qword ptr [rax+8],rdi
  00000001412EDA30: 48 89 58 10        mov         qword ptr [rax+10h],rbx
  00000001412EDA34: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  00000001412EDA39: 48 8B 08           mov         rcx,qword ptr [rax]
  00000001412EDA3C: 49 89 4B 18        mov         qword ptr [r11+18h],rcx
  00000001412EDA40: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  00000001412EDA44: 49 89 43 20        mov         qword ptr [r11+20h],rax
  00000001412EDA48: 0F B6 44 24 68     movzx       eax,byte ptr [rsp+68h]
  00000001412EDA4D: 41 88 43 28        mov         byte ptr [r11+28h],al
  00000001412EDA51: 41 C6 43 29 00     mov         byte ptr [r11+29h],0
  00000001412EDA56: 49 8B C3           mov         rax,r11
  00000001412EDA59: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  00000001412EDA5E: 48 8B 74 24 48     mov         rsi,qword ptr [rsp+48h]
  00000001412EDA63: 48 83 C4 30        add         rsp,30h
  00000001412EDA67: 5F                 pop         rdi
  00000001412EDA68: C3                 ret

  Summary

     1E02000 .text
