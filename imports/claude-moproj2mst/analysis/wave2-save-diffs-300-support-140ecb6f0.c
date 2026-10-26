
void FUN_140ecb6f0(longlong param_1)

{
  char cVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 uStack_14;
  
  FUN_140eb5500(param_1);
  iVar2 = _currentRow_QListWidget__QEBAHXZ(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 8));
  if (cRam00000001446ee587 != '\0') {
    cVar1 = _isChecked_QAbstractButton__QEBA_NXZ
                      (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xa8));
    if (cVar1 != '\0') {
      uVar3 = _indexOf_QTabWidget__QEBAHPEAVQWidget___Z
                        (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xcb8),
                         *(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xeb8));
      _setTabEnabled_QTabWidget__QEAAXH_N_Z
                (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xcb8),uVar3,1);
      goto code_r0x000140ecb7ea;
    }
  }
  uVar3 = _indexOf_QTabWidget__QEBAHPEAVQWidget___Z
                    (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xcb8),
                     *(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xeb8));
  _setTabEnabled_QTabWidget__QEAAXH_N_Z
            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xcb8),uVar3,0);
code_r0x000140ecb7ea:
  if (iVar2 == 0) {
    uStack_14 = 0;
  }
  else if (iVar2 == 1) {
    uStack_14 = 8;
  }
  else if (iVar2 == 2) {
    uStack_14 = 1;
  }
  else if (iVar2 == 3) {
    uStack_14 = 2;
  }
  else if (iVar2 == 2) {
    uStack_14 = 1;
  }
  else if (iVar2 == 4) {
    uStack_14 = 3;
  }
  else if (iVar2 == 5) {
    uStack_14 = 4;
  }
  else if (iVar2 == 6) {
    uStack_14 = 5;
  }
  else if (iVar2 == 7) {
    uStack_14 = 6;
  }
  else if (iVar2 == 8) {
    uStack_14 = 7;
  }
  else if (iVar2 == 9) {
    uStack_14 = 9;
  }
  _setCurrentIndex_QStackedWidget__QEAAXH_Z
            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x10),uStack_14);
  return;
}

