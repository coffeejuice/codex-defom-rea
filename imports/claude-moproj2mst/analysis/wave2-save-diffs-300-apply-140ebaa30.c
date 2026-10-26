
void FUN_140ebaa30(longlong param_1,longlong param_2)

{
  bool bVar1;
  char cVar2;
  undefined4 uVar3;
  int iVar4;
  undefined8 uVar5;
  int iStack_98;
  undefined1 auStack_88 [8];
  longlong lStack_80;
  undefined8 uStack_78;
  undefined1 auStack_70 [8];
  undefined1 auStack_68 [8];
  undefined1 auStack_60 [8];
  undefined1 auStack_58 [8];
  undefined8 uStack_50;
  undefined8 uStack_48;
  undefined8 uStack_40;
  undefined8 uStack_38;
  undefined8 uStack_30;
  undefined8 uStack_28;
  undefined8 uStack_20;
  undefined4 uStack_18;
  
  uStack_50 = 0xfffffffffffffffe;
  __0QString__QEAA_XZ(auStack_70);
  uStack_48 = _text_QLineEdit__QEBA_AVQString__XZ
                        (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x128),auStack_68);
  uStack_40 = uStack_48;
  __4QString__QEAAAEAV0_AEBV0__Z(auStack_70,uStack_48);
  __1QString__QEAA_XZ(auStack_68);
  _toLocal8Bit_QString__QEBA_AVQByteArray__XZ(auStack_70,auStack_88);
  uVar5 = _data_QByteArray__QEAAPEADXZ(auStack_88);
  func_0x00014146fec0(param_2,uVar5);
  uStack_38 = _text_QLineEdit__QEBA_AVQString__XZ
                        (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x138),auStack_60);
  uStack_30 = uStack_38;
  __4QString__QEAAAEAV0_AEBV0__Z(auStack_70,uStack_38);
  __1QString__QEAA_XZ(auStack_60);
  uStack_28 = _toLocal8Bit_QString__QEBA_AVQByteArray__XZ(auStack_70,auStack_58);
  uStack_20 = uStack_28;
  __4QByteArray__QEAAAEAV0_AEBV0__Z(auStack_88,uStack_28);
  __1QByteArray__QEAA_XZ(auStack_58);
  uVar5 = _data_QByteArray__QEAAPEADXZ(auStack_88);
  func_0x00014146fec0(param_2 + 0x51,uVar5);
  cVar2 = _isVisible_QWidget__QEBA_NXZ(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x158));
  if (cVar2 != '\0') {
    cVar2 = _isEnabled_QWidget__QEBA_NXZ(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x158));
    if (cVar2 != '\0') {
      uVar3 = _value_QSpinBox__QEBAHXZ(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x158));
      *(undefined4 *)(param_2 + 0xf0) = uVar3;
    }
  }
  cVar2 = _isChecked_QAbstractButton__QEBA_NXZ
                    (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x50));
  if (cVar2 == '\0') {
    cVar2 = _isChecked_QAbstractButton__QEBA_NXZ
                      (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x58));
    if (cVar2 != '\0') {
      *(undefined4 *)(param_2 + 0xfc) = 1;
    }
  }
  else {
    *(undefined4 *)(param_2 + 0xfc) = 2;
  }
  FUN_140917d60(*(undefined4 *)(param_2 + 0xfc));
  cVar2 = FUN_140130ea0(param_1);
  if (cVar2 != '\0') {
    cVar2 = _isChecked_QAbstractButton__QEBA_NXZ
                      (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x178));
    if (cVar2 == '\0') {
      cVar2 = _isChecked_QAbstractButton__QEBA_NXZ
                        (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x188));
      if (cVar2 == '\0') {
        cVar2 = _isChecked_QAbstractButton__QEBA_NXZ
                          (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x180));
        if (cVar2 == '\0') {
          cVar2 = _isChecked_QAbstractButton__QEBA_NXZ
                            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 400));
          if (cVar2 != '\0') {
            *(undefined4 *)(param_2 + 0x100) = 4;
          }
        }
        else {
          *(undefined4 *)(param_2 + 0x100) = 3;
        }
      }
      else {
        *(undefined4 *)(param_2 + 0x100) = 2;
      }
    }
    else {
      *(undefined4 *)(param_2 + 0x100) = 1;
    }
  }
  cVar2 = _isChecked_QAbstractButton__QEBA_NXZ
                    (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe8));
  if (cVar2 == '\0') {
    cVar2 = _isChecked_QAbstractButton__QEBA_NXZ
                      (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xf0));
    if (cVar2 == '\0') {
      cVar2 = _isChecked_QAbstractButton__QEBA_NXZ
                        (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xf8));
      if (cVar2 == '\0') {
        cVar2 = _isChecked_QAbstractButton__QEBA_NXZ
                          (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe0));
        if (cVar2 == '\0') {
          cVar2 = _isChecked_QAbstractButton__QEBA_NXZ
                            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x100));
          if (cVar2 == '\0') {
            cVar2 = _isChecked_QAbstractButton__QEBA_NXZ
                              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x108));
            if (cVar2 != '\0') {
              *(undefined4 *)(param_2 + 0xf8) = 7;
            }
          }
          else {
            *(undefined4 *)(param_2 + 0xf8) = 6;
          }
        }
        else {
          *(undefined4 *)(param_2 + 0xf8) = 5;
        }
      }
      else {
        *(undefined4 *)(param_2 + 0xf8) = 4;
      }
    }
    else {
      *(undefined4 *)(param_2 + 0xf8) = 3;
    }
  }
  else {
    *(undefined4 *)(param_2 + 0xf8) = 1;
  }
  bVar1 = false;
  uVar5 = func_0x0001402c8090(0x143d766b8);
  uStack_78 = FUN_14146f8fc(uVar5,0,0x143569280,0x143569258,0);
  uVar5 = FUN_1400133a0(uStack_78);
  uVar5 = FUN_140045780(uVar5);
  lStack_80 = FUN_1400a7390(uVar5);
  if (lStack_80 != 0) {
    cVar2 = FUN_140010330(lStack_80);
    if (cVar2 == '\0') {
      cVar2 = FUN_140010360(lStack_80);
      if (cVar2 == '\0') goto code_r0x000140ebaf66;
    }
    bVar1 = true;
  }
code_r0x000140ebaf66:
  if (!bVar1) {
    for (iStack_98 = 0; iStack_98 < 6; iStack_98 = iStack_98 + 1) {
      cVar2 = _isChecked_QAbstractButton__QEBA_NXZ
                        (*(undefined8 *)(param_1 + 0x50 + (longlong)iStack_98 * 8));
      if (cVar2 == '\0') {
code_r0x000140ebafda:
        uStack_18 = 0;
      }
      else {
        cVar2 = _isEnabled_QWidget__QEBA_NXZ
                          (*(undefined8 *)(param_1 + 0x50 + (longlong)iStack_98 * 8));
        if (cVar2 == '\0') goto code_r0x000140ebafda;
        uStack_18 = 1;
      }
      *(undefined4 *)(param_2 + 0x104 + (longlong)iStack_98 * 4) = uStack_18;
    }
  }
  if (*(int *)(param_2 + 0x10c) != 0) {
    cVar2 = _isChecked_QAbstractButton__QEBA_NXZ
                      (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x10d8));
    if (cVar2 != '\0') {
      *(undefined4 *)(param_2 + 0x10c) = 2;
    }
  }
  if (*(int *)(param_2 + 0x118) != 0) {
    cVar2 = FUN_140130ea0(param_1);
    if (cVar2 == '\0') {
      iVar4 = _currentIndex_QComboBox__QEBAHXZ
                        (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xb8));
      *(int *)(param_2 + 0x118) = iVar4 + 1;
    }
    else {
      iVar4 = _currentIndex_QComboBox__QEBAHXZ
                        (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xb8));
      *(int *)(param_2 + 0x118) = iVar4 + 1;
    }
  }
  FUN_1408e00d0(param_2);
  __1QByteArray__QEAA_XZ(auStack_88);
  __1QString__QEAA_XZ(auStack_70);
  return;
}

