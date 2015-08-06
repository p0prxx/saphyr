# Saphyr Compiler
[![Build Status](https://travis-ci.org/jdm64/saphyr.svg)](https://travis-ci.org/jdm64/saphyr)
[![Scan Status](https://scan.coverity.com/projects/4591/badge.svg)](https://scan.coverity.com/projects/4591)
[![Coverage Status](https://coveralls.io/repos/jdm64/saphyr/badge.svg?branch=master)](https://coveralls.io/r/jdm64/saphyr?branch=master)

A C-Like compiler using LLVM as a backend.

## License ##

Unless otherwise stated the source code is licensed under the GPLv3 -- see LICENSE.

## Build Dependencies ##

* [FlexC++](http://flexcpp.sourceforge.net/)
* [BisonC++](http://bisoncpp.sourceforge.net/)
* [LLVM](http://llvm.org/) 3.4+
* C++11 compiler (either GCC or Clang)
* Python 3 (for running tests)

Note: Building FlexC++/BisonC++ from sources requires the [Icmake](http://icmake.sourceforge.net/) build
system and the [Bobcat](http://bobcat.sourceforge.net/) library which requires a C++14 compiler.

### Debian/Ubuntu ###

Debian 7 (Wheezy) and Ubuntu 14.04 (Trusty) or newer have all the required packages in their repositories.

`sudo apt-get install flexc++ bisonc++ make llvm-dev libboost-program-options-dev`
`sudo apt-get install clang python3`

### Gentoo ###

A local portage overlay containing bobcat/flexc++/bisonc++ is provided in `scripts/portage`.
After adding the overlay run `emerge saphyr` to install all required dependencies.

=== Arch ===

FlexC++/BisonC++ can be found in the AUR along with their dependencies icmake and libbobcat.
The other dependencies can be install using:

`pacman -S make llvm boost clang`

## Build Instructions ##

Run `make` in the root directory and it will build the compiler binary `saphyr`.
