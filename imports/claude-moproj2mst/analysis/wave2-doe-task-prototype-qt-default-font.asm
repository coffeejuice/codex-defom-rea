
Dump of file evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000651F3560: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000000651F3565: 57                 push        rdi
  00000000651F3566: 48 83 EC 40        sub         rsp,40h
  00000000651F356A: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000651F3573: 48 89 5C 24 58     mov         qword ptr [rsp+58h],rbx
  00000000651F3578: 48 8B D9           mov         rbx,rcx
  00000000651F357B: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000000651F3580: E8 AB 7D E2 FF     call        000000006501B330
  00000000651F3585: 90                 nop
  00000000651F3586: 48 8B 08           mov         rcx,qword ptr [rax]
  00000000651F3589: 48 89 0B           mov         qword ptr [rbx],rcx
  00000000651F358C: 48 85 C9           test        rcx,rcx
  00000000651F358F: 74 07              je          00000000651F3598
  00000000651F3591: FF 15 29 33 4A 00  call        qword ptr [00000000656968C0h]
  00000000651F3597: 90                 nop
  00000000651F3598: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  00000000651F359D: 48 85 C9           test        rcx,rcx
  00000000651F35A0: 74 24              je          00000000651F35C6
  00000000651F35A2: FF 15 D0 33 4A 00  call        qword ptr [0000000065696978h]
  00000000651F35A8: 84 C0              test        al,al
  00000000651F35AA: 75 1A              jne         00000000651F35C6
  00000000651F35AC: 48 8B 7C 24 28     mov         rdi,qword ptr [rsp+28h]
  00000000651F35B1: 48 85 FF           test        rdi,rdi
  00000000651F35B4: 74 10              je          00000000651F35C6
  00000000651F35B6: 48 8B CF           mov         rcx,rdi
  00000000651F35B9: E8 82 EA FF FF     call        00000000651F2040
  00000000651F35BE: 48 8B CF           mov         rcx,rdi
  00000000651F35C1: E8 E8 DA 3E 00     call        00000000655E10AE
  00000000651F35C6: C7 43 08 00 00 00  mov         dword ptr [rbx+8],0
                    00
  00000000651F35CD: 48 8B C3           mov         rax,rbx
  00000000651F35D0: 48 8B 5C 24 58     mov         rbx,qword ptr [rsp+58h]
  00000000651F35D5: 48 83 C4 40        add         rsp,40h
  00000000651F35D9: 5F                 pop         rdi
  00000000651F35DA: C3                 ret

  Summary

      695000 .text
