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

#ifndef GB_REG_H_
#define GB_REG_H_

namespace GB_NS {

	#define GB_FLAG_C 0x10
	#define GB_FLAG_H 0x20
	#define GB_FLAG_N 0x40
	#define GB_FLAG_Z 0x80

	#define GB_INT_HALT_INIT false
	#define GB_INT_IME_INIT true
	#define GB_INT_STOP_INIT false

	#define GB_REG_A_INIT 0
	#define GB_REG_A_ALT_INIT 0
	#define GB_REG_B_INIT 0
	#define GB_REG_B_ALT_INIT 0
	#define GB_REG_BGP 0xff47
	#define GB_REG_C_INIT 0
	#define GB_REG_C_ALT_INIT 0
	#define GB_REG_D_INIT 0
	#define GB_REG_D_ALT_INIT 0
	#define GB_REG_DIV 0xff04
	#define GB_REG_DMA 0xff46
	#define GB_REG_E_INIT 0
	#define GB_REG_E_ALT_INIT 0
	#define GB_REG_F_INIT 0
	#define GB_REG_F_ALT_INIT 0
	#define GB_REG_H_INIT 0
	#define GB_REG_H_ALT_INIT 0
	#define GB_REG_IE 0xffff
	#define GB_REG_IF 0xff0f
	#define GB_REG_L_INIT 0
	#define GB_REG_L_ALT_INIT 0
	#define GB_REG_LCDC 0xff40
	#define GB_REG_LY 0xff44
	#define GB_REG_LYC 0xff45
	#define GB_REG_NR_10 0xff10
	#define GB_REG_NR_11 0xff11
	#define GB_REG_NR_12 0xff12
	#define GB_REG_NR_13 0xff13
	#define GB_REG_NR_14 0xff14
	#define GB_REG_NR_21 0xff16
	#define GB_REG_NR_22 0xff17
	#define GB_REG_NR_23 0xff18
	#define GB_REG_NR_24 0xff19
	#define GB_REG_NR_30 0xff1a
	#define GB_REG_NR_31 0xff1b
	#define GB_REG_NR_32 0xff1c
	#define GB_REG_NR_33 0xff1d
	#define GB_REG_NR_34 0xff1e
	#define GB_REG_NR_41 0xff20
	#define GB_REG_NR_42 0xff21
	#define GB_REG_NR_43 0xff22
	#define GB_REG_NR_44 0xff23
	#define GB_REG_NR_50 0xff24
	#define GB_REG_NR_51 0xff25
	#define GB_REG_NR_52 0xff26
	#define GB_REG_OBP0 0xff48
	#define GB_REG_OBP1 0xff49
	#define GB_REG_P1 0xff00
	#define GB_REG_PC_INIT 0
	#define GB_REG_RST_VEC_0 0
	#define GB_REG_RST_VEC_8 0x8
	#define GB_REG_RST_VEC_10 0x10
	#define GB_REG_RST_VEC_18 0x18
	#define GB_REG_RST_VEC_20 0x20
	#define GB_REG_RST_VEC_28 0x28
	#define GB_REG_RST_VEC_30 0x30
	#define GB_REG_RST_VEC_38 0x38
	#define GB_REG_SB 0xff01
	#define GB_REG_SC 0xff02
	#define GB_REG_SCX 0xff43
	#define GB_REG_SCY 0xff42
	#define GB_REG_SP_INIT 0
	#define GB_REG_STAT 0xff41
	#define GB_REG_TIMA 0xff05
	#define GB_REG_TMA 0xff06
	#define GB_REG_TAC 0xff07
	#define GB_REG_WAVE_LOW 0xff30
	#define GB_REG_WAVE_HIGH 0xff3f
	#define GB_REG_WX 0xff4b
	#define GB_REG_WY 0xff4a

	#define GB_ROM_ADDR_START 0x0100 // - 0x0103
	#define GB_ROM_ADDR_GRX 0x0104 // - 0x0133
	#define GB_ROM_ADDR_TITLE 0x0134 // - 0x0142
	#define GB_ROM_ADDR_TYPE 0x0143
	#define GB_ROM_ADDR_LIC 0x0144 // - 0x0145
	#define GB_ROM_ADDR_IND 0x0146
	#define GB_ROM_ADDR_CART_TYPE 0x0147
	#define GB_ROM_ADDR_ROM_SIZE 0x0148
	#define GB_ROM_ADDR_RAM_SIZE 0x0149
	#define GB_ROM_ADDR_DEST 0x014a
	#define GB_ROM_ADDR_LIC_OLD 0x014b
	#define GB_ROM_ADDR_MROM_VER 0x014c
	#define GB_ROM_ADDR_COMP_CHK 0x014d
	#define GB_ROM_ADDR_CHKSUM 0x14e // - 0x014f
}

#endif // GB_REG_H_
