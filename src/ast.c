#include <stdio.h>

#include "ast.h"

/**
** \brief Will free the ast
** \param ast Is the ast that will be cleaned
*/
void clean_ast(struct ast_node *ast)
{
  if (!ast)
    return;
  clean_ast(ast->left);
  clean_ast(ast->right);
  free(ast->arg);
  free(ast);
}

/**
** \brief Will create a node of the ast with the paramaters of exec
** \param data Is the type de the node
** \param args Is the list of all args
** \param begin Is the begin of the args in the list
** \return Return the node created
*/
struct ast_node *init_node_exec(char *data, char **args, int *begin)
{
  struct ast_node *node = malloc(sizeof (struct ast_node));
  if (!node)
  {
    fprintf(stderr, "myfind: %s\n", strerror(errno));
    return NULL;
  }
  node->data = data;
  int size = 0;
  while (args[*begin + size][0] != ';')
    size++;
  char **list_arg = malloc(sizeof (char*) * (size));
  if (!list_arg)
  {
    free(node);
    fprintf(stderr, "myfind: %s\n", strerror(errno));
    return NULL;
  }
  for (int i = 0; i < size; i++)
  {
    list_arg[i] = args[*begin + 1 + i];
    if (list_arg[i][0] == ';')
      list_arg[i] = NULL;
  }
  node->arg = list_arg;
  node->left = NULL;
  node->right = NULL;
  *begin += size;
  return node; 
}
/**
** \brief Will create a node of the ast
** \param data Is the type de the node
** \param args Is the list of all args
** \return Return the node created
*/
struct ast_node *init_node(char *data, char *arg)
{
  struct ast_node *node = malloc(sizeof (struct ast_node));
  if (!node)
  {
    fprintf(stderr, "myfind: %s\n", strerror(errno));
    return NULL;
  }
  char **args = malloc(sizeof (char*) * 1);
  if (!args)
  {
    fprintf(stderr, "myfind: %s\n", strerror(errno));
    return NULL;
  }
  args[0] = arg;
  node->data = data;
  node->arg = args;
  node->left = NULL;
  node->right = NULL;
  return node;
}

/**
** \brief Will add the child to the root
** \param node Is the root
** \param left Is the left child
** \param right Is the right child
*/
void add_child(struct ast_node *node, struct ast_node *left,
               struct ast_node *right)
{
  node->left = left;
  node->right = right;
}

/**
** \brief Will create a list with an ast
** \param node Is the ast that will be in the list
** \return Return the list just created
*/
struct list *init_list(struct ast_node *node)
{
  struct list *list = malloc(sizeof (struct list)); 
  if (!list)
  {
    fprintf(stderr, "myfind: %s\n", strerror(errno));
    return NULL;
  }
  list->data = node;
  list->next = NULL;
  return list;
}

/**
** \brief Will create a stack
** \return Return the stack just created
*/
struct stack *init_stack(void)
{
  struct stack *stack = malloc(sizeof (struct stack));
  if (!stack)
  {
    fprintf(stderr, "myfind: %s\n", strerror(errno));
    return NULL;
  }
  stack->head = NULL;
  stack->size = 0;
  return stack;
}

/**
** \brief Will add to the stack a list
** \param stack Is the stack
** \param list Is the list that will be push in the stack
*/
void add_stack(struct stack *stack, struct list *list)
{
  if (stack->size == 0)
    stack->head = list;
  else
  {
    list->next = stack->head;
    stack->head = list;
  }
  stack->size += 1;
}

/**
** \brief Will pop the head of the stack
** \param stack Is the stack
** \return Return the ast in the head
*/
struct ast_node *remove_stack(struct stack *stack)
{
  struct list *tmp = stack->head;
  stack->head = stack->head->next;
  stack->size -= 1;
  struct ast_node *node = tmp->data;
  free(tmp);
  return node;
}

/**
** \brief Will create the entire ast form an list of arguments
** \param args Is the list of arguments
** \param begin Is the begin of the expressions
** \param argc is the length of args
** \return Retturn the entire ast
*/
struct ast_node *create_ast(char** args, int begin, int argc, int *is_print)
{
  struct stack *operators = init_stack();
  struct stack *node_ast = init_stack();
  int prev = 0;
  for (int i = begin; i < argc; i++)
  {
    char *tmp = args[i];
    if (tmp[0] == '-')
    {
      if (tmp[1] == 'p' || tmp[1] == 'e')
        *is_print = 1;
      if (tmp[1] == 'o')
      {
        while (operators->size > 0)
        {
          struct ast_node *node = remove_stack(operators);
          struct ast_node *right = remove_stack(node_ast);
          struct ast_node *left = remove_stack(node_ast);
          node->left = left;
          node->right = right;
          struct list *new_list = init_list(node);
          add_stack(node_ast, new_list);
        }
        struct ast_node *new_node = init_node("-o", NULL);
        struct list *new_list = init_list(new_node);
        add_stack(operators, new_list);
        prev = 0;
      }
      else if (tmp[1] == 'a' || prev == 1)
      {
        struct ast_node *new_node = init_node("-a", NULL);
        struct list *new_list = init_list(new_node);
        add_stack(operators, new_list);
        prev = 0;
      }
      if (tmp[1] != 'a' && tmp[1] != 'o' && tmp[1] != '-')
      {
        struct ast_node *new_node = NULL;
        if (tmp[1] == 'e')
          new_node = init_node_exec(tmp, args, &i);
        else
          new_node = init_node(tmp, args[i+1]);
        struct list *new_list = init_list(new_node);
        add_stack(node_ast, new_list);
        prev = 1;
      }
    }
  }
  while (operators->size > 0)
  {
    struct ast_node *node = remove_stack(operators);
    struct ast_node *right = remove_stack(node_ast);
    struct ast_node *left = remove_stack(node_ast);
    node->left = left;
    node->right = right;
    struct list *new_list = init_list(node);
    add_stack(node_ast, new_list);
  }
  struct ast_node *node = remove_stack(node_ast);
  free(operators);
  free(node_ast);
  return node;
}

/**
** \brief Will exec the childs of the node if the results are the good one
** \param ast Is the node
** \param res_left Is the result of the left child
** \param res_right Is the result of the right child
** \param path Is the path of the directory
*/
void exec_next(struct ast_node *ast, int res_left, int res_right, char *path,
                int is_print)
{
  if (!ast)
    return;
  if (is_print)
  {
    if ((ast->left->data)[1] == 'p' || (ast->right->data)[1] == 'p')
      printf("%s\n", path);
    return;
  }
  /* test if the next node is a expression different of "and" or "or" */
  if (((ast->left->data)[1] != 'a' && (ast->left->data)[1] != 'o') ||
      ((ast->right->data)[1] != 'a' && (ast->right->data)[1] != 'o'))
  {
    if ((ast->data)[1] == 'a') 
    {
      if (res_left && res_right)
        printf("%s\n", path);
    }
    else if ((ast->data)[1] == 'o') 
    {
      if (res_left || res_right)
        printf("%s\n", path);
    }
  }
}

/**
** \brief Will evalutate the ast
** \param ast Is the ast
** \param path Is the path of the directory
** \return Return the result of the ast
*/
int eval_ast(struct ast_node *ast, char *path, int is_print)
{
  if (!ast)
    return 1;
  int res_left = 1; // 1 == True 0 == False
  int res_right = 1;
  res_left = eval_ast(ast->left, path, is_print);
  if ((ast->data)[1] == 'a' && res_left == 0) //and
    return 0;
  if ((ast->data)[1] == 'o' && res_left == 1) //or
  {
    if ((ast->right->data)[1] != 'p')
      printf("%s\n", path);
    return 1;
  }
  if ((ast->data)[1] == 'n') //name
    return exp_name((ast->arg)[0], path);
  if ((ast->data)[1] == 't')
    return exp_type((ast->arg)[0], path);
  if ((ast->data)[1] == 'p')
    return 1;
  res_right = eval_ast(ast->right, path, is_print);
  if ((ast->data)[1] == 'a' || (ast->data)[1] == 'o')
    exec_next(ast, res_left, res_right, path, is_print);
  return res_right;
}
