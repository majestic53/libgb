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

#ifndef GB_H_
#define GB_H_

//#define NDEBUG

#ifndef GB_NS
#define GB_NS gb_ns
#endif // GB_NS

#include "gb_define.h"
#include "gb_except.h"
#include "gb_cmd.h"

using namespace GB_NS;

#ifndef GB_COMP_NS
#define GB_COMP_NS gb_comp
#endif // GB_COMP_NS

#include "gb_mmu.h"
#include "gb_cpu.h"

using namespace GB_NS::GB_COMP_NS;

namespace GB_NS {

	typedef class _gb {

		public:

			~_gb(void);

			static void _delete(void);

			static _gb *acquire(void);

			gb_cpu_ptr acquire_cpu(void);

			gb_mmu_ptr acquire_mmu(void);

			void initialize(void);

			static bool is_allocated(void);

			bool is_initialized(void);

			std::string to_string(
				__in_opt bool verb = false
				);

			void uninitialize(void);

			static std::string version(
				__in_opt bool verb = false
				);

		protected:

			_gb(void);

			_gb(
				__in const _gb &other
				);

			_gb &operator=(
				__in const _gb &other
				);

			void release_cpu(void);

			void release_mmu(void);

			bool m_init;

			static _gb *m_inst;

			gb_cpu_ptr m_inst_cpu;

			gb_mmu_ptr m_inst_mmu;

	} gb, *gb_ptr;
}

#endif // GB_H_
