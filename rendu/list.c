#include "crawler.h"

/*
  Créé une nouvelle liste avec le string passé en argument
  - str : le string à mettre dans la liste
*/
list *create_list(char *str, int profondeur)
{
  list *my_list;

  if ((my_list = (list *)malloc(sizeof(list))) == NULL)
    return NULL;
  my_list->next = NULL;
  my_list->str = strdup(str);
  my_list->profondeur = profondeur;
  //NOTE : A voir s'il ne faudrait pas copier plutôt le data
  return my_list;
}

/*
  Supprime entièrement une liste
  - my_list : la liste à supprimer
*/
void delete_list(list **my_list)
{
  list *tmp;
  while (*my_list != NULL)
  {
    tmp = (*my_list)->next;
    free((*my_list)->str);
    (*my_list)->str = NULL; //Peut-être faux
    free(*my_list); //Peut-être faux
    *my_list = tmp;
  }
}

/*
  Ajoute une liste au début d'une liste déjà existante
  - my_list : la liste où on ajoute l'autre liste
  - new : la liste à ajouter
*/
void add_list(list **my_list, list *new)
{
  if (*my_list)
    new->next = *my_list;
  *my_list = new;
}

/*
  Ajoute une liste à la fin d'une liste déjà existante
  - my_list : la liste où on ajoute l'autre liste
  - new : la liste à ajouter
*/
void add_list_end(list **my_list, list *new)
{
  list *tmp;

  tmp = *my_list;
  if (tmp)
  {
    while (tmp->next)
      tmp = tmp->next;
    tmp->next = new;
  }
  else
    *my_list = new;
}

/*
  Regarde si le string passé en argument existe déjà dans la liste
  Renvoie 1 s'il trouve le string, 0 sinon
  - my_list : la liste dans laquelle on cherche le string
  - str : le string qu'on cherche
*/
int is_in_list(list **my_list, char *str)
{
  list *tmp;

  tmp = *my_list;
  if (tmp)
  {
    while (tmp)
    {
      //printf("Je compare : \n%s\navec\n%s\n", tmp->str, str);
      if (strcmp(tmp->str, str) == 0)
        return 1;
      tmp = tmp->next;
    }
    return 0;
  }
  else
    return 0;
}

/*
  Affiche lle contenu de la liste passée en argument
  - my_list : la liste à parcourir
*/
void print_list(list **my_list)
{
  list *tmp;

  tmp = *my_list;
  while (tmp != NULL)
  {
    printf("Contenu : %s\n", tmp->str);
    tmp = tmp->next;
  }
}
