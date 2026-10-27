140e23a00  4c894c2420               MOV qword ptr [RSP + 0x20],R9
140e23a05  4c89442418               MOV qword ptr [RSP + 0x18],R8
140e23a0a  4889542410               MOV qword ptr [RSP + 0x10],RDX
140e23a0f  48894c2408               MOV qword ptr [RSP + 0x8],RCX
140e23a14  4881ec28020000           SUB RSP,0x228
140e23a1b  48c7842408020000feffffff MOV qword ptr [RSP + 0x208],-0x2
140e23a27  48c78424b001000000000000 MOV qword ptr [RSP + 0x1b0],0x0
140e23a33  48c744243800000000       MOV qword ptr [RSP + 0x38],0x0
140e23a3c  488d8c2488010000         LEA RCX,[RSP + 0x188]
140e23a44  e8378a3f00               CALL 0x14121c480
140e23a49  90                       NOP
140e23a4a  488d8c24b8010000         LEA RCX,[RSP + 0x1b8]
140e23a52  e8298a3f00               CALL 0x14121c480
140e23a57  90                       NOP
140e23a58  488b8c2438020000         MOV RCX,qword ptr [RSP + 0x238]
140e23a60  e8abdf3c00               CALL 0x1411f1a10
140e23a65  4889842460010000         MOV qword ptr [RSP + 0x160],RAX
140e23a6d  c78424f801000001000000   MOV dword ptr [RSP + 0x1f8],0x1
140e23a78  488d9424f8010000         LEA RDX,[RSP + 0x1f8]
140e23a80  488b8c2460010000         MOV RCX,qword ptr [RSP + 0x160]
140e23a88  e8b3af3c00               CALL 0x1411eea40
140e23a8d  0fb6c0                   MOVZX EAX,AL
140e23a90  85c0                     TEST EAX,EAX
140e23a92  744e                     JZ 0x140e23ae2
140e23a94  c78424fc01000002000000   MOV dword ptr [RSP + 0x1fc],0x2
140e23a9f  488d9424fc010000         LEA RDX,[RSP + 0x1fc]
140e23aa7  488b8c2460010000         MOV RCX,qword ptr [RSP + 0x160]
140e23aaf  e88caf3c00               CALL 0x1411eea40
140e23ab4  0fb6c0                   MOVZX EAX,AL
140e23ab7  85c0                     TEST EAX,EAX
140e23ab9  7547                     JNZ 0x140e23b02
140e23abb  c784240002000003000000   MOV dword ptr [RSP + 0x200],0x3
140e23ac6  488d942400020000         LEA RDX,[RSP + 0x200]
140e23ace  488b8c2460010000         MOV RCX,qword ptr [RSP + 0x160]
140e23ad6  e865af3c00               CALL 0x1411eea40
140e23adb  0fb6c0                   MOVZX EAX,AL
140e23ade  85c0                     TEST EAX,EAX
140e23ae0  7520                     JNZ 0x140e23b02
140e23ae2  488d8c24b8010000         LEA RCX,[RSP + 0x1b8]
140e23aea  e8818a3f00               CALL 0x14121c570
140e23aef  90                       NOP
140e23af0  488d8c2488010000         LEA RCX,[RSP + 0x188]
140e23af8  e8738a3f00               CALL 0x14121c570
140e23afd  e96c020000               JMP 0x140e23d6e
140e23b02  488b842438020000         MOV RAX,qword ptr [RSP + 0x238]
140e23b0a  480548030000             ADD RAX,0x348
140e23b10  4889442438               MOV qword ptr [RSP + 0x38],RAX
140e23b15  488b4c2438               MOV RCX,qword ptr [RSP + 0x38]
140e23b1a  e801594200               CALL 0x141249420
140e23b1f  448bd8                   MOV R11D,EAX
140e23b22  488b842440020000         MOV RAX,qword ptr [RSP + 0x240]
140e23b2a  488d8c24e0000000         LEA RCX,[RSP + 0xe0]
140e23b32  e8e921b2ff               CALL 0x140945d20
140e23b37  4c8b842450020000         MOV R8,qword ptr [RSP + 0x250]
140e23b3f  488b942448020000         MOV RDX,qword ptr [RSP + 0x248]
140e23b47  488d8c24e0000000         LEA RCX,[RSP + 0xe0]
140e23b4f  e84c31b2ff               CALL 0x140946ca0
140e23b54  488b842440020000         MOV RAX,qword ptr [RSP + 0x240]
140e23b5c  833800                   CMP dword ptr [RAX],0x0
140e23b5f  750d                     JNZ 0x140e23b6e
140e23b61  c684246801000000         MOV byte ptr [RSP + 0x168],0x0
140e23b69  e95a010000               JMP 0x140e23cc8
140e23b6e  488b842440020000         MOV RAX,qword ptr [RSP + 0x240]
140e23b76  8b00                     MOV EAX,dword ptr [RAX]
140e23b78  83e801                   SUB EAX,0x1
140e23b7b  4863c8                   MOVSXD RCX,EAX
140e23b7e  4869c908010000           IMUL RCX,RCX,0x108
140e23b85  488b442438               MOV RAX,qword ptr [RSP + 0x38]
140e23b8a  4803c1                   ADD RAX,RCX
140e23b8d  48898424b0010000         MOV qword ptr [RSP + 0x1b0],RAX
140e23b95  4883bc24b001000000       CMP qword ptr [RSP + 0x1b0],0x0
140e23b9e  7520                     JNZ 0x140e23bc0
140e23ba0  488d8c24b8010000         LEA RCX,[RSP + 0x1b8]
140e23ba8  e8c3893f00               CALL 0x14121c570
140e23bad  90                       NOP
140e23bae  488d8c2488010000         LEA RCX,[RSP + 0x188]
140e23bb6  e8b5893f00               CALL 0x14121c570
140e23bbb  e9ae010000               JMP 0x140e23d6e
140e23bc0  660f57c0                 XORPD XMM0,XMM0
140e23bc4  f20f118424e0010000       MOVSD qword ptr [RSP + 0x1e0],XMM0
140e23bcd  660f57c0                 XORPD XMM0,XMM0
140e23bd1  f20f118424e8010000       MOVSD qword ptr [RSP + 0x1e8],XMM0
140e23bda  660f57c0                 XORPD XMM0,XMM0
140e23bde  f20f118424f0010000       MOVSD qword ptr [RSP + 0x1f0],XMM0
140e23be7  4c8d8424e0010000         LEA R8,[RSP + 0x1e0]
140e23bef  488b942448020000         MOV RDX,qword ptr [RSP + 0x248]
140e23bf7  488b8c2450020000         MOV RCX,qword ptr [RSP + 0x250]
140e23bff  e80c11b2ff               CALL 0x140944d10
140e23c04  488b8c24b0010000         MOV RCX,qword ptr [RSP + 0x1b0]
140e23c0c  4883c140                 ADD RCX,0x40
140e23c10  488d9424e0010000         LEA RDX,[RSP + 0x1e0]
140e23c18  e8a328b2ff               CALL 0x1409464c0
140e23c1d  488b8c24b0010000         MOV RCX,qword ptr [RSP + 0x1b0]
140e23c25  4883c140                 ADD RCX,0x40
140e23c29  488b942450020000         MOV RDX,qword ptr [RSP + 0x250]
140e23c31  e88a28b2ff               CALL 0x1409464c0
140e23c36  4c8b842448020000         MOV R8,qword ptr [RSP + 0x248]
140e23c3e  488b942450020000         MOV RDX,qword ptr [RSP + 0x250]
140e23c46  488d8c24e0010000         LEA RCX,[RSP + 0x1e0]
140e23c4e  e8cd12b2ff               CALL 0x140944f20
140e23c53  488b8c24b0010000         MOV RCX,qword ptr [RSP + 0x1b0]
140e23c5b  e830424200               CALL 0x141247e90
140e23c60  0fb6c0                   MOVZX EAX,AL
140e23c63  85c0                     TEST EAX,EAX
140e23c65  750d                     JNZ 0x140e23c74
140e23c67  c784241002000001000000   MOV dword ptr [RSP + 0x210],0x1
140e23c72  eb0b                     JMP 0x140e23c7f
140e23c74  c784241002000000000000   MOV dword ptr [RSP + 0x210],0x0
140e23c7f  0fb6842410020000         MOVZX EAX,byte ptr [RSP + 0x210]
140e23c87  88842468010000           MOV byte ptr [RSP + 0x168],AL
140e23c8e  488b9424b0010000         MOV RDX,qword ptr [RSP + 0x1b0]
140e23c96  4883c240                 ADD RDX,0x40
140e23c9a  488d4c2440               LEA RCX,[RSP + 0x40]
140e23c9f  e8cc21b2ff               CALL 0x140945e70
140e23ca4  488d9424e0000000         LEA RDX,[RSP + 0xe0]
140e23cac  488d4c2440               LEA RCX,[RSP + 0x40]
140e23cb1  e88a23b2ff               CALL 0x140946040
140e23cb6  488d542440               LEA RDX,[RSP + 0x40]
140e23cbb  488d8c24e0000000         LEA RCX,[RSP + 0xe0]
140e23cc3  e8a821b2ff               CALL 0x140945e70
140e23cc8  488d8424c8000000         LEA RAX,[RSP + 0xc8]
140e23cd0  4889442420               MOV qword ptr [RSP + 0x20],RAX
140e23cd5  4c8d8c2470010000         LEA R9,[RSP + 0x170]
140e23cdd  440fb6842468010000       MOVZX R8D,byte ptr [RSP + 0x168]
140e23ce6  488d9424e0000000         LEA RDX,[RSP + 0xe0]
140e23cee  488b4c2438               MOV RCX,qword ptr [RSP + 0x38]
140e23cf3  e8385a4200               CALL 0x141249730
140e23cf8  898424c0000000           MOV dword ptr [RSP + 0xc0],EAX
140e23cff  488d9424c0000000         LEA RDX,[RSP + 0xc0]
140e23d07  488b4c2438               MOV RCX,qword ptr [RSP + 0x38]
140e23d0c  e8af574200               CALL 0x1412494c0
140e23d11  4889442430               MOV qword ptr [RSP + 0x30],RAX
140e23d16  48837c243000             CMP qword ptr [RSP + 0x30],0x0
140e23d1c  741a                     JZ 0x140e23d38
140e23d1e  4c8b842450020000         MOV R8,qword ptr [RSP + 0x250]
140e23d26  488b942448020000         MOV RDX,qword ptr [RSP + 0x248]
140e23d2e  488b4c2430               MOV RCX,qword ptr [RSP + 0x30]
140e23d33  e8683e4200               CALL 0x141247ba0
140e23d38  4c8d8424c0000000         LEA R8,[RSP + 0xc0]
140e23d40  488b942440020000         MOV RDX,qword ptr [RSP + 0x240]
140e23d48  488b4c2438               MOV RCX,qword ptr [RSP + 0x38]
140e23d4d  e82e624200               CALL 0x141249f80
140e23d52  90                       NOP
140e23d53  488d8c24b8010000         LEA RCX,[RSP + 0x1b8]
140e23d5b  e810883f00               CALL 0x14121c570
140e23d60  90                       NOP
140e23d61  488d8c2488010000         LEA RCX,[RSP + 0x188]
140e23d69  e802883f00               CALL 0x14121c570
140e23d6e  4881c428020000           ADD RSP,0x228
140e23d75  c3                       RET
140e23d76  cc                       INT3
140e23d77  cc                       INT3
140e23d78  cc                       INT3
140e23d79  cc                       INT3
140e23d7a  cc                       INT3
140e23d7b  cc                       INT3
140e23d7c  cc                       INT3
140e23d7d  cc                       INT3
140e23d7e  cc                       INT3
140e23d7f  cc                       INT3
