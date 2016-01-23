#include "scheme.h"

int
main ()
{
  object *env = init_env ();
  object *ans = NULL;

  /* (+ 1 (* 2 (+ 5 6)) 4) -> 27 */
  object *sexp = cons (func (&fn_add),
                       cons (number ("1"),
                             cons (cons (func (&fn_mul),
                                         cons (number ("2"),
                                               cons (cons (func (&fn_add),
                                                           cons (number ("5"),
                                                                 cons (number ("6"),
                                                                       NULL))),
                                                     NULL))),
                                   cons (number ("4"),
                                         NULL))));

  ans = eval (env, sexp);
  free_object (sexp);
  println (ans);
  free_object (ans);

  free_object (env);
  return 0;
}
