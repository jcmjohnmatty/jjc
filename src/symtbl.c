#include <stdio.h>
#include <stdlib.h>

#include <ast.h>
#include <errors.h>
#include <tokens.h>
#include <strtbl.h>
#include <symtbl.h>

/*
 * symbol table array
 */
int symtbl_array[SYMTBL_SIZE];

/*
 * attribute array.  attributes are allocated from this array instead of from
 * dynamically allocated space.  It may easy testing and debugging.
 */
attribute_type attribute_array[ATTR_SIZE];

/*
 * The stack of symbol tables.
 */
symtbl_stack stack[STACK_SIZE];

/* stack top counter */
int stack_top = 0;
/* symbol table top counter */
int symtbl_top = 0;
/* nesting level counter */
int nesting = 0;
/* attribute array counter */
int attr_top = 0;

extern int yyline;
extern int yycolumn;

void
_symtbl_process_declaration_block (ast* declarations)
{
  /* Get to the declarations first. */
  while (!ast_is_null (declarations))
    {
      /* Process. */
      ast* field_declaration;

      field_declaration = declarations->right;

      if (ast_is_null (field_declaration))
        {
          error_line_column (field_declaration->line,
                             field_declaration->column,
                             "empty field declaration");
          /* Let's find more errors!!! */
          declarations = declarations->left;
          continue;
        }

      while (!ast_is_null (field_declaration))
        {
          ast* variable = field_declaration->right;
          ast* variable_type;
          int symtbl_index;
          int id;

          if (ast_is_null (variable))
            {
              error_line_column (variable->line,
                                 variable->column,
                                 "empty variable declaration");
              /* Let's find more errors!!! */
              field_declaration = field_declaration->left;
              continue;
            }

          /* Process the field declaration. */
          variable_type = variable->right->left;
          variable = variable->left;

          if (variable->operation_type == 0)
            {
              symtbl_index =
                symtbl_insert_entry (variable->data,
                                     variable->line,
                                     variable->column);
              symtbl_set_attribute (symtbl_index, TREE_ATTR,
                                    variable);
              symtbl_set_attribute (symtbl_index, TYPE_ATTR,
                                    variable_type);
              symtbl_set_attribute (symtbl_index, PREDE_ATTR, 0);
              symtbl_set_attribute (symtbl_index, KIND_ATTR, VAR);
            }
          else if (variable->left->operation_type == COMMAOP)
            {
              ast* variable_declaration_id = variable;
              int dim = 0;
              do
                {
                  variable_declaration_id = variable_declaration_id->right;
                  ++dim;
                }
              while (variable_declaration_id->operation_type != 0);

              int symtbl_index =
                symtbl_insert_entry (variable->data,
                                     variable->line,
                                     variable->column);
              symtbl_set_attribute (symtbl_index, TREE_ATTR,
                                    variable_declaration_id);
              symtbl_set_attribute (symtbl_index, PREDE_ATTR, 0);
              symtbl_set_attribute (symtbl_index, TYPE_ATTR, variable_type);
              symtbl_set_attribute (symtbl_index, KIND_ATTR, ARR);
              symtbl_set_attribute (symtbl_index, DIMEN_ATTR, dim);
            }
          else
            {
              char message[64];
              sprintf (message, "illegal variable with node type %d",
                       variable->left->node_type);
              error_line_column (variable->line,
                                 variable->column,
                                 message);
            }

          /* Get the next field declaration. */
          field_declaration = field_declaration->left;
        }

      /* Finally, get the next declaration. */
      declarations = declarations->left;
    }
}

int
_symtbl_process_expression (ast* expression);

int
_symtbl_process_variable (ast* variable)
{
  int dim = 0;
  int symtbl_index;
  int real_dim = -1;
  ast* var_index_field;
  int l;
  int c;

  if (!ast_is_null (variable->right))
    {
      while (!ast_is_null (variable))
        {
          switch (variable->operation_type)
            {
            case SELECTOP:
              var_index_field = variable->left;
              l = var_index_field->line;
              c = var_index_field->column;
              symtbl_index = symtbl_lookup (var_index_field->data, l, c);
              if (var_index_field->operation_type == FIELDOP)
                {
                  /* if (symtbl_index == 0) */
                  /*   { */
                  /*     /\* Undeclared. *\/ */
                  /*     semantic_error (UNDECLARATION, CONTINUE, */
                  /*                     var_index_field->data, 0, */
                  /*                     var_index_field->line, */
                  /*                     var_index_field->column); */
                  /*   } */
                  break;
                }

            case INDEXOP:
              /* Make sure we don't have too many dimensions. */
              while (!ast_is_null (var_index_field))
                {
                  ++dim;
                  _symtbl_process_expression (var_index_field->left);
                  var_index_field = var_index_field->right;
                }
              if (dim != symtbl_get_attribute (symtbl_index, DIMEN_ATTR))
                {
                  error_line_column (l, c, "dimension mismatch");
                }

              break;

            default:

              break;
            }

          /* Get the thing to the right. */
          variable = variable->right;
        }
    }

  /* Finally, return the dimensions of variable. */
  return dim;
}

int
_symtbl_process_type(ast* type)
{
  int kind;
  int dim = 0;

  if (type->left->node_type == INTEGERTNODE)
    {
      /* Just an integer (or an integer array). */
      if (!ast_is_null (type->right))
        {
          /* Get the number of dimensions. */
          while (!ast_is_null (type))
            {
              ++dim;
              type = type->right;
            }
          return dim;
        }
      else
        {
          return 0;
        }
    }

  if (type->left->node_type == IDNODE)
    {
      /* Check if this id has been declared. */
      int symtbl_index = symtbl_lookup (type->left->data,
                                        type->left->line,
                                        type->left->column);

      /* Check if this id is a variable. */
      kind = symtbl_get_attribute (symtbl_index, KIND_ATTR);
      if (kind != CLASS_KIND)
        {
          char* s = string_table->buffer + type->left->data + 1;
          char* c = malloc (strlen (s) + 16);
          sprintf (c, "no such class %s\n", s);
          error_line_column (type->left->line, type->left->column, c);
          free (c);
        }

      /* Check to make sure this isn't an array if we aren't at the end. */
      if (!ast_is_null (type->right))
        {
          type = type->right;
        }
      while (!ast_is_null (type->right))
        {
          if (type->operation_type == INDEXOP)
            {
              if (ast_is_null (type->right) && dim > 0 && !ast_is_null (type->left))
                {
                  error_line_column (type->right->line,
                                     type->right->column,
                                     "member selection on array");
                  return -30;
                }
              ++dim;
            }
          if (type->operation_type == FIELDOP)
            {
              if (dim > 0)
                {
                  error_line_column (type->right->line,
                                     type->right->column,
                                     "member selection on array");
                  return -30;
                }
              type = type->left;
              /* Check if this id has been declared. */
              int symtbl_index = symtbl_lookup (type->left->data,
                                                type->left->line,
                                                type->left->column);

              /* Check if this id is a variable. */
              kind = symtbl_get_attribute (symtbl_index, KIND_ATTR);
              if (kind != CLASS_KIND)
                {
                  char* s = string_table->buffer + type->left->data + 1;
                  char* c = malloc (strlen (s) + 16);
                  sprintf (c, "no such class %s\n", s);
                  error_line_column (type->left->line, type->left->column, c);
                  free (c);
                }
            }
          type = type->right;
        }
    }
}

int
_symtbl_process_methodcall (ast* methodcall)
{
  int num_args = 0;
  int var_dim = -5;
  int dim = -1;
  int methodcall_symtbl_index = -4;

  /* Check the variable we are calling the method from. */
  var_dim = _symtbl_check_variable (methodcall->left);

  if (var_dim > 0)
    {
      error_line_column (methodcall->line, methodcall->column,
                         "unable to call method on array");
    }

  /* Get the index in the symbol table for this method. */

  ast* last_variable = methodcall->left;
  int id = -6;
  int l = last_variable->line;
  int c = last_variable->column;
  while (!ast_is_null (last_variable->right))
    {
      if (last_variable->left->node_type == IDNODE)
        {
          id = last_variable->left->data;
          l = last_variable->left->line;
          c = last_variable->left->column;
        }
      if (last_variable->left->operation_type == FIELDOP)
        {
          id = last_variable->left->data;
          l = last_variable->left->line;
          c = last_variable->left->column;
        }
    }

  methodcall_symtbl_index = symtbl_lookup (id, l, c);

  while (!ast_is_null (methodcall))
    {
      ++num_args;

      /* Each of the arguments needs to be a legal expression. */
      dim = _symtbl_process_expression (methodcall->left);

      /*
       * Arrays cannot be passed as arguments, so if dim > 1, we already know
       * this is not a legal method call statement.
       */
      if (dim > 1)
        {
          error_line_column (methodcall->line, methodcall->column,
                             "arrays cannot be passed as arguments");
        }

      /* Check the next one. */
      methodcall = methodcall->right;
    }

  /* Make sure we have the required number of arguments. */
  int real_num_args = symtbl_get_attribute (methodcall_symtbl_index, ARGNUM_ATTR);
  if (num_args != real_num_args)
    {
      char message[64];
      sprintf (message, "expected %d arguments, got %d", real_num_args, num_args);
      error_line_column (methodcall->line, methodcall->column, message);
    }

  return symtbl_get_attribute (methodcall_symtbl_index, DIMEN_ATTR);
}

int
_symtbl_process_unsigned_constant (ast* unsigned_constant)
{
  /* These are constant, so there is no need to do any further checking. */
  return 0;
}

int
_symtbl_process_routinecall (ast* routinecall);

int
_symtbl_process_factor (ast* factor)
{
  int dim = -20;
  int operation_type = factor->operation_type;
  char message[64];

  /* Check based on the factor type. */
  switch (operation_type)
    {
    case CONSTANTIDOP:
      return 0;

    case VAROP:
      dim = _symtbl_process_variable (factor);
      break;

    case ROUTINECALLOP:
      dim = _symtbl_process_routinecall (factor);
      break;

    case UNARYNEGOP:
      dim = _symtbl_process_factor (factor->left);
      break;

    /* Any of these are an expression. */
    /** @todo Lookup and insert other possible types. */
    case ASSIGNOP:
    case RETURNOP:
    case IFELSEOP:
    case LOOPOP:
    case LTOP:
    case LEOP:
    case EQOP:
    case NEOP:
    case GEOP:
    case GTOP:
      dim = _symtbl_process_expression (factor);
      break;

    default:
      sprintf (message, "unrecognized operation %d",
               factor->operation_type);
      error_line_column (factor->line,
                         factor->column,
                         message);
      break;
    }

  return dim;
}

int
_symtbl_process_term (ast* term)
{
  int rhs_dim = -12;
  int lhs_dim = -13;

  /** @todo Check if term->right is NULL? */
  rhs_dim = _symtbl_process_factor (term->right);

  /* Make sure we don't have a type mismatch. */

  /* (term->right->node_type == STRINGNODE */
  /*  && (!ast_is_null (term->left) || term->left->node_type != INTEGERTNODE)) */
  /*   || term->right->node_type  */
  /* if (term->right->node_type != term->left->node_type) */

  if (ast_is_null (term->left))
    {
      lhs_dim = _symtbl_process_factor (term->left);
    }
  else
    {
      lhs_dim = _symtbl_process_term (term->left);
    }

  if (rhs_dim != lhs_dim)
    {
      error_line_column (term->line, term->column, "dimension mismatch");
      return -14;
    }

  return lhs_dim;
}

int
_symtbl_process_simple_expression (ast* simple_expression)
{
  int rhs_dim = -9;
  int lhs_dim = -10;

  /** @todo Check if term->right is NULL? */
  if (simple_expression->operation_type == UNARYNEGOP)
    {
      rhs_dim = _symtbl_process_term (simple_expression->right->left);
    }
  else
    {
      rhs_dim = _symtbl_process_term (simple_expression->right);
    }

  if (ast_is_null (simple_expression->left))
    {
      lhs_dim = _symtbl_process_term (simple_expression->left);
    }
  else
    {
      lhs_dim = _symtbl_process_simple_expression (simple_expression->left);
    }

  if (rhs_dim != lhs_dim)
    {
      error_line_column (simple_expression->line, simple_expression->column, "dimension mismatch");
      return -11;
    }

  return lhs_dim;
}

/** @note All expressions are 0 dimensional (scalars). */
int
_symtbl_process_expression (ast* expression)
{
  int left_dim = _symtbl_process_simple_expression (expression->left);
  int right_dim = -7;
  if (!ast_is_null (expression->right))
    {
      right_dim = _symtbl_process_simple_expression (expression->right);

      if (left_dim != right_dim)
        {
          error_line_column (expression->line, expression->column,
                             "dimension mismatch");
          /* Return some invalid value. */
          return -8;
        }
    }

  /* Finally, return the dimensions of expression. */
  return left_dim;
}

void
_symtbl_process_method_statements (ast* statements)
{
  char message[64];
  while (!ast_is_null (statements))
    {
      ast* statement = statements->right;
      int lhs_dim = -1;
      int rhs_dim = -2;

      /* Empty statements are premetted. */
      if (ast_is_null (statement))
        {
          statements = statements->left;
          continue;
        }

      /* Check based on operation type. */
      switch (statement->operation_type)
        {
        case ASSIGNOP:
          /* Check the variable. */
          if (ast_is_null (statement->left))
            {
              error_line_column (statement->line, statement->column,
                                 "missng rvalue in assignment statement");
              break;
            }
          rhs_dim = _symtbl_process_variable (statement->left->right);

          /* Check the expression. */
          if (ast_is_numm (statement->right))
            {
              error_line_column (statement->line, statement->column,
                                 "missng expression in assignment statement");
              break;
            }
          lhs_dim = _symtbl_process_expression (statement->right);

          /* Finally, make sure that the dimensions match. */
          if (rhs_dim != lhs_dim)
            {
              error_line_column (statement->line, statement->column,
                                 "dimension mismatch");
            }

            break;

        case ROUTINECALLOP:
          /* Count the number of arguments. */
          _symtbl_process_routinecall (statement->right);
          break;

        case RETURNOP:
            if (!ast_is_null (statement->left))
              {
                _symtbl_process_expression (statement->left);
              }

          break;

        case IFELSEOP:
          break;

        case LOOPOP:
          break;

        case LTOP:
        case LEOP:
        case EQOP:
        case NEOP:
        case GEOP:
        case GTOP:
          break;

        default:
          sprintf (message, "unrecognized operation %d",
                   statement->operation_type);
          error_line_column (statement->line,
                             statement->column,
                             message);
          break;
        }
      /* Get the next statement. */
      statements = statements->left;
    }
}

/** @todo More descriptive error codes? */
void
symtbl_construct (const ast* const root)
{
  ast* class_declaration;

  symtbl_init ();

  /* Make sure we have a program declaration. */

  if (root->operation_type != PROGRAMOP)
    {
      error_line_column (root->line, root->column,
                         "missing program declaration");
      return;
    }

  /* Begin traversal. */

  /* Get the first class declaration. */
  class_declaration = root->left;

  /* Assert we have at least one class declaration. */
  if (ast_is_null (class_declaration))
    {
      error_line_column (class_declaration->line, class_declaration->column,
                         "class declaration expected");
      return;
    }

  /* While we still have class declarations. */
  while (!ast_is_null (class_declaration))
    {
      /* First, add the entry. */
      int symtbl_index = symtbl_insert_entry (class_declaration->right->right->data,
                                              class_declaration->line,
                                              class_declaration->column);
      symtbl_set_attribute (symtbl_index, TREE_ATTR, class_declaration);
      symtbl_set_attribute (symtbl_index, PREDE_ATTR, 0);
      symtbl_set_attribute (symtbl_index, KIND_ATTR, CLASS_KIND);

      symtbl_open_block ();

      /* Process the class declaration. */
      ast* class_body = class_declaration->right;

      /* Process the class body. */

      /* if (ast_is_null (class_body)) */
      /*   { */
      /*     error_line_column(class_declaration->line, class_declaration->column, */
      /*                       "missing class body"); */
      /*   } */

      if (ast_is_null (class_body))
        {
          /* We don't have anything to process, so continue. */
          class_declaration = class_declaration->left;
          continue;
        }
      else
        {
          /*
           * Declarations are all the way down in the tree for some reason, and
           * as a result, we need to search for them.
           */
          ast* declarations = class_body;
          // int has_field_declaration;
          do
            {
              /* if (!ast_is_null (declarations->right)) */
              /*   { */
              /*     has_field_declaration = declarations->right; */
              /*   } */
              declarations = declarations->left;
            }
          while (declarations->left->right->operation_type != DECLOP);
          declarations = declarations->left;

          _symtbl_process_declaration_block (declarations);

          /* Now, process the methods. */
          ast* t = class_body->left;
          while (!ast_is_null (t) && t->operation_type == BODYOP)
            {
              ast* method_declaration = t->right;
              if (method_declaration->operation_type == DECLOP)
                {
                  break;
                }

              /*
               * Empty method declarations are legal, so if we have one just
               * continue.
               */
              if (ast_is_null (method_declaration))
                {
                  t = t->left;
                  continue;
                }

              /* Put the name in our symbol table. */
              ast* method_declaration_type;

              if (!ast_is_null (method_declaration->left->right))
                {
                  method_declaration_type =
                    method_declaration->left->right->right;
                }
              else
                {
                  method_declaration_type = NULL;
                }

              int return_dim = _symtbl_process_type (method_declaration_type);

              int symtbl_index =
                symtbl_insert_entry (method_declaration->left->left->data,
                                     method_declaration->left->left->line,
                                     method_declaration->left->left->column);
              symtbl_set_attribute (symtbl_index, TREE_ATTR,
                                    method_declaration);
              symtbl_set_attribute (symtbl_index, TYPE_ATTR,
                                    method_declaration_type);
              symtbl_set_attribute (symtbl_index, PREDE_ATTR, 0);
              symtbl_set_attribute (symtbl_index, KIND_ATTR, FUNC);
              symtbl_set_attribute (symtbl_index, DIMEN_ATTR, return_dim);
              /* Open a new block and put in the variables in the method. */
              symtbl_open_block ();

              ast* parameter_list = method_declaration->left->right->left;
              ast* int_type = ast_make_leaf (INTEGERTNODE, INT);
              int n_args = 0;
              while (!ast_is_null (parameter_list))
                {
                  // Frist add the class entry.
                  int symtbl_index = symtbl_insert_entry (parameter_list->left->left->data,
                                                          parameter_list->left->left->line,
                                                          parameter_list->left->left->column);
                  symtbl_set_attribute (symtbl_index, PREDE_ATTR, 0);
                  symtbl_set_attribute (symtbl_index, TYPE_ATTR, int_type);
                  if (parameter_list->operation_type == VARGTYPEOP)
                    {
                      symtbl_set_attribute (symtbl_index, KIND_ATTR, VALUE_ARG);
                    }
                  else
                    {
                      symtbl_set_attribute (symtbl_index, KIND_ATTR, REF_ARG);
                    }

                  ++n_args;
                  parameter_list = parameter_list->right;
                }
              symtbl_set_attribute (symtbl_index, ARGNUM_ATTR, n_args);
              /* Finally, process the block declarations, if there are any. */
              _symtbl_process_declaration_block (method_declaration->right->left);
              _symtbl_process_method_statements (method_declaration->right->right);

              symtbl_close_block ();

              /* Get the next one. */
              t = t->left;
            }
        }

      symtbl_close_block ();

      /* Get the next declaration. */
      class_declaration = class_declaration->left;
    }
}

void
symtbl_init (void)
{
  int strtbl_index;
  int symtbl_index;

  strtbl_index = strtbl_get_index (string_table, "system");

  if (strtbl_index != -1)
    {
      symtbl_index = symtbl_insert_entry (strtbl_index, -1, -1);
      symtbl_set_attribute (symtbl_index, PREDE_ATTR, 1);
      symtbl_set_attribute (symtbl_index, KIND_ATTR, CLASS_KIND);
    }

  strtbl_index = strtbl_get_index (string_table, "readln");
  if (strtbl_index != -1)
    {
      symtbl_index = symtbl_insert_entry (strtbl_index, -1, -1);
      symtbl_set_attribute (symtbl_index, NEST_ATTR, nesting + 1);
      symtbl_set_attribute (symtbl_index, PREDE_ATTR, 1);
      symtbl_set_attribute (symtbl_index, KIND_ATTR, PROCE);
    }

  strtbl_index = strtbl_get_index (string_table, "println");
  if (strtbl_index != -1)
    {
      symtbl_index = symtbl_insert_entry (strtbl_index, -1, -1);
      symtbl_set_attribute (symtbl_index, NEST_ATTR, nesting + 1);
      symtbl_set_attribute (symtbl_index, PREDE_ATTR, 1);
      symtbl_set_attribute (symtbl_index, KIND_ATTR, PROCE);
    }
}

int
symtbl_insert_entry (int id, int line, int column)
{
  /* id is already declared in the current block. */
  if (symtbl_lookup_here (id))
    {
      semantic_error (REDECLARATION, CONTINUE, id, 0, line, column);
      return 0;
    }

  if (symtbl_top >= SYMTBL_SIZE - 1)
    {
      semantic_error (SYMTBL_OVERFLOW, ABORT, 0 , 0, line, column);
    }

  ++symtbl_top;
  symtbl_array[symtbl_top] = 0;
  symtbl_set_attribute (symtbl_top, NAME_ATTR, id);
  symtbl_set_attribute (symtbl_top, NEST_ATTR, nesting);
  symtbl_push (0, id, symtbl_top, 0);
  return symtbl_top;
}

int
symtbl_lookup (int id, int line, int column)
{
  int i;

  for (i = stack_top; i > 0; --i)
    {
      if (!stack[i].is_marker && stack[i].name == id)
        {
          stack[i].used = 1;
          return stack[i].symtbl_index;
        }
    }

  /* id is undefined, push a dummy element onto stack. */
  semantic_error (UNDECLARATION, CONTINUE, id, 0, line, column);
  symtbl_push (0, id, 0, 1);
  return 0;
}

int
symtbl_lookup_here (int id)
{
  int i;

  for (i = stack_top; !stack[i].is_marker; --i)
    {
      if (stack[i].name == id && !stack[i].is_dummy)
        {
          return stack[i].symtbl_index;
        }
    }

  return 0;
}

void
symtbl_open_block (void)
{
  ++nesting;
  /* Push a marker onto stack. */
  symtbl_push (1, 0, 0, 0);
}

void
symtbl_close_block (void)
{
  int i;

  for (i = stack_top; !stack[i].is_marker; --i)
    {
      if (!stack[i].used && !stack[i].is_dummy)
        {
          /* semantic_error (NOT_USED, CONTINUE, stack[i].name, 0);*/
        }
    }

  --nesting;
  /* trim the stack */
  stack_top = i - 1;
}

int
symtbl_is_attribute (int symtbl_index, int attribute_number)
{
  int i;

  i = symtbl_array[symtbl_index];
  while (i)
  {
    if (attribute_array[i].attribute_number == attribute_number)
      {
        return i;
      }
    if (attribute_array[i].attribute_number > attribute_number)
      {
        break;
      }
    i = attribute_array[i].next_attribute_index;
  }
  return 0;
}

int
symtbl_get_attribute (int symtbl_index, int attribute_number)
{
  int i;

  i = symtbl_is_attribute (symtbl_index, attribute_number);
  if (!i)
    {
      printf ("DEBUG: attribute number %d does not exist\n", attribute_number);
      return 0;
    }

  return attribute_array[i].attribute_value;
}

void
symtbl_set_attribute (int symtbl_index, int attribute_number, int attribute_value)
{
  int* p;
  int next;
  int i;

  i = symtbl_is_attribute (symtbl_index, attribute_number);
  if (i)
    {
      attribute_array[i].attribute_value = attribute_value;
      return;
    }

  p = &symtbl_array[symtbl_index];
  next = symtbl_array[symtbl_index];

  /* Search the link list for the right insert position. */
  while (next)
    {
      if (attribute_array[next].attribute_number < attribute_number)
        {
          p = &(attribute_array[next].next_attribute_index);
          next = attribute_array[next].next_attribute_index;
        }
      else
        {
          break;
        }
    }

  if (attr_top >= ATTR_SIZE-1)
    {
      semantic_error (ATTR_OVERFLOW, ABORT, 0, 0, 0, 0);
    }

  ++attr_top;
  attribute_array[attr_top].attribute_number = attribute_number;
  attribute_array[attr_top].attribute_value = attribute_value;
  attribute_array[attr_top].next_attribute_index = next;
  *p = attr_top;
}

char* kind_name[] = {"constant", "variable", "funcforw", "function", "ref_arg",
                     "val_arg", "field", "typedef", "procforw", "procedure",
                     "class", "array"};

void
symtbl_print (void)
{
  int i;
  int attribute_number;
  int attribute_value;
  int attr_ptr;
  int type_attr;
  int tree_value = 0;
  ast* tree_ptr;
  char* s;

  printf ("********************************Symbol Table************************************\n\n");
  printf ("          Name Nest-Level  Tree-Node Predefined        Kind       Type      Value  Offset Dimension   Argnum\n\n");

  for (i = 1; i <= symtbl_top; ++i)
    {
      tree_value = 0;
      printf ("%3d", i);
      for (attribute_number = NAME_ATTR; attribute_number <= ARGNUM_ATTR; ++attribute_number)
        {
          if (attr_ptr = symtbl_is_attribute (i, attribute_number))
            {
              attribute_value = attribute_array[attr_ptr].attribute_value;
              switch (attribute_number)
                {
                case NAME_ATTR:
                  s = string_table->buffer + attribute_value + 1;
                  printf ("%11s", s);
                  break;

                case OFFSET_ATTR:
                case NEST_ATTR:
                case TREE_ATTR:
                  printf ("%11d", attribute_value);
                  break;

                case TYPE_ATTR:
                  tree_value = attribute_value;
                  printf ("%11d", attribute_value);
                  break;

                case PREDE_ATTR:
                  if (attribute_value)
                    {
                      printf ("%11s", "yes");
                    }
                  else
                    {
                      printf ("%11s", "no");
                    }
                  break;

                case KIND_ATTR:
                  printf ("%11s", kind_name[attribute_value - 1]);
                  break;

                case VALUE_ATTR:
                  if (symtbl_get_attribute (i, KIND_ATTR) == CONST)
                    {
                      tree_ptr = (ast*) attribute_value;
                      switch (tree_ptr->node_type)
                        {
                        case NUMNODE:
                          printf ("%11d", ast_get_data (tree_ptr));
                          break;

                        case STRINGNODE:
                          s = string_table->buffer + ast_get_data (tree_ptr) + 1;
                          printf ("%11s", s);
                          break;
                        }
                    }
                  break;

                case DIMEN_ATTR:
                  printf ("%11d", attribute_value);
                  break;

                case ARGNUM_ATTR:
                  printf ("%11d", attribute_value);
                  break;
                }
            }
          else
            {
              printf ("%11s", " ");
            }

        }
      printf ("\n");
    }
}

void
symtbl_push (int is_marker, int name, int symtbl_index, int is_dummy)
{

  if (stack_top >= STACK_SIZE - 1)
    {
      semantic_error (STACK_OVERFLOW, ABORT, 0, 0, 0, 0);
    }

  ++stack_top;

  stack[stack_top].is_marker = is_marker;
  stack[stack_top].name = name;
  stack[stack_top].symtbl_index = symtbl_index;
  stack[stack_top].is_dummy = is_dummy;
  stack[stack_top].used = 0;

}

char*
symtbl_ordinal_abbreviation (int i)
{
  /** @todo This is bad, should not use local array. */
  char s[10];

  switch (i)
    {
    case 0:
      return("0th");

    case 1:
      return("1st");

    case 2:
      return("2nd");

    case 3:
      return("3rd");

    default:
      sprintf (s, "%dth", i);
      return s;
    }
}
