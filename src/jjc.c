#include <assert.h>
#include <getopt.h>
#include <stdio.h>

#include <lexer.h>

extern int yyline;
extern int yycolumn;
extern int yylval;

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
		  fprintf (stderr, "jjc: fatal error: missing input file\n");
		  print_usage ();
		  exit (1);
        }

      printf ("The output of our lexer for file `%s':\n\n", argv[optind]);

      ++optind;

      yyin = infile;

#define PRINT_STBL_ROWS(l, c, t, i)             \
      printf("%-6s",     (l));                  \
      printf("%-10s",    (c));                  \
      printf("%-19s",    (t));                  \
      printf("%-19s\n",  (i));

#define PRINT_STBL_ROWIN(l, c, t)               \
      printf("\r%-6d",   (l));                  \
      printf("%-10d",    (c));                  \
      printf("%-19s\n",  (t));

#define PRINT_STBL_ROWIY(l, c, t, i)            \
      printf("\r%-6d",   (l));                  \
      printf("%-10d",    (c));                  \
      printf("%-19s",    (t));                  \
      printf("%-19d\n",  (i));

      /* Recongnize the tokens. */
      int idx = -1;
      int hit_eof = 0;
      int token = -1;
      PRINT_STBL_ROWS("Line", "Column", "Token", "Index_in_String_table")

      while (1)
        {
          token = yylex ();

          switch (token)
            {
            case ANDnum:
              PRINT_STBL_ROWIN(yyline, yycolumn, "ANDnum")
                break;

            case ASSGNnum:
              PRINT_STBL_ROWIN(yyline, yycolumn, "ASSGNnum")
                break;

            case DECLARATIONnum:
              PRINT_STBL_ROWIN(yyline, yycolumn, "DECLARATIONnum")
                break;

            case DOTnum:
              PRINT_STBL_ROWIN(yyline, yycolumn, "DOTnum")
                break;

            case ENDDECLARATIONnum:
              PRINT_STBL_ROWIN(yyline, yycolumn, "ENDDECLARATIONnum")
                break;

            case EQUALnum:
              PRINT_STBL_ROWIN(yyline, yycolumn, "EQUALnum")
                break;

            case GTnum:
              PRINT_STBL_ROWIN(yyline, yycolumn, "GTnum")
                break;

            case IDnum:
              PRINT_STBL_ROWIY(yyline, yycolumn, "IDnum", yylval)
                break;

            case INTnum:
              PRINT_STBL_ROWIN(yyline, yycolumn, "INDnum")
                break;

            case LBRACnum:
              PRINT_STBL_ROWIN(yyline, yycolumn, "LBRACnum")
                break;

            case LPARENnum:
              PRINT_STBL_ROWIN(yyline, yycolumn, "LPARENnum")
                break;

            case METHODnum:
              PRINT_STBL_ROWIN(yyline, yycolumn, "METHODnum")
                break;

            case NEnum:
              PRINT_STBL_ROWIN(yyline, yycolumn, "NEnum")
                break;

            case ORnum:
              PRINT_STBL_ROWIN(yyline, yycolumn, "ORnum")
                break;

            case PROGRAMnum:
              PRINT_STBL_ROWIN(yyline, yycolumn, "PROGRAMnum")
                break;

            case RBRACnum:
              PRINT_STBL_ROWIN(yyline, yycolumn, "RBRACnum")
                break;

            case RPARENnum:
              PRINT_STBL_ROWIN(yyline, yycolumn, "RPARENnum")
                break;

            case SEMInum:
              PRINT_STBL_ROWIN(yyline, yycolumn, "SEMInum")
                break;

            case VALnum:
              PRINT_STBL_ROWIN(yyline, yycolumn, "VALnum")
                break;

            case WHILEnum:
              PRINT_STBL_ROWIN(yyline, yycolumn, "WHILEnum")
                break;

            case CLASSnum:
              PRINT_STBL_ROWIN(yyline, yycolumn, "CLASSnum")
                break;

            case COMMAnum:
              PRINT_STBL_ROWIN(yyline, yycolumn, "COMMAnum")
                break;

            case DIVIDEnum:
              PRINT_STBL_ROWIN(yyline, yycolumn, "DIVIDEnum")
                break;

            case ELSEnum:
              PRINT_STBL_ROWIN(yyline, yycolumn, "ELSEnum")
                break;

            case EQnum:
              PRINT_STBL_ROWIN(yyline, yycolumn, "EQnum")
                break;

            case GEnum:
              PRINT_STBL_ROWIN(yyline, yycolumn, "GEnum")
                break;

            case ICONSTnum:
              PRINT_STBL_ROWIY(yyline, yycolumn, "ICONSTnum", yylval)
                break;

            case IFnum:
              PRINT_STBL_ROWIN(yyline, yycolumn, "IFnum")
                break;

            case LBRACEnum:
              PRINT_STBL_ROWIN(yyline, yycolumn, "LBRACEnum")
                break;

            case LEnum:
              PRINT_STBL_ROWIN(yyline, yycolumn, "LEnum")
                break;

            case LTnum:
              PRINT_STBL_ROWIN(yyline, yycolumn, "LTnum")
                break;

            case MINUSnum:
              PRINT_STBL_ROWIN(yyline, yycolumn, "MINUSnum")
                break;

            case NOTnum:
              PRINT_STBL_ROWIN(yyline, yycolumn, "NOTnum")
                break;

            case PLUSnum:
              PRINT_STBL_ROWIN(yyline, yycolumn, "PLUSnum")
                break;

            case RBRACEnum:
              PRINT_STBL_ROWIN(yyline, yycolumn, "RBRACEnum")
                break;

            case RETURNnum:
              PRINT_STBL_ROWIN(yyline, yycolumn, "RETURNnum")
                break;

            case SCONSTnum:
              PRINT_STBL_ROWIY(yyline, yycolumn, "SCONSTnum", yylval)
                break;

            case TIMESnum:
              PRINT_STBL_ROWIN(yyline, yycolumn, "TIMESnum")
                break;

            case VOIDnum:
              PRINT_STBL_ROWIN(yyline, yycolumn, "VOIDnum")
                break;

            case EOFnum:
              PRINT_STBL_ROWS("", "", "EOFnum", "")
                hit_eof = 1;
              break;
            }

          if (hit_eof == 1)
            {
              printf ("\nString Table : ");
              strtbl_print (string_table);
              printf ("End of file\n");
              break;
            }
        }
    }
  else
	{
	  fprintf (stderr, "jjc: fatal error: missing input file\n");
	  print_usage ();
	  exit (1);
	}
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
