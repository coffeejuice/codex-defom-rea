Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000650CF590: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000000650CF595: 41 54              push        r12
  00000000650CF597: 48 83 EC 50        sub         rsp,50h
  00000000650CF59B: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000650CF5A4: 48 89 5C 24 68     mov         qword ptr [rsp+68h],rbx
  00000000650CF5A9: 48 89 74 24 70     mov         qword ptr [rsp+70h],rsi
  00000000650CF5AE: 48 89 7C 24 78     mov         qword ptr [rsp+78h],rdi
  00000000650CF5B3: 48 8B DA           mov         rbx,rdx
  00000000650CF5B6: 48 8B F1           mov         rsi,rcx
  00000000650CF5B9: E8 82 8B 06 00     call        0000000065138140
  00000000650CF5BE: 90                 nop
  00000000650CF5BF: 4C 8D 25 6A 61 5E  lea         r12,[00000000656B5730h]
                    00
  00000000650CF5C6: 4C 89 26           mov         qword ptr [rsi],r12
  00000000650CF5C9: 48 C7 46 10 00 00  mov         qword ptr [rsi+10h],0
                    00 00
  00000000650CF5D1: E8 2A E9 FF FF     call        00000000650CDF00
  00000000650CF5D6: 84 C0              test        al,al
  00000000650CF5D8: 0F 85 97 00 00 00  jne         00000000650CF675
  00000000650CF5DE: 83 3D 8B A0 8F 00  cmp         dword ptr [00000000659C9670h],0
                    00
  00000000650CF5E5: 75 4F              jne         00000000650CF636
  00000000650CF5E7: 48 8D 0D D2 60 5E  lea         rcx,[00000000656B56C0h]
                    00
  00000000650CF5EE: FF 15 A4 73 5C 00  call        qword ptr [0000000065696998h]
  00000000650CF5F4: 48 8B 5E 10        mov         rbx,qword ptr [rsi+10h]
  00000000650CF5F8: 48 85 DB           test        rbx,rbx
  00000000650CF5FB: 0F 84 EF 00 00 00  je          00000000650CF6F0
  00000000650CF601: 48 C7 46 10 00 00  mov         qword ptr [rsi+10h],0
                    00 00
  00000000650CF609: 48 85 DB           test        rbx,rbx
  00000000650CF60C: 0F 84 DE 00 00 00  je          00000000650CF6F0
  00000000650CF612: 48 8D 4B 18        lea         rcx,[rbx+18h]
  00000000650CF616: FF 15 5C 73 5C 00  call        qword ptr [0000000065696978h]
  00000000650CF61C: 84 C0              test        al,al
  00000000650CF61E: 0F 85 CC 00 00 00  jne         00000000650CF6F0
  00000000650CF624: 48 8B 03           mov         rax,qword ptr [rbx]
  00000000650CF627: BA 01 00 00 00     mov         edx,1
  00000000650CF62C: 48 8B CB           mov         rcx,rbx
  00000000650CF62F: FF 10              call        qword ptr [rax]
  00000000650CF631: E9 BA 00 00 00     jmp         00000000650CF6F0
  00000000650CF636: 48 8B 5E 10        mov         rbx,qword ptr [rsi+10h]
  00000000650CF63A: 48 85 DB           test        rbx,rbx
  00000000650CF63D: 0F 84 AD 00 00 00  je          00000000650CF6F0
  00000000650CF643: 48 C7 46 10 00 00  mov         qword ptr [rsi+10h],0
                    00 00
  00000000650CF64B: 48 85 DB           test        rbx,rbx
  00000000650CF64E: 0F 84 9C 00 00 00  je          00000000650CF6F0
  00000000650CF654: 48 8D 4B 18        lea         rcx,[rbx+18h]
  00000000650CF658: FF 15 1A 73 5C 00  call        qword ptr [0000000065696978h]
  00000000650CF65E: 84 C0              test        al,al
  00000000650CF660: 0F 85 8A 00 00 00  jne         00000000650CF6F0
  00000000650CF666: 48 8B 03           mov         rax,qword ptr [rbx]
  00000000650CF669: BA 01 00 00 00     mov         edx,1
  00000000650CF66E: 48 8B CB           mov         rcx,rbx
  00000000650CF671: FF 10              call        qword ptr [rax]
  00000000650CF673: EB 7B              jmp         00000000650CF6F0
  00000000650CF675: 66 83 7B 08 00     cmp         word ptr [rbx+8],0
  00000000650CF67A: 74 66              je          00000000650CF6E2
  00000000650CF67C: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000000650CF681: FF 15 B1 71 5C 00  call        qword ptr [0000000065696838h]
  00000000650CF687: 4C 8B C0           mov         r8,rax
  00000000650CF68A: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  00000000650CF68F: 48 8B CB           mov         rcx,rbx
  00000000650CF692: E8 49 F8 FF FF     call        00000000650CEEE0
  00000000650CF697: 90                 nop
  00000000650CF698: 48 8B D0           mov         rdx,rax
  00000000650CF69B: 48 8B CE           mov         rcx,rsi
  00000000650CF69E: E8 3D F9 FF FF     call        00000000650CEFE0
  00000000650CF6A3: 90                 nop
  00000000650CF6A4: 4C 89 64 24 38     mov         qword ptr [rsp+38h],r12
  00000000650CF6A9: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  00000000650CF6AE: 48 85 C9           test        rcx,rcx
  00000000650CF6B1: 74 23              je          00000000650CF6D6
  00000000650CF6B3: 48 83 C1 18        add         rcx,18h
  00000000650CF6B7: FF 15 BB 72 5C 00  call        qword ptr [0000000065696978h]
  00000000650CF6BD: 84 C0              test        al,al
  00000000650CF6BF: 75 15              jne         00000000650CF6D6
  00000000650CF6C1: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  00000000650CF6C6: 48 85 C9           test        rcx,rcx
  00000000650CF6C9: 74 0B              je          00000000650CF6D6
  00000000650CF6CB: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000650CF6CE: BA 01 00 00 00     mov         edx,1
  00000000650CF6D3: FF 10              call        qword ptr [rax]
  00000000650CF6D5: 90                 nop
  00000000650CF6D6: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  00000000650CF6DB: E8 B0 8A 06 00     call        0000000065138190
  00000000650CF6E0: EB 0E              jmp         00000000650CF6F0
  00000000650CF6E2: 48 8D 53 10        lea         rdx,[rbx+10h]
  00000000650CF6E6: 48 8D 4E 10        lea         rcx,[rsi+10h]
  00000000650CF6EA: E8 B1 E9 FF FF     call        00000000650CE0A0
  00000000650CF6EF: 90                 nop
  00000000650CF6F0: 48 8B C6           mov         rax,rsi
  00000000650CF6F3: 48 8B 5C 24 68     mov         rbx,qword ptr [rsp+68h]
  00000000650CF6F8: 48 8B 74 24 70     mov         rsi,qword ptr [rsp+70h]
  00000000650CF6FD: 48 8B 7C 24 78     mov         rdi,qword ptr [rsp+78h]
  00000000650CF702: 48 83 C4 50        add         rsp,50h
  00000000650CF706: 41 5C              pop         r12
  00000000650CF708: C3                 ret

  Summary

      695000 .text
