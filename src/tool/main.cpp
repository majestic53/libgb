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

#include "../lib/include/gb.h"

int 
main(void) 
{
	int res = 0;

	std::cout << gb::version(true) << std::endl << std::endl;

	try {

		// TODO
		gb_ptr inst = gb::acquire();
		std::cout << inst->to_string(true) << std::endl;
		inst->initialize();
		std::cout << inst->to_string(true) << std::endl;
		inst->uninitialize();
		std::cout << inst->to_string(true) << std::endl;
		// ---

	} catch(std::runtime_error &exc) {
		std::cerr << exc.what() << std::endl;
		res = INVALID(int);
	}

	return res;
}
