
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_140eceb60(longlong param_1,int param_2)

{
  double *pdVar1;
  char cVar2;
  int iVar3;
  undefined8 uVar4;
  ulonglong uVar5;
  longlong lStack_108;
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
  
  uStack_a8 = 0xfffffffffffffffe;
  __0QString__QEAA_XZ(auStack_f8);
  if (param_2 < 0) {
    __1QString__QEAA_XZ(auStack_f8);
  }
  else if (*(longlong *)(param_1 + 0xf8) == 0) {
    __1QString__QEAA_XZ(auStack_f8);
  }
  else {
    if (*(int *)(param_1 + 0x100) != -1) {
      lStack_108 = 0;
      cVar2 = _isChecked_QAbstractButton__QEBA_NXZ
                        (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x978));
      if (cVar2 == '\0') {
        cVar2 = _isChecked_QAbstractButton__QEBA_NXZ
                          (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x988));
        if (cVar2 != '\0') {
          lStack_108 = 0;
        }
      }
      else {
        lStack_108 = *(longlong *)(*(longlong *)(param_1 + 0x140) + 0x978);
      }
      cVar2 = FUN_140130ea0(param_1);
      if (cVar2 == '\0') {
        if (lStack_108 == 0) {
          uStack_90 = _text_QLineEdit__QEBA_AVQString__XZ
                                (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x970),auStack_e8)
          ;
          uStack_88 = uStack_90;
          _toDouble_QString__QEBANPEA_N_Z(uStack_90,0);
          uVar4 = FUN_141470a78();
          *(undefined8 *)
           (*(longlong *)(param_1 + 0xf8) + 0xb8 + (longlong)*(int *)(param_1 + 0x100) * 0x158) =
               uVar4;
          __1QString__QEAA_XZ(auStack_e8);
        }
        else {
          uStack_80 = _text_QLineEdit__QEBA_AVQString__XZ
                                (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x970),auStack_e0)
          ;
          uStack_78 = uStack_80;
          _toDouble_QString__QEBANPEA_N_Z(uStack_80,0);
          uVar5 = FUN_141470a78();
          *(ulonglong *)
           (*(longlong *)(param_1 + 0xf8) + 0xb8 + (longlong)*(int *)(param_1 + 0x100) * 0x158) =
               uVar5 ^ _UNK_1421d3d30;
          __1QString__QEAA_XZ(auStack_e0);
        }
      }
      else {
        uStack_a0 = _text_QLineEdit__QEBA_AVQString__XZ
                              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x950),auStack_f0);
        uStack_98 = uStack_a0;
        uVar4 = _toDouble_QString__QEBANPEA_N_Z(uStack_a0,0);
        *(undefined8 *)
         (*(longlong *)(param_1 + 0xf8) + 0xb8 + (longlong)*(int *)(param_1 + 0x100) * 0x158) =
             uVar4;
        __1QString__QEAA_XZ(auStack_f0);
      }
      uStack_70 = _text_QLineEdit__QEBA_AVQString__XZ
                            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x920),auStack_d8);
      uStack_68 = uStack_70;
      uVar4 = _toDouble_QString__QEBANPEA_N_Z(uStack_70,0);
      *(undefined8 *)
       (*(longlong *)(param_1 + 0xf8) + 0xc0 + (longlong)*(int *)(param_1 + 0x100) * 0x158) = uVar4;
      __1QString__QEAA_XZ(auStack_d8);
      uStack_60 = _text_QLineEdit__QEBA_AVQString__XZ
                            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x918),auStack_d0);
      uStack_58 = uStack_60;
      _toDouble_QString__QEBANPEA_N_Z(uStack_60,0);
      uVar4 = FUN_141470a78();
      *(undefined8 *)
       (*(longlong *)(param_1 + 0xf8) + 0xd0 + (longlong)*(int *)(param_1 + 0x100) * 0x158) = uVar4;
      __1QString__QEAA_XZ(auStack_d0);
      iVar3 = _value_QSpinBox__QEBAHXZ(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x938));
      *(double *)(*(longlong *)(param_1 + 0xf8) + 200 + (longlong)*(int *)(param_1 + 0x100) * 0x158)
           = (double)iVar3;
    }
    pdVar1 = (double *)(*(longlong *)(param_1 + 0xf8) + 0xb8 + (longlong)param_2 * 0x158);
    if (0.0 < *pdVar1 || *pdVar1 == 0.0) {
      _setChecked_QAbstractButton__QEAAX_N_Z
                (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x978),0);
      _setChecked_QAbstractButton__QEAAX_N_Z
                (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x988),1);
    }
    else {
      _setChecked_QAbstractButton__QEAAX_N_Z
                (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x978),1);
      _setChecked_QAbstractButton__QEAAX_N_Z
                (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x988),0);
    }
    cVar2 = FUN_140130ea0(param_1);
    if (cVar2 == '\0') {
      uVar4 = FUN_141470a78(*(undefined8 *)
                             (*(longlong *)(param_1 + 0xf8) + 0xb8 + (longlong)param_2 * 0x158));
      uStack_40 = _number_QString__SA_AV1_NDH_Z(auStack_c0,uVar4,0x67,6);
      uStack_38 = uStack_40;
      _setText_QLineEdit__QEAAXAEBVQString___Z
                (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x970),uStack_40);
      __1QString__QEAA_XZ(auStack_c0);
    }
    else {
      uStack_50 = _number_QString__SA_AV1_NDH_Z
                            (auStack_c8,
                             *(undefined8 *)
                              (*(longlong *)(param_1 + 0xf8) + 0xb8 + (longlong)param_2 * 0x158),
                             0x67,6);
      uStack_48 = uStack_50;
      _setText_QLineEdit__QEAAXAEBVQString___Z
                (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x950),uStack_50);
      __1QString__QEAA_XZ(auStack_c8);
    }
    uStack_30 = _number_QString__SA_AV1_NDH_Z
                          (auStack_b8,
                           *(undefined8 *)
                            (*(longlong *)(param_1 + 0xf8) + 0xc0 + (longlong)param_2 * 0x158),0x67,
                           6);
    uStack_28 = uStack_30;
    _setText_QLineEdit__QEAAXAEBVQString___Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x920),uStack_30);
    __1QString__QEAA_XZ(auStack_b8);
    uStack_20 = _number_QString__SA_AV1_NDH_Z
                          (auStack_b0,
                           *(undefined8 *)
                            (*(longlong *)(param_1 + 0xf8) + 0xd0 + (longlong)param_2 * 0x158),0x67,
                           6);
    uStack_18 = uStack_20;
    _setText_QLineEdit__QEAAXAEBVQString___Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x918),uStack_20);
    __1QString__QEAA_XZ(auStack_b0);
    _setValue_QSpinBox__QEAAXH_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x938),
               (int)*(double *)(*(longlong *)(param_1 + 0xf8) + 200 + (longlong)param_2 * 0x158));
    *(int *)(param_1 + 0x100) = param_2;
    __1QString__QEAA_XZ(auStack_f8);
  }
  return;
}

