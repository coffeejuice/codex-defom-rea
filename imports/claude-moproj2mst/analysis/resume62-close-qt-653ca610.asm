
Dump of file evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000653CA610: 40 57              push        rdi
  00000000653CA612: 48 83 EC 30        sub         rsp,30h
  00000000653CA616: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000653CA61F: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  00000000653CA624: 48 8B F9           mov         rdi,rcx
  00000000653CA627: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  00000000653CA62B: 48 8B 89 E0 01 00  mov         rcx,qword ptr [rcx+00000000000001E0h]
                    00
  00000000653CA632: E8 49 27 00 00     call        00000000653CCD80
  00000000653CA637: 48 85 C0           test        rax,rax
  00000000653CA63A: 75 3F              jne         00000000653CA67B
  00000000653CA63C: 8D 48 28           lea         ecx,[rax+28h]
  00000000653CA63F: E8 2E 6B 21 00     call        00000000655E1172
  00000000653CA644: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000000653CA649: 48 85 C0           test        rax,rax
  00000000653CA64C: 74 10              je          00000000653CA65E
  00000000653CA64E: 48 8B D7           mov         rdx,rdi
  00000000653CA651: 48 8B C8           mov         rcx,rax
  00000000653CA654: E8 F7 C6 03 00     call        0000000065406D50
  00000000653CA659: 48 8B D8           mov         rbx,rax
  00000000653CA65C: EB 02              jmp         00000000653CA660
  00000000653CA65E: 33 DB              xor         ebx,ebx
  00000000653CA660: BA 0D 00 00 00     mov         edx,0Dh
  00000000653CA665: 48 8B CB           mov         rcx,rbx
  00000000653CA668: E8 B3 34 CA FF     call        000000006506DB20
  00000000653CA66D: 48 8B D3           mov         rdx,rbx
  00000000653CA670: 48 8B CF           mov         rcx,rdi
  00000000653CA673: E8 D8 F5 FF FF     call        00000000653C9C50
  00000000653CA678: 48 8B C3           mov         rax,rbx
  00000000653CA67B: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  00000000653CA680: 48 83 C4 30        add         rsp,30h
  00000000653CA684: 5F                 pop         rdi
  00000000653CA685: C3                 ret

  Summary

      695000 .text
