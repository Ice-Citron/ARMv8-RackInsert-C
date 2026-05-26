#include "dp.h"

static uint64_t mask_from_sf(uint32_t sf) {
    return sf == 0 ? UINT32_MAX : UINT64_MAX;
}

static uint64_t read_dp_register(uint32_t reg) {
    return reg == 31 ? zeroRegister : registers[reg];
}

static void write_dp_result(uint32_t rd, uint32_t sf, uint64_t entry) {
    if (rd == 31) {
        return;
    }

    if (sf == 0) {
        registers[rd] = (registers[rd] & 0xffffffff00000000ULL)
                      | (entry & UINT32_MAX);
    } else {
        registers[rd] = entry;
    }
}

void dpimm(uint32_t instr) {
    pState = (state){false, false, false, false};

    uint32_t sf_dpimm = bitmask_check(31, 31, instr);
    uint32_t opc_dpimm = bitmask_check(30, 29, instr);
    uint32_t opi_dpimm = bitmask_check(25, 23, instr);
    uint32_t rd_dpimm = bitmask_check(4, 0, instr);
    uint64_t mask = mask_from_sf(sf_dpimm);
    uint64_t entry = 0;

    if (opi_dpimm == 2) {
        uint32_t rn_arith = bitmask_check(9, 5, instr);
        uint32_t shift_arith = bitmask_check(22, 22, instr);
        uint64_t operand2 = bitmask_check(21, 10, instr);
        uint64_t rn_value = read_dp_register(rn_arith) & mask;

        if (shift_arith == 1) {
            operand2 <<= 12;
        }
        operand2 &= mask;

        if (opc_dpimm < 2) {
            entry = (rn_value + operand2) & mask;
            if (opc_dpimm == 1) {
                if (sf_dpimm == 0) {
                    add32flags(rn_arith, entry, &pState, operand2);
                } else {
                    add64flags(rn_arith, entry, &pState, operand2);
                }
            }
        } else {
            entry = (rn_value - operand2) & mask;
            if (opc_dpimm == 3) {
                if (sf_dpimm == 0) {
                    sub32flags(rn_arith, entry, &pState, operand2);
                } else {
                    sub64flags(rn_arith, entry, &pState, operand2);
                }
            }
        }

        write_dp_result(rd_dpimm, sf_dpimm, entry);
    } else if (opi_dpimm == 5) {
        uint32_t sh_wm = bitmask_check(22, 21, instr);
        uint64_t imm16_wm = bitmask_check(20, 5, instr);
        uint32_t shift = 16 * sh_wm;
        uint64_t shifted = (imm16_wm << shift) & mask;

        if (opc_dpimm == 0) {
            entry = ~shifted & mask;
        } else if (opc_dpimm == 2) {
            entry = shifted;
        } else if (opc_dpimm == 3) {
            uint64_t clear_mask = ~(0xffffULL << shift) & mask;
            entry = (read_dp_register(rd_dpimm) & clear_mask) | shifted;
        } else {
            return;
        }

        write_dp_result(rd_dpimm, sf_dpimm, entry);
    }
}
