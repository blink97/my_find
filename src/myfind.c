/**
** \author Etienne Gandilhon
** \version 1.0
** \date 2017-10-22
** \copyright EPITA
** \mainpage Myfind
*/

#include "myfind.h"

/**
** \brief Will manage some error
** \param s Is the name of a file
** \return 0 if no error occures, 1 else
*/
int manage_error(char *s)
{
  if (open(s, O_RDONLY) >= 0)
  {
    printf("%s\n", s);
    return 0;
  }
  fprintf(stderr, "myfind: %s: %s\n", s, strerror(errno));
  return 1;
}

/**
** \brief Will check if the path contains an / at the end
** \param path Is the path
** \param to_free Is a variable to know if we need to free it after
** \return Return the new path
*/
char *check_slash(char *path, int *to_free)
{
  size_t len = my_strlen(path);
  if (path[len-1] == '/')
    return path;
  else
  {
    *to_free = 1;
    path = my_concat(path, "/");
  }
  return path;
}

/**
** \brief Will executes the right function deppeding the option
** \param entry Is the curent directory
** \param path Is the path of the file
** \param option Is the option
** \return Return the value of the executed function
*/
int choose_option(struct dirent *entry, char *path, int *option)
{
  int res = 0;
  switch (option[0])
  {
    case 0:
      res = printing_rec(entry, path, option[1]);
      break;
    case 1:
      res = printing_d(entry, path, option[1]);
      break;
    default:
      break;
  }
  return res;
}

/**
** \brief Will runs in file and executes function on each
** \param path Is the path of directory
** \param option Is the option
*/
int start_exec(char *path, int *option)
{
  DIR *dir = opendir(path);
  if (!dir)
    return manage_error(path);
  struct dirent *entry = readdir(dir);
  int res = 0;
  if (option[0] != 1)
    printf("%s\n", path);
  struct stat stat;
  char *link = check_for_link(path);
  if (lstat(link, &stat) < 0)
    return 1;
  if (option[1] != 1 && S_ISLNK(stat.st_mode))
    res = 1;
  free(link);
  char *tmp = path;
  int to_free = 0;
  char *tmp2 = check_slash(path, &to_free);
  if (option[1] == 1)
    option[1] = 0;
  while (entry && res != 1)
  {
    if (my_strcmp(entry->d_name, "..") != 0 &&
        my_strcmp(entry->d_name, ".") != 0)
      res = choose_option(entry, tmp2, option);
    entry = readdir(dir);
  }
  if (to_free)
    free(tmp2);
  if (option[0] == 1)
    printf("%s\n", tmp);
  closedir(dir);
  return res;
}

/**
** \brief Will add an new expression to the expressions array
** \param expressions Is the array containing the expressions
** \param arg Is the that will be add to the array
** \param nb_ex Is the number of expression in the array
*/
void adding_exp(char **expressions, char *arg, int *nb_ex)
{
  (*nb_ex)++;
  expressions[(*nb_ex) - 1] = arg;
}

/**
** \brief Will parse the expression
** \param arg is the argument parse
** \param option Is the array of option
** \param nb_path Is the number of path
** \param paths Is the array of path
** \param nb_ex Is the number of expression
** \param expressions Is the array of expressions
*/
void check_first(char *arg, int *option,int *nb_path, char **paths, int *nb_ex,
                 char **expressions)
{
  if (!arg)
  {
    option[1] = -2;
    return;
  }
  if (arg[0] == '-')
  {
    switch (arg[1])
    {
      case 'd':
        option[0] = 1;
        break;
      case 'H':
        option[1] = 1;
        break;
      case 'L':
        option[1] = 2;
        break;
      case 'P':
        option[1] = 0;
         break;
      default:
        adding_exp(expressions, arg, nb_ex);
    }
    return;
  }
  (*nb_path)++;
  paths[(*nb_path) - 1] = arg;
}

/**
** \brief Will executes the ast
** \param ast Is the ast
** \param path Is the path of the file
** \return Return 1 if the ast execute the good argument, 0 else
*/
int exec_ast(struct ast_node *ast, char *path, int is_print)
{
  DIR *dir = opendir(path);
  if (!dir)
    return manage_error(path);
  struct dirent *entry = readdir(dir);
  int res = 0;
  res = eval_ast(ast, path, is_print);
  int to_free = 0;
  char *tmp = check_slash(path, &to_free);
  while (entry)
  {
    if (my_strcmp(entry->d_name, "..") != 0 &&
        my_strcmp(entry->d_name, ".") != 0)
    {
      char *new_path = my_concat(tmp, entry->d_name);
      if (entry->d_type == DT_DIR)
        res = exec_ast(ast, new_path, is_print);
      res = eval_ast(ast, new_path, is_print);
      free(new_path);
    }
    entry = readdir(dir);
  }
  if (to_free)
    free(tmp);
  closedir(dir);
  return res;
}

/**
** \brief Is the recursive function of exec_exp
*/
int exec_exp_rec(char *expressions, char **arg, struct dirent *entry,
                 char *path)
{
  int res = 1;
  if (my_strcmp(expressions, "-name") == 0)
  {
    if (exp_name(arg[0], entry->d_name) == 1)
      printf("%s\n", path);
  }
  if (my_strcmp(expressions, "-type") == 0)
  {
    if (exp_type(arg[0], path) == 1)
      printf("%s\n", path);
  }
  if (my_strcmp(expressions, "-print") == 0)
    printf("%s\n", path);
  if (my_strcmp(expressions, "-exec") == 0)
  {
    exp_exec(arg, path);
  }
  return res;
}

/**
** \brief Will execute the expression if there are only one expression
** \param exepressions Is the expresion
** \param arg Is the list of argument of the function
** \param path Is the path of the file
*/
int exec_exp(char *expressions, char **arg, char *path)
{
  DIR *dir = opendir(path);
  if (!dir)
    return manage_error(path);
  struct dirent *entry = readdir(dir);
  int to_free = 0;
  char *tmp = check_slash(path, &to_free);
  int res = 1;
  res = exec_exp_rec(expressions, arg, entry, path);
  while(entry)
  {
    if (my_strcmp(entry->d_name, "..") != 0 &&
        my_strcmp(entry->d_name, ".") != 0)
    {
      char *new_path = my_concat(tmp, entry->d_name);
      if (entry->d_type == DT_DIR)
        res = exec_exp(expressions, arg, new_path);
      res = exec_exp_rec(expressions, arg, entry, new_path);
      free(new_path);
    }
    entry = readdir(dir);
  }
  if (to_free)
    free(tmp);
  closedir(dir);
  return res;
}

/**
** \brief Is the main function. Will executes the program
** \param argc Is the number of arguments
** \param argv Is the list of arguments
*/
int main(int argc, char **argv)
{
  char *path = ".";
  char **paths = malloc(sizeof (char*) * argc);
  if (!paths)
  {
    fprintf(stderr, "myfind: %s\n", strerror(errno));
    return 1;
  }
  char **expressions = malloc(sizeof (char*) * argc);
  if (!expressions)
  {
    free(paths);
    fprintf(stderr, "myfind: %s\n", strerror(errno));
    return 1;
  }
  int option[2] = { 0,0 };
  if (argc == 1) //0 arg given
  {
    free(paths);
    free(expressions);
    return start_exec(path, option);
  }
  int nb_path = 0;
  int nb_ex = 0;
  int is_print = 0;
  struct ast_node *ast = NULL;
  for (int i = 1; i < argc; i++)
  {
    int tmp = nb_ex;
    check_first(argv[i], option, &nb_path, paths, &nb_ex, expressions);
    if (tmp != nb_ex)
    {
      ast = create_ast(argv, i, argc, &is_print);
      i = argc;
    }
  }
  int res = 0;
  if (nb_path == 0)
  {    
    if (nb_ex == 0)
    {
      free(paths);
      free(expressions);
      return start_exec(path, option);
    }
    else
    {
      if (!ast->left && !ast->right)
        exec_exp(ast->data, ast->arg, "./");
      res = exec_ast(ast, "./", is_print);
    }
  }
  for (int i = 0; i < nb_path; i++)
  {
    if (nb_ex == 0)
    {
      res = start_exec(paths[i], option);
    }
    else
    {
      if (!ast->left && !ast->right)
        exec_exp(ast->data, ast->arg, paths[i]);
      res = exec_ast(ast, paths[i], is_print);
    }
  }
  clean_ast(ast);
  free(paths);
  free(expressions);
  return res;
}
