/**
 * libgb
 * Copyright (C) 2015 David Jolly
 * ----------------------
 *
 * libgb is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libgb is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "../include/gb.h"
#include "../include/gb_cpu_type.h"

namespace GB_NS {

	namespace GB_COMP_NS {

		#define GB_FLAG_C 0x10
		#define GB_FLAG_H 0x20
		#define GB_FLAG_N 0x40
		#define GB_FLAG_Z 0x80
		#define GB_INT_HALT_INIT false
		#define GB_INT_IME_INIT true
		#define GB_INT_STOP_INIT false
		#define GB_REG_A_INIT 0x00
		#define GB_REG_A_ALT_INIT 0x00
		#define GB_REG_B_INIT 0x00
		#define GB_REG_B_ALT_INIT 0x00
		#define GB_REG_C_INIT 0x00
		#define GB_REG_C_ALT_INIT 0x00
		#define GB_REG_D_INIT 0x00
		#define GB_REG_D_ALT_INIT 0x00
		#define GB_REG_E_INIT 0x00
		#define GB_REG_E_ALT_INIT 0x00
		#define GB_REG_F_INIT 0x00
		#define GB_REG_F_ALT_INIT 0x00
		#define GB_REG_H_INIT 0x00
		#define GB_REG_H_ALT_INIT 0x00
		#define GB_REG_L_INIT 0x00
		#define GB_REG_L_ALT_INIT 0x00
		#define GB_REG_PC_INIT 0x0000
		#define GB_REG_SP_INIT 0x0000
		#define GB_RST_VEC_0 0x0000
		#define GB_RST_VEC_8 0x0008
		#define GB_RST_VEC_10 0x0010
		#define GB_RST_VEC_18 0x0018
		#define GB_RST_VEC_20 0x0020
		#define GB_RST_VEC_28 0x0028
		#define GB_RST_VEC_30 0x0030
		#define GB_RST_VEC_38 0x0038

		#define DETERMINE_HALF_CARRY(_A_, _B_) \
			(((_A_) ^ (_B_)) & (UINT4_MAX + 1))
		#define _DETERMINE_HALF_CARRY(_RES_, _A_, _B_, _LEN_) \
			(((_RES_) ^ (_A_) ^ (_B_)) & (_LEN_))
		#define DETERMINE_HALF_CARRY_BYTE(_RES_, _A_, _B_) \
			_DETERMINE_HALF_CARRY(_RES_, _A_, _B_, UINT4_MAX + 1)
		#define DETERMINE_HALF_CARRY_WORD(_RES_, _A_, _B_) \
			_DETERMINE_HALF_CARRY(_RES_, _A_, _B_, UINT8_MAX + 1)

		gb_cpu_ptr gb_cpu::m_inst = NULL;

		_gb_cpu::_gb_cpu(void) :
			m_init(false),
			m_mmu(gb_mmu::acquire()),
			m_ra(GB_REG_A_INIT),
			m_rb(GB_REG_B_INIT),
			m_rc(GB_REG_C_INIT),
			m_rd(GB_REG_D_INIT),
			m_re(GB_REG_E_INIT),
			m_rf(GB_REG_F_INIT),
			m_rh(GB_REG_H_INIT),
			m_rl(GB_REG_L_INIT),
			m_last(0),
			m_rva(GB_REG_A_ALT_INIT),
			m_rvb(GB_REG_B_ALT_INIT),
			m_rvc(GB_REG_C_ALT_INIT),
			m_rvd(GB_REG_D_ALT_INIT),
			m_rve(GB_REG_E_ALT_INIT),
			m_rvf(GB_REG_F_ALT_INIT),
			m_rvh(GB_REG_H_ALT_INIT),
			m_rvl(GB_REG_L_ALT_INIT),
			m_pc(GB_REG_PC_INIT),
			m_sp(GB_REG_SP_INIT),
			m_tot(0),
			m_halt(GB_INT_HALT_INIT),
			m_ime(GB_INT_IME_INIT),
			m_stop(GB_INT_STOP_INIT)
		{
			std::atexit(gb_cpu::_delete);
		}

		_gb_cpu::~_gb_cpu(void)
		{

			if(m_init) {
				uninitialize();
			}
		}

		void 
		_gb_cpu::_delete(void)
		{

			if(gb_cpu::m_inst) {
				delete gb_cpu::m_inst;
				gb_cpu::m_inst = NULL;
			}
		}

		gb_cpu_ptr 
		_gb_cpu::acquire(void)
		{

			if(!gb_cpu::m_inst) {

				gb_cpu::m_inst = new gb_cpu;
				if(!gb_cpu::m_inst) {
					THROW_GB_CPU_EXCEPTION(GB_CPU_EXCEPTION_ALLOC_FAILED);
				}
			}

			return gb_cpu::m_inst;
		}

		gbw_t 
		_gb_cpu::af(void)
		{
			return ((m_ra << BITS_PER_BYTE) | m_rf);
		}

		gbw_t 
		_gb_cpu::bc(void)
		{
			return ((m_rb << BITS_PER_BYTE) | m_rc);
		}

		gbw_t 
		_gb_cpu::de(void)
		{
			return ((m_rd << BITS_PER_BYTE) | m_re);
		}

		void 
		_gb_cpu::execute(
			__in gbb_t code
			)
		{
			gb_cmd_t cmd = GB_CMD_TYPE(GB_CMD_TYPE_EXT, code);

			switch(cmd) {
				case GB_CMD_ADC:
					execute_cmd_adc(code);
					break;
				case GB_CMD_ADD:
					execute_cmd_add(code);
					break;
				case GB_CMD_AND:
					execute_cmd_and(code);
					break;
				case GB_CMD_CALL:
					execute_cmd_call(code);
					break;
				case GB_CMD_CCF:
					execute_cmd_ccf(code);
					break;
				case GB_CMD_CP:
					execute_cmd_cp(code);
					break;
				case GB_CMD_CPL:
					execute_cmd_cpl(code);
					break;
				case GB_CMD_DAA:
					execute_cmd_daa(code);
					break;
				case GB_CMD_DEC:
					execute_cmd_dec(code);
					break;
				case GB_CMD_DI:
					execute_cmd_di(code);
					break;
				case GB_CMD_EI:
					execute_cmd_ei(code);
					break;
				case GB_CMD_HALT:
					execute_cmd_halt(code);
					break;
				case GB_CMD_INC:
					execute_cmd_inc(code);
					break;
				case GB_CMD_JP:
					execute_cmd_jp(code);
					break;
				case GB_CMD_JR:
					execute_cmd_jr(code);
					break;
				case GB_CMD_LD:
					execute_cmd_ld(code);
					break;
				case GB_CMD_LDD:
					execute_cmd_ldd(code);
					break;
				case GB_CMD_LDH:
					execute_cmd_ldh(code);
					break;
				case GB_CMD_LDHL:
					execute_cmd_ldhl(code);
					break;
				case GB_CMD_LDI:
					execute_cmd_ldi(code);
					break;
				case GB_CMD_NOP:
					execute_cmd_nop(code);
					break;
				case GB_CMD_OR:
					execute_cmd_or(code);
					break;
				case GB_CMD_POP:
					execute_cmd_pop(code);
					break;
				case GB_CMD_PUSH:
					execute_cmd_push(code);
					break;
				case GB_CMD_RET:
					execute_cmd_ret(code);
					break;
				case GB_CMD_RETI:
					execute_cmd_reti(code);
					break;
				case GB_CMD_RL:
					execute_cmd_rl(code);
					break;
				case GB_CMD_RLC:
					execute_cmd_rlc(code);
					break;
				case GB_CMD_RR:
					execute_cmd_rr(code);
					break;
				case GB_CMD_RRC:
					execute_cmd_rrc(code);
					break;
				case GB_CMD_RST:
					execute_cmd_rst(code);
					break;
				case GB_CMD_SBC:
					execute_cmd_sbc(code);
					break;
				case GB_CMD_SCF:
					execute_cmd_scf(code);
					break;
				case GB_CMD_STOP:
					execute_cmd_stop(code);
					break;
				case GB_CMD_SUB:
					execute_cmd_sub(code);
					break;
				case GB_CMD_XOR:
					execute_cmd_xor(code);
					break;
				default:
					THROW_GB_CPU_EXCEPTION_MESSAGE(GB_CPU_EXCEPTION_INVALID_CMD,
						"0x%x", cmd);
			}
		}

		void 
		_gb_cpu::execute_cmd_adc(
			__in gbb_t code
			)
		{
			gbw_t res;
			gbb_t carry = (m_rf & GB_FLAG_C), val;

			m_rf = 0;

			switch(code) {
				case GB_CODE_ADC_A_A:

					res = m_ra + m_ra + carry;
					if(res > GBB_MAX_LEN) {
						m_rf |= GB_FLAG_C;
					}

					if(DETERMINE_HALF_CARRY_BYTE(res, m_ra, m_ra + carry)) {
						m_rf |= GB_FLAG_H;
					}

					m_ra = res;
					if(!m_ra) {
						m_rf |= GB_FLAG_Z;
					}
					m_last = 1;
					break;
				case GB_CODE_ADC_A_B:

					res = m_ra + m_rb + carry;
					if(res > GBB_MAX_LEN) {
						m_rf |= GB_FLAG_C;
					}

					if(DETERMINE_HALF_CARRY_BYTE(res, m_ra, m_rb + carry)) {
						m_rf |= GB_FLAG_H;
					}

					m_ra = res;
					if(!m_ra) {
						m_rf |= GB_FLAG_Z;
					}
					m_last = 1;
					break;
				case GB_CODE_ADC_A_C:

					res = m_ra + m_rc + carry;
					if(res > GBB_MAX_LEN) {
						m_rf |= GB_FLAG_C;
					}

					if(DETERMINE_HALF_CARRY_BYTE(res, m_ra, m_rc + carry)) {
						m_rf |= GB_FLAG_H;
					}

					m_ra = res;
					if(!m_ra) {
						m_rf |= GB_FLAG_Z;
					}
					m_last = 1;
					break;
				case GB_CODE_ADC_A_D:

					res = m_ra + m_rd + carry;
					if(res > GBB_MAX_LEN) {
						m_rf |= GB_FLAG_C;
					}

					if(DETERMINE_HALF_CARRY_BYTE(res, m_ra, m_rd + carry)) {
						m_rf |= GB_FLAG_H;
					}

					m_ra = res;
					if(!m_ra) {
						m_rf |= GB_FLAG_Z;
					}
					m_last = 1;
					break;
				case GB_CODE_ADC_A_E:

					res = m_ra + m_re + carry;
					if(res > GBB_MAX_LEN) {
						m_rf |= GB_FLAG_C;
					}

					if(DETERMINE_HALF_CARRY_BYTE(res, m_ra, m_re + carry)) {
						m_rf |= GB_FLAG_H;
					}

					m_ra = res;
					if(!m_ra) {
						m_rf |= GB_FLAG_Z;
					}
					m_last = 1;
					break;
				case GB_CODE_ADC_A_H:

					res = m_ra + m_rh + carry;
					if(res > GBB_MAX_LEN) {
						m_rf |= GB_FLAG_C;
					}

					if(DETERMINE_HALF_CARRY_BYTE(res, m_ra, m_rh + carry)) {
						m_rf |= GB_FLAG_H;
					}

					m_ra = res;
					if(!m_ra) {
						m_rf |= GB_FLAG_Z;
					}
					m_last = 1;
					break;
				case GB_CODE_ADC_A_L:

					res = m_ra + m_rl + carry;
					if(res > GBB_MAX_LEN) {
						m_rf |= GB_FLAG_C;
					}

					if(DETERMINE_HALF_CARRY_BYTE(res, m_ra, m_rl + carry)) {
						m_rf |= GB_FLAG_H;
					}

					m_ra = res;
					if(!m_ra) {
						m_rf |= GB_FLAG_Z;
					}
					m_last = 1;
					break;
				case GB_CODE_ADC_A_HL_INDIRECT:
					val = m_mmu->read_byte(hl());

					res = m_ra + val + carry;
					if(res > GBB_MAX_LEN) {
						m_rf |= GB_FLAG_C;
					}

					if(DETERMINE_HALF_CARRY_BYTE(res, m_ra, val + carry)) {
						m_rf |= GB_FLAG_H;
					}

					m_ra = res;
					if(!m_ra) {
						m_rf |= GB_FLAG_Z;
					}
					m_last = 2;
					break;
				case GB_CODE_ADC_A_N:
					val = m_mmu->read_byte(m_pc++);

					res = m_ra + val + carry;
					if(res > GBB_MAX_LEN) {
						m_rf |= GB_FLAG_C;
					}

					if(DETERMINE_HALF_CARRY_BYTE(res, m_ra, val + carry)) {
						m_rf |= GB_FLAG_H;
					}

					m_ra = res;
					if(!m_ra) {
						m_rf |= GB_FLAG_Z;
					}
					m_last = 2;
					break;
				default:
					THROW_GB_CPU_EXCEPTION_MESSAGE(GB_CPU_EXCEPTION_INVALID_CODE,
						"%s:0x%x", GB_CMD_STRING(GB_CMD_ADC), code);
			}
		}

		void 
		_gb_cpu::execute_cmd_add(
			__in gbb_t code
			)
		{			
			gbb_t val;
			gbdw_t res;
			int8_t off;
			gbw_t tmp0, tmp1;

			switch(code) {
				case GB_CODE_ADD_A_A:
					m_rf = 0;

					res = m_ra + m_ra;
					if(res > GBB_MAX_LEN) {
						m_rf |= GB_FLAG_C;
					}

					if(DETERMINE_HALF_CARRY_BYTE(res, m_ra, m_ra)) {
						m_rf |= GB_FLAG_H;
					}

					m_ra = res;
					if(!m_ra) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 1;
					break;
				case GB_CODE_ADD_A_B:
					m_rf = 0;

					res = m_ra + m_rb;
					if(res > GBB_MAX_LEN) {
						m_rf |= GB_FLAG_C;
					}

					if(DETERMINE_HALF_CARRY_BYTE(res, m_ra, m_rb)) {
						m_rf |= GB_FLAG_H;
					}

					m_ra = res;
					if(!m_ra) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 1;
					break;
				case GB_CODE_ADD_A_C:
					m_rf = 0;

					res = m_ra + m_rc;
					if(res > GBB_MAX_LEN) {
						m_rf |= GB_FLAG_C;
					}

					if(DETERMINE_HALF_CARRY_BYTE(res, m_ra, m_rc)) {
						m_rf |= GB_FLAG_H;
					}

					m_ra = res;
					if(!m_ra) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 1;
					break;
				case GB_CODE_ADD_A_D:
					m_rf = 0;

					res = m_ra + m_rd;
					if(res > GBB_MAX_LEN) {
						m_rf |= GB_FLAG_C;
					}

					if(DETERMINE_HALF_CARRY_BYTE(res, m_ra, m_rd)) {
						m_rf |= GB_FLAG_H;
					}

					m_ra = res;
					if(!m_ra) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 1;
					break;
				case GB_CODE_ADD_A_E:
					m_rf = 0;

					res = m_ra + m_re;
					if(res > GBB_MAX_LEN) {
						m_rf |= GB_FLAG_C;
					}

					if(DETERMINE_HALF_CARRY_BYTE(res, m_ra, m_re)) {
						m_rf |= GB_FLAG_H;
					}

					m_ra = res;
					if(!m_ra) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 1;
					break;
				case GB_CODE_ADD_A_H:
					m_rf = 0;

					res = m_ra + m_rh;
					if(res > GBB_MAX_LEN) {
						m_rf |= GB_FLAG_C;
					}

					if(DETERMINE_HALF_CARRY_BYTE(res, m_ra, m_rh)) {
						m_rf |= GB_FLAG_H;
					}

					m_ra = res;
					if(!m_ra) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 1;
					break;
				case GB_CODE_ADD_A_L:
					m_rf = 0;

					res = m_ra + m_rl;
					if(res > GBB_MAX_LEN) {
						m_rf |= GB_FLAG_C;
					}

					if(DETERMINE_HALF_CARRY_BYTE(res, m_ra, m_rl)) {
						m_rf |= GB_FLAG_H;
					}

					m_ra = res;
					if(!m_ra) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 1;
					break;
				case GB_CODE_ADD_A_HL_INDIRECT:
					m_rf = 0;
					val = m_mmu->read_byte(hl());

					res = m_ra + val;
					if(res > GBB_MAX_LEN) {
						m_rf |= GB_FLAG_C;
					}

					if(DETERMINE_HALF_CARRY_BYTE(res, m_ra, val)) {
						m_rf |= GB_FLAG_H;
					}

					m_ra = res;
					if(!m_ra) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_ADD_A_N:
					m_rf = 0;
					val = m_mmu->read_byte(m_pc++);

					res = m_ra + val;
					if(res > GBB_MAX_LEN) {
						m_rf |= GB_FLAG_C;
					}

					if(DETERMINE_HALF_CARRY_BYTE(res, m_ra, val)) {
						m_rf |= GB_FLAG_H;
					}

					m_ra = res;
					if(!m_ra) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_ADD_HL_BC:
					m_rf &= ~(GB_FLAG_C | GB_FLAG_H | GB_FLAG_N);
					tmp0 = hl();
					tmp1 = bc();

					res = tmp0 + tmp1;
					if(res > GBW_MAX_LEN) {
						m_rf |= GB_FLAG_C;
					}

					if(DETERMINE_HALF_CARRY_WORD(res, tmp0, tmp1)) {
						m_rf |= GB_FLAG_H;
					}

					if(!res) {
						m_rf |= GB_FLAG_Z;
					}

					set_hl(res);
					m_last = 2;
					break;
				case GB_CODE_ADD_HL_DE:
					m_rf &= ~(GB_FLAG_C | GB_FLAG_H | GB_FLAG_N);
					tmp0 = hl();
					tmp1 = de();

					res = tmp0 + tmp1;
					if(res > GBW_MAX_LEN) {
						m_rf |= GB_FLAG_C;
					}

					if(DETERMINE_HALF_CARRY_WORD(res, tmp0, tmp1)) {
						m_rf |= GB_FLAG_H;
					}

					if(!res) {
						m_rf |= GB_FLAG_Z;
					}

					set_hl(res);
					m_last = 2;
					break;
				case GB_CODE_ADD_HL_HL:
					m_rf &= ~(GB_FLAG_C | GB_FLAG_H | GB_FLAG_N);
					tmp0 = hl();
					tmp1 = hl();

					res = tmp0 + tmp1;
					if(res > GBW_MAX_LEN) {
						m_rf |= GB_FLAG_C;
					}

					if(DETERMINE_HALF_CARRY_WORD(res, tmp0, tmp1)) {
						m_rf |= GB_FLAG_H;
					}

					if(!res) {
						m_rf |= GB_FLAG_Z;
					}

					set_hl(res);
					m_last = 2;
					break;
				case GB_CODE_ADD_HL_SP:
					m_rf &= ~(GB_FLAG_C | GB_FLAG_H | GB_FLAG_N);
					tmp0 = hl();

					res = tmp0 + m_sp;
					if(res > GBW_MAX_LEN) {
						m_rf |= GB_FLAG_C;
					}

					if(DETERMINE_HALF_CARRY_WORD(res, tmp0, m_sp)) {
						m_rf |= GB_FLAG_H;
					}

					if(!res) {
						m_rf |= GB_FLAG_Z;
					}

					set_hl(res);
					m_last = 2;
					break;
				case GB_CODE_ADD_SP_N:
					m_rf = 0;
					tmp0 = m_sp;
					off = m_mmu->read_byte(m_pc++);

					if((m_sp + off) > GBW_MAX_LEN) {
						m_rf |= GB_FLAG_C;
					}

					m_sp += off;

					if(DETERMINE_HALF_CARRY_WORD(m_sp, tmp0, off)) {
						m_rf |= GB_FLAG_H;
					}

					m_last = 4;
					break;
				default:
					THROW_GB_CPU_EXCEPTION_MESSAGE(GB_CPU_EXCEPTION_INVALID_CODE,
						"%s:0x%x", GB_CMD_STRING(GB_CMD_ADD), code);
			}
		}

		void 
		_gb_cpu::execute_cmd_and(
			__in gbb_t code
			)
		{
			m_rf = GB_FLAG_H;

			switch(code) {
				case GB_CODE_AND_A:
					m_ra &= m_ra;
					m_last = 1;
					break;
				case GB_CODE_AND_B:
					m_ra &= m_rb;
					m_last = 1;
					break;
				case GB_CODE_AND_C:
					m_ra &= m_rc;
					m_last = 1;
					break;
				case GB_CODE_AND_D:
					m_ra &= m_rd;
					m_last = 1;
					break;
				case GB_CODE_AND_E:
					m_ra &= m_re;
					m_last = 1;
					break;
				case GB_CODE_AND_H:
					m_ra &= m_rh;
					m_last = 1;
					break;
				case GB_CODE_AND_L:
					m_ra &= m_rl;
					m_last = 1;
					break;
				case GB_CODE_AND_HL_INDIRECT:
					m_ra &= m_mmu->read_byte(hl());
					m_last = 2;
					break;
				case GB_CODE_AND_N:
					m_ra &= m_mmu->read_byte(m_pc++);
					m_last = 2;
					break;
				default:
					THROW_GB_CPU_EXCEPTION_MESSAGE(GB_CPU_EXCEPTION_INVALID_CODE,
						"%s:0x%x", GB_CMD_STRING(GB_CMD_AND), code);
			}

			if(!m_ra) {
				m_rf |= GB_FLAG_Z;
			}
		}

		void 
		_gb_cpu::execute_cmd_bit(
			__in gbb_t code
			)
		{
			m_rf |= GB_FLAG_H;
			m_rf &= ~GB_FLAG_N;

			switch(code) {
				case GB_CODE_BIT_0_A:

					if(!(m_ra & 0x1)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_0_B:

					if(!(m_rb & 0x1)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_0_C:

					if(!(m_rc & 0x1)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_0_D:

					if(!(m_rd & 0x1)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_0_E:

					if(!(m_re & 0x1)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_0_H:

					if(!(m_rh & 0x1)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_0_L:

					if(!(m_rl & 0x1)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_0_HL_INDIRECT:

					if(!(m_mmu->read_byte(hl()) & 0x1)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 4;
					break;
				case GB_CODE_BIT_1_A:

					if(!(m_ra & 0x2)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_1_B:

					if(!(m_rb & 0x2)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_1_C:

					if(!(m_rc & 0x2)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_1_D:

					if(!(m_rd & 0x2)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_1_E:

					if(!(m_re & 0x2)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_1_H:

					if(!(m_rh & 0x2)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_1_L:

					if(!(m_rl & 0x2)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_1_HL_INDIRECT:

					if(!(m_mmu->read_byte(hl()) & 0x2)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 4;
					break;
				case GB_CODE_BIT_2_A:

					if(!(m_ra & 0x4)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_2_B:

					if(!(m_rb & 0x4)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_2_C:

					if(!(m_rc & 0x4)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_2_D:

					if(!(m_rd & 0x4)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_2_E:

					if(!(m_re & 0x4)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_2_H:

					if(!(m_rh & 0x4)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_2_L:

					if(!(m_rl & 0x4)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_2_HL_INDIRECT:

					if(!(m_mmu->read_byte(hl()) & 0x4)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 4;
					break;
				case GB_CODE_BIT_3_A:

					if(!(m_ra & 0x8)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_3_B:

					if(!(m_rb & 0x8)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_3_C:

					if(!(m_rc & 0x8)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_3_D:

					if(!(m_rd & 0x8)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_3_E:

					if(!(m_re & 0x8)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_3_H:

					if(!(m_rh & 0x8)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_3_L:

					if(!(m_rl & 0x8)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_3_HL_INDIRECT:

					if(!(m_mmu->read_byte(hl()) & 0x8)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 4;
					break;
				case GB_CODE_BIT_4_A:

					if(!(m_ra & 0x10)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_4_B:

					if(!(m_rb & 0x10)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_4_C:

					if(!(m_rc & 0x10)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_4_D:

					if(!(m_rd & 0x10)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_4_E:

					if(!(m_re & 0x10)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_4_H:

					if(!(m_rh & 0x10)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_4_L:

					if(!(m_rl & 0x10)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_4_HL_INDIRECT:

					if(!(m_mmu->read_byte(hl()) & 0x10)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 4;
					break;
				case GB_CODE_BIT_5_A:

					if(!(m_ra & 0x20)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_5_B:

					if(!(m_rb & 0x20)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_5_C:

					if(!(m_rc & 0x20)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_5_D:

					if(!(m_rd & 0x20)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_5_E:

					if(!(m_re & 0x20)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_5_H:

					if(!(m_rh & 0x20)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_5_L:

					if(!(m_rl & 0x20)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_5_HL_INDIRECT:

					if(!(m_mmu->read_byte(hl()) & 0x20)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 4;
					break;
				case GB_CODE_BIT_6_A:

					if(!(m_ra & 0x40)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_6_B:

					if(!(m_rb & 0x40)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_6_C:

					if(!(m_rc & 0x40)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_6_D:

					if(!(m_rd & 0x40)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_6_E:

					if(!(m_re & 0x40)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_6_H:

					if(!(m_rh & 0x40)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_6_L:

					if(!(m_rl & 0x40)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_6_HL_INDIRECT:

					if(!(m_mmu->read_byte(hl()) & 0x40)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 4;
					break;
				case GB_CODE_BIT_7_A:

					if(!(m_ra & 0x80)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_7_B:

					if(!(m_rb & 0x80)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_7_C:

					if(!(m_rc & 0x80)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_7_D:

					if(!(m_rd & 0x80)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_7_E:

					if(!(m_re & 0x80)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_7_H:

					if(!(m_rh & 0x80)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_7_L:

					if(!(m_rl & 0x80)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_BIT_7_HL_INDIRECT:

					if(!(m_mmu->read_byte(hl()) & 0x80)) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 4;
					break;
				default:
					THROW_GB_CPU_EXCEPTION_MESSAGE(GB_CPU_EXCEPTION_INVALID_CODE,
						"%s:0x%x", GB_CMD_STRING(GB_CMD_BIT), code);
			}
		}

		void 
		_gb_cpu::execute_cmd_call(
			__in gbb_t code
			)
		{
			gbw_t addr;

			switch(code) {
				case GB_CODE_CALL_NN:
					addr = m_mmu->read_word(m_pc);
					m_mmu->write_word(m_sp, m_pc + sizeof(gbw_t));
					m_pc = addr;
					m_sp -= sizeof(gbw_t);
					break;
				case GB_CODE_CALL_NZ_NN:

					if(!(m_rf & GB_FLAG_Z)) {
						addr = m_mmu->read_word(m_pc);
						m_mmu->write_word(m_sp, m_pc + sizeof(gbw_t));
						m_pc = addr;
						m_sp -= sizeof(gbw_t);
					} else {
						m_pc += sizeof(gbw_t);
					}
					break;
				case GB_CODE_CALL_Z_NN:

					if(m_rf & GB_FLAG_Z) {
						addr = m_mmu->read_word(m_pc);
						m_mmu->write_word(m_sp, m_pc + sizeof(gbw_t));
						m_pc = addr;
						m_sp -= sizeof(gbw_t);
					} else {
						m_pc += sizeof(gbw_t);
					}
					break;
				case GB_CODE_CALL_NC_NN:

					if(!(m_rf & GB_FLAG_C)) {
						addr = m_mmu->read_word(m_pc);
						m_mmu->write_word(m_sp, m_pc + sizeof(gbw_t));
						m_pc = addr;
						m_sp -= sizeof(gbw_t);
					} else {
						m_pc += sizeof(gbw_t);
					}
					break;
				case GB_CODE_CALL_C_NN:

					if(m_rf & GB_FLAG_C) {
						addr = m_mmu->read_word(m_pc);
						m_mmu->write_word(m_sp, m_pc + sizeof(gbw_t));
						m_pc = addr;
						m_sp -= sizeof(gbw_t);
					} else {
						m_pc += sizeof(gbw_t);
					}
					break;
				default:
					THROW_GB_CPU_EXCEPTION_MESSAGE(GB_CPU_EXCEPTION_INVALID_CODE,
						"%s:0x%x", GB_CMD_STRING(GB_CMD_CALL), code);
			}

			m_last = 3;
		}

		void 
		_gb_cpu::execute_cmd_ccf(
			__in gbb_t code
			)
		{
			m_rf &= ~(GB_FLAG_H | GB_FLAG_N);
			m_rf ^= GB_FLAG_C;
			m_last = 1;
		}

		void 
		_gb_cpu::execute_cmd_cp(
			__in gbb_t code
			)
		{
			gbb_t res, val;

			m_rf = GB_FLAG_N;

			switch(code) {
				case GB_CODE_CP_A:

					res = m_ra - m_ra;
					if(!res) {
						m_rf |= GB_FLAG_Z;
					}

					if(DETERMINE_HALF_CARRY_BYTE(res, m_ra, m_ra)) {
						m_rf |= GB_FLAG_H;
					}

					m_last = 1;
					break;
				case GB_CODE_CP_B:

					if(m_ra < m_rb) {
						m_rf |= GB_FLAG_C;
					}

					res = m_ra - m_rb;
					if(!res) {
						m_rf |= GB_FLAG_Z;
					}

					if(DETERMINE_HALF_CARRY_BYTE(res, m_ra, m_rb)) {
						m_rf |= GB_FLAG_H;
					}

					m_last = 1;
					break;
				case GB_CODE_CP_C:

					if(m_ra < m_rc) {
						m_rf |= GB_FLAG_C;
					}

					res = m_ra - m_rc;
					if(!res) {
						m_rf |= GB_FLAG_Z;
					}

					if(DETERMINE_HALF_CARRY_BYTE(res, m_ra, m_rc)) {
						m_rf |= GB_FLAG_H;
					}

					m_last = 1;
					break;
				case GB_CODE_CP_D:

					if(m_ra < m_rd) {
						m_rf |= GB_FLAG_C;
					}

					res = m_ra - m_rd;
					if(!res) {
						m_rf |= GB_FLAG_Z;
					}

					if(DETERMINE_HALF_CARRY_BYTE(res, m_ra, m_rd)) {
						m_rf |= GB_FLAG_H;
					}

					m_last = 1;
					break;
				case GB_CODE_CP_E:

					if(m_ra < m_re) {
						m_rf |= GB_FLAG_C;
					}

					res = m_ra - m_re;
					if(!res) {
						m_rf |= GB_FLAG_Z;
					}

					if(DETERMINE_HALF_CARRY_BYTE(res, m_ra, m_re)) {
						m_rf |= GB_FLAG_H;
					}

					m_last = 1;
					break;
				case GB_CODE_CP_H:

					if(m_ra < m_rh) {
						m_rf |= GB_FLAG_C;
					}

					res = m_ra - m_rh;
					if(!res) {
						m_rf |= GB_FLAG_Z;
					}

					if(DETERMINE_HALF_CARRY_BYTE(res, m_ra, m_rh)) {
						m_rf |= GB_FLAG_H;
					}

					m_last = 1;
					break;
				case GB_CODE_CP_L:

					if(m_ra < m_rl) {
						m_rf |= GB_FLAG_C;
					}

					res = m_ra - m_rl;
					if(!res) {
						m_rf |= GB_FLAG_Z;
					}

					if(DETERMINE_HALF_CARRY_BYTE(res, m_ra, m_rl)) {
						m_rf |= GB_FLAG_H;
					}

					m_last = 1;
					break;
				case GB_CODE_CP_HL_INDIRECT:

					val = m_mmu->read_byte(hl());
					if(m_ra < val) {
						m_rf |= GB_FLAG_C;
					}

					res = m_ra - val;
					if(!res) {
						m_rf |= GB_FLAG_Z;
					}

					if(DETERMINE_HALF_CARRY_BYTE(res, m_ra, val)) {
						m_rf |= GB_FLAG_H;
					}

					m_last = 2;
					break;
				case GB_CODE_CP_N:

					val = m_mmu->read_byte(m_pc++);
					if(m_ra < val) {
						m_rf |= GB_FLAG_C;
					}

					res = m_ra - val;
					if(!res) {
						m_rf |= GB_FLAG_Z;
					}

					if(DETERMINE_HALF_CARRY_BYTE(res, m_ra, val)) {
						m_rf |= GB_FLAG_H;
					}

					m_last = 2;
					break;
				default:
					THROW_GB_CPU_EXCEPTION_MESSAGE(GB_CPU_EXCEPTION_INVALID_CODE,
						"%s:0x%x", GB_CMD_STRING(GB_CMD_CP), code);
			}
		}

		void 
		_gb_cpu::execute_cmd_cpl(
			__in gbb_t code
			)
		{
			m_ra = ~m_ra;
			m_rf |= (GB_FLAG_H | GB_FLAG_N);
			m_last = 1;
		}

		void 
		_gb_cpu::execute_cmd_daa(
			__in gbb_t code
			)
		{
			gbb_t old = m_ra;

			if((m_rf & GB_FLAG_H) 
					|| ((old & UINT4_MAX) > GCD_MAX)) {
				m_ra += 0x6;
			}

			m_rf &= ~(GB_FLAG_H | GB_FLAG_Z);

			if((m_rf & GB_FLAG_C) 
					|| (((old >> BITS_PER_NIBBLE) & UINT4_MAX) > GCD_MAX)) {
				m_ra += 0x60;
				m_rf |= GB_FLAG_C;
			} else {
				m_rf &= ~GB_FLAG_C;
			}

			if(!m_ra) {
				m_rf |= GB_FLAG_Z;
			}

			m_last = 1;
		}

		void 
		_gb_cpu::execute_cmd_dec(
			__in gbb_t code
			)
		{
			gbb_t orig, tmp;

			m_rf &= ~(GB_FLAG_H | GB_FLAG_Z);
			m_rf |= GB_FLAG_N;

			switch(code) {
				case GB_CODE_DEC_A:
					orig = m_ra;
					--m_ra;

					if(DETERMINE_HALF_CARRY(m_ra, orig)) {
						m_rf |= GB_FLAG_H;
					}

					if(!m_ra) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 1;
					break;
				case GB_CODE_DEC_B:
					orig = m_rb;
					--m_rb;

					if(DETERMINE_HALF_CARRY(m_rb, orig)) {
						m_rf |= GB_FLAG_H;
					}

					if(!m_rb) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 1;
					break;
				case GB_CODE_DEC_C:
					orig = m_rc;
					--m_rc;

					if(DETERMINE_HALF_CARRY(m_rc, orig)) {
						m_rf |= GB_FLAG_H;
					}

					if(!m_rc) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 1;
					break;
				case GB_CODE_DEC_D:
					orig = m_rd;
					--m_rd;

					if(DETERMINE_HALF_CARRY(m_rd, orig)) {
						m_rf |= GB_FLAG_H;
					}

					if(!m_rd) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 1;
					break;
				case GB_CODE_DEC_E:
					orig = m_re;
					--m_re;

					if(DETERMINE_HALF_CARRY(m_re, orig)) {
						m_rf |= GB_FLAG_H;
					}

					if(!m_re) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 1;
					break;
				case GB_CODE_DEC_H:
					orig = m_rh;
					--m_rh;

					if(DETERMINE_HALF_CARRY(m_rh, orig)) {
						m_rf |= GB_FLAG_H;
					}

					if(!m_rh) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 1;
					break;
				case GB_CODE_DEC_L:
					orig = m_rl;
					--m_rl;

					if(DETERMINE_HALF_CARRY(m_rl, orig)) {
						m_rf |= GB_FLAG_H;
					}

					if(!m_rl) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 1;
					break;
				case GB_CODE_DEC_HL_INDIRECT:
					orig = m_mmu->read_byte(hl());
					tmp = orig;
					--orig;

					if(DETERMINE_HALF_CARRY(orig, tmp)) {
						m_rf |= GB_FLAG_H;
					}

					if(!orig) {
						m_rf |= GB_FLAG_Z;
					}

					m_mmu->write_byte(hl(), orig);
					m_last = 3;
					break;
				case GB_CODE_DEC_BC:

					if(m_rb) {
						--m_rb;
					} else if(m_rc) {
						--m_rc;
					} else {
						m_rb = GBB_MAX_LEN;
						m_rc = GBB_MAX_LEN;
					}

					m_last = 2;
					break;
				case GB_CODE_DEC_DE:

					if(m_rd) {
						--m_rd;
					} else if(m_re) {
						--m_re;
					} else {
						m_rd = GBB_MAX_LEN;
						m_re = GBB_MAX_LEN;
					}

					m_last = 2;
					break;
				case GB_CODE_DEC_HL:

					if(m_rh) {
						--m_rh;
					} else if(m_rl) {
						--m_rl;
					} else {
						m_rh = GBB_MAX_LEN;
						m_rl = GBB_MAX_LEN;
					}

					m_last = 2;
					break;
				case GB_CODE_DEC_SP:
					--m_sp;
					m_last = 2;
					break;
				default:
					THROW_GB_CPU_EXCEPTION_MESSAGE(GB_CPU_EXCEPTION_INVALID_CODE,
						"%s:0x%x", GB_CMD_STRING(GB_CMD_DEC), code);
			}
		}

		void 
		_gb_cpu::execute_cmd_di(
			__in gbb_t code
			)
		{
			m_ime = false;
			m_last = 1;
		}

		void 
		_gb_cpu::execute_cmd_ei(
			__in gbb_t code
			)
		{
			m_ime = true;
			m_last = 1;
		}

		void 
		_gb_cpu::execute_cmd_halt(
			__in gbb_t code
			)
		{
			halt();
			m_last = 1;
		}

		void 
		_gb_cpu::execute_cmd_inc(
			__in gbb_t code
			)
		{
			gbb_t orig, tmp;

			switch(code) {
				case GB_CODE_INC_A:
					orig = m_ra;
					++m_ra;

					if(DETERMINE_HALF_CARRY(m_ra, orig)) {
						m_rf |= GB_FLAG_H;
					}

					if(!m_ra) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 1;
					break;
				case GB_CODE_INC_B:
					orig = m_rb;
					++m_rb;

					if(DETERMINE_HALF_CARRY(m_rb, orig)) {
						m_rf |= GB_FLAG_H;
					}

					if(!m_rb) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 1;
					break;
				case GB_CODE_INC_C:
					orig = m_rc;
					++m_rc;

					if(DETERMINE_HALF_CARRY(m_rc, orig)) {
						m_rf |= GB_FLAG_H;
					}

					if(!m_rc) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 1;
					break;		
				case GB_CODE_INC_D:
					orig = m_rd;
					++m_rd;

					if(DETERMINE_HALF_CARRY(m_rd, orig)) {
						m_rf |= GB_FLAG_H;
					}

					if(!m_rd) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 1;
					break;
				case GB_CODE_INC_E:
					orig = m_re;
					++m_re;

					if(DETERMINE_HALF_CARRY(m_re, orig)) {
						m_rf |= GB_FLAG_H;
					}

					if(!m_re) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 1;
					break;		
				case GB_CODE_INC_H:
					orig = m_rh;
					++m_rh;

					if(DETERMINE_HALF_CARRY(m_rh, orig)) {
						m_rf |= GB_FLAG_H;
					}

					if(!m_rh) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 1;
					break;
				case GB_CODE_INC_L:
					orig = m_rl;
					++m_rl;

					if(DETERMINE_HALF_CARRY(m_rl, orig)) {
						m_rf |= GB_FLAG_H;
					}

					if(!m_rl) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 1;
					break;
				case GB_CODE_INC_HL_INDIRECT:
					orig = m_mmu->read_byte(hl());
					tmp = orig;
					++orig;

					if(DETERMINE_HALF_CARRY(orig, tmp)) {
						m_rf |= GB_FLAG_H;
					}

					if(!orig) {
						m_rf |= GB_FLAG_Z;
					}

					m_mmu->write_byte(hl(), orig);
					m_last = 3;
					break;
				case GB_CODE_INC_BC:

					if(m_rc < GBB_MAX_LEN) {
						++m_rc;
					} else if(m_rb < GBB_MAX_LEN) {
						++m_rb;
					} else {
						m_rb = 0;
						m_rc = 0;
					}

					m_last = 2;
					break;
				case GB_CODE_INC_DE:

					if(m_re < GBB_MAX_LEN) {
						++m_re;
					} else if(m_rd < GBB_MAX_LEN) {
						++m_rd;
					} else {
						m_rd = 0;
						m_re = 0;
					}

					m_last = 2;
					break;
				case GB_CODE_INC_HL:

					if(m_rl < GBB_MAX_LEN) {
						++m_rl;
					} else if(m_rh < GBB_MAX_LEN) {
						++m_rh;
					} else {
						m_rh = 0;
						m_rl = 0;
					}

					m_last = 2;
					break;
				case GB_CODE_INC_SP:
					++m_sp;
					m_last = 2;
					break;
				default:
					THROW_GB_CPU_EXCEPTION_MESSAGE(GB_CPU_EXCEPTION_INVALID_CODE,
						"%s:0x%x", GB_CMD_STRING(GB_CMD_INC), code);
			}
		}

		void 
		_gb_cpu::execute_cmd_jp(
			__in gbb_t code
			)
		{

			switch(code) {
				case GB_CODE_JP_NN:
					m_pc = m_mmu->read_word(m_pc);
					m_last = 3;
					break;
				case GB_CODE_JP_NZ_NN:

					if(!(m_rf & GB_FLAG_Z)) {
						m_pc = m_mmu->read_word(m_pc);
					} else {
						m_pc += sizeof(gbw_t);
					}

					m_last = 3;
					break;
				case GB_CODE_JP_Z_NN:

					if(m_rf & GB_FLAG_Z) {
						m_pc = m_mmu->read_word(m_pc);
					} else {
						m_pc += sizeof(gbw_t);
					}

					m_last = 3;
					break;
				case GB_CODE_JP_NC_NN:

					if(!(m_rf & GB_FLAG_C)) {
						m_pc = m_mmu->read_word(m_pc);
					} else {
						m_pc += sizeof(gbw_t);
					}

					m_last = 3;
					break;
				case GB_CODE_JP_C_NN:

					if(m_rf & GB_FLAG_C) {
						m_pc = m_mmu->read_word(m_pc);
					} else {
						m_pc += sizeof(gbw_t);
					}

					m_last = 3;
					break;
				case GB_CODE_JP_HL_INDIRECT:
					m_pc = hl();
					m_last = 1;
					break;
				default:
					THROW_GB_CPU_EXCEPTION_MESSAGE(GB_CPU_EXCEPTION_INVALID_CODE,
						"%s:0x%x", GB_CMD_STRING(GB_CMD_JP), code);
			}
		}

		void 
		_gb_cpu::execute_cmd_jr(
			__in gbb_t code
			)
		{
			int8_t off = m_mmu->read_byte(m_pc++);

			switch(code) {
				case GB_CODE_JR_N:
					m_pc += off;
					break;
				case GB_CODE_JR_NZ_N:

					if(!(m_rf & GB_FLAG_Z)) {
						m_pc += off;
					}
					break;		
				case GB_CODE_JR_Z_N:

					if(m_rf & GB_FLAG_Z) {
						m_pc += off;
					}
					break;
				case GB_CODE_JR_NC_N:

					if(!(m_rf & GB_FLAG_C)) {
						m_pc += off;
					}
					break;
				case GB_CODE_JR_C_N:

					if(m_rf & GB_FLAG_C) {
						m_pc += off;
					}
					break;
				default:
					THROW_GB_CPU_EXCEPTION_MESSAGE(GB_CPU_EXCEPTION_INVALID_CODE,
						"%s:0x%x", GB_CMD_STRING(GB_CMD_JR), code);
			}

			m_last = 2;
		}

		void 
		_gb_cpu::execute_cmd_ld(
			__in gbb_t code
			)
		{

			switch(code) {
				case GB_CODE_LD_B_N:
					m_rb = m_mmu->read_byte(m_pc++);
					m_last = 2;
					break;
				case GB_CODE_LD_C_N:
					m_rc = m_mmu->read_byte(m_pc++);
					m_last = 2;
					break;
				case GB_CODE_LD_D_N:
					m_rd = m_mmu->read_byte(m_pc++);
					m_last = 2;
					break;
				case GB_CODE_LD_E_N:
					m_re = m_mmu->read_byte(m_pc++);
					m_last = 2;
					break;
				case GB_CODE_LD_H_N:
					m_rh = m_mmu->read_byte(m_pc++);
					m_last = 2;
					break;
				case GB_CODE_LD_L_N:
					m_rl = m_mmu->read_byte(m_pc++);
					m_last = 2;
					break;
				case GB_CODE_LD_A_A:
					m_last = 1;
					break;
				case GB_CODE_LD_A_B:
					m_ra = m_rb;
					m_last = 1;
					break;
				case GB_CODE_LD_A_C:
					m_ra = m_rc;
					m_last = 1;
					break;
				case GB_CODE_LD_A_D:
					m_ra = m_rd;
					m_last = 1;
					break;
				case GB_CODE_LD_A_E:
					m_ra = m_re;
					m_last = 1;
					break;
				case GB_CODE_LD_A_H:
					m_ra = m_rh;
					m_last = 1;
					break;
				case GB_CODE_LD_A_L:
					m_ra = m_rl;
					m_last = 1;
					break;
				case GB_CODE_LD_A_HL_INDIRECT:
					m_ra = m_mmu->read_byte(hl());
					m_last = 2;
					break;
				case GB_CODE_LD_B_B:
					m_last = 1;
					break;
				case GB_CODE_LD_B_C:
					m_rb = m_rc;
					m_last = 1;
					break;
				case GB_CODE_LD_B_D:
					m_rb = m_rd;
					m_last = 1;
					break;
				case GB_CODE_LD_B_E:
					m_rb = m_re;
					m_last = 1;
					break;
				case GB_CODE_LD_B_H:
					m_rb = m_rh;
					m_last = 1;
					break;
				case GB_CODE_LD_B_L:
					m_rb = m_rl;
					m_last = 1;
					break;
				case GB_CODE_LD_B_HL_INDIRECT:
					m_rb = m_mmu->read_byte(hl());
					m_last = 2;
					break;
				case GB_CODE_LD_C_B:
					m_rc = m_rb;
					m_last = 1;
					break;
				case GB_CODE_LD_C_C:
					m_last = 1;
					break;
				case GB_CODE_LD_C_D:
					m_rc = m_rd;
					m_last = 1;
					break;
				case GB_CODE_LD_C_E:
					m_rc = m_re;
					m_last = 1;
					break;
				case GB_CODE_LD_C_H:
					m_rc = m_rh;
					m_last = 1;
					break;		
				case GB_CODE_LD_C_L:
					m_rc = m_rl;
					m_last = 1;
					break;
				case GB_CODE_LD_C_HL_INDIRECT:
					m_rc = m_mmu->read_byte(hl());
					m_last = 2;
					break;
				case GB_CODE_LD_D_B:
					m_rd = m_rb;
					m_last = 1;
					break;
				case GB_CODE_LD_D_C:
					m_rd = m_rc;
					m_last = 1;
					break;
				case GB_CODE_LD_D_D:
					m_last = 1;
					break;
				case GB_CODE_LD_D_E:
					m_rd = m_re;
					m_last = 1;
					break;
				case GB_CODE_LD_D_H:
					m_rd = m_rh;
					m_last = 1;
					break;		
				case GB_CODE_LD_D_L:
					m_rd = m_rl;
					m_last = 1;
					break;
				case GB_CODE_LD_D_HL_INDIRECT:
					m_rd = m_mmu->read_byte(hl());
					m_last = 2;
					break;
				case GB_CODE_LD_E_B:
					m_re = m_rb;
					m_last = 1;
					break;
				case GB_CODE_LD_E_C:
					m_re = m_rc;
					m_last = 1;
					break;
				case GB_CODE_LD_E_D:
					m_re = m_rd;
					m_last = 1;
					break;
				case GB_CODE_LD_E_E:
					m_last = 1;
					break;
				case GB_CODE_LD_E_H:
					m_re = m_rh;
					m_last = 1;
					break;		
				case GB_CODE_LD_E_L:
					m_re = m_rl;
					m_last = 1;
					break;
				case GB_CODE_LD_E_HL_INDIRECT:
					m_re = m_mmu->read_byte(hl());
					m_last = 2;
					break;
				case GB_CODE_LD_H_B:
					m_rh = m_rb;
					m_last = 1;
					break;
				case GB_CODE_LD_H_C:
					m_rh = m_rc;
					m_last = 1;
					break;
				case GB_CODE_LD_H_D:
					m_rh = m_rd;
					m_last = 1;
					break;
				case GB_CODE_LD_H_E:
					m_rh = m_re;
					m_last = 1;
					break;
				case GB_CODE_LD_H_H:
					m_last = 1;
					break;		
				case GB_CODE_LD_H_L:
					m_rh = m_rl;
					m_last = 1;
					break;
				case GB_CODE_LD_H_HL_INDIRECT:
					m_rh = m_mmu->read_byte(hl());
					m_last = 2;
					break;
				case GB_CODE_LD_L_B:
					m_rl = m_rb;
					m_last = 1;
					break;
				case GB_CODE_LD_L_C:
					m_rl = m_rc;
					m_last = 1;
					break;
				case GB_CODE_LD_L_D:
					m_rl = m_rd;
					m_last = 1;
					break;
				case GB_CODE_LD_L_E:
					m_rl = m_re;
					m_last = 1;
					break;
				case GB_CODE_LD_L_H:
					m_rl = m_rh;
					m_last = 1;
					break;
				case GB_CODE_LD_L_L:
					m_last = 1;
					break;
				case GB_CODE_LD_L_HL_INDIRECT:
					m_rl = m_mmu->read_byte(hl());
					m_last = 2;
					break;
				case GB_CODE_LD_HL_INDIRECT_B:
					m_mmu->write_byte(hl(), m_rb);
					m_last = 2;
					break;
				case GB_CODE_LD_HL_INDIRECT_C:
					m_mmu->write_byte(hl(), m_rc);
					m_last = 2;
					break;
				case GB_CODE_LD_HL_INDIRECT_D:
					m_mmu->write_byte(hl(), m_rd);
					m_last = 2;
					break;
				case GB_CODE_LD_HL_INDIRECT_E:
					m_mmu->write_byte(hl(), m_re);
					m_last = 2;
					break;
				case GB_CODE_LD_HL_INDIRECT_H:
					m_mmu->write_byte(hl(), m_rh);
					m_last = 2;
					break;
				case GB_CODE_LD_HL_INDIRECT_L:
					m_mmu->write_byte(hl(), m_rl);
					m_last = 2;
					break;	
				case GB_CODE_LD_HL_INDIRECT_N:
					m_mmu->write_byte(hl(), m_mmu->read_byte(m_pc++));
					m_last = 3;
					break;
				case GB_CODE_LD_A_BC_INDIRECT:
					m_ra = m_mmu->read_byte(bc());
					m_last = 2;
					break;
				case GB_CODE_LD_A_DE_INDIRECT:
					m_ra = m_mmu->read_byte(de());
					m_last = 2;
					break;
				case GB_CODE_LD_A_NN_INDIRECT:
					m_ra = m_mmu->read_byte(m_mmu->read_word(m_pc));
					m_pc += sizeof(gbw_t);
					m_last = 4;
					break;
				case GB_CODE_LD_A_N:
					m_ra = m_mmu->read_byte(m_pc++);
					m_last = 2;
					break;
				case GB_CODE_LD_B_A:
					m_rb = m_ra;
					m_last = 1;
					break;
				case GB_CODE_LD_C_A:
					m_rc = m_ra;
					m_last = 1;
					break;
				case GB_CODE_LD_D_A:
					m_rd = m_ra;
					m_last = 1;
					break;
				case GB_CODE_LD_E_A:
					m_re = m_ra;
					m_last = 1;
					break;
				case GB_CODE_LD_H_A:
					m_rh = m_ra;
					m_last = 1;
					break;
				case GB_CODE_LD_L_A:
					m_rl = m_ra;
					m_last = 1;
					break;
				case GB_CODE_LD_BC_INDIRECT_A:
					m_mmu->write_byte(m_mmu->read_byte(bc()), m_ra);
					m_last = 2;
					break;
				case GB_CODE_LD_DE_INDIRECT_A:
					m_mmu->write_byte(m_mmu->read_byte(de()), m_ra);
					m_last = 2;
					break;
				case GB_CODE_LD_HL_INDIRECT_A:
					m_mmu->write_byte(m_mmu->read_byte(hl()), m_ra);
					m_last = 2;
					break;
				case GB_CODE_LD_NN_INDIRECT_A:
					m_mmu->write_byte(m_mmu->read_byte(m_mmu->read_word(m_pc)), m_ra);
					m_pc += sizeof(gbw_t);
					m_last = 4;
					break;
				case GB_CODE_LD_A_C_INDIRECT:
					m_ra = m_mmu->read_byte(m_rc + 0xff00);
					m_last = 2;
					break;
				case GB_CODE_LD_C_INDIRECT_A:
					m_mmu->write_byte(m_rc + 0xff00, m_ra);
					m_last = 2;
					break;
				case GB_CODE_LD_BC_NN:
					set_bc(m_mmu->read_word(m_pc));
					m_pc += sizeof(gbw_t);
					m_last = 3;
					break;
				case GB_CODE_LD_DE_NN:
					set_de(m_mmu->read_word(m_pc));
					m_pc += sizeof(gbw_t);
					m_last = 3;
					break;
				case GB_CODE_LD_HL_NN:
					set_hl(m_mmu->read_word(m_pc));
					m_pc += sizeof(gbw_t);
					m_last = 3;
					break;
				case GB_CODE_LD_SP_NN:
					m_sp = m_mmu->read_word(m_pc);
					m_pc += sizeof(gbw_t);
					m_last = 3;
					break;
				case GB_CODE_LD_SP_HL:
					m_sp = hl();
					m_last = 2;
					break;
				case GB_CODE_LD_NN_INDIRECT_SP:
					m_mmu->write_word(m_mmu->read_word(m_pc), m_sp);
					m_pc += sizeof(gbw_t);
					m_last = 5;
					break;
				default:
					THROW_GB_CPU_EXCEPTION_MESSAGE(GB_CPU_EXCEPTION_INVALID_CODE,
						"%s:0x%x", GB_CMD_STRING(GB_CMD_LD), code);
			}
		}

		void 
		_gb_cpu::execute_cmd_ldd(
			__in gbb_t code
			)
		{
			// TODO
		}

		void 
		_gb_cpu::execute_cmd_ldh(
			__in gbb_t code
			)
		{
			// TODO
		}

		void 
		_gb_cpu::execute_cmd_ldhl(
			__in gbb_t code
			)
		{
			// TODO
		}

		void 
		_gb_cpu::execute_cmd_ldi(
			__in gbb_t code
			)
		{
			// TODO
		}

		void 
		_gb_cpu::execute_cmd_nop(
			__in gbb_t code
			)
		{
			m_last = 1;
		}

		void 
		_gb_cpu::execute_cmd_or(
			__in gbb_t code
			)
		{
			m_rf = 0;

			switch(code) {
				case GB_CODE_OR_A:
					m_ra |= m_ra;
					m_last = 1;
					break;
				case GB_CODE_OR_B:
					m_ra |= m_rb;
					m_last = 1;
					break;
				case GB_CODE_OR_C:
					m_ra |= m_rc;
					m_last = 1;
					break;
				case GB_CODE_OR_D:
					m_ra |= m_rd;
					m_last = 1;
					break;
				case GB_CODE_OR_E:
					m_ra |= m_re;
					m_last = 1;
					break;
				case GB_CODE_OR_H:
					m_ra |= m_rh;
					m_last = 1;
					break;
				case GB_CODE_OR_L:
					m_ra |= m_rl;
					m_last = 1;
					break;
				case GB_CODE_OR_HL_INDIRECT:
					m_ra |= m_mmu->read_byte(hl());
					m_last = 2;
					break;
				case GB_CODE_OR_N:
					m_ra |= m_mmu->read_byte(m_pc++);
					m_last = 2;
					break;
				default:
					THROW_GB_CPU_EXCEPTION_MESSAGE(GB_CPU_EXCEPTION_INVALID_CODE,
						"%s:0x%x", GB_CMD_STRING(GB_CMD_OR), code);
			}

			if(!m_ra) {
				m_rf |= GB_FLAG_Z;
			}
		}

		void 
		_gb_cpu::execute_cmd_pop(
			__in gbb_t code
			)
		{

			switch(code) {
				case GB_CODE_POP_AF:
					set_af(m_mmu->read_word(m_sp));
					break;
				case GB_CODE_POP_BC:
					set_bc(m_mmu->read_word(m_sp));
					break;
				case GB_CODE_POP_DE:
					set_de(m_mmu->read_word(m_sp));
					break;
				case GB_CODE_POP_HL:
					set_hl(m_mmu->read_word(m_sp));
					break;
				default:
					THROW_GB_CPU_EXCEPTION_MESSAGE(GB_CPU_EXCEPTION_INVALID_CODE,
						"%s:0x%x", GB_CMD_STRING(GB_CMD_POP), code);
			}

			m_sp += sizeof(gbw_t);
			m_last = 3;
		}

		void 
		_gb_cpu::execute_cmd_push(
			__in gbb_t code
			)
		{

			switch(code) {
				case GB_CODE_PUSH_AF:
					m_mmu->write_word(m_sp, af());
					break;
				case GB_CODE_PUSH_BC:
					m_mmu->write_word(m_sp, bc());
					break;
				case GB_CODE_PUSH_DE:
					m_mmu->write_word(m_sp, de());
					break;
				case GB_CODE_PUSH_HL:
					m_mmu->write_word(m_sp, hl());
					break;
				default:
					THROW_GB_CPU_EXCEPTION_MESSAGE(GB_CPU_EXCEPTION_INVALID_CODE,
						"%s:0x%x", GB_CMD_STRING(GB_CMD_PUSH), code);
			}

			m_sp -= sizeof(gbw_t);
			m_last = 4;
		}

		void 
		_gb_cpu::execute_cmd_res(
			__in gbb_t code
			)
		{

			switch(code) {
				case GB_CODE_RES_0_A:
					m_ra &= ~0x1;
					m_last = 2;
					break;
				case GB_CODE_RES_0_B:
					m_rb &= ~0x1;
					m_last = 2;
					break;
				case GB_CODE_RES_0_C:
					m_rc &= ~0x1;
					m_last = 2;
					break;
				case GB_CODE_RES_0_D:
					m_rd &= ~0x1;
					m_last = 2;
					break;
				case GB_CODE_RES_0_E:
					m_re &= ~0x1;
					m_last = 2;
					break;
				case GB_CODE_RES_0_H:
					m_rh &= ~0x1;
					m_last = 2;
					break;
				case GB_CODE_RES_0_L:
					m_rl &= ~0x1;
					m_last = 2;
					break;
				case GB_CODE_RES_0_HL_INDIRECT:
					m_mmu->write_byte(hl(), m_mmu->read_byte(hl()) & ~0x1);
					m_last = 4;
					break;
				case GB_CODE_RES_1_A:
					m_ra &= ~0x2;
					m_last = 2;
					break;
				case GB_CODE_RES_1_B:
					m_rb &= ~0x2;
					m_last = 2;
					break;
				case GB_CODE_RES_1_C:
					m_rc &= ~0x2;
					m_last = 2;
					break;
				case GB_CODE_RES_1_D:
					m_rd &= ~0x2;
					m_last = 2;
					break;
				case GB_CODE_RES_1_E:
					m_re &= ~0x2;
					m_last = 2;
					break;
				case GB_CODE_RES_1_H:
					m_rh &= ~0x2;
					m_last = 2;
					break;
				case GB_CODE_RES_1_L:
					m_rl &= ~0x2;
					m_last = 2;
					break;
				case GB_CODE_RES_1_HL_INDIRECT:
					m_mmu->write_byte(hl(), m_mmu->read_byte(hl()) & ~0x2);
					m_last = 4;
					break;
				case GB_CODE_RES_2_A:
					m_ra &= ~0x4;
					m_last = 2;
					break;
				case GB_CODE_RES_2_B:
					m_rb &= ~0x4;
					m_last = 2;
					break;
				case GB_CODE_RES_2_C:
					m_rc &= ~0x4;
					m_last = 2;
					break;
				case GB_CODE_RES_2_D:
					m_rd &= ~0x4;
					m_last = 2;
					break;
				case GB_CODE_RES_2_E:
					m_re &= ~0x4;
					m_last = 2;
					break;
				case GB_CODE_RES_2_H:
					m_rh &= ~0x4;
					m_last = 2;
					break;
				case GB_CODE_RES_2_L:
					m_rl &= ~0x4;
					m_last = 2;
					break;
				case GB_CODE_RES_2_HL_INDIRECT:
					m_mmu->write_byte(hl(), m_mmu->read_byte(hl()) & ~0x4);
					m_last = 4;
					break;
				case GB_CODE_RES_3_A:
					m_ra &= ~0x8;
					m_last = 2;
					break;
				case GB_CODE_RES_3_B:
					m_rb &= ~0x8;
					m_last = 2;
					break;
				case GB_CODE_RES_3_C:
					m_rc &= ~0x8;
					m_last = 2;
					break;
				case GB_CODE_RES_3_D:
					m_rd &= ~0x8;
					m_last = 2;
					break;
				case GB_CODE_RES_3_E:
					m_re &= ~0x8;
					m_last = 2;
					break;
				case GB_CODE_RES_3_H:
					m_rh &= ~0x8;
					m_last = 2;
					break;
				case GB_CODE_RES_3_L:
					m_rl &= ~0x8;
					m_last = 2;
					break;
				case GB_CODE_RES_3_HL_INDIRECT:
					m_mmu->write_byte(hl(), m_mmu->read_byte(hl()) & ~0x8);
					m_last = 4;
					break;
				case GB_CODE_RES_4_A:
					m_ra &= ~0x10;
					m_last = 2;
					break;
				case GB_CODE_RES_4_B:
					m_rb &= ~0x10;
					m_last = 2;
					break;
				case GB_CODE_RES_4_C:
					m_rc &= ~0x10;
					m_last = 2;
					break;
				case GB_CODE_RES_4_D:
					m_rd &= ~0x10;
					m_last = 2;
					break;
				case GB_CODE_RES_4_E:
					m_re &= ~0x10;
					m_last = 2;
					break;
				case GB_CODE_RES_4_H:
					m_rh &= ~0x10;
					m_last = 2;
					break;
				case GB_CODE_RES_4_L:
					m_rl &= ~0x10;
					m_last = 2;
					break;
				case GB_CODE_RES_4_HL_INDIRECT:
					m_mmu->write_byte(hl(), m_mmu->read_byte(hl()) & ~0x10);
					m_last = 4;
					break;
				case GB_CODE_RES_5_A:
					m_ra &= ~0x20;
					m_last = 2;
					break;
				case GB_CODE_RES_5_B:
					m_rb &= ~0x20;
					m_last = 2;
					break;
				case GB_CODE_RES_5_C:
					m_rc &= ~0x20;
					m_last = 2;
					break;
				case GB_CODE_RES_5_D:
					m_rd &= ~0x20;
					m_last = 2;
					break;
				case GB_CODE_RES_5_E:
					m_re &= ~0x20;
					m_last = 2;
					break;
				case GB_CODE_RES_5_H:
					m_rh &= ~0x20;
					m_last = 2;
					break;
				case GB_CODE_RES_5_L:
					m_rl &= ~0x20;
					m_last = 2;
					break;
				case GB_CODE_RES_5_HL_INDIRECT:
					m_mmu->write_byte(hl(), m_mmu->read_byte(hl()) & ~0x20);
					m_last = 4;
					break;
				case GB_CODE_RES_6_A:
					m_ra &= ~0x40;
					m_last = 2;
					break;
				case GB_CODE_RES_6_B:
					m_rb &= ~0x40;
					m_last = 2;
					break;
				case GB_CODE_RES_6_C:
					m_rc &= ~0x40;
					m_last = 2;
					break;
				case GB_CODE_RES_6_D:
					m_rd &= ~0x40;
					m_last = 2;
					break;
				case GB_CODE_RES_6_E:
					m_re &= ~0x40;
					m_last = 2;
					break;
				case GB_CODE_RES_6_H:
					m_rh &= ~0x40;
					m_last = 2;
					break;
				case GB_CODE_RES_6_L:
					m_rl &= ~0x40;
					m_last = 2;
					break;
				case GB_CODE_RES_6_HL_INDIRECT:
					m_mmu->write_byte(hl(), m_mmu->read_byte(hl()) & ~0x40);
					m_last = 4;
					break;
				case GB_CODE_RES_7_A:
					m_ra &= ~0x80;
					m_last = 2;
					break;
				case GB_CODE_RES_7_B:
					m_rb &= ~0x80;
					m_last = 2;
					break;
				case GB_CODE_RES_7_C:
					m_rc &= ~0x80;
					m_last = 2;
					break;
				case GB_CODE_RES_7_D:
					m_rd &= ~0x80;
					m_last = 2;
					break;
				case GB_CODE_RES_7_E:
					m_re &= ~0x80;
					m_last = 2;
					break;
				case GB_CODE_RES_7_H:
					m_rh &= ~0x80;
					m_last = 2;
					break;
				case GB_CODE_RES_7_L:
					m_rl &= ~0x80;
					m_last = 2;
					break;
				case GB_CODE_RES_7_HL_INDIRECT:
					m_mmu->write_byte(hl(), m_mmu->read_byte(hl()) & ~0x80);
					m_last = 4;
					break;
				default:
					THROW_GB_CPU_EXCEPTION_MESSAGE(GB_CPU_EXCEPTION_INVALID_CODE,
						"%s:0x%x", GB_CMD_STRING(GB_CMD_RES), code);
			}
		}

		void 
		_gb_cpu::execute_cmd_ret(
			__in gbb_t code
			)
		{

			switch(code) {
				case GB_CODE_RET_S:
					m_pc = m_mmu->read_word(m_sp);
					m_sp += sizeof(gbw_t);
					break;
				case GB_CODE_RET_NZ:

					if(!(m_rf & GB_FLAG_Z)) {
						m_pc = m_mmu->read_word(m_sp);
						m_sp += sizeof(gbw_t);
					}

					break;
				case GB_CODE_RET_Z:

					if(m_rf & GB_FLAG_Z) {
						m_pc = m_mmu->read_word(m_sp);
						m_sp += sizeof(gbw_t);
					}

					break;
				case GB_CODE_RET_NC:

					if(!(m_rf & GB_FLAG_C)) {
						m_pc = m_mmu->read_word(m_sp);
						m_sp += sizeof(gbw_t);
					}

					break;
				case GB_CODE_RET_C:

					if(m_rf & GB_FLAG_C) {
						m_pc = m_mmu->read_word(m_sp);
						m_sp += sizeof(gbw_t);
					}

					break;
				default:
					THROW_GB_CPU_EXCEPTION_MESSAGE(GB_CPU_EXCEPTION_INVALID_CODE,
						"%s:0x%x", GB_CMD_STRING(GB_CMD_RET), code);
			}

			m_last = 2;
		}

		void 
		_gb_cpu::execute_cmd_reti(
			__in gbb_t code
			)
		{
			m_pc = m_mmu->read_word(m_sp);
			m_sp += sizeof(gbw_t);
			m_ime = true;
			m_last = 2;
		}

		void 
		_gb_cpu::execute_cmd_rl(
			__in gbb_t code
			)
		{
			gbb_t carry = (m_rf & GB_FLAG_C) ? 1 : 0, val;

			m_rf = 0;

			switch(code) {
				case GB_CODE_RL_A:

					if(m_ra & 0x80) {
						m_rf |= GB_FLAG_C;
					}

					m_ra <<= 1;
					m_ra += carry;

					if(!m_ra) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_RL_B:

					if(m_rb & 0x80) {
						m_rf |= GB_FLAG_C;
					}

					m_rb <<= 1;
					m_rb += carry;

					if(!m_rb) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_RL_C:

					if(m_rc & 0x80) {
						m_rf |= GB_FLAG_C;
					}

					m_rc <<= 1;
					m_rc += carry;

					if(!m_rc) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_RL_D:

					if(m_rd & 0x80) {
						m_rf |= GB_FLAG_C;
					}

					m_rd <<= 1;
					m_rd += carry;

					if(!m_rd) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_RL_E:

					if(m_re & 0x80) {
						m_rf |= GB_FLAG_C;
					}

					m_re <<= 1;
					m_re += carry;

					if(!m_re) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_RL_H:

					if(m_rh & 0x80) {
						m_rf |= GB_FLAG_C;
					}

					m_rh <<= 1;
					m_rh += carry;

					if(!m_rh) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;		
				case GB_CODE_RL_L:

					if(m_rl & 0x80) {
						m_rf |= GB_FLAG_C;
					}

					m_rl <<= 1;
					m_rl += carry;

					if(!m_rl) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_RL_HL_INDIRECT:

					val = m_mmu->read_byte(hl());
					if(val & 0x80) {
						m_rf |= GB_FLAG_C;
					}

					val <<= 1;
					val += carry;

					if(!val) {
						m_rf |= GB_FLAG_Z;
					}

					m_mmu->write_byte(hl(), val);
					m_last = 4;
					break;
				default:
					THROW_GB_CPU_EXCEPTION_MESSAGE(GB_CPU_EXCEPTION_INVALID_CODE,
						"%s:0x%x", GB_CMD_STRING(GB_CMD_RL), code);
			}
		}

		void 
		_gb_cpu::execute_cmd_rlc(
			__in gbb_t code
			)
		{
			gbb_t carry = 0, val;

			m_rf = 0;

			switch(code) {
				case GB_CODE_RLC_A:

					if(m_ra & 0x80) {
						m_rf |= GB_FLAG_C;
						carry = 1;
					}

					m_ra <<= 1;
					m_ra += carry;

					if(!m_ra) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_RLC_B:

					if(m_rb & 0x80) {
						m_rf |= GB_FLAG_C;
						carry = 1;
					}

					m_rb <<= 1;
					m_rb += carry;

					if(!m_rb) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_RLC_C:

					if(m_rc & 0x80) {
						m_rf |= GB_FLAG_C;
						carry = 1;
					}

					m_rc <<= 1;
					m_rc += carry;

					if(!m_rc) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_RLC_D:

					if(m_rd & 0x80) {
						m_rf |= GB_FLAG_C;
						carry = 1;
					}

					m_rd <<= 1;
					m_rd += carry;

					if(!m_rd) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_RLC_E:

					if(m_re & 0x80) {
						m_rf |= GB_FLAG_C;
						carry = 1;
					}

					m_re <<= 1;
					m_re += carry;

					if(!m_re) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_RLC_H:

					if(m_rh & 0x80) {
						m_rf |= GB_FLAG_C;
						carry = 1;
					}

					m_rh <<= 1;
					m_rh += carry;

					if(!m_rh) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;		
				case GB_CODE_RLC_L:

					if(m_rl & 0x80) {
						m_rf |= GB_FLAG_C;
						carry = 1;
					}

					m_rl <<= 1;
					m_rl += carry;

					if(!m_rl) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_RLC_HL_INDIRECT:

					val = m_mmu->read_byte(hl());
					if(val & 0x80) {
						m_rf |= GB_FLAG_C;
						carry = 1;
					}

					val <<= 1;
					val += carry;

					if(!val) {
						m_rf |= GB_FLAG_Z;
					}

					m_mmu->write_byte(hl(), val);
					m_last = 4;
					break;
				default:
					THROW_GB_CPU_EXCEPTION_MESSAGE(GB_CPU_EXCEPTION_INVALID_CODE,
						"%s:0x%x", GB_CMD_STRING(GB_CMD_RLC), code);
			}
		}

		void 
		_gb_cpu::execute_cmd_rr(
			__in gbb_t code
			)
		{
			gbb_t carry = (m_rf & GB_FLAG_C) ? 0x80 : 0, val;

			m_rf = 0;

			switch(code) {
				case GB_CODE_RR_A:

					if(m_ra & 1) {
						m_rf |= GB_FLAG_C;
					}

					m_ra >>= 1;
					m_ra += carry;

					if(!m_ra) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_RR_B:

					if(m_rb & 1) {
						m_rf |= GB_FLAG_C;
					}

					m_rb >>= 1;
					m_rb += carry;

					if(!m_rb) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_RR_C:

					if(m_rc & 1) {
						m_rf |= GB_FLAG_C;
					}

					m_rc >>= 1;
					m_rc += carry;

					if(!m_rc) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_RR_D:

					if(m_rd & 1) {
						m_rf |= GB_FLAG_C;
					}

					m_rd >>= 1;
					m_rd += carry;

					if(!m_rd) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_RR_E:

					if(m_re & 1) {
						m_rf |= GB_FLAG_C;
					}

					m_re >>= 1;
					m_re += carry;

					if(!m_re) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_RR_H:

					if(m_rh & 1) {
						m_rf |= GB_FLAG_C;
					}

					m_rh >>= 1;
					m_rh += carry;

					if(!m_rh) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;		
				case GB_CODE_RR_L:

					if(m_rl & 1) {
						m_rf |= GB_FLAG_C;
					}

					m_rl >>= 1;
					m_rl += carry;

					if(!m_rl) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_RR_HL_INDIRECT:

					val = m_mmu->read_byte(hl());
					if(val & 1) {
						m_rf |= GB_FLAG_C;
					}

					val >>= 1;
					val += carry;

					if(!val) {
						m_rf |= GB_FLAG_Z;
					}

					m_mmu->write_byte(hl(), val);
					m_last = 4;
					break;
				default:
					THROW_GB_CPU_EXCEPTION_MESSAGE(GB_CPU_EXCEPTION_INVALID_CODE,
						"%s:0x%x", GB_CMD_STRING(GB_CMD_RR), code);
			}
		}

		void 
		_gb_cpu::execute_cmd_rrc(
			__in gbb_t code
			)
		{
			gbb_t carry = 0, val;

			m_rf = 0;

			switch(code) {
				case GB_CODE_RRC_A:

					if(m_ra & 1) {
						m_rf |= GB_FLAG_C;
						carry = 0x80;
					}

					m_ra >>= 1;
					m_ra += carry;

					if(!m_ra) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_RRC_B:

					if(m_rb & 1) {
						m_rf |= GB_FLAG_C;
						carry = 0x80;
					}

					m_rb >>= 1;
					m_rb += carry;

					if(!m_rb) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_RRC_C:

					if(m_rc & 1) {
						m_rf |= GB_FLAG_C;
						carry = 0x80;
					}

					m_rc >>= 1;
					m_rc += carry;

					if(!m_rc) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_RRC_D:

					if(m_rd & 1) {
						m_rf |= GB_FLAG_C;
						carry = 0x80;
					}

					m_rd >>= 1;
					m_rd += carry;

					if(!m_rd) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_RRC_E:

					if(m_re & 1) {
						m_rf |= GB_FLAG_C;
						carry = 0x80;
					}

					m_re >>= 1;
					m_re += carry;

					if(!m_re) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_RRC_H:

					if(m_rh & 1) {
						m_rf |= GB_FLAG_C;
						carry = 0x80;
					}

					m_rh >>= 1;
					m_rh += carry;

					if(!m_rh) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;		
				case GB_CODE_RRC_L:

					if(m_rl & 1) {
						m_rf |= GB_FLAG_C;
						carry = 0x80;
					}

					m_rl >>= 1;
					m_rl += carry;

					if(!m_rl) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_RRC_HL_INDIRECT:

					val = m_mmu->read_byte(hl());
					if(val & 1) {
						m_rf |= GB_FLAG_C;
						carry = 0x80;
					}

					val >>= 1;
					val += carry;

					if(!val) {
						m_rf |= GB_FLAG_Z;
					}

					m_mmu->write_byte(hl(), val);
					m_last = 4;
					break;
				default:
					THROW_GB_CPU_EXCEPTION_MESSAGE(GB_CPU_EXCEPTION_INVALID_CODE,
						"%s:0x%x", GB_CMD_STRING(GB_CMD_RRC), code);
			}
		}

		void 
		_gb_cpu::execute_cmd_rst(
			__in gbb_t code
			)
		{
			m_mmu->write_word(m_sp, m_pc);
			m_pc += sizeof(gbw_t);
			m_sp -= sizeof(gbw_t);

			switch(code) {
				case GB_CODE_RST_0:
					m_pc = GB_RST_VEC_0;
					break;
				case GB_CODE_RST_8:
					m_pc = GB_RST_VEC_8;
					break;
				case GB_CODE_RST_10:
					m_pc = GB_RST_VEC_10;
					break;
				case GB_CODE_RST_18:
					m_pc = GB_RST_VEC_18;
					break;
				case GB_CODE_RST_20:
					m_pc = GB_RST_VEC_20;
					break;
				case GB_CODE_RST_28:
					m_pc = GB_RST_VEC_28;
					break;
				case GB_CODE_RST_30:
					m_pc = GB_RST_VEC_30;
					break;
				case GB_CODE_RST_38:
					m_pc = GB_RST_VEC_38;
					break;
				default:
					THROW_GB_CPU_EXCEPTION_MESSAGE(GB_CPU_EXCEPTION_INVALID_CODE,
						"%s:0x%x", GB_CMD_STRING(GB_CMD_RST), code);
			}

			m_last = 8;
		}

		void 
		_gb_cpu::execute_cmd_sbc(
			__in gbb_t code
			)
		{
			gbb_t carry = (m_rf & GB_FLAG_C) ? 1 : 0, res, val;

			m_rf = GB_FLAG_N;

			switch(code) {
				case GB_CODE_SBC_A_A:
					val = m_ra + carry;

					res = m_ra - val;
					if(!res) {
						m_rf |= GB_FLAG_Z;
					}

					if(DETERMINE_HALF_CARRY_BYTE(res, m_ra, val)) {
						m_rf |= GB_FLAG_H;
					}

					m_ra = res;
					m_last = 1;
					break;
				case GB_CODE_SBC_A_B:

					val = m_rb + carry;
					if(m_ra < val) {
						m_rf |= GB_FLAG_C;
					}

					res = m_ra - val;
					if(!res) {
						m_rf |= GB_FLAG_Z;
					}

					if(DETERMINE_HALF_CARRY_BYTE(res, m_ra, val)) {
						m_rf |= GB_FLAG_H;
					}

					m_ra = res;
					m_last = 1;
					break;
				case GB_CODE_SBC_A_C:

					val = m_rc + carry;
					if(m_ra < val) {
						m_rf |= GB_FLAG_C;
					}

					res = m_ra - val;
					if(!res) {
						m_rf |= GB_FLAG_Z;
					}

					if(DETERMINE_HALF_CARRY_BYTE(res, m_ra, val)) {
						m_rf |= GB_FLAG_H;
					}

					m_ra = res;
					m_last = 1;
					break;
				case GB_CODE_SBC_A_D:

					val = m_rd + carry;
					if(m_ra < val) {
						m_rf |= GB_FLAG_C;
					}

					res = m_ra - val;
					if(!res) {
						m_rf |= GB_FLAG_Z;
					}

					if(DETERMINE_HALF_CARRY_BYTE(res, m_ra, val)) {
						m_rf |= GB_FLAG_H;
					}

					m_ra = res;
					m_last = 1;
					break;
				case GB_CODE_SBC_A_E:

					val = m_re + carry;
					if(m_ra < val) {
						m_rf |= GB_FLAG_C;
					}

					res = m_ra - val;
					if(!res) {
						m_rf |= GB_FLAG_Z;
					}

					if(DETERMINE_HALF_CARRY_BYTE(res, m_ra, val)) {
						m_rf |= GB_FLAG_H;
					}

					m_ra = res;
					m_last = 1;
					break;
				case GB_CODE_SBC_A_H:

					val = m_rh + carry;
					if(m_ra < val) {
						m_rf |= GB_FLAG_C;
					}

					res = m_ra - val;
					if(!res) {
						m_rf |= GB_FLAG_Z;
					}

					if(DETERMINE_HALF_CARRY_BYTE(res, m_ra, val)) {
						m_rf |= GB_FLAG_H;
					}

					m_ra = res;
					m_last = 1;
					break;
				case GB_CODE_SBC_A_L:

					val = m_rl + carry;
					if(m_ra < val) {
						m_rf |= GB_FLAG_C;
					}

					res = m_ra - val;
					if(!res) {
						m_rf |= GB_FLAG_Z;
					}

					if(DETERMINE_HALF_CARRY_BYTE(res, m_ra, val)) {
						m_rf |= GB_FLAG_H;
					}

					m_ra = res;
					m_last = 1;
					break;
				case GB_CODE_SBC_A_HL_INDIRECT:
					val = m_mmu->read_byte(hl()) + carry;

					if(m_ra < val) {
						m_rf |= GB_FLAG_C;
					}

					res = m_ra - val;
					if(!res) {
						m_rf |= GB_FLAG_Z;
					}

					if(DETERMINE_HALF_CARRY_BYTE(res, m_ra, val)) {
						m_rf |= GB_FLAG_H;
					}

					m_ra = res;
					m_last = 2;
					break;
				case GB_CODE_SBC_A_N:
					val = m_mmu->read_byte(m_pc++) + carry;

					if(m_ra < val) {
						m_rf |= GB_FLAG_C;
					}

					res = m_ra - val;
					if(!res) {
						m_rf |= GB_FLAG_Z;
					}

					if(DETERMINE_HALF_CARRY_BYTE(res, m_ra, val)) {
						m_rf |= GB_FLAG_H;
					}

					m_ra = res;
					m_last = 2;
					break;
				default:
					THROW_GB_CPU_EXCEPTION_MESSAGE(GB_CPU_EXCEPTION_INVALID_CODE,
						"%s:0x%x", GB_CMD_STRING(GB_CMD_SBC), code);
			}
		}

		void 
		_gb_cpu::execute_cmd_scf(
			__in gbb_t code
			)
		{
			m_rf &= ~(GB_FLAG_H | GB_FLAG_N);
			m_rf |= GB_FLAG_C;
			m_last = 1;
		}

		void 
		_gb_cpu::execute_cmd_set(
			__in gbb_t code
			)
		{

			switch(code) {
				case GB_CODE_SET_0_A:
					m_ra |= 0x1;
					m_last = 2;
					break;
				case GB_CODE_SET_0_B:
					m_rb |= 0x1;
					m_last = 2;
					break;
				case GB_CODE_SET_0_C:
					m_rc |= 0x1;
					m_last = 2;
					break;
				case GB_CODE_SET_0_D:
					m_rd |= 0x1;
					m_last = 2;
					break;
				case GB_CODE_SET_0_E:
					m_re |= 0x1;
					m_last = 2;
					break;
				case GB_CODE_SET_0_H:
					m_rh |= 0x1;
					m_last = 2;
					break;
				case GB_CODE_SET_0_L:
					m_rl |= 0x1;
					m_last = 2;
					break;
				case GB_CODE_SET_0_HL_INDIRECT:
					m_mmu->write_byte(hl(), m_mmu->read_byte(hl()) | 0x1);
					m_last = 4;
					break;
				case GB_CODE_SET_1_A:
					m_ra |= 0x2;
					m_last = 2;
					break;
				case GB_CODE_SET_1_B:
					m_rb |= 0x2;
					m_last = 2;
					break;
				case GB_CODE_SET_1_C:
					m_rc |= 0x2;
					m_last = 2;
					break;
				case GB_CODE_SET_1_D:
					m_rd |= 0x2;
					m_last = 2;
					break;
				case GB_CODE_SET_1_E:
					m_re |= 0x2;
					m_last = 2;
					break;
				case GB_CODE_SET_1_H:
					m_rh |= 0x2;
					m_last = 2;
					break;
				case GB_CODE_SET_1_L:
					m_rl |= 0x2;
					m_last = 2;
					break;
				case GB_CODE_SET_1_HL_INDIRECT:
					m_mmu->write_byte(hl(), m_mmu->read_byte(hl()) | 0x2);
					m_last = 4;
					break;
				case GB_CODE_SET_2_A:
					m_ra |= 0x4;
					m_last = 2;
					break;
				case GB_CODE_SET_2_B:
					m_rb |= 0x4;
					m_last = 2;
					break;
				case GB_CODE_SET_2_C:
					m_rc |= 0x4;
					m_last = 2;
					break;
				case GB_CODE_SET_2_D:
					m_rd |= 0x4;
					m_last = 2;
					break;
				case GB_CODE_SET_2_E:
					m_re |= 0x4;
					m_last = 2;
					break;
				case GB_CODE_SET_2_H:
					m_rh |= 0x4;
					m_last = 2;
					break;
				case GB_CODE_SET_2_L:
					m_rl |= 0x4;
					m_last = 2;
					break;
				case GB_CODE_SET_2_HL_INDIRECT:
					m_mmu->write_byte(hl(), m_mmu->read_byte(hl()) | 0x4);
					m_last = 4;
					break;
				case GB_CODE_SET_3_A:
					m_ra |= 0x8;
					m_last = 2;
					break;
				case GB_CODE_SET_3_B:
					m_rb |= 0x8;
					m_last = 2;
					break;
				case GB_CODE_SET_3_C:
					m_rc |= 0x8;
					m_last = 2;
					break;
				case GB_CODE_SET_3_D:
					m_rd |= 0x8;
					m_last = 2;
					break;
				case GB_CODE_SET_3_E:
					m_re |= 0x8;
					m_last = 2;
					break;
				case GB_CODE_SET_3_H:
					m_rh |= 0x8;
					m_last = 2;
					break;
				case GB_CODE_SET_3_L:
					m_rl |= 0x8;
					m_last = 2;
					break;
				case GB_CODE_SET_3_HL_INDIRECT:
					m_mmu->write_byte(hl(), m_mmu->read_byte(hl()) | 0x8);
					m_last = 4;
					break;
				case GB_CODE_SET_4_A:
					m_ra |= 0x10;
					m_last = 2;
					break;
				case GB_CODE_SET_4_B:
					m_rb |= 0x10;
					m_last = 2;
					break;
				case GB_CODE_SET_4_C:
					m_rc |= 0x10;
					m_last = 2;
					break;
				case GB_CODE_SET_4_D:
					m_rd |= 0x10;
					m_last = 2;
					break;
				case GB_CODE_SET_4_E:
					m_re |= 0x10;
					m_last = 2;
					break;
				case GB_CODE_SET_4_H:
					m_rh |= 0x10;
					m_last = 2;
					break;
				case GB_CODE_SET_4_L:
					m_rl |= 0x10;
					m_last = 2;
					break;
				case GB_CODE_SET_4_HL_INDIRECT:
					m_mmu->write_byte(hl(), m_mmu->read_byte(hl()) | 0x10);
					m_last = 4;
					break;
				case GB_CODE_SET_5_A:
					m_ra |= 0x20;
					m_last = 2;
					break;
				case GB_CODE_SET_5_B:
					m_rb |= 0x20;
					m_last = 2;
					break;
				case GB_CODE_SET_5_C:
					m_rc |= 0x20;
					m_last = 2;
					break;
				case GB_CODE_SET_5_D:
					m_rd |= 0x20;
					m_last = 2;
					break;
				case GB_CODE_SET_5_E:
					m_re |= 0x20;
					m_last = 2;
					break;
				case GB_CODE_SET_5_H:
					m_rh |= 0x20;
					m_last = 2;
					break;
				case GB_CODE_SET_5_L:
					m_rl |= 0x20;
					m_last = 2;
					break;
				case GB_CODE_SET_5_HL_INDIRECT:
					m_mmu->write_byte(hl(), m_mmu->read_byte(hl()) | 0x20);
					m_last = 4;
					break;
				case GB_CODE_SET_6_A:
					m_ra |= 0x40;
					m_last = 2;
					break;
				case GB_CODE_SET_6_B:
					m_rb |= 0x40;
					m_last = 2;
					break;
				case GB_CODE_SET_6_C:
					m_rc |= 0x40;
					m_last = 2;
					break;
				case GB_CODE_SET_6_D:
					m_rd |= 0x40;
					m_last = 2;
					break;
				case GB_CODE_SET_6_E:
					m_re |= 0x40;
					m_last = 2;
					break;
				case GB_CODE_SET_6_H:
					m_rh |= 0x40;
					m_last = 2;
					break;
				case GB_CODE_SET_6_L:
					m_rl |= 0x40;
					m_last = 2;
					break;
				case GB_CODE_SET_6_HL_INDIRECT:
					m_mmu->write_byte(hl(), m_mmu->read_byte(hl()) | 0x40);
					m_last = 4;
					break;
				case GB_CODE_SET_7_A:
					m_ra |= 0x80;
					m_last = 2;
					break;
				case GB_CODE_SET_7_B:
					m_rb |= 0x80;
					m_last = 2;
					break;
				case GB_CODE_SET_7_C:
					m_rc |= 0x80;
					m_last = 2;
					break;
				case GB_CODE_SET_7_D:
					m_rd |= 0x80;
					m_last = 2;
					break;
				case GB_CODE_SET_7_E:
					m_re |= 0x80;
					m_last = 2;
					break;
				case GB_CODE_SET_7_H:
					m_rh |= 0x80;
					m_last = 2;
					break;
				case GB_CODE_SET_7_L:
					m_rl |= 0x80;
					m_last = 2;
					break;
				case GB_CODE_SET_7_HL_INDIRECT:
					m_mmu->write_byte(hl(), m_mmu->read_byte(hl()) | 0x80);
					m_last = 4;
					break;
				default:
					THROW_GB_CPU_EXCEPTION_MESSAGE(GB_CPU_EXCEPTION_INVALID_CODE,
						"%s:0x%x", GB_CMD_STRING(GB_CMD_SET), code);
			}
		}

		void 
		_gb_cpu::execute_cmd_sla(
			__in gbb_t code
			)
		{
			gbb_t val;

			m_rf = 0;

			switch(code) {
				case GB_CODE_SLA_A:

					if(m_ra & 0x80) {
						m_rf |= GB_FLAG_C;
					}

					m_ra <<= 1;
					if(!m_ra) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_SLA_B:

					if(m_rb & 0x80) {
						m_rf |= GB_FLAG_C;
					}

					m_rb <<= 1;
					if(!m_rb) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_SLA_C:

					if(m_rc & 0x80) {
						m_rf |= GB_FLAG_C;
					}

					m_rc <<= 1;
					if(!m_rc) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_SLA_D:

					if(m_rd & 0x80) {
						m_rf |= GB_FLAG_C;
					}

					m_rd <<= 1;
					if(!m_rd) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_SLA_E:

					if(m_re & 0x80) {
						m_rf |= GB_FLAG_C;
					}

					m_re <<= 1;
					if(!m_re) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_SLA_H:

					if(m_rh & 0x80) {
						m_rf |= GB_FLAG_C;
					}

					m_rh <<= 1;
					if(!m_rh) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_SLA_L:

					if(m_rl & 0x80) {
						m_rf |= GB_FLAG_C;
					}

					m_rl <<= 1;
					if(!m_rl) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_SLA_HL_INDIRECT:

					val = m_mmu->read_byte(hl());
					if(val & 0x80) {
						m_rf |= GB_FLAG_C;
					}

					val <<= 1;
					if(!val) {
						m_rf |= GB_FLAG_Z;
					}

					m_mmu->write_byte(hl(), val);
					m_last = 4;
					break;
				default:
					THROW_GB_CPU_EXCEPTION_MESSAGE(GB_CPU_EXCEPTION_INVALID_CODE,
						"%s:0x%x", GB_CMD_STRING(GB_CMD_SLA), code);
			}
		}

		void 
		_gb_cpu::execute_cmd_sra(
			__in gbb_t code
			)
		{
			gbb_t val;

			m_rf = 0;

			switch(code) {
				case GB_CODE_SRA_A:

					if(m_ra & 1) {
						m_rf |= GB_FLAG_C;
					}

					m_ra >>= 1;
					if(m_ra & 0x40) {
						m_ra |= 0x80;
					}

					if(!m_ra) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_SRA_B:

					if(m_rb & 1) {
						m_rf |= GB_FLAG_C;
					}

					m_rb >>= 1;
					if(m_rb & 0x40) {
						m_rb |= 0x80;
					}

					if(!m_rb) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_SRA_C:

					if(m_rc & 1) {
						m_rf |= GB_FLAG_C;
					}

					m_rc >>= 1;
					if(m_rc & 0x40) {
						m_rc |= 0x80;
					}

					if(!m_rc) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_SRA_D:

					if(m_rd & 1) {
						m_rf |= GB_FLAG_C;
					}

					m_rd >>= 1;
					if(m_rd & 0x40) {
						m_rd |= 0x80;
					}

					if(!m_rd) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_SRA_E:

					if(m_re & 1) {
						m_rf |= GB_FLAG_C;
					}

					m_re >>= 1;
					if(m_re & 0x40) {
						m_re |= 0x80;
					}

					if(!m_re) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_SRA_H:

					if(m_rh & 1) {
						m_rf |= GB_FLAG_C;
					}

					m_rh >>= 1;
					if(m_rh & 0x40) {
						m_rh |= 0x80;
					}

					if(!m_rh) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_SRA_L:

					if(m_rl & 1) {
						m_rf |= GB_FLAG_C;
					}

					m_rl >>= 1;
					if(m_rl & 0x40) {
						m_rl |= 0x80;
					}

					if(!m_rl) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_SRA_HL_INDIRECT:

					val = m_mmu->read_byte(hl());
					if(val & 1) {
						m_rf |= GB_FLAG_C;
					}

					val >>= 1;
					if(val & 0x40) {
						val |= 0x80;
					}

					if(!val) {
						m_rf |= GB_FLAG_Z;
					}

					m_mmu->write_byte(hl(), val);
					m_last = 4;
					break;
				default:
					THROW_GB_CPU_EXCEPTION_MESSAGE(GB_CPU_EXCEPTION_INVALID_CODE,
						"%s:0x%x", GB_CMD_STRING(GB_CMD_SRA), code);
			}
		}

		void 
		_gb_cpu::execute_cmd_srl(
			__in gbb_t code
			)
		{
			gbb_t val;

			m_rf = 0;

			switch(code) {
				case GB_CODE_SRL_A:

					if(m_ra & 1) {
						m_rf |= GB_FLAG_C;
					}

					m_ra >>= 1;
					if(!m_ra) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_SRL_B:

					if(m_rb & 1) {
						m_rf |= GB_FLAG_C;
					}

					m_rb >>= 1;
					if(!m_rb) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_SRL_C:

					if(m_rc & 1) {
						m_rf |= GB_FLAG_C;
					}

					m_rc >>= 1;
					if(!m_rc) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_SRL_D:

					if(m_rd & 1) {
						m_rf |= GB_FLAG_C;
					}

					m_rd >>= 1;
					if(!m_rd) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_SRL_E:

					if(m_re & 1) {
						m_rf |= GB_FLAG_C;
					}

					m_re >>= 1;
					if(!m_re) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_SRL_H:

					if(m_rh & 1) {
						m_rf |= GB_FLAG_C;
					}

					m_rh >>= 1;
					if(!m_rh) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_SRL_L:

					if(m_rl & 1) {
						m_rf |= GB_FLAG_C;
					}

					m_rl >>= 1;
					if(!m_rl) {
						m_rf |= GB_FLAG_Z;
					}

					m_last = 2;
					break;
				case GB_CODE_SRL_HL_INDIRECT:

					val = m_mmu->read_byte(hl());
					if(val & 1) {
						m_rf |= GB_FLAG_C;
					}

					val >>= 1;
					if(!val) {
						m_rf |= GB_FLAG_Z;
					}

					m_mmu->write_byte(hl(), val);
					m_last = 4;
					break;
				default:
					THROW_GB_CPU_EXCEPTION_MESSAGE(GB_CPU_EXCEPTION_INVALID_CODE,
						"%s:0x%x", GB_CMD_STRING(GB_CMD_SRL), code);
			}
		}

		void 
		_gb_cpu::execute_cmd_stop(
			__in gbb_t code
			)
		{
			stop();
			m_last = 1;
		}

		void 
		_gb_cpu::execute_cmd_sub(
			__in gbb_t code
			)
		{
			gbb_t res, val;

			m_rf = GB_FLAG_N;

			switch(code) {
				case GB_CODE_SUB_A_A:

					res = m_ra - m_ra;
					if(!res) {
						m_rf |= GB_FLAG_Z;
					}

					if(DETERMINE_HALF_CARRY_BYTE(res, m_ra, m_ra)) {
						m_rf |= GB_FLAG_H;
					}

					m_ra = res;
					m_last = 1;
					break;
				case GB_CODE_SUB_A_B:

					if(m_ra < m_rb) {
						m_rf |= GB_FLAG_C;
					}

					res = m_ra - m_rb;
					if(!res) {
						m_rf |= GB_FLAG_Z;
					}

					if(DETERMINE_HALF_CARRY_BYTE(res, m_ra, m_rb)) {
						m_rf |= GB_FLAG_H;
					}

					m_ra = res;
					m_last = 1;
					break;
				case GB_CODE_SUB_A_C:

					if(m_ra < m_rc) {
						m_rf |= GB_FLAG_C;
					}

					res = m_ra - m_rc;
					if(!res) {
						m_rf |= GB_FLAG_Z;
					}

					if(DETERMINE_HALF_CARRY_BYTE(res, m_ra, m_rc)) {
						m_rf |= GB_FLAG_H;
					}

					m_ra = res;
					m_last = 1;
					break;
				case GB_CODE_SUB_A_D:

					if(m_ra < m_rd) {
						m_rf |= GB_FLAG_C;
					}

					res = m_ra - m_rd;
					if(!res) {
						m_rf |= GB_FLAG_Z;
					}

					if(DETERMINE_HALF_CARRY_BYTE(res, m_ra, m_rd)) {
						m_rf |= GB_FLAG_H;
					}

					m_ra = res;
					m_last = 1;
					break;
				case GB_CODE_SUB_A_E:

					if(m_ra < m_re) {
						m_rf |= GB_FLAG_C;
					}

					res = m_ra - m_re;
					if(!res) {
						m_rf |= GB_FLAG_Z;
					}

					if(DETERMINE_HALF_CARRY_BYTE(res, m_ra, m_re)) {
						m_rf |= GB_FLAG_H;
					}

					m_ra = res;
					m_last = 1;
					break;
				case GB_CODE_SUB_A_H:

					if(m_ra < m_rh) {
						m_rf |= GB_FLAG_C;
					}

					res = m_ra - m_rh;
					if(!res) {
						m_rf |= GB_FLAG_Z;
					}

					if(DETERMINE_HALF_CARRY_BYTE(res, m_ra, m_rh)) {
						m_rf |= GB_FLAG_H;
					}

					m_ra = res;
					m_last = 1;
					break;
				case GB_CODE_SUB_A_L:

					if(m_ra < m_rl) {
						m_rf |= GB_FLAG_C;
					}

					res = m_ra - m_rl;
					if(!res) {
						m_rf |= GB_FLAG_Z;
					}

					if(DETERMINE_HALF_CARRY_BYTE(res, m_ra, m_rl)) {
						m_rf |= GB_FLAG_H;
					}

					m_ra = res;
					m_last = 1;
					break;
				case GB_CODE_SUB_A_HL_INDIRECT:
					val = m_mmu->read_byte(hl());

					if(m_ra < val) {
						m_rf |= GB_FLAG_C;
					}

					res = m_ra - val;
					if(!res) {
						m_rf |= GB_FLAG_Z;
					}

					if(DETERMINE_HALF_CARRY_BYTE(res, m_ra, val)) {
						m_rf |= GB_FLAG_H;
					}

					m_ra = res;
					m_last = 2;
					break;
				case GB_CODE_SUB_A_N:
					val = m_mmu->read_byte(m_pc++);

					if(m_ra < val) {
						m_rf |= GB_FLAG_C;
					}

					res = m_ra - val;
					if(!res) {
						m_rf |= GB_FLAG_Z;
					}

					if(DETERMINE_HALF_CARRY_BYTE(res, m_ra, val)) {
						m_rf |= GB_FLAG_H;
					}

					m_ra = res;
					m_last = 2;
					break;
				default:
					THROW_GB_CPU_EXCEPTION_MESSAGE(GB_CPU_EXCEPTION_INVALID_CODE,
						"%s:0x%x", GB_CMD_STRING(GB_CMD_SUB), code);
			}
		}

		void 
		_gb_cpu::execute_cmd_swap(
			__in gbb_t code
			)
		{
			gbb_t orig;

			m_rf = 0;

			switch(code) {
				case GB_CODE_SWAP_A:
					orig = m_ra;
					m_ra = ((orig << BITS_PER_NIBBLE) | (orig >> BITS_PER_NIBBLE));
					m_last = 2;
					break;
				case GB_CODE_SWAP_B:
					orig = m_rb;
					m_rb = ((orig << BITS_PER_NIBBLE) | (orig >> BITS_PER_NIBBLE));
					m_last = 2;
					break;
				case GB_CODE_SWAP_C:
					orig = m_rc;
					m_rc = ((orig << BITS_PER_NIBBLE) | (orig >> BITS_PER_NIBBLE));
					m_last = 2;
					break;
				case GB_CODE_SWAP_D:
					orig = m_rd;
					m_rd = ((orig << BITS_PER_NIBBLE) | (orig >> BITS_PER_NIBBLE));
					m_last = 2;
					break;
				case GB_CODE_SWAP_E:
					orig = m_re;
					m_re = ((orig << BITS_PER_NIBBLE) | (orig >> BITS_PER_NIBBLE));
					m_last = 2;
					break;
				case GB_CODE_SWAP_H:
					orig = m_rh;
					m_rh = ((orig << BITS_PER_NIBBLE) | (orig >> BITS_PER_NIBBLE));
					m_last = 2;
					break;		
				case GB_CODE_SWAP_L:
					orig = m_rl;
					m_rl = ((orig << BITS_PER_NIBBLE) | (orig >> BITS_PER_NIBBLE));
					m_last = 2;
					break;
				case GB_CODE_SWAP_HL_INDIRECT:
					orig = m_mmu->read_byte(hl());
					m_mmu->write_byte(hl(), (orig << BITS_PER_NIBBLE) | (orig >> BITS_PER_NIBBLE));
					m_last = 4;
					break;
				default:
					THROW_GB_CPU_EXCEPTION_MESSAGE(GB_CPU_EXCEPTION_INVALID_CODE,
						"%s:0x%x", GB_CMD_STRING(GB_CMD_SWAP), code);
			}
		}

		void 
		_gb_cpu::execute_cmd_xor(
			__in gbb_t code
			)
		{
			m_rf = 0;

			switch(code) {
				case GB_CODE_XOR_A:
					m_ra ^= m_ra;
					m_last = 1;
					break;
				case GB_CODE_XOR_B:
					m_ra ^= m_rb;
					m_last = 1;
					break;
				case GB_CODE_XOR_C:
					m_ra ^= m_rc;
					m_last = 1;
					break;
				case GB_CODE_XOR_D:
					m_ra ^= m_rd;
					m_last = 1;
					break;
				case GB_CODE_XOR_E:
					m_ra ^= m_re;
					m_last = 1;
					break;
				case GB_CODE_XOR_H:
					m_ra ^= m_rh;
					m_last = 1;
					break;
				case GB_CODE_XOR_L:
					m_ra ^= m_rl;
					m_last = 1;
					break;
				case GB_CODE_XOR_HL_INDIRECT:
					m_ra ^= m_mmu->read_byte(hl());
					m_last = 2;
					break;
				case GB_CODE_XOR_N:
					m_ra ^= m_mmu->read_byte(m_pc++);
					m_last = 2;
					break;
				default:
					THROW_GB_CPU_EXCEPTION_MESSAGE(GB_CPU_EXCEPTION_INVALID_CODE,
						"%s:0x%x", GB_CMD_STRING(GB_CMD_XOR), code);
			}

			if(!m_ra) {
				m_rf |= GB_FLAG_Z;
			}
		}

		void 
		_gb_cpu::execute_extended(
			__in gbb_t code
			)
		{
			gb_cmd_t cmd = GB_CMD_TYPE(GB_CMD_TYPE_EXT, code);

			switch(cmd) {
				case GB_CMD_BIT:
					execute_cmd_bit(code);
					break;
				case GB_CMD_RES:
					execute_cmd_res(code);
					break;
				case GB_CMD_RL:
					execute_cmd_rl(code);
					break;
				case GB_CMD_RLC:
					execute_cmd_rlc(code);
					break;
				case GB_CMD_RR:
					execute_cmd_rr(code);
					break;
				case GB_CMD_RRC:
					execute_cmd_rrc(code);
					break;
				case GB_CMD_SET:
					execute_cmd_set(code);
					break;
				case GB_CMD_SLA:
					execute_cmd_sla(code);
					break;
				case GB_CMD_SRA:
					execute_cmd_sra(code);
					break;
				case GB_CMD_SRL:
					execute_cmd_srl(code);
					break;
				case GB_CMD_SWAP:
					execute_cmd_swap(code);
					break;
				default:
					THROW_GB_CPU_EXCEPTION_MESSAGE(GB_CPU_EXCEPTION_INVALID_CMD,
						"0x%x", cmd);
			}
		}

		void 
		_gb_cpu::halt(void)
		{

			if(m_halt) {
				THROW_GB_CPU_EXCEPTION(GB_CPU_EXCEPTION_INVALID_STATE);
			}

			m_halt = true;
		}

		gbw_t 
		_gb_cpu::hl(void)
		{
			return ((m_rh << BITS_PER_BYTE) | m_rl);
		}

		void 
		_gb_cpu::initialize(void)
		{

			if(m_init) {
				THROW_GB_CPU_EXCEPTION(GB_CPU_EXCEPTION_INITIALIZED);
			}

			m_init = true;
			reset();
		}

		bool 
		_gb_cpu::is_allocated(void)
		{
			return (gb_cpu::m_inst != NULL);
		}

		bool 
		_gb_cpu::is_halted(void)
		{
			return m_halt;
		}

		bool 
		_gb_cpu::is_initialized(void)
		{
			return m_init;
		}

		bool 
		_gb_cpu::is_stopped(void)
		{
			return m_stop;
		}

		void 
		_gb_cpu::reset(void)
		{

			if(!m_init) {
				THROW_GB_CPU_EXCEPTION(GB_CPU_EXCEPTION_UNINITIALIZED);
			}

			m_ra = GB_REG_A_INIT;
			m_rb = GB_REG_B_INIT;
			m_rc = GB_REG_C_INIT;
			m_rd = GB_REG_D_INIT;
			m_re = GB_REG_E_INIT;
			m_rf = GB_REG_F_INIT;
			m_rh = GB_REG_H_INIT;
			m_rl = GB_REG_L_INIT;
			m_last = 0;
			m_rva = GB_REG_A_ALT_INIT;
			m_rvb = GB_REG_B_ALT_INIT;
			m_rvc = GB_REG_C_ALT_INIT;
			m_rvd = GB_REG_D_ALT_INIT;
			m_rve = GB_REG_E_ALT_INIT;
			m_rvf = GB_REG_F_ALT_INIT;
			m_rvh = GB_REG_H_ALT_INIT;
			m_rvl = GB_REG_L_ALT_INIT;
			m_pc = GB_REG_PC_INIT;
			m_sp = GB_REG_SP_INIT;
			m_tot = 0;
			m_halt = GB_INT_HALT_INIT;
			m_ime = GB_INT_IME_INIT;
			m_stop = GB_INT_STOP_INIT;
		}

		void 
		_gb_cpu::resume(void)
		{

			if(!m_halt) {
				THROW_GB_CPU_EXCEPTION(GB_CPU_EXCEPTION_INVALID_STATE);
			}

			m_halt = false;
		}

		void 
		_gb_cpu::set_af(
			__in gbw_t val
			)
		{
			m_ra = (val >> BITS_PER_BYTE) & GBB_MAX_LEN;
			m_rf = val & GBB_MAX_LEN;
		}

		void 
		_gb_cpu::set_bc(
			__in gbw_t val
			)
		{
			m_rb = (val >> BITS_PER_BYTE) & GBB_MAX_LEN;
			m_rc = val & GBB_MAX_LEN;
		}

		void 
		_gb_cpu::set_de(
			__in gbw_t val
			)
		{
			m_rd = (val >> BITS_PER_BYTE) & GBB_MAX_LEN;
			m_re = val & GBB_MAX_LEN;
		}

		void 
		_gb_cpu::set_hl(
			__in gbw_t val
			)
		{
			m_rh = (val >> BITS_PER_BYTE) & GBB_MAX_LEN;
			m_rl = val & GBB_MAX_LEN;
		}

		void 
		_gb_cpu::start(void)
		{

			if(!m_stop) {
				THROW_GB_CPU_EXCEPTION(GB_CPU_EXCEPTION_INVALID_STATE);
			}

			m_stop = false;
		}

		gbb_t 
		_gb_cpu::step(void)
		{
			gbb_t code;

			if(!m_init) {
				THROW_GB_CPU_EXCEPTION(GB_CPU_EXCEPTION_UNINITIALIZED);
			}

			code = m_mmu->read_byte(m_pc++);
			if(code == GB_CODE_EXT) {
				code = m_mmu->read_byte(m_pc++);
				execute_extended(code);
			} else {
				execute(code);
			}

			m_tot += m_last;

			return m_last;
		}

		void 
		_gb_cpu::stop(void)
		{

			if(m_stop) {
				THROW_GB_CPU_EXCEPTION(GB_CPU_EXCEPTION_INVALID_STATE);
			}

			m_stop = true;
		}

		void 
		_gb_cpu::swap(void)
		{

			if(!m_init) {
				THROW_GB_CPU_EXCEPTION(GB_CPU_EXCEPTION_UNINITIALIZED);
			}

			m_ra = m_ra ^ m_rva;
			m_rva = m_ra ^ m_rva;
			m_ra = m_ra ^ m_rva;
			m_rb = m_rb ^ m_rvb;
			m_rvb = m_rb ^ m_rvb;
			m_rb = m_rb ^ m_rvb;
			m_rc = m_rc ^ m_rvc;
			m_rvc = m_rc ^ m_rvc;
			m_rc = m_rc ^ m_rvc;
			m_rd = m_rd ^ m_rvd;
			m_rvd = m_rd ^ m_rvd;
			m_rd = m_rd ^ m_rvd;
			m_re = m_re ^ m_rve;
			m_rve = m_re ^ m_rve;
			m_re = m_re ^ m_rve;
			m_rf = m_rf ^ m_rvf;
			m_rvf = m_rf ^ m_rvf;
			m_rf = m_rf ^ m_rvf;
			m_rh = m_rh ^ m_rvh;
			m_rvh = m_rh ^ m_rvh;
			m_rh = m_rh ^ m_rvh;
			m_rl = m_rl ^ m_rvl;
			m_rvl = m_rl ^ m_rvl;
			m_rl = m_rl ^ m_rvl;
		}

		std::string 
		_gb_cpu::to_string(
			__in_opt bool verb
			)
		{
			std::stringstream res;

			res << "[" << (m_init ? "INIT" : "UNINIT") << "] " << GB_CPU_HEADER
					<< " (ptr=0x" << VAL_AS_HEX(gb_cpu_ptr, this) << ")";

			if(m_init && verb) {
				res << std::endl << "--- A: " << VAL_AS_HEX(gbb_t, m_ra) << " (" << VAL_AS_HEX(gbb_t, m_rva)
					<< "), B: " << VAL_AS_HEX(gbb_t, m_rb) << " (" << VAL_AS_HEX(gbb_t, m_rvb)
					<< "), C: " << VAL_AS_HEX(gbb_t, m_rc) << " (" << VAL_AS_HEX(gbb_t, m_rvc) 
					<< "), D: " << VAL_AS_HEX(gbb_t, m_rd) << " (" << VAL_AS_HEX(gbb_t, m_rvd) 
					<< "), E: " << VAL_AS_HEX(gbb_t, m_re) << " (" << VAL_AS_HEX(gbb_t, m_rve) 
					<< "), F: " << VAL_AS_HEX(gbb_t, m_rf);

				if(m_rf >= GB_FLAG_C) {
					res << " [";

					if(m_rf & GB_FLAG_Z) {
						res << "Z";
					}

					if(m_rf & GB_FLAG_N) {
						res << "N";
					}

					if(m_rf & GB_FLAG_H) {
						res << "H";
					}

					if(m_rf & GB_FLAG_C) {
						res << "C";
					}

					res << "]";
				}

				res << " (" << VAL_AS_HEX(gbb_t, m_rvf);

				if(m_rvf >= GB_FLAG_C) {
					res << " [";

					if(m_rvf & GB_FLAG_Z) {
						res << "Z";
					}

					if(m_rvf & GB_FLAG_N) {
						res << "N";
					}

					if(m_rvf & GB_FLAG_H) {
						res << "H";
					}

					if(m_rvf & GB_FLAG_C) {
						res << "C";
					}

					res << "]";
				}

				res << "), H: " << VAL_AS_HEX(gbb_t, m_rh) << " (" << VAL_AS_HEX(gbb_t, m_rvh)
					<< "), L: " << VAL_AS_HEX(gbb_t, m_rl) << " (" << VAL_AS_HEX(gbb_t, m_rvl) << ")"
					<< std::endl << "--- PC: " << VAL_AS_HEX(gbw_t, m_pc) << ", SP: " 
					<< VAL_AS_HEX(gbw_t, m_sp) << ", LAST: " << (uint16_t) m_last << ", TOTAL: " << m_tot 
					<< ", HALT: " << m_halt << ", IME: " << m_ime << ", STOP: " << m_stop;
			}

			return res.str();
		}

		void 
		_gb_cpu::uninitialize(void)
		{

			if(!m_init) {
				THROW_GB_CPU_EXCEPTION(GB_CPU_EXCEPTION_UNINITIALIZED);
			}

			reset();
			m_init = false;
		}
	}
}
