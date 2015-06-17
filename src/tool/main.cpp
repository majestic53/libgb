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

// TODO: DEBUG
/*
org $0000
init:
	set 3, c	; cb d9
	jp test		; c3 10 00
org $0010
test:
	ld a, $01	; 3e 01
	ld b, $0f	; 06 0f
	add a, b	; 80
	halt		; 76
 */

static const gb_buf_t test = { 
	0xcb, 0xd9, 0xc3, 0x10, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x3e, 0x01, 0x06, 0x0f, 0x80, 0x76, 
};

#define TEST_CMD_COUNT 6
// ---

int 
main(void) 
{
	gb_ptr inst = NULL;
	int iter = 0, res = 0;
	gb_cpu_ptr cpu_inst = NULL;
	gb_mmu_ptr mmu_inst = NULL;

	std::cout << gb::version(true) << std::endl << "----------------------" 
		<< std::endl;

	try {
		inst = gb::acquire();
		inst->initialize();

		// TODO: DEBUG
		cpu_inst = inst->acquire_cpu();
		mmu_inst = inst->acquire_mmu();

		cpu_inst->reset();
		mmu_inst->insert(0x0000, test);

		for(; iter < TEST_CMD_COUNT; ++iter) {
			cpu_inst->step();
		}
		
		std::cout << inst->to_string(true, 0x0000, test.size()) << std::endl;
		// ---

		inst->uninitialize();
	} catch(std::runtime_error &exc) {
		std::cerr << exc.what() << std::endl;
		res = INVALID(int);
	}

	return res;
}
