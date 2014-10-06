#############################################################################
#
# Makefile for librf24mesh examples on Raspberry Pi
#
# By:  TMRh20
# Date:    2013/09
#
# Description:
# ------------
# use make all and make install to install the examples
# You can change the install directory by editing the prefix line
#
prefix := /usr/local

# The recommended compiler flags for the Raspberry Pi
CCFLAGS=-Ofast -mfpu=vfp -mfloat-abi=hard -march=armv6zk -mtune=arm1176jzf-s
#CCFLAGS=

# define all programs
PROGRAMS = RF24Mesh_Ncurses_Master
SOURCES = ${PROGRAMS2:=.cpp}

all: ${PROGRAMS}

${PROGRAMS}: ${SOURCES}
	g++ ${CCFLAGS} -Wall -I../ -lncurses -lrf24-bcm -lrf24network -lrf24mesh $@.cpp -o $@

clean:
	rm -rf $(PROGRAMS)
	
install: all
	test -d $(prefix) || mkdir $(prefix)
	test -d $(prefix)/bin || mkdir $(prefix)/bin
	for prog in $(PROGRAMS); do \
	  install -m 0755 $$prog $(prefix)/bin; \
	done

.PHONY: install
