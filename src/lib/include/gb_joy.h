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

#ifndef GB_JOY_H_
#define GB_JOY_H_

namespace GB_NS {

	namespace GB_COMP_NS {

		#define GB_KEY_COLUMN_ACT 0x10
		#define GB_KEY_COLUMN_DIR 0x20

		typedef class _gb_joy {

			public:

				~_gb_joy(void);

				static void _delete(void);

				static _gb_joy *acquire(void);

				void clear_action_a(void);

				void clear_action_b(void);

				void clear_action_select(void);

				void clear_action_start(void);

				void clear_direction_down(void);

				void clear_direction_left(void);

				void clear_direction_right(void);

				void clear_direction_up(void);

				void initialize(void);

				static bool is_allocated(void);

				bool is_initialized(void);

				gbb_t read_action(void);

				gbb_t read_direction(void);

				void reset(void);

				void set_action_a(void);

				void set_action_b(void);

				void set_action_select(void);

				void set_action_start(void);

				void set_direction_down(void);

				void set_direction_left(void);

				void set_direction_right(void);

				void set_direction_up(void);

				std::string to_string(
					__in_opt bool verb = false
					);

				void uninitialize(void);

			protected:

				_gb_joy(void);

				_gb_joy(
					__in const _gb_joy &other
					);

				_gb_joy &operator=(
					__in const _gb_joy &other
					);

				bool m_init;

				static _gb_joy *m_inst;

			private:

				gbb_t m_act, m_dir;

				bool m_ja, m_jb, m_jst, m_jsel, 
					m_jup, m_jdn, m_jlt, m_jrt;

		} gb_joy, *gb_joy_ptr;
	}
}

#endif // GB_JOY_H_
