
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014093AA30: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014093AA35: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014093AA3A: 56                 push        rsi
  000000014093AA3B: 57                 push        rdi
  000000014093AA3C: 48 83 EC 68        sub         rsp,68h
  000000014093AA40: 48 8D 7C 24 40     lea         rdi,[rsp+40h]
  000000014093AA45: 48 8B B4 24 88 00  mov         rsi,qword ptr [rsp+0000000000000088h]
                    00 00
  000000014093AA4D: B9 18 00 00 00     mov         ecx,18h
  000000014093AA52: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014093AA54: 48 8D 7C 24 20     lea         rdi,[rsp+20h]
  000000014093AA59: 48 8D 74 24 40     lea         rsi,[rsp+40h]
  000000014093AA5E: B9 18 00 00 00     mov         ecx,18h
  000000014093AA63: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014093AA65: 48 8B 8C 24 88 00  mov         rcx,qword ptr [rsp+0000000000000088h]
                    00 00
  000000014093AA6D: E8 AE FE FF FF     call        000000014093A920
  000000014093AA72: F2 0F 11 44 24 38  movsd       mmword ptr [rsp+38h],xmm0
  000000014093AA78: F2 0F 10 44 24 38  movsd       xmm0,mmword ptr [rsp+38h]
  000000014093AA7E: E8 F5 5F B3 00     call        0000000141470A78
  000000014093AA83: 66 0F 2F 05 05 D7  comisd      xmm0,mmword ptr [0000000142248190h]
                    90 01
  000000014093AA8B: 76 38              jbe         000000014093AAC5
  000000014093AA8D: F2 0F 10 44 24 20  movsd       xmm0,mmword ptr [rsp+20h]
  000000014093AA93: F2 0F 5E 44 24 38  divsd       xmm0,mmword ptr [rsp+38h]
  000000014093AA99: F2 0F 11 44 24 20  movsd       mmword ptr [rsp+20h],xmm0
  000000014093AA9F: F2 0F 10 44 24 28  movsd       xmm0,mmword ptr [rsp+28h]
  000000014093AAA5: F2 0F 5E 44 24 38  divsd       xmm0,mmword ptr [rsp+38h]
  000000014093AAAB: F2 0F 11 44 24 28  movsd       mmword ptr [rsp+28h],xmm0
  000000014093AAB1: F2 0F 10 44 24 30  movsd       xmm0,mmword ptr [rsp+30h]
  000000014093AAB7: F2 0F 5E 44 24 38  divsd       xmm0,mmword ptr [rsp+38h]
  000000014093AABD: F2 0F 11 44 24 30  movsd       mmword ptr [rsp+30h],xmm0
  000000014093AAC3: EB 22              jmp         000000014093AAE7
  000000014093AAC5: 66 0F 57 C0        xorpd       xmm0,xmm0
  000000014093AAC9: F2 0F 11 44 24 30  movsd       mmword ptr [rsp+30h],xmm0
  000000014093AACF: F2 0F 10 44 24 30  movsd       xmm0,mmword ptr [rsp+30h]
  000000014093AAD5: F2 0F 11 44 24 28  movsd       mmword ptr [rsp+28h],xmm0
  000000014093AADB: F2 0F 10 44 24 28  movsd       xmm0,mmword ptr [rsp+28h]
  000000014093AAE1: F2 0F 11 44 24 20  movsd       mmword ptr [rsp+20h],xmm0
  000000014093AAE7: 48 8D 74 24 20     lea         rsi,[rsp+20h]
  000000014093AAEC: 48 8B BC 24 80 00  mov         rdi,qword ptr [rsp+0000000000000080h]
                    00 00
  000000014093AAF4: B9 18 00 00 00     mov         ecx,18h
  000000014093AAF9: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014093AAFB: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  000000014093AB03: 48 83 C4 68        add         rsp,68h
  000000014093AB07: 5F                 pop         rdi
  000000014093AB08: 5E                 pop         rsi
  000000014093AB09: C3                 ret

  Summary

     1E02000 .text
