#ifndef MYSTRLIB_H
# define MYSTRLIB_H

# include <errno.h>
# include <string.h>
# include <stdio.h>
# include <stddef.h>
# include <stdlib.h>

size_t my_strlen(const char *s);
int my_strcmp(const char *s1, const char *s2);
char *my_concat(const char *s1, const char *s2);
char *check_for_link(char *s1);

#endif /* !MYSTRLIB_H */
