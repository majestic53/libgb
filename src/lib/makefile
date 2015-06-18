# libgb
# Copyright (C) 2015 David Jolly
# ----------------------
#
# libgb is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# libgb is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

CC=clang++
CC_FLAGS=-march=native -pthread -std=gnu++11 -O3 -Wall
DIR_BIN=./../../bin/
DIR_BUILD=./../../build/
DIR_INC=./include/
DIR_SRC=./src/
LIB=libgb.a

all: build archive

archive:
	@echo ""
	@echo "--- BUILDING LIBRARY -----------------------"
	ar rcs $(DIR_BUILD)$(LIB) $(DIR_BUILD)gb.o $(DIR_BUILD)gb_cpu.o $(DIR_BUILD)gb_except.o $(DIR_BUILD)gb_mmu.o $(DIR_BUILD)gb_rom.o
	@echo "--- DONE -----------------------------------"
	@echo ""

build: gb.o gb_cpu.o gb_except.o gb_mmu.o gb_rom.o

gb.o: $(DIR_SRC)gb.cpp $(DIR_INC)gb.h
	$(CC) $(CC_FLAGS) -c $(DIR_SRC)gb.cpp -o $(DIR_BUILD)gb.o

gb_except.o: $(DIR_SRC)gb_except.cpp $(DIR_INC)gb_except.h
	$(CC) $(CC_FLAGS) -c $(DIR_SRC)gb_except.cpp -o $(DIR_BUILD)gb_except.o

gb_rom.o: $(DIR_SRC)gb_rom.cpp $(DIR_INC)gb_rom.h
	$(CC) $(CC_FLAGS) -c $(DIR_SRC)gb_rom.cpp -o $(DIR_BUILD)gb_rom.o

# COMPONENTS

gb_cpu.o: $(DIR_SRC)gb_cpu.cpp $(DIR_INC)gb_cpu.h
	$(CC) $(CC_FLAGS) -c $(DIR_SRC)gb_cpu.cpp -o $(DIR_BUILD)gb_cpu.o

gb_mmu.o: $(DIR_SRC)gb_mmu.cpp $(DIR_INC)gb_mmu.h
	$(CC) $(CC_FLAGS) -c $(DIR_SRC)gb_mmu.cpp -o $(DIR_BUILD)gb_mmu.o
