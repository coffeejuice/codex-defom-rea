Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065500300: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000065500305: 57                 push        rdi
  0000000065500306: 48 83 EC 30        sub         rsp,30h
  000000006550030A: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000065500313: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  0000000065500318: 48 89 74 24 58     mov         qword ptr [rsp+58h],rsi
  000000006550031D: 48 8B F2           mov         rsi,rdx
  0000000065500320: 48 8B F9           mov         rdi,rcx
  0000000065500323: B9 30 04 00 00     mov         ecx,430h
  0000000065500328: E8 45 0E 0E 00     call        00000000655E1172
  000000006550032D: 48 8B D8           mov         rbx,rax
  0000000065500330: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000065500335: 48 85 C0           test        rax,rax
  0000000065500338: 74 25              je          000000006550035F
  000000006550033A: 48 8B C8           mov         rcx,rax
  000000006550033D: E8 FE 98 FC FF     call        00000000654C9C40
  0000000065500342: 4C 8D 1D EF 49 23  lea         r11,[0000000065734D38h]
                    00
  0000000065500349: 4C 89 1B           mov         qword ptr [rbx],r11
  000000006550034C: C7 83 28 04 00 00  mov         dword ptr [rbx+0000000000000428h],0
                    00 00 00 00
  0000000065500356: C6 83 2C 04 00 00  mov         byte ptr [rbx+000000000000042Ch],0
                    00
  000000006550035D: EB 02              jmp         0000000065500361
  000000006550035F: 33 DB              xor         ebx,ebx
  0000000065500361: 4C 8B C6           mov         r8,rsi
  0000000065500364: 48 8B D3           mov         rdx,rbx
  0000000065500367: 48 8B CF           mov         rcx,rdi
  000000006550036A: E8 A1 CE FC FF     call        00000000654CD210
  000000006550036F: 90                 nop
  0000000065500370: 48 8D 05 C1 4A 23  lea         rax,[0000000065734E38h]
                    00
  0000000065500377: 48 89 07           mov         qword ptr [rdi],rax
  000000006550037A: 48 8D 05 7F 4A 23  lea         rax,[0000000065734E00h]
                    00
  0000000065500381: 48 89 47 10        mov         qword ptr [rdi+10h],rax
  0000000065500385: 48 8B 4F 08        mov         rcx,qword ptr [rdi+8]
  0000000065500389: E8 92 F7 FF FF     call        00000000654FFB20
  000000006550038E: 90                 nop
  000000006550038F: 48 8B C7           mov         rax,rdi
  0000000065500392: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  0000000065500397: 48 8B 74 24 58     mov         rsi,qword ptr [rsp+58h]
  000000006550039C: 48 83 C4 30        add         rsp,30h
  00000000655003A0: 5F                 pop         rdi
  00000000655003A1: C3                 ret

  Summary

      695000 .text
