#include "scheme.h"

int
main ()
{
  object *env = init_env ();
  object *args = cons (cons (atom ("\"Tom\""), atom ("\"Jerry\"")), NULL);
  println (fn_car (env, args));
  free_object (env);
  free_object (args);
  return 0;
}
