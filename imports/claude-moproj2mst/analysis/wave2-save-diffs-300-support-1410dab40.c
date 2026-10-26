
char FUN_1410dab40(undefined8 param_1,longlong *param_2,int *param_3)

{
  undefined8 uVar1;
  longlong lVar2;
  undefined1 auStack_108 [16];
  int iStack_f8;
  undefined1 auStack_f0 [24];
  undefined8 uStack_d8;
  undefined8 uStack_d0;
  undefined8 uStack_c8;
  longlong lStack_88;
  int aiStack_80 [10];
  char cStack_58;
  undefined1 auStack_50 [8];
  undefined1 auStack_48 [8];
  undefined1 *puStack_40;
  char cStack_38;
  undefined1 auStack_30 [8];
  char cStack_28;
  undefined8 uStack_20;
  uint uStack_18;
  
  uStack_20 = 0xfffffffffffffffe;
  lStack_88 = 0;
  __0QString__QEAA_PEBD_Z(auStack_50,&UNK_142c143c0);
  __0QFile__QEAA_AEBVQString___Z(auStack_108,auStack_50);
  __1QString__QEAA_XZ(auStack_50);
  cStack_58 = _exists_QFile__QEBA_NXZ(auStack_108);
  if (cStack_58 != '\0') {
    puStack_40 = auStack_48;
    uVar1 = func_0x000140004a60(puStack_40,1);
    cStack_58 = _open_QFile__UEAA_NV__QFlags_W4OpenModeFlag_QIODevice_____Z(auStack_108,uVar1);
  }
  if (cStack_58 == '\0') {
    cStack_38 = '\0';
    __1QFile__UEAA_XZ(auStack_108);
  }
  else {
    __0QTextStream__QEAA_PEAVQIODevice___Z(auStack_f0,auStack_108);
    cStack_58 = FUN_1410da800(param_1,aiStack_80,1,auStack_f0);
    if (cStack_58 != '\0') {
      *param_3 = aiStack_80[0];
      cStack_58 = 0 < *param_3;
      uStack_18 = (uint)(byte)cStack_58;
    }
    if (cStack_58 != '\0') {
      lStack_88 = FUN_14147489c((longlong)*param_3 << 2);
      lVar2 = FUN_1410db4f0(*param_2,*param_3);
      *param_2 = lVar2;
      if ((lStack_88 == 0) || (*param_2 == 0)) {
        cStack_58 = '\0';
      }
      else {
        __0QString__QEAA_PEBD_Z(auStack_30,&UNK_142c143d0);
        cStack_58 = FUN_1410da690(param_1,auStack_30,lStack_88,*param_3);
        __1QString__QEAA_XZ(auStack_30);
        if (cStack_58 == '\0') {
          for (iStack_f8 = 0; iStack_f8 < *param_3; iStack_f8 = iStack_f8 + 1) {
            *(undefined4 *)(lStack_88 + (longlong)iStack_f8 * 4) = 0;
          }
          cStack_58 = '\x01';
        }
      }
    }
    if (cStack_58 != '\0') {
      for (iStack_f8 = 0; iStack_f8 < *param_3; iStack_f8 = iStack_f8 + 1) {
        cStack_58 = FUN_1410da800(param_1,aiStack_80,1,auStack_f0);
        if (cStack_58 == '\0') {
          cStack_58 = '\0';
          break;
        }
        *(int *)((longlong)iStack_f8 * 0x40 + *param_2) = aiStack_80[0];
        cStack_58 = FUN_1410da8f0(param_1,&uStack_d8,2,auStack_f0);
        if (cStack_58 == '\0') {
          cStack_58 = '\0';
          break;
        }
        *(undefined8 *)(*param_2 + 8 + (longlong)iStack_f8 * 0x40) = uStack_d8;
        *(undefined8 *)(*param_2 + 0x10 + (longlong)iStack_f8 * 0x40) = uStack_d0;
        cStack_58 = FUN_1410da800(param_1,aiStack_80,1,auStack_f0);
        if (cStack_58 == '\0') {
          cStack_58 = '\0';
          break;
        }
        *(int *)(*param_2 + 0x18 + (longlong)iStack_f8 * 0x40) = aiStack_80[0];
        cStack_58 = FUN_1410da8f0(param_1,&uStack_d8,3,auStack_f0);
        if (cStack_58 == '\0') break;
        *(undefined8 *)(*param_2 + 0x20 + (longlong)iStack_f8 * 0x40) = uStack_d8;
        *(undefined8 *)(*param_2 + 0x28 + (longlong)iStack_f8 * 0x40) = uStack_d0;
        *(undefined8 *)(*param_2 + 0x30 + (longlong)iStack_f8 * 0x40) = uStack_c8;
        *(undefined4 *)(*param_2 + 0x38 + (longlong)iStack_f8 * 0x40) =
             *(undefined4 *)(lStack_88 + (longlong)iStack_f8 * 4);
      }
    }
    _close_QFile__UEAAXXZ(auStack_108);
    cStack_28 = cStack_58;
    __1QTextStream__UEAA_XZ(auStack_f0);
    __1QFile__UEAA_XZ(auStack_108);
    cStack_38 = cStack_28;
  }
  return cStack_38;
}

