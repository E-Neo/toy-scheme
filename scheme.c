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
  else if (PAIR == obj->type)
    {
      printf ("(");
      print (car (obj));
      printf (" . ");
      print (cdr (obj));
      printf (")");
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
  else
    {
      print_error (ETYPE);
      exit (1);
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
atom (char *str)
{
  object *atom = malloc (sizeof (atom_object));
  if (NULL == atom)
    {
      print_error (ENOMEM);
      exit (1);
    }
  char *name = malloc (sizeof (strlen (str) + 1));
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

object *
init_env ()
{
  object *env = NULL;
  append (&env, cons (cons (atom ("car"), func (&fn_car)), NULL));
  append (&env, cons (cons (atom ("cdr"), func (&fn_cdr)), NULL));
  return env;
}

object *
fn_car (object *env, object *args)
{
  return car (car (args));
}

object *
fn_cdr (object *env, object *args)
{
  return cdr (car (args));
}
