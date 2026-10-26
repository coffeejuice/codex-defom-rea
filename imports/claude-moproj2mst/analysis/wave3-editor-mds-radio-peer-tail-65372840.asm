Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065372840: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000065372845: 53                 push        rbx
  0000000065372846: 57                 push        rdi
  0000000065372847: 48 83 EC 38        sub         rsp,38h
  000000006537284B: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000065372854: 48 8B FA           mov         rdi,rdx
  0000000065372857: 48 8B D9           mov         rbx,rcx
  000000006537285A: C7 44 24 50 00 00  mov         dword ptr [rsp+50h],0
                    00 00
  0000000065372862: 48 8B 81 08 02 00  mov         rax,qword ptr [rcx+0000000000000208h]
                    00
  0000000065372869: 48 85 C0           test        rax,rax
  000000006537286C: 74 33              je          00000000653728A1
  000000006537286E: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  0000000065372872: 48 8B 88 88 00 00  mov         rcx,qword ptr [rax+0000000000000088h]
                    00
  0000000065372879: 48 89 0A           mov         qword ptr [rdx],rcx
  000000006537287C: FF 15 3E 40 32 00  call        qword ptr [00000000656968C0h]
  0000000065372882: 4C 8B 1F           mov         r11,qword ptr [rdi]
  0000000065372885: 41 F6 43 10 01     test        byte ptr [r11+10h],1
  000000006537288A: 75 0D              jne         0000000065372899
  000000006537288C: 41 8B 53 04        mov         edx,dword ptr [r11+4]
  0000000065372890: 48 8B CF           mov         rcx,rdi
  0000000065372893: E8 88 16 E3 FF     call        00000000651A3F20
  0000000065372898: 90                 nop
  0000000065372899: 48 8B C7           mov         rax,rdi
  000000006537289C: E9 20 01 00 00     jmp         00000000653729C1
  00000000653728A1: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000000653728A6: FF 15 BC 3E 32 00  call        qword ptr [0000000065696768h]
  00000000653728AC: 90                 nop
  00000000653728AD: 4C 8B C0           mov         r8,rax
  00000000653728B0: 48 8D 54 24 60     lea         rdx,[rsp+60h]
  00000000653728B5: 48 8B 4B 10        mov         rcx,qword ptr [rbx+10h]
  00000000653728B9: E8 F2 FD FF FF     call        00000000653726B0
  00000000653728BE: 90                 nop
  00000000653728BF: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000000653728C4: FF 15 96 3E 32 00  call        qword ptr [0000000065696760h]
  00000000653728CA: F6 83 04 02 00 00  test        byte ptr [rbx+0000000000000204h],8
                    08
  00000000653728D1: 0F 84 A1 00 00 00  je          0000000065372978
  00000000653728D7: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  00000000653728DC: FF 15 D6 3F 32 00  call        qword ptr [00000000656968B8h]
  00000000653728E2: 8B D8              mov         ebx,eax
  00000000653728E4: 83 EB 01           sub         ebx,1
  00000000653728E7: 0F 88 8B 00 00 00  js          0000000065372978
  00000000653728ED: 0F 1F 00           nop         dword ptr [rax]
  00000000653728F0: 8B D3              mov         edx,ebx
  00000000653728F2: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  00000000653728F7: FF 15 43 40 32 00  call        qword ptr [0000000065696940h]
  00000000653728FD: 48 8B 08           mov         rcx,qword ptr [rax]
  0000000065372900: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  0000000065372904: 8B 88 04 02 00 00  mov         ecx,dword ptr [rax+0000000000000204h]
  000000006537290A: C1 E9 03           shr         ecx,3
  000000006537290D: F6 C1 01           test        cl,1
  0000000065372910: 74 0A              je          000000006537291C
  0000000065372912: 48 83 B8 08 02 00  cmp         qword ptr [rax+0000000000000208h],0
                    00 00
  000000006537291A: 74 53              je          000000006537296F
  000000006537291C: 85 DB              test        ebx,ebx
  000000006537291E: 78 4F              js          000000006537296F
  0000000065372920: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  0000000065372925: FF 15 8D 3F 32 00  call        qword ptr [00000000656968B8h]
  000000006537292B: 3B D8              cmp         ebx,eax
  000000006537292D: 7D 40              jge         000000006537296F
  000000006537292F: BA 01 00 00 00     mov         edx,1
  0000000065372934: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000065372939: FF 15 31 41 32 00  call        qword ptr [0000000065696A70h]
  000000006537293F: 84 C0              test        al,al
  0000000065372941: 74 12              je          0000000065372955
  0000000065372943: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  0000000065372948: 8B 50 04           mov         edx,dword ptr [rax+4]
  000000006537294B: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  0000000065372950: E8 CB 15 E3 FF     call        00000000651A3F20
  0000000065372955: 8B D3              mov         edx,ebx
  0000000065372957: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  000000006537295C: FF 15 DE 3F 32 00  call        qword ptr [0000000065696940h]
  0000000065372962: 8B D3              mov         edx,ebx
  0000000065372964: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  0000000065372969: FF 15 11 41 32 00  call        qword ptr [0000000065696A80h]
  000000006537296F: 83 EB 01           sub         ebx,1
  0000000065372972: 0F 89 78 FF FF FF  jns         00000000653728F0
  0000000065372978: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000006537297D: 48 89 0F           mov         qword ptr [rdi],rcx
  0000000065372980: FF 15 3A 3F 32 00  call        qword ptr [00000000656968C0h]
  0000000065372986: 4C 8B 1F           mov         r11,qword ptr [rdi]
  0000000065372989: 41 F6 43 10 01     test        byte ptr [r11+10h],1
  000000006537298E: 75 0C              jne         000000006537299C
  0000000065372990: 41 8B 53 04        mov         edx,dword ptr [r11+4]
  0000000065372994: 48 8B CF           mov         rcx,rdi
  0000000065372997: E8 84 15 E3 FF     call        00000000651A3F20
  000000006537299C: C7 44 24 50 01 00  mov         dword ptr [rsp+50h],1
                    00 00
  00000000653729A4: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000000653729A9: FF 15 C9 3F 32 00  call        qword ptr [0000000065696978h]
  00000000653729AF: 84 C0              test        al,al
  00000000653729B1: 75 0B              jne         00000000653729BE
  00000000653729B3: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000000653729B8: FF 15 A2 3F 32 00  call        qword ptr [0000000065696960h]
  00000000653729BE: 48 8B C7           mov         rax,rdi
  00000000653729C1: 48 83 C4 38        add         rsp,38h
  00000000653729C5: 5F                 pop         rdi
  00000000653729C6: 5B                 pop         rbx
  00000000653729C7: C3                 ret
  00000000653729C8: CC                 int         3

  Summary

      695000 .text
