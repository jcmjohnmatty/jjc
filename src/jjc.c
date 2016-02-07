#include <getopt.h>
#include <stdio.h>

#include <lexer.h>

extern int yyline;
extern int yycolumn;

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
		  printf ("The output of our lexer for file `%s'\n", argv[optind]);
          ++optind;

          if (!infile)
            {
              /* ERROR */
            }
          yyin = infile;

#define PRINT_STBL_ROWS(l, c, t, i)     \
          printf("%-19s",   (l));       \
          printf("%-19s",   (c));       \
          printf("%-19s",   (t));       \
          printf("%-19s\n", (i));

#define PRINT_STBL_ROWIN(l, c, t)       \
          printf("\r%-19d",   (l));       \
          printf("%-19d",   (c));       \
          printf("%-19s\n",   (t));

#define PRINT_STBL_ROWIY(l, c, t, i)    \
          printf("\r%-19d",   (l));       \
          printf("%-19d",   (c));       \
          printf("%-19s",   (t));       \
          printf("%-19d\n", (i));

          /* Recongnize the tokens. */
          symtbl* symtbl = symtbl_new ();
          int symcnt = 0;
          int hit_eof = 0;
          int token = -1;
		  PRINT_STBL_ROWS("Line", "Column", "Token", "Index in String table")
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
                  if (!symtbl_contains_value (symtbl, yytext))
                    {
                      symtbl_put (symtbl, symcnt, yytext);
                    }
				  PRINT_STBL_ROWIY(yyline, yycolumn, "IDnum", symcnt)
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
                  if (!symtbl_contains_value (symtbl, yytext))
                    {
                      symtbl_put (symtbl, symcnt, yytext);
                    }
				  PRINT_STBL_ROWIY(yyline, yycolumn, "ICONSTnum", symcnt)
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
                  if (!symtbl_contains_value (symtbl, yytext))
                    {
                      symtbl_put (symtbl, symcnt, yytext);
                    }
				  PRINT_STBL_ROWIY(yyline, yycolumn, "SCONSTnum", symcnt)
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
                  printf ("\nEnd of file\n");
                  break;
                }
              else
                {
                  /* ERROR */
                }
              ++symcnt;
            }
          symtbl_delete (symtbl);
        }
    }
}
