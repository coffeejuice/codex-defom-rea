
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_140ec7500(longlong *param_1,longlong param_2)

{
  bool bVar1;
  bool bVar2;
  char cVar3;
  byte bVar4;
  int iVar5;
  undefined4 uVar6;
  double dVar7;
  undefined8 uVar8;
  uint uStack_144;
  int iStack_140;
  undefined1 auStack_128 [40];
  undefined4 auStack_100 [2];
  undefined1 auStack_f8 [8];
  undefined1 auStack_f0 [8];
  undefined1 auStack_e8 [8];
  undefined1 auStack_e0 [8];
  undefined1 auStack_d8 [8];
  undefined1 auStack_d0 [8];
  undefined1 auStack_c8 [8];
  undefined1 auStack_c0 [8];
  undefined1 auStack_b8 [8];
  undefined1 auStack_b0 [8];
  undefined1 auStack_a8 [8];
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
  
  uStack_a0 = 0xfffffffffffffffe;
  auStack_100[0] = 0;
  __0QString__QEAA_XZ(auStack_f8);
  __0QDoubleValidator__QEAA_PEAVQObject___Z(auStack_128,param_1);
  uStack_98 = _text_QLineEdit__QEBA_AVQString__XZ
                        (*(undefined8 *)(param_1[0x28] + 0x1070),auStack_e8);
  uStack_90 = uStack_98;
  __4QString__QEAAAEAV0_AEBV0__Z(auStack_f8,uStack_98);
  __1QString__QEAA_XZ(auStack_e8);
  iVar5 = _validate_QDoubleValidator__UEBA_AW4State_QValidator__AEAVQString__AEAH_Z
                    (auStack_128,auStack_f8,auStack_100);
  if (iVar5 == 0) {
    __1QDoubleValidator__UEAA_XZ(auStack_128);
    __1QString__QEAA_XZ(auStack_f8);
  }
  else {
    dVar7 = (double)_toDouble_QString__QEBANPEA_N_Z(auStack_f8,0);
    dVar7 = (double)FUN_141470a78(dVar7 - *(double *)(param_2 + 0x640));
    bVar1 = _UNK_142157420 < dVar7;
    uVar8 = _toDouble_QString__QEBANPEA_N_Z(auStack_f8,0);
    *(undefined8 *)(param_2 + 0x640) = uVar8;
    uStack_88 = _text_QLineEdit__QEBA_AVQString__XZ
                          (*(undefined8 *)(param_1[0x28] + 0x1068),auStack_e0);
    uStack_80 = uStack_88;
    __4QString__QEAAAEAV0_AEBV0__Z(auStack_f8,uStack_88);
    __1QString__QEAA_XZ(auStack_e0);
    iVar5 = _validate_QDoubleValidator__UEBA_AW4State_QValidator__AEAVQString__AEAH_Z
                      (auStack_128,auStack_f8,auStack_100);
    if (iVar5 == 0) {
      __1QDoubleValidator__UEAA_XZ(auStack_128);
      __1QString__QEAA_XZ(auStack_f8);
    }
    else {
      dVar7 = (double)_toDouble_QString__QEBANPEA_N_Z(auStack_f8,0);
      dVar7 = (double)FUN_141470a78(dVar7 - *(double *)(param_2 + 0x648));
      bVar2 = _UNK_142157420 < dVar7;
      uVar8 = _toDouble_QString__QEBANPEA_N_Z(auStack_f8,0);
      *(undefined8 *)(param_2 + 0x648) = uVar8;
      cVar3 = _isChecked_QAbstractButton__QEBA_NXZ(*(undefined8 *)(param_1[0x28] + 0x1030));
      if (cVar3 == '\0') {
        cVar3 = _isChecked_QAbstractButton__QEBA_NXZ(*(undefined8 *)(param_1[0x28] + 0x1038));
        if (cVar3 != '\0') {
          *(undefined4 *)(param_2 + 0x650) = 1;
        }
      }
      else {
        *(undefined4 *)(param_2 + 0x650) = 0;
      }
      uVar8 = (**(code **)(*param_1 + 0x198))(param_1);
      uVar8 = FUN_140c7acd0(uVar8);
      FUN_1408bfe60(uVar8);
      if (bVar2 || bVar1) {
        uVar8 = (**(code **)(*param_1 + 0x198))(param_1);
        uVar8 = FUN_140c7acd0(uVar8);
        FUN_1408bfe60(uVar8);
      }
      uStack_78 = _text_QLineEdit__QEBA_AVQString__XZ
                            (*(undefined8 *)(param_1[0x28] + 0x1110),auStack_d8);
      uStack_70 = uStack_78;
      __4QString__QEAAAEAV0_AEBV0__Z(auStack_f8,uStack_78);
      __1QString__QEAA_XZ(auStack_d8);
      iVar5 = _validate_QDoubleValidator__UEBA_AW4State_QValidator__AEAVQString__AEAH_Z
                        (auStack_128,auStack_f8,auStack_100);
      if (iVar5 == 0) {
        __1QDoubleValidator__UEAA_XZ(auStack_128);
        __1QString__QEAA_XZ(auStack_f8);
      }
      else {
        dVar7 = (double)_toDouble_QString__QEBANPEA_N_Z(auStack_f8,0);
        *(double *)(param_2 + 0x660) = dVar7 / _UNK_1421de200;
        uStack_68 = _text_QLineEdit__QEBA_AVQString__XZ
                              (*(undefined8 *)(param_1[0x28] + 0x1128),auStack_d0);
        uStack_60 = uStack_68;
        __4QString__QEAAAEAV0_AEBV0__Z(auStack_f8,uStack_68);
        __1QString__QEAA_XZ(auStack_d0);
        iVar5 = _validate_QDoubleValidator__UEBA_AW4State_QValidator__AEAVQString__AEAH_Z
                          (auStack_128,auStack_f8,auStack_100);
        if (iVar5 == 0) {
          __1QDoubleValidator__UEAA_XZ(auStack_128);
          __1QString__QEAA_XZ(auStack_f8);
        }
        else {
          dVar7 = (double)_toDouble_QString__QEBANPEA_N_Z(auStack_f8,0);
          *(double *)(param_2 + 0x668) = dVar7 / _UNK_1421de200;
          uStack_58 = _text_QLineEdit__QEBA_AVQString__XZ
                                (*(undefined8 *)(param_1[0x28] + 0x1140),auStack_c8);
          uStack_50 = uStack_58;
          __4QString__QEAAAEAV0_AEBV0__Z(auStack_f8,uStack_58);
          __1QString__QEAA_XZ(auStack_c8);
          iVar5 = _validate_QDoubleValidator__UEBA_AW4State_QValidator__AEAVQString__AEAH_Z
                            (auStack_128,auStack_f8,auStack_100);
          if (iVar5 == 0) {
            __1QDoubleValidator__UEAA_XZ(auStack_128);
            __1QString__QEAA_XZ(auStack_f8);
          }
          else {
            dVar7 = (double)_toDouble_QString__QEBANPEA_N_Z(auStack_f8,0);
            *(double *)(param_2 + 0x670) = dVar7 / _UNK_1421de200;
            uStack_48 = _text_QLineEdit__QEBA_AVQString__XZ
                                  (*(undefined8 *)(param_1[0x28] + 0x1158),auStack_c0);
            uStack_40 = uStack_48;
            __4QString__QEAAAEAV0_AEBV0__Z(auStack_f8,uStack_48);
            __1QString__QEAA_XZ(auStack_c0);
            iVar5 = _validate_QDoubleValidator__UEBA_AW4State_QValidator__AEAVQString__AEAH_Z
                              (auStack_128,auStack_f8,auStack_100);
            if (iVar5 == 0) {
              __1QDoubleValidator__UEAA_XZ(auStack_128);
              __1QString__QEAA_XZ(auStack_f8);
            }
            else {
              dVar7 = (double)_toDouble_QString__QEBANPEA_N_Z(auStack_f8,0);
              *(double *)(param_2 + 0x678) = dVar7 / _UNK_1421de200;
              for (iStack_140 = 0; iStack_140 < 10; iStack_140 = iStack_140 + 1) {
                uVar8 = _item_QTableWidget__QEBAPEAVQTableWidgetItem__HH_Z
                                  (*(undefined8 *)(param_1[0x28] + 0x11c0),iStack_140,0);
                uStack_38 = _text_QTableWidgetItem__QEBA_AVQString__XZ(uVar8,auStack_b8);
                uStack_30 = uStack_38;
                __4QString__QEAAAEAV0_AEBV0__Z(auStack_f8,uStack_38);
                __1QString__QEAA_XZ(auStack_b8);
                cVar3 = _isEmpty_QString__QEBA_NXZ(auStack_f8);
                if (cVar3 == '\0') {
                  _toLocal8Bit_QString__QEBA_AVQByteArray__XZ(auStack_f8,auStack_f0);
                  uVar8 = _data_QByteArray__QEAAPEADXZ(auStack_f0);
                  func_0x00014146fec0(param_2 + 0x688 + (longlong)iStack_140 * 0x51,uVar8);
                  __1QByteArray__QEAA_XZ(auStack_f0);
                }
                else {
                  func_0x00014146fec0(param_2 + 0x688 + (longlong)iStack_140 * 0x51,&UNK_142b0ffd7);
                }
              }
              cVar3 = FUN_140130ea0(param_1);
              if (cVar3 != '\0') {
                cVar3 = _isChecked_QAbstractButton__QEBA_NXZ
                                  (*(undefined8 *)(param_1[0x28] + 0x1180));
                uStack_144 = (uint)(cVar3 != '\0');
                *(uint *)(param_2 + 0x9c8) = uStack_144;
                *(undefined8 *)(param_2 + 0x9d0) = 0;
                if (uStack_144 != 0) {
                  uStack_28 = _text_QLineEdit__QEBA_AVQString__XZ
                                        (*(undefined8 *)(param_1[0x28] + 0x1190),auStack_b0);
                  uStack_20 = uStack_28;
                  __4QString__QEAAAEAV0_AEBV0__Z(auStack_f8,uStack_28);
                  __1QString__QEAA_XZ(auStack_b0);
                  iVar5 = _validate_QDoubleValidator__UEBA_AW4State_QValidator__AEAVQString__AEAH_Z
                                    (auStack_128,auStack_f8,auStack_100);
                  if (iVar5 != 0) {
                    uVar8 = _toDouble_QString__QEBANPEA_N_Z(auStack_f8,0);
                    *(undefined8 *)(param_2 + 0x9d0) = uVar8;
                  }
                }
              }
              for (iStack_140 = 0; iStack_140 < 7; iStack_140 = iStack_140 + 1) {
                bVar4 = _isChecked_QAbstractButton__QEBA_NXZ(param_1[(longlong)iStack_140 + 0x10]);
                *(uint *)(param_2 + 0x11c + (longlong)iStack_140 * 4) = (uint)bVar4;
              }
              cVar3 = _isChecked_QAbstractButton__QEBA_NXZ(*(undefined8 *)(param_1[0x28] + 0x12d8));
              if (cVar3 == '\0') {
                cVar3 = _isChecked_QAbstractButton__QEBA_NXZ
                                  (*(undefined8 *)(param_1[0x28] + 0x12e0));
                if (cVar3 == '\0') {
                  cVar3 = _isChecked_QAbstractButton__QEBA_NXZ
                                    (*(undefined8 *)(param_1[0x28] + 0x12e8));
                  if (cVar3 != '\0') {
                    *(undefined4 *)(param_2 + 0x138) = 3;
                  }
                }
                else {
                  *(undefined4 *)(param_2 + 0x138) = 2;
                }
              }
              else {
                *(undefined4 *)(param_2 + 0x138) = 0;
              }
              cVar3 = _isChecked_QAbstractButton__QEBA_NXZ(*(undefined8 *)(param_1[0x28] + 0x1300));
              if (cVar3 == '\0') {
                cVar3 = _isChecked_QAbstractButton__QEBA_NXZ
                                  (*(undefined8 *)(param_1[0x28] + 0x1308));
                if (cVar3 == '\0') {
                  cVar3 = _isChecked_QAbstractButton__QEBA_NXZ
                                    (*(undefined8 *)(param_1[0x28] + 0x1310));
                  if (cVar3 != '\0') {
                    *(undefined4 *)(param_2 + 0x13c) = 3;
                  }
                }
                else {
                  *(undefined4 *)(param_2 + 0x13c) = 2;
                }
              }
              else {
                *(undefined4 *)(param_2 + 0x13c) = 0;
              }
              cVar3 = _isChecked_QAbstractButton__QEBA_NXZ(*(undefined8 *)(param_1[0x28] + 0x1328));
              if (cVar3 == '\0') {
                cVar3 = _isChecked_QAbstractButton__QEBA_NXZ
                                  (*(undefined8 *)(param_1[0x28] + 0x1330));
                if (cVar3 == '\0') {
                  cVar3 = _isChecked_QAbstractButton__QEBA_NXZ
                                    (*(undefined8 *)(param_1[0x28] + 0x1338));
                  if (cVar3 != '\0') {
                    *(undefined4 *)(param_2 + 0x140) = 3;
                  }
                }
                else {
                  *(undefined4 *)(param_2 + 0x140) = 2;
                }
              }
              else {
                *(undefined4 *)(param_2 + 0x140) = 0;
              }
              uStack_18 = _currentText_QComboBox__QEBA_AVQString__XZ
                                    (*(undefined8 *)(param_1[0x28] + 0x10c0),auStack_a8);
              uStack_10 = uStack_18;
              uVar6 = FUN_140ec9980(param_1,uStack_18);
              *(undefined4 *)(param_2 + 0x9b4) = uVar6;
              __1QString__QEAA_XZ(auStack_a8);
              cVar3 = FUN_140130ea0(param_1);
              if (cVar3 != '\0') {
                uVar6 = _value_QSpinBox__QEBAHXZ(*(undefined8 *)(param_1[0x28] + 0x1200));
                *(undefined4 *)(param_2 + 0x9b8) = uVar6;
                uVar6 = _value_QSpinBox__QEBAHXZ(*(undefined8 *)(param_1[0x28] + 0x11f8));
                *(undefined4 *)(param_2 + 0x9bc) = uVar6;
                cVar3 = _isChecked_QAbstractButton__QEBA_NXZ
                                  (*(undefined8 *)(param_1[0x28] + 0x1220));
                if (cVar3 == '\0') {
                  *(undefined4 *)(param_2 + 0x9c0) = 1;
                }
                else {
                  *(undefined4 *)(param_2 + 0x9c0) = 0;
                }
                uVar6 = _value_QSpinBox__QEBAHXZ(*(undefined8 *)(param_1[0x28] + 0x1238));
                *(undefined4 *)(param_2 + 0x9c4) = uVar6;
              }
              uVar8 = (**(code **)(*param_1 + 0x198))(param_1);
              uVar8 = FUN_140c7acd0(uVar8);
              FUN_14082a360(uVar8,0);
              __1QDoubleValidator__UEAA_XZ(auStack_128);
              __1QString__QEAA_XZ(auStack_f8);
            }
          }
        }
      }
    }
  }
  return;
}

