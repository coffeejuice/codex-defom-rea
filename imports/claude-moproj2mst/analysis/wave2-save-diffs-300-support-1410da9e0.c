
undefined1 FUN_1410da9e0(undefined8 param_1,undefined8 param_2,undefined8 param_3)

{
  char cVar1;
  int iVar2;
  undefined8 uVar3;
  undefined1 auStack_48 [16];
  undefined1 auStack_38 [16];
  int iStack_28;
  undefined1 auStack_24 [4];
  undefined1 *puStack_20;
  undefined1 uStack_18;
  undefined1 uStack_17;
  undefined8 uStack_10;
  
  uStack_10 = 0xfffffffffffffffe;
  __0QFile__QEAA_AEBVQString___Z(auStack_48,param_2);
  puStack_20 = auStack_24;
  uVar3 = func_0x000140004a60(puStack_20,2);
  cVar1 = _open_QFile__UEAA_NV__QFlags_W4OpenModeFlag_QIODevice_____Z(auStack_48,uVar3);
  if (cVar1 == '\0') {
    uStack_17 = 0;
    __1QFile__UEAA_XZ(auStack_48);
    uStack_18 = uStack_17;
  }
  else {
    __0QTextStream__QEAA_PEAVQIODevice___Z(auStack_38,auStack_48);
    iStack_28 = 0;
    while( true ) {
      iVar2 = FUN_140004ae0(param_3);
      if (iVar2 <= iStack_28) break;
      uVar3 = FUN_14014b5b0(param_3,iStack_28);
      __6QTextStream__QEAAAEAV0_AEBVQString___Z(auStack_38,uVar3);
      uVar3 = FUN_14014b5b0(param_3,iStack_28);
      cVar1 = __9QString__QEBA_NPEBD_Z(uVar3,&UNK_142c143b0);
      if (cVar1 != '\0') {
        __6QTextStream__QEAAAEAV0_PEBD_Z(auStack_38,&UNK_142c143b4);
      }
      iStack_28 = iStack_28 + 1;
    }
    __6QTextStream__QEAAAEAV0_PEBD_Z(auStack_38,&UNK_142c143b8);
    _close_QFile__UEAAXXZ(auStack_48);
    uStack_18 = 1;
    __1QTextStream__UEAA_XZ(auStack_38);
    __1QFile__UEAA_XZ(auStack_48);
  }
  return uStack_18;
}

