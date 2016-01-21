#include "scheme.h"

int
main ()
{
  object *env = init_env ();
  object *ans = NULL;
  object *sexp = cons (func (&fn_add),
                       (cons (number ("1"),
                              cons (number ("2"),
                                    cons (number ("3"),
                                          NULL)))));

  ans = eval (env, sexp);
  free_object (sexp);
  println (ans);
  free_object (ans);

  free_object (env);
  return 0;
}
