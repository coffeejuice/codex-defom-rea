Filtered bounded disassembly from retained DUMPBIN captures; original PE SHA-256 fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc.
Factory constructor c36b00 writes SfPstGraphFactory vftable:
  0000000140C36B00: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  0000000140C36B05: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  0000000140C36B0A: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140C36B0F: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C36B14: 48 83 EC 28        sub         rsp,28h
  0000000140C36B18: 4C 8B 4C 24 48     mov         r9,qword ptr [rsp+48h]
  0000000140C36B1D: 4C 8B 44 24 40     mov         r8,qword ptr [rsp+40h]
  0000000140C36B22: 48 8B 54 24 38     mov         rdx,qword ptr [rsp+38h]
  0000000140C36B27: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140C36B2C: E8 8F 1F 00 00     call        0000000140C38AC0
  0000000140C36B31: 4C 8B 5C 24 30     mov         r11,qword ptr [rsp+30h]
  0000000140C36B36: 48 8D 05 8B 65 C1  lea         rax,[000000014284D0C8h]
  0000000140C36B3D: 49 89 03           mov         qword ptr [r11],rax
  0000000140C36B40: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140C36B45: 48 83 C4 28        add         rsp,28h
  0000000140C36B49: C3                 ret

Derived factory destructor c36b50 restores the base vftable before base cleanup:
  0000000140C36B50: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C36B55: 48 83 EC 28        sub         rsp,28h
  0000000140C36B59: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140C36B5E: 48 8D 05 63 65 C1  lea         rax,[000000014284D0C8h]
  0000000140C36B65: 48 89 01           mov         qword ptr [rcx],rax
  0000000140C36B68: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140C36B6D: E8 BE 1F 00 00     call        0000000140C38B30
  0000000140C36B72: 48 83 C4 28        add         rsp,28h
  0000000140C36B76: C3                 ret

Manager wrapper c15910 dispatches factory virtual +0x60 and returns the result:
  0000000140C15910: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140C15915: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C1591A: 48 83 EC 38        sub         rsp,38h
  0000000140C1591E: 48 C7 44 24 20 00  mov         qword ptr [rsp+20h],0
  0000000140C15927: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140C1592C: 48 83 78 18 00     cmp         qword ptr [rax+18h],0
  0000000140C15931: 75 04              jne         0000000140C15937
  0000000140C15933: 33 C0              xor         eax,eax
  0000000140C15935: EB 27              jmp         0000000140C1595E
  0000000140C15937: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140C1593C: 48 8B 40 18        mov         rax,qword ptr [rax+18h]
  0000000140C15940: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140C15945: 48 8B 49 18        mov         rcx,qword ptr [rcx+18h]
  0000000140C15949: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C1594C: 48 8B 54 24 48     mov         rdx,qword ptr [rsp+48h]
  0000000140C15951: FF 50 60           call        qword ptr [rax+60h]
  0000000140C15954: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140C15959: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140C1595E: 48 83 C4 38        add         rsp,38h
  0000000140C15962: C3                 ret
  0000000140C15963: CC                 int         3
