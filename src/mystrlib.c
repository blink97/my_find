#include "mystrlib.h"


/**
** \brief Will conts the number of char in the string
** \param s Is the string
** \return Return the number of char
*/
size_t my_strlen(const char *s)
{
  size_t len = 0;
  while (s[len] != '\0')
    len++;
  return len;
}

/**
** \brief Will compares two string
** \param s1 Is the first string
** \param s2 Is the second string
** return 0 is the two string are the same, an int different from 0 else
*/
int my_strcmp(const char *s1, const char *s2)
{
  size_t len = 0;
  while (s1[len] != '\0' && s2[len] != '\0')
  {
    if (s1[len] != s2[len])
      return s1[len] - s2[len];
    len++;
  }
  return s1[len] - s2[len];
}

/**
** \brief Will concatenate two string
** \param s1 Is the first string
** \param s2 Is the second string
** \return Return the final string
*/
char *my_concat(const char *s1, const char *s2)
{
  size_t len1 = my_strlen(s1);
  size_t len2 = my_strlen(s2);
  char *new_s = malloc(sizeof (char) * (len1 + len2 + 1));
  if (!new_s)
  {
    fprintf(stderr, "myfind: %s\n", strerror(errno));
    return NULL;
  }
  for (size_t i = 0; i < len1; i++)
    new_s[i] = s1[i];
  for (size_t j = 0; j < len2; j++)
    new_s[len1 + j] = s2[j];
  new_s[len1 + len2] = '\0';
  return new_s;
}

/*
** \brief Will check the if the string have a "\" at the end for the symlink
** \param s1 Is the string
** \return Return the new string
*/
char *check_for_link(char *s1)
{
  int len = my_strlen(s1);
  char *new_s = malloc(sizeof (char) * len + 1);
  if (!new_s)
  {
    fprintf(stderr, "myfind: %s\n", strerror(errno));
    return NULL;
  }
  for (int i = 0; i <= len; i++)
  {
    new_s[i] = s1[i];
  }
  if (new_s[len - 1] != '/')
  {
    return new_s;
  }
  new_s[len - 1] = '\0';
  return new_s;
}
