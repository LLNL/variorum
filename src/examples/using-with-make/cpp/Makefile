###############################################################################
# Example showing how to use an installed instance of Variorum in
# Makefile-based build system.
#
# To build:
#   env VARIORUM_DIR={variorum install path} make
#   ./variorum-print-power-example
#
# From within a Variorum install:
#   make
#   ./example
#
# Which corresponds to:
#   make VARIORUM_DIR=../../../
#   ./example
###############################################################################

VARIORUM_DIR ?= ../../..

include ${VARIORUM_DIR}/share/variorum/variorum_config.mk

INC_FLAGS = ${VARIORUM_INCLUDE_FLAGS}
LNK_FLAGS = ${VARIORUM_LINK_RPATH} ${VARIORUM_LIB_FLAGS}

main:
	${CXX} ${INC_FLAGS} -o variorum-print-power-example -DSECOND_RUN variorum-print-power-example.cpp ${LNK_FLAGS}

clean:
	rm -f variorum-print-power-example
