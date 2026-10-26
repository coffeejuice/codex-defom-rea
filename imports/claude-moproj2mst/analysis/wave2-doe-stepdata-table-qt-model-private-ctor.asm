
Dump of file evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006552B680: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000006552B685: 57                 push        rdi
  000000006552B686: 48 83 EC 40        sub         rsp,40h
  000000006552B68A: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006552B693: 48 89 5C 24 68     mov         qword ptr [rsp+68h],rbx
  000000006552B698: 48 8B F9           mov         rdi,rcx
  000000006552B69B: FF 15 B7 DA 16 00  call        qword ptr [0000000065699158h]
  000000006552B6A1: 90                 nop
  000000006552B6A2: 48 8D 05 07 20 21  lea         rax,[000000006573D6B0h]
                    00
  000000006552B6A9: 48 89 07           mov         qword ptr [rdi],rax
  000000006552B6AC: 48 8B 05 8D B4 16  mov         rax,qword ptr [0000000065696B40h]
                    00
  000000006552B6B3: 48 89 87 B8 00 00  mov         qword ptr [rdi+00000000000000B8h],rax
                    00
  000000006552B6BA: 48 8B 0D 7F B4 16  mov         rcx,qword ptr [0000000065696B40h]
                    00
  000000006552B6C1: FF 15 F9 B1 16 00  call        qword ptr [00000000656968C0h]
  000000006552B6C7: 90                 nop
  000000006552B6C8: 48 8B 05 71 B4 16  mov         rax,qword ptr [0000000065696B40h]
                    00
  000000006552B6CF: 48 89 87 C0 00 00  mov         qword ptr [rdi+00000000000000C0h],rax
                    00
  000000006552B6D6: 48 8B 0D 63 B4 16  mov         rcx,qword ptr [0000000065696B40h]
                    00
  000000006552B6DD: FF 15 DD B1 16 00  call        qword ptr [00000000656968C0h]
  000000006552B6E3: 90                 nop
  000000006552B6E4: B9 10 00 00 00     mov         ecx,10h
  000000006552B6E9: E8 84 5A 0B 00     call        00000000655E1172
  000000006552B6EE: 48 8B D8           mov         rbx,rax
  000000006552B6F1: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  000000006552B6F6: 48 85 C0           test        rax,rax
  000000006552B6F9: 74 34              je          000000006552B72F
  000000006552B6FB: 48 8D 05 46 1D 21  lea         rax,[000000006573D448h]
                    00
  000000006552B702: 48 89 03           mov         qword ptr [rbx],rax
  000000006552B705: B9 40 00 00 00     mov         ecx,40h
  000000006552B70A: E8 63 5A 0B 00     call        00000000655E1172
  000000006552B70F: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  000000006552B714: 48 85 C0           test        rax,rax
  000000006552B717: 74 0A              je          000000006552B723
  000000006552B719: 48 8B C8           mov         rcx,rax
  000000006552B71C: E8 5F F2 FF FF     call        000000006552A980
  000000006552B721: EB 02              jmp         000000006552B725
  000000006552B723: 33 C0              xor         eax,eax
  000000006552B725: 48 89 43 08        mov         qword ptr [rbx+8],rax
  000000006552B729: 48 89 58 30        mov         qword ptr [rax+30h],rbx
  000000006552B72D: EB 02              jmp         000000006552B731
  000000006552B72F: 33 DB              xor         ebx,ebx
  000000006552B731: 48 89 9F C8 00 00  mov         qword ptr [rdi+00000000000000C8h],rbx
                    00
  000000006552B738: 48 C7 87 D0 00 00  mov         qword ptr [rdi+00000000000000D0h],0
                    00 00 00 00 00
  000000006552B743: C7 87 D8 00 00 00  mov         dword ptr [rdi+00000000000000D8h],0
                    00 00 00 00
  000000006552B74D: 48 8B 9F C8 00 00  mov         rbx,qword ptr [rdi+00000000000000C8h]
                    00
  000000006552B754: BA 08 00 00 00     mov         edx,8
  000000006552B759: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006552B75E: FF 15 F4 BA 16 00  call        qword ptr [0000000065697258h]
  000000006552B764: 90                 nop
  000000006552B765: 48 8B 03           mov         rax,qword ptr [rbx]
  000000006552B768: 41 B8 1F 00 00 00  mov         r8d,1Fh
  000000006552B76E: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  000000006552B773: 48 8B CB           mov         rcx,rbx
  000000006552B776: FF 50 10           call        qword ptr [rax+10h]
  000000006552B779: 90                 nop
  000000006552B77A: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006552B77F: FF 15 5B AF 16 00  call        qword ptr [00000000656966E0h]
  000000006552B785: 90                 nop
  000000006552B786: 48 8B C7           mov         rax,rdi
  000000006552B789: 48 8B 5C 24 68     mov         rbx,qword ptr [rsp+68h]
  000000006552B78E: 48 83 C4 40        add         rsp,40h
  000000006552B792: 5F                 pop         rdi
  000000006552B793: C3                 ret

  Summary

      695000 .text
