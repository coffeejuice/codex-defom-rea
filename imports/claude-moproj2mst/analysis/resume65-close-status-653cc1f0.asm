
Dump of file evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000653CC1F0: 40 57              push        rdi
  00000000653CC1F2: 48 83 EC 30        sub         rsp,30h
  00000000653CC1F6: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000653CC1FF: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  00000000653CC204: 48 8B DA           mov         rbx,rdx
  00000000653CC207: 48 8B F9           mov         rdi,rcx
  00000000653CC20A: 48 85 D2           test        rdx,rdx
  00000000653CC20D: 74 05              je          00000000653CC214
  00000000653CC20F: E8 2C 93 C7 FF     call        0000000065045540
  00000000653CC214: 48 8B 8F F8 04 00  mov         rcx,qword ptr [rdi+00000000000004F8h]
                    00
  00000000653CC21B: 48 85 C9           test        rcx,rcx
  00000000653CC21E: 74 0A              je          00000000653CC22A
  00000000653CC220: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000653CC223: BA 01 00 00 00     mov         edx,1
  00000000653CC228: FF 10              call        qword ptr [rax]
  00000000653CC22A: 48 85 DB           test        rbx,rbx
  00000000653CC22D: 74 25              je          00000000653CC254
  00000000653CC22F: B9 58 00 00 00     mov         ecx,58h
  00000000653CC234: E8 39 4F 21 00     call        00000000655E1172
  00000000653CC239: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000000653CC23E: 48 85 C0           test        rax,rax
  00000000653CC241: 74 0D              je          00000000653CC250
  00000000653CC243: 48 8B D3           mov         rdx,rbx
  00000000653CC246: 48 8B C8           mov         rcx,rax
  00000000653CC249: E8 62 D2 C7 FF     call        00000000650494B0
  00000000653CC24E: EB 02              jmp         00000000653CC252
  00000000653CC250: 33 C0              xor         eax,eax
  00000000653CC252: EB 02              jmp         00000000653CC256
  00000000653CC254: 33 C0              xor         eax,eax
  00000000653CC256: 48 89 87 F8 04 00  mov         qword ptr [rdi+00000000000004F8h],rax
                    00
  00000000653CC25D: 48 8D 4F 10        lea         rcx,[rdi+10h]
  00000000653CC261: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000653CC264: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  00000000653CC269: 48 83 C4 30        add         rsp,30h
  00000000653CC26D: 5F                 pop         rdi
  00000000653CC26E: 48 FF 60 58        jmp         qword ptr [rax+58h]

  Summary

      695000 .text
