/* Header file for the scheme object manipulation primitives.

Copyright (C) 2016 E-Neo

This file is part of toy-scheme.

toy-scheme is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

toy-scheme is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with toy-scheme.  If not, see <http://www.gnu.org/licenses/>.  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Enumeration of object type.  */

enum type
  {
    ATOM, /* Note: This should be removed in the future.  */
    VARIABLE,
    NUMBER,
    PAIR,
    FUNC,
    LAMBDA
  };

/* Enumeration of error type.  */

enum error
  {
    ENOMEM,
    ETYPE
  };

/* Structure for a scheme object.  */

typedef struct
{
  enum type type;
} object;

/* Structures for different scheme objects.
   The previous object is supertype
   and the following objects are subtypes.   */

typedef struct
{
  enum type type;
  char *name;
} atom_object;

typedef struct
{
  enum type type;
  char *name;
  object *value;
} variable_object;

typedef struct
{
  enum type type;
  double num;
} number_object;

typedef struct
{
  enum type type;
  object *car;
  object *cdr;
} pair_object;

typedef struct
{
  enum type type;
  object * (*fn) (object *, object *);
} func_object;

typedef struct
{
  enum type type;
  object *args;
  object *sexp;
} lambda_object;

/* Macros to extract a pair.  */

#define car(X) (((pair_object *) X)->car)
#define cdr(X) (((pair_object *) X)->cdr)

/* Print errors to stderr and print objects to stdout.  */

void print_error (enum error error);
void print (object *obj);
void println (object *obj);

/* Functions to manipulate objects: copy, free...  */

object *copy_object (object *obj);
void free_object (object *obj);
void free_li_with (object *li_with);

/* Return 0 or 1.  */

int null_p (object *obj);
int pair_p (object *obj);
int list_p (object *obj);

/* Functions to create objects.

   Warning: The object parameters will be assigned directly without copying.
   Sometimes you should copy the objects before passing them to these functions
   to avoid troubles.  */

object *atom (const char *str);
object *variable (const char *name, object *value);
object *number (const char *str);
object *number_from_double (double x);
object *cons (object *obj1, object *obj2);
object *func (object * (*fn) (object *, object *));
object *lambda (object *args, object *sexp);

/* Some low level functions to manipulate list object.  */

#define foreach(ptr, li) for (ptr = li; NULL != ptr; ptr = cdr (ptr))
void append (object **li, object *obj);
void replace (object **li, object *li_with);
size_t length (object *li);

/* Functions to manipulate scheme environment.  */

object *init_env ();

/* Evaluate s-expressions.  */

object *eval_fn (object *env, object *sexp);
object *eval_lambda (object *env, object *sexp);
object *eval (object *env, object *sexp);

/* Builtin functions.  */

object *fn_car (object *env, object *args);
object *fn_cdr (object *env, object *args);
object *fn_cons (object *env, object *args);
object *fn_add (object *env, object *args);
object *fn_mul (object *env, object *args);
object *fn_sub (object *env, object *args);
object *fn_div (object *env, object *args);
