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

#ifndef GB_TYPE_H_
#define GB_TYPE_H_

namespace GB_NS {

	#define GB_HEADER "(GB)"
	
	#ifndef NDEBUG
	#define GB_GB_EXCEPTION_HEADER GB_HEADER
	#else
	#define GB_GB_EXCEPTION_HEADER GB_EXCEPTION_HEADER
	#endif // NDEBUG
	
	enum {
		GB_GB_EXCEPTION_ALLOC_FAILED = 0,
		GB_GB_EXCEPTION_BIOS_FAILED,
		GB_GB_EXCEPTION_BIOS_EXPORT_FAILED,
		GB_GB_EXCEPTION_INITIALIZED,
		GB_GB_EXCEPTION_UNINITIALIZED,
		GB_GB_EXCEPTION_UNSUPPORTED,
	};
	
	#define GB_GB_EXCEPTION_MAX GB_GB_EXCEPTION_UNSUPPORTED
	
	static const std::string GB_GB_EXCEPTION_STR[] = {
		GB_GB_EXCEPTION_HEADER " Failed to allocate library",
		GB_GB_EXCEPTION_HEADER " Failed to transition to operational mode",
		GB_GB_EXCEPTION_HEADER " Failed to export BIOS",
		GB_GB_EXCEPTION_HEADER " Library is already initialized",
		GB_GB_EXCEPTION_HEADER " Library is uninitialized",
		GB_GB_EXCEPTION_HEADER " Unsupported cartridge type",
		};
		
	#define GB_GB_EXCEPTION_STRING(_TYPE_) \
		((_TYPE_) > GB_GB_EXCEPTION_MAX ? UNKNOWN_EXCEPTION : \
		GB_GB_EXCEPTION_STR[_TYPE_].c_str())
		
	#define THROW_GB_GB_EXCEPTION(_TYPE_) \
		THROW_GB_EXCEPTION(GB_GB_EXCEPTION_STRING(_TYPE_))
	#define THROW_GB_GB_EXCEPTION_MESSAGE(_TYPE_, _FORMAT_, ...) \
		THROW_GB_EXCEPTION_MESSAGE(GB_GB_EXCEPTION_STRING(_TYPE_), \
		_FORMAT_, __VA_ARGS__)
		
	class _gb;
	typedef _gb gb, *gb_ptr;
}

#endif // GB_TYPE_H_
