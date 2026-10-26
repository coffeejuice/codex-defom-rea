
undefined1 FUN_1410da510(undefined8 param_1,undefined8 param_2,longlong param_3,int param_4)

{
  char cVar1;
  undefined8 uVar2;
  undefined1 auStack_58 [16];
  int iStack_48;
  undefined1 auStack_40 [16];
  undefined1 auStack_30 [8];
  undefined1 *puStack_28;
  undefined1 uStack_20;
  undefined1 uStack_1f;
  undefined8 uStack_18;
  longlong lStack_10;
  
  uStack_18 = 0xfffffffffffffffe;
  for (iStack_48 = 0; iStack_48 < param_4; iStack_48 = iStack_48 + 1) {
    *(undefined8 *)(param_3 + (longlong)iStack_48 * 8) = 0;
  }
  __0QFile__QEAA_AEBVQString___Z(auStack_58,param_2);
  cVar1 = _exists_QFile__QEBA_NXZ(auStack_58);
  if (cVar1 != '\0') {
    puStack_28 = auStack_30;
    uVar2 = func_0x000140004a60(puStack_28,1);
    cVar1 = _open_QFile__UEAA_NV__QFlags_W4OpenModeFlag_QIODevice_____Z(auStack_58,uVar2);
    if (cVar1 != '\0') {
      __0QTextStream__QEAA_PEAVQIODevice___Z(auStack_40,auStack_58);
      iStack_48 = 0;
      while ((cVar1 = _atEnd_QTextStream__QEBA_NXZ(auStack_40), cVar1 == '\0' &&
             (iStack_48 < param_4))) {
        lStack_10 = param_3 + (longlong)iStack_48 * 8;
        __5QTextStream__QEAAAEAV0_AEAN_Z(auStack_40,lStack_10);
        iStack_48 = iStack_48 + 1;
      }
      _close_QFile__UEAAXXZ(auStack_58);
      uStack_20 = 1;
      __1QTextStream__UEAA_XZ(auStack_40);
      __1QFile__UEAA_XZ(auStack_58);
      return uStack_20;
    }
  }
  uStack_1f = 0;
  __1QFile__UEAA_XZ(auStack_58);
  return uStack_1f;
}

