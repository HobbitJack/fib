MAKEFLAGS = '-r'
PREFIX ?= /usr/local

fib:	fib.c fib.go.c fib.go.h fib.l.c fib.l.h
	cc -lgmp -o $@ fib.c fib.go.c fib.l.c

fib.l.c fib.l.h:	fib.l
	lex -o fib.l.c fib.l

fib.go.c fib.go.h:	fib.go
	gengetopt <fib.go
	sed -E 's/(\\n)?[[:blank:]]+\(default=.*\)//' <fib.go.c >fib.go.c.tmp
	mv -f fib.go.c.tmp fib.go.c
	
.PHONY:	clean source release install uninstall
clean:
	rm -f fib fib.go.? fib.l.? fib*.tar.gz fib.1.gz

source:
	rm -f fib_source.tar.gz
	tar -cf fib_source.tar fib.c fib.go fib.l fib.1 Makefile
	gzip fib_source.tar

release:	fib
	rm -f fib.tar.gz
	tar -cf fib.tar fib fib.c fib.go fib.l fib.1 Makefile
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
