
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_140ec3130(longlong param_1,longlong param_2)

{
  char cVar1;
  byte bVar2;
  int iVar3;
  undefined4 uVar4;
  undefined8 uVar5;
  undefined4 uStack_1c4;
  undefined1 auStack_1c0 [24];
  undefined4 uStack_1a8;
  undefined1 auStack_1a0 [40];
  undefined4 auStack_178 [2];
  undefined1 auStack_170 [8];
  undefined4 uStack_168;
  undefined1 auStack_160 [8];
  undefined1 auStack_158 [8];
  undefined1 auStack_150 [8];
  undefined1 auStack_148 [8];
  undefined1 auStack_140 [8];
  undefined1 auStack_138 [8];
  undefined1 auStack_130 [8];
  undefined1 auStack_128 [8];
  undefined1 auStack_120 [8];
  undefined1 auStack_118 [8];
  undefined1 auStack_110 [8];
  undefined1 auStack_108 [8];
  undefined1 auStack_100 [8];
  undefined1 auStack_f8 [8];
  undefined8 uStack_f0;
  undefined8 uStack_e8;
  undefined8 uStack_e0;
  undefined8 uStack_d8;
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
  undefined8 uStack_10;
  
  uStack_f0 = 0xfffffffffffffffe;
  auStack_178[0] = 0;
  uStack_168 = 0;
  uStack_1a8 = 0;
  __0QString__QEAA_XZ(auStack_170);
  __0QDoubleValidator__QEAA_PEAVQObject___Z(auStack_1a0,param_1);
  __0QIntValidator__QEAA_PEAVQObject___Z(auStack_1c0,param_1);
  cVar1 = FUN_140130ea0(param_1);
  if (cVar1 == '\0') {
    *(undefined4 *)(param_2 + 0x2e8) = 0;
    cVar1 = _isChecked_QAbstractButton__QEBA_NXZ
                      (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xa90));
    if (cVar1 == '\0') {
      cVar1 = _isChecked_QAbstractButton__QEBA_NXZ
                        (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xaf0));
      if (cVar1 == '\0') {
        cVar1 = _isChecked_QAbstractButton__QEBA_NXZ
                          (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xae8));
        if (cVar1 == '\0') {
          cVar1 = _isChecked_QAbstractButton__QEBA_NXZ
                            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xaf8));
          if (cVar1 == '\0') {
            cVar1 = _isChecked_QAbstractButton__QEBA_NXZ
                              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xb00));
            if (cVar1 == '\0') {
              uStack_168 = 0;
            }
            else {
              uStack_168 = 6;
              *(undefined4 *)(param_2 + 0x2e8) = 4;
              uStack_88 = _text_QLineEdit__QEBA_AVQString__XZ
                                    (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xb18),
                                     auStack_130);
              uStack_80 = uStack_88;
              uVar5 = _toDouble_QString__QEBANPEA_N_Z(uStack_88,0);
              *(undefined8 *)(param_2 + 0x2f0) = uVar5;
              __1QString__QEAA_XZ(auStack_130);
              uStack_78 = _text_QLineEdit__QEBA_AVQString__XZ
                                    (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xb28),
                                     auStack_128);
              uStack_70 = uStack_78;
              uVar5 = _toDouble_QString__QEBANPEA_N_Z(uStack_78,0);
              *(undefined8 *)(param_2 + 0x2f8) = uVar5;
              __1QString__QEAA_XZ(auStack_128);
              uStack_68 = _text_QLineEdit__QEBA_AVQString__XZ
                                    (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xb38),
                                     auStack_120);
              uStack_60 = uStack_68;
              uVar5 = _toDouble_QString__QEBANPEA_N_Z(uStack_68,0);
              *(undefined8 *)(param_2 + 0x300) = uVar5;
              __1QString__QEAA_XZ(auStack_120);
              uStack_58 = _text_QLineEdit__QEBA_AVQString__XZ
                                    (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xb48),
                                     auStack_118);
              uStack_50 = uStack_58;
              uVar5 = _toDouble_QString__QEBANPEA_N_Z(uStack_58,0);
              *(undefined8 *)(param_2 + 0x308) = uVar5;
              __1QString__QEAA_XZ(auStack_118);
            }
          }
          else {
            uStack_168 = 5;
          }
        }
        else {
          uStack_168 = 4;
        }
      }
      else {
        uStack_168 = 1;
      }
    }
    else {
      uStack_168 = 2;
      *(undefined4 *)(param_2 + 0x2e8) = 4;
      iVar3 = _currentIndex_QComboBox__QEBAHXZ
                        (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xaa8));
      if (iVar3 == 0) {
        *(undefined8 *)(param_2 + 0x2f0) = 0;
      }
      else {
        iVar3 = _currentIndex_QComboBox__QEBAHXZ
                          (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xaa8));
        if (iVar3 == 1) {
          *(undefined8 *)(param_2 + 0x2f0) = _DAT_1421249e0;
        }
        else {
          *(undefined8 *)(param_2 + 0x2f0) = _UNK_142101520;
        }
      }
      uStack_a8 = _text_QLineEdit__QEBA_AVQString__XZ
                            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xab8),auStack_140);
      uStack_a0 = uStack_a8;
      uVar5 = _toDouble_QString__QEBANPEA_N_Z(uStack_a8,0);
      *(undefined8 *)(param_2 + 0x2f8) = uVar5;
      __1QString__QEAA_XZ(auStack_140);
      uStack_98 = _text_QLineEdit__QEBA_AVQString__XZ
                            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xac8),auStack_138);
      uStack_90 = uStack_98;
      uVar5 = _toDouble_QString__QEBANPEA_N_Z(uStack_98,0);
      *(undefined8 *)(param_2 + 0x300) = uVar5;
      __1QString__QEAA_XZ(auStack_138);
      iVar3 = _currentIndex_QComboBox__QEBAHXZ
                        (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xad8));
      if (iVar3 == 0) {
        *(undefined8 *)(param_2 + 0x308) = _DAT_1421249e0;
      }
      else {
        *(undefined8 *)(param_2 + 0x308) = _DAT_1421d3c60;
      }
    }
    cVar1 = _isChecked_QAbstractButton__QEBA_NXZ
                      (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xb60));
    if (cVar1 == '\0') {
      cVar1 = _isChecked_QAbstractButton__QEBA_NXZ
                        (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xb68));
      if (cVar1 == '\0') {
        uStack_1a8 = 0;
      }
      else {
        uStack_1a8 = 1;
      }
    }
    else {
      uStack_1a8 = 2;
    }
    cVar1 = _isChecked_QAbstractButton__QEBA_NXZ
                      (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xba8));
    if (cVar1 == '\0') {
      cVar1 = _isChecked_QAbstractButton__QEBA_NXZ
                        (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xbb0));
      if (cVar1 == '\0') {
        cVar1 = _isChecked_QAbstractButton__QEBA_NXZ
                          (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 3000));
        if (cVar1 == '\0') {
          cVar1 = _isChecked_QAbstractButton__QEBA_NXZ
                            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xbc0));
          if (cVar1 == '\0') {
            cVar1 = _isChecked_QAbstractButton__QEBA_NXZ
                              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xbc8));
            if (cVar1 == '\0') {
              uStack_1c4 = 0;
            }
            else {
              uStack_1c4 = 5;
            }
          }
          else {
            uStack_1c4 = 4;
          }
        }
        else {
          uStack_1c4 = 3;
        }
      }
      else {
        uStack_1c4 = 2;
      }
    }
    else {
      uStack_1c4 = 1;
    }
    *(undefined4 *)(param_2 + 0x2e4) = uStack_168;
    *(undefined4 *)(param_2 + 0x2e0) = uStack_1a8;
    *(undefined4 *)(param_2 + 0x318) = uStack_1c4;
  }
  else {
    *(undefined4 *)(param_2 + 0x2e8) = 0;
    cVar1 = _isChecked_QAbstractButton__QEBA_NXZ
                      (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xae0));
    if (cVar1 == '\0') {
      cVar1 = _isChecked_QAbstractButton__QEBA_NXZ
                        (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xaf0));
      if (cVar1 == '\0') {
        cVar1 = _isChecked_QAbstractButton__QEBA_NXZ
                          (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xaf8));
        if (cVar1 == '\0') {
          cVar1 = _isChecked_QAbstractButton__QEBA_NXZ
                            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xb00));
          if (cVar1 == '\0') {
            uStack_168 = 0;
          }
          else {
            uStack_168 = 6;
            *(undefined4 *)(param_2 + 0x2e8) = 4;
            uStack_e8 = _text_QLineEdit__QEBA_AVQString__XZ
                                  (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xb18),
                                   auStack_160);
            uStack_e0 = uStack_e8;
            uVar5 = _toDouble_QString__QEBANPEA_N_Z(uStack_e8,0);
            *(undefined8 *)(param_2 + 0x2f0) = uVar5;
            __1QString__QEAA_XZ(auStack_160);
            uStack_d8 = _text_QLineEdit__QEBA_AVQString__XZ
                                  (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xb28),
                                   auStack_158);
            uStack_d0 = uStack_d8;
            uVar5 = _toDouble_QString__QEBANPEA_N_Z(uStack_d8,0);
            *(undefined8 *)(param_2 + 0x2f8) = uVar5;
            __1QString__QEAA_XZ(auStack_158);
            uStack_c8 = _text_QLineEdit__QEBA_AVQString__XZ
                                  (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xb38),
                                   auStack_150);
            uStack_c0 = uStack_c8;
            uVar5 = _toDouble_QString__QEBANPEA_N_Z(uStack_c8,0);
            *(undefined8 *)(param_2 + 0x300) = uVar5;
            __1QString__QEAA_XZ(auStack_150);
            uStack_b8 = _text_QLineEdit__QEBA_AVQString__XZ
                                  (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xb48),
                                   auStack_148);
            uStack_b0 = uStack_b8;
            uVar5 = _toDouble_QString__QEBANPEA_N_Z(uStack_b8,0);
            *(undefined8 *)(param_2 + 0x308) = uVar5;
            __1QString__QEAA_XZ(auStack_148);
          }
        }
        else {
          uStack_168 = 5;
        }
      }
      else {
        uStack_168 = 3;
      }
    }
    else {
      uStack_168 = 1;
    }
    cVar1 = _isChecked_QAbstractButton__QEBA_NXZ
                      (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xb60));
    if (cVar1 == '\0') {
      cVar1 = _isChecked_QAbstractButton__QEBA_NXZ
                        (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xb68));
      if (cVar1 == '\0') {
        cVar1 = _isChecked_QAbstractButton__QEBA_NXZ
                          (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xb70));
        if (cVar1 == '\0') {
          uStack_1a8 = 0;
        }
        else {
          uStack_1a8 = 3;
        }
      }
      else {
        uStack_1a8 = 1;
      }
    }
    else {
      uStack_1a8 = 2;
    }
    cVar1 = _isChecked_QAbstractButton__QEBA_NXZ
                      (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xba8));
    if (cVar1 == '\0') {
      cVar1 = _isChecked_QAbstractButton__QEBA_NXZ
                        (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 3000));
      if (cVar1 == '\0') {
        cVar1 = _isChecked_QAbstractButton__QEBA_NXZ
                          (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xbc0));
        if (cVar1 == '\0') {
          cVar1 = _isChecked_QAbstractButton__QEBA_NXZ
                            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xbc8));
          if (cVar1 == '\0') {
            uStack_1c4 = 0;
          }
          else {
            uStack_1c4 = 5;
          }
        }
        else {
          uStack_1c4 = 4;
        }
      }
      else {
        uStack_1c4 = 1;
      }
    }
    else {
      uStack_1c4 = 3;
    }
    *(undefined4 *)(param_2 + 0x2e4) = uStack_168;
    *(undefined4 *)(param_2 + 0x2e0) = uStack_1a8;
    *(undefined4 *)(param_2 + 0x318) = uStack_1c4;
  }
  uStack_48 = _text_QLineEdit__QEBA_AVQString__XZ
                        (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xc08),auStack_110);
  uStack_40 = uStack_48;
  __4QString__QEAAAEAV0_AEBV0__Z(auStack_170,uStack_48);
  __1QString__QEAA_XZ(auStack_110);
  iVar3 = _validate_QDoubleValidator__UEBA_AW4State_QValidator__AEAVQString__AEAH_Z
                    (auStack_1a0,auStack_170,auStack_178);
  if (iVar3 == 0) {
    __1QIntValidator__UEAA_XZ(auStack_1c0);
    __1QDoubleValidator__UEAA_XZ(auStack_1a0);
    __1QString__QEAA_XZ(auStack_170);
  }
  else {
    uVar5 = _toDouble_QString__QEBANPEA_N_Z(auStack_170,0);
    *(undefined8 *)(param_2 + 0x2d0) = uVar5;
    uStack_38 = _text_QLineEdit__QEBA_AVQString__XZ
                          (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xc10),auStack_108);
    uStack_30 = uStack_38;
    __4QString__QEAAAEAV0_AEBV0__Z(auStack_170,uStack_38);
    __1QString__QEAA_XZ(auStack_108);
    iVar3 = _validate_QDoubleValidator__UEBA_AW4State_QValidator__AEAVQString__AEAH_Z
                      (auStack_1a0,auStack_170,auStack_178);
    if (iVar3 == 0) {
      __1QIntValidator__UEAA_XZ(auStack_1c0);
      __1QDoubleValidator__UEAA_XZ(auStack_1a0);
      __1QString__QEAA_XZ(auStack_170);
    }
    else {
      uVar5 = _toDouble_QString__QEBANPEA_N_Z(auStack_170,0);
      *(undefined8 *)(param_2 + 0x2d8) = uVar5;
      bVar2 = _isChecked_QAbstractButton__QEBA_NXZ
                        (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xc38));
      *(uint *)(param_2 + 0x314) = (uint)bVar2;
      bVar2 = _isChecked_QAbstractButton__QEBA_NXZ
                        (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xc78));
      *(uint *)(param_2 + 800) = (uint)bVar2;
      uStack_28 = _text_QLineEdit__QEBA_AVQString__XZ
                            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xc60),auStack_100);
      uStack_20 = uStack_28;
      __4QString__QEAAAEAV0_AEBV0__Z(auStack_170,uStack_28);
      __1QString__QEAA_XZ(auStack_100);
      iVar3 = _validate_QIntValidator__UEBA_AW4State_QValidator__AEAVQString__AEAH_Z
                        (auStack_1c0,auStack_170,auStack_178);
      if (iVar3 == 0) {
        *(undefined4 *)(param_2 + 0x310) = 200;
      }
      else {
        uVar4 = _toInt_QString__QEBAHPEA_NH_Z(auStack_170,0,10);
        *(undefined4 *)(param_2 + 0x310) = uVar4;
      }
      uStack_18 = _text_QLineEdit__QEBA_AVQString__XZ
                            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xca0),auStack_f8);
      uStack_10 = uStack_18;
      __4QString__QEAAAEAV0_AEBV0__Z(auStack_170,uStack_18);
      __1QString__QEAA_XZ(auStack_f8);
      iVar3 = _validate_QIntValidator__UEBA_AW4State_QValidator__AEAVQString__AEAH_Z
                        (auStack_1c0,auStack_170,auStack_178);
      if (iVar3 == 0) {
        *(undefined4 *)(param_2 + 0x31c) = 200;
      }
      else {
        uVar4 = _toInt_QString__QEBAHPEA_NH_Z(auStack_170,0,10);
        *(undefined4 *)(param_2 + 0x31c) = uVar4;
      }
      __1QIntValidator__UEAA_XZ(auStack_1c0);
      __1QDoubleValidator__UEAA_XZ(auStack_1a0);
      __1QString__QEAA_XZ(auStack_170);
    }
  }
  return;
}

