
Dump of file evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000650AF620: 40 57              push        rdi
  00000000650AF622: 48 83 EC 40        sub         rsp,40h
  00000000650AF626: 48 C7 44 24 38 FE  mov         qword ptr [rsp+38h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000650AF62F: 48 89 5C 24 58     mov         qword ptr [rsp+58h],rbx
  00000000650AF634: 48 89 74 24 60     mov         qword ptr [rsp+60h],rsi
  00000000650AF639: 41 8B D9           mov         ebx,r9d
  00000000650AF63C: 48 8B F2           mov         rsi,rdx
  00000000650AF63F: 48 8B F9           mov         rdi,rcx
  00000000650AF642: 48 83 79 10 00     cmp         qword ptr [rcx+10h],0
  00000000650AF647: 75 07              jne         00000000650AF650
  00000000650AF649: 32 C0              xor         al,al
  00000000650AF64B: E9 8D 00 00 00     jmp         00000000650AF6DD
  00000000650AF650: 49 8B D0           mov         rdx,r8
  00000000650AF653: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000000650AF658: FF 15 CA 76 5E 00  call        qword ptr [0000000065696D28h]
  00000000650AF65E: 90                 nop
  00000000650AF65F: 4C 8D 44 24 30     lea         r8,[rsp+30h]
  00000000650AF664: 48 8B D6           mov         rdx,rsi
  00000000650AF667: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000000650AF66C: E8 9F 40 01 00     call        00000000650C3710
  00000000650AF671: 90                 nop
  00000000650AF672: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000000650AF677: FF 15 D3 73 5E 00  call        qword ptr [0000000065696A50h]
  00000000650AF67D: 89 5C 24 50        mov         dword ptr [rsp+50h],ebx
  00000000650AF681: 83 FB 64           cmp         ebx,64h
  00000000650AF684: 7F 05              jg          00000000650AF68B
  00000000650AF686: 83 FB FF           cmp         ebx,0FFFFFFFFh
  00000000650AF689: 7D 0D              jge         00000000650AF698
  00000000650AF68B: 48 8D 0D 56 33 60  lea         rcx,[00000000656B29E8h]
                    00
  00000000650AF692: FF 15 00 73 5E 00  call        qword ptr [0000000065696998h]
  00000000650AF698: 85 DB              test        ebx,ebx

  Summary

      695000 .text
