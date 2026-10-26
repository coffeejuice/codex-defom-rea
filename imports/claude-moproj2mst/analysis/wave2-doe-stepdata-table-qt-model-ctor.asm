
Dump of file evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006552CE10: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000006552CE15: 57                 push        rdi
  000000006552CE16: 48 83 EC 40        sub         rsp,40h
  000000006552CE1A: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006552CE23: 48 89 5C 24 58     mov         qword ptr [rsp+58h],rbx
  000000006552CE28: 48 8B DA           mov         rbx,rdx
  000000006552CE2B: 48 8B F9           mov         rdi,rcx
  000000006552CE2E: B9 E0 00 00 00     mov         ecx,0E0h
  000000006552CE33: E8 3A 43 0B 00     call        00000000655E1172
  000000006552CE38: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  000000006552CE3D: 48 85 C0           test        rax,rax
  000000006552CE40: 74 0A              je          000000006552CE4C
  000000006552CE42: 48 8B C8           mov         rcx,rax
  000000006552CE45: E8 36 E8 FF FF     call        000000006552B680
  000000006552CE4A: EB 02              jmp         000000006552CE4E
  000000006552CE4C: 33 C0              xor         eax,eax
  000000006552CE4E: 4C 8B C3           mov         r8,rbx
  000000006552CE51: 48 8B D0           mov         rdx,rax
  000000006552CE54: 48 8B CF           mov         rcx,rdi
  000000006552CE57: FF 15 C3 C3 16 00  call        qword ptr [0000000065699220h]
  000000006552CE5D: 90                 nop
  000000006552CE5E: 48 8D 05 13 04 21  lea         rax,[000000006573D278h]
                    00
  000000006552CE65: 48 89 07           mov         qword ptr [rdi],rax
  000000006552CE68: 48 8B 5F 08        mov         rbx,qword ptr [rdi+8]
  000000006552CE6C: 48 8B 4B 08        mov         rcx,qword ptr [rbx+8]
  000000006552CE70: C7 44 24 20 03 00  mov         dword ptr [rsp+20h],3
                    00 00
  000000006552CE78: 4C 8D 0D B9 03 21  lea         r9,[000000006573D238h]
                    00
  000000006552CE7F: 4C 8B C1           mov         r8,rcx
  000000006552CE82: 48 8D 15 3F CF 1C  lea         rdx,[00000000656F9DC8h]
                    00
  000000006552CE89: FF 15 89 98 16 00  call        qword ptr [0000000065696718h]
  000000006552CE8F: 4C 8B 9B C8 00 00  mov         r11,qword ptr [rbx+00000000000000C8h]
                    00
  000000006552CE96: 48 8B D7           mov         rdx,rdi
  000000006552CE99: 49 8B 4B 08        mov         rcx,qword ptr [r11+8]
  000000006552CE9D: E8 9E F4 FF FF     call        000000006552C340
  000000006552CEA2: 90                 nop
  000000006552CEA3: 48 8B C7           mov         rax,rdi
  000000006552CEA6: 48 8B 5C 24 58     mov         rbx,qword ptr [rsp+58h]
  000000006552CEAB: 48 83 C4 40        add         rsp,40h
  000000006552CEAF: 5F                 pop         rdi
  000000006552CEB0: C3                 ret

  Summary

      695000 .text
