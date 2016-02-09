/* toy-scheme interpreter.

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
#include <readline/readline.h>
#include <readline/history.h>

static void
print_version ()
{
  printf (SCM_COPYRIGHT "\n");
}

static void
doREPL (object **env)
{
  int status = 1;
  char *line;
  object *sexp;
  object *ans;
  while (status)
    {
      line = readline ("> ");
      add_history (line);
      sexp = parse (env, line);
      ans = eval (env, sexp);
      println (ans);
      free_object (sexp);
      free_object (ans);
      free (line);
    }
}

int
main ()
{
  object *env = init_env ();
  print_version ();
  doREPL (&env);
  free_object (env);
  return 0;
}
