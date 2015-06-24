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

#include <cstdarg>
#include <stdexcept>
#include "../include/gb.h"
#include "../include/gb_except_type.h"

namespace GB_NS {

	#define GB_EXCEPT_MSG_MAX_LEN 0x400

	void 
	_throw_except(
		__in const std::string &except,
		__in const std::string &file,
		__in const std::string &line,
		__in const char *format,
		...
		)
	{
		int len;
		va_list lst;
		char *msg = NULL;
		std::stringstream res;

		res << except;

		if(format) {
			res << ": ";
			va_start(lst, format);
			len = std::min(vsnprintf(NULL, 0, format, lst), GB_EXCEPT_MSG_MAX_LEN);
			va_end(lst);
			va_start(lst, format);

			if(len > 0) {

				msg = new char[len];
				if(msg) {
					std::memset(msg, 0, sizeof(char) * len);

					len = vsnprintf(msg, len, format, lst);
					if(len > 0) {
						res << msg;
					} else if(!len) {
						res << GB_EXCEPTION_STRING(GB_EXCEPTION_EMPTY_MESSAGE);
					} else {
						res << GB_EXCEPTION_STRING(GB_EXCEPTION_INVALID_MESSAGE);
					}

					delete[] msg;
					msg = NULL;
				} else {
					res << GB_EXCEPTION_STRING(GB_EXCEPTION_ALLOC_FAILED);
				}
			} else if(!len) {
				res << GB_EXCEPTION_STRING(GB_EXCEPTION_EMPTY_MESSAGE);
			} else {
				res << GB_EXCEPTION_STRING(GB_EXCEPTION_INVALID_MESSAGE);
			}

			va_end(lst);
		}

#ifndef NDEBUG
		res << " (" << file << ":" << line << ")";
#endif // NDEBUG
		throw std::runtime_error(res.str().c_str());
	}
}
