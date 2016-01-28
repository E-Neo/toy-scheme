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
  object *args = cons (atom ("x"),
                       cons (atom ("y"),
                             NULL));
  object *sexp = cons (func (&fn_add),
                       cons (atom ("x"),
                             cons (atom ("y"),
                                   NULL)));
  object *obj = lambda (args, sexp);

  object *new1 = number ("1");
  object *new2 = number ("2");
  object *ans;

  println (sexp);
  atom_replace (&sexp, car (args), new1);
  atom_replace (&sexp, car (cdr (args)), new2);
  println (sexp);

  /* ((lambda (x y) (+ x y)) 1 2) => 3 */
  ans = eval (env, sexp);
  println (ans);
  free_object (ans);

  free_object (obj);
  free_object (new1);
  free_object (new2);
  free_object (env);

  return 0;
}
