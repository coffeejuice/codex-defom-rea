Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtXml4_64.dll

File Type: DLL

  0000000061017070: 48 89 5C 24 10     mov         qword ptr [rsp+10h],rbx
  0000000061017075: 48 89 74 24 18     mov         qword ptr [rsp+18h],rsi
  000000006101707A: 57                 push        rdi
  000000006101707B: 48 83 EC 20        sub         rsp,20h
  000000006101707F: 48 8B D9           mov         rbx,rcx
  0000000061017082: 48 81 C1 20 01 00  add         rcx,120h
                    00
  0000000061017089: FF 15 39 C8 02 00  call        qword ptr [00000000610438C8h]
  000000006101708F: BE FF FF 00 00     mov         esi,0FFFFh
  0000000061017094: 0F B7 10           movzx       edx,word ptr [rax]
  0000000061017097: 66 83 CA 01        or          dx,1
  000000006101709B: 66 3B D6           cmp         dx,si
  000000006101709E: 74 53              je          00000000610170F3
  00000000610170A0: 0F B7 83 20 01 00  movzx       eax,word ptr [rbx+0000000000000120h]
                    00
  00000000610170A7: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000000610170AC: 66 89 44 24 30     mov         word ptr [rsp+30h],ax
  00000000610170B1: FF 15 11 C8 02 00  call        qword ptr [00000000610438C8h]
  00000000610170B7: 0F B7 08           movzx       ecx,word ptr [rax]
  00000000610170BA: 66 83 F9 20        cmp         cx,20h
  00000000610170BE: 74 12              je          00000000610170D2
  00000000610170C0: 66 83 F9 09        cmp         cx,9
  00000000610170C4: 74 0C              je          00000000610170D2
  00000000610170C6: 66 83 F9 0A        cmp         cx,0Ah
  00000000610170CA: 74 06              je          00000000610170D2
  00000000610170CC: 66 83 F9 0D        cmp         cx,0Dh
  00000000610170D0: 75 4F              jne         0000000061017121
  00000000610170D2: 48 8B CB           mov         rcx,rbx
  00000000610170D5: E8 66 FD FF FF     call        0000000061016E40
  00000000610170DA: 48 8D 8B 20 01 00  lea         rcx,[rbx+0000000000000120h]
                    00
  00000000610170E1: FF 15 E1 C7 02 00  call        qword ptr [00000000610438C8h]
  00000000610170E7: 0F B7 08           movzx       ecx,word ptr [rax]
  00000000610170EA: 66 83 C9 01        or          cx,1
  00000000610170EE: 66 3B CE           cmp         cx,si
  00000000610170F1: 75 AD              jne         00000000610170A0
  00000000610170F3: 48 83 BB D8 00 00  cmp         qword ptr [rbx+00000000000000D8h],0
                    00 00
  00000000610170FB: 74 24              je          0000000061017121
  00000000610170FD: 48 8D 15 6C FF FF  lea         rdx,[0000000061017070h]
                    FF
  0000000061017104: 45 33 C0           xor         r8d,r8d
  0000000061017107: 48 8B CB           mov         rcx,rbx
  000000006101710A: E8 91 F2 FF FF     call        00000000610163A0
  000000006101710F: 32 C0              xor         al,al
  0000000061017111: 48 8B 5C 24 38     mov         rbx,qword ptr [rsp+38h]
  0000000061017116: 48 8B 74 24 40     mov         rsi,qword ptr [rsp+40h]
  000000006101711B: 48 83 C4 20        add         rsp,20h
  000000006101711F: 5F                 pop         rdi
  0000000061017120: C3                 ret
  0000000061017121: 48 8B 5C 24 38     mov         rbx,qword ptr [rsp+38h]
  0000000061017126: 48 8B 74 24 40     mov         rsi,qword ptr [rsp+40h]
  000000006101712B: B0 01              mov         al,1
  000000006101712D: 48 83 C4 20        add         rsp,20h
  0000000061017131: 5F                 pop         rdi
  0000000061017132: C3                 ret

  Summary

       42000 .text
