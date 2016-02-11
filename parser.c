/* Parser for toy-scheme.

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

#include "parser.h"

object *
parse_atom (object **env, const char *str)
{
  return SCM_atom (str);
}

object *
parse_bool (object **env, const char *str)
{
  char *msg;
  object *ret;
  if (2 == strlen (str) && '#' == str[0])
    {
      if ('t' == str[1] || 'T' == str[1])
        ret = SCM_bool (1);
      else if ('f' == str[1] || 'F' == str[1])
        ret = SCM_bool (0);
      else
        {
          msg = malloc (18 + 2 + 1);
          sprintf (msg, "Unbound variable: %s", str);
          ret = SCM_error (EUNBOUND, msg);
          free (msg);
        }
    }
  else
    {
      msg = malloc (18 + strlen (str) + 1);
      sprintf (msg, "Unbound variable: %s", str);
      ret = SCM_error (EUNBOUND, msg);
      free (msg);
    }
  return ret;
}

object *
parse_number (object **env, const char *str)
{
  return SCM_number (str);
}

object *
parse (object **env, const char *str)
{
  return SCM_number ("3.14159265");
}
