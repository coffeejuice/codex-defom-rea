Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001412F0BB0: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  00000001412F0BB5: 48 89 6C 24 10     mov         qword ptr [rsp+10h],rbp
  00000001412F0BBA: 48 89 74 24 18     mov         qword ptr [rsp+18h],rsi
  00000001412F0BBF: 57                 push        rdi
  00000001412F0BC0: 41 54              push        r12
  00000001412F0BC2: 41 55              push        r13
  00000001412F0BC4: 48 83 EC 60        sub         rsp,60h
  00000001412F0BC8: 4C 8B C2           mov         r8,rdx
  00000001412F0BCB: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  00000001412F0BD0: 4C 8B E9           mov         r13,rcx
  00000001412F0BD3: E8 B8 D8 FF FF     call        00000001412EE490
  00000001412F0BD8: 48 8B 6C 24 58     mov         rbp,qword ptr [rsp+58h]
  00000001412F0BDD: 4C 8B 64 24 50     mov         r12,qword ptr [rsp+50h]
  00000001412F0BE2: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000001412F0BE7: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  00000001412F0BEC: 33 F6              xor         esi,esi
  00000001412F0BEE: 48 8B 01           mov         rax,qword ptr [rcx]
  00000001412F0BF1: 48 89 02           mov         qword ptr [rdx],rax
  00000001412F0BF4: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  00000001412F0BF8: 48 89 42 08        mov         qword ptr [rdx+8],rax
  00000001412F0BFC: 48 8B 7C 24 20     mov         rdi,qword ptr [rsp+20h]
  00000001412F0C01: 48 8B 5C 24 28     mov         rbx,qword ptr [rsp+28h]
  00000001412F0C06: 48 85 FF           test        rdi,rdi
  00000001412F0C09: 74 05              je          00000001412F0C10
  00000001412F0C0B: 49 3B FC           cmp         rdi,r12
  00000001412F0C0E: 74 05              je          00000001412F0C15
  00000001412F0C10: E8 DB F9 17 00     call        00000001414705F0
  00000001412F0C15: 48 3B DD           cmp         rbx,rbp
  00000001412F0C18: 74 66              je          00000001412F0C80
  00000001412F0C1A: 48 FF C6           inc         rsi
  00000001412F0C1D: 48 85 FF           test        rdi,rdi
  00000001412F0C20: 75 05              jne         00000001412F0C27
  00000001412F0C22: E8 C9 F9 17 00     call        00000001414705F0
  00000001412F0C27: 80 7B 29 00        cmp         byte ptr [rbx+29h],0
  00000001412F0C2B: 74 07              je          00000001412F0C34
  00000001412F0C2D: E8 BE F9 17 00     call        00000001414705F0
  00000001412F0C32: EB D2              jmp         00000001412F0C06
  00000001412F0C34: 48 8B 43 10        mov         rax,qword ptr [rbx+10h]
  00000001412F0C38: 80 78 29 00        cmp         byte ptr [rax+29h],0
  00000001412F0C3C: 75 20              jne         00000001412F0C5E
  00000001412F0C3E: 48 8B D8           mov         rbx,rax
  00000001412F0C41: 48 8B 00           mov         rax,qword ptr [rax]
  00000001412F0C44: 80 78 29 00        cmp         byte ptr [rax+29h],0
  00000001412F0C48: 75 BC              jne         00000001412F0C06
  00000001412F0C4A: 66 0F 1F 44 00 00  nop         word ptr [rax+rax]
  00000001412F0C50: 48 8B D8           mov         rbx,rax
  00000001412F0C53: 48 8B 00           mov         rax,qword ptr [rax]
  00000001412F0C56: 80 78 29 00        cmp         byte ptr [rax+29h],0
  00000001412F0C5A: 74 F4              je          00000001412F0C50
  00000001412F0C5C: EB A8              jmp         00000001412F0C06
  00000001412F0C5E: 48 8B 43 08        mov         rax,qword ptr [rbx+8]
  00000001412F0C62: 80 78 29 00        cmp         byte ptr [rax+29h],0
  00000001412F0C66: 75 13              jne         00000001412F0C7B
  00000001412F0C68: 48 3B 58 10        cmp         rbx,qword ptr [rax+10h]
  00000001412F0C6C: 75 0D              jne         00000001412F0C7B
  00000001412F0C6E: 48 8B D8           mov         rbx,rax
  00000001412F0C71: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  00000001412F0C75: 80 78 29 00        cmp         byte ptr [rax+29h],0
  00000001412F0C79: 74 ED              je          00000001412F0C68
  00000001412F0C7B: 48 8B D8           mov         rbx,rax
  00000001412F0C7E: EB 86              jmp         00000001412F0C06
  00000001412F0C80: 48 8D 44 24 50     lea         rax,[rsp+50h]
  00000001412F0C85: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  00000001412F0C8A: 4C 8D 4C 24 20     lea         r9,[rsp+20h]
  00000001412F0C8F: 48 8B 08           mov         rcx,qword ptr [rax]
  00000001412F0C92: 4C 8D 44 24 30     lea         r8,[rsp+30h]
  00000001412F0C97: 48 89 0A           mov         qword ptr [rdx],rcx
  00000001412F0C9A: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  00000001412F0C9E: 48 8D 44 24 40     lea         rax,[rsp+40h]
  00000001412F0CA3: 48 89 4A 08        mov         qword ptr [rdx+8],rcx
  00000001412F0CA7: 48 8B 08           mov         rcx,qword ptr [rax]
  00000001412F0CAA: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  00000001412F0CAF: 48 89 0A           mov         qword ptr [rdx],rcx
  00000001412F0CB2: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  00000001412F0CB6: 48 89 4A 08        mov         qword ptr [rdx+8],rcx
  00000001412F0CBA: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  00000001412F0CBF: 49 8B CD           mov         rcx,r13
  00000001412F0CC2: E8 E9 F9 FF FF     call        00000001412F06B0
  00000001412F0CC7: 4C 8D 5C 24 60     lea         r11,[rsp+60h]
  00000001412F0CCC: 48 8B C6           mov         rax,rsi
  00000001412F0CCF: 49 8B 5B 20        mov         rbx,qword ptr [r11+20h]
  00000001412F0CD3: 49 8B 6B 28        mov         rbp,qword ptr [r11+28h]
  00000001412F0CD7: 49 8B 73 30        mov         rsi,qword ptr [r11+30h]
  00000001412F0CDB: 49 8B E3           mov         rsp,r11
  00000001412F0CDE: 41 5D              pop         r13
  00000001412F0CE0: 41 5C              pop         r12
  00000001412F0CE2: 5F                 pop         rdi
  00000001412F0CE3: C3                 ret

  Summary

     1E02000 .text
