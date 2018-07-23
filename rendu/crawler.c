#include "index_functions.h"
#include "crawler.h"
//TODO : faire pareil que l'index avec le parser, plutôt que de recopier
//TODO : Ensuite voir si on met tout dans un seul .h, mais je ne pense pas

/*
  Cette fonction sert à modifier l'url trouvée par le parser dans le document HTML
  afin de rendre l'url correcte pour libcurl
  -url_list : la liste d'url, avec en premier l'url d'où a été récupéré url
  -url : l'url récupérée depuis le premier url contenu dans url_list
*/
char *analyze_url(list **url_list, char *url)
{
  //printf("Je rentre dans analyze_url\n");
  char *new_url;
  char *url_mom = (*url_list)->str;

  printf("On me passe l'url : %s\n", url);
  //printf("URL_MOM : %s\n", url_mom);

  //Si l'url est un http ou https
  if (strncmp("http://", url, 7) == 0 || strncmp("https://", url, 8) == 0)
    return url;

  //Si l'url est une fichier local
  if (strncmp("file://", url_mom, 7) == 0)
  {
    //Si on a un lien dans le dossier local, du genre "docs/html.html"
    int index = 7;
    int index_max = 7;
    while (url_mom[++index])
    {
      if (url_mom[index] == '/')
        index_max = index + 1;
    }

    new_url = malloc(sizeof(char)*(index_max + strlen(url) + 2));
    new_url[0] = '\0';
    strncpy(new_url, url_mom, index_max);
    new_url[index_max] = '\0';
    strcat(new_url, url);
    return new_url;
  }

  if (strncmp("http://", url_mom, 7) == 0 || strncmp("https://", url_mom, 8) == 0)
  {
    //Si on a un lien du genre "index.html", il faut prendre le nom de domaine et ajouter cette url
    //Ou si on a une addresse qui va vers nomdedomaine.com/
    int index = 8;
    while(url_mom[index] != '/')
      index++;

    new_url = (char *)malloc(sizeof(char)*(index + strlen(url) + 2));
    new_url[0] = '\0';
    strncpy(new_url, url_mom, index);
    if (strncmp("/", url, 1) != 0)
    {
      new_url[index] = '/';
      new_url[index + 1] = '\0';
    }
    else
      new_url[index] = '\0';
    strcat(new_url, url);

    return new_url;
  }

  return NULL;
}

int main(int argc, char *argv[])
{
  //On regarde que l'utilisateur envoit bien au moins une url pour crawler
  if (argc < 3)
  {
    printf("usage : ./crawler [profondeur de parcours max] [url 1] [...] [url n]\n");
    exit(1);
  }

  //Initialisation des variables utiles
  int profondeur_max;
  list *url_list = NULL;
  list *url_list_first = NULL; //Utilse pour check si une url a déjà été visitée
  GHashTable *hashtable = extract_hash_table();
  FILE *graph;
  time_t start_t, end_t;
  double diff_t;

  graph = fopen("graph.dot", "w");
  fprintf(graph, "digraph G {\n");

  //On lit les paramètres passés au programme et on agit en conséquence
  profondeur_max = atoi(argv[1]);
  printf("max: %d\n", profondeur_max);
  for(int i = 2; i < argc; i++)
  {
    //printf("%d\n", i);
    add_list(&url_list, create_list(argv[i], 0));
  }
  url_list_first = url_list;

  //On commence la parcours des URLs
  time(&start_t);
  while (1)
  {
    if (url_list)
    {
      struct MemoryStruct chunk = fetch_page(url_list->str);
      //write(1, chunk.memory, (long)chunk.size);

      //Récupérer les listes de mots et d'url
      struct values *inThisDoc = malloc(sizeof(struct values));
      inThisDoc = parseMemory(chunk.memory);

      //NOTE : LIGNES DE DEBUG
      printf("----Je visite : %s\n", url_list->str);
      //printf("profondeur : %d\n", url_list->profondeur);

      //Ajouter les urls filles à la liste
        //Lorsqu'une url a une profondeur de profondeur_max, on n'ajoute pas les
        //liens de sa page à la liste
      for(node *cur_node = inThisDoc->URL_list; cur_node; cur_node=cur_node->next){
        if(url_list->profondeur + 1 <= profondeur_max && strcmp(cur_node->data, "End of List\n") != 0)
        {
          char *new_url = analyze_url(&url_list, cur_node->data);

          //On ajoute au graph
          fprintf(graph, "\"%s\" -> \"%s\";\n", url_list->str, new_url);

          //modifier cur_node->data en newurl, puis free newurl ?
          if (is_in_list(&url_list, new_url) == 0)
          {
            add_list_end(&url_list, create_list(new_url, url_list->profondeur + 1));
            //printf("Ajout URL : %s\n", cur_node->data);
            printf("Ajout URL : %s\n", new_url);
            //printf('"%s" -> "%s";', url_list->str, new_url);
          }
        }
      }

      //Faire les opérations pour l'index
      for(node *cur_node = inThisDoc->word_list; cur_node; cur_node=cur_node->next){
        if (cur_node->data != NULL && strcmp(cur_node->data, "\n") != 0 && strcmp(cur_node->data, "End of List\n") != 0)
        {
          add_url(cur_node->data, url_list->str, hashtable);
          printf("Ajout mot : \"%s\" -- %s\n", cur_node->data, url_list->str);
        }
      }

      free(chunk.memory);
      //free(inThisDoc);
      url_list = url_list->next;
    }
    else
      break;
    time(&end_t);
    diff_t = difftime(end_t, start_t);
    if (diff_t > 60)
    {
      save_index_changes(hashtable);
      start_t = end_t;
    }
  }

  save_index_changes(hashtable);
  fprintf(graph, "}");
  fclose(graph);

  //Générer le graph ici
  system("dot -Tpng -o graphe.png graph.dot");
}
