/* Tests for toy-scheme.

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

#include "scheme.h"

int
main ()
{
  object *env = init_env ();
  object *ans = NULL;
  object *sexp = NULL;

  /* 3.14 => 3.14 */
  printf ("> 3.14\n");
  sexp = number ("3.14");
  ans = eval (env, sexp);
  println (ans);
  free_object (sexp);
  free_object (ans);

  /* (+ 1 (- 6 4) (/ 81 (* 3 3 3))) => 6 */
  printf ("> (+ 1 (- 6 4) (/ 81 (* 3 3 3)))\n");
  sexp = cons (func (&fn_add),
               cons (number ("1"),
                     cons (cons (func (&fn_sub),
                                 cons (number ("6"),
                                       cons (number ("4"),
                                             NULL))),
                           (cons (cons (func (&fn_div),
                                        cons (number ("81"),
                                              cons (cons (func (&fn_mul),
                                                          cons (number ("3"),
                                                                cons (number ("3"),
                                                                      cons (number ("3"),
                                                                            NULL)))),
                                                    NULL))),
                                  NULL)))));
  ans = eval (env, sexp);
  println (ans);
  free_object (sexp);
  free_object (ans);

  /* ((lambda (x y) (+ x y)) 1 2) => 3 */
  printf ("> ((lambda (x y) (+ x y)) 1 2)\n");
  sexp = cons (lambda (cons (atom ("x"),
                             cons (atom ("y"),
                                   NULL)),
                       cons (func (&fn_add),
                             cons (atom ("x"),
                                   cons (atom ("y"),
                                         NULL)))),
               cons (number ("1"),
                     cons (number ("2"),
                           NULL)));
  ans = eval (env, sexp);
  println (ans);
  free_object (sexp);
  free_object (ans);

  free_object (env);
  return 0;
}
