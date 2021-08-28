# Makefile
#
# Copyright (c) 2021 Jörgen Grahn <grahn+src@snipabacken.se>
# All rights reserved.

SHELL=/bin/bash
INSTALLBASE=/usr/local

.PHONY: all
all: example

.PHONY: clean
clean:
	$(RM) example
	$(RM) *.[oa]
	$(RM) -r dep

CXXFLAGS=-Wall -Wextra -pedantic -Wold-style-cast -std=c++11 -g -O0

example: example.o
	$(CXX) -o $@ example.o

.PHONY: tags TAGS
tags: TAGS
TAGS:
	etags *.cc *.h

love:
	@echo "not war?"

$(shell mkdir -p dep)
DEPFLAGS=-MT $@ -MMD -MP -MF dep/$*.Td
COMPILE.cc=$(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
COMPILE.c=$(CC) $(DEPFLAGS) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c

%.o: %.cc
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	@mv dep/$*.{Td,d}

dep/%.d: ;
-include dep/*.d
