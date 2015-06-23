LIBGB
=====

LIBGB is a lightwight GameBoy emulation library, written in C++.

Table of Contents
===============

1. [Changelog](https://github.com/majestic53/libgb#changelog)
2. [Usage](https://github.com/majestic53/libgb#usage)
	* [Build](https://github.com/majestic53/libgb#build)
3. [License](https://github.com/majestic53/libgb#license)

Changelog
=========

###Version 0.1.1526
*Updated: 6/22/2015*

* Added operational mode
* General code cleanup

###Version 0.1.1525
*Updated: 6/21/2015*

* Bootup successful!
	* Emulator is able to boot and transition to operational mode
* Added support for vertical blank and joystick press interrupts
* Fixed various minor CPU issues
	* Fixed HALT/STOP issues

*Updated: 6/20/2015*

* Added joystick support (complete)
* GLFW window support (partial)
* GLFW keyboard support (partial)

*Updated: 6/19/2015*

* Opengl window integration (partial)
* Minor bug fix in cpu class

*Updated: 6/18/2015*

* Added gpu class

*Updated: 6/17/2015*

* Added rom class
* Added special register definitions
* Added bios definition

*Updated: 6/16/2015*

* Added cpu command helper routines (complete)

*Updated: 6/15/2015*

* Added cpu command helper routines (partial)

*Changelog truncated (see file history for full log)*

Usage
=====

###Build

__PREREQ__: Building this project requires the GNU and/or Clang c++ compilers (g++/clang++).

To build the entire project, simply run the command below in the projects root directory.

```
make
```

LIBGB produces a series of terminal based applications and libraries, which can be invoked using the interfaces below.

###Emulator

To invoke the emulator, use the command listed below.

```
__TBD__
```

License
======

Copyright(C) 2015 David Jolly <majestic53@gmail.com>

libgb is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

libgb is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
