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

		#define _DETERMINE_HALF_CARRY(_A_, _B_, _RES_, _LEN_) \
			(((_RES_) ^ (_A_) ^ (_B_)) & (_LEN_))
		#define DETERMINE_HALF_CARRY_BYTE(_A_, _B_, _RES_) \
			_DETERMINE_HALF_CARRY(_A_, _B_, _RES_, UINT8_MAX + 1)
		#define DETERMINE_HALF_CARRY_NIBBLE(_A_, _B_, _RES_) \
			_DETERMINE_HALF_CARRY(_A_, _B_, _RES_, UINT4_MAX + 1)

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
			// TODO
		}

		void 
		_gb_cpu::execute_cmd_add(
			__in gbb_t code
			)
		{
			// TODO
		}

		void 
		_gb_cpu::execute_cmd_and(
			__in gbb_t code
			)
		{
			// TODO
		}

		void 
		_gb_cpu::execute_cmd_bit(
			__in gbb_t code
			)
		{
			// TODO
		}

		void 
		_gb_cpu::execute_cmd_call(
			__in gbb_t code
			)
		{
			// TODO
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
			// TODO
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
			// TODO
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
			// TODO
		}

		void 
		_gb_cpu::execute_cmd_jp(
			__in gbb_t code
			)
		{
			// TODO
		}

		void 
		_gb_cpu::execute_cmd_jr(
			__in gbb_t code
			)
		{
			// TODO
		}

		void 
		_gb_cpu::execute_cmd_ld(
			__in gbb_t code
			)
		{

			switch(code) {
				case GB_CMD_LD_B_N:
					m_rb = m_mmu->read_byte(m_pc++);
					m_last = 2;
					break;
				case GB_CMD_LD_C_N:
					m_rc = m_mmu->read_byte(m_pc++);
					m_last = 2;
					break;
				case GB_CMD_LD_D_N:
					m_rd = m_mmu->read_byte(m_pc++);
					m_last = 2;
					break;
				case GB_CMD_LD_E_N:
					m_re = m_mmu->read_byte(m_pc++);
					m_last = 2;
					break;
				case GB_CMD_LD_H_N:
					m_rh = m_mmu->read_byte(m_pc++);
					m_last = 2;
					break;
				case GB_CMD_LD_L_N:
					m_rl = m_mmu->read_byte(m_pc++);
					m_last = 2;
					break;
				case GB_CMD_LD_A_A:
					m_last = 1;
					break;
				case GB_CMD_LD_A_B:
					m_ra = m_rb;
					m_last = 1;
					break;
				case GB_CMD_LD_A_C:
					m_ra = m_rc;
					m_last = 1;
					break;
				case GB_CMD_LD_A_D:
					m_ra = m_rd;
					m_last = 1;
					break;
				case GB_CMD_LD_A_E:
					m_ra = m_re;
					m_last = 1;
					break;
				case GB_CMD_LD_A_H:
					m_ra = m_rh;
					m_last = 1;
					break;
				case GB_CMD_LD_A_L:
					m_ra = m_rl;
					m_last = 1;
					break;
				case GB_CMD_LD_A_HL_INDIRECT:
					m_ra = m_mmu->read_byte(hl());
					m_last = 2;
					break;
				case GB_CMD_LD_B_B:
					m_last = 1;
					break;
				case GB_CMD_LD_B_C:
					m_rb = m_rc;
					m_last = 1;
					break;
				case GB_CMD_LD_B_D:
					m_rb = m_rd;
					m_last = 1;
					break;
				case GB_CMD_LD_B_E:
					m_rb = m_re;
					m_last = 1;
					break;
				case GB_CMD_LD_B_H:
					m_rb = m_rh;
					m_last = 1;
					break;
				case GB_CMD_LD_B_L:
					m_rb = m_rl;
					m_last = 1;
					break;
				case GB_CMD_LD_B_HL_INDIRECT:
					m_rb = m_mmu->read_byte(hl());
					m_last = 2;
					break;
				case GB_CMD_LD_C_B:
					m_rc = m_rb;
					m_last = 1;
					break;
				case GB_CMD_LD_C_C:
					m_last = 1;
					break;
				case GB_CMD_LD_C_D:
					m_rc = m_rd;
					m_last = 1;
					break;
				case GB_CMD_LD_C_E:
					m_rc = m_re;
					m_last = 1;
					break;
				case GB_CMD_LD_C_H:
					m_rc = m_rh;
					m_last = 1;
					break;		
				case GB_CMD_LD_C_L:
					m_rc = m_rl;
					m_last = 1;
					break;
				case GB_CMD_LD_C_HL_INDIRECT:
					m_rc = m_mmu->read_byte(hl());
					m_last = 2;
					break;
				case GB_CMD_LD_D_B:
					m_rd = m_rb;
					m_last = 1;
					break;
				case GB_CMD_LD_D_C:
					m_rd = m_rc;
					m_last = 1;
					break;
				case GB_CMD_LD_D_D:
					m_last = 1;
					break;
				case GB_CMD_LD_D_E:
					m_rd = m_re;
					m_last = 1;
					break;
				case GB_CMD_LD_D_H:
					m_rd = m_rh;
					m_last = 1;
					break;		
				case GB_CMD_LD_D_L:
					m_rd = m_rl;
					m_last = 1;
					break;
				case GB_CMD_LD_D_HL_INDIRECT:
					m_rd = m_mmu->read_byte(hl());
					m_last = 2;
					break;
				case GB_CMD_LD_E_B:
					m_re = m_rb;
					m_last = 1;
					break;
				case GB_CMD_LD_E_C:
					m_re = m_rc;
					m_last = 1;
					break;
				case GB_CMD_LD_E_D:
					m_re = m_rd;
					m_last = 1;
					break;
				case GB_CMD_LD_E_E:
					m_last = 1;
					break;
				case GB_CMD_LD_E_H:
					m_re = m_rh;
					m_last = 1;
					break;		
				case GB_CMD_LD_E_L:
					m_re = m_rl;
					m_last = 1;
					break;
				case GB_CMD_LD_E_HL_INDIRECT:
					m_re = m_mmu->read_byte(hl());
					m_last = 2;
					break;
				case GB_CMD_LD_H_B:
					m_rh = m_rb;
					m_last = 1;
					break;
				case GB_CMD_LD_H_C:
					m_rh = m_rc;
					m_last = 1;
					break;
				case GB_CMD_LD_H_D:
					m_rh = m_rd;
					m_last = 1;
					break;
				case GB_CMD_LD_H_E:
					m_rh = m_re;
					m_last = 1;
					break;
				case GB_CMD_LD_H_H:
					m_last = 1;
					break;		
				case GB_CMD_LD_H_L:
					m_rh = m_rl;
					m_last = 1;
					break;
				case GB_CMD_LD_H_HL_INDIRECT:
					m_rh = m_mmu->read_byte(hl());
					m_last = 2;
					break;
				case GB_CMD_LD_L_B:
					m_rl = m_rb;
					m_last = 1;
					break;
				case GB_CMD_LD_L_C:
					m_rl = m_rc;
					m_last = 1;
					break;
				case GB_CMD_LD_L_D:
					m_rl = m_rd;
					m_last = 1;
					break;
				case GB_CMD_LD_L_E:
					m_rl = m_re;
					m_last = 1;
					break;
				case GB_CMD_LD_L_H:
					m_rl = m_rh;
					m_last = 1;
					break;
				case GB_CMD_LD_L_L:
					m_last = 1;
					break;
				case GB_CMD_LD_L_HL_INDIRECT:
					m_rl = m_mmu->read_byte(hl());
					m_last = 2;
					break;
				case GB_CMD_LD_HL_INDIRECT_B:
					m_mmu->write_byte(hl(), m_rb);
					m_last = 2;
					break;
				case GB_CMD_LD_HL_INDIRECT_C:
					m_mmu->write_byte(hl(), m_rc);
					m_last = 2;
					break;
				case GB_CMD_LD_HL_INDIRECT_D:
					m_mmu->write_byte(hl(), m_rd);
					m_last = 2;
					break;
				case GB_CMD_LD_HL_INDIRECT_E:
					m_mmu->write_byte(hl(), m_re);
					m_last = 2;
					break;
				case GB_CMD_LD_HL_INDIRECT_H:
					m_mmu->write_byte(hl(), m_rh);
					m_last = 2;
					break;
				case GB_CMD_LD_HL_INDIRECT_L:
					m_mmu->write_byte(hl(), m_rl);
					m_last = 2;
					break;	
				case GB_CMD_LD_HL_INDIRECT_N:
					m_mmu->write_byte(hl(), m_mmu->read_byte(m_pc++));
					m_last = 3;
					break;
				case GB_CMD_LD_A_BC_INDIRECT:
					m_ra = m_mmu->read_byte(bc());
					m_last = 2;
					break;
				case GB_CMD_LD_A_DE_INDIRECT:
					m_ra = m_mmu->read_byte(de());
					m_last = 2;
					break;
				case GB_CMD_LD_A_NN_INDIRECT:
					m_ra = m_mmu->read_byte(m_mmu->read_word(m_pc));
					m_pc += sizeof(gbw_t);
					m_last = 4;
					break;
				case GB_CMD_LD_A_N:
					m_ra = m_mmu->read_byte(m_pc++);
					m_last = 2;
					break;
				case GB_CMD_LD_B_A:
					m_rb = m_ra;
					m_last = 1;
					break;
				case GB_CMD_LD_C_A:
					m_rc = m_ra;
					m_last = 1;
					break;
				case GB_CMD_LD_D_A:
					m_rd = m_ra;
					m_last = 1;
					break;
				case GB_CMD_LD_E_A:
					m_re = m_ra;
					m_last = 1;
					break;
				case GB_CMD_LD_H_A:
					m_rh = m_ra;
					m_last = 1;
					break;
				case GB_CMD_LD_L_A:
					m_rl = m_ra;
					m_last = 1;
					break;
				case GB_CMD_LD_BC_INDIRECT_A:
					m_mmu->write_byte(m_mmu->read_byte(bc()), m_ra);
					m_last = 2;
					break;
				case GB_CMD_LD_DE_INDIRECT_A:
					m_mmu->write_byte(m_mmu->read_byte(de()), m_ra);
					m_last = 2;
					break;
				case GB_CMD_LD_HL_INDIRECT_A:
					m_mmu->write_byte(m_mmu->read_byte(hl()), m_ra);
					m_last = 2;
					break;
				case GB_CMD_LD_NN_INDIRECT_A:
					m_mmu->write_byte(m_mmu->read_byte(m_mmu->read_word(m_pc)), m_ra);
					m_pc += sizeof(gbw_t);
					m_last = 4;
					break;
				case GB_CMD_LD_A_C_INDIRECT:
					m_ra = m_mmu->read_byte(m_rc + 0xff00);
					m_last = 2;
					break;
				case GB_CMD_LD_C_INDIRECT_A:
					m_mmu->write_byte(m_rc + 0xff00, m_ra);
					m_last = 2;
					break;
				case GB_CMD_LD_BC_NN:
					set_bc(m_mmu->read_word(m_pc));
					m_pc += sizeof(gbw_t);
					m_last = 3;
					break;
				case GB_CMD_LD_DE_NN:
					set_de(m_mmu->read_word(m_pc));
					m_pc += sizeof(gbw_t);
					m_last = 3;
					break;
				case GB_CMD_LD_HL_NN:
					set_hl(m_mmu->read_word(m_pc));
					m_pc += sizeof(gbw_t);
					m_last = 3;
					break;
				case GB_CMD_LD_SP_NN:
					m_sp = m_mmu->read_word(m_pc);
					m_pc += sizeof(gbw_t);
					m_last = 3;
					break;
				case GB_CMD_LD_SP_HL:
					m_sp = hl();
					m_last = 2;
					break;
				case GB_CMD_LD_NN_INDIRECT_SP:
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
			// TODO
		}

		void 
		_gb_cpu::execute_cmd_pop(
			__in gbb_t code
			)
		{
			// TODO
		}

		void 
		_gb_cpu::execute_cmd_push(
			__in gbb_t code
			)
		{
			// TODO
		}

		void 
		_gb_cpu::execute_cmd_res(
			__in gbb_t code
			)
		{
			// TODO
		}

		void 
		_gb_cpu::execute_cmd_ret(
			__in gbb_t code
			)
		{
			// TODO
		}

		void 
		_gb_cpu::execute_cmd_reti(
			__in gbb_t code
			)
		{
			// TODO
		}

		void 
		_gb_cpu::execute_cmd_rl(
			__in gbb_t code
			)
		{
			// TODO
		}

		void 
		_gb_cpu::execute_cmd_rlc(
			__in gbb_t code
			)
		{
			// TODO
		}

		void 
		_gb_cpu::execute_cmd_rr(
			__in gbb_t code
			)
		{
			// TODO
		}

		void 
		_gb_cpu::execute_cmd_rrc(
			__in gbb_t code
			)
		{
			// TODO
		}

		void 
		_gb_cpu::execute_cmd_rst(
			__in gbb_t code
			)
		{
			// TODO
		}

		void 
		_gb_cpu::execute_cmd_sbc(
			__in gbb_t code
			)
		{
			// TODO
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
			// TODO
		}

		void 
		_gb_cpu::execute_cmd_sla(
			__in gbb_t code
			)
		{
			// TODO
		}

		void 
		_gb_cpu::execute_cmd_sra(
			__in gbb_t code
			)
		{
			// TODO
		}

		void 
		_gb_cpu::execute_cmd_srl(
			__in gbb_t code
			)
		{
			// TODO
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
			// TODO
		}

		void 
		_gb_cpu::execute_cmd_swap(
			__in gbb_t code
			)
		{
			// TODO
		}

		void 
		_gb_cpu::execute_cmd_xor(
			__in gbb_t code
			)
		{
			// TODO
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
			if(code == GB_CMD_EXT) {
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
