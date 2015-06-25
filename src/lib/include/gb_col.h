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

#ifndef GB_COL_H_
#define GB_COL_H_

namespace GB_NS {

	typedef enum {
		GB_PX_COL_WHITE = 0,
		GB_PX_COL_GREY,
		GB_PX_COL_GREY_DARK,
		GB_PX_COL_BLACK,
	} gb_col_t;

	#define GB_PX_COL_MAX GB_PX_COL_BLACK

	typedef enum {
		GB_COL_TH_GREY = 0,
		GB_COL_TH_GREEN,
		GB_COL_TH_TEAL,
	} gb_col_th_t;

	#define GB_COL_TH_MAX GB_COL_TH_TEAL

	static const std::string GB_COL_TH_STR[] = {
		"GREY", "GREEN", "TEAL",
		};

	#define GB_COL_TH_STRING(_TYPE_) \
		((_TYPE_) > GB_COL_TH_MAX ? UNKNOWN : \
		GB_COL_TH_STR[_TYPE_].c_str())

	typedef struct __attribute__((__packed__)) {
		gbb_t red;
		gbb_t green;
		gbb_t blue;
	} gb_px_t;

	static const gb_px_t GB_PX_COL_VAL[][GB_PX_COL_MAX + 1] = {

		// grey
		{
			{ 255, 255, 255, },
			{ 148, 148, 148, },
			{ 82, 82, 82, },
			{ 0, 0, 0, },
		},

		// green
		{
			{ 155, 187, 14, },
			{ 115, 160, 103, },
			{ 53, 98, 55, },
			{ 15, 56, 14, },
		},

		// teal
		{
			{ 224, 248, 208, },
			{ 136, 192, 112, },
			{ 48, 104, 80, },
			{ 8, 24, 32, },
		}
		};

	#define GB_PX_COL_VALUE(_TH_, _TYPE_) \
		((((_TH_) > GB_COL_TH_MAX) || ((_TYPE_) > GB_PX_COL_MAX)) ? \
		GB_PX_COL_VAL[GB_COL_TH_GREY][GB_PX_COL_MAX] : \
		GB_PX_COL_VAL[_TH_][_TYPE_])
}

#endif // GB_COL_H_
