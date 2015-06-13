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

#ifndef GB_EXCEPT_H_
#define GB_EXCEPT_H_

namespace GB_NS {

	#define GB_EXCEPTION_HEADER "(EXCEPTION)"
	#define UNKNOWN_EXCEPTION "Unknown exception"

	#define THROW_GB_EXCEPTION(_EXCEPT_) \
		_throw_except(_EXCEPT_, __FILE__, CAT_STR(__LINE__), NULL)
	#define THROW_GB_EXCEPTION_MESSAGE(_EXCEPT_, _FORMAT_, ...) \
		_throw_except(_EXCEPT_, __FILE__, CAT_STR(__LINE__), \
			_FORMAT_, __VA_ARGS__)

	extern void _throw_except(
		__in const std::string &except,
		__in const std::string &file,
		__in const std::string &line,
		__in const char *format,
		...
		);
}

#endif // GB_EXCEPT_H_
