#include "scheme.h"

int
main ()
{
  object *env = init_env ();
  object *ans = NULL;
  object *sexp = cons (func (&fn_cons),
                       (cons (cons (atom ("\"Tom\""),
                                    atom ("\"Jerry\"")),
                              cons (cons (atom ("pi"),
                                          atom ("3.14159265")),
                                    NULL))));

  ans = eval (env, sexp);
  free_object (sexp);
  println (ans);
  free_object (ans);

  free_object (env);
  return 0;
}
