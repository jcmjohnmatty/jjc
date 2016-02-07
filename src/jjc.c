#include <getopt.h>
#include <stdio.h>

#include <lexer.h>

#include <symtbl.h>
#include <tokens.h>

static int verbose_flag = -1;

static struct option long_options[] =
  {
	{"verbose", no_argument, 0, 'V'},
	{"brief",   no_argument, 0, 'b'},
	{"help",    no_argument, 0, 'h'},
	{0, 0, 0, 0}
  };

int
main (int argc, char* argv[]) {
  int c;
  char* filename;

  /* Parse arguments. */
  while (1)
    {
      int option_index = 0;

      c = getopt_long (argc, argv, "Vbh",
                       long_options, &option_index);

      /* Detect the end of the options. */
      if (c == -1)
        break;

      switch (c)
        {
		case 'V':
		  if (verbose_flag == 0)
			{
			  /* ERROR */
			}
		  verbose_flag = 1;
		  break;

		case 'b':
		  if (verbose_flag == 1)
			{
			  /* ERROR */
			}
		  break;

		case 'h':
		  exit (0);
		  break;

        case '?':
          break;

        default:
          abort ();
        }
    }

  /* Print any remaining command line arguments (not options). */
  if (optind < argc)
	{
	  while (optind < argc)
		{
		  /* Specify the input file. */
		  FILE *infile = fopen (argv[optind], "r");
		  ++optind;

		  if (!infile)
			{
			  /* ERROR */
			}
		  yyin = infile;

		  /* Recongnize the tokens. */
		  symtbl* symtbl = malloc (sizeof (symtbl));
		  int symcnt = 0;
		  while (1)
			{
			  int hit_eof = 0;
			  int token = yylex ();

			  switch (token)
				{
				case ANDnum:
				  break;

				case ASSGNnum:
				  break;

				case DECLARATIONnum:
				  break;

				case DOTnum:
				  break;

				case ENDDECLARATIONnum:
				  break;

				case EQUALnum:
				  break;

				case GTnum:
				  break;

				case IDnum:
				  if (!symtbl_contains_value (symtbl, yytext))
					{
					  symtbl_put (symtbl, symcnt, yytext);
					}
				  printf ("%s ", yytext);
				  break;

				case INTnum:
				  break;

				case LBRACnum:
				  break;

				case LPARENnum:
				  break;

				case METHODnum:
				  break;

				case NEnum:
				  break;

				case ORnum:
				  break;

				case PROGRAMnum:
				  break;

				case RBRACnum:
				  break;

				case RPARENnum:
				  break;

				case SEMInum:
				  break;

				case VALnum:
				  break;

				case WHILEnum:
				  break;

				case CLASSnum:
				  break;

				case COMMAnum:
				  break;

				case DIVIDEnum:
				  break;

				case ELSEnum:
				  break;

				case EQnum:
				  break;

				case GEnum:
				  break;

				case ICONSTnum:
				  if (!symtbl_contains_value (symtbl, yytext))
					{
					  symtbl_put (symtbl, symcnt, yytext);
					}
				  printf ("%s ", yytext);
				  break;

				case IFnum:
				  break;

				case LBRACEnum:
				  break;

				case LEnum:
				  break;

				case LTnum:
				  break;

				case MINUSnum:
				  break;

				case NOTnum:
				  break;

				case PLUSnum:
				  break;

				case RBRACEnum:
				  break;

				case RETURNnum:
				  break;

				case SCONSTnum:
				  if (!symtbl_contains_value (symtbl, yytext))
					{
					  symtbl_put (symtbl, symcnt, yytext);
					}
				  printf ("%s ", yytext);
				  break;

				case TIMESnum:
				  break;

				case VOIDnum:
				  break;

				case EOFnum:
				  hit_eof = 1;
				  break;
				}

			  if (hit_eof == 1)
				{
				  printf ("\nEnd of file\n");
				  break;
				}
			  else
				{
				  /* ERROR */
				}
			}
		  free (symtbl);
		}
	}
}
