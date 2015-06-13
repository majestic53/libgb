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

#ifndef GB_CMD_H_
#define GB_CMD_H_

namespace GB_NS {

	// ADC
	#define GB_CMD_ADC_A_A 0x8f
	#define GB_CMD_ADC_A_B 0x88
	#define GB_CMD_ADC_A_C 0x89
	#define GB_CMD_ADC_A_D 0x8a
	#define GB_CMD_ADC_A_E 0x8b
	#define GB_CMD_ADC_A_H 0x8c
	#define GB_CMD_ADC_A_L 0x8d
	#define GB_CMD_ADC_A_HL_INDIRECT 0x8e
	#define GB_CMD_ADC_A_N 0xce

	// ADD
	#define GB_CMD_ADD_A_A 0x87
	#define GB_CMD_ADD_A_B 0x80
	#define GB_CMD_ADD_A_C 0x81
	#define GB_CMD_ADD_A_D 0x82
	#define GB_CMD_ADD_A_E 0x83
	#define GB_CMD_ADD_A_H 0x84
	#define GB_CMD_ADD_A_L 0x85
	#define GB_CMD_ADD_A_HL_INDIRECT 0x86
	#define GB_CMD_ADD_A_N 0xc6
	#define GB_CMD_ADD_HL_BC 0x09
	#define GB_CMD_ADD_HL_DE 0x19
	#define GB_CMD_ADD_HL_HL 0x29
	#define GB_CMD_ADD_HL_SP 0x39
	#define GB_CMD_ADD_SP_N 0xe8

	// AND
	#define GB_CMD_AND_A 0xa7
	#define GB_CMD_AND_B 0xa0
	#define GB_CMD_AND_C 0xa1
	#define GB_CMD_AND_D 0xa2
	#define GB_CMD_AND_E 0xa3
	#define GB_CMD_AND_H 0xa4
	#define GB_CMD_AND_L 0xa5
	#define GB_CMD_AND_HL_INDIRECT 0xa6
	#define GB_CMD_AND_N 0xe6

	// BIT (extended)
	#define GB_CMD_BIT_0_A 0x47
	#define GB_CMD_BIT_0_B 0x40
	#define GB_CMD_BIT_0_C 0x41
	#define GB_CMD_BIT_0_D 0x42
	#define GB_CMD_BIT_0_E 0x43
	#define GB_CMD_BIT_0_H 0x44
	#define GB_CMD_BIT_0_L 0x45
	#define GB_CMD_BIT_0_HL_INDIRECT 0x46
	#define GB_CMD_BIT_1_A 0x4f
	#define GB_CMD_BIT_1_B 0x48
	#define GB_CMD_BIT_1_C 0x49
	#define GB_CMD_BIT_1_D 0x4a
	#define GB_CMD_BIT_1_E 0x4b
	#define GB_CMD_BIT_1_H 0x4c
	#define GB_CMD_BIT_1_L 0x4d
	#define GB_CMD_BIT_1_HL_INDIRECT 0x4e
	#define GB_CMD_BIT_2_A 0x57
	#define GB_CMD_BIT_2_B 0x50
	#define GB_CMD_BIT_2_C 0x51
	#define GB_CMD_BIT_2_D 0x52
	#define GB_CMD_BIT_2_E 0x53
	#define GB_CMD_BIT_2_H 0x54
	#define GB_CMD_BIT_2_L 0x55
	#define GB_CMD_BIT_2_HL_INDIRECT 0x56
	#define GB_CMD_BIT_3_A 0x5f
	#define GB_CMD_BIT_3_B 0x58
	#define GB_CMD_BIT_3_C 0x59
	#define GB_CMD_BIT_3_D 0x5a
	#define GB_CMD_BIT_3_E 0x5b
	#define GB_CMD_BIT_3_H 0x5c
	#define GB_CMD_BIT_3_L 0x5d
	#define GB_CMD_BIT_3_HL_INDIRECT 0x5e
	#define GB_CMD_BIT_4_A 0x67
	#define GB_CMD_BIT_4_B 0x60
	#define GB_CMD_BIT_4_C 0x61
	#define GB_CMD_BIT_4_D 0x62
	#define GB_CMD_BIT_4_E 0x63
	#define GB_CMD_BIT_4_H 0x64
	#define GB_CMD_BIT_4_L 0x65
	#define GB_CMD_BIT_4_HL_INDIRECT 0x66
	#define GB_CMD_BIT_5_A 0x6f
	#define GB_CMD_BIT_5_B 0x68
	#define GB_CMD_BIT_5_C 0x69
	#define GB_CMD_BIT_5_D 0x6a
	#define GB_CMD_BIT_5_E 0x6b
	#define GB_CMD_BIT_5_H 0x6c
	#define GB_CMD_BIT_5_L 0x6d
	#define GB_CMD_BIT_5_HL_INDIRECT 0x6e
	#define GB_CMD_BIT_6_A 0x77
	#define GB_CMD_BIT_6_B 0x70
	#define GB_CMD_BIT_6_C 0x71
	#define GB_CMD_BIT_6_D 0x72
	#define GB_CMD_BIT_6_E 0x73
	#define GB_CMD_BIT_6_H 0x74
	#define GB_CMD_BIT_6_L 0x75
	#define GB_CMD_BIT_6_HL_INDIRECT 0x76
	#define GB_CMD_BIT_7_A 0x7f
	#define GB_CMD_BIT_7_B 0x78
	#define GB_CMD_BIT_7_C 0x79
	#define GB_CMD_BIT_7_D 0x7a
	#define GB_CMD_BIT_7_E 0x7b
	#define GB_CMD_BIT_7_H 0x7c
	#define GB_CMD_BIT_7_L 0x7d
	#define GB_CMD_BIT_7_HL_INDIRECT 0x7e

	// CALL
	#define GB_CMD_CALL_NN 0xcd
	#define GB_CMD_CALL_NZ_NN 0xc4
	#define GB_CMD_CALL_Z_NN 0xcc
	#define GB_CMD_CALL_NC_NN 0xd4
	#define GB_CMD_CALL_C_NN 0xdc

	// CCF
	#define GB_CMD_CCF_S 0x3f

	// CP
	#define GB_CMD_CP_A 0xbf
	#define GB_CMD_CP_B 0xb8
	#define GB_CMD_CP_C 0xb9
	#define GB_CMD_CP_D 0xba
	#define GB_CMD_CP_E 0xbb
	#define GB_CMD_CP_H 0xbc
	#define GB_CMD_CP_L 0xbd
	#define GB_CMD_CP_HL_INDIRECT 0xbe
	#define GB_CMD_CP_N 0xfe

	// CPL
	#define GB_CMD_CPL_S 0x2f

	// DAA
	#define GB_CMD_DAA_S 0x27

	// DEC
	#define GB_CMD_DEC_A 0x3d
	#define GB_CMD_DEC_B 0x05
	#define GB_CMD_DEC_C 0x0d
	#define GB_CMD_DEC_D 0x15
	#define GB_CMD_DEC_E 0x1d
	#define GB_CMD_DEC_H 0x25
	#define GB_CMD_DEC_L 0x2d
	#define GB_CMD_DEC_HL_INDIRECT 0x35
	#define GB_CMD_DEC_BC 0x0b
	#define GB_CMD_DEC_DE 0x1b
	#define GB_CMD_DEC_HL 0x2b
	#define GB_CMD_DEC_SP 0x3b

	// DI
	#define GB_CMD_DI_S 0xf3

	// EI
	#define GB_CMD_EI_S 0xfb

	// Extended CMD prefix
	#define GB_CMD_EXT 0xcb

	// HALT
	#define GB_CMD_HALT_S 0x76

	// INC
	#define GB_CMD_INC_A 0x3c
	#define GB_CMD_INC_B 0x04
	#define GB_CMD_INC_C 0x0c		
	#define GB_CMD_INC_D 0x14
	#define GB_CMD_INC_E 0x1c		
	#define GB_CMD_INC_H 0x24
	#define GB_CMD_INC_L 0x2c
	#define GB_CMD_INC_HL_INDIRECT 0x34
	#define GB_CMD_INC_BC 0x03
	#define GB_CMD_INC_DE 0x13
	#define GB_CMD_INC_HL 0x23
	#define GB_CMD_INC_SP 0x33

	// JP
	#define GB_CMD_JP_NN 0xc3
	#define GB_CMD_JP_NZ_NN 0xc2
	#define GB_CMD_JP_Z_NN 0xca
	#define GB_CMD_JP_NC_NN 0xd2
	#define GB_CMD_JP_C_NN 0xda
	#define GB_CMD_JP_HL_INDIRECT 0xe9

	// JR
	#define GB_CMD_JR_N 0x18
	#define GB_CMD_JR_NZ_N 0x20		
	#define GB_CMD_JR_Z_N 0x28
	#define GB_CMD_JR_NC_N 0x30
	#define GB_CMD_JR_C_N 0x38

	// LD
	#define GB_CMD_LD_B_N 0x06
	#define GB_CMD_LD_C_N 0x0e
	#define GB_CMD_LD_D_N 0x16
	#define GB_CMD_LD_E_N 0x1e
	#define GB_CMD_LD_H_N 0x26
	#define GB_CMD_LD_L_N 0x2e
	#define GB_CMD_LD_A_A 0x7f
	#define GB_CMD_LD_A_B 0x78
	#define GB_CMD_LD_A_C 0x79
	#define GB_CMD_LD_A_D 0x7a
	#define GB_CMD_LD_A_E 0x7b
	#define GB_CMD_LD_A_H 0x7c
	#define GB_CMD_LD_A_L 0x7d
	#define GB_CMD_LD_A_HL_INDIRECT 0x7e
	#define GB_CMD_LD_B_B 0x40
	#define GB_CMD_LD_B_C 0x41
	#define GB_CMD_LD_B_D 0x42
	#define GB_CMD_LD_B_E 0x43
	#define GB_CMD_LD_B_H 0x44
	#define GB_CMD_LD_B_L 0x45
	#define GB_CMD_LD_B_HL_INDIRECT 0x46
	#define GB_CMD_LD_C_B 0x48
	#define GB_CMD_LD_C_C 0x49
	#define GB_CMD_LD_C_D 0x4a
	#define GB_CMD_LD_C_E 0x4b
	#define GB_CMD_LD_C_H 0x4c		
	#define GB_CMD_LD_C_L 0x4d
	#define GB_CMD_LD_C_HL_INDIRECT 0x4e
	#define GB_CMD_LD_D_B 0x50
	#define GB_CMD_LD_D_C 0x51
	#define GB_CMD_LD_D_D 0x52
	#define GB_CMD_LD_D_E 0x53
	#define GB_CMD_LD_D_H 0x54		
	#define GB_CMD_LD_D_L 0x55
	#define GB_CMD_LD_D_HL_INDIRECT 0x56
	#define GB_CMD_LD_E_B 0x58
	#define GB_CMD_LD_E_C 0x59
	#define GB_CMD_LD_E_D 0x5a
	#define GB_CMD_LD_E_E 0x5b
	#define GB_CMD_LD_E_H 0x5c		
	#define GB_CMD_LD_E_L 0x5d
	#define GB_CMD_LD_E_HL_INDIRECT 0x5e
	#define GB_CMD_LD_H_B 0x60
	#define GB_CMD_LD_H_C 0x61
	#define GB_CMD_LD_H_D 0x62
	#define GB_CMD_LD_H_E 0x63
	#define GB_CMD_LD_H_H 0x64		
	#define GB_CMD_LD_H_L 0x65
	#define GB_CMD_LD_H_HL_INDIRECT 0x66
	#define GB_CMD_LD_L_B 0x68
	#define GB_CMD_LD_L_C 0x69
	#define GB_CMD_LD_L_D 0x6a
	#define GB_CMD_LD_L_E 0x6b
	#define GB_CMD_LD_L_H 0x6c
	#define GB_CMD_LD_L_L 0x6d
	#define GB_CMD_LD_L_HL_INDIRECT 0x6e
	#define GB_CMD_LD_HL_INDIRECT_B 0x70
	#define GB_CMD_LD_HL_INDIRECT_C 0x71
	#define GB_CMD_LD_HL_INDIRECT_D 0x72
	#define GB_CMD_LD_HL_INDIRECT_E 0x73
	#define GB_CMD_LD_HL_INDIRECT_H 0x74
	#define GB_CMD_LD_HL_INDIRECT_L 0x75	
	#define GB_CMD_LD_HL_INDIRECT_N 0x36
	#define GB_CMD_LD_A_BC_INDIRECT 0x0a
	#define GB_CMD_LD_A_DE_INDIRECT 0x1a
	#define GB_CMD_LD_A_NN_INDIRECT 0xfa
	#define GB_CMD_LD_A_N 0x3e
	#define GB_CMD_LD_B_A 0x47
	#define GB_CMD_LD_C_A 0x4f
	#define GB_CMD_LD_D_A 0x57
	#define GB_CMD_LD_E_A 0x5f
	#define GB_CMD_LD_H_A 0x67
	#define GB_CMD_LD_L_A 0x6f
	#define GB_CMD_LD_BC_INDIRECT_A 0x02
	#define GB_CMD_LD_DE_INDIRECT_A 0x12
	#define GB_CMD_LD_HL_INDIRECT_A 0x77
	#define GB_CMD_LD_NN_INDIRECT_A 0xea
	#define GB_CMD_LD_A_C_INDIRECT 0xf2
	#define GB_CMD_LD_C_INDIRECT_A 0xe2
	#define GB_CMD_LD_BC_NN 0x01
	#define GB_CMD_LD_DE_NN 0x11
	#define GB_CMD_LD_HL_NN 0x21
	#define GB_CMD_LD_SP_NN 0x31
	#define GB_CMD_LD_SP_HL 0xf9
	#define GB_CMD_LD_NN_INDIRECT_SP 0x08

	// LDD
	#define GB_CMD_LDD_A_HL_INDIRECT 0x3a
	#define GB_CMD_LDD_HL_INDIRECT_A 0x32

	// LDH
	#define GB_CMD_LDH_N_INDIRECT_A 0xe0
	#define GB_CMD_LDH_A_N_INDIRECT 0xf0

	// LDHL
	#define GB_CMD_LDHL_SP_N 0xf8

	// LDI
	#define GB_CMD_LDI_A_HL_INDIRECT 0x2a
	#define GB_CMD_LDI_HL_INDIRECT_A 0x22

	// NOP
	#define GB_CMD_NOP_S 0x00

	// OR
	#define GB_CMD_OR_A 0xb7
	#define GB_CMD_OR_B 0xb0
	#define GB_CMD_OR_C 0xb1
	#define GB_CMD_OR_D 0xb2
	#define GB_CMD_OR_E 0xb3
	#define GB_CMD_OR_H 0xb4		
	#define GB_CMD_OR_L 0xb5
	#define GB_CMD_OR_HL_INDIRECT 0xb6
	#define GB_CMD_OR_N 0xf6

	// POP
	#define GB_CMD_POP_AF 0xf1
	#define GB_CMD_POP_BC 0xc1
	#define GB_CMD_POP_DE 0xd1
	#define GB_CMD_POP_HL 0xe1

	// PUSH
	#define GB_CMD_PUSH_AF 0xf5
	#define GB_CMD_PUSH_BC 0xc5
	#define GB_CMD_PUSH_DE 0xd5
	#define GB_CMD_PUSH_HL 0xe5

	// RES (extended)
	#define GB_CMD_RES_0_A 0x87
	#define GB_CMD_RES_0_B 0x80
	#define GB_CMD_RES_0_C 0x81
	#define GB_CMD_RES_0_D 0x82
	#define GB_CMD_RES_0_E 0x83
	#define GB_CMD_RES_0_H 0x84
	#define GB_CMD_RES_0_L 0x85
	#define GB_CMD_RES_0_HL_INDIRECT 0x86
	#define GB_CMD_RES_1_A 0x8f
	#define GB_CMD_RES_1_B 0x88
	#define GB_CMD_RES_1_C 0x89
	#define GB_CMD_RES_1_D 0x8a
	#define GB_CMD_RES_1_E 0x8b
	#define GB_CMD_RES_1_H 0x8c
	#define GB_CMD_RES_1_L 0x8d
	#define GB_CMD_RES_1_HL_INDIRECT 0x8e
	#define GB_CMD_RES_2_A 0x97
	#define GB_CMD_RES_2_B 0x90
	#define GB_CMD_RES_2_C 0x91
	#define GB_CMD_RES_2_D 0x92
	#define GB_CMD_RES_2_E 0x93
	#define GB_CMD_RES_2_H 0x94
	#define GB_CMD_RES_2_L 0x95
	#define GB_CMD_RES_2_HL_INDIRECT 0x96
	#define GB_CMD_RES_3_A 0x9f
	#define GB_CMD_RES_3_B 0x98
	#define GB_CMD_RES_3_C 0x99
	#define GB_CMD_RES_3_D 0x9a
	#define GB_CMD_RES_3_E 0x9b
	#define GB_CMD_RES_3_H 0x9c
	#define GB_CMD_RES_3_L 0x9d
	#define GB_CMD_RES_3_HL_INDIRECT 0x9e
	#define GB_CMD_RES_4_A 0xa7
	#define GB_CMD_RES_4_B 0xa0
	#define GB_CMD_RES_4_C 0xa1
	#define GB_CMD_RES_4_D 0xa2
	#define GB_CMD_RES_4_E 0xa3
	#define GB_CMD_RES_4_H 0xa4
	#define GB_CMD_RES_4_L 0xa5
	#define GB_CMD_RES_4_HL_INDIRECT 0xa6
	#define GB_CMD_RES_5_A 0xaf
	#define GB_CMD_RES_5_B 0xa8
	#define GB_CMD_RES_5_C 0xa9
	#define GB_CMD_RES_5_D 0xaa
	#define GB_CMD_RES_5_E 0xab
	#define GB_CMD_RES_5_H 0xac
	#define GB_CMD_RES_5_L 0xad
	#define GB_CMD_RES_5_HL_INDIRECT 0xae
	#define GB_CMD_RES_6_A 0xb7
	#define GB_CMD_RES_6_B 0xb0
	#define GB_CMD_RES_6_C 0xb1
	#define GB_CMD_RES_6_D 0xb2
	#define GB_CMD_RES_6_E 0xb3
	#define GB_CMD_RES_6_H 0xb4
	#define GB_CMD_RES_6_L 0xb5
	#define GB_CMD_RES_6_HL_INDIRECT 0xb6
	#define GB_CMD_RES_7_A 0xbf
	#define GB_CMD_RES_7_B 0xb8
	#define GB_CMD_RES_7_C 0xb9
	#define GB_CMD_RES_7_D 0xba
	#define GB_CMD_RES_7_E 0xbb
	#define GB_CMD_RES_7_H 0xbc
	#define GB_CMD_RES_7_L 0xbd
	#define GB_CMD_RES_7_HL_INDIRECT 0xbe

	// RET		
	#define GB_CMD_RET_S 0xc9
	#define GB_CMD_RET_NZ 0xc0
	#define GB_CMD_RET_Z 0xc8
	#define GB_CMD_RET_NC 0xd0
	#define GB_CMD_RET_C 0xd8

	// RETI
	#define GB_CMD_RETI_S 0xd9

	// RL (extended)
	#define GB_CMD_RL_A 0x17
	#define GB_CMD_RL_B 0x10
	#define GB_CMD_RL_C 0x11
	#define GB_CMD_RL_D 0x12
	#define GB_CMD_RL_E 0x13
	#define GB_CMD_RL_H 0x14		
	#define GB_CMD_RL_L 0x15
	#define GB_CMD_RL_HL_INDIRECT 0x16

	// RLC (extended)
	#define GB_CMD_RLC_A 0x07
	#define GB_CMD_RLC_B 0x00
	#define GB_CMD_RLC_C 0x01
	#define GB_CMD_RLC_D 0x02
	#define GB_CMD_RLC_E 0x03
	#define GB_CMD_RLC_H 0x04		
	#define GB_CMD_RLC_L 0x05
	#define GB_CMD_RLC_HL_INDIRECT 0x06

	// RR (extended)
	#define GB_CMD_RR_A 0x1f
	#define GB_CMD_RR_B 0x18
	#define GB_CMD_RR_C 0x19
	#define GB_CMD_RR_D 0x1a
	#define GB_CMD_RR_E 0x1b
	#define GB_CMD_RR_H 0x1c		
	#define GB_CMD_RR_L 0x1d
	#define GB_CMD_RR_HL_INDIRECT 0x1e

	// RRC (extended)
	#define GB_CMD_RRC_A 0x0f
	#define GB_CMD_RRC_B 0x08
	#define GB_CMD_RRC_C 0x09
	#define GB_CMD_RRC_D 0x0a
	#define GB_CMD_RRC_E 0x0b
	#define GB_CMD_RRC_H 0x0c		
	#define GB_CMD_RRC_L 0x0d
	#define GB_CMD_RRC_HL_INDIRECT 0x0e

	// RST
	#define GB_CMD_RST_0 0xc7
	#define GB_CMD_RST_8 0xcf
	#define GB_CMD_RST_10 0xd7		
	#define GB_CMD_RST_18 0xdf
	#define GB_CMD_RST_20 0xe7
	#define GB_CMD_RST_28 0xef
	#define GB_CMD_RST_30 0xf7
	#define GB_CMD_RST_38 0xff

	// SBC
	#define GB_CMD_SBC_A_A 0x9f
	#define GB_CMD_SBC_A_B 0x98
	#define GB_CMD_SBC_A_C 0x99
	#define GB_CMD_SBC_A_D 0x9a
	#define GB_CMD_SBC_A_E 0x9b
	#define GB_CMD_SBC_A_H 0x9c		
	#define GB_CMD_SBC_A_L 0x9d
	#define GB_CMD_SBC_A_HL_INDIRECT 0x9e
	#define GB_CMD_SBC_A_N 0xde

	// SCF
	#define GB_CMD_SCF_S 0x37

	// SET (extended)
	#define GB_CMD_SET_0_A 0xc7
	#define GB_CMD_SET_0_B 0xc0
	#define GB_CMD_SET_0_C 0xc1
	#define GB_CMD_SET_0_D 0xc2
	#define GB_CMD_SET_0_E 0xc3
	#define GB_CMD_SET_0_H 0xc4
	#define GB_CMD_SET_0_L 0xc5
	#define GB_CMD_SET_0_HL_INDIRECT 0xc6
	#define GB_CMD_SET_1_A 0xcf
	#define GB_CMD_SET_1_B 0xc8
	#define GB_CMD_SET_1_C 0xc9
	#define GB_CMD_SET_1_D 0xca
	#define GB_CMD_SET_1_E 0xcb
	#define GB_CMD_SET_1_H 0xcc
	#define GB_CMD_SET_1_L 0xcd
	#define GB_CMD_SET_1_HL_INDIRECT 0xce
	#define GB_CMD_SET_2_A 0xd7
	#define GB_CMD_SET_2_B 0xd0
	#define GB_CMD_SET_2_C 0xd1
	#define GB_CMD_SET_2_D 0xd2
	#define GB_CMD_SET_2_E 0xd3
	#define GB_CMD_SET_2_H 0xd4
	#define GB_CMD_SET_2_L 0xd5
	#define GB_CMD_SET_2_HL_INDIRECT 0xd6
	#define GB_CMD_SET_3_A 0xdf
	#define GB_CMD_SET_3_B 0xd8
	#define GB_CMD_SET_3_C 0xd9
	#define GB_CMD_SET_3_D 0xda
	#define GB_CMD_SET_3_E 0xdb
	#define GB_CMD_SET_3_H 0xdc
	#define GB_CMD_SET_3_L 0xdd
	#define GB_CMD_SET_3_HL_INDIRECT 0xde
	#define GB_CMD_SET_4_A 0xe7
	#define GB_CMD_SET_4_B 0xe0
	#define GB_CMD_SET_4_C 0xe1
	#define GB_CMD_SET_4_D 0xe2
	#define GB_CMD_SET_4_E 0xe3
	#define GB_CMD_SET_4_H 0xe4
	#define GB_CMD_SET_4_L 0xe5
	#define GB_CMD_SET_4_HL_INDIRECT 0xe6
	#define GB_CMD_SET_5_A 0xef
	#define GB_CMD_SET_5_B 0xe8
	#define GB_CMD_SET_5_C 0xe9
	#define GB_CMD_SET_5_D 0xea
	#define GB_CMD_SET_5_E 0xeb
	#define GB_CMD_SET_5_H 0xec
	#define GB_CMD_SET_5_L 0xed
	#define GB_CMD_SET_5_HL_INDIRECT 0xee
	#define GB_CMD_SET_6_A 0xf7
	#define GB_CMD_SET_6_B 0xf0
	#define GB_CMD_SET_6_C 0xf1
	#define GB_CMD_SET_6_D 0xf2
	#define GB_CMD_SET_6_E 0xf3
	#define GB_CMD_SET_6_H 0xf4
	#define GB_CMD_SET_6_L 0xf5
	#define GB_CMD_SET_6_HL_INDIRECT 0xf6
	#define GB_CMD_SET_7_A 0xff
	#define GB_CMD_SET_7_B 0xf8
	#define GB_CMD_SET_7_C 0xf9
	#define GB_CMD_SET_7_D 0xfa
	#define GB_CMD_SET_7_E 0xfb
	#define GB_CMD_SET_7_H 0xfc
	#define GB_CMD_SET_7_L 0xfd
	#define GB_CMD_SET_7_HL_INDIRECT 0xfe

	// SLA (extended)
	#define GB_CMD_SLA_A 0x27
	#define GB_CMD_SLA_B 0x20
	#define GB_CMD_SLA_C 0x21
	#define GB_CMD_SLA_D 0x22
	#define GB_CMD_SLA_E 0x23
	#define GB_CMD_SLA_H 0x24
	#define GB_CMD_SLA_L 0x25
	#define GB_CMD_SLA_HL_INDIRECT 0x26
	
	// SRA (extended)
	#define GB_CMD_SRA_A 0x2f
	#define GB_CMD_SRA_B 0x28
	#define GB_CMD_SRA_C 0x29
	#define GB_CMD_SRA_D 0x2a
	#define GB_CMD_SRA_E 0x2b
	#define GB_CMD_SRA_H 0x2c
	#define GB_CMD_SRA_L 0x2d
	#define GB_CMD_SRA_HL_INDIRECT 0x2e
	
	// SRL (extended)
	#define GB_CMD_SRL_A 0x3f
	#define GB_CMD_SRL_B 0x38
	#define GB_CMD_SRL_C 0x39
	#define GB_CMD_SRL_D 0x3a
	#define GB_CMD_SRL_E 0x3b
	#define GB_CMD_SRL_H 0x3c
	#define GB_CMD_SRL_L 0x3d
	#define GB_CMD_SRL_HL_INDIRECT 0x3e

	// STOP
	#define GB_CMD_STOP_S 0x10

	// SUB
	#define GB_CMD_SUB_A_A 0x97
	#define GB_CMD_SUB_A_B 0x90
	#define GB_CMD_SUB_A_C 0x91
	#define GB_CMD_SUB_A_D 0x92
	#define GB_CMD_SUB_A_E 0x93
	#define GB_CMD_SUB_A_H 0x94
	#define GB_CMD_SUB_A_L 0x95
	#define GB_CMD_SUB_A_HL_INDIRECT 0x96
	#define GB_CMD_SUB_A_N 0xd6

	// SWAP (extended)
	#define GB_CMD_SWAP_A 0x37
	#define GB_CMD_SWAP_B 0x30
	#define GB_CMD_SWAP_C 0x31
	#define GB_CMD_SWAP_D 0x32
	#define GB_CMD_SWAP_E 0x33
	#define GB_CMD_SWAP_H 0x34		
	#define GB_CMD_SWAP_L 0x35
	#define GB_CMD_SWAP_HL_INDIRECT 0x36

	// XOR
	#define GB_CMD_XOR_A 0xaf
	#define GB_CMD_XOR_B 0xa8
	#define GB_CMD_XOR_C 0xa9
	#define GB_CMD_XOR_D 0xaa
	#define GB_CMD_XOR_E 0xab
	#define GB_CMD_XOR_H 0xac		
	#define GB_CMD_XOR_L 0xad
	#define GB_CMD_XOR_HL_INDIRECT 0xae
	#define GB_CMD_XOR_N 0xee

	typedef enum {
		GB_CMD_ADC = 0, GB_CMD_ADD, GB_CMD_AND, GB_CMD_BIT, GB_CMD_CALL, 
		GB_CMD_CCF, GB_CMD_CP, GB_CMD_CPL, GB_CMD_DAA, GB_CMD_DEC, 
		GB_CMD_DI, GB_CMD_EI, GB_CMD_HALT, GB_CMD_INC, GB_CMD_JP, 
		GB_CMD_JR, GB_CMD_LD, GB_CMD_LDD, GB_CMD_LDH, GB_CMD_LDHL,
		GB_CMD_LDI, GB_CMD_NOP, GB_CMD_OR, GB_CMD_POP, GB_CMD_PUSH, 
		GB_CMD_RES, GB_CMD_RET, GB_CMD_RETI, GB_CMD_RL, GB_CMD_RLC, 
		GB_CMD_RR, GB_CMD_RRC, GB_CMD_RST, GB_CMD_SBC, GB_CMD_SCF, 
		GB_CMD_SET, GB_CMD_SLA, GB_CMD_SRA, GB_CMD_SRL, GB_CMD_STOP, 
		GB_CMD_SUB, GB_CMD_SWAP, GB_CMD_XOR,
	} gb_cmd_t;

	#define GB_CMD_MAX GB_CMD_XOR

	static const std::string GB_CMD_STR[] = {
		"ADC", "ADD", "AND", "BIT", "CALL", "CCF", "CP", "CPL",
		"DAA", "DEC", "DI", "EI", "HALT", "INC", "JP", "JR",
		"LD", "LDD", "LDH", "LDHL", "LDI", "NOP", "OR", "POP",
		"PUSH", "RES", "RET", "RETI", "RL", "RLC", "RR", "RRC", 
		"RST", "SBC", "SCF", "SET", "SLA", "SRA", "SRL", "STOP", 
		"SUB", "SWAP", "XOR",
		};

	#define GB_CMD_STRING(_TYPE_) \
		((_TYPE_) > GB_CMD_MAX ? UNKNOWN : \
		GB_CMD_STR[_TYPE_].c_str())

	#define GB_CMD_TYPE_EXT 1
	#define GB_CMD_TYPE_SINGLE 0

	static const gb_cmd_t GB_CMD_TY[][GBB_MAX_LEN + 1] = {

		// single
		{ 

			// 0x00
			GB_CMD_NOP, GB_CMD_LD, GB_CMD_LD, GB_CMD_INC, 
			GB_CMD_INC, GB_CMD_DEC, GB_CMD_LD, GB_CMD_RLC, 
			GB_CMD_LD, GB_CMD_ADD, GB_CMD_LD, GB_CMD_DEC, 
			GB_CMD_INC, GB_CMD_DEC, GB_CMD_LD, GB_CMD_RRC, 

			// 0x10
			GB_CMD_STOP, GB_CMD_LD, GB_CMD_LD, GB_CMD_INC, 
			GB_CMD_INC, GB_CMD_DEC, GB_CMD_LD, GB_CMD_RL, 
			GB_CMD_JR, GB_CMD_ADD, GB_CMD_LD, GB_CMD_DEC, 
			GB_CMD_INC, GB_CMD_DEC, GB_CMD_LD, GB_CMD_RR, 

			// 0x20
			GB_CMD_JR, GB_CMD_LD, GB_CMD_LDI, GB_CMD_INC, 
			GB_CMD_INC, GB_CMD_DEC, GB_CMD_LD, GB_CMD_DAA, 
			GB_CMD_JR, GB_CMD_ADD, GB_CMD_LDI, GB_CMD_DEC, 
			GB_CMD_INC, GB_CMD_DEC, GB_CMD_LD, GB_CMD_CPL, 

			// 0x30
			GB_CMD_JR, GB_CMD_LD, GB_CMD_LDD, GB_CMD_INC, 
			GB_CMD_INC, GB_CMD_DEC, GB_CMD_LD, GB_CMD_SCF, 
			GB_CMD_JR, GB_CMD_ADD, GB_CMD_LDD, GB_CMD_DEC, 
			GB_CMD_INC, GB_CMD_DEC, GB_CMD_LD, GB_CMD_CCF, 

			// 0x40
			GB_CMD_LD, GB_CMD_LD, GB_CMD_LD, GB_CMD_LD, 
			GB_CMD_LD, GB_CMD_LD, GB_CMD_LD, GB_CMD_LD, 
			GB_CMD_LD, GB_CMD_LD, GB_CMD_LD, GB_CMD_LD, 
			GB_CMD_LD, GB_CMD_LD, GB_CMD_LD, GB_CMD_LD, 

			// 0x50
			GB_CMD_LD, GB_CMD_LD, GB_CMD_LD, GB_CMD_LD, 
			GB_CMD_LD, GB_CMD_LD, GB_CMD_LD, GB_CMD_LD, 
			GB_CMD_LD, GB_CMD_LD, GB_CMD_LD, GB_CMD_LD, 
			GB_CMD_LD, GB_CMD_LD, GB_CMD_LD, GB_CMD_LD, 

			// 0x60
			GB_CMD_LD, GB_CMD_LD, GB_CMD_LD, GB_CMD_LD, 
			GB_CMD_LD, GB_CMD_LD, GB_CMD_LD, GB_CMD_LD, 
			GB_CMD_LD, GB_CMD_LD, GB_CMD_LD, GB_CMD_LD, 
			GB_CMD_LD, GB_CMD_LD, GB_CMD_LD, GB_CMD_LD, 

			// 0x70
			GB_CMD_LD, GB_CMD_LD, GB_CMD_LD, GB_CMD_LD, 
			GB_CMD_LD, GB_CMD_LD, GB_CMD_HALT, GB_CMD_LD, 
			GB_CMD_LD, GB_CMD_LD, GB_CMD_LD, GB_CMD_LD, 
			GB_CMD_LD, GB_CMD_LD, GB_CMD_LD, GB_CMD_LD, 

			// 0x80
			GB_CMD_ADD, GB_CMD_ADD, GB_CMD_ADD, GB_CMD_ADD, 
			GB_CMD_ADD, GB_CMD_ADD, GB_CMD_ADD, GB_CMD_ADD, 
			GB_CMD_ADC, GB_CMD_ADC, GB_CMD_ADC, GB_CMD_ADC, 
			GB_CMD_ADC, GB_CMD_ADC, GB_CMD_ADC, GB_CMD_ADC, 

			// 0x90
			GB_CMD_SUB, GB_CMD_SUB, GB_CMD_SUB, GB_CMD_SUB, 
			GB_CMD_SUB, GB_CMD_SUB, GB_CMD_SUB, GB_CMD_SUB, 
			GB_CMD_SBC, GB_CMD_SBC, GB_CMD_SBC, GB_CMD_SBC, 
			GB_CMD_SBC, GB_CMD_SBC, GB_CMD_SBC, GB_CMD_SBC, 

			// 0xa0
			GB_CMD_AND, GB_CMD_AND, GB_CMD_AND, GB_CMD_AND, 
			GB_CMD_AND, GB_CMD_AND, GB_CMD_AND, GB_CMD_AND, 
			GB_CMD_XOR, GB_CMD_XOR, GB_CMD_XOR, GB_CMD_XOR, 
			GB_CMD_XOR, GB_CMD_XOR, GB_CMD_XOR, GB_CMD_XOR, 

			// 0xb0
			GB_CMD_OR, GB_CMD_OR, GB_CMD_OR, GB_CMD_OR, 
			GB_CMD_OR, GB_CMD_OR, GB_CMD_OR, GB_CMD_OR, 
			GB_CMD_CP, GB_CMD_CP, GB_CMD_CP, GB_CMD_CP, 
			GB_CMD_CP, GB_CMD_CP, GB_CMD_CP, GB_CMD_CP, 

			// 0xc0
			GB_CMD_RET, GB_CMD_POP, GB_CMD_JP, GB_CMD_JP, 
			GB_CMD_CALL, GB_CMD_PUSH, GB_CMD_ADD, GB_CMD_RST, 
			GB_CMD_RET, GB_CMD_RET, GB_CMD_JP, INVALID(gb_cmd_t), 
			GB_CMD_CALL, GB_CMD_CALL, GB_CMD_ADC, GB_CMD_RST, 

			// 0xd0
			GB_CMD_RET, GB_CMD_POP, GB_CMD_JP, INVALID(gb_cmd_t), 
			GB_CMD_CALL, GB_CMD_PUSH, GB_CMD_SUB, GB_CMD_RST, 
			GB_CMD_RET, GB_CMD_RETI, GB_CMD_JP, INVALID(gb_cmd_t), 
			GB_CMD_CALL, INVALID(gb_cmd_t), GB_CMD_SBC, GB_CMD_RST, 

			// 0xe0
			GB_CMD_LDH, GB_CMD_POP, GB_CMD_LDH, INVALID(gb_cmd_t), 
			INVALID(gb_cmd_t), GB_CMD_PUSH, GB_CMD_AND, GB_CMD_RST, 
			GB_CMD_ADD, GB_CMD_JP, GB_CMD_LD, INVALID(gb_cmd_t), 
			INVALID(gb_cmd_t), INVALID(gb_cmd_t), GB_CMD_XOR, GB_CMD_RST, 

			// 0xf0
			GB_CMD_LDH, GB_CMD_POP, INVALID(gb_cmd_t), GB_CMD_DI, 
			INVALID(gb_cmd_t), GB_CMD_PUSH, GB_CMD_OR, GB_CMD_RST, 
			GB_CMD_LDHL, GB_CMD_LD, GB_CMD_LD, GB_CMD_EI, 
			INVALID(gb_cmd_t), INVALID(gb_cmd_t), GB_CMD_CP, GB_CMD_RST,
		  },
		
		// extended
		{

			// 0x00
			GB_CMD_RLC, GB_CMD_RLC, GB_CMD_RLC, GB_CMD_RLC, 
			GB_CMD_RLC, GB_CMD_RLC, GB_CMD_RLC, GB_CMD_RLC, 
			GB_CMD_RRC, GB_CMD_RRC, GB_CMD_RRC, GB_CMD_RRC, 
			GB_CMD_RRC, GB_CMD_RRC, GB_CMD_RRC, GB_CMD_RRC, 

			// 0x10
			GB_CMD_RL, GB_CMD_RL, GB_CMD_RL, GB_CMD_RL, 
			GB_CMD_RL, GB_CMD_RL, GB_CMD_RL, GB_CMD_RL, 
			GB_CMD_RR, GB_CMD_RR, GB_CMD_RR, GB_CMD_RR, 
			GB_CMD_RR, GB_CMD_RR, GB_CMD_RR, GB_CMD_RR, 

			// 0x20
			GB_CMD_SLA, GB_CMD_SLA, GB_CMD_SLA, GB_CMD_SLA, 
			GB_CMD_SLA, GB_CMD_SLA, GB_CMD_SLA, GB_CMD_SLA, 
			GB_CMD_SRA, GB_CMD_SRA, GB_CMD_SRA, GB_CMD_SRA, 
			GB_CMD_SRA, GB_CMD_SRA, GB_CMD_SRA, GB_CMD_SRA, 

			// 0x30
			GB_CMD_SWAP, GB_CMD_SWAP, GB_CMD_SWAP, GB_CMD_SWAP, 
			GB_CMD_SWAP, GB_CMD_SWAP, GB_CMD_SWAP, GB_CMD_SWAP, 
			GB_CMD_SRL, GB_CMD_SRL, GB_CMD_SRL, GB_CMD_SRL, 
			GB_CMD_SRL, GB_CMD_SRL, GB_CMD_SRL, GB_CMD_SRL, 

			// 0x40
			GB_CMD_BIT, GB_CMD_BIT, GB_CMD_BIT, GB_CMD_BIT, 
			GB_CMD_BIT, GB_CMD_BIT, GB_CMD_BIT, GB_CMD_BIT, 
			GB_CMD_BIT, GB_CMD_BIT, GB_CMD_BIT, GB_CMD_BIT, 
			GB_CMD_BIT, GB_CMD_BIT, GB_CMD_BIT, GB_CMD_BIT, 

			// 0x50
			GB_CMD_BIT, GB_CMD_BIT, GB_CMD_BIT, GB_CMD_BIT, 
			GB_CMD_BIT, GB_CMD_BIT, GB_CMD_BIT, GB_CMD_BIT, 
			GB_CMD_BIT, GB_CMD_BIT, GB_CMD_BIT, GB_CMD_BIT, 
			GB_CMD_BIT, GB_CMD_BIT, GB_CMD_BIT, GB_CMD_BIT, 

			// 0x60
			GB_CMD_BIT, GB_CMD_BIT, GB_CMD_BIT, GB_CMD_BIT, 
			GB_CMD_BIT, GB_CMD_BIT, GB_CMD_BIT, GB_CMD_BIT, 
			GB_CMD_BIT, GB_CMD_BIT, GB_CMD_BIT, GB_CMD_BIT, 
			GB_CMD_BIT, GB_CMD_BIT, GB_CMD_BIT, GB_CMD_BIT, 

			// 0x70
			GB_CMD_BIT, GB_CMD_BIT, GB_CMD_BIT, GB_CMD_BIT, 
			GB_CMD_BIT, GB_CMD_BIT, GB_CMD_BIT, GB_CMD_BIT, 
			GB_CMD_BIT, GB_CMD_BIT, GB_CMD_BIT, GB_CMD_BIT, 
			GB_CMD_BIT, GB_CMD_BIT, GB_CMD_BIT, GB_CMD_BIT, 

			// 0x80
			GB_CMD_RES, GB_CMD_RES, GB_CMD_RES, GB_CMD_RES, 
			GB_CMD_RES, GB_CMD_RES, GB_CMD_RES, GB_CMD_RES, 
			GB_CMD_RES, GB_CMD_RES, GB_CMD_RES, GB_CMD_RES, 
			GB_CMD_RES, GB_CMD_RES, GB_CMD_RES, GB_CMD_RES, 

			// 0x90
			GB_CMD_RES, GB_CMD_RES, GB_CMD_RES, GB_CMD_RES, 
			GB_CMD_RES, GB_CMD_RES, GB_CMD_RES, GB_CMD_RES, 
			GB_CMD_RES, GB_CMD_RES, GB_CMD_RES, GB_CMD_RES, 
			GB_CMD_RES, GB_CMD_RES, GB_CMD_RES, GB_CMD_RES, 

			// 0xa0
			GB_CMD_RES, GB_CMD_RES, GB_CMD_RES, GB_CMD_RES, 
			GB_CMD_RES, GB_CMD_RES, GB_CMD_RES, GB_CMD_RES, 
			GB_CMD_RES, GB_CMD_RES, GB_CMD_RES, GB_CMD_RES, 
			GB_CMD_RES, GB_CMD_RES, GB_CMD_RES, GB_CMD_RES, 

			// 0xb0
			GB_CMD_RES, GB_CMD_RES, GB_CMD_RES, GB_CMD_RES, 
			GB_CMD_RES, GB_CMD_RES, GB_CMD_RES, GB_CMD_RES, 
			GB_CMD_RES, GB_CMD_RES, GB_CMD_RES, GB_CMD_RES, 
			GB_CMD_RES, GB_CMD_RES, GB_CMD_RES, GB_CMD_RES, 

			// 0xc0
			GB_CMD_SET, GB_CMD_SET, GB_CMD_SET, GB_CMD_SET, 
			GB_CMD_SET, GB_CMD_SET, GB_CMD_SET, GB_CMD_SET, 
			GB_CMD_SET, GB_CMD_SET, GB_CMD_SET, GB_CMD_SET, 
			GB_CMD_SET, GB_CMD_SET, GB_CMD_SET, GB_CMD_SET, 

			// 0xd0
			GB_CMD_SET, GB_CMD_SET, GB_CMD_SET, GB_CMD_SET, 
			GB_CMD_SET, GB_CMD_SET, GB_CMD_SET, GB_CMD_SET, 
			GB_CMD_SET, GB_CMD_SET, GB_CMD_SET, GB_CMD_SET, 
			GB_CMD_SET, GB_CMD_SET, GB_CMD_SET, GB_CMD_SET, 

			// 0xe0
			GB_CMD_SET, GB_CMD_SET, GB_CMD_SET, GB_CMD_SET, 
			GB_CMD_SET, GB_CMD_SET, GB_CMD_SET, GB_CMD_SET, 
			GB_CMD_SET, GB_CMD_SET, GB_CMD_SET, GB_CMD_SET, 
			GB_CMD_SET, GB_CMD_SET, GB_CMD_SET, GB_CMD_SET, 

			// 0xf0
			GB_CMD_SET, GB_CMD_SET, GB_CMD_SET, GB_CMD_SET, 
			GB_CMD_SET, GB_CMD_SET, GB_CMD_SET, GB_CMD_SET, 
			GB_CMD_SET, GB_CMD_SET, GB_CMD_SET, GB_CMD_SET, 
			GB_CMD_SET, GB_CMD_SET, GB_CMD_SET, GB_CMD_SET, 
		},
	};

	#define GB_CMD_TYPE(_TYPE_, _CODE_) \
		((_TYPE_) > GB_CMD_TYPE_EXT ? INVALID(gb_cmd_t) : \
		(_CODE_) > GBB_MAX_LEN ? INVALID(gb_cmd_t) : \
		GB_CMD_TY[_TYPE_][_CODE_])
}

#endif // GB_CMD_H_
