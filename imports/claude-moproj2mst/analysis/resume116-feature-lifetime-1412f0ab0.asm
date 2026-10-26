Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001412F0AB0: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  00000001412F0AB5: 48 89 6C 24 10     mov         qword ptr [rsp+10h],rbp
  00000001412F0ABA: 48 89 74 24 18     mov         qword ptr [rsp+18h],rsi
  00000001412F0ABF: 57                 push        rdi
  00000001412F0AC0: 41 54              push        r12
  00000001412F0AC2: 41 55              push        r13
  00000001412F0AC4: 48 83 EC 60        sub         rsp,60h
  00000001412F0AC8: 48 8B 59 30        mov         rbx,qword ptr [rcx+30h]
  00000001412F0ACC: 4C 8B EA           mov         r13,rdx
  00000001412F0ACF: 48 8B E9           mov         rbp,rcx
  00000001412F0AD2: 48 8B 43 08        mov         rax,qword ptr [rbx+8]
  00000001412F0AD6: 80 78 29 00        cmp         byte ptr [rax+29h],0
  00000001412F0ADA: 75 1C              jne         00000001412F0AF8
  00000001412F0ADC: 4C 8B 02           mov         r8,qword ptr [rdx]
  00000001412F0ADF: 90                 nop
  00000001412F0AE0: 4C 39 40 18        cmp         qword ptr [rax+18h],r8
  00000001412F0AE4: 73 06              jae         00000001412F0AEC
  00000001412F0AE6: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  00000001412F0AEA: EB 06              jmp         00000001412F0AF2
  00000001412F0AEC: 48 8B D8           mov         rbx,rax
  00000001412F0AEF: 48 8B 00           mov         rax,qword ptr [rax]
  00000001412F0AF2: 80 78 29 00        cmp         byte ptr [rax+29h],0
  00000001412F0AF6: 74 E8              je          00000001412F0AE0
  00000001412F0AF8: 48 8B 39           mov         rdi,qword ptr [rcx]
  00000001412F0AFB: 4C 8B 61 30        mov         r12,qword ptr [rcx+30h]
  00000001412F0AFF: 48 89 5C 24 28     mov         qword ptr [rsp+28h],rbx
  00000001412F0B04: 48 89 7C 24 20     mov         qword ptr [rsp+20h],rdi
  00000001412F0B09: 48 85 FF           test        rdi,rdi
  00000001412F0B0C: 74 05              je          00000001412F0B13
  00000001412F0B0E: 48 3B FF           cmp         rdi,rdi
  00000001412F0B11: 74 05              je          00000001412F0B18
  00000001412F0B13: E8 D8 FA 17 00     call        00000001414705F0
  00000001412F0B18: 33 F6              xor         esi,esi
  00000001412F0B1A: 49 3B DC           cmp         rbx,r12
  00000001412F0B1D: 74 0A              je          00000001412F0B29
  00000001412F0B1F: 48 8B 43 18        mov         rax,qword ptr [rbx+18h]
  00000001412F0B23: 49 39 45 00        cmp         qword ptr [r13],rax
  00000001412F0B27: 73 4D              jae         00000001412F0B76
  00000001412F0B29: 0F 28 44 24 20     movaps      xmm0,xmmword ptr [rsp+20h]
  00000001412F0B2E: 49 8B 45 00        mov         rax,qword ptr [r13]
  00000001412F0B32: 4C 8D 4C 24 30     lea         r9,[rsp+30h]
  00000001412F0B37: 4C 8D 44 24 40     lea         r8,[rsp+40h]
  00000001412F0B3C: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  00000001412F0B41: 48 8B CD           mov         rcx,rbp
  00000001412F0B44: 66 0F 7F 44 24 40  movdqa      xmmword ptr [rsp+40h],xmm0
  00000001412F0B4A: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001412F0B4F: 48 89 74 24 38     mov         qword ptr [rsp+38h],rsi
  00000001412F0B54: E8 27 F8 FF FF     call        00000001412F0380
  00000001412F0B59: 48 8B 08           mov         rcx,qword ptr [rax]
  00000001412F0B5C: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  00000001412F0B60: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  00000001412F0B65: 48 89 0A           mov         qword ptr [rdx],rcx
  00000001412F0B68: 48 89 42 08        mov         qword ptr [rdx+8],rax
  00000001412F0B6C: 48 8B 5C 24 28     mov         rbx,qword ptr [rsp+28h]
  00000001412F0B71: 48 8B 7C 24 20     mov         rdi,qword ptr [rsp+20h]
  00000001412F0B76: 48 85 FF           test        rdi,rdi
  00000001412F0B79: 75 07              jne         00000001412F0B82
  00000001412F0B7B: E8 70 FA 17 00     call        00000001414705F0
  00000001412F0B80: EB 03              jmp         00000001412F0B85
  00000001412F0B82: 48 8B 37           mov         rsi,qword ptr [rdi]
  00000001412F0B85: 48 3B 5E 30        cmp         rbx,qword ptr [rsi+30h]
  00000001412F0B89: 75 05              jne         00000001412F0B90
  00000001412F0B8B: E8 60 FA 17 00     call        00000001414705F0
  00000001412F0B90: 4C 8D 5C 24 60     lea         r11,[rsp+60h]
  00000001412F0B95: 48 8D 43 20        lea         rax,[rbx+20h]
  00000001412F0B99: 49 8B 5B 20        mov         rbx,qword ptr [r11+20h]
  00000001412F0B9D: 49 8B 6B 28        mov         rbp,qword ptr [r11+28h]
  00000001412F0BA1: 49 8B 73 30        mov         rsi,qword ptr [r11+30h]
  00000001412F0BA5: 49 8B E3           mov         rsp,r11
  00000001412F0BA8: 41 5D              pop         r13
  00000001412F0BAA: 41 5C              pop         r12
  00000001412F0BAC: 5F                 pop         rdi
  00000001412F0BAD: C3                 ret

  Summary

     1E02000 .text
