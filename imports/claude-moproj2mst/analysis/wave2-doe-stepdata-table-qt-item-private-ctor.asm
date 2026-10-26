
Dump of file evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006552A980: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000006552A985: 57                 push        rdi
  000000006552A986: 48 83 EC 30        sub         rsp,30h
  000000006552A98A: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006552A993: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  000000006552A998: 48 8B D9           mov         rbx,rcx
  000000006552A99B: 48 8D 05 CE 2C 21  lea         rax,[000000006573D670h]
                    00
  000000006552A9A2: 48 89 01           mov         qword ptr [rcx],rax
  000000006552A9A5: 33 FF              xor         edi,edi
  000000006552A9A7: 48 89 79 08        mov         qword ptr [rcx+8],rdi
  000000006552A9AB: 48 89 79 10        mov         qword ptr [rcx+10h],rdi
  000000006552A9AF: 48 8B 05 8A C1 16  mov         rax,qword ptr [0000000065696B40h]
                    00
  000000006552A9B6: 48 89 41 18        mov         qword ptr [rcx+18h],rax
  000000006552A9BA: 48 8B 0D 7F C1 16  mov         rcx,qword ptr [0000000065696B40h]
                    00
  000000006552A9C1: FF 15 F9 BE 16 00  call        qword ptr [00000000656968C0h]
  000000006552A9C7: 90                 nop
  000000006552A9C8: 48 8B 05 71 C1 16  mov         rax,qword ptr [0000000065696B40h]
                    00
  000000006552A9CF: 48 89 43 20        mov         qword ptr [rbx+20h],rax
  000000006552A9D3: 48 8B 0D 66 C1 16  mov         rcx,qword ptr [0000000065696B40h]
                    00
  000000006552A9DA: FF 15 E0 BE 16 00  call        qword ptr [00000000656968C0h]
  000000006552A9E0: 89 7B 28           mov         dword ptr [rbx+28h],edi
  000000006552A9E3: 89 7B 2C           mov         dword ptr [rbx+2Ch],edi
  000000006552A9E6: 48 89 7B 30        mov         qword ptr [rbx+30h],rdi
  000000006552A9EA: C7 43 38 02 00 00  mov         dword ptr [rbx+38h],2
                    00
  000000006552A9F1: 48 8B C3           mov         rax,rbx
  000000006552A9F4: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  000000006552A9F9: 48 83 C4 30        add         rsp,30h
  000000006552A9FD: 5F                 pop         rdi
  000000006552A9FE: C3                 ret

  Summary

      695000 .text
