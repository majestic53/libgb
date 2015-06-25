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

#include <cmath>
#include "../include/gb.h"
#include "../include/gb_gpu_type.h"

namespace GB_NS {

	namespace GB_COMP_NS {

		#define GB_GFX_TILE_DATA_0 0x8800
		#define GB_GFX_TILE_DATA_1 0x8000
		#define GB_GFX_TILE_DIM 32
		#define GB_GFX_TILE_LEN 16
		#define GB_GFX_TILE_MAP_0 0x9800
		#define GB_GFX_TILE_MAP_1 0x9c00
		#define GB_GPU_HBLNK_CLK 204
		#define GB_GPU_HLINE_LEN 144
		#define GB_GPU_HLINE_MAX 143
		#define GB_GPU_OAM_CLK 80
		#define GB_GPU_VBLNK_CLK 456
		#define GB_GPU_VLINE_LEN 160
		#define GB_GPU_VLINE_MAX 153
		#define GB_GPU_VRAM_CLK 172

		#define GB_LCDC_DISP 0x01
		#define GB_LCDC_SPRITE_DISP 0x02
		#define GB_LCDC_SPRITE_SIZE 0x04
		#define GB_LCDC_TILE_MAP_SEL 0x08
		#define GB_LCDC_TILE_WIN_DATA_SEL 0x10
		#define GB_LCDC_WIN_DISP 0x20
		#define GB_LCDC_WIN_TILE_MAP_SEL 0x40
		#define GB_LCDC_ENABLE 0x80

		#define GB_RGB_LEN 3
		#define GB_SWAP_INTERVAL 1

		static const std::string GB_GPU_STATE_STR[] = {
			"HBLANK", "VBLANK", "OAM", "VRAM",
			};

		#define GB_GPU_STATE_STRING(_TYPE_) \
			((_TYPE_) > GB_GPU_STATE_MAX ? UNKNOWN : \
			GB_GPU_STATE_STR[_TYPE_].c_str())

		#define GB_GPU_SET_PIX(_TH_, _COL_, _X_, _Y_) {\
			size_t off = ((_Y_) * GB_GPU_SCR_DIM) + (_X_); \
			const gb_px_t *col = &GB_PX_COL_VALUE(_TH_, _COL_); \
			m_buf[off].red = col->red; \
			m_buf[off].green = col->green; \
			m_buf[off].blue = col->blue; \
			}

		bool gb_gpu::m_active = false;
		bool gb_gpu::m_update = false;
		gb_px_buf_t gb_gpu::m_buf;
		gbb_t gb_gpu::m_line = 0;
		std::string gb_gpu::m_title(EMPTY);
		gb_mmu_ptr gb_gpu::m_mmu = gb_mmu::acquire();
		gb_gpu_ptr gb_gpu::m_inst = NULL;

		_gb_gpu::_gb_gpu(void) :			
			m_init(false),
			m_state(GB_GPU_STATE_HBLNK),
			m_theme(GB_COL_TH_GREY),
			m_tot(0)
		{
			std::atexit(gb_gpu::_delete);
		}

		_gb_gpu::~_gb_gpu(void)
		{

			if(m_init) {
				uninitialize();
			}
		}

		void 
		_gb_gpu::_delete(void)
		{

			if(gb_gpu::m_inst) {
				delete gb_gpu::m_inst;
				gb_gpu::m_inst = NULL;
			}
		}

		gb_gpu_ptr 
		_gb_gpu::acquire(void)
		{

			if(!gb_gpu::m_inst) {

				gb_gpu::m_inst = new gb_gpu;
				if(!gb_gpu::m_inst) {
					THROW_GB_GPU_EXCEPTION(GB_GPU_EXCEPTION_ALLOC_FAILED);
				}
			}

			return gb_gpu::m_inst;
		}

		void 
		_gb_gpu::graphics(void)
		{
			GLFWwindow *win = NULL;
			const GLFWvidmode *mode = NULL;

			glfwSetErrorCallback(graphics_error);

			if(!glfwInit()) {
				THROW_GB_GPU_EXCEPTION_MESSAGE(GB_GPU_EXCEPTION_INTERNAL, 
					"%s", CAT_STR(glfwInit));
			}

			win = glfwCreateWindow(GB_GPU_SCR_DIM, GB_GPU_SCR_DIM, //GB_GPU_VLINE_LEN, GB_GPU_HLINE_LEN, 
				CHK_STR(gb_gpu::m_title), NULL, NULL);

			if(!win) {
				glfwTerminate();
				THROW_GB_GPU_EXCEPTION_MESSAGE(GB_GPU_EXCEPTION_INTERNAL, 
					"%s", CAT_STR(glfwCreateWindow));
			}

			mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
			glfwSetWindowPos(win, (mode->width - GB_GPU_SCR_DIM /*GB_GPU_VLINE_LEN*/) / 2,
				(mode->height - GB_GPU_SCR_DIM /*GB_GPU_HLINE_LEN*/) / 2);
			glfwMakeContextCurrent(win);
			glfwSwapInterval(GB_SWAP_INTERVAL);
			glfwSetWindowCloseCallback(win, graphics_close);
			glfwSetKeyCallback(win, graphics_key);

			while(!glfwWindowShouldClose(win)) {

				if(!gb_gpu::m_active) {
					break;
				}

				graphics_update(win);
				glfwSwapBuffers(win);
				glfwPollEvents();
			}

			if(gb_gpu::m_active) {
				gb::acquire()->acquire_cpu()->stop();
			}

			glfwDestroyWindow(win);
			glfwTerminate();
			gb::stop();
		}

		void 
		_gb_gpu::graphics_close(
			__in GLFWwindow *win
			)
		{
			gb::stop();
		}

		void 
		_gb_gpu::graphics_error(
			__in int code,
			__in const char *desc
			)
		{
			THROW_GB_GPU_EXCEPTION_MESSAGE(GB_GPU_EXCEPTION_INTERNAL,
				"(0x%x) %s", code, desc ? desc : EMPTY);
		}

		void 
		_gb_gpu::graphics_update(
			__in GLFWwindow *win
			)
		{
			int height, width;

			if(gb_gpu::m_update) {
				gb_gpu::m_update = false;
				glfwGetFramebufferSize(win, &width, &height);
				glViewport(0, 0, width, height);
				glLoadIdentity();
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				glDrawPixels(GB_GPU_SCR_DIM, GB_GPU_SCR_DIM, GL_RGB, GL_UNSIGNED_BYTE, 
					(GLvoid *) m_buf);
			}
		}

		void 
		_gb_gpu::graphics_key(
			__in GLFWwindow *win, 
			__in int key, 
			__in int code, 
			__in int action, 
			__in int mods
			)
		{
			gbb_t off;
			bool pressed = false;

			switch(key) {

				case GB_KEY_A:

					switch(action) {
						case GLFW_PRESS:
							m_mmu->_joystick()->set_action_a();
							pressed = true;
							break;
						case GLFW_RELEASE:
							m_mmu->_joystick()->clear_action_a();
							break;
					}
					break;
				case GB_KEY_B:

					switch(action) {
						case GLFW_PRESS:
							m_mmu->_joystick()->set_action_b();
							pressed = true;
							break;
						case GLFW_RELEASE:
							m_mmu->_joystick()->clear_action_b();
							break;
					}
					break;
				case GB_KEY_DOWN:

					switch(action) {
						case GLFW_PRESS:
							m_mmu->_joystick()->set_direction_down();
							pressed = true;
							break;
						case GLFW_RELEASE:
							m_mmu->_joystick()->clear_direction_down();
							break;
					}
					break;
				case GB_KEY_ESCAPE:

					if(action == GLFW_RELEASE) {
						glfwSetWindowShouldClose(win, GL_TRUE);
					}
					break;
				case GB_KEY_LEFT:

					switch(action) {
						case GLFW_PRESS:
							m_mmu->_joystick()->set_direction_left();
							pressed = true;
							break;
						case GLFW_RELEASE:
							m_mmu->_joystick()->clear_direction_left();
							break;
					}
					break;
				case GB_KEY_RIGHT:

					switch(action) {
						case GLFW_PRESS:
							m_mmu->_joystick()->set_direction_right();
							pressed = true;
							break;
						case GLFW_RELEASE:
							m_mmu->_joystick()->clear_direction_right();
							break;
					}
					break;
				case GB_KEY_SELECT:

					switch(action) {
						case GLFW_PRESS:
							m_mmu->_joystick()->set_action_select();
							pressed = true;
							break;
						case GLFW_RELEASE:
							m_mmu->_joystick()->clear_action_select();
							break;
					}
					break;
				case GB_KEY_START:

					switch(action) {
						case GLFW_PRESS:
							m_mmu->_joystick()->set_action_start();
							pressed = true;
							break;
						case GLFW_RELEASE:
							m_mmu->_joystick()->clear_action_start();
							break;
					}
					break;
				case GB_KEY_UP:

					switch(action) {
						case GLFW_PRESS:
							m_mmu->_joystick()->set_direction_up();
							pressed = true;
							break;
						case GLFW_RELEASE:
							m_mmu->_joystick()->clear_direction_up();
							break;
					}
					break;
			}

			if(pressed && gb::interrupt_master_enable()) {

				off = (1 << GB_INTERRUPT_JOY_PRESS);
				if(m_mmu->read_byte(GB_REG_IE) & off) {
					m_mmu->write_byte(GB_REG_IF, m_mmu->read_byte(GB_REG_IF) | off);
				}
			}
		}

		void 
		_gb_gpu::initialize(void)
		{

			if(m_init) {
				THROW_GB_GPU_EXCEPTION(GB_GPU_EXCEPTION_INITIALIZED);
			}

			m_init = true;
			reset();
		}

		bool 
		_gb_gpu::is_active(void)
		{
			return m_active;
		}

		bool 
		_gb_gpu::is_allocated(void)
		{

			return (gb_gpu::m_inst != NULL);
		}

		bool 
		_gb_gpu::is_initialized(void)
		{

			return m_init;
		}

		void 
		_gb_gpu::render_line(void)
		{
			gbw_t off_x;
			gbb_t lcdc, tile_id;
			gb_addr_t data_base_addr, map_base_addr, tile_addr;

			lcdc = m_mmu->read_byte(GB_REG_LCDC);
			if(lcdc & GB_LCDC_ENABLE) {
				data_base_addr  = lcdc & GB_LCDC_TILE_WIN_DATA_SEL ? 
					GB_GFX_TILE_DATA_1 : GB_GFX_TILE_DATA_0;
				map_base_addr = lcdc & GB_LCDC_TILE_MAP_SEL ? 
					GB_GFX_TILE_MAP_1 : GB_GFX_TILE_MAP_0;

				for(off_x = 0; off_x < GB_GFX_TILE_DIM; ++off_x) {
					tile_id = m_mmu->read_byte(map_base_addr 
						+ ((m_line * GB_GFX_TILE_DIM) + off_x));
					tile_addr = data_base_addr + (tile_id * GB_GFX_TILE_LEN);

					// TODO: handle 8x8 tile held in tile_addr (16 bytes)
					// and place into m_buf
				}
			}
		}

		void 
		_gb_gpu::reset(void)
		{

			if(!m_init) {
				THROW_GB_GPU_EXCEPTION(GB_GPU_EXCEPTION_UNINITIALIZED);
			}

			m_active = false;
			m_update = false;
			memset(m_buf, 0, sizeof(gb_px_t) * GB_GPU_SCR_DIM * GB_GPU_SCR_DIM);
			m_line = 0;
			m_state = GB_GPU_STATE_HBLNK;
			m_title.clear();
			m_theme = GB_COL_TH_GREY;
			m_tot = 0;
		}

		void 
		_gb_gpu::start(
			__in_opt const std::string &title,
			__in_opt gb_col_th_t theme,
			__in_opt bool detach
			)
		{
			gbw_t x_off, y_off = 0;

			if(!m_init) {
				THROW_GB_GPU_EXCEPTION(GB_GPU_EXCEPTION_UNINITIALIZED);
			}

			if(m_active) {
				THROW_GB_GPU_EXCEPTION(GB_GPU_EXCEPTION_ACTIVE);
			}

			m_title = title;
			m_theme = theme;
			memset(m_buf, 0, sizeof(gb_px_t) * GB_GPU_SCR_DIM * GB_GPU_SCR_DIM);

			for(; y_off < GB_GPU_SCR_DIM; ++y_off) {

				for(x_off = 0; x_off < GB_GPU_SCR_DIM; ++x_off) {
					GB_GPU_SET_PIX(m_theme, GB_PX_COL_WHITE, x_off, y_off);
				}
			}

			m_graphics_thread = std::thread(gb_gpu::graphics);

			if(detach) {
				m_graphics_thread.detach();
			}

			m_active = true;
		}

		void 
		_gb_gpu::step(
			__in gbb_t last
			)
		{
			gbb_t off;

			if(!m_init) {
				THROW_GB_GPU_EXCEPTION(GB_GPU_EXCEPTION_UNINITIALIZED);
			}

			m_tot += last;

			switch(m_state) {
				case GB_GPU_STATE_HBLNK:

					if(m_tot >= GB_GPU_HBLNK_CLK) {
						m_tot = 0;

						if(++m_line >= GB_GPU_HLINE_MAX) {
							m_state = GB_GPU_STATE_VBLNK;

							if(m_active) {
								m_update = true;
							}

						} else {
							m_state = GB_GPU_STATE_OAM;
						}
					}
					break;
				case GB_GPU_STATE_VBLNK:

					if(gb::interrupt_master_enable()) {

						off = (1 << GB_INTERRUPT_VBLNK);
						if(m_mmu->read_byte(GB_REG_IE) & off) {
							m_mmu->write_byte(GB_REG_IF, m_mmu->read_byte(GB_REG_IF) | off);
						}
					}

					if(m_tot >= GB_GPU_VBLNK_CLK) {
						m_tot = 0;

						if(++m_line > GB_GPU_VLINE_MAX) {
							m_state = GB_GPU_STATE_OAM;
							m_line = 0;
						}

						m_mmu->write_byte(GB_REG_LY, m_line);
					}
					break;
				case GB_GPU_STATE_OAM:

					if(m_tot >= GB_GPU_OAM_CLK) {
						m_state = GB_GPU_STATE_VRAM;
						m_tot = 0;
					}
					break;
				case GB_GPU_STATE_VRAM:

					if(m_tot >= GB_GPU_VRAM_CLK) {
						m_state = GB_GPU_STATE_HBLNK;
						m_tot = 0;

						if(m_active) {
							render_line();
						}
					}
					break;
				default:
					THROW_GB_GPU_EXCEPTION_MESSAGE(GB_GPU_EXCEPTION_UNKNOWN_STATE,
						"state=0x%x", m_state);
			}
		}

		void 
		_gb_gpu::stop(void)
		{

			if(!m_init) {
				THROW_GB_GPU_EXCEPTION(GB_GPU_EXCEPTION_UNINITIALIZED);
			}

			if(!m_active) {
				THROW_GB_GPU_EXCEPTION(GB_GPU_EXCPETION_INACTIVE);
			}
			
			m_active = false;

			if(m_graphics_thread.joinable()) {
				m_graphics_thread.join();
			}

			memset(m_buf, 0, sizeof(gb_px_t) * GB_GPU_SCR_DIM * GB_GPU_SCR_DIM);
		}

		std::string 
		_gb_gpu::to_string(
			__in_opt bool verb
			)
		{
			std::stringstream res;

			res << "[" << (m_init ? "INIT" : "UNINIT") << "] " << GB_GPU_HEADER
					<< " (ptr=0x" << VAL_AS_HEX(gb_cpu_ptr, this) << ")";

			if(m_init && verb) {
				res << std::endl << "STATE=" << GB_GPU_STATE_STRING(m_state) 
					<< ", LINE=" << (int)m_line << ", TOTAL=" << m_tot << ", ACTIVE=" 
					<< m_active << ", THEME=" << GB_COL_TH_STRING(m_theme);
			}

			return res.str();
		}

		void 
		_gb_gpu::uninitialize(void)
		{

			if(!m_init) {
				THROW_GB_GPU_EXCEPTION(GB_GPU_EXCEPTION_UNINITIALIZED);
			}

			reset();
			m_init = false;
		}
	}
}
