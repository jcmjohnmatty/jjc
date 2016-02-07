#ifndef __JJC_ERRORS_H_
#define __JJC_ERRORS_H_

int
error_line_column (const char* sourcefile,
				   int         line,
				   const char* message);

#endif /* __JJC_ERRORS_H_ */
