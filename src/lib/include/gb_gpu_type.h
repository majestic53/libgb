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

#ifndef GB_GPU_TYPE_H_
#define GB_GPU_TYPE_H_

namespace GB_NS {

	namespace GB_COMP_NS {

		#define GB_GPU_HEADER "(GPU)"

		#ifndef NDEBUG
		#define GB_GPU_EXCEPTION_HEADER GB_GPU_HEADER
		#else
		#define GB_GPU_EXCEPTION_HEADER EXCEPTION_HEADER
		#endif // NDEBUG

		enum {
			GB_GPU_EXCEPTION_ACTIVE = 0,
			GB_GPU_EXCEPTION_ALLOC_FAILED,
			GB_GPU_EXCPETION_INACTIVE,
			GB_GPU_EXCEPTION_INITIALIZED,
			GB_GPU_EXCEPTION_INVALID_PARAM,
			GB_GPU_EXCEPTION_UNINITIALIZED,
			GB_GPU_EXCEPTION_UNKNOWN_STATE,
		};

		#define GB_GPU_EXCEPTION_MAX GB_GPU_EXCEPTION_UNKNOWN_STATE

		static const std::string GB_GPU_EXCEPTION_STR[] = {
			GB_GPU_EXCEPTION_HEADER " Component is already active",
			GB_GPU_EXCEPTION_HEADER " Failed to allocate component",
			GB_GPU_EXCEPTION_HEADER " Component is inactive", 
			GB_GPU_EXCEPTION_HEADER " Component is already initialized",
			GB_GPU_EXCEPTION_HEADER " Invalid component parameter",
			GB_GPU_EXCEPTION_HEADER " Component is uninitialized",
			GB_GPU_EXCEPTION_HEADER " Unknown state",
			};

		#define GB_GPU_EXCEPTION_STRING(_TYPE_) \
			((_TYPE_) > GB_GPU_EXCEPTION_MAX ? UNKNOWN_EXCEPTION : \
			GB_GPU_EXCEPTION_STR[_TYPE_].c_str())

		#define THROW_GB_GPU_EXCEPTION(_TYPE_) \
			THROW_GB_EXCEPTION(GB_GPU_EXCEPTION_STRING(_TYPE_))
		#define THROW_GB_GPU_EXCEPTION_MESSAGE(_TYPE_, _FORMAT_, ...) \
			THROW_GB_EXCEPTION_MESSAGE(GB_GPU_EXCEPTION_STRING(_TYPE_), _FORMAT_, __VA_ARGS__)

		class _gb_gpu;
		typedef _gb_gpu gb_gpu, *gb_gpu_ptr;
	}
}

#endif // GB_GPU_TYPE_H_
