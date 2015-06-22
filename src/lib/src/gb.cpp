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

	bool gb::m_active = false;
	bool gb::m_ime = GB_INT_IME_INIT;
	gb_ptr gb::m_inst = NULL;

	_gb::_gb(void) :
		m_init(false),
		m_inst_cpu(gb_cpu::acquire()),
		m_inst_gpu(gb_gpu::acquire()),
		m_inst_joy(gb_joy::acquire()),
		m_inst_mmu(gb_mmu::acquire())
	{
		std::atexit(gb::_delete);
	}

	_gb::~_gb(void)
	{

		if(m_init) {
			release_cpu();
			release_gpu();
			release_mmu();
			release_joy();
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

	gb_gpu_ptr 
	_gb::acquire_gpu(void)
	{

		if(!m_inst_gpu) {
			m_inst_gpu = gb_gpu::acquire();
		}

		return m_inst_gpu;
	}

	gb_joy_ptr 
	_gb::acquire_joy(void)
	{

		if(!m_inst_joy) {
			m_inst_joy = gb_joy::acquire();
		}

		return m_inst_joy;
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

		m_inst_joy->initialize();
		m_inst_mmu->initialize();
		m_inst_gpu->initialize();
		m_inst_cpu->initialize();
		m_init = true;
	}

	bool &
	_gb::interrupt_master_enable(void)
	{
		return m_ime;
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
	_gb::release_gpu(void)
	{

		if(gb_gpu::is_allocated()) {
			gb_gpu::_delete();
			m_inst_gpu = NULL;
		}
	}

	void 
	_gb::release_joy(void)
	{

		if(gb_joy::is_allocated()) {
			gb_joy::_delete();
			m_inst_joy = NULL;
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
	_gb::start(
		__in const std::string &in,
		__in_opt bool path
		)
	{
		bool bios = true;
		gb_rom rom(in, path);

		if(!m_init) {
			THROW_GB_GB_EXCEPTION(GB_GB_EXCEPTION_UNINITIALIZED);
		}

		if(rom.cartridge() != GB_CART_ROM_ONLY) {
			THROW_GB_GB_EXCEPTION_MESSAGE(GB_GB_EXCEPTION_UNSUPPORTED,
				"%s (0x%x)", CHK_STR(gb_rom::cartridge_as_string(rom.cartridge())), 
				rom.cartridge());
		}

		m_active = true;
		m_inst_gpu->start(rom.title(), true);
		m_inst_mmu->insert(0, rom.buffer());
		m_inst_mmu->insert(0, GB_BIOS_RAW);
		m_inst_cpu->start();

		while(m_active) {

			if(!m_inst_cpu->is_halted() && !m_inst_cpu->is_stopped()) {

				if(bios && !m_inst_cpu->is_zero_page()) {
					bios = false;
					m_inst_mmu->insert(0, gb_buf_t(rom.buffer().begin(), 
						rom.buffer().begin() + (GB_BIOS_ADDR_STOP + 1)));

					std::cout << m_inst_mmu->to_string(true, 0, 0x8000) << std::endl;

					// TODO: set all registers to post-bios defaults
					std::cin.get();
					//

				} else {

					// TODO: step CPU here
					m_inst_cpu->step();
					//sleep(1);
					// ---
				}
			} else {

				// TODO: busy wait, fix this!
			}
		}

		m_inst_gpu->stop();
	}

	void 
	_gb::stop(void)
	{
		m_active = false;
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
			<< std::endl << "--- " << m_inst_gpu->to_string(verb)
			<< std::endl << "--- " << m_inst_joy->to_string(verb)
			<< std::endl << "--- " << m_inst_mmu->to_string(verb, addr, off);

		return res.str();
	}

	void 
	_gb::uninitialize(void)
	{

		if(!m_init) {
			THROW_GB_GB_EXCEPTION(GB_GB_EXCEPTION_UNINITIALIZED);
		}

		m_inst_cpu->uninitialize();
		m_inst_gpu->uninitialize();
		m_inst_mmu->uninitialize();
		m_inst_joy->uninitialize();
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
