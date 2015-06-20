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

#ifndef GB_CPU_H_
#define GB_CPU_H_

namespace GB_NS {

	namespace GB_COMP_NS {

		typedef class _gb_cpu {

			public:

				~_gb_cpu(void);

				static void _delete(void);

				static _gb_cpu *acquire(void);

				void halt(void);

				void initialize(void);

				static bool is_allocated(void);

				bool is_halted(void);

				bool is_initialized(void);

				bool is_stopped(void);

				bool is_zero_page(void);

				void reset(void);

				void resume(void);

				void start(
					__in_opt const std::string &title = std::string(),
					__in_opt bool detach = true
					);

				gbb_t step(void);

				void stop(void);

				std::string to_string(
					__in_opt bool verb = false
					);

				void uninitialize(void);

			protected:

				_gb_cpu(void);

				_gb_cpu(
					__in const _gb_cpu &other
					);

				_gb_cpu &operator=(
					__in const _gb_cpu &other
					);

				gbw_t af(void);

				gbw_t bc(void);

				gbw_t de(void);

				void execute(
					__in gbb_t code
					);

				void execute_cmd_adc(
					__in gbb_t code
					);

				void execute_cmd_add(
					__in gbb_t code
					);

				void execute_cmd_and(
					__in gbb_t code
					);

				void execute_cmd_bit(
					__in gbb_t code
					);

				void execute_cmd_call(
					__in gbb_t code
					);

				void execute_cmd_ccf(
					__in gbb_t code
					);

				void execute_cmd_cp(
					__in gbb_t code
					);

				void execute_cmd_cpl(
					__in gbb_t code
					);

				void execute_cmd_daa(
					__in gbb_t code
					);

				void execute_cmd_dec(
					__in gbb_t code
					);

				void execute_cmd_di(
					__in gbb_t code
					);

				void execute_cmd_ei(
					__in gbb_t code
					);

				void execute_cmd_halt(
					__in gbb_t code
					);

				void execute_cmd_inc(
					__in gbb_t code
					);

				void execute_cmd_jp(
					__in gbb_t code
					);

				void execute_cmd_jr(
					__in gbb_t code
					);

				void execute_cmd_ld(
					__in gbb_t code
					);

				void execute_cmd_ldd(
					__in gbb_t code
					);

				void execute_cmd_ldh(
					__in gbb_t code
					);

				void execute_cmd_ldhl(
					__in gbb_t code
					);

				void execute_cmd_ldi(
					__in gbb_t code
					);

				void execute_cmd_nop(
					__in gbb_t code
					);

				void execute_cmd_or(
					__in gbb_t code
					);

				void execute_cmd_pop(
					__in gbb_t code
					);

				void execute_cmd_push(
					__in gbb_t code
					);

				void execute_cmd_res(
					__in gbb_t code
					);

				void execute_cmd_ret(
					__in gbb_t code
					);

				void execute_cmd_reti(
					__in gbb_t code
					);

				void execute_cmd_rl(
					__in gbb_t code
					);

				void execute_cmd_rlc(
					__in gbb_t code
					);

				void execute_cmd_rr(
					__in gbb_t code
					);

				void execute_cmd_rrc(
					__in gbb_t code
					);

				void execute_cmd_rst(
					__in gbb_t code
					);

				void execute_cmd_sbc(
					__in gbb_t code
					);

				void execute_cmd_scf(
					__in gbb_t code
					);

				void execute_cmd_set(
					__in gbb_t code
					);

				void execute_cmd_sla(
					__in gbb_t code
					);

				void execute_cmd_sra(
					__in gbb_t code
					);

				void execute_cmd_srl(
					__in gbb_t code
					);

				void execute_cmd_stop(
					__in gbb_t code
					);

				void execute_cmd_sub(
					__in gbb_t code
					);

				void execute_cmd_swap(
					__in gbb_t code
					);

				void execute_cmd_xor(
					__in gbb_t code
					);

				void execute_extended(
					__in gbb_t code
					);

				gbw_t hl(void);

				void set_af(
					__in gbw_t val
					);

				void set_bc(
					__in gbw_t val
					);

				void set_de(
					__in gbw_t val
					);

				void set_hl(
					__in gbw_t val
					);

				void swap(void);

				bool m_init;

				static _gb_cpu *m_inst;

				gb_gpu_ptr m_gpu;

				gb_mmu_ptr m_mmu;

			private:

				gbb_t m_ra, m_rb, m_rc, m_rd, m_re, m_rf, m_rh, m_rl, m_last, 
					m_rva, m_rvb, m_rvc, m_rvd, m_rve, m_rvf, m_rvh, m_rvl;

				gbw_t m_pc, m_sp;

				uint32_t m_tot;

				bool m_halt, m_ime, m_stop;

		} gb_cpu, *gb_cpu_ptr;
	}
}

#endif // GB_CPU_H_
