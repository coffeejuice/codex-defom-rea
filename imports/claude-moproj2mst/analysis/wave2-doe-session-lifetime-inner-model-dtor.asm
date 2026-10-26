
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014064F520: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014064F525: 48 83 EC 38        sub         rsp,38h
  000000014064F529: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000014064F532: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014064F537: 48 8D 05 EA 51 E9  lea         rax,[00000001424E4728h]
                    01
  000000014064F53E: 48 89 01           mov         qword ptr [rcx],rax
  000000014064F541: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014064F546: 48 83 C1 38        add         rcx,38h
  000000014064F54A: E8 B1 D3 66 00     call        0000000140CBC900
  000000014064F54F: 90                 nop
  000000014064F550: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014064F555: 48 83 C1 30        add         rcx,30h
  000000014064F559: E8 42 8C BB FF     call        00000001402081A0
  000000014064F55E: 90                 nop
  000000014064F55F: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014064F564: 48 83 C1 18        add         rcx,18h
  000000014064F568: FF 15 42 D1 7B 01  call        qword ptr [0000000141E0C6B0h]
  000000014064F56E: 90                 nop
  000000014064F56F: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014064F574: FF 15 36 4D 7B 01  call        qword ptr [0000000141E042B0h]
  000000014064F57A: 48 83 C4 38        add         rsp,38h
  000000014064F57E: C3                 ret

  Summary

     1E02000 .text
