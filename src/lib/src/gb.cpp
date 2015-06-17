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

#include "../include/gb.h"
#include "../include/gb_type.h"

namespace GB_NS {

	static const gb_buf_t GB_BIOS = {
		0x31, 0xfe, 0xff, 0xaf, 0x21, 0xff, 0x9f, 0x32, 0xcb, 0x7c, 0x20, 0xfb, 0x21, 0x26, 0xff, 0x0e,
		0x11, 0x3e, 0x80, 0x32, 0xe2, 0x0c, 0x3e, 0xf3, 0xe2, 0x32, 0x3e, 0x77, 0x77, 0x3e, 0xfc, 0xe0,
		0x47, 0x11, 0x04, 0x01, 0x21, 0x10, 0x80, 0x1a, 0xcd, 0x95, 0x00, 0xcd, 0x96, 0x00, 0x13, 0x7b,
		0xfe, 0x34, 0x20, 0xf3, 0x11, 0xd8, 0x00, 0x06, 0x08, 0x1a, 0x13, 0x22, 0x23, 0x05, 0x20, 0xf9,
		0x3e, 0x19, 0xea, 0x10, 0x99, 0x21, 0x2f, 0x99, 0x0e, 0x0c, 0x3d, 0x28, 0x08, 0x32, 0x0d, 0x20,
		0xf9, 0x2e, 0x0f, 0x18, 0xf3, 0x67, 0x3e, 0x64, 0x57, 0xe0, 0x42, 0x3e, 0x91, 0xe0, 0x40, 0x04,
		0x1e, 0x02, 0x0e, 0x0c, 0xf0, 0x44, 0xfe, 0x90, 0x20, 0xfa, 0x0d, 0x20, 0xf7, 0x1d, 0x20, 0xf2,
		0x0e, 0x13, 0x24, 0x7c, 0x1e, 0x83, 0xfe, 0x62, 0x28, 0x06, 0x1e, 0xc1, 0xfe, 0x64, 0x20, 0x06,
		0x7b, 0xe2, 0x0c, 0x3e, 0x87, 0xf2, 0xf0, 0x42, 0x90, 0xe0, 0x42, 0x15, 0x20, 0xd2, 0x05, 0x20,
		0x4f, 0x16, 0x20, 0x18, 0xcb, 0x4f, 0x06, 0x04, 0xc5, 0xcb, 0x11, 0x17, 0xc1, 0xcb, 0x11, 0x17,
		0x05, 0x20, 0xf5, 0x22, 0x23, 0x22, 0x23, 0xc9, 0xce, 0xed, 0x66, 0x66, 0xcc, 0x0d, 0x00, 0x0b,
		0x03, 0x73, 0x00, 0x83, 0x00, 0x0c, 0x00, 0x0d, 0x00, 0x08, 0x11, 0x1f, 0x88, 0x89, 0x00, 0x0e,
		0xdc, 0xcc, 0x6e, 0xe6, 0xdd, 0xdd, 0xd9, 0x99, 0xbb, 0xbb, 0x67, 0x63, 0x6e, 0x0e, 0xec, 0xcc,
		0xdd, 0xdc, 0x99, 0x9f, 0xbb, 0xb9, 0x33, 0x3e, 0x3c, 0x42, 0xb9, 0xa5, 0xb9, 0xa5, 0x42, 0x4c,
		0x21, 0x04, 0x01, 0x11, 0xa8, 0x00, 0x1a, 0x13, 0xbe, 0x20, 0xfe, 0x23, 0x7d, 0xfe, 0x34, 0x20,
		0xf5, 0x06, 0x19, 0x78, 0x86, 0x23, 0x05, 0x20, 0xfb, 0x86, 0x20, 0xfe, 0x3e, 0x01, 0xe0, 0x50,
	};

	gb_ptr gb::m_inst = NULL;

	_gb::_gb(void) :
		m_init(false),
		m_inst_cpu(gb_cpu::acquire()),
		m_inst_mmu(gb_mmu::acquire())
	{
		std::atexit(gb::_delete);
	}

	_gb::~_gb(void)
	{

		if(m_init) {

			// TODO

			release_cpu();
			release_mmu();
			uninitialize();
		}
	}

	void 
	_gb::_delete(void)
	{

		if(gb::m_inst) {
			delete gb::m_inst;
			gb::m_inst = NULL;
		}
	}

	gb_ptr 
	_gb::acquire(void)
	{

		if(!gb::m_inst) {

			gb::m_inst = new gb;
			if(!gb::m_inst) {
				THROW_GB_GB_EXCEPTION(GB_GB_EXCEPTION_ALLOC_FAILED);
			}
		}

		return gb::m_inst;
	}

	gb_cpu_ptr 
	_gb::acquire_cpu(void)
	{

		if(!m_inst_cpu) {
			m_inst_cpu = gb_cpu::acquire();
		}

		return m_inst_cpu;
	}

	gb_mmu_ptr 
	_gb::acquire_mmu(void)
	{

		if(!m_inst_mmu) {
			m_inst_mmu = gb_mmu::acquire();
		}

		return m_inst_mmu;
	}

	void 
	_gb::initialize(void)
	{

		if(m_init) {
			THROW_GB_GB_EXCEPTION(GB_GB_EXCEPTION_INITIALIZED);
		}

		m_inst_mmu->initialize();
		m_inst_cpu->initialize();

		// TODO

		m_init = true;
	}

	bool 
	_gb::is_allocated(void)
	{
		return (gb::m_inst != NULL);
	}

	bool 
	_gb::is_initialized(void)
	{
		return m_init;
	}

	void 
	_gb::release_cpu(void)
	{

		if(gb_cpu::is_allocated()) {
			gb_cpu::_delete();
			m_inst_cpu = NULL;
		}
	}

	void 
	_gb::release_mmu(void)
	{

		if(gb_mmu::is_allocated()) {
			gb_mmu::_delete();
			m_inst_mmu = NULL;
		}
	}

	void 
	_gb::run(
		__in const gb_buf_t &cart
		)
	{
		// TODO: startup
		// TODO: run cart in loop (sync tick between components, checking for stop/halt/etc.)
		// TODO: cleanup
	}

	std::string 
	_gb::to_string(
		__in_opt bool verb,
		__in_opt gb_addr_t addr,
		__in_opt gbw_t off
		)
	{
		std::stringstream res;

		res << "[" << (m_init ? "INIT" : "UNINIT") << "] " << GB_HEADER
			<< " (ptr=0x" << VAL_AS_HEX(gb_ptr, this) << ")"
			<< std::endl << "--- " << m_inst_cpu->to_string(verb)
			<< std::endl << "--- " << m_inst_mmu->to_string(verb, addr, off);

		// TODO

		return res.str();
	}

	void 
	_gb::uninitialize(void)
	{

		if(!m_init) {
			THROW_GB_GB_EXCEPTION(GB_GB_EXCEPTION_UNINITIALIZED);
		}

		// TODO

		m_inst_cpu->uninitialize();
		m_inst_mmu->uninitialize();
		m_init = false;
	}

	std::string 
	_gb::version(
		__in_opt bool verb
		)
	{
		return verb ? VER_VERB_STR : VER_STR;
	}
}
