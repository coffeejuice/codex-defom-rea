
void FUN_1411e25b0(longlong param_1)

{
  char cVar1;
  int iVar2;
  longlong lVar3;
  longlong lVar4;
  undefined8 uVar5;
  int iStack_4c;
  undefined1 auStack_48 [8];
  int iStack_40;
  undefined1 auStack_38 [8];
  int iStack_30;
  int iStack_2c;
  longlong lStack_28;
  int iStack_20;
  int iStack_1c;
  undefined8 uStack_18;
  
  uStack_18 = 0xfffffffffffffffe;
  lVar3 = FUN_140130af0(param_1);
  if ((lVar3 != 0) && (lVar4 = FUN_14005f770(lVar3), lVar4 != 0)) {
    uVar5 = func_0x000140051230(lVar3);
    lVar4 = FUN_14146f8fc(uVar5,0,0x143579b60,0x143a76500,0);
    if ((lVar4 != 0) && (lVar4 = func_0x000140025010(lVar4), lVar4 != 0)) {
      iStack_40 = func_0x00014142ff20(lVar4);
      iStack_30 = func_0x000140cbc690(lVar4);
      iStack_20 = 0;
      iStack_2c = 0;
      iStack_4c = 0;
      FUN_1400653e0(auStack_48);
      __0QString__QEAA_XZ(auStack_38);
      iVar2 = FUN_140061220(lVar3);
      __4QString__QEAAAEAV0_PEBD_Z(auStack_38,&UNK_142cd9a10);
      FUN_140004b90(auStack_48,auStack_38);
      for (iStack_1c = 0; iStack_1c < iVar2; iStack_1c = iStack_1c + 1) {
        lStack_28 = FUN_140057690(lVar3,iStack_1c);
        if ((lStack_28 != 0) && (cVar1 = FUN_140834fa0(lStack_28), cVar1 != '\0')) {
          iStack_20 = iStack_20 + 1;
          _sprintf_QString__QEAAAEAV1_PEBDZZ
                    (auStack_38,&UNK_142cd9a20,*(undefined4 *)(lStack_28 + 8),lStack_28 + 0x828);
          FUN_140004b90(auStack_48,auStack_38);
          if (*(int *)(lStack_28 + 8) == iStack_40) {
            iStack_4c = iStack_20;
          }
          else if (*(int *)(lStack_28 + 8) == iStack_30) {
            iStack_2c = iStack_20;
          }
        }
      }
      _addItems_QComboBox__QEAAXAEBVQStringList___Z
                (*(undefined8 *)(*(longlong *)(param_1 + 0x48) + 0x18),auStack_48);
      _setCurrentIndex_QComboBox__QEAAXH_Z
                (*(undefined8 *)(*(longlong *)(param_1 + 0x48) + 0x18),iStack_4c);
      _addItems_QComboBox__QEAAXAEBVQStringList___Z
                (*(undefined8 *)(*(longlong *)(param_1 + 0x48) + 0x30),auStack_48);
      _setCurrentIndex_QComboBox__QEAAXH_Z
                (*(undefined8 *)(*(longlong *)(param_1 + 0x48) + 0x30),iStack_2c);
      __1QString__QEAA_XZ(auStack_38);
      FUN_1400048d0(auStack_48);
    }
  }
  return;
}

