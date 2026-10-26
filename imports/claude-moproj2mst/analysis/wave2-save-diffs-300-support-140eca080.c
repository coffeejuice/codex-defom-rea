
void FUN_140eca080(longlong param_1)

{
  char cVar1;
  longlong lVar2;
  
  if ((cRam00000001446d49a8 == '\0') && (lVar2 = FUN_140130f90(param_1), lVar2 != 0)) {
    FUN_1402ca7a0(0x143d766b8,1);
    switch(*(undefined4 *)(param_1 + 0xb8)) {
    case 1:
      FUN_140ebaa30(param_1,lVar2);
      break;
    case 2:
      FUN_140ebb770(param_1,lVar2);
      break;
    case 3:
      FUN_140ebc1f0(param_1,lVar2);
      break;
    case 4:
      FUN_140ebf8f0(param_1,lVar2);
      break;
    case 5:
      FUN_140ec1350(param_1);
      break;
    case 6:
      FUN_140ec3130(param_1,lVar2);
      break;
    case 7:
    case 8:
    case 9:
      FUN_140ec50f0(param_1,lVar2);
      break;
    case 10:
      FUN_140ec7500(param_1,lVar2);
      break;
    default:
      FUN_140ebaa30(param_1,lVar2);
      FUN_140ebb770(param_1,lVar2);
      FUN_140ebc1f0(param_1,lVar2);
      FUN_140ebf8f0(param_1,lVar2);
      FUN_140ec1350(param_1);
      FUN_140ec3130(param_1,lVar2);
      FUN_140ec50f0(param_1,lVar2);
      FUN_140ec7500(param_1,lVar2);
      cVar1 = FUN_140130ee0(param_1);
      if (cVar1 != '\0') {
        FUN_140ec9040(param_1,lVar2);
      }
    }
    FUN_140eb52f0(param_1);
    _setFlat_QPushButton__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x9c0),0);
    FUN_140ecd2f0(param_1,0);
    FUN_140ecd430(param_1,0);
    func_0x000140ecf340(param_1);
    FUN_140ed5dd0(param_1);
    FUN_1402ca7a0(0x143d766b8,0);
    cRam00000001446d49a8 = '\0';
  }
  return;
}

