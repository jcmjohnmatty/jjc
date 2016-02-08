#ifndef __JJC_ERRORS_H_
#define __JJC_ERRORS_H_

#define JJC_ERR_WARNING 0
#define JJC_ERR_ERROR   1
#define JJC_ERR_FATAL   2

extern int jjc_errno;

/**
 * @return An appropriately defined macro indicating the severity.
 */
int
fatal_error_line_column (int         line,
                         int         column,
                         const char* message);

/**
 * @return An appropriately defined macro indicating the severity.
 */
int
error_line_column (int         line,
                   int         column,
                   const char* message);

/**
 * @return An appropriately defined macro indicating the severity.
 */
int
warning_line_column (int         line,
                     int         column,
                     const char* message);

#endif /* __JJC_ERRORS_H_ */
