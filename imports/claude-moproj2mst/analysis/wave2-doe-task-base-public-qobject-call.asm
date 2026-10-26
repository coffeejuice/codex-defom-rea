
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000141403210: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000141403215: 48 83 EC 38        sub         rsp,38h
  0000000141403219: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000141403222: 33 D2              xor         edx,edx
  0000000141403224: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141403229: FF 15 91 0F A0 00  call        qword ptr [0000000141E041C0h]
  000000014140322F: 90                 nop
  0000000141403230: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141403235: 48 8D 05 EC 08 A9  lea         rax,[0000000142E93B28h]
                    01
  000000014140323C: 48 89 01           mov         qword ptr [rcx],rax
  000000014140323F: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141403244: 48 83 C1 20        add         rcx,20h
  0000000141403248: E8 43 AE 00 00     call        000000014140E090

  Summary

     1E02000 .text
