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

#ifndef GB_MMU_H_
#define GB_MMU_H_

namespace GB_NS {

	namespace GB_COMP_NS {
	
		#define GB_MMU_MAX_ADDR GBW_MAX_LEN

		typedef uint16_t gb_addr_t;
	
		typedef std::vector<gbb_t> gb_buf_t;
	
		typedef class _gb_mmu {
		
			public:
						
				~_gb_mmu(void);
			
				static void _delete(void);

				static _gb_mmu *acquire(void);
				
				static std::string buffer_as_string(
					__in const gb_buf_t &buf,
					__in_opt bool verb = false,
					__in_opt gb_addr_t org = 0
					);
				
				void clear(void);
				
				void fill(
					__in gb_addr_t addr,
					__in gbw_t off,
					__in gbb_t val
					);
				
				void initialize(void);

				void insert(
					__in gb_addr_t addr,
					__in const gb_buf_t &buf
					);

				static bool is_allocated(void);
				
				bool is_initialized(void);
				
				gbb_t read_byte(
					__in gb_addr_t addr
					);
				
				gbw_t read_word(
					__in gb_addr_t addr
					);
				
				std::string to_string(
					__in_opt bool verb = false,
					__in_opt gb_addr_t addr = 0,
					__in_opt gbw_t off = 0
					);
				
				void uninitialize(void);
			
				void write_byte(
					__in gb_addr_t addr,
					__in gbb_t val
					);
				
				void write_word(
					__in gb_addr_t addr,
					__in gbw_t val
					);
			
			protected:
			
				_gb_mmu(void);
				
				_gb_mmu(
					__in const _gb_mmu &other
					);
					
				_gb_mmu &operator=(
					__in const _gb_mmu &other
					);
			
				bool m_init;
			
				static _gb_mmu *m_inst;
			
			private:
			
				gb_buf_t m_buf;
		
		} gb_mmu, *gb_mmu_ptr;
	}
}

#endif // GB_MMU_H_
