#include <assert.h>
#include <getopt.h>
#include <stdio.h>

#include <lexer.h>

extern int yyline;
extern int yycolumn;
extern int yylval;

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
            case AND:
              PRINT_STBL_ROWIN(yyline, yycolumn, "AND")
                break;

            case ASSGN:
              PRINT_STBL_ROWIN(yyline, yycolumn, "ASSGN")
                break;

            case DECLARATION:
              PRINT_STBL_ROWIN(yyline, yycolumn, "DECLARATION")
                break;

            case DOT:
              PRINT_STBL_ROWIN(yyline, yycolumn, "DOT")
                break;

            case ENDDECLARATION:
              PRINT_STBL_ROWIN(yyline, yycolumn, "ENDDECLARATION")
                break;

            case EQUAL:
              PRINT_STBL_ROWIN(yyline, yycolumn, "EQUAL")
                break;

            case GT:
              PRINT_STBL_ROWIN(yyline, yycolumn, "GT")
                break;

            case ID:
              PRINT_STBL_ROWIY(yyline, yycolumn, "ID", yylval)
                break;

            case INT:
              PRINT_STBL_ROWIN(yyline, yycolumn, "IND")
                break;

            case LBRAC:
              PRINT_STBL_ROWIN(yyline, yycolumn, "LBRAC")
                break;

            case LPAREN:
              PRINT_STBL_ROWIN(yyline, yycolumn, "LPAREN")
                break;

            case METHOD:
              PRINT_STBL_ROWIN(yyline, yycolumn, "METHOD")
                break;

            case NE:
              PRINT_STBL_ROWIN(yyline, yycolumn, "NE")
                break;

            case OR:
              PRINT_STBL_ROWIN(yyline, yycolumn, "OR")
                break;

            case PROGRAM:
              PRINT_STBL_ROWIN(yyline, yycolumn, "PROGRAM")
                break;

            case RBRAC:
              PRINT_STBL_ROWIN(yyline, yycolumn, "RBRAC")
                break;

            case RPAREN:
              PRINT_STBL_ROWIN(yyline, yycolumn, "RPAREN")
                break;

            case SEMI:
              PRINT_STBL_ROWIN(yyline, yycolumn, "SEMI")
                break;

            case VAL:
              PRINT_STBL_ROWIN(yyline, yycolumn, "VAL")
                break;

            case WHILE:
              PRINT_STBL_ROWIN(yyline, yycolumn, "WHILE")
                break;

            case CLASS:
              PRINT_STBL_ROWIN(yyline, yycolumn, "CLASS")
                break;

            case COMMA:
              PRINT_STBL_ROWIN(yyline, yycolumn, "COMMA")
                break;

            case DIVIDE:
              PRINT_STBL_ROWIN(yyline, yycolumn, "DIVIDE")
                break;

            case ELSE:
              PRINT_STBL_ROWIN(yyline, yycolumn, "ELSE")
                break;

            case EQ:
              PRINT_STBL_ROWIN(yyline, yycolumn, "EQ")
                break;

            case GE:
              PRINT_STBL_ROWIN(yyline, yycolumn, "GE")
                break;

            case ICONST:
              PRINT_STBL_ROWIY(yyline, yycolumn, "ICONST", yylval)
                break;

            case IF:
              PRINT_STBL_ROWIN(yyline, yycolumn, "IF")
                break;

            case LBRACE:
              PRINT_STBL_ROWIN(yyline, yycolumn, "LBRACE")
                break;

            case LE:
              PRINT_STBL_ROWIN(yyline, yycolumn, "LE")
                break;

            case LT:
              PRINT_STBL_ROWIN(yyline, yycolumn, "LT")
                break;

            case MINUS:
              PRINT_STBL_ROWIN(yyline, yycolumn, "MINUS")
                break;

            case NOT:
              PRINT_STBL_ROWIN(yyline, yycolumn, "NOT")
                break;

            case PLUS:
              PRINT_STBL_ROWIN(yyline, yycolumn, "PLUS")
                break;

            case RBRACE:
              PRINT_STBL_ROWIN(yyline, yycolumn, "RBRACE")
                break;

            case RETURN:
              PRINT_STBL_ROWIN(yyline, yycolumn, "RETURN")
                break;

            case SCONST:
              PRINT_STBL_ROWIY(yyline, yycolumn, "SCONST", yylval)
                break;

            case TIMES:
              PRINT_STBL_ROWIN(yyline, yycolumn, "TIMES")
                break;

            case VOID:
              PRINT_STBL_ROWIN(yyline, yycolumn, "VOID")
                break;

            case EOF:
              PRINT_STBL_ROWS("", "", "EOF", "")
                hit_eof = 1;
              break;
            }

          if (hit_eof == 1)
            {
			  if (jjc_errno <= 0)
				{
				  printf ("\nString Table : ");
				  strtbl_print (string_table);
				}
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
