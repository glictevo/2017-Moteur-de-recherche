#ifndef CRAWLER_H
# define CRAWLER_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <curl/curl.h>
# include <time.h>

struct MemoryStruct {
  char *memory;
  size_t size;
};

typedef struct _list {
  char         *str;
  int           profondeur;
  struct _list *next;
} list;

typedef struct node_ {
  char *data;
  struct node *next;
}              node;

struct values {
  struct node *word_list;
  struct node *URL_list;
} ;

list *create_list(char *str, int profondeur);
void delete_list(list **my_list);
void add_list(list **my_list, list *new);
void add_list_end(list **my_list, list *new);
int is_in_list(list **my_list, char *str);
void print_list(list **my_list);

struct MemoryStruct fetch_page(char *str);

struct values* parseMemory(char *toParse);


#endif
