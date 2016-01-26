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
  object *old1 = lambda (cons (atom ("x"),
                               NULL),
                         cons (func (&fn_mul),
                               cons (atom ("x"),
                                     cons (atom ("x"),
                                                 NULL))));
  object *old2 = cons (atom ("pi"), number ("3.14159265"));
  object *li = cons (old1,
                     cons (number ("1"),
                           cons (old2,
                                 NULL)));
  object *li_with = cons (cons (old1, atom ("a")),
                          cons (cons (old2, atom ("haha")),
                                NULL));
  printf ("old list: ");
  println (li);
  replace (&li, li_with);
  printf ("new list: ");
  println (li);
  free_object (li);
  free_li_with (li_with);
  return 0;
}
