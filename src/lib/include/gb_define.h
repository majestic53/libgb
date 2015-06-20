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

#ifndef GB_DEFINE_H_
#define GB_DEFINE_H_

#include <cstring>
#include <cstdbool>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>
#include <GLFW/glfw3.h>

namespace GB_NS {

	#define __in
	#define __in_opt
	#define __out
	#define __out_opt
	#define __inout
	#define __inout_opt

	#define BLD_INFO "Copyright (C) 2015 David Jolly"
	#define BLD_NAME "LIBGB"
	#define BLD_VER_MAJ 0
	#define BLD_VER_MIN 1
	#define BLD_VER_REV 6
	#define BLD_VER_WEEK 1525
	#define BLK_LEN 0x10
	#define BITS_PER_BYTE 8
	#define BITS_PER_NIBBLE 4
	#define BYTES_PER_KBYTE (1024.0)
	#define GCD_MAX 9
	#define EMPTY "EMPTY"
	#define UINT4_MAX 0xf
	#define UNKNOWN "UNKNOWN"
	#define VER_STR \
		CAT_STR(BLD_VER_MAJ) "." CAT_STR(BLD_VER_MIN) "." \
		CAT_STR(BLD_VER_WEEK) "." CAT_STR(BLD_VER_REV)
	#define VER_VERB_STR \
		BLD_NAME " " VER_STR "\n" BLD_INFO

	#define CHK_STR(_STR_) \
		((_STR_).empty() ? EMPTY : (_STR_).c_str())

	#define _CAT_STR(_STR_) # _STR_
	#define CAT_STR(_STR_) _CAT_STR(_STR_)

	#define INVALID(_TYPE_) ((_TYPE_) -1)

	#define UNREF_PARAM(_PARAM_) (void) _PARAM_

	#define VAL_AS_HEX(_TYPE_, _VAL_) \
		std::setw(sizeof(_TYPE_) * 2) << std::setfill('0') << std::hex \
		<< (uintmax_t) ((_TYPE_) (_VAL_)) << std::dec

	#define GBB_MAX_LEN UINT8_MAX

	typedef uint8_t gbb_t;

	#define GBW_MAX_LEN UINT16_MAX

	typedef uint16_t gbw_t;

	#define GBDW_MAX_LEN UINT32_MAX

	typedef uint32_t gbdw_t;

	#define GB_MMU_MAX_ADDR GBW_MAX_LEN

	typedef uint16_t gb_addr_t;

	typedef std::vector<gbb_t> gb_buf_t;
}

#endif // GB_DEFINE_H_
