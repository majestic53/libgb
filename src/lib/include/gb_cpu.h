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

				void initialize(void);

				static bool is_allocated(void);

				bool is_initialized(void);

				void reset(void);

				gbb_t step(void);

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
					__in gb_cmd_t cmd,
					__in gbb_t code
					);

				void execute_extended(
					__in gb_cmd_t cmd,
					__in gbb_t code
					);

				// TODO: add execution helper routines

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

				gb_mmu_ptr m_mmu;

			private:

				gbb_t m_ra, m_rb, m_rc, m_rd, m_re, m_rf, m_rh, m_rl, m_last, 
					m_rva, m_rvb, m_rvc, m_rvd, m_rve, m_rvf, m_rvh, m_rvl;

				gbw_t m_pc, m_sp;

				uint32_t m_tot;

		} gb_cpu, *gb_cpu_ptr;
	}
}

#endif // GB_CPU_H_
