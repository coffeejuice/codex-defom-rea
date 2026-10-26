
Dump of file evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006552ADC0: 40 53              push        rbx
  000000006552ADC2: 48 83 EC 30        sub         rsp,30h
  000000006552ADC6: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006552ADCF: 48 8B D9           mov         rbx,rcx
  000000006552ADD2: 48 8D 05 6F 26 21  lea         rax,[000000006573D448h]
                    00
  000000006552ADD9: 48 89 01           mov         qword ptr [rcx],rax
  000000006552ADDC: B9 40 00 00 00     mov         ecx,40h
  000000006552ADE1: E8 8C 63 0B 00     call        00000000655E1172
  000000006552ADE6: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  000000006552ADEB: 48 85 C0           test        rax,rax
  000000006552ADEE: 74 0A              je          000000006552ADFA
  000000006552ADF0: 48 8B C8           mov         rcx,rax
  000000006552ADF3: E8 88 FB FF FF     call        000000006552A980
  000000006552ADF8: EB 02              jmp         000000006552ADFC
  000000006552ADFA: 33 C0              xor         eax,eax
  000000006552ADFC: 48 89 43 08        mov         qword ptr [rbx+8],rax
  000000006552AE00: 48 89 58 30        mov         qword ptr [rax+30h],rbx
  000000006552AE04: 48 8B C3           mov         rax,rbx
  000000006552AE07: 48 83 C4 30        add         rsp,30h
  000000006552AE0B: 5B                 pop         rbx
  000000006552AE0C: C3                 ret

  Summary

      695000 .text
