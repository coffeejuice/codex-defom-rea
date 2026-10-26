
Dump of file evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000653C9C50: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  00000000653C9C55: 57                 push        rdi
  00000000653C9C56: 48 83 EC 20        sub         rsp,20h
  00000000653C9C5A: 48 8B 59 08        mov         rbx,qword ptr [rcx+8]
  00000000653C9C5E: 48 8B FA           mov         rdi,rdx
  00000000653C9C61: 48 8B 8B E0 01 00  mov         rcx,qword ptr [rbx+00000000000001E0h]
                    00
  00000000653C9C68: E8 13 31 00 00     call        00000000653CCD80
  00000000653C9C6D: 48 85 C0           test        rax,rax
  00000000653C9C70: 74 3E              je          00000000653C9CB0
  00000000653C9C72: 48 8B 8B E0 01 00  mov         rcx,qword ptr [rbx+00000000000001E0h]
                    00
  00000000653C9C79: E8 02 31 00 00     call        00000000653CCD80
  00000000653C9C7E: 48 3B C7           cmp         rax,rdi
  00000000653C9C81: 74 2D              je          00000000653C9CB0
  00000000653C9C83: 48 8B 8B E0 01 00  mov         rcx,qword ptr [rbx+00000000000001E0h]
                    00
  00000000653C9C8A: E8 F1 30 00 00     call        00000000653CCD80
  00000000653C9C8F: 33 D2              xor         edx,edx
  00000000653C9C91: 4C 8B 00           mov         r8,qword ptr [rax]
  00000000653C9C94: 48 8B C8           mov         rcx,rax
  00000000653C9C97: 41 FF 50 58        call        qword ptr [r8+58h]
  00000000653C9C9B: 48 8B 8B E0 01 00  mov         rcx,qword ptr [rbx+00000000000001E0h]
                    00
  00000000653C9CA2: E8 D9 30 00 00     call        00000000653CCD80
  00000000653C9CA7: 48 8B C8           mov         rcx,rax
  00000000653C9CAA: FF 15 68 D5 2C 00  call        qword ptr [0000000065697218h]
  00000000653C9CB0: 48 8B 8B E0 01 00  mov         rcx,qword ptr [rbx+00000000000001E0h]
                    00
  00000000653C9CB7: 48 8B D7           mov         rdx,rdi
  00000000653C9CBA: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  00000000653C9CBF: 48 83 C4 20        add         rsp,20h
  00000000653C9CC3: 5F                 pop         rdi
  00000000653C9CC4: E9 27 25 00 00     jmp         00000000653CC1F0

  Summary

      695000 .text
