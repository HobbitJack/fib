.POSIX:	
.PHONY:	all clean install release source uninstall
.SUFFIXES:

PREFIX 	?= /usr/local

all:	fib

fib:	fib.c fib.g.c fib.g.h fib.l.c fib.l.h
	cc -lgmp -o $@ fib.c fib.g.c fib.l.c

fib.l.c fib.l.h:	fib.l
	lex -o fib.l.c fib.l

fib.g.c fib.g.h:	fib.g
	gengetopt <fib.g
	sed -E 's/(\\n)?[[:blank:]]+\(default=.*\)//' <fib.g.c >fib.g.c.tmp
	mv -f fib.g.c.tmp fib.g.c

clean:
	rm -f fib fib.g.? fib.l.? fib*.tar.gz fib.1.gz

source:
	rm -f fib_source.tar.gz
	tar -cf fib_source.tar fib.c fib.g fib.l fib.1 Makefile
	gzip fib_source.tar

release:	fib
	rm -f fib.tar.gz
	tar -cf fib.tar fib fib.c fib.g fib.l fib.1 Makefile
	gzip fib.tar

install:	fib
	mkdir -p $(PREFIX)/bin/
	install fib $(PREFIX)/bin/
	gzip -k fib.1
	mkdir -p $(PREFIX)/share/man/man1/
	install fib.1.gz $(PREFIX)/share/man/man1/

uninstall:
	rm $(PREFIX)/bin/fib
	rm $(PREFIX)/share/man/man1/fib.1.gz 
