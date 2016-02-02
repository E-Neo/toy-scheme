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
  ans = eval (&env, sexp);
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
  ans = eval (&env, sexp);
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
  ans = eval (&env, sexp);
  println (ans);
  free_object (sexp);
  free_object (ans);

  /* (cons 1 2) => (1 . 2) */
  printf ("> (cons 1 2)\n");
  sexp = cons (func (&fn_cons),
               cons (number ("1"),
                     cons (number ("2"),
                           NULL)));
  ans = eval (&env, sexp);
  println (ans);
  free_object (sexp);
  free_object (ans);

  /* (define square (lambda (x) (* x x))) */
  printf ("> (define square (lambda (x) (* x x)))\n");
  sexp = cons (func (&fn_define),
               cons (atom ("square"),
                     cons (lambda (cons (atom ("x"),
                                         NULL),
                                   cons (func (&fn_mul),
                                         cons (atom ("x"),
                                               cons (atom ("x"),
                                                     NULL)))),
                           NULL)));
  ans = eval (&env, sexp);
  println (ans);
  free_object (sexp);
  free_object (ans);

  /* (square 8) => 64 */
  printf ("> (square 8)\n");
  sexp = cons (atom ("square"),
               cons (number ("8"),
                     NULL));
  ans = eval (&env, sexp);
  println (ans);
  free_object (sexp);
  free_object (ans);

  /* (define new_square square) */
  printf ("> (define new_square square)\n");
  sexp = cons (func (&fn_define),
               cons (atom ("new_square"),
                     cons (atom ("square"),
                           NULL)));
  ans = eval (&env, sexp);
  println (ans);
  free_object (sexp);
  free_object (ans);

  /* (define new_square (lambda (x) (square x))) */
  printf ("> (define new_square (lambda (x) (square x)))\n");
  sexp = cons (func (&fn_define),
               cons (atom ("new_square"),
                     cons (lambda (cons (atom ("x"),
                                         NULL),
                                   cons (atom ("square"),
                                         cons (atom ("x"),
                                               NULL))),
                           NULL)));
  ans = eval (&env, sexp);
  println (ans);
  free_object (sexp);
  free_object (ans);

  /* (define factorial (lambda (n) (if (< n 1) 1 (* n (factorial (- n 1)))))) */
  printf ("> (define factorial (lambda (n) (if (< n 1) 1 (* n (factorial (- n 1))))))\n");
  sexp = cons (func (&fn_define),
               cons (atom ("factorial"),
                     cons (lambda (cons (atom ("n"),
                                         NULL),
                                   cons (func (&fn_if),
                                         cons (cons (func (&fn_lt),
                                                     cons (atom ("n"),
                                                           cons (number ("1"),
                                                                 NULL))),
                                               cons (number ("1"),
                                                     cons (cons (func (&fn_mul),
                                                                 cons (atom ("n"),
                                                                       cons (cons (atom ("factorial"),
                                                                                   cons (cons (func (&fn_sub),
                                                                                               cons (atom ("n"),
                                                                                                     cons (number ("1"),
                                                                                                           NULL))),
                                                                                         NULL)),
                                                                             NULL))),
                                                           NULL))))),
                           NULL)));
  ans = eval (&env, sexp);
  println (ans);
  free_object (sexp);
  free_object (ans);

  /* (factorial 3) => 6
  printf ("> (factorial 3)\n");
  sexp = cons (atom ("factorial"),
               cons (number ("3"),
                     NULL));
  ans = eval (&env, sexp);
  println (ans);
  free_object (sexp);
  free_object (ans);*/

  /* (new_square 9) => 81 */
  printf ("> (new_square 9)\n");
  sexp = cons (atom ("new_square"),
               cons (number ("9"),
                     NULL));
  ans = eval (&env, sexp);
  println (ans);
  free_object (sexp);
  free_object (ans);

  /* ((lambda (x) (if (< x 0) (- x) x)) -10) => 10 */
  printf ("> ((lambda (x) (if (< x 0) (- x) x)) -10)\n");
  sexp = cons (lambda (cons (atom ("x"),
                             NULL),
                       cons (func (&fn_if),
                             cons (cons (func (&fn_lt),
                                         cons (atom ("x"),
                                               cons (number ("0"),
                                                     NULL))),
                                   cons (cons (func (&fn_sub),
                                               cons (atom ("x"),
                                                     NULL)),
                                         cons (atom ("x"),
                                               NULL))))),
               cons (number ("-10"),
                     NULL));
  ans = eval (&env, sexp);
  println (ans);
  free_object (sexp);
  free_object (ans);

  /* (define x unbound) */
  printf ("> (define x unbound)\n");
  sexp = cons (func (&fn_define),
               cons (atom ("x"),
                     cons (atom ("unbound"),
                           NULL)));
  ans = eval (&env, sexp);
  println (ans);
  free_object (sexp);
  free_object (ans);

  object *i;
  printf ("Environment list:\n");
  foreach (i, env)
    {
      printf ("%s: ", ((variable_object *) car (i))->name);
      println (((variable_object *) car (i))->value);
    }

  free_object (env);
  return 0;
}
