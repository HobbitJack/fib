#include <libgen.h>
#include <stdio.h>
#include <unistd.h>

#include <gmp.h>

#include "fib.gg.h"
#include "fib.l.h"

static char *progname;
static int status;

static struct gengetopt_args_info args;

void
fib_maxv(mpz_t target)
{
	 mpz_out_str(NULL, args.base_arg, target);
	 putchar('\n');
	 return;
}

void
fib_iter(mpz_t target)
{
	mpz_out_str(NULL, args.base_arg, target);
	putchar('\n');
	return;
}

int
main(int argc, char *argv[])
{
	int i;
	mpz_t target;
	
	progname = basename(argv[0]);
	mpz_init(target);

	if (ggo(argc, argv, &args))
		return 1;

	if (args.inputs_num)
	{
		for (i=0; i<args.inputs_num; i++)
		{
			if (mpz_set_str(target, args.inputs[i], 0) || (mpz_cmp_d(target, 0) < 0))
			{
				if (!args.quiet_flag)
					fprintf(stderr, "%s: %s: Bad argument\n", progname, args.inputs[i]);

				status = 1;
				continue;
			}

			if (args.max_value_flag)
				fib_maxv(target);
			else
				fib_iter(target);
		}
	}
	else
	{
		while (yylex() != -1)
		{
			if ((errno < 0) || mpz_set_str(target, yytext, 0) || (mpz_cmp_d(target, 0) < 0))
			{
				if (!args.quiet_flag)
					fprintf(stderr, "%s: %s: Bad argument\n", progname, yytext);

				errno = 0;
				status = 1;
				continue;
			}

			if (args.max_value_flag)
				fib_maxv(target);
			else
				fib_iter(target);
		}
	}

	return args.loose_exit_status_flag ? 0 : status;
}
