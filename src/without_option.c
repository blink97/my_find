#include "without_option.h"

/**
** \brief Will display the path of each files if the directory
** \param entry Is the dirent of the current directory
** \param path Is the path of the file
** \param option Is the option that is executed with the function
** \return Return the an int: 1 if an error occure, else return 0
*/
int printing_rec(struct dirent *entry, char *path, int option)
{
  printf ("%s%s\n", path, entry->d_name);
  int res = 0;
  char *new_path = my_concat(path, entry->d_name);
  char *to_free = new_path;
  new_path = my_concat(new_path, "/");
  free(to_free);
  char *link = check_for_link(new_path);
  struct stat stat;
  if (lstat(link, &stat) < 0)
    return 1;
  free(link);
  if (S_ISDIR(stat.st_mode) || (option == 2 && S_ISLNK(stat.st_mode)))
  {
    DIR *dir = opendir(new_path);
    if (!dir)
      return manage_error(new_path);
    struct dirent *new_entry = readdir(dir);
    while (new_entry && res != 1)
    {
      if (my_strcmp(new_entry->d_name, "..") != 0 &&
          my_strcmp(new_entry->d_name, ".") != 0)
        res = printing_rec(new_entry, new_path, option);
      new_entry = readdir(dir);
    }
    closedir(dir);
  }
  free(new_path);
  return 0;
}
