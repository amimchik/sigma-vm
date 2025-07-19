LDI SP 3000;
LDI SBP 3000;
LDI A _START;
JMP;

TEST:
    LDI A 0x1001;
    LDI B 10;
    LDI FLG 0x11;
    MOV B SP;
    LOD;
    JMP;

_START:
    LDI A 0x1001;
    LDI B 72;
    LDI FLG 0x11;
    MOV A SP;
    LDI B 1;
    MIN;
    MOV SP C;
    MOV B SP;
    LDI A _START_RET;
    SAV;
    LDI A TEST;
    JMP;
_START_RET:
_LOOP:
    LDI A _LOOP;
    JMP;
