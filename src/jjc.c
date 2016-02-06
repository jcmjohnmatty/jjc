#include <lex.yy.h>

int
main (int argc, char* argv[]) {
  while (true)
	{
	  int token = yylex ();
	  printf ("%s\n", yytext);
	  switch (token)
		{
		case ANDnum:
		  break;

		case ASSGNnum:
		  break;

		case DECLARATIONSnum:
		  break;

		case DOTnum:
		  break;

		case ENDDECLARATIONSnum:
		  break;

		case EQUALnum:
		  break;

		case GTnum:
		  break;

		case IDnum:
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
		  break;

		case TIMESnum:
		  break;

		case VOIDnum:
		  break;

		case EOFnum:
		  break;
		}
	}
}
