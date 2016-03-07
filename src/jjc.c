#include <assert.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include <lexer.h>
#include <parser.h>

#include <errors.h>
#include <jjc.h>
#include <strtbl.h>
#include <tokens.h>

static const char* version_no = "1.0.0";

static struct option long_options[] =
  {
    {"verbose", no_argument, 0, 'V'},
	{"version", no_argument, 0, 'v'},
    {"brief",   no_argument, 0, 'b'},
    {"help",    no_argument, 0, 'h'},
    {0, 0, 0, 0}
  };

char* sourcefile;

int
main (int argc, char* argv[]) {
  int c;
  int verbose_flag;
  char* filename;

  /* Parse arguments. */
  while (1)
    {
      int option_index = 0;

      c = getopt_long (argc, argv, "Vvbh",
                       long_options, &option_index);

      /* Detect the end of the options. */
      if (c == -1)
        break;

      switch (c)
        {
        case 'V':
          if (verbose_flag == 0)
            {
			  fprintf (stderr, "jjc: fatal error: brief flag already set\n");
			  print_usage ();
			  exit (1);
            }
          verbose_flag = 1;
          break;

		case 'v':
		  print_version ();
		  exit (0);
		  break;

        case 'b':
          if (verbose_flag == 1)
            {
			  fprintf (stderr, "jjc: fatal error: verbose flag already set\n");
			  print_usage ();
			  exit (1);
            }
          break;

        case 'h':
		  print_usage ();
          exit (0);
          break;

        case '?':
		  fprintf (stderr, "jjc: fatal error: unrecognized flag `-%s'\n", c);
          break;

        default:
          abort ();
        }
    }

  /* Print any remaining command line arguments (not options). */
  if (optind < argc)
    {
      /* Just one input source... */
      assert (optind + 1 == argc);

      /* Allocate string table. */
      string_table = strtbl_new ();
      /* Specify the input file. */
	  sourcefile = argv[optind];
      FILE *infile = fopen (argv[optind], "r");

      if (!infile)
        {
		  fprintf (stderr, "jjc: fatal error: unable to open file `%s'\n", sourcefile);
		  print_usage ();
		  exit (1);
        }

      printf ("The output of our lexer for file `%s':\n\n", argv[optind]);

      ++optind;

      yyin = infile;

      yyparse ();
    }
  else
	{
	  fprintf (stderr, "jjc: fatal error: missing input file\n");
	  print_usage ();
	  exit (1);
	}

  if (jjc_errno > 0)
	{
	  exit (1);
	}

  return 0;
}

void
print_version (void)
{
  printf ("jjc %s\nCopyright 2016 John C. Matty.\n", version_no);
}

void
print_usage (void)
{
  printf (
"Usage: jjc [options] file\n"
"Options:\n"
"  --help       Print a brief help message, and then exit.\n"
"  --brief      Print as little information about the compilation of the input\n"
"               FILEs as possible.\n"
"  --verbose    Print verbose information about the compilation of the input\n"
"               FILEs.\n"
"  --verbose    Print jjc\'s version and exit.\n"
"\n"
"To report bugs, please contact John C. Matty <jcmjohnmatty@me.com>.\n");
}
