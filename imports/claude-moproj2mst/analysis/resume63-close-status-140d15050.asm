
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D15050: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140D15055: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D1505A: 48 83 EC 48        sub         rsp,48h
  0000000140D1505E: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140D15067: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  0000000140D1506C: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140D15071: FF 15 11 12 0F 01  call        qword ptr [0000000141E06288h]
  0000000140D15077: 90                 nop
  0000000140D15078: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140D1507D: 48 8D 05 84 83 CE  lea         rax,[00000001429FD408h]
                    01
  0000000140D15084: 48 89 01           mov         qword ptr [rcx],rax
  0000000140D15087: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140D1508C: 48 8D 05 3D 83 CE  lea         rax,[00000001429FD3D0h]
                    01
  0000000140D15093: 48 89 41 10        mov         qword ptr [rcx+10h],rax
  0000000140D15097: B2 01              mov         dl,1
  0000000140D15099: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140D1509E: FF 15 EC 41 0F 01  call        qword ptr [0000000141E09290h]
  0000000140D150A4: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  0000000140D150A9: 4C 8D 0D 68 82 CE  lea         r9,[00000001429FD318h]
                    01
  0000000140D150B0: 4C 8B 44 24 50     mov         r8,qword ptr [rsp+50h]
  0000000140D150B5: 48 8D 15 74 82 CE  lea         rdx,[00000001429FD330h]
                    01
  0000000140D150BC: 48 8D 0D 3D 16 06  lea         rcx,[0000000143D76700h]
                    03
  0000000140D150C3: E8 18 AB 5B FF     call        00000001402CFBE0
  0000000140D150C8: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  0000000140D150CD: 4C 8D 0D 74 82 CE  lea         r9,[00000001429FD348h]
                    01
  0000000140D150D4: 4C 8B 44 24 50     mov         r8,qword ptr [rsp+50h]
  0000000140D150D9: 48 8D 15 80 82 CE  lea         rdx,[00000001429FD360h]
                    01
  0000000140D150E0: 48 8D 0D 19 16 06  lea         rcx,[0000000143D76700h]
                    03
  0000000140D150E7: E8 F4 AA 5B FF     call        00000001402CFBE0
  0000000140D150EC: 90                 nop
  0000000140D150ED: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140D150F2: 48 83 C4 48        add         rsp,48h
  0000000140D150F6: C3                 ret

  Summary

     1E02000 .text
