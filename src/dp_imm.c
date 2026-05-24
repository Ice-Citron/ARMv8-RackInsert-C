#include "emulate.h"

void dpimm (uint32_t instr) {
    clearstate(&pState);
    uint32_t sf_dpimm;
    uint32_t opc_dpimm;
    uint32_t opi_dpimm;
    uint32_t rd_dpimm;
    sf_dpimm = bitmask_check(31, 31, instr);
    opc_dpimm = bitmask_check(30, 29, instr);
    opi_dpimm = bitmask_check(25, 23, instr);
    rd_dpimm = bitmask_check(4, 0, instr);
    if (2 == opi_dpimm) {
        uint32_t rn_arith;
        rn_arith = bitmask_check(9, 5, instr);
        uint32_t shift_arith;
        shift_arith = bitmask_check(22, 22, instr);
        uint32_t operand2;
        operand2 = bitmask_check(21, 10, instr);
        if (shift_arith == 1) {
            operand2 = operand2 << 12;
            //op2 should be unsigned
        }
        if (opc_dpimm < 2) {
            if (sf_dpimm == 0) {
                uint32_t temp = registers[rd_dpimm];
                temp = registers[rn_arith] + operand2;
                if (opc_dpimm == 1) {
                    if (temp == 0) {
                        pState.z = true;
                    }
                    if (sign32(temp) == 1) {
                        pState.n = true;
                    }
                    if (operand2 > ~get32from64(registers[rn_arith])) {
                        pState.c = true;
                    }
                    if (sign32(registers[rn_arith]) == 0 && sign32(temp) == 1) {
                        pState.v = true;
                    }
                }
                registers[rd_dpimm] = bitmask_check(63, 32, registers[rd_dpimm]) << 32;
                registers[rd_dpimm] = registers[rd_dpimm] |  temp;
            } else {
                registers[rd_dpimm] = registers[rn_arith] + operand2;
                if (opc_dpimm == 1) {
                    if (registers[rd_dpimm] == 0) {
                        pState.z = true;
                    }
                    if (sign64(registers[rd_dpimm]) == 1) {
                        pState.n = true;
                    }
                    if (operand2 > ~registers[rn_arith]) {
                        pState.c = true;
                    }
                    if (sign64(registers[rn_arith]) == 0 && sign64(registers[rd_dpimm]) == 1) {
                        pState.v = true;
                    }
                }
            }
        } else {
            if (sf_dpimm == 0) {
                uint32_t temp = registers[rd_dpimm];
                temp = registers[rn_arith] - operand2;
                if (opc_dpimm == 1) {
                    if (temp == 0) {
                        pState.z = true;
                    }
                    if (sign32(temp) == 1) {
                        pState.n = true;
                    }
                    if (operand2 > get32from64(registers[rn_arith])) {
                        pState.c = true;
                    }
                    if (sign32(registers[rn_arith]) == 1 && sign32(temp) == 0) {
                        pState.v = true;
                    }
                }
                registers[rd_dpimm] = bitmask_check(63, 32, registers[rd_dpimm]) << 32;
                registers[rd_dpimm] = registers[rd_dpimm] |  temp;
            } else {
                registers[rd_dpimm] = registers[rn_arith] - operand2;
                if (opc_dpimm == 1) {
                    if (registers[rd_dpimm] == 0) {
                        pState.z = true;
                    }
                    if (sign64(registers[rd_dpimm]) == 1) {
                        pState.n = true;
                    }
                    if (operand2 > registers[rn_arith]) {
                        pState.c = true;
                    }
                    if (sign64(registers[rn_arith]) == 1 && sign64(registers[rd_dpimm]) == 0) {
                        pState.v = true;
                    }
                }
            }
        }
    }
    if (5 == opi_dpimm) {
        uint32_t sh_wm;
        sh_wm = bitmask_check(22, 21, instr);
        uint64_t imm16_wm;
        imm16_wm = bitmask_check(20, 5, instr) << (16 * sh_wm);
        if (opc_dpimm == 0) {
            if (sf_dpimm == 0) {
                uint32_t temp = imm16_wm;
                registers[rd_dpimm] = bitmask_check(63, 32, registers[rd_dpimm]) << 32;
                registers[rd_dpimm] = registers[rd_dpimm] |  ~temp;
            } else {
                registers[rd_dpimm] = ~imm16_wm;
            }
        }
        if (opc_dpimm == 2) {
            if (sf_dpimm == 0) {
                uint32_t temp = imm16_wm;
                registers[rd_dpimm] = bitmask_check(63, 32, registers[rd_dpimm]) << 32;
                registers[rd_dpimm] = registers[rd_dpimm] |  temp;
            } else {
                registers[rd_dpimm] = imm16_wm;
            }
        }
        if (opc_dpimm == 3) {
            registers[rd_dpimm] = registers[rd_dpimm] |  bitmask(sh_wm*16 + 15, sh_wm*16);
            registers[rd_dpimm] = registers[rd_dpimm] & imm16_wm;
        }
    }
}