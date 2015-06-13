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

#ifndef GB_EXCEPT_TYPE_H_
#define GB_EXCEPT_TYPE_H_

namespace GB_NS {	

	enum {
		GB_EXCEPTION_ALLOC_FAILED = 0,
		GB_EXCEPTION_EMPTY_MESSAGE,
		GB_EXCEPTION_INVALID_MESSAGE,
	};
	
	#define GB_EXCEPTION_MAX GB_EXCEPTION_INVALID_MESSAGE
	
	static const std::string GB_EXCEPTION_STR[] = {
		"Failed to allocate exception message",
		"Empty exception message",
		"Invalid exception message",
		};
		
	#define GB_EXCEPTION_STRING(_TYPE_) \
		((_TYPE_) > GB_EXCEPTION_MAX ? UNKNOWN_EXCEPTION : \
		GB_EXCEPTION_STR[_TYPE_].c_str())
}

#endif // GB_EXCEPT_TYPE_H_
