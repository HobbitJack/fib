#include <libgen.h>
#include <stdio.h>
#include <unistd.h>

#include <gmp.h>

#include "fib.g.h"
#include "fib.l.h"

static char *progname;
static int status;

static struct gengetopt_args_info args;

void
fib_maxv(mpz_t target)
{
	mpz_t a, b, p;

	mpz_inits(a, b, p, NULL);
	// Cheeky hack. But this does what we want.
	mpz_set_ui(a, 1);
	
	while (mpz_cmp(target, p) >= 0)
	{
		mpz_add(p, a, b);
		mpz_set(a, b);
		mpz_set(b, p);
		
		if (args.print_all_flag || (mpz_cmp(target, p) < 0))
		{
			mpz_out_str(NULL, args.base_arg, a);
			putchar('\n');
		}
	}

	mpz_clears(a, b, p, NULL);
	
	return;
}

void
fib_iter(mpz_t target)
{
	mpz_t a, b, p;
	
	mpz_inits(a, b, p, NULL);
		
	if (args.print_all_flag)
	{
		mpz_t i;
		mpz_init(i);

		mpz_set_ui(a, 1);		
		for (mpz_add_ui(target, target, 1); mpz_cmp(target, i); mpz_add_ui(i, i, 1))
		{
			mpz_add(p, a, b);
			mpz_set(a, b);
			mpz_set(b, p);
		
			if (args.print_all_flag || (mpz_cmp(target, p) <= 0))
			{
				mpz_out_str(NULL, args.base_arg, a);
				putchar('\n');
			}
		}
	}
	else
	{
		mpz_t q, tmp;
		
		mpz_inits(q, tmp, NULL);
		mpz_set_ui(b, 1);
		mpz_set_ui(q, 1);

		while (mpz_cmp_ui(target, 0))
		{
			mpz_cdiv_r_ui(tmp, target, 2);
			if (mpz_cmp_ui(tmp, 0))
			{
				mpz_mul(tmp, b, q);
				
				mpz_mul(b, b, p);
				mpz_addmul(b, a, q);
				mpz_add(b, b, tmp);

				mpz_mul(a, a, p);
				mpz_add(a, a, tmp);

				mpz_sub_ui(target, target, 1);
			}
			else
			{
				mpz_mul(tmp, q, q);

				mpz_mul(q, q, p);
				mpz_mul_2exp(q, q, 1);
				mpz_add(q, q, tmp);

				mpz_mul(p, p, p);
				mpz_add(p, p, tmp);

				mpz_cdiv_q_ui(target, target, 2);
			}
		}

		mpz_out_str(NULL, args.base_arg, a);
		putchar('\n');
		mpz_clears(q, tmp, NULL);
	}

	mpz_clears(a, b, p, NULL);
}

int
main(int argc, char *argv[])
{
	unsigned int i;
	mpz_t target;
	
	progname = basename(argv[0]);
	mpz_init(target);

	if (ggo(argc, argv, &args))
		return 1;

	if (args.help_given)
	{
		ggo_print_help();
		return 0;
	}
	if (args.version_given)
	{
		ggo_print_version();
		return 0;
	}
	
	if (args.inputs_num)
	{
		for (i=0; i<args.inputs_num; i++)
		{
			if (mpz_set_str(target, args.inputs[i], 0) || (mpz_cmp_d(target, 0) < 0))
			{
				if (!args.silent_flag)
					fprintf(stderr, "%s: %s: Bad argument\n", progname, args.inputs[i]);

				status = 1;
				continue;
			}

			if (!args.quiet_flag)
			{
				mpz_out_str(NULL, 10, target);
				fputs(": ", stdout);
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
				if (!args.silent_flag)
					fprintf(stderr, "%s: %s: Bad argument\n", progname, yytext);

				errno = 0;
				status = 1;
				continue;
			}

			if (!args.quiet_flag)
			{
				mpz_out_str(NULL, 10, target);
				fputs(": ", stdout);
			}
			if (args.max_value_flag)
				fib_maxv(target);
			else
				fib_iter(target);
		}
	}

	return args.loose_exit_status_flag ? 0 : status;
}
