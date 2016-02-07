/* Scheme object manipulation primitives for toy-scheme.

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

#include "object.h"

void
print_fatal (enum error error)
{
  fprintf (stderr, "FATAL: ");
  switch (error)
    {
    case ENOMEM:
      fprintf (stderr, "don't have enough memory.\n");
      break;
    case ETYPE:
      fprintf (stderr, "type error.\n");
      break;
    case ECONFLICT:
      fprintf (stderr, "conflict error.\n");
    default: fprintf (stderr, "unknown error occurred.\n");
    }
}

void
print (object *obj)
{
  if (NULL == obj)
    {
      printf ("()");
      return;
    }

  if (SCM_VOID == obj->type);
  else if (SCM_ERROR == obj->type)
    printf ("ERROR: %s", ((error_object *) obj)->msg);
  else if (SCM_ATOM == obj->type)
    printf ("%s", ((atom_object *) obj)->name);
  else if (SCM_BOOL == obj->type)
    printf ("%s", ((bool_object *) obj)->value ? "#t" : "#f");
  else if (SCM_VARIABLE == obj->type)
    print (((variable_object *) obj)->value);
  else if (SCM_NUMBER == obj->type)
    printf ("%lf", ((number_object *) obj)->num);
  else if (SCM_STRING == obj->type)
    printf ("\"%s\"", ((string_object *) obj)->str);
  else if (SCM_PAIR == obj->type)
    {
      printf ("(");
      print (car (obj));
      printf (" . ");
      print (cdr (obj));
      printf (")");
    }
  else if (SCM_FUNC == obj->type)
    {
      printf ("#<procedure builtin>");
    }
  else if (SCM_LAMBDA == obj->type)
    {
      printf ("#<procedure lambda ");
      print (((lambda_object *) obj)->args);
      printf (">");
    }
  else
    {
      print_fatal (ETYPE);
      exit (1);
    }
}

void
println (object *obj)
{
  print (obj);
  if (NULL != obj && SCM_VOID != obj->type)
    printf ("\n");
}

object *
copy_object (object *obj)
{
  object *ret = NULL;
  if (NULL == obj)
    return NULL;
  else if (SCM_VOID == obj->type)
    ret = SCM_void ();
  else if (SCM_ERROR == obj->type)
    ret = SCM_error (((error_object *) obj)->error,
                     ((error_object *) obj)->msg);
  else if (SCM_ATOM == obj->type)
    ret = atom (((atom_object *) obj)->name);
  else if (SCM_BOOL == obj->type)
    ret = bool (((bool_object *) obj)->value);
  else if (SCM_VARIABLE == obj->type)
    ret = variable (((variable_object *) obj)->name,
                    copy_object (((variable_object *) obj)->value));
  else if (SCM_NUMBER == obj->type)
    ret = number_from_double (((number_object *) obj)->num);
  else if (SCM_STRING == obj->type)
    ret = SCM_string (((string_object *) obj)->str);
  else if (SCM_PAIR == obj->type)
    ret = cons (copy_object (car (obj)), copy_object (cdr (obj)));
  else if (SCM_FUNC == obj->type)
    ret = func (((func_object *) obj)->fn);
  else if (SCM_LAMBDA == obj->type)
    ret = lambda (copy_object (((lambda_object *) obj)->args),
                  copy_object (((lambda_object *) obj)->sexp));
  else
    {
      print_fatal (ETYPE);
      exit (1);
    }

  return ret;
}

void
free_object (object *obj)
{
  if (NULL == obj)
    {
      return;
    }
  else if (SCM_VOID == obj->type)
    free (obj);
  else if (SCM_ERROR == obj->type)
    {
      free (((error_object *) obj)->msg);
      free (obj);
    }
  else if (SCM_ATOM == obj->type)
    {
      free (((atom_object *) obj)->name);
      free (obj);
    }
  else if (SCM_BOOL == obj->type)
    {
      free (obj);
    }
  else if (SCM_VARIABLE == obj->type)
    {
      free (((variable_object *) obj)->name);
      free_object (((variable_object *) obj)->value);
      free (obj);
    }
  else if (SCM_NUMBER == obj->type)
    {
      free (obj);
    }
  else if (SCM_STRING == obj->type)
    {
      free (((string_object *) obj)->str);
      free (obj);
    }
  else if (SCM_PAIR == obj->type)
    {
      free_object (car (obj));
      free_object (cdr (obj));
      free (obj);
    }
  else if (SCM_FUNC == obj->type)
    {
      free (obj);
    }
  else if (SCM_LAMBDA == obj->type)
    {
      free_object (((lambda_object *) obj)->args);
      free_object (((lambda_object *) obj)->sexp);
      free (obj);
    }
  else
    {
      print_fatal (ETYPE);
      exit (1);
    }
}

int
atom_replace (object **obj, object *old, object *new)
{
  int count = 0;
  if (NULL == *obj) return 0;
  else if (SCM_ATOM == (*obj)->type)
    {
      if (!strcmp (((atom_object *) *obj)->name,
                   ((atom_object *) old)->name))
        {
          free_object (*obj);
          *obj = copy_object (new);
          count++;
          return count;
        }
      else return 0;
    }
  else if (SCM_PAIR == (*obj)->type)
    {
      count += atom_replace (&car (*obj), old, new);
      count += atom_replace (&cdr (*obj), old, new);
      return count;
    }
  else return 0;
}

int
null_p (object *obj)
{
  return NULL == obj ? 1 : 0;
}

int
pair_p (object *obj)
{
  return NULL == obj || SCM_PAIR != obj->type ? 0 : 1;
}

int
list_p (object *obj)
{
  if (NULL == obj) return 1;

  object *tmp;
  for (tmp = obj; NULL != tmp && SCM_PAIR == tmp->type; tmp = cdr (tmp));
  if (NULL == tmp) return 1;

  return 0;
}

int
atom_eqv_p (object *obj1, object *obj2)
{
  return !strcmp (((atom_object *) obj1)->name,
                  ((atom_object *) obj2)->name);
}

int
varible_eqv_p (object *obj1, object *obj2)
{
  if (!strcpy (((variable_object *) obj1)->name,
               ((variable_object *) obj2)->name))
    {
      if (eqv_p (((variable_object *) obj1)->value,
                 ((variable_object *) obj2)->value))
        {
          return 1;
        }
      else
        {
          print_fatal (ECONFLICT);
          exit (1);
        }
    }
  else
    return 0;
}

int
number_eqv_p (object *obj1, object *obj2)
{
  return ((number_object *) obj1)->num == ((number_object *) obj2)->num ?
    1 : 0;
}

int
pair_eqv_p (object *obj1, object *obj2)
{
  return obj1 == obj2 ? 1 : 0;
}

int
func_eqv_p (object *obj1, object *obj2)
{
  return ((func_object *) obj1)->fn == ((func_object *) obj2)->fn ?
    1 : 0;
}

int
lambda_eqv_p (object *obj1, object *obj2)
{
  return obj1 == obj2 ? 1 : 0;
}

int
eqv_p (object *obj1, object *obj2)
{
  if (obj1 == obj2) return 1;
  if (NULL == obj1 || NULL == obj2) return 0;
  if (obj1->type != obj2->type) return 0;

  if (SCM_ATOM == obj1->type) return atom_eqv_p (obj1, obj2);
  else if (SCM_VARIABLE == obj1->type) return varible_eqv_p (obj1, obj2);
  else if (SCM_NUMBER == obj1->type) return number_eqv_p (obj1, obj2);
  else if (SCM_PAIR == obj1->type) return pair_eqv_p (obj1, obj2);
  else if (SCM_FUNC == obj1->type) return func_eqv_p (obj1, obj2);
  else if (SCM_LAMBDA == obj1->type) return lambda_eqv_p (obj1, obj2);
  else
    {
      print_fatal (ETYPE);
      exit (1);
    }
}

object *
SCM_void ()
{
  object *ptr = malloc (sizeof (void_object));
  if (NULL == ptr)
    {
      print_fatal (ENOMEM);
      exit (1);
    }
  ptr->type = SCM_VOID;
  return ptr;
}

object *
SCM_error (enum error x, const char *str)
{
  error_object *ptr = malloc (sizeof (error_object));
  if (NULL == ptr)
    {
      print_fatal (ENOMEM);
      exit (1);
    }
  char *msg = malloc (strlen (str) + 1);
  if (NULL == msg)
    {
      print_fatal (ENOMEM);
      exit (1);
    }
  ptr->type = SCM_ERROR;
  ptr->error = x;
  strcpy (msg, str);
  ptr->msg = msg;
  return (object *) ptr;
}

object *
atom (const char *str)
{
  object *atom = malloc (sizeof (atom_object));
  if (NULL == atom)
    {
      print_fatal (ENOMEM);
      exit (1);
    }
  char *name = malloc (strlen (str) + 1);
  if (NULL == name)
    {
      print_fatal (ENOMEM);
      exit (1);
    }
  strcpy (name, str);
  atom->type = SCM_ATOM;
  ((atom_object *) atom)->name = name;
  return atom;
}

object *
bool (char x)
{
  bool_object *ptr = malloc (sizeof (bool_object));
  if (NULL == ptr)
    {
      print_fatal (ENOMEM);
      exit (1);
    }
  ptr->type = SCM_BOOL;
  ptr->value = x ? 1 : 0;
  return (object *) ptr;
}

object *
variable (const char *name, object *value)
{
  variable_object *ptr = malloc (sizeof (variable_object));
  if (NULL == ptr)
    {
      print_fatal (ENOMEM);
      exit (1);
    }
  char *str = malloc (strlen (name) + 1);
  if (NULL == str)
    {
      print_fatal (ENOMEM);
      exit (1);
    }
  strcpy (str, name);
  ptr->type = SCM_VARIABLE;
  ptr->name = str;
  ptr->value = value;
  return (object *) ptr;
}

object *
number (const char *str)
{
  number_object *ptr = malloc (sizeof (number_object));
  if (NULL == ptr)
    {
      print_fatal (ENOMEM);
      exit (1);
    }
  ptr->type = SCM_NUMBER;
  ptr->num = atof (str);
  return (object *) ptr;
}

object *
number_from_double (double x)
{
  number_object *ptr = malloc (sizeof (number_object));
  if (NULL == ptr)
    {
      print_fatal (ENOMEM);
      exit (1);
    }
  ptr->type = SCM_NUMBER;
  ptr->num = x;
  return (object *) ptr;
}

object *
SCM_string (const char *str)
{
  string_object *ptr = malloc (sizeof (string_object));
  if (NULL == ptr)
    {
      print_fatal (ENOMEM);
      exit (1);
    }
  char *tmp = malloc (strlen (str) + 1);
  if (NULL == tmp)
    {
      print_fatal (ENOMEM);
      exit (1);
    }
  strcpy (tmp, str);
  ptr->type = SCM_STRING;
  ptr->str = tmp;
  return (object *) ptr;
}

object *
cons (object *obj1, object *obj2)
{
  object *pair = malloc (sizeof (pair_object));
  if (NULL == pair)
    {
      print_fatal (ENOMEM);
      exit (1);
    }
  pair->type = SCM_PAIR;
  car (pair) = obj1;
  cdr (pair) = obj2;
  return pair;
}

object *
func (object * (*fn) (object **, object *))
{
  func_object *ptr = malloc (sizeof (func_object));
  if (NULL == ptr)
    {
      print_fatal (ENOMEM);
      exit (1);
    }
  ptr->type = SCM_FUNC;
  ptr->fn = fn;
  return (object *) ptr;
}

object *
lambda (object *args, object *sexp)
{
  lambda_object *ptr = malloc (sizeof (lambda_object));
  if (NULL == ptr)
    {
      print_fatal (ENOMEM);
      exit (1);
    }
  ptr->type = SCM_LAMBDA;
  ptr->args = args;
  ptr->sexp = sexp;
  return (object *) ptr;
}

void
append (object **li, object *obj)
{
  if (!list_p (*li))
    {
      print_fatal (ETYPE);
      exit (1);
    }

  if (null_p (*li))
    {
      *li = obj;
      return;
    }

  object *tmp;
  for (tmp = *li; NULL != cdr (tmp); tmp = cdr (tmp));
  cdr (tmp) = obj;
}

size_t
length (object *li)
{
  object *tmp;
  size_t len;
  for (tmp = li, len = 0; NULL != tmp; tmp = cdr (tmp), len++);
  return len;
}

object *
init_env ()
{
  object *env = NULL;
  /* Warning: Using append here will waste time.  */
  append (&env, cons (variable ("define", func (&fn_define)), NULL));
  append (&env, cons (variable ("cons", func (&fn_cons)), NULL));
  append (&env, cons (variable ("car", func (&fn_car)), NULL));
  append (&env, cons (variable ("cdr", func (&fn_cdr)), NULL));
  append (&env, cons (variable ("+", func (&fn_add)), NULL));
  append (&env, cons (variable ("*", func (&fn_mul)), NULL));
  append (&env, cons (variable ("-", func (&fn_sub)), NULL));
  append (&env, cons (variable ("/", func (&fn_div)), NULL));
  append (&env, cons (variable ("<", func (&fn_lt)), NULL));
  append (&env, cons (variable (">", func (&fn_gt)), NULL));
  append (&env, cons (variable ("if", func (&fn_if)), NULL));
  return env;
}

object *
env_search (object **env, object *at)
{
  object *i;
  foreach (i, *env)
    {
      if (!strcmp (((atom_object *) at)->name,
                   ((variable_object *) car (i))->name))
        {
          return copy_object (((variable_object *) car (i))->value);
        }
    }
  char *msg = malloc (18 + strlen (((atom_object *) at)->name) + 1);
  sprintf (msg, "Unbound variable: %s", ((atom_object *) at)->name);
  object *ret = SCM_error (EUNBOUND, msg);
  free (msg);
  return ret;
}

void
env_append (object **env, object *var)
{
  object *i;
  foreach (i, *env)
    {
      if (!strcmp (((variable_object *) car (i))->name,
                   ((variable_object *) var)->name))
        {
          free_object (car (i));
          car (i) = var;
          return;
        }
    }
  append (env, cons (var, NULL));
}

object *
eval_fn (object **env, object *sexp)
{
  object *symbol = car (sexp);
  object *args = cdr (sexp);

  return (((func_object *) symbol)->fn) (env, args);
}

object *
eval_lambda (object **env, object *sexp)
{
  object *lamb = car (sexp);
  object *args = cdr (sexp);
  if (length (((lambda_object *) lamb)->args) !=
      length (args))
    return SCM_error (EARGNUM,
                      "Wrong number of arguments to #<procedure lambda>.");
  object *obj = copy_object (((lambda_object *) lamb)->sexp);
  object *i, *j;
  object *ret;
  for (i = ((lambda_object *) lamb)->args, j = args;
       NULL != i;
       i = cdr (i), j = cdr (j))
    {
      atom_replace (&obj, car (i), car (j));
    }
  ret = eval (env, obj);
  free_object (obj);
  return ret;
}

object *
eval_pair (object **env, object *sexp)
{
  if (SCM_ATOM == (car (sexp))->type)
    {
      object *tmp = env_search (env, car (sexp));
      object *new_sexp = cons (tmp, cdr (sexp));
      tmp = eval (env, new_sexp);
      free_object (car (new_sexp));
      free (new_sexp);
      return tmp;
    }
  else if (SCM_FUNC == (car (sexp))->type)
    return eval_fn (env, sexp);
  else if (SCM_LAMBDA == (car (sexp))->type)
    return eval_lambda (env, sexp);
  else
    return SCM_error (ETYPE, "Wrong type to apply.");
}

object *
eval (object **env, object *sexp)
{
  if (NULL == sexp)
    return NULL;
  else if (SCM_ERROR == sexp->type)
    {
      println (sexp);
      return copy_object (sexp);
    }
  else if (SCM_ATOM == sexp->type)
    return copy_object (sexp);
  else if (SCM_BOOL == sexp->type)
    return copy_object (sexp);
  else if (SCM_VARIABLE == sexp->type)
    return eval (env, ((variable_object *) sexp)->value);
  else if (SCM_NUMBER == sexp->type)
    return copy_object (sexp);
  else if (SCM_STRING == sexp->type)
    return copy_object (sexp);
  else if (SCM_PAIR == sexp->type)
    return eval_pair (env, sexp);
  else if (SCM_FUNC == sexp->type)
    return copy_object (sexp);
  else if (SCM_LAMBDA == sexp->type)
    return copy_object (sexp);
  else
    {
      print_fatal (ETYPE);
      exit (1);
    }
}

/* Builtin functions.  */

object *
fn_define (object **env, object *args)
{
  object *ret;
  if (2 != length (args))
    {
      ret = SCM_error (EARGNUM,
                       "Wrong number of arguments to #<procedure define>.");
      println (ret);
      return ret;
    }
  if (SCM_ATOM != (car (args))->type)
    {
      ret = SCM_error (ETYPE,
                       "Wrong type argument in position 1.");
      println (ret);
      return ret;
    }
  object *value;
  if (SCM_ATOM == (car (cdr (args)))->type)
    {
      value = env_search (env, car (cdr (args)));
      if (SCM_ERROR == value->type)
        {
          println (value);
          return value;
        }
      env_append (env, variable (((atom_object *) car (args))->name, value));
    }
  else
    {
      value = eval (env, car (cdr (args)));
      if (SCM_ERROR == value->type)
        {
          println (value);
          return value;
        }
      env_append (env, variable (((atom_object *) car (args))->name, value));
    }
  return SCM_void ();
}

object *
fn_cons (object **env, object *args)
{
  object *ret;
  if (2 != length (args))
    {
      ret = SCM_error (EARGNUM,
                       "Wrong number of arguments to #<procedure cons>.");
      println (ret);
      return ret;
    }
  object *obj1 = eval (env, car (args));
  if (SCM_ERROR == obj1->type)
    {
      println (obj1);
      return obj1;
    }
  object *obj2 = eval (env, car (cdr (args)));
  if (SCM_ERROR == obj2->type)
    {
      println (obj2);
      return obj2;
    }
  return cons (obj1, obj2);
}

object *
fn_car (object **env, object *args)
{
  object *ret;
  if (1 != length (args))
    {
      ret = SCM_error (EARGNUM,
                       "Wrong number of arguments to #<procedure car>.");
      println (ret);
      return ret;
    }
  if (SCM_PAIR != (car (args))->type)
    {
      ret = SCM_error (ETYPE,
                       "Wrong type argument in position 1.");
      println (ret);
      return ret;
    }
  return copy_object (car (car (args)));
}

object *
fn_cdr (object **env, object *args)
{
  object *ret;
  if (1 != length (args))
    {
      ret = SCM_error (EARGNUM,
                       "Wrong number of arguments to #<procedure cdr>.");
      println (ret);
      return ret;
    }
  if (SCM_PAIR != (car (args))->type)
    {
      ret = SCM_error (ETYPE,
                       "Wrong type argument in position 1.");
      println (ret);
      return ret;
    }
  return copy_object (cdr (car (args)));
}

object *
fn_add (object **env, object *args)
{
  object *ret;
  object *tmp;
  object *i;
  double res = 0.0;
  foreach (i, args)
    {
      tmp = eval (env, car (i));
      if (SCM_NUMBER != tmp->type)
        {
          free_object (tmp);
          ret = SCM_error (ETYPE,
                           "In procedure +: Wrong type.");
          println (ret);
          return ret;
        }
      res += ((number_object *) tmp)->num;
      free_object (tmp);
    }
  return number_from_double (res);
}

object *
fn_mul (object **env, object *args)
{
  object *ret;
  object *tmp;
  object *i;
  double res = 1.0;
  foreach (i, args)
    {
      tmp = eval (env, car (i));
      if (SCM_NUMBER != tmp->type)
        {
          free_object (tmp);
          ret = SCM_error (ETYPE,
                           "In procedure *: Wrong type.");
          println (ret);
          return ret;
        }
      res *= ((number_object *) tmp)->num;
      free_object (tmp);
    }
  return number_from_double (res);
}

object *
fn_sub (object **env, object *args)
{
  object *ret;
  object *tmp;
  object *i;
  double res;
  if (0 == length (args))
    {
      ret = SCM_error (EARGNUM,
                       "Wrong number of arguments to #<procedure ->.");
      println (ret);
      return ret;
    }
  else
    {
      tmp = eval (env, car (args));
      if (SCM_NUMBER != tmp->type)
        {
          free_object (tmp);
          ret = SCM_error (ETYPE,
                           "In procedure -: Wrong type.");
          println (ret);
          return ret;
        }
      res = ((number_object *) tmp)->num;
      free_object (tmp);
      if (1 == length (args))
        return number_from_double (-res);
      else
        {
          foreach (i, cdr (args))
            {
              tmp = eval (env, car (i));
              if (SCM_NUMBER != tmp->type)
                {
                  free_object (tmp);
                  ret = SCM_error (ETYPE,
                                   "In procedure -: Wrong type.");
                  println (ret);
                  return ret;
                }
              res -= ((number_object *) tmp)->num;
              free_object (tmp);
            }
          return number_from_double (res);
        }
    }
}

object *
fn_div (object **env, object *args)
{
  object *ret;
  object *tmp;
  object *i;
  double res;
  if (0 == length (args))
    {
      ret = SCM_error (EARGNUM,
                       "Wrong number of arguments to #<procedure />.");
      println (ret);
      return ret;
    }
  else
    {
      tmp = eval (env, car (args));
      if (SCM_NUMBER != tmp->type)
        {
          free_object (tmp);
          ret = SCM_error (ETYPE,
                           "In procedure /: Wrong type.");
          println (ret);
          return ret;
        }
      res = ((number_object *) tmp)->num;
      free_object (tmp);
      if (1 == length (args))
        return number_from_double (1/res);
      else
        {
          foreach (i, cdr (args))
            {
              tmp = eval (env, car (i));
              if (SCM_NUMBER != tmp->type)
                {
                  free_object (tmp);
                  ret = SCM_error (ETYPE,
                                   "In procedure /: Wrong type.");
                  println (ret);
                  return ret;
                }
              res /= ((number_object *) tmp)->num;
              free_object (tmp);
            }
          return number_from_double (res);
        }
    }
}

object *
fn_lt (object **env, object *args)
{
  object *ret;
  object *tmp;
  object *i;
  double res;
  if (0 == length (args))
    return bool (1);
  else
    {
      tmp = eval (env, car (args));
      if (SCM_NUMBER != tmp->type)
        {
          free_object (tmp);
          ret = SCM_error (ETYPE,
                           "In procedure <: Wrong type.");
          println (ret);
          return ret;
        }
      res = ((number_object *) tmp)->num;
      free_object (tmp);
      if (1 == length (args))
        return bool (1);
      else
        {
          foreach (i, cdr (args))
            {
              tmp = eval (env, car (i));
              if (SCM_NUMBER != tmp->type)
                {
                  free_object (tmp);
                  ret = SCM_error (ETYPE,
                                   "In procedure <: Wrong type.");
                  println (ret);
                  return ret;
                }
              if (res > ((number_object *) tmp)->num)
                {
                  free_object (tmp);
                  return bool (0);
                }
              res = ((number_object *) tmp)->num;
              free_object (tmp);
            }
        }
    }
  return bool (1);
}
object *
fn_gt (object **env, object *args)
{
  object *ret;
  object *tmp;
  object *i;
  double res;
  if (0 == length (args))
    return bool (1);
  else
    {
      tmp = eval (env, car (args));
      if (SCM_NUMBER != tmp->type)
        {
          free_object (tmp);
          ret = SCM_error (ETYPE,
                           "In procedure >: Wrong type.");
          println (ret);
          return ret;
        }
      res = ((number_object *) tmp)->num;
      free_object (tmp);
      if (1 == length (args))
        return bool (1);
      else
        {
          foreach (i, cdr (args))
            {
              tmp = eval (env, car (i));
              if (SCM_NUMBER != tmp->type)
                {
                  free_object (tmp);
                  ret = SCM_error (ETYPE,
                                   "In procedure >: Wrong type.");
                  println (ret);
                  return ret;
                }
              if (res < ((number_object *) tmp)->num)
                {
                  free_object (tmp);
                  return bool (0);
                }
              res = ((number_object *) tmp)->num;
              free_object (tmp);
            }
        }
    }
  return bool (1);
}

object *
fn_if (object **env, object *args)
{
  object *ret;
  object *tmp;
  if (3 != length (args))
    {
      ret = SCM_error (EARGNUM,
                       "Wrong number of arguments to #<procedure if>.");
      println (ret);
      return ret;
    }
  tmp = eval (env, car (args));
  if (NULL == tmp || SCM_BOOL != tmp->type || ((bool_object *) tmp)->value)
    {
      free_object (tmp);
      return eval (env, car (cdr (args)));
    }
  else
    {
      free_object (tmp);
      return eval (env, car (cdr (cdr (args))));
    }
}
