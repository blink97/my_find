#include "expression_name.h"

int exp_name(char *name, char *path)
{
  if (fnmatch(name, path, 0) == 0)
  {
    return 1;
  }
  return 0;
}

/**
** \brief Will looks if the path corespond to the type
** \param type Is the type that we look for
** \param path Is the path of the file
** \return Return 0 if the type isn't found, 1 else
*/
int exp_type(char *type, char *path)
{
  struct stat stat;
  if (lstat(path, &stat) < 0)
    return 0;
  if (type[0] == 'd' && S_ISDIR(stat.st_mode))
    return 1;
  if (type[0] == 'f' && S_ISREG(stat.st_mode))
    return 1;
  if (type[0] == 'l' && S_ISLNK(stat.st_mode))
    return 1;
  if (type[0] == 's' && S_ISSOCK(stat.st_mode))
    return 1;
  if (type[0] == 'l' && S_ISFIFO(stat.st_mode))
    return 1;
  if (type[0] == 'c' && S_ISCHR(stat.st_mode))
    return 1;
  if (type[0] == 'b' && S_ISBLK(stat.st_mode))
    return 1;
  return 0;
}

/**
** \brief Will copy an array of string
** \param exe Is the array of string
** \param path if the path of the file if exe contains "{}" patern
** \return Return the copy
*/
char **copy_exp(char **exe, char *path)
{
  int size = 0;
  while (exe[size] != NULL)
    size++;
  char **res = malloc(sizeof (char *) * (size + 1));
  if (!res)
  {
    fprintf(stderr, "myfind: %s\n", strerror(errno));
    return NULL;
  }
  for (int i = 0; i < size; i++)
  {
    size_t len = my_strlen(exe[i]);
    size_t len_tmp = len;
    for (size_t j = 0; j < len_tmp; j++)
    {
      if (exe[i][j] == '{')
      {
        if (j + 1 < len && exe[i][j+1] == '}')
        {
          len += my_strlen(path);
          j++;
        }
      }
    }
    char *cpy = malloc(sizeof (char) * (len + 1));
    if (!cpy)
    {
      free(res);
      fprintf(stderr, "myfind: %s\n", strerror(errno));
      return NULL;
    }
    size_t cpt = 0;
    for (size_t j = 0; j < len; j++)
    {
      if (exe[i][cpt] == '{')
      {
        if ((cpt + 1) < len_tmp && exe[i][cpt + 1] == '}')
        {
          for (size_t h = 0; h < my_strlen(path); h++)
          {
            cpy[j] = path[h];
            j++;
          }
          j--;
          cpt++;
        }
        else
          cpy[j] = exe[i][cpt];
      }
      else
        cpy[j] = exe[i][cpt];
      cpt++;
    }
    cpy[len] = '\0';
    res[i] = cpy;
  }
  return res;
}

/**
** \brief Will execute the fucntion that is in exe
** \param exe Is the array of string that contains the function
** \param path Is the path of the file
** \return Return 1 if no error ocures, 0 else
*/
int exp_exec(char **exe, char *path)
{
  int res = 0;
  pid_t pid = fork();
  char **tmp = copy_exp(exe, path);
  if (pid == 0)
  {
    execvp(tmp[0], tmp);
    fprintf(stderr, "myfind: %s\n", strerror(errno));
    exit(1);
  }
  else
  {
    int status = 0;
    waitpid(pid, &status, 0);
    res = 1;
  }
  int len = 0;
  while (exe[len] != NULL)
    len++;
  for (int i = 0; i < len; i++)
    free(tmp[i]);
  free(tmp);
  return res;
}
