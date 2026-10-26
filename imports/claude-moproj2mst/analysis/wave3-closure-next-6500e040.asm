Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006500E040: 48 8B C4           mov         rax,rsp
  000000006500E043: 57                 push        rdi
  000000006500E044: 48 81 EC A0 00 00  sub         rsp,0A0h
                    00
  000000006500E04B: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006500E054: 48 89 58 10        mov         qword ptr [rax+10h],rbx
  000000006500E058: 48 89 70 18        mov         qword ptr [rax+18h],rsi
  000000006500E05C: 48 8B F1           mov         rsi,rcx
  000000006500E05F: B9 08 00 00 00     mov         ecx,8
  000000006500E064: FF 15 96 89 68 00  call        qword ptr [0000000065696A00h]
  000000006500E06A: 48 8B D8           mov         rbx,rax
  000000006500E06D: 48 89 84 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],rax
                    00 00
  000000006500E075: 48 8B 06           mov         rax,qword ptr [rsi]
  000000006500E078: 83 78 70 00        cmp         dword ptr [rax+70h],0
  000000006500E07C: 74 3F              je          000000006500E0BD
  000000006500E07E: 83 4B 78 01        or          dword ptr [rbx+78h],1
  000000006500E082: 48 8B 06           mov         rax,qword ptr [rsi]
  000000006500E085: 48 8B 78 08        mov         rdi,qword ptr [rax+8]
  000000006500E089: 48 89 5C 24 40     mov         qword ptr [rsp+40h],rbx
  000000006500E08E: 66 90              nop
  000000006500E090: 48 3B 3E           cmp         rdi,qword ptr [rsi]
  000000006500E093: 74 24              je          000000006500E0B9
  000000006500E095: 4C 8D 4F F8        lea         r9,[rdi-8]
  000000006500E099: 49 8D 41 04        lea         rax,[r9+4]
  000000006500E09D: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000006500E0A2: 4C 8D 44 24 40     lea         r8,[rsp+40h]
  000000006500E0A7: 48 8B D3           mov         rdx,rbx
  000000006500E0AA: 48 8B CE           mov         rcx,rsi
  000000006500E0AD: E8 0E DB FF FF     call        000000006500BBC0
  000000006500E0B2: 90                 nop
  000000006500E0B3: 48 8B 7F 08        mov         rdi,qword ptr [rdi+8]
  000000006500E0B7: EB D7              jmp         000000006500E090
  000000006500E0B9: 83 63 78 FE        and         dword ptr [rbx+78h],0FFFFFFFEh
  000000006500E0BD: 48 8B 0E           mov         rcx,qword ptr [rsi]
  000000006500E0C0: 48 83 C1 68        add         rcx,68h
  000000006500E0C4: FF 15 AE 88 68 00  call        qword ptr [0000000065696978h]
  000000006500E0CA: 84 C0              test        al,al
  000000006500E0CC: 75 26              jne         000000006500E0F4
  000000006500E0CE: 48 8B 0E           mov         rcx,qword ptr [rsi]
  000000006500E0D1: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  000000006500E0D5: 48 3B C1           cmp         rax,rcx
  000000006500E0D8: 74 0F              je          000000006500E0E9
  000000006500E0DA: 66 0F 1F 44 00 00  nop         word ptr [rax+rax]
  000000006500E0E0: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  000000006500E0E4: 48 3B C1           cmp         rax,rcx
  000000006500E0E7: 75 F7              jne         000000006500E0E0
  000000006500E0E9: BA 08 00 00 00     mov         edx,8
  000000006500E0EE: FF 15 64 88 68 00  call        qword ptr [0000000065696958h]
  000000006500E0F4: 48 89 1E           mov         qword ptr [rsi],rbx
  000000006500E0F7: 4C 8D 9C 24 A0 00  lea         r11,[rsp+00000000000000A0h]
                    00 00
  000000006500E0FF: 49 8B 5B 18        mov         rbx,qword ptr [r11+18h]
  000000006500E103: 49 8B 73 20        mov         rsi,qword ptr [r11+20h]
  000000006500E107: 49 8B E3           mov         rsp,r11
  000000006500E10A: 5F                 pop         rdi
  000000006500E10B: C3                 ret

  Summary

      695000 .text
