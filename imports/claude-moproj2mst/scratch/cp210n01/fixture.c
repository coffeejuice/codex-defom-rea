#include <windows.h>
#include <stdlib.h>
#include <stdint.h>
int g_fail=5;
extern uintptr_t synthetic_open(void);
int main(int argc,char **argv){
 SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX|SEM_NOOPENFILEERRORBOX);
 if(argc!=2)return 73;
 g_fail=atoi(argv[1]);
 if(g_fail<0||g_fail>5)return 74;
 volatile uintptr_t value=synthetic_open();
 if((g_fail==5&&value!=0x100)||(g_fail<5&&value!=0))return 75;
 *(volatile uintptr_t*)0x58=value;
 return 76;
}
