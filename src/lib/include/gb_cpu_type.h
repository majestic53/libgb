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

#ifndef GB_CPU_TYPE_H_
#define GB_CPU_TYPE_H_

namespace GB_NS {

	namespace GB_COMP_NS {
	
		#define GB_CPU_HEADER "(CPU)"
		
		#ifndef NDEBUG
		#define GB_CPU_EXCEPTION_HEADER GB_CPU_HEADER
		#else
		#define GB_CPU_EXCEPTION_HEADER GB_EXCEPTION_HEADER
		#endif // NDEBUG
		
		enum {
			GB_CPU_EXCEPTION_ALLOC_FAILED = 0,
			GB_CPU_EXCEPTION_INITIALIZED,
			GB_CPU_EXCEPTION_INVALID_CMD,
			GB_CPU_EXCEPTION_UNINITIALIZED,
		};
		
		#define GB_CPU_EXCEPTION_MAX GB_CPU_EXCEPTION_UNINITIALIZED
		
		static const std::string GB_CPU_EXCEPTION_STR[] = {
			GB_CPU_EXCEPTION_HEADER " Failed to allocate component",
			GB_CPU_EXCEPTION_HEADER " Component is already initialized",
			GB_CPU_EXCEPTION_HEADER " Invalid command",
			GB_CPU_EXCEPTION_HEADER " Component is uninitialized",
			};
			
		#define GB_CPU_EXCEPTION_STRING(_TYPE_) \
			((_TYPE_) > GB_CPU_EXCEPTION_MAX ? UNKNOWN_EXCEPTION : \
			GB_CPU_EXCEPTION_STR[_TYPE_].c_str())
			
		#define THROW_GB_CPU_EXCEPTION(_TYPE_) \
			THROW_GB_EXCEPTION(GB_CPU_EXCEPTION_STRING(_TYPE_))
		#define THROW_GB_CPU_EXCEPTION_MESSAGE(_TYPE_, _FORMAT_, ...) \
			THROW_GB_EXCEPTION_MESSAGE(GB_CPU_EXCEPTION_STRING(_TYPE_), \
			_FORMAT_, __VA_ARGS__)
			
		class _gb_cpu;
		typedef _gb_cpu gb_cpu, *gb_cpu_ptr;
	}
}

#endif // GB_CPU_TYPE_H_
