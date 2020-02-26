#ifndef AST_H
# define AST_H

# include <stdlib.h>

# include "expression_name.h"

struct list
{
  struct ast_node *data;
  struct list *next;
};

struct ast_node
{
  char *data;
  char **arg;
  struct ast_node *left;
  struct ast_node *right;
};

struct stack
{
  struct list *head;
  int size;
};

void clean_ast(struct ast_node *ast);

struct ast_node *init_node(char *data, char *arg);
void add_child(struct ast_node *node, struct ast_node *left,
               struct ast_node *right);

struct list *init_list(struct ast_node *node);

struct stack *init_stack(void);
void add_stack(struct stack *stack, struct list *list);
struct ast_node *remove_stack(struct stack *stack);

struct ast_node *create_ast(char **args, int begin, int argc, int *is_print);
void print_ast(struct ast_node *ast);

int eval_ast(struct ast_node *ast, char *path, int is_print);
#endif /* !AST_H */
