#ifndef EXPRESSION_NAME_H
# define EXPRESSION_NAME_H

# include <fnmatch.h>
# include <sys/wait.h>

# include "myfind.h"

int exp_name(char *name, char *path);

int exp_type(char *type, char *path);

int exp_exec(char **exe, char *path);

#endif /* !EXPRESSION_NAME_H */
