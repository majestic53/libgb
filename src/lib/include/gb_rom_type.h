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

#ifndef GB_ROM_TYPE_H_
#define GB_ROM_TYPE_H_

namespace GB_NS {

	#define GB_ROM_HEADER "(ROM)"

	#ifndef NDEBUG
	#define GB_ROM_EXCEPTION_HEADER GB_ROM_HEADER
	#else
	#define GB_ROM_EXCEPTION_HEADER GB_EXCEPTION_HEADER
	#endif // NDEBUG

	enum {
		GB_ROM_EXCEPTION_FILE_NOT_FOUND = 0,
		GB_ROM_EXCEPTION_INVALID_ROM_OFFSET,
		GB_ROM_EXCEPTION_MALFORMED_ROM,
	};

	#define GB_ROM_EXCEPTION_MAX GB_ROM_EXCEPTION_MALFORMED_ROM

	static const std::string GB_ROM_EXCEPTION_STR[] = {
		GB_ROM_EXCEPTION_HEADER " File does not exist",
		GB_ROM_EXCEPTION_HEADER " Invalid rom offset",
		GB_ROM_EXCEPTION_HEADER " Malformed rom file",
		};

	#define GB_ROM_EXCEPTION_STRING(_TYPE_) \
		((_TYPE_) > GB_ROM_EXCEPTION_MAX ? UNKNOWN_EXCEPTION : \
		GB_ROM_EXCEPTION_STR[_TYPE_].c_str())

	#define THROW_GB_ROM_EXCEPTION(_TYPE_) \
		THROW_GB_EXCEPTION(GB_ROM_EXCEPTION_STRING(_TYPE_))
	#define THROW_GB_ROM_EXCEPTION_MESSAGE(_TYPE_, _FORMAT_, ...) \
		THROW_GB_EXCEPTION_MESSAGE(GB_ROM_EXCEPTION_STRING(_TYPE_), _FORMAT_, __VA_ARGS__)

	class _gb_rom;
	typedef _gb_rom gb_rom, *gb_rom_ptr;
}

#endif // GB_ROM_TYPE_H_
