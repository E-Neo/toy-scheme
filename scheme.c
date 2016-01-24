#include "scheme.h"

void
print_error (enum error error)
{
  fprintf (stderr, "ERROR: ");
  switch (error)
    {
    case ENOMEM:
      fprintf (stderr, "don't have enough memory.\n");
      break;
    case ETYPE:
      fprintf (stderr, "type error.\n");
      break;
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

  if (ATOM == obj->type)
    printf ("%s", ((atom_object *) obj)->name);
  else if (NUMBER == obj->type)
    printf ("%lf", ((number_object *) obj)->num);
  else if (PAIR == obj->type)
    {
      printf ("(");
      print (car (obj));
      printf (" . ");
      print (cdr (obj));
      printf (")");
    }
  else if (FUNC == obj->type)
    {
      printf ("#<procedure builtin>");
    }
  else if (LAMBDA == obj->type)
    {
      printf ("#<procedure lambda ");
      print (((lambda_object *) obj)->args);
      printf (">");
    }
  else
    {
      print_error (ETYPE);
      exit (1);
    }
}

void
println (object *obj)
{
  print (obj);
  printf ("\n");
}

object *
copy_object (object *obj)
{
  object *ret = NULL;
  if (NULL == obj)
    return NULL;
  else if (ATOM == obj->type)
    ret = atom (((atom_object *) obj)->name);
  else if (NUMBER == obj->type)
    ret = number_from_double (((number_object *) obj)->num);
  else if (PAIR == obj->type)
    ret = cons (copy_object (car (obj)), copy_object (cdr (obj)));
  else if (FUNC == obj->type)
    ret = func (((func_object *) obj)->fn);
  else if (LAMBDA == obj->type)
    ret = lambda (copy_object (((lambda_object *) obj)->args),
                  copy_object (((lambda_object *) obj)->sexp));
  else
    {
      print_error (ETYPE);
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
  else if (ATOM == obj->type)
    {
      free (((atom_object *) obj)->name);
      free (obj);
    }
  else if (NUMBER == obj->type)
    {
      free (obj);
    }
  else if (PAIR == obj->type)
    {
      free_object (car (obj));
      free_object (cdr (obj));
      free (obj);
    }
  else if (FUNC == obj->type)
    {
      free (obj);
    }
  else if (LAMBDA == obj->type)
    {
      free_object (((lambda_object *) obj)->args);
      free_object (((lambda_object *) obj)->sexp);
      free (obj);
    }
  else
    {
      print_error (ETYPE);
      exit (1);
    }
}

void
free_li_with (object *li_with)
{
  object *i, *tmp;
  for (i = li_with; NULL != i; i = tmp)
    {
      tmp = cdr (i);
      free (car (i));
      free (i);
    }
}

int
null_p (object *obj)
{
  return NULL == obj ? 1 : 0;
}

int
pair_p (object *obj)
{
  return NULL == obj || PAIR != obj->type ? 0 : 1;
}

int
list_p (object *obj)
{
  if (NULL == obj) return 1;

  object *tmp;
  for (tmp = obj; NULL != tmp && PAIR == tmp->type; tmp = cdr (tmp));
  if (NULL == tmp) return 1;

  return 0;
}

object *
atom (const char *str)
{
  object *atom = malloc (sizeof (atom_object));
  if (NULL == atom)
    {
      print_error (ENOMEM);
      exit (1);
    }
  char *name = malloc (strlen (str) + 1);
  if (NULL == name)
    {
      print_error (ENOMEM);
      exit (1);
    }
  strcpy (name, str);
  atom->type = ATOM;
  ((atom_object *) atom)->name = name;
  return atom;
}

object *
number (const char *str)
{
  number_object *ptr = malloc (sizeof (number_object));
  if (NULL == ptr)
    {
      print_error (ENOMEM);
      exit (1);
    }
  ptr->type = NUMBER;
  ptr->num = atof (str);
  return (object *) ptr;
}

object *
number_from_double (double x)
{
  number_object *ptr = malloc (sizeof (number_object));
  if (NULL == ptr)
    {
      print_error (ENOMEM);
      exit (1);
    }
  ptr->type = NUMBER;
  ptr->num = x;
  return (object *) ptr;
}

object *
cons (object *obj1, object *obj2)
{
  object *pair = malloc (sizeof (pair_object));
  if (NULL == pair)
    {
      print_error (ENOMEM);
      exit (1);
    }
  pair->type = PAIR;
  car (pair) = obj1;
  cdr (pair) = obj2;
  return pair;
}

object *
func (object * (*fn) (object *, object *))
{
  func_object *ptr = malloc (sizeof (func_object));
  if (NULL == ptr)
    {
      print_error (ENOMEM);
      exit (1);
    }
  ptr->type = FUNC;
  ptr->fn = fn;
  return (object *) ptr;
}

object *
lambda (object *args, object *sexp)
{
  lambda_object *ptr = malloc (sizeof (lambda_object));
  if (NULL == ptr)
    {
      print_error (ENOMEM);
      exit (1);
    }
  ptr->type = LAMBDA;
  ptr->args = args;
  ptr->sexp = sexp;
  return (object *) ptr;
}

void
append (object **li, object *obj)
{
  if (!list_p (*li))
    {
      print_error (ETYPE);
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

void
replace (object **li, object *li_with)
{
  object *i, *j;
  foreach (i, *li)
    {
      foreach (j, li_with)
        {
          if (car (car (j)) == car (i))
            {
              free_object (car (i));
              car (i) = cdr (car (j));
            }
        }
    }
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
  append (&env, cons (cons (atom ("car"), func (&fn_car)), NULL));
  append (&env, cons (cons (atom ("cdr"), func (&fn_cdr)), NULL));
  return env;
}

object *
eval_fn (object *env, object *sexp)
{
  object *symbol = car (sexp);
  object *args = cdr (sexp);

  return (((func_object *) symbol)->fn) (env, args);
}

object *
eval (object *env, object *sexp)
{
  if (NULL == sexp) return NULL;

  if (PAIR == sexp->type)
    {
      object *new_sexp = NULL;
      object *tmp;
      foreach (tmp, sexp)
        {
          append (&new_sexp, cons (eval (env, car (tmp)), NULL));
        }
      tmp = eval_fn (env, new_sexp);
      free_object (new_sexp);
      return tmp;
    }
  else
    return copy_object (sexp);
}

object *
fn_car (object *env, object *args)
{
  return copy_object (car (car (args)));
}

object *
fn_cdr (object *env, object *args)
{
  return copy_object (cdr (car (args)));
}

object *
fn_cons (object *env, object *args)
{
  object *obj1 = car (args);
  object *obj2 = car (cdr (args));
  return cons (copy_object (obj1), copy_object (obj2));
}

object *
fn_add (object *env, object *args)
{
  double res = 0.0;
  object *tmp;
  foreach (tmp, args)
    {
      res += ((number_object *) car (tmp))->num;
    }
  return number_from_double (res);
}

object *
fn_mul (object *env, object *args)
{
  double res = 1.0;
  object *tmp;
  foreach (tmp, args)
    {
      res *= ((number_object *) car (tmp))->num;
    }
  return number_from_double (res);
}

object *
fn_sub (object *env, object *args)
{
  double res = ((number_object *) car (args))->num;
  object *tmp;
  if (1 == length (args))
    return number_from_double (-res);
  else
    foreach (tmp, cdr (args))
      {
        res -= ((number_object *) car (tmp))->num;
      }
  return number_from_double (res);
}

object *
fn_div (object *env, object *args)
{
  double res = ((number_object *) car (args))->num;
  object *tmp;
  if (1 == length (args))
    return number_from_double (1 / res);
  else
    foreach (tmp, cdr (args))
      {
        res /= ((number_object *) car (tmp))->num;
      }
  return number_from_double (res);
}
