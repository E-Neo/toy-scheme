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
  sexp = SCM_number ("3.14");
  ans = eval (&env, sexp);
  println (ans);
  free_object (sexp);
  free_object (ans);

  /* (+ 1 (- 6 4) (/ 81 (* 3 3 3))) => 6 */
  printf ("> (+ 1 (- 6 4) (/ 81 (* 3 3 3)))\n");
  sexp = SCM_cons (SCM_func (&fn_add),
                   SCM_cons (SCM_number ("1"),
                             SCM_cons (SCM_cons (SCM_func (&fn_sub),
                                                 SCM_cons (SCM_number ("6"),
                                                           SCM_cons (SCM_number ("4"),
                                                                     NULL))),
                                       (SCM_cons (SCM_cons (SCM_func (&fn_div),
                                                            SCM_cons (SCM_number ("81"),
                                                                      SCM_cons (SCM_cons (SCM_func (&fn_mul),
                                                                                          SCM_cons (SCM_number ("3"),
                                                                                                    SCM_cons (SCM_number ("3"),
                                                                                                              SCM_cons (SCM_number ("3"),
                                                                                                                        NULL)))),
                                                                                NULL))),
                                                  NULL)))));
  ans = eval (&env, sexp);
  println (ans);
  free_object (sexp);
  free_object (ans);

  /* ((lambda (x y) (+ x y)) 1 2) => 3 */
  printf ("> ((lambda (x y) (+ x y)) 1 2)\n");
  sexp = SCM_cons (SCM_lambda (SCM_cons (SCM_atom ("x"),
                                         SCM_cons (SCM_atom ("y"),
                                                   NULL)),
                               SCM_cons (SCM_func (&fn_add),
                                         SCM_cons (SCM_atom ("x"),
                                                   SCM_cons (SCM_atom ("y"),
                                                             NULL)))),
                   SCM_cons (SCM_number ("1"),
                             SCM_cons (SCM_number ("2"),
                                       NULL)));
  ans = eval (&env, sexp);
  println (ans);
  free_object (sexp);
  free_object (ans);

  /* (cons 1 2) => (1 . 2) */
  printf ("> (cons 1 2)\n");
  sexp = SCM_cons (SCM_func (&fn_cons),
                   SCM_cons (SCM_number ("1"),
                             SCM_cons (SCM_number ("2"),
                                       NULL)));
  ans = eval (&env, sexp);
  println (ans);
  free_object (sexp);
  free_object (ans);

  /* (define square (lambda (x) (* x x))) */
  printf ("> (define square (lambda (x) (* x x)))\n");
  sexp = SCM_cons (SCM_func (&fn_define),
                   SCM_cons (SCM_atom ("square"),
                             SCM_cons (SCM_lambda (SCM_cons (SCM_atom ("x"),
                                                             NULL),
                                                   SCM_cons (SCM_func (&fn_mul),
                                                             SCM_cons (SCM_atom ("x"),
                                                                       SCM_cons (SCM_atom ("x"),
                                                                                 NULL)))),
                                       NULL)));
  ans = eval (&env, sexp);
  println (ans);
  free_object (sexp);
  free_object (ans);

  /* (square 8) => 64 */
  printf ("> (square 8)\n");
  sexp = SCM_cons (SCM_atom ("square"),
                   SCM_cons (SCM_number ("8"),
                             NULL));
  ans = eval (&env, sexp);
  println (ans);
  free_object (sexp);
  free_object (ans);

  /* (define new_square square) */
  printf ("> (define new_square square)\n");
  sexp = SCM_cons (SCM_func (&fn_define),
                   SCM_cons (SCM_atom ("new_square"),
                             SCM_cons (SCM_atom ("square"),
                                       NULL)));
  ans = eval (&env, sexp);
  println (ans);
  free_object (sexp);
  free_object (ans);

  /* (define new_square (lambda (x) (square x))) */
  printf ("> (define new_square (lambda (x) (square x)))\n");
  sexp = SCM_cons (SCM_func (&fn_define),
                   SCM_cons (SCM_atom ("new_square"),
                             SCM_cons (SCM_lambda (SCM_cons (SCM_atom ("x"),
                                                             NULL),
                                                   SCM_cons (SCM_atom ("square"),
                                                             SCM_cons (SCM_atom ("x"),
                                                                       NULL))),
                                       NULL)));
  ans = eval (&env, sexp);
  println (ans);
  free_object (sexp);
  free_object (ans);

  /* (define factorial (lambda (n) (if (< n 1) 1 (* n (factorial (- n 1)))))) */
  printf ("> (define factorial (lambda (n) (if (< n 1) 1 (* n (factorial (- n 1))))))\n");
  sexp = SCM_cons (SCM_func (&fn_define),
                   SCM_cons (SCM_atom ("factorial"),
                             SCM_cons (SCM_lambda (SCM_cons (SCM_atom ("n"),
                                                             NULL),
                                                   SCM_cons (SCM_func (&fn_if),
                                                             SCM_cons (SCM_cons (SCM_func (&fn_lt),
                                                                                 SCM_cons (SCM_atom ("n"),
                                                                                           SCM_cons (SCM_number ("1"),
                                                                                                     NULL))),
                                                                       SCM_cons (SCM_number ("1"),
                                                                                 SCM_cons (SCM_cons (SCM_func (&fn_mul),
                                                                                                     SCM_cons (SCM_atom ("n"),
                                                                                                               SCM_cons (SCM_cons (SCM_atom ("factorial"),
                                                                                                                                   SCM_cons (SCM_cons (SCM_func (&fn_sub),
                                                                                                                                                       SCM_cons (SCM_atom ("n"),
                                                                                                                                                                 SCM_cons (SCM_number ("1"),
                                                                                                                                                                           NULL))),
                                                                                                                                             NULL)),
                                                                                                                         NULL))),
                                                                                           NULL))))),
                                       NULL)));
  ans = eval (&env, sexp);
  println (ans);
  free_object (sexp);
  free_object (ans);

  /* (factorial 6) => 720 */
  printf ("> (factorial 6)\n");
  sexp = SCM_cons (SCM_atom ("factorial"),
                   SCM_cons (SCM_number ("6"),
                             NULL));
  ans = eval (&env, sexp);
  println (ans);
  free_object (sexp);
  free_object (ans);

  /* (new_square 9) => 81 */
  printf ("> (new_square 9)\n");
  sexp = SCM_cons (SCM_atom ("new_square"),
                   SCM_cons (SCM_number ("9"),
                             NULL));
  ans = eval (&env, sexp);
  println (ans);
  free_object (sexp);
  free_object (ans);

  /* ((lambda (x) (if (< x 0) (- x) x)) -10) => 10 */
  printf ("> ((lambda (x) (if (< x 0) (- x) x)) -10)\n");
  sexp = SCM_cons (SCM_lambda (SCM_cons (SCM_atom ("x"),
                                         NULL),
                               SCM_cons (SCM_func (&fn_if),
                                         SCM_cons (SCM_cons (SCM_func (&fn_lt),
                                                             SCM_cons (SCM_atom ("x"),
                                                                       SCM_cons (SCM_number ("0"),
                                                                                 NULL))),
                                                   SCM_cons (SCM_cons (SCM_func (&fn_sub),
                                                                       SCM_cons (SCM_atom ("x"),
                                                                                 NULL)),
                                                             SCM_cons (SCM_atom ("x"),
                                                                       NULL))))),
                   SCM_cons (SCM_number ("-10"),
                             NULL));
  ans = eval (&env, sexp);
  println (ans);
  free_object (sexp);
  free_object (ans);

  /* (define x unbound) */
  printf ("> (define x unbound)\n");
  sexp = SCM_cons (SCM_func (&fn_define),
                   SCM_cons (SCM_atom ("x"),
                             SCM_cons (SCM_atom ("unbound"),
                                       NULL)));
  ans = eval (&env, sexp);
  println (ans);
  free_object (sexp);
  free_object (ans);

  /* (1 2 3) */
  printf ("> (1 2 3)\n");
  sexp = SCM_cons (SCM_number ("1"),
                   SCM_cons (SCM_number ("2"),
                             SCM_cons (SCM_number ("3"),
                                       NULL)));
  ans = eval (&env, sexp);
  println (ans);
  free_object (sexp);
  free_object (ans);

  /* (+ 1 (* "haha" 1)) */
  printf ("> (+ 1 (* \"haha\" 1))\n");
  sexp = SCM_cons (SCM_func (&fn_add),
                   SCM_cons (SCM_cons (SCM_func (&fn_mul),
                                       SCM_cons (SCM_string ("haha"),
                                                 SCM_cons (SCM_number ("1"),
                                                           NULL))),
                             NULL));
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
