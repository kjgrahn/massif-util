# Makefile
#
# Copyright (c) 2021 Jörgen Grahn <grahn+src@snipabacken.se>
# All rights reserved.

SHELL=/bin/bash
INSTALLBASE=/usr/local
CXXFLAGS=-Wall -Wextra -pedantic -std=c++14 -g -Os -Wold-style-cast
CPPFLAGS=
ARFLAGS=rTP

.PHONY: all
all: example
all: massif_parse
all: tests

.PHONY: check checkv
check: tests
	./tests
checkv: tests
	valgrind -q ./tests -v

.PHONY: clean
clean:
	$(RM) example massif_parse
	$(RM) tests test.cc
	$(RM) *.[oa] test/*.o
	$(RM) -r dep

libmassif.a: snapshot.o
libmassif.a: split.o
	$(AR) $(ARFLAGS) $@ $^

example.o: CXXFLAGS+=-O0

example: example.o
	$(CXX) -o $@ example.o

massif_parse: parse.o libmassif.a
	$(CXX) -o $@ parse.o -L . -lmassif

libtest.a: test/split.o
libtest.a: test/snapshot.o
	$(AR) $(ARFLAGS) $@ $^

test/%.o: CPPFLAGS+=-I.

test.cc: libtest.a
	orchis -o$@ $^

tests: test.o libmassif.a libtest.a
	$(CXX) $(CXXFLAGS) -o $@ test.o -L. -ltest -lmassif

.PHONY: tags TAGS
tags: TAGS
TAGS:
	etags *.cc *.h

love:
	@echo "not war?"

$(shell mkdir -p dep/{,test})
DEPFLAGS=-MT $@ -MMD -MP -MF dep/$*.Td
COMPILE.cc=$(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c

%.o: %.cc
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	@mv dep/$*.{Td,d}

dep/%.d: ;
dep/test/%.d: ;

-include dep/test/*.d
-include dep/*.d
