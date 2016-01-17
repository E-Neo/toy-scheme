#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum type { ATOM, PAIR };

enum error { ENOMEM, ETYPE };

typedef struct
{
  enum type type;
} object;

typedef struct
{
  enum type type;
  char *name;
} atom_object;

typedef struct
{
  enum type type;
  object *car;
  object *cdr;
} pair_object;

#define car(X) (((pair_object *) X)->car)
#define cdr(X) (((pair_object *) X)->cdr)

void print_error (enum error error);
void print (object *obj);
void println (object *obj);

void free_object (object *obj);

int null_p (object *obj);
int pair_p (object *obj);
int list_p (object *obj);

object *atom (char *str);
object *cons (object *obj1, object *obj2);

void append (object **li, object *obj);
