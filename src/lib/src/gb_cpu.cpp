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
			m_tot(0)
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
			__in gb_cmd_t cmd,
			__in gbb_t code
			)
		{

			switch(cmd) {

				// TODO

				default:
					THROW_GB_CPU_EXCEPTION_MESSAGE(GB_CPU_EXCEPTION_INVALID_CMD,
						"0x%x", cmd);
			}
		}

		void 
		_gb_cpu::execute_extended(
			__in gb_cmd_t cmd,
			__in gbb_t code
			)
		{

			switch(cmd) {

				// TODO

				default:
					THROW_GB_CPU_EXCEPTION_MESSAGE(GB_CPU_EXCEPTION_INVALID_CMD,
						"0x%x", cmd);
			}
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
			return (gb_cpu::m_inst);
		}

		bool 
		_gb_cpu::is_initialized(void)
		{
			return m_init;
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
				execute_extended(GB_CMD_TYPE(GB_CMD_TYPE_EXT, code), code);
			} else {
				execute(GB_CMD_TYPE(GB_CMD_TYPE_SINGLE, code), code);
			}

			m_tot += m_last;

			return m_last;
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
				res << std::endl << "--- A: " << VAL_AS_HEX(gbb_t, m_ra) << ", B: " 
					<< VAL_AS_HEX(gbb_t, m_rb) << ", C: " << VAL_AS_HEX(gbb_t, m_rc) << ", D: " 
					<< VAL_AS_HEX(gbb_t, m_rd) << ", E: " << VAL_AS_HEX(gbb_t, m_re) << ", F: " 
					<< VAL_AS_HEX(gbb_t, m_rf);

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

				res << ", H: " << VAL_AS_HEX(gbb_t, m_rh) << ", L: " << VAL_AS_HEX(gbb_t, m_rl)
					<< std::endl << "--- A': " << VAL_AS_HEX(gbb_t, m_rva) << ", B': " 
					<< VAL_AS_HEX(gbb_t, m_rvb) << ", C': " << VAL_AS_HEX(gbb_t, m_rvc) << ", D': " 
					<< VAL_AS_HEX(gbb_t, m_rvd) << ", E': " << VAL_AS_HEX(gbb_t, m_rve) << ", F': " 
					<< VAL_AS_HEX(gbb_t, m_rvf);

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

				res << ", H': " << VAL_AS_HEX(gbb_t, m_rvh) << ", L': " << VAL_AS_HEX(gbb_t, m_rvl)
					<< std::endl << "--- PC: " << VAL_AS_HEX(gbw_t, m_pc) << ", SP: " 
					<< VAL_AS_HEX(gbw_t, m_sp) << std::endl << "--- LAST: " << m_last << ", TOTAL: " 
					<< m_tot;
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
