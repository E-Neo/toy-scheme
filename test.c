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
