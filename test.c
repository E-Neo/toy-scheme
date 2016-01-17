#include "scheme.h"

int
main ()
{
  object *a, *b, *c, *li;
  a = atom ("3.14159");
  b = atom ("cat");
  c = atom ("dog");
  li = NULL;
  append (&li, cons (a, NULL));
  append (&li, cons (b, NULL));
  append (&li, cons (c, NULL));
  println (li);
  free_object (li);
  return 0;
}
