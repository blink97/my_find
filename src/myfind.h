#ifndef MYFIND_H
# define MYFIND_H

# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <string.h>
# include <errno.h>
# include <dirent.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# include "mystrlib.h"
# include "without_option.h"
# include "option_d.h"
# include "expression_name.h"
# include "ast.h"

int manage_error(char *s);

#endif /* !MYFIND_H */
