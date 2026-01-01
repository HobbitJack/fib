MAKEFLAGS='-r'

fib:	fib.c fib.gg.c fib.gg.h fib.l.c fib.l.h
	cc -lgmp -o $@ fib.c fib.gg.c fib.l.c

fib.l.c fib.l.h:	fib.l
	lex -o fib.l.c fib.l

fib.gg.c fib.gg.h:	fib.gg
	gengetopt <fib.gg
	sed -E 's/(\\n)?[[:blank:]]+\(default=.*\)//' <fib.gg.c | sponge fib.gg.c
	
.PHONY: clean
clean:
	rm -f fib fib.gg.? fib.l.?
