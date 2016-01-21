#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum type { ATOM, NUMBER, PAIR, FUNC, LAMBDA };

enum error { ENOMEM, ETYPE };

typedef struct
{
  enum type type;
} object;

typedef struct
{
  enum type type;
  char *name;
} atom_object;

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

#define car(X) (((pair_object *) X)->car)
#define cdr(X) (((pair_object *) X)->cdr)

void print_error (enum error error);
void print (object *obj);
void println (object *obj);

object *copy_object (object *obj);
void free_object (object *obj);

int null_p (object *obj);
int pair_p (object *obj);
int list_p (object *obj);

object *atom (const char *str);
object *number (const char *str);
object *number_from_double (double x);
object *cons (object *obj1, object *obj2);
object *func (object * (*fn) (object *, object *));
object *lambda (object *args, object *sexp);

#define foreach(ptr, li) for (ptr = li; NULL != ptr; ptr = cdr (ptr))
void append (object **li, object *obj);
size_t length (object *li);

object *init_env ();

object *eval_fn (object *env, object *sexp);
object *eval (object *env, object *sexp);

object *fn_car (object *env, object *args);
object *fn_cdr (object *env, object *args);
object *fn_cons (object *env, object *args);
object *fn_add (object *env, object *args);
object *fn_mul (object *env, object *args);
