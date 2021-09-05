# Makefile
#
# Copyright (c) 2021 Jörgen Grahn <grahn+src@snipabacken.se>
# All rights reserved.

SHELL=/bin/bash
INSTALLBASE=/usr/local
CXXFLAGS=-Wall -Wextra -pedantic -std=c++14 -g -Os -Wold-style-cast
CFLAGS=-W -Wall -pedantic -std=c99 -g -Os
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
libmassif.a: graph.o
libmassif.a: split.o
libmassif.a: files...o
libmassif.a: rjust.o
libmassif.a: md5pp.o
libmassif.a: md5.o
	$(AR) $(ARFLAGS) $@ $^

example.o: CXXFLAGS+=-O0

example: example.o
	$(CXX) -o $@ example.o

massif_parse: parse.o libmassif.a
	$(CXX) -o $@ parse.o -L . -lmassif

libtest.a: test/split.o
libtest.a: test/snapshot.o
libtest.a: test/files.o
libtest.a: test/md5.o
	$(AR) $(ARFLAGS) $@ $^

test/%.o: CPPFLAGS+=-I.

test.cc: libtest.a
	orchis -o$@ $^

tests: test.o libmassif.a libtest.a
	$(CXX) $(CXXFLAGS) -o $@ test.o -L. -ltest -lmassif

.PHONY: tags TAGS
tags: TAGS
TAGS:
	ctags --exclude='test' -eR . --extra=q

love:
	@echo "not war?"

.PHONY: install
install: massif_parse
install: massif_parse.1
	install -s -m755 massif_parse $(INSTALLBASE)/bin/
	install -m644 massif_parse.1 $(INSTALLBASE)/man/man1/

$(shell mkdir -p dep/{,test})
DEPFLAGS=-MT $@ -MMD -MP -MF dep/$*.Td
COMPILE.cc=$(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
COMPILE.c=$(CC) $(DEPFLAGS) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c

%.o: %.cc
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	@mv dep/$*.{Td,d}

%.o: %.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<
	@mv dep/$*.{Td,d}

dep/%.d: ;
dep/test/%.d: ;

-include dep/test/*.d
-include dep/*.d
