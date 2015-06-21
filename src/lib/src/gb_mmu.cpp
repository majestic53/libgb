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
#include "../include/gb_mmu_type.h"

namespace GB_NS {

	namespace GB_COMP_NS {

		gb_mmu_ptr gb_mmu::m_inst = NULL;
		
		_gb_mmu::_gb_mmu(void) :
			m_init(false),
			m_joy(gb_joy::acquire())
		{
			std::atexit(gb_mmu::_delete);
		}
		
		_gb_mmu::~_gb_mmu(void)
		{
		
			if(m_init) {
				uninitialize();
			}
		}
	
		void 
		_gb_mmu::_delete(void)
		{
		
			if(gb_mmu::m_inst) {
				delete gb_mmu::m_inst;
				gb_mmu::m_inst = NULL;
			}
		}

		gb_joy_ptr 
		_gb_mmu::_joystick(void)
		{
			return m_joy;
		}

		gb_mmu_ptr 
		_gb_mmu::acquire(void)
		{
		
			if(!gb_mmu::m_inst) {
			
				gb_mmu::m_inst = new gb_mmu;
				if(!gb_mmu::m_inst) {
					THROW_GB_MMU_EXCEPTION(GB_MMU_EXCEPTION_ALLOC_FAILED);
				}
			}
			
			return gb_mmu::m_inst;
		}
		
		std::string 
		_gb_mmu::buffer_as_string(
			__in const gb_buf_t &buf,
			__in_opt bool verb,
			__in_opt gb_addr_t org
			)
		{
			gbb_t val;
			gb_buf_t sect;
			gbw_t iter = 0;
			uint8_t tab_iter = 0;
			std::stringstream res;
			gb_buf_t::iterator sect_iter;
		
			for(; iter < buf.size(); ++iter) {
			
				if(!(iter % BLK_LEN)) {
				
					if(verb && !sect.empty()) {
						res << "\t";
					
						for(sect_iter = sect.begin(); 
								sect_iter != sect.end();
								++sect_iter) {
								
							res << (std::isprint((char) *sect_iter) ? 
								(char) *sect_iter : '.');
						}
						
						sect.clear();
					}
				
					if(iter) {
						res << std::endl;
					}
					
					res << "0x" << VAL_AS_HEX(gbw_t, iter + org) << " |";
				}
				
				val = buf.at(iter);
				res << " " << VAL_AS_HEX(gbb_t, val);
				sect.push_back(val);
			}
			
			if(verb && !sect.empty()) {
			
				if(iter % BLK_LEN) {

					for(; tab_iter < (BLK_LEN - (iter % BLK_LEN)); ++tab_iter) {
						res << "   ";
					}
				}

				res << "\t";

				for(sect_iter = sect.begin(); 
						sect_iter != sect.end();
						++sect_iter) {
						
					res << (std::isprint((char) *sect_iter) ? 
						(char) *sect_iter : '.');
				}
				
				sect.clear();
			}
			
			return res.str();
		}
		
		void 
		_gb_mmu::clear(void)
		{
		
			if(!m_init) {
				THROW_GB_MMU_EXCEPTION(GB_MMU_EXCEPTION_UNINITIALIZED);
			}
			
			m_buf.clear();
			m_buf.resize(GB_MMU_MAX_ADDR + 1);
		}
		
		void 
		_gb_mmu::fill(
			__in gb_addr_t addr,
			__in gbw_t off,
			__in gbb_t val
			)
		{
			gbw_t iter = 0;
		
			if(!m_init) {
				THROW_GB_MMU_EXCEPTION(GB_MMU_EXCEPTION_UNINITIALIZED);
			}
			
			for(; iter < off; ++iter) {
				m_buf.at((addr + iter) % GB_MMU_MAX_ADDR) = val;
			}
		}
		
		void 
		_gb_mmu::initialize(void)
		{
		
			if(m_init) {
				THROW_GB_MMU_EXCEPTION(GB_MMU_EXCEPTION_INITIALIZED);
			}
			
			m_buf.clear();
			m_buf.resize(GB_MMU_MAX_ADDR + 1);
			m_init = true;
		}
		
		void 
		_gb_mmu::insert(
			__in gb_addr_t addr,
			__in const gb_buf_t &buf
			)
		{
			gb_buf_t::const_iterator iter;

			if(!m_init) {
				THROW_GB_MMU_EXCEPTION(GB_MMU_EXCEPTION_UNINITIALIZED);
			}

			for(iter = buf.begin(); iter != buf.end(); ++iter, ++addr) {
				m_buf.at(addr) = *iter;
			}
		}

		bool 
		_gb_mmu::is_allocated(void)
		{
			return (gb_mmu::m_inst != NULL);
		}
		
		bool 
		_gb_mmu::is_initialized(void)
		{
			return m_init;
		}
		
		gbb_t 
		_gb_mmu::read_byte(
			__in gb_addr_t addr
			)
		{
		
			if(!m_init) {
				THROW_GB_MMU_EXCEPTION(GB_MMU_EXCEPTION_UNINITIALIZED);
			}
			
			return m_buf.at(addr);
		}
		
		gbw_t 
		_gb_mmu::read_word(
			__in gb_addr_t addr
			)
		{
			gbw_t res;
		
			if(!m_init) {
				THROW_GB_MMU_EXCEPTION(GB_MMU_EXCEPTION_UNINITIALIZED);
			}
			
			res = m_buf.at(addr + 1) << BITS_PER_BYTE;
			return res | m_buf.at(addr);
		}
		
		std::string 
		_gb_mmu::to_string(
			__in_opt bool verb,
			__in_opt gb_addr_t addr,
			__in_opt gbw_t off
			)
		{
			gb_buf_t buf;
			std::stringstream res;		

			res << "[" << (m_init ? "INIT" : "UNINIT") << "] " << GB_MMU_HEADER
				<< " (ptr=0x" << VAL_AS_HEX(gb_mmu_ptr, this) << ")";
			
			if(verb) {			

				if(m_init && off) {

					if((addr + off) > GB_MMU_MAX_ADDR) {
						off -= ((addr + off) - GB_MMU_MAX_ADDR - 1);
					}

					buf = gb_buf_t(m_buf.begin() + addr, m_buf.begin() + addr + off);
					res << std::endl << buffer_as_string(buf, true, addr);
				}
			}
			
			return res.str();
		}
		
		void 
		_gb_mmu::uninitialize(void)
		{
		
			if(!m_init) {
				THROW_GB_MMU_EXCEPTION(GB_MMU_EXCEPTION_UNINITIALIZED);
			}
			
			m_buf.clear();
			m_init = false;
		}
	
		void 
		_gb_mmu::write_byte(
			__in gb_addr_t addr,
			__in gbb_t val
			)
		{
		
			if(!m_init) {
				THROW_GB_MMU_EXCEPTION(GB_MMU_EXCEPTION_UNINITIALIZED);
			}
			
			switch(addr) {
				case GB_REG_P1:

					switch(val) {
						case GB_KEY_COLUMN_ACT:
							m_buf.at(addr) = m_joy->read_action();
							break;
						case GB_KEY_COLUMN_DIR:
							m_buf.at(addr) = m_joy->read_direction();
							break;
						default:
							m_buf.at(addr) = val;
							break;
					}
					break;
				default:
					m_buf.at(addr) = val;
					break;
			}
		}
		
		void 
		_gb_mmu::write_word(
			__in gb_addr_t addr,
			__in gbw_t val
			)
		{
		
			if(!m_init) {
				THROW_GB_MMU_EXCEPTION(GB_MMU_EXCEPTION_UNINITIALIZED);
			}
			
			m_buf.at(addr) = val & GBB_MAX_LEN;
			m_buf.at(addr + 1) = (val >> BITS_PER_BYTE) & GBB_MAX_LEN;
		}
	}
}
