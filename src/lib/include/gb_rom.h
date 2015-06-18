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

#ifndef GB_ROM_H_
#define GB_ROM_H_

namespace GB_NS {

	typedef enum {
		GB_CART_ROM_ONLY = 0,
		GB_CART_ROM_MBC1,
		GB_CART_ROM_MBC1_RAM,
		GB_CART_ROM_MBC1_RAM_BATT,
		GB_CART_ROM_MBC2 = 5,
		GB_CART_ROM_MBC2_BATT,
		GB_CART_ROM_RAM = 8,
		GB_CART_ROM_RAM_BATT,
		GB_CART_ROM_MMM01 = 0xb,
		GB_CART_ROM_MMM01_SRAM,
		GB_CART_ROM_MMM01_SRAM_BATT,
		GB_CART_ROM_MBC3_RAM = 0x12,
		GB_CART_ROM_MBC3_RAM_BATT,
		GB_CART_ROM_MBC5 = 0x19,
		GB_CART_ROM_MBC5_RAM,
		GB_CART_ROM_MBC5_RAM_BATT,
		GB_CART_ROM_MBC5_RUMBLE,
		GB_CART_ROM_MBC5_RUMBLE_SRAM,
		GB_CART_ROM_MBC5_RUMBLE_SRAM_BATT,
		GB_CART_POCKET_CAM,
		GB_CART_BANDAI_TAMA5 = 0xfd,
		GB_CART_HUDSON_HUC3 = 0xfe,
	} gb_cart_t;

	typedef enum {
		GB_DEST_JAPAN = 0,
		GB_DEST_OTHER,
	} gb_dest_t;

	typedef enum {
		GB_IND_GAMEBOY = 0,
		GB_IND_SUPER_GAMEBOY = 3,
	} gb_ind_t;

	typedef enum {
		GB_RAM_NONE = 0,
		GB_RAM_2KB,
		GB_RAM_8KB,
		GB_RAM_32KB,
		GB_RAM_128KB,
	} gb_ram_t;

	typedef enum {
		GB_ROM_32KB = 0,
		GB_ROM_64KB,
		GB_ROM_128KB,
		GB_ROM_256KB,
		GB_ROM_512KB,
		GB_ROM_1MB,
		GB_ROM_2MB,
		GB_ROM_1_1MB = 0x52,
		GB_ROM_1_2MB,
		GB_ROM_1_5MB,
	} gb_rom_t;

	typedef enum {
		GB_SYS_GAMEBOY = 0,
		GB_SYS_GAMEBOY_COLOR = 0x80,
	} gb_sys_t;

	typedef class _gb_rom {

		public:

			_gb_rom(
				__in const std::string &in,
				__in_opt bool path = true
				);

			_gb_rom(
				__in const _gb_rom &other
				);

			virtual ~_gb_rom(void);

			_gb_rom &operator=(
				__in const _gb_rom &other
				);

			gb_buf_t &buffer(void);

			gb_cart_t cartridge(void);

			gbw_t checksum(void);

			gbb_t complement(void);

			gb_dest_t destination(void);

			gb_ind_t indicator(void);

			gbb_t license(void);

			gbb_t license_old(void);

			gbb_t mask_rom(void);

			gb_ram_t ram(void);

			gbb_t ram_banks(void);

			gb_rom_t rom(void);

			gbb_t rom_banks(void);

			void set(
				__in const std::string &in,
				__in_opt bool path = true
				);

			gb_sys_t system(void);

			std::string title(void);

			virtual std::string to_string(
				__in_opt bool verb = false
				);

		protected:

			static std::string cartridge_type_as_string(
				__in gb_cart_t type
				);

			static std::string destination_type_as_string(
				__in gb_dest_t type
				);

			static std::string indicator_type_as_string(
				__in gb_ind_t type
				);

			static std::string ram_type_as_string(
				__in gb_ram_t type
				);

			static std::string rom_type_as_string(
				__in gb_rom_t type
				);

			static std::string system_type_as_string(
				__in gb_sys_t type
				);

			gb_buf_t m_buf;

	} gb_rom, *gb_rom_ptr;
}

#endif // GB_ROM_H_
