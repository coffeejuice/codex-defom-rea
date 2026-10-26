
void FUN_140ebc1f0(longlong param_1,longlong param_2)

{
  char cVar1;
  int iVar2;
  undefined8 uVar3;
  int iStack_208;
  int iStack_204;
  undefined4 auStack_200 [2];
  undefined1 auStack_1f8 [40];
  undefined4 auStack_1d0 [2];
  undefined1 auStack_1c8 [8];
  char cStack_1c0;
  undefined4 uStack_1bc;
  undefined1 auStack_1b8 [8];
  undefined1 auStack_1b0 [8];
  undefined1 auStack_1a8 [8];
  undefined1 auStack_1a0 [8];
  undefined1 auStack_198 [8];
  undefined1 auStack_190 [8];
  undefined1 auStack_188 [8];
  undefined1 auStack_180 [8];
  undefined1 auStack_178 [8];
  undefined1 auStack_170 [8];
  undefined1 auStack_168 [8];
  undefined1 auStack_160 [8];
  undefined1 auStack_158 [8];
  undefined1 auStack_150 [8];
  undefined1 auStack_148 [8];
  undefined1 auStack_140 [8];
  undefined1 auStack_138 [8];
  undefined8 uStack_130;
  undefined8 uStack_128;
  undefined8 uStack_120;
  undefined8 uStack_118;
  undefined8 uStack_110;
  undefined8 uStack_108;
  undefined8 uStack_100;
  undefined8 uStack_f8;
  undefined8 uStack_f0;
  undefined8 uStack_e8;
  undefined8 uStack_e0;
  uint uStack_d8;
  undefined8 uStack_d0;
  undefined8 uStack_c8;
  undefined8 uStack_c0;
  undefined8 uStack_b8;
  undefined8 uStack_b0;
  undefined8 uStack_a8;
  undefined8 uStack_a0;
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
  
  uStack_130 = 0xfffffffffffffffe;
  auStack_1d0[0] = 0;
  iStack_208 = 0;
  __0QString__QEAA_XZ(auStack_1c8);
  __0QDoubleValidator__QEAA_PEAVQObject___Z(auStack_1f8,param_1);
  cVar1 = _isChecked_QAbstractButton__QEBA_NXZ
                    (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x2f0));
  if (cVar1 == '\0') {
    cVar1 = _isChecked_QAbstractButton__QEBA_NXZ
                      (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x2f8));
    if (cVar1 == '\0') {
      cVar1 = _isChecked_QAbstractButton__QEBA_NXZ
                        (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x300));
      if (cVar1 != '\0') {
        iStack_208 = 2;
      }
    }
    else {
      iStack_208 = 1;
    }
  }
  else {
    iStack_208 = 0;
  }
  if (iStack_208 == 2) {
    uStack_1bc = 1;
    func_0x0001408da6f0(param_2,&uStack_1bc);
  }
  else {
    func_0x0001408da6f0(param_2,&iStack_208);
  }
  iVar2 = func_0x0001408da6d0(param_2);
  if (iVar2 == 1) {
    uStack_128 = _text_QLineEdit__QEBA_AVQString__XZ
                           (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x248),auStack_1b8);
    uStack_120 = uStack_128;
    __4QString__QEAAAEAV0_AEBV0__Z(auStack_1c8,uStack_128);
    __1QString__QEAA_XZ(auStack_1b8);
    iStack_204 = _validate_QDoubleValidator__UEBA_AW4State_QValidator__AEAVQString__AEAH_Z
                           (auStack_1f8,auStack_1c8,auStack_1d0);
    if (iStack_204 == 0) {
      __1QDoubleValidator__UEAA_XZ(auStack_1f8);
      __1QString__QEAA_XZ(auStack_1c8);
      return;
    }
    uVar3 = _toDouble_QString__QEBANPEA_N_Z(auStack_1c8,0);
    *(undefined8 *)(param_2 + 0x1d0) = uVar3;
    func_0x0001405fd4b0(param_2,0);
  }
  else {
    uStack_118 = _text_QLineEdit__QEBA_AVQString__XZ
                           (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x200),auStack_1b0);
    uStack_110 = uStack_118;
    __4QString__QEAAAEAV0_AEBV0__Z(auStack_1c8,uStack_118);
    __1QString__QEAA_XZ(auStack_1b0);
    iStack_204 = _validate_QDoubleValidator__UEBA_AW4State_QValidator__AEAVQString__AEAH_Z
                           (auStack_1f8,auStack_1c8,auStack_1d0);
    if (iStack_204 == 0) {
      __1QDoubleValidator__UEAA_XZ(auStack_1f8);
      __1QString__QEAA_XZ(auStack_1c8);
      return;
    }
    uVar3 = _toDouble_QString__QEBANPEA_N_Z(auStack_1c8,0);
    func_0x0001405fd4b0(param_2,uVar3);
    *(undefined8 *)(param_2 + 0x1d0) = 0;
  }
  cVar1 = _isChecked_QAbstractButton__QEBA_NXZ
                    (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x318));
  if (cVar1 == '\0') {
    cVar1 = _isChecked_QAbstractButton__QEBA_NXZ
                      (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 800));
    if (cVar1 != '\0') {
      *(undefined4 *)(param_2 + 0x1c8) = 2;
    }
  }
  else {
    *(undefined4 *)(param_2 + 0x1c8) = 1;
  }
  cVar1 = _isChecked_QAbstractButton__QEBA_NXZ
                    (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x300));
  if (cVar1 != '\0') {
    *(undefined4 *)(param_2 + 0x1c8) = 3;
  }
  uStack_108 = _text_QLineEdit__QEBA_AVQString__XZ
                         (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x348),auStack_1a8);
  uStack_100 = uStack_108;
  __4QString__QEAAAEAV0_AEBV0__Z(auStack_1c8,uStack_108);
  __1QString__QEAA_XZ(auStack_1a8);
  iStack_204 = _validate_QDoubleValidator__UEBA_AW4State_QValidator__AEAVQString__AEAH_Z
                         (auStack_1f8,auStack_1c8,auStack_1d0);
  if (iStack_204 == 0) {
    __1QDoubleValidator__UEAA_XZ(auStack_1f8);
    __1QString__QEAA_XZ(auStack_1c8);
    return;
  }
  uVar3 = _toDouble_QString__QEBANPEA_N_Z(auStack_1c8,0);
  *(undefined8 *)(param_2 + 0x260) = uVar3;
  cVar1 = FUN_140130ee0(param_1);
  if (cVar1 != '\0') {
    uStack_f8 = _text_QLineEdit__QEBA_AVQString__XZ
                          (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x3c0),auStack_1a0);
    uStack_f0 = uStack_f8;
    __4QString__QEAAAEAV0_AEBV0__Z(auStack_1c8,uStack_f8);
    __1QString__QEAA_XZ(auStack_1a0);
    iStack_204 = _validate_QDoubleValidator__UEBA_AW4State_QValidator__AEAVQString__AEAH_Z
                           (auStack_1f8,auStack_1c8,auStack_1d0);
    if (iStack_204 == 0) {
      __1QDoubleValidator__UEAA_XZ(auStack_1f8);
      __1QString__QEAA_XZ(auStack_1c8);
      return;
    }
    uVar3 = _toDouble_QString__QEBANPEA_N_Z(auStack_1c8,0);
    *(undefined8 *)(param_2 + 0x2c8) = uVar3;
  }
  cVar1 = FUN_140130ee0(param_1);
  if (cVar1 == '\0') {
    cStack_1c0 = _isChecked_QAbstractButton__QEBA_NXZ
                           (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x370));
    uStack_d8 = (uint)(cStack_1c0 == '\0');
    *(double *)(param_2 + 0x2c0) = (double)uStack_d8;
  }
  else {
    uStack_e8 = _text_QLineEdit__QEBA_AVQString__XZ
                          (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x3f8),auStack_198);
    uStack_e0 = uStack_e8;
    __4QString__QEAAAEAV0_AEBV0__Z(auStack_1c8,uStack_e8);
    __1QString__QEAA_XZ(auStack_198);
    iStack_204 = _validate_QDoubleValidator__UEBA_AW4State_QValidator__AEAVQString__AEAH_Z
                           (auStack_1f8,auStack_1c8,auStack_1d0);
    if (iStack_204 == 0) {
      __1QDoubleValidator__UEAA_XZ(auStack_1f8);
      __1QString__QEAA_XZ(auStack_1c8);
      return;
    }
    uVar3 = _toDouble_QString__QEBANPEA_N_Z(auStack_1c8,0);
    *(undefined8 *)(param_2 + 0x2c0) = uVar3;
  }
  cVar1 = FUN_140130ea0(param_1);
  if (cVar1 != '\0') {
    uStack_d0 = _text_QLineEdit__QEBA_AVQString__XZ
                          (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x4f8),auStack_190);
    uStack_c8 = uStack_d0;
    __4QString__QEAAAEAV0_AEBV0__Z(auStack_1c8,uStack_d0);
    __1QString__QEAA_XZ(auStack_190);
    iStack_204 = _validate_QDoubleValidator__UEBA_AW4State_QValidator__AEAVQString__AEAH_Z
                           (auStack_1f8,auStack_1c8,auStack_1d0);
    if (iStack_204 == 0) {
      __1QDoubleValidator__UEAA_XZ(auStack_1f8);
      __1QString__QEAA_XZ(auStack_1c8);
      return;
    }
    uVar3 = _toDouble_QString__QEBANPEA_N_Z(auStack_1c8,0);
    *(undefined8 *)(param_2 + 0x278) = uVar3;
    uStack_c0 = _text_QLineEdit__QEBA_AVQString__XZ
                          (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x500),auStack_188);
    uStack_b8 = uStack_c0;
    __4QString__QEAAAEAV0_AEBV0__Z(auStack_1c8,uStack_c0);
    __1QString__QEAA_XZ(auStack_188);
    iStack_204 = _validate_QDoubleValidator__UEBA_AW4State_QValidator__AEAVQString__AEAH_Z
                           (auStack_1f8,auStack_1c8,auStack_1d0);
    if (iStack_204 == 0) {
      __1QDoubleValidator__UEAA_XZ(auStack_1f8);
      __1QString__QEAA_XZ(auStack_1c8);
      return;
    }
    uVar3 = _toDouble_QString__QEBANPEA_N_Z(auStack_1c8,0);
    *(undefined8 *)(param_2 + 0x280) = uVar3;
  }
  cVar1 = _isChecked_QAbstractButton__QEBA_NXZ
                    (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x2f0));
  if (cVar1 == '\0') {
    cVar1 = _isChecked_QAbstractButton__QEBA_NXZ
                      (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x2f8));
    if (cVar1 == '\0') {
      cVar1 = _isChecked_QAbstractButton__QEBA_NXZ
                        (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x300));
      if (cVar1 != '\0') {
        iStack_208 = 2;
      }
    }
    else {
      iStack_208 = 1;
    }
  }
  else {
    iStack_208 = 0;
  }
  if (iStack_208 == 2) {
    uStack_b0 = _text_QLineEdit__QEBA_AVQString__XZ
                          (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x288),auStack_180);
    uStack_a8 = uStack_b0;
    __4QString__QEAAAEAV0_AEBV0__Z(auStack_1c8,uStack_b0);
    __1QString__QEAA_XZ(auStack_180);
    iStack_204 = _validate_QDoubleValidator__UEBA_AW4State_QValidator__AEAVQString__AEAH_Z
                           (auStack_1f8,auStack_1c8,auStack_1d0);
    if (iStack_204 == 0) {
      __1QDoubleValidator__UEAA_XZ(auStack_1f8);
      __1QString__QEAA_XZ(auStack_1c8);
      return;
    }
    uVar3 = _toDouble_QString__QEBANPEA_N_Z(auStack_1c8,0);
    *(undefined8 *)(param_2 + 0x1d0) = uVar3;
    uStack_a0 = _text_QLineEdit__QEBA_AVQString__XZ
                          (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x2a0),auStack_178);
    uStack_98 = uStack_a0;
    __4QString__QEAAAEAV0_AEBV0__Z(auStack_1c8,uStack_a0);
    __1QString__QEAA_XZ(auStack_178);
    iStack_204 = _validate_QDoubleValidator__UEBA_AW4State_QValidator__AEAVQString__AEAH_Z
                           (auStack_1f8,auStack_1c8,auStack_1d0);
    if (iStack_204 == 0) {
      __1QDoubleValidator__UEAA_XZ(auStack_1f8);
      __1QString__QEAA_XZ(auStack_1c8);
      return;
    }
    uVar3 = _toDouble_QString__QEBANPEA_N_Z(auStack_1c8,0);
    *(undefined8 *)(param_2 + 0x270) = uVar3;
    uStack_90 = _text_QLineEdit__QEBA_AVQString__XZ
                          (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x2c0),auStack_170);
    uStack_88 = uStack_90;
    __4QString__QEAAAEAV0_AEBV0__Z(auStack_1c8,uStack_90);
    __1QString__QEAA_XZ(auStack_170);
    iStack_204 = _validate_QDoubleValidator__UEBA_AW4State_QValidator__AEAVQString__AEAH_Z
                           (auStack_1f8,auStack_1c8,auStack_1d0);
    if (iStack_204 == 0) {
      __1QDoubleValidator__UEAA_XZ(auStack_1f8);
      __1QString__QEAA_XZ(auStack_1c8);
      return;
    }
    uVar3 = _toDouble_QString__QEBANPEA_N_Z(auStack_1c8,0);
    *(undefined8 *)(param_2 + 0x2a0) = uVar3;
    uStack_80 = _text_QLineEdit__QEBA_AVQString__XZ
                          (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x2c8),auStack_168);
    uStack_78 = uStack_80;
    __4QString__QEAAAEAV0_AEBV0__Z(auStack_1c8,uStack_80);
    __1QString__QEAA_XZ(auStack_168);
    iStack_204 = _validate_QDoubleValidator__UEBA_AW4State_QValidator__AEAVQString__AEAH_Z
                           (auStack_1f8,auStack_1c8,auStack_1d0);
    if (iStack_204 == 0) {
      __1QDoubleValidator__UEAA_XZ(auStack_1f8);
      __1QString__QEAA_XZ(auStack_1c8);
      return;
    }
    uVar3 = _toDouble_QString__QEBANPEA_N_Z(auStack_1c8,0);
    *(undefined8 *)(param_2 + 0x2a8) = uVar3;
  }
  else {
    uStack_70 = _text_QLineEdit__QEBA_AVQString__XZ
                          (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x450),auStack_160);
    uStack_68 = uStack_70;
    __4QString__QEAAAEAV0_AEBV0__Z(auStack_1c8,uStack_70);
    __1QString__QEAA_XZ(auStack_160);
    iStack_204 = _validate_QDoubleValidator__UEBA_AW4State_QValidator__AEAVQString__AEAH_Z
                           (auStack_1f8,auStack_1c8,auStack_1d0);
    if (iStack_204 == 0) {
      __1QDoubleValidator__UEAA_XZ(auStack_1f8);
      __1QString__QEAA_XZ(auStack_1c8);
      return;
    }
    uVar3 = _toDouble_QString__QEBANPEA_N_Z(auStack_1c8,0);
    *(undefined8 *)(param_2 + 0x270) = uVar3;
    uStack_60 = _text_QLineEdit__QEBA_AVQString__XZ
                          (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x430),auStack_158);
    uStack_58 = uStack_60;
    __4QString__QEAAAEAV0_AEBV0__Z(auStack_1c8,uStack_60);
    __1QString__QEAA_XZ(auStack_158);
    iStack_204 = _validate_QDoubleValidator__UEBA_AW4State_QValidator__AEAVQString__AEAH_Z
                           (auStack_1f8,auStack_1c8,auStack_1d0);
    if (iStack_204 == 0) {
      __1QDoubleValidator__UEAA_XZ(auStack_1f8);
      __1QString__QEAA_XZ(auStack_1c8);
      return;
    }
    uVar3 = _toDouble_QString__QEBANPEA_N_Z(auStack_1c8,0);
    *(undefined8 *)(param_2 + 0x2a0) = uVar3;
    uStack_50 = _text_QLineEdit__QEBA_AVQString__XZ
                          (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x438),auStack_150);
    uStack_48 = uStack_50;
    __4QString__QEAAAEAV0_AEBV0__Z(auStack_1c8,uStack_50);
    __1QString__QEAA_XZ(auStack_150);
    iStack_204 = _validate_QDoubleValidator__UEBA_AW4State_QValidator__AEAVQString__AEAH_Z
                           (auStack_1f8,auStack_1c8,auStack_1d0);
    if (iStack_204 == 0) {
      __1QDoubleValidator__UEAA_XZ(auStack_1f8);
      __1QString__QEAA_XZ(auStack_1c8);
      return;
    }
    uVar3 = _toDouble_QString__QEBANPEA_N_Z(auStack_1c8,0);
    *(undefined8 *)(param_2 + 0x2a8) = uVar3;
  }
  uStack_40 = _text_QLineEdit__QEBA_AVQString__XZ
                        (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x4c8),auStack_148);
  uStack_38 = uStack_40;
  __4QString__QEAAAEAV0_AEBV0__Z(auStack_1c8,uStack_40);
  __1QString__QEAA_XZ(auStack_148);
  iStack_204 = _validate_QDoubleValidator__UEBA_AW4State_QValidator__AEAVQString__AEAH_Z
                         (auStack_1f8,auStack_1c8,auStack_1d0);
  if (iStack_204 == 0) {
    __1QDoubleValidator__UEAA_XZ(auStack_1f8);
    __1QString__QEAA_XZ(auStack_1c8);
  }
  else {
    uVar3 = _toDouble_QString__QEBANPEA_N_Z(auStack_1c8,0);
    *(undefined8 *)(param_2 + 0x268) = uVar3;
    cVar1 = FUN_140130ea0(param_1);
    if (cVar1 != '\0') {
      uStack_30 = _text_QLineEdit__QEBA_AVQString__XZ
                            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x490),auStack_140);
      uStack_28 = uStack_30;
      __4QString__QEAAAEAV0_AEBV0__Z(auStack_1c8,uStack_30);
      __1QString__QEAA_XZ(auStack_140);
      iStack_204 = _validate_QDoubleValidator__UEBA_AW4State_QValidator__AEAVQString__AEAH_Z
                             (auStack_1f8,auStack_1c8,auStack_1d0);
      if (iStack_204 != 0) {
        uVar3 = _toDouble_QString__QEBANPEA_N_Z(auStack_1c8,0);
        *(undefined8 *)(param_2 + 0x2b0) = uVar3;
      }
      uStack_20 = _text_QLineEdit__QEBA_AVQString__XZ
                            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x480),auStack_138);
      uStack_18 = uStack_20;
      __4QString__QEAAAEAV0_AEBV0__Z(auStack_1c8,uStack_20);
      __1QString__QEAA_XZ(auStack_138);
      iStack_204 = _validate_QDoubleValidator__UEBA_AW4State_QValidator__AEAVQString__AEAH_Z
                             (auStack_1f8,auStack_1c8,auStack_1d0);
      if (iStack_204 != 0) {
        uVar3 = _toDouble_QString__QEBANPEA_N_Z(auStack_1c8,0);
        *(undefined8 *)(param_2 + 0x2b8) = uVar3;
      }
    }
    auStack_200[0] = func_0x0001408da6d0(param_2);
    FUN_14089ad60(param_2 + 0x1e8,*(undefined8 *)(param_1 + 0xe8));
    func_0x0001408da6f0(param_2,auStack_200);
    __1QDoubleValidator__UEAA_XZ(auStack_1f8);
    __1QString__QEAA_XZ(auStack_1c8);
  }
  return;
}

