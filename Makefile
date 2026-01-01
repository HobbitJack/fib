MAKEFLAGS='-r'

fib:	fib.c fib.gg.c fib.gg.h fib.l.c fib.l.h
	cc -lgmp -o $@ fib.c fib.gg.c fib.l.c

fib.l.c fib.l.h:	fib.l
	lex -o fib.l.c fib.l

fib.gg.c fib.gg.h:	fib.gg
	gengetopt <fib.gg
	sed -E 's/(\\n)?[[:blank:]]+\(default=.*\)//' <fib.gg.c | sponge fib.gg.c
	
.PHONY:	clean release install uninstall
clean:
	rm -f fib fib.gg.? fib.l.? fib.tar.gz fib.1.gz
	
release:	fib
	rm -f fib.tar.gz
	tar -cf fib.tar fib.c fib.gg fib.l fib.1 Makefile
	gzip fib.tar

install:	fib
	install fib /usr/local/bin/
	gzip -k fib.1
	install fib.1.gz /usr/local/share/man/man1/

uninstall:
	rm /usr/local/bin/fib
	rm /usr/local/share/man/man1/fib.1.gz 
