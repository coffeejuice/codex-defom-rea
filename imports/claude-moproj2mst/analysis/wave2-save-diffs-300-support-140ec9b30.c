
void FUN_140ec9b30(longlong param_1,longlong param_2)

{
  char cVar1;
  int iVar2;
  undefined8 uVar3;
  undefined1 auStack_110 [40];
  undefined4 auStack_e8 [2];
  undefined1 auStack_e0 [8];
  undefined1 auStack_d8 [8];
  undefined1 auStack_d0 [8];
  undefined1 auStack_c8 [8];
  undefined1 auStack_c0 [8];
  undefined1 auStack_b8 [8];
  undefined1 auStack_b0 [8];
  undefined1 auStack_a8 [8];
  undefined1 auStack_a0 [8];
  undefined8 uStack_98;
  undefined8 uStack_90;
  undefined8 uStack_88;
  undefined8 uStack_80;
  undefined8 uStack_78;
  undefined8 uStack_70;
  undefined8 uStack_68;
  undefined8 uStack_60;
  undefined8 uStack_58;
  undefined8 uStack_50;
  undefined8 uStack_48;
  undefined8 uStack_40;
  undefined8 uStack_38;
  undefined8 uStack_30;
  undefined8 uStack_28;
  undefined8 uStack_20;
  undefined8 uStack_18;
  
  uStack_98 = 0xfffffffffffffffe;
  auStack_e8[0] = 0;
  __0QString__QEAA_XZ(auStack_e0);
  __0QDoubleValidator__QEAA_PEAVQObject___Z(auStack_110,param_1);
  cVar1 = _isChecked_QAbstractButton__QEBA_NXZ
                    (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xcf8));
  if (cVar1 != '\0') {
    *(undefined4 *)(*(longlong *)(param_1 + 200) + 4) = 0;
    uStack_90 = _text_QLineEdit__QEBA_AVQString__XZ
                          (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xd00),auStack_d8);
    uStack_88 = uStack_90;
    uStack_80 = _simplified_QString__QEBA_AV1_XZ(uStack_90,auStack_d0);
    uStack_78 = uStack_80;
    __4QString__QEAAAEAV0_AEBV0__Z(auStack_e0,uStack_80);
    __1QString__QEAA_XZ(auStack_d0);
    __1QString__QEAA_XZ(auStack_d8);
    iVar2 = _validate_QDoubleValidator__UEBA_AW4State_QValidator__AEAVQString__AEAH_Z
                      (auStack_110,auStack_e0,auStack_e8);
    if (iVar2 != 0) {
      uVar3 = _toDouble_QString__QEBANPEA_N_Z(auStack_e0,0);
      *(undefined8 *)(*(longlong *)(param_1 + 200) + 8) = uVar3;
    }
  }
  FUN_14089ad60(param_2 + 0x3c8,*(undefined8 *)(param_1 + 200));
  cVar1 = _isChecked_QAbstractButton__QEBA_NXZ
                    (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xd58));
  if (cVar1 != '\0') {
    *(undefined4 *)(*(longlong *)(param_1 + 0xd0) + 4) = 0;
    uStack_70 = _text_QLineEdit__QEBA_AVQString__XZ
                          (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xd60),auStack_c8);
    uStack_68 = uStack_70;
    uStack_60 = _simplified_QString__QEBA_AV1_XZ(uStack_70,auStack_c0);
    uStack_58 = uStack_60;
    __4QString__QEAAAEAV0_AEBV0__Z(auStack_e0,uStack_60);
    __1QString__QEAA_XZ(auStack_c0);
    __1QString__QEAA_XZ(auStack_c8);
    iVar2 = _validate_QDoubleValidator__UEBA_AW4State_QValidator__AEAVQString__AEAH_Z
                      (auStack_110,auStack_e0,auStack_e8);
    if (iVar2 != 0) {
      uVar3 = _toDouble_QString__QEBANPEA_N_Z(auStack_e0,0);
      *(undefined8 *)(*(longlong *)(param_1 + 0xd0) + 8) = uVar3;
    }
  }
  FUN_14089ad60(param_2 + 0x4c0,*(undefined8 *)(param_1 + 0xd0));
  cVar1 = _isChecked_QAbstractButton__QEBA_NXZ
                    (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe08));
  if (cVar1 != '\0') {
    *(undefined4 *)(*(longlong *)(param_1 + 0xd8) + 4) = 0;
    uStack_50 = _text_QLineEdit__QEBA_AVQString__XZ
                          (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe18),auStack_b8);
    uStack_48 = uStack_50;
    uStack_40 = _simplified_QString__QEBA_AV1_XZ(uStack_50,auStack_b0);
    uStack_38 = uStack_40;
    __4QString__QEAAAEAV0_AEBV0__Z(auStack_e0,uStack_40);
    __1QString__QEAA_XZ(auStack_b0);
    __1QString__QEAA_XZ(auStack_b8);
    iVar2 = _validate_QDoubleValidator__UEBA_AW4State_QValidator__AEAVQString__AEAH_Z
                      (auStack_110,auStack_e0,auStack_e8);
    if (iVar2 != 0) {
      uVar3 = _toDouble_QString__QEBANPEA_N_Z(auStack_e0,0);
      *(undefined8 *)(*(longlong *)(param_1 + 0xd8) + 8) = uVar3;
    }
  }
  FUN_14089ad60(param_2 + 0x538,*(undefined8 *)(param_1 + 0xd8));
  cVar1 = _isChecked_QAbstractButton__QEBA_NXZ
                    (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe70));
  if (cVar1 != '\0') {
    *(undefined4 *)(*(longlong *)(param_1 + 0xe0) + 4) = 0;
    uStack_30 = _text_QLineEdit__QEBA_AVQString__XZ
                          (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe60),auStack_a8);
    uStack_28 = uStack_30;
    uStack_20 = _simplified_QString__QEBA_AV1_XZ(uStack_30,auStack_a0);
    uStack_18 = uStack_20;
    __4QString__QEAAAEAV0_AEBV0__Z(auStack_e0,uStack_20);
    __1QString__QEAA_XZ(auStack_a0);
    __1QString__QEAA_XZ(auStack_a8);
    iVar2 = _validate_QDoubleValidator__UEBA_AW4State_QValidator__AEAVQString__AEAH_Z
                      (auStack_110,auStack_e0,auStack_e8);
    if (iVar2 != 0) {
      uVar3 = _toDouble_QString__QEBANPEA_N_Z(auStack_e0,0);
      *(undefined8 *)(*(longlong *)(param_1 + 0xe0) + 8) = uVar3;
    }
  }
  FUN_14089ad60(param_2 + 0x5b0,*(undefined8 *)(param_1 + 0xe0));
  __1QDoubleValidator__UEAA_XZ(auStack_110);
  __1QString__QEAA_XZ(auStack_e0);
  return;
}

