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
#include "../include/gb_rom_type.h"

namespace GB_NS {

	_gb_rom::_gb_rom(
		__in const std::string &in,
		__in_opt bool path
		)
	{
		set(in, path);
	}

	_gb_rom::_gb_rom(
		__in const _gb_rom &other
		) :
			m_buf(other.m_buf)
	{
		return;
	}

	_gb_rom::~_gb_rom(void)
	{
		m_buf.clear();
	}

	_gb_rom &
	_gb_rom::operator=(
		__in const _gb_rom &other
		)
	{

		if(this != &other) {
			m_buf = other.m_buf;
		}

		return *this;
	}

	gb_buf_t &
	_gb_rom::buffer(void)
	{
		return m_buf;
	}

	gb_cart_t 
	_gb_rom::cartridge(void)
	{

		if(GB_ROM_ADDR_CART_TYPE >= m_buf.size()) {
			THROW_GB_ROM_EXCEPTION_MESSAGE(GB_ROM_EXCEPTION_INVALID_ROM_OFFSET,
				"off=0x%04x", GB_ROM_ADDR_CART_TYPE);
		}

		return (gb_cart_t) m_buf.at(GB_ROM_ADDR_CART_TYPE);
	}

	std::string 
	_gb_rom::cartridge_type_as_string(
		__in gb_cart_t type
		)
	{
		std::string res;

		switch(type) {
			case GB_CART_ROM_ONLY:
				res = "ROM";
				break;
			case GB_CART_ROM_MBC1:
				res = "ROM+MBC1";
				break;
			case GB_CART_ROM_MBC1_RAM:
				res = "ROM+MBC1+RAM";
				break;
			case GB_CART_ROM_MBC1_RAM_BATT:
				res = "ROM+MBC1+RAM+BATT";
				break;
			case GB_CART_ROM_MBC2:
				res = "ROM+MBC2";
				break;
			case GB_CART_ROM_MBC2_BATT:
				res = "ROM+MBC2+BATT";
				break;
			case GB_CART_ROM_RAM:
				res = "ROM+RAM";
				break;
			case GB_CART_ROM_RAM_BATT:
				res = "ROM+RAM+BATT";
				break;
			case GB_CART_ROM_MMM01:
				res = "ROM+MMM01";
				break;
			case GB_CART_ROM_MMM01_SRAM:
				res = "ROM+MMM01+SRAM";
				break;
			case GB_CART_ROM_MMM01_SRAM_BATT:
				res = "ROM+MMM01+SRAM_BATT";
				break;
			case GB_CART_ROM_MBC3_RAM:
				res = "ROM+MBC3+RAM";
				break;
			case GB_CART_ROM_MBC3_RAM_BATT:
				res = "ROM+MBC3+RAM+BATT";
				break;
			case GB_CART_ROM_MBC5:
				res = "ROM+MBC5";
				break;
			case GB_CART_ROM_MBC5_RAM:
				res = "ROM+MBC5+RAM";
				break;
			case GB_CART_ROM_MBC5_RAM_BATT:
				res = "ROM+MBC5+RAM+BATT";
				break;
			case GB_CART_ROM_MBC5_RUMBLE:
				res = "ROM+MBC5+RUMBLE";
				break;
			case GB_CART_ROM_MBC5_RUMBLE_SRAM:
				res = "ROM+MBC5+RUMBLE+SRAM";
				break;
			case GB_CART_ROM_MBC5_RUMBLE_SRAM_BATT:
				res = "ROM+MBC5+RUMBLE+SRAM+BATT";
				break;
			case GB_CART_POCKET_CAM:
				res = "POCKET CAMERA";
				break;
			case GB_CART_BANDAI_TAMA5:
				res = "BANDAI TAMA5";
				break;
			case GB_CART_HUDSON_HUC3:
				res = "HUDSON HUC-3";
				break;
			default:
				res = UNKNOWN;
				break;
		}

		return res;
	}

	gbw_t 
	_gb_rom::checksum(void)
	{
		gbw_t res;
		gb_addr_t off = GB_ROM_ADDR_CHKSUM;

		if((off + 1) >= m_buf.size()) {
			THROW_GB_ROM_EXCEPTION_MESSAGE(GB_ROM_EXCEPTION_INVALID_ROM_OFFSET,
				"off=0x%04x-0x%04x", off, off + 1);
		}

		res = (m_buf.at(off) << BITS_PER_BYTE) | m_buf.at(off + 1);

		return res;
	}

	gbb_t 
	_gb_rom::complement(void)
	{

		if(GB_ROM_ADDR_COMP_CHK >= m_buf.size()) {
			THROW_GB_ROM_EXCEPTION_MESSAGE(GB_ROM_EXCEPTION_INVALID_ROM_OFFSET,
				"off=0x%04x", GB_ROM_ADDR_COMP_CHK);
		}

		return m_buf.at(GB_ROM_ADDR_COMP_CHK);
	}

	gb_dest_t 
	_gb_rom::destination(void)
	{

		if(GB_ROM_ADDR_DEST >= m_buf.size()) {
			THROW_GB_ROM_EXCEPTION_MESSAGE(GB_ROM_EXCEPTION_INVALID_ROM_OFFSET,
				"off=0x%04x", GB_ROM_ADDR_DEST);
		}

		return (gb_dest_t) m_buf.at(GB_ROM_ADDR_DEST);
	}

	std::string 
	_gb_rom::destination_type_as_string(
		__in gb_dest_t type
		)
	{
		std::string res;

		switch(type) {
			case GB_DEST_JAPAN:
				res = "JAPANESE";
				break;
			case GB_DEST_OTHER:
				res = "NON-JAPANESE";
				break;
			default:
				res = UNKNOWN;
				break;
		}

		return res;
	}

	gb_ind_t 
	_gb_rom::indicator(void)
	{

		if(GB_ROM_ADDR_IND >= m_buf.size()) {
			THROW_GB_ROM_EXCEPTION_MESSAGE(GB_ROM_EXCEPTION_INVALID_ROM_OFFSET,
				"off=0x%04x", GB_ROM_ADDR_IND);
		}

		return (gb_ind_t) m_buf.at(GB_ROM_ADDR_IND);
	}

	std::string 
	_gb_rom::indicator_type_as_string(
		__in gb_ind_t type
		)
	{
		std::string res;

		switch(type) {
			case GB_IND_GAMEBOY:
				res = "GAMEBOY";
				break;
			case GB_IND_SUPER_GAMEBOY:
				res = "SUPER_GAMEBOY";
				break;
			default:
				res = UNKNOWN;
				break;
		}

		return res;
	}

	gbb_t 
	_gb_rom::license(void)
	{
		char ch;
		gbb_t res;
		gb_addr_t off = GB_ROM_ADDR_LIC;

		if((off + 1) >= m_buf.size()) {
			THROW_GB_ROM_EXCEPTION_MESSAGE(GB_ROM_EXCEPTION_INVALID_ROM_OFFSET,
				"off=0x%04x-0x%04x", off, off + 1);
		}

		ch = std::tolower((char) m_buf.at(off));
		if(std::isalpha(ch)) {
			ch -= 'a';
		} else if(std::isdigit(ch)) {
			ch -= '0';
		} else {
			ch = 0;
		}

		res = (((gbb_t) ch) << BITS_PER_NIBBLE);

		ch = std::tolower((char) m_buf.at(off + 1));
		if(std::isalpha(ch)) {
			ch -= 'a';
		} else if(std::isdigit(ch)) {
			ch -= '0';
		} else {
			ch = 0;
		}

		res |= (((gbb_t) ch) & UINT4_MAX);

		return res;
	}

	gbb_t 
	_gb_rom::license_old(void)
	{

		if(GB_ROM_ADDR_LIC_OLD >= m_buf.size()) {
			THROW_GB_ROM_EXCEPTION_MESSAGE(GB_ROM_EXCEPTION_INVALID_ROM_OFFSET,
				"off=0x%04x", GB_ROM_ADDR_LIC_OLD);
		}

		return m_buf.at(GB_ROM_ADDR_LIC_OLD);
	}

	gbb_t 
	_gb_rom::mask_rom(void)
	{

		if(GB_ROM_ADDR_MROM_VER >= m_buf.size()) {
			THROW_GB_ROM_EXCEPTION_MESSAGE(GB_ROM_EXCEPTION_INVALID_ROM_OFFSET,
				"off=0x%04x", GB_ROM_ADDR_MROM_VER);
		}

		return m_buf.at(GB_ROM_ADDR_MROM_VER);
	}

	gb_ram_t 
	_gb_rom::ram(void)
	{

		if(GB_ROM_ADDR_RAM_SIZE >= m_buf.size()) {
			THROW_GB_ROM_EXCEPTION_MESSAGE(GB_ROM_EXCEPTION_INVALID_ROM_OFFSET,
				"off=0x%04x", GB_ROM_ADDR_RAM_SIZE);
		}

		return (gb_ram_t) m_buf.at(GB_ROM_ADDR_RAM_SIZE);
	}

	gbb_t 
	_gb_rom::ram_banks(void)
	{
		gbb_t res;

		switch(ram()) {
			case GB_RAM_NONE:
				res = 0;
				break;
			case GB_RAM_2KB:
				res = 1;
				break;
			case GB_RAM_8KB:
				res = 1;
				break;
			case GB_RAM_32KB:
				res = 4;
				break;
			case GB_RAM_128KB:
				res = 16;
				break;
			default:
				res = 0;
				break;
		}

		return res;
	}

	std::string 
	_gb_rom::ram_type_as_string(
		__in gb_ram_t type
		)
	{
		std::string res;

		switch(type) {
			case GB_RAM_NONE:
				res = "NONE";
				break;
			case GB_RAM_2KB:
				res = "2KB (1BNK)";
				break;
			case GB_RAM_8KB:
				res = "8KB (1BNK)";
				break;
			case GB_RAM_32KB:
				res = "32KB (4BNK)";
				break;
			case GB_RAM_128KB:
				res = "128KB (16BNK)";
				break;
			default:
				res = UNKNOWN;
				break;
		}

		return res;
	}

	gb_rom_t 
	_gb_rom::rom(void)
	{

		if(GB_ROM_ADDR_ROM_SIZE >= m_buf.size()) {
			THROW_GB_ROM_EXCEPTION_MESSAGE(GB_ROM_EXCEPTION_INVALID_ROM_OFFSET,
				"off=0x%04x", GB_ROM_ADDR_ROM_SIZE);
		}

		return (gb_rom_t) m_buf.at(GB_ROM_ADDR_ROM_SIZE);
	}

	gbb_t 
	_gb_rom::rom_banks(void)
	{
		gbb_t res;

		switch(rom()) {
			case GB_ROM_32KB:
				res = 2;
				break;
			case GB_ROM_64KB:
				res = 4;
				break;
			case GB_ROM_128KB:
				res = 8;
				break;
			case GB_ROM_256KB:
				res = 16;
				break;
			case GB_ROM_512KB:
				res = 32;
				break;
			case GB_ROM_1MB:
				res = 64;
				break;
			case GB_ROM_2MB:
				res = 128;
				break;
			case GB_ROM_1_1MB:
				res = 72;
				break;
			case GB_ROM_1_2MB:
				res = 80;
				break;
			case GB_ROM_1_5MB:
				res = 96;
				break;
			default:
				res = 0;
				break;
		}

		return res;
	}

	std::string 
	_gb_rom::rom_type_as_string(
		__in gb_rom_t type
		)
	{
		std::string res;

		switch(type) {
			case GB_ROM_32KB:
				res = "32KB (2BNK)";
				break;
			case GB_ROM_64KB:
				res = "64KB (4BNK)";
				break;
			case GB_ROM_128KB:
				res = "128KB (8BNK)";
				break;
			case GB_ROM_256KB:
				res = "256KB (16BNK)";
				break;
			case GB_ROM_512KB:
				res = "512KB (32BNK)";
				break;
			case GB_ROM_1MB:
				res = "1MB (64BNK)";
				break;
			case GB_ROM_2MB:
				res = "2MB (128BNK)";
				break;
			case GB_ROM_1_1MB:
				res = "1.1MB (72BNK)";
				break;
			case GB_ROM_1_2MB:
				res = "1.2MB (80BNK)";
				break;
			case GB_ROM_1_5MB:
				res = "1.5MB (96BNK)";
				break;
			default:
				res = UNKNOWN;
				break;
		}

		return res;
	}

	void 
	_gb_rom::set(
		__in const std::string &in,
		__in_opt bool path
		)
	{
		size_t len;

		if(path) {

			std::ifstream file(in.c_str(), std::ios::in | std::ios::binary);
			if(!file) {
				THROW_GB_ROM_EXCEPTION_MESSAGE(GB_ROM_EXCEPTION_FILE_NOT_FOUND,
					"path=%s", CHK_STR(in));
			}

			file.seekg(0, std::ios::end);
			len = file.tellg();
			file.seekg(0, std::ios::beg);
			m_buf.resize(len);
			file.read((char *) &m_buf[0], m_buf.size());
			file.close();
		} else {
			m_buf = gb_buf_t(in.begin(), in.end());
		}
	}

	gb_sys_t 
	_gb_rom::system(void)
	{

		if(GB_ROM_ADDR_TYPE >= m_buf.size()) {
			THROW_GB_ROM_EXCEPTION_MESSAGE(GB_ROM_EXCEPTION_INVALID_ROM_OFFSET,
				"off=0x%04x", GB_ROM_ADDR_TYPE);
		}

		return (gb_sys_t) m_buf.at(GB_ROM_ADDR_TYPE);
	}

	std::string 
	_gb_rom::system_type_as_string(
		__in gb_sys_t type
		)
	{
		std::string res;

		switch(type) {
			case GB_SYS_GAMEBOY:
				res = "GAMEBOY";
				break;
			case GB_SYS_GAMEBOY_COLOR:
				res = "GAMEBOY_COLOR";
				break;
			default:
				res = UNKNOWN;
				break;
		}

		return res;
	}

	std::string 
	_gb_rom::title(void)
	{
		gb_addr_t off;
		std::string res;

		for(off = GB_ROM_ADDR_TITLE; off < GB_ROM_ADDR_TYPE; ++off) {

			if(off >= m_buf.size()) {
				THROW_GB_ROM_EXCEPTION_MESSAGE(GB_ROM_EXCEPTION_INVALID_ROM_OFFSET,
					"off=0x%04x", off);
			}

			res += (char) m_buf.at(off);
		}

		return res;
	}

	std::string 
	_gb_rom::to_string(
		__in_opt bool verb
		)
	{
		std::stringstream res;

		res << GB_ROM_HEADER << " " << std::setprecision(3) << (float) (m_buf.size() / BYTES_PER_KBYTE) 
			<< "KB (" << m_buf.size() << " bytes)";

		if(verb) {
			res << std::endl << "--- TITLE=\'" << CHK_STR(title()) << "\'" << std::endl
			<< "--- SYSTEM=" << system_type_as_string(system()) << std::endl
			<< "--- LICENSE=0x" << VAL_AS_HEX(gbb_t, license()) << " (OLD=0x" 
			<< VAL_AS_HEX(gbb_t, license_old()) << ")" << std::endl
			<< "--- INDICATOR=" << indicator_type_as_string(indicator()) << std::endl
			<< "--- CARTRIDGE=" << cartridge_type_as_string(cartridge()) << std::endl
			<< "--- ROM=" << rom_type_as_string(rom()) << std::endl
			<< "--- RAM=" << ram_type_as_string(ram()) << std::endl
			<< "--- DESTINATION=" << destination_type_as_string(destination()) << std::endl
			<< "--- MASK_ROM_VER=0x" << VAL_AS_HEX(gbb_t, mask_rom()) << std::endl
			<< "--- COMPLEMENT=0x" << VAL_AS_HEX(gbb_t, complement()) << std::endl
			<< "--- CHECKSUM=0x" << VAL_AS_HEX(gbw_t, checksum());
		}

		return res.str();
	}
}
