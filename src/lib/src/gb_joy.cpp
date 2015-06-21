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
#include "../include/gb_joy_type.h"

namespace GB_NS {

	namespace GB_COMP_NS {

		#define GB_JOY_ACTION_A 0x01
		#define GB_JOY_ACTION_B 0x02
		#define GB_JOY_ACTION_SEL 0x04
		#define GB_JOY_ACTION_ST 0x08
		#define GB_JOY_DIRECTION_DN 0x08
		#define GB_JOY_DIRECTION_LT 0x02
		#define GB_JOY_DIRECTION_RT 0x01
		#define GB_JOY_DIRECTION_UP 0x04

		gb_joy_ptr gb_joy::m_inst = NULL;

		_gb_joy::_gb_joy(void) :
			m_init(false),
			m_act(0), m_dir(0), 
			m_ja(false), m_jb(false), m_jst(false), m_jsel(false), 
			m_jup(false), m_jdn(false), m_jlt(false), m_jrt(false)
		{
			std::atexit(gb_joy::_delete);
		}

		_gb_joy::~_gb_joy(void)
		{

			if(m_init) {
				uninitialize();
			}
		}

		void 
		_gb_joy::_delete(void)
		{

			if(gb_joy::m_inst) {
				delete gb_joy::m_inst;
				gb_joy::m_inst = NULL;
			}
		}

		gb_joy_ptr 
		_gb_joy::acquire(void)
		{

			if(!gb_joy::m_inst) {

				gb_joy::m_inst = new gb_joy;
				if(!gb_joy::m_inst) {
					THROW_GB_JOY_EXCEPTION(GB_JOY_EXCEPTION_ALLOC_FAILED);
				}
			}

			return gb_joy::m_inst;
		}

		void 
		_gb_joy::clear_action_a(void)
		{

			if(!m_init) {
				THROW_GB_JOY_EXCEPTION(GB_JOY_EXCEPTION_UNINITIALIZED);
			}

			m_act |= GB_JOY_ACTION_A;
			m_ja = false;
		}

		void 
		_gb_joy::clear_action_b(void)
		{

			if(!m_init) {
				THROW_GB_JOY_EXCEPTION(GB_JOY_EXCEPTION_UNINITIALIZED);
			}

			m_act |= GB_JOY_ACTION_B;
			m_jb = false;
		}

		void 
		_gb_joy::clear_action_select(void)
		{

			if(!m_init) {
				THROW_GB_JOY_EXCEPTION(GB_JOY_EXCEPTION_UNINITIALIZED);
			}

			m_act |= GB_JOY_ACTION_SEL;
			m_jsel = false;
		}

		void 
		_gb_joy::clear_action_start(void)
		{

			if(!m_init) {
				THROW_GB_JOY_EXCEPTION(GB_JOY_EXCEPTION_UNINITIALIZED);
			}

			m_act |= GB_JOY_ACTION_ST;
			m_jst = false;
		}

		void 
		_gb_joy::clear_direction_down(void)
		{

			if(!m_init) {
				THROW_GB_JOY_EXCEPTION(GB_JOY_EXCEPTION_UNINITIALIZED);
			}

			m_dir |= GB_JOY_DIRECTION_DN;
			m_jdn = false;
		}

		void 
		_gb_joy::clear_direction_left(void)
		{

			if(!m_init) {
				THROW_GB_JOY_EXCEPTION(GB_JOY_EXCEPTION_UNINITIALIZED);
			}

			m_dir |= GB_JOY_DIRECTION_LT;
			m_jlt = false;
		}

		void 
		_gb_joy::clear_direction_right(void)
		{

			if(!m_init) {
				THROW_GB_JOY_EXCEPTION(GB_JOY_EXCEPTION_UNINITIALIZED);
			}

			m_dir |= GB_JOY_DIRECTION_RT;
			m_jrt = false;
		}

		void 
		_gb_joy::clear_direction_up(void)
		{

			if(!m_init) {
				THROW_GB_JOY_EXCEPTION(GB_JOY_EXCEPTION_UNINITIALIZED);
			}

			m_dir |= GB_JOY_DIRECTION_UP;
			m_jup = false;
		}

		void 
		_gb_joy::initialize(void)
		{

			if(m_init) {
				THROW_GB_JOY_EXCEPTION(GB_JOY_EXCEPTION_INITIALIZED);
			}

			m_init = true;
			reset();
		}

		bool 
		_gb_joy::is_allocated(void)
		{
			return (gb_joy::m_inst != NULL);
		}

		bool 
		_gb_joy::is_initialized(void)
		{
			return m_init;
		}

		gbb_t 
		_gb_joy::read_action(void)
		{

			if(!m_init) {
				THROW_GB_JOY_EXCEPTION(GB_JOY_EXCEPTION_UNINITIALIZED);
			}

			return m_act;
		}

		gbb_t 
		_gb_joy::read_direction(void)
		{

			if(!m_init) {
				THROW_GB_JOY_EXCEPTION(GB_JOY_EXCEPTION_UNINITIALIZED);
			}

			return m_dir;
		}

		void 
		_gb_joy::reset(void)
		{

			if(!m_init) {
				THROW_GB_JOY_EXCEPTION(GB_JOY_EXCEPTION_UNINITIALIZED);
			}

			m_act = UINT4_MAX;
			m_dir = UINT4_MAX;
			m_ja = false;
			m_jb = false;
			m_jst = false;
			m_jsel = false;
			m_jup = false;
			m_jdn = false;
			m_jlt = false;
			m_jrt = false;
		}

		void 
		_gb_joy::set_action_a(void)
		{

			if(!m_init) {
				THROW_GB_JOY_EXCEPTION(GB_JOY_EXCEPTION_UNINITIALIZED);
			}

			m_act &= ~GB_JOY_ACTION_A;
			m_ja = true;
		}

		void 
		_gb_joy::set_action_b(void)
		{

			if(!m_init) {
				THROW_GB_JOY_EXCEPTION(GB_JOY_EXCEPTION_UNINITIALIZED);
			}

			m_act &= ~GB_JOY_ACTION_B;
			m_jb = true;
		}

		void 
		_gb_joy::set_action_select(void)
		{

			if(!m_init) {
				THROW_GB_JOY_EXCEPTION(GB_JOY_EXCEPTION_UNINITIALIZED);
			}

			m_act &= ~GB_JOY_ACTION_SEL;
			m_jsel = true;
		}

		void 
		_gb_joy::set_action_start(void)
		{

			if(!m_init) {
				THROW_GB_JOY_EXCEPTION(GB_JOY_EXCEPTION_UNINITIALIZED);
			}

			m_act &= ~GB_JOY_ACTION_ST;
			m_jst = true;
		}

		void 
		_gb_joy::set_direction_down(void)
		{

			if(!m_init) {
				THROW_GB_JOY_EXCEPTION(GB_JOY_EXCEPTION_UNINITIALIZED);
			}

			m_dir &= ~GB_JOY_DIRECTION_DN;
			m_jdn = true;
		}

		void 
		_gb_joy::set_direction_left(void)
		{

			if(!m_init) {
				THROW_GB_JOY_EXCEPTION(GB_JOY_EXCEPTION_UNINITIALIZED);
			}

			m_dir &= ~GB_JOY_DIRECTION_LT;
			m_jlt = true;
		}

		void 
		_gb_joy::set_direction_right(void)
		{

			if(!m_init) {
				THROW_GB_JOY_EXCEPTION(GB_JOY_EXCEPTION_UNINITIALIZED);
			}

			m_dir &= ~GB_JOY_DIRECTION_RT;
			m_jrt = true;
		}

		void 
		_gb_joy::set_direction_up(void)
		{

			if(!m_init) {
				THROW_GB_JOY_EXCEPTION(GB_JOY_EXCEPTION_UNINITIALIZED);
			}

			m_dir &= ~GB_JOY_DIRECTION_UP;
			m_jup = true;
		}

		std::string 
		_gb_joy::to_string(
			__in_opt bool verb
			)
		{
			std::stringstream res;

			res << "[" << (m_init ? "INIT" : "UNINIT") << "] " << GB_JOY_HEADER
					<< " (ptr=0x" << VAL_AS_HEX(gb_joy_ptr, this) << ")";

			if(verb) {
				res << std::endl << "ACTION=0x" << VAL_AS_HEX(gbb_t, m_act)
					<< std::endl << "DIRECTION=0x" << VAL_AS_HEX(gbb_t, m_dir)
					<< std::endl << "A=" << m_ja << ", B=" << m_jb << ", START=" 
					<< m_jst << ", SELECT=" << m_jsel << std::endl
					<< "UP=" << m_jup << ", DOWN=" << m_jdn << ", LEFT=" << m_jlt
					<< ", RIGHT=" << m_jrt;
			}

			return res.str();
		}

		void 
		_gb_joy::uninitialize(void)
		{

			if(!m_init) {
				THROW_GB_JOY_EXCEPTION(GB_JOY_EXCEPTION_UNINITIALIZED);
			}

			reset();
			m_init = false;
		}
	}
}
