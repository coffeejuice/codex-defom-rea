Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670EF690: 48 89 5C 24 10     mov         qword ptr [rsp+10h],rbx
  00000000670EF695: 48 89 74 24 18     mov         qword ptr [rsp+18h],rsi
  00000000670EF69A: 57                 push        rdi
  00000000670EF69B: 48 83 EC 20        sub         rsp,20h
  00000000670EF69F: 33 F6              xor         esi,esi
  00000000670EF6A1: 48 8B DA           mov         rbx,rdx
  00000000670EF6A4: 48 8B F9           mov         rdi,rcx
  00000000670EF6A7: 89 74 24 30        mov         dword ptr [rsp+30h],esi
  00000000670EF6AB: E8 B0 FB FF FF     call        00000000670EF260
  00000000670EF6B0: 44 0F B7 5F 14     movzx       r11d,word ptr [rdi+14h]
  00000000670EF6B5: 66 41 83 FB FF     cmp         r11w,0FFFFh
  00000000670EF6BA: 75 25              jne         00000000670EF6E1
  00000000670EF6BC: 48 8D 05 6D 14 20  lea         rax,[00000000672F0B30h]
                    00
  00000000670EF6C3: 48 89 03           mov         qword ptr [rbx],rax
  00000000670EF6C6: F0 83 05 62 14 20  lock add    dword ptr [00000000672F0B30h],1
                    00 01
  00000000670EF6CE: 48 8B C3           mov         rax,rbx
  00000000670EF6D1: 48 8B 5C 24 38     mov         rbx,qword ptr [rsp+38h]
  00000000670EF6D6: 48 8B 74 24 40     mov         rsi,qword ptr [rsp+40h]
  00000000670EF6DB: 48 83 C4 20        add         rsp,20h
  00000000670EF6DF: 5F                 pop         rdi
  00000000670EF6E0: C3                 ret
  00000000670EF6E1: 66 3B 77 10        cmp         si,word ptr [rdi+10h]
  00000000670EF6E5: 48 8D 4F 10        lea         rcx,[rdi+10h]
  00000000670EF6E9: 48 8D 44 24 30     lea         rax,[rsp+30h]
  00000000670EF6EE: 48 0F 4C C1        cmovl       rax,rcx
  00000000670EF6F2: 66 89 74 24 30     mov         word ptr [rsp+30h],si
  00000000670EF6F7: 48 8B D3           mov         rdx,rbx
  00000000670EF6FA: 0F BF 08           movsx       ecx,word ptr [rax]
  00000000670EF6FD: 0F BF 47 12        movsx       eax,word ptr [rdi+12h]
  00000000670EF701: 41 83 C9 FF        or          r9d,0FFFFFFFFh
  00000000670EF705: 03 C8              add         ecx,eax
  00000000670EF707: 41 0F BF C3        movsx       eax,r11w
  00000000670EF70B: 44 8D 44 01 01     lea         r8d,[rcx+rax+1]
  00000000670EF710: 48 8B CF           mov         rcx,rdi
  00000000670EF713: E8 88 48 F8 FF     call        0000000067073FA0
  00000000670EF718: 48 8B 74 24 40     mov         rsi,qword ptr [rsp+40h]
  00000000670EF71D: 48 8B C3           mov         rax,rbx
  00000000670EF720: 48 8B 5C 24 38     mov         rbx,qword ptr [rsp+38h]
  00000000670EF725: 48 83 C4 20        add         rsp,20h
  00000000670EF729: 5F                 pop         rdi
  00000000670EF72A: C3                 ret

  Summary

      1A7000 .text
