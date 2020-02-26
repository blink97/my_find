#ifndef WITHOUT_OPTION_H
# define WITHOUT_OPTION_H

# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>

# include "myfind.h"
# include "mystrlib.h"

int printing_rec(struct dirent *entry, char *path, int option);

#endif /* !WITHOUT_OPTION */
