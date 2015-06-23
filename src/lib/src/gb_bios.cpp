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

#include <fstream>
#include "../include/gb.h"
#include "../include/gb_type.h"

namespace GB_NS {

	void 
	bios_export(
		__in const std::string &path
		)
	{
		std::ofstream file(CHK_STR(path), std::ios::out | std::ios::binary);

		if(file) {
			THROW_GB_GB_EXCEPTION(GB_GB_EXCEPTION_BIOS_EXPORT_FAILED);
		}

		file.write((const char *) &GB_BIOS_RAW[0], GB_BIOS_RAW.size());
		file.close();
	}
}
