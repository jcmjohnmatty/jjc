#ifndef __JJC_ERRORS_H_
#define __JJC_ERRORS_H_

#define JJC_ERR_WARNING 0
#define JJC_ERR_ERROR   1
#define JJC_ERR_FATAL   2

/*
 * Processing instruction for error reporting routine.
 */

/** Print error and return to the caller. */
#define CONTINUE 0
/** Print the fatal error and abort execution. */
#define ABORT 1

/*
 * error type for error reporting routine
 */

#define STACK_OVERFLOW 100
#define REDECLARATION 101
#define SYMTBL_OVERFLOW 102
#define UNDECLARATION 103
#define ATTR_OVERFLOW 104
#define VAR_VAL 110
#define CONSTANT_VAR 111
#define EXPR_VAR 112
#define CONSTANT_ASSIGN 113
#define FIELD_MIS 115
#define FORW_REDECLARE 116
#define REC_TYPE_MIS 117
#define ARR_TYPE_MIS 118
#define VARIABLE_MIS 119
#define FUNC_MISMATCH 120
#define TYPE_MIS 121
#define MULTI_MAIN 124

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
 *
 */
int
semantic_error (int type,
                int action,
                int id,
                int symtbl_number,
                int line,
                int column);

/**
 * @return An appropriately defined macro indicating the severity.
 */
int
warning_line_column (int         line,
                     int         column,
                     const char* message);

#endif /* __JJC_ERRORS_H_ */
