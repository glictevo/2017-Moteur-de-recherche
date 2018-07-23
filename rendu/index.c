# include "index_functions.h"

void save_index_changes (GHashTable *hashtable);
void write_on_index (gpointer key, gpointer value, gpointer file);
int check_word_pres (char *word, GHashTable *hashtable);
int check_url_pres (char *word, char *url, GHashTable *hashtable);
GHashTable *extract_hash_table ();
char *read_index_line (FILE *file);
void print_index_row (gpointer key, gpointer value, gpointer user_data);
void print_hashtable (GHashTable *hashtable);

/*
* Cette fonction ajoute un mot dans l'index, sauf s'il existe déjà. La fonction renvoie 0 si le mot a été ajouté correctement, 1 s'il existe déjà,
* ou 2 en cas d'erreur.
*/
int add_word (char *word, GHashTable *hashtable) {
  if (check_word_pres(word, hashtable) == 1) {
    g_hash_table_insert(hashtable, word, NULL);
    return 0;
  } else if (check_word_pres(word, hashtable) == 0) {
    return 1;
  }
  return 2;
}

/*
* Cete fonction ajoute une URL à la liste d'URLs associée à un mot. La fonction renvoie 0 si l'URL a été ajoutée correctement,
* 1 si elle existait déjà et 2 en cas d'erreur.
*/
int add_url (char *word, char *url, GHashTable *hashtable) {
  char * urls = malloc(sizeof(urls));
  /*On vérifie si le mot passé argument existe. Si ce n'est pas le cas (1), on l'ajoute à l'index, et on ne vérifie pas que l'URL existe déjà pour ce mot.*/
  if (check_word_pres(word, hashtable) == 1) {
    /*On ajoute le mot à l'index.*/
    if (add_word(word, hashtable) == 0) {
      urls = strdup(url);
    } else {
      return 2;
    }
  } else {
    /*On vérifie que l'URL n'existe pas déjà. Si elle n'existe pas, on extraie la liste d'URL et on la prépare pour y ajouter la nouvelle URL.*/
    if (check_url_pres(word, url, hashtable) == 1) {
      urls = (char *)g_hash_table_lookup(hashtable, word);
      //printf("On extrait urls : %s\n", urls);
      if (urls == NULL) { /*S'il n'y a aucune URL associée au mot.*/
        urls = strdup(url);
      } else {  /*S'il y a déjà des URLs associées au mot.*/
        urls = realloc(urls, sizeof(char)*(strlen(urls) + strlen(url)) + sizeof(char)*2);
        urls = strcat(urls, "{");
        urls = strcat(urls, url);
      }
    } else {
      return 1;
    }
  }
  /*On ajoute l'URL dans la hashtable.*/
  //printf("On ajoute : %s\n", urls);
  g_hash_table_replace(hashtable, word, urls);
  return 0;
}

/*
* Cette fonction supprime l'entrée word de l'index, ainsi que les URLs qui lui sont associées.
* La fonction renvoie 0 si le mot existe et a été supprimé, 1 dans le cas où le mot n'existe pas ou n'a pas pu être supprimé.
*/
int remove_word (char *word, GHashTable *hashtable) {
  if (check_word_pres(word, hashtable) == 0) {
    if (g_hash_table_remove(hashtable, word) == TRUE) {
      return 0;
    }
  }
  return 1;
}

/*
* Supprime l'URL passée en paramètre, associée au mot passé en paramètre.
* Renvoie 0 si l'URL existe et est supprimée correctement, 1 sinon.
*/
int remove_url (char *word, char *url, GHashTable *hashtable) {
  if (check_word_pres(word, hashtable) == 0) {
    if (check_url_pres(word, url, hashtable) == 0) {
      char *urls = (char *)strdup(g_hash_table_lookup(hashtable, word));
      char *new_urls = malloc(sizeof(char)*(strlen(urls) - strlen(url) + 1));
      new_urls[0] = '\0';
      char *token = strtok(urls, "{");
      while (token != NULL) {
        if (strcmp(token, url) != 0) {
          new_urls = strcat(new_urls, token);
        }
        token = strtok(NULL, "{");
        if (token != NULL) {
          new_urls = strcat(new_urls, "{");
        }
      }
      g_hash_table_replace(hashtable, word, new_urls);
      return 0;
    }
  }
  return 1;
}

char *send_urls (char *word, GHashTable *hashtable) {
  char *urls;
  if (check_word_pres(word, hashtable) == 0) {
    urls = g_hash_table_lookup(hashtable, word);
  } else {
    urls = SENDURLS_ERROR;
  }
  return urls;
}

/*Fonctions internes à l'index*/

/*
* Cette fonction sauvegarde l'index modélisé par la hashtable dans le fichier index.csv
*/
void save_index_changes (GHashTable *hashtable) {
  if (access("new_index_file", F_OK) != -1) {
    remove("new_index_file");
  }
  FILE *new_index_file = fopen("new_index_file", "a");
  g_hash_table_foreach(hashtable, write_on_index, new_index_file);
  fclose(new_index_file);
  remove(INDEX_PATHNAME);
  rename("new_index_file", INDEX_PATHNAME);
}

/*
* Cette fonction auxiliaire permet d'écrire une ligne depuis la hashtable vers le fichier index.
* Cette fonction est appelée par save_index_changes et considère donc que le fichier file est déjà ouvert en écriture.
*/
void write_on_index (gpointer key, gpointer value, gpointer file) {
  //int line_size = strlen(key) + strlen(value) + 2;
  //char *line = (char *)malloc(line_size);
  char *line = strdup(key);
  line = realloc(line, sizeof(key)*strlen(key) + sizeof(value)*strlen(value) + 2*sizeof(char));
  line = strcat(line, "}");
  line = strcat(line, value);
  line = strcat(line, "\n");
  fputs (line,(FILE *) file);
  free(line);
}

/*
* Cette fonction vérifie dans l'index la présence du mot donné en paramètre. Elle retourne 0 si le mot existe, 1 sinon.
*/
int check_word_pres (char *word, GHashTable *hashtable) {
  if (g_hash_table_contains(hashtable, word) == TRUE) {
    return 0;
  } else {
    return 1;
  }
}

/*
* Cette fonction vérifie la présence de l'url donnée en argument pour le mot donné en argument.
* Renvoie 0 si l'URL est présente, 1 sinon.
*/
int check_url_pres (char *word, char *url, GHashTable *hashtable) {
  if (check_word_pres(word, hashtable) == 0) {
    char *value = strdup((char *)g_hash_table_lookup(hashtable, word));
    if (value != NULL) {
      char *token = strtok(value, "{");
      while (token != NULL) {
        if (strcmp(token, url) == 0) {
          free(value);
          return 0;
        } else {
          token = strtok(NULL, "{");
        }
      }
    }
    free(value);
  }
  return 1;
}

/*
* Crée une table de hachage à partir du fichier index. Cette table de hachage est utilisée pour la recherche de mots et d'URL.
*/
GHashTable *extract_hash_table () {
  //lire une ligne ; la décomposer ; stocker les données dans la table ; passer à la ligne suivante
  FILE *index_file = fopen(INDEX_PATHNAME, "r");
  char *line, *word;
  GHashTable *hashtable = g_hash_table_new(g_str_hash, g_str_equal);  //on crée la hashtable qui contiendra les données de l'index
  line = read_index_line(index_file);
  while (line != NULL) {
    word = strdup(strtok(line, "}")); //on récupère le premier token, c'est à dire le mot
    char *url_string, *url_temp;
    url_temp = strtok(NULL, "}");
    if (url_temp != NULL) {
      url_string = strdup(url_temp);
      g_hash_table_insert(hashtable, word, url_string);
    }
    line = read_index_line(index_file);
  }
  fclose(index_file);
  free(line);
  return hashtable;
}

/*
* Cette fonction permet de lire une ligne de l'index dont la longueur est inconnue et arbitrairement grande.
* L'argument file est supposé être ouvert en écriture.
* Le code a été trouvé à l'adresse : http://stackoverflow.com/questions/12167946/how-do-i-read-an-arbitrarily-long-line-in-c
*/
char *read_index_line (FILE *file) {
  char *line = NULL;
    size_t  maxlength = 0;
    assert(file != NULL);
    while(1) { // Read the line in BLOCKSIZE -blocks.
        char *crlf, *block;

        maxlength += BLOCKSIZE;
        // This exploits realloc behaviour upon hitting NULL
        if (NULL == (line = realloc(line, maxlength+1))) {
            break; // realloc error returns NULL.
        }
        block = line + maxlength - BLOCKSIZE;
        // BLOCKSIZE+1 to accommodate final zero
        if (NULL == fgets(block, BLOCKSIZE+1, file)) {
            if (block == line) {
                    // Error.
                    free(line);
                    line = NULL;
            }
            break;
        }
        // On vérifie la présence d'un caractère de retour à la ligne. Si c'est le cas, on arrête la lecture ici.
        if (NULL != (crlf = strchr(block, '\n'))) {
            *crlf = 0x0;
            if (crlf != block) {
                if ('\r' == *(--crlf))
                    *crlf = 0x0;
            }
            break;
        }
    }
    return line;
}

/*
* Cette fonction affiche la table de hachage dans sa totalité
*/
void print_hashtable (GHashTable *hashtable) {
  g_hash_table_foreach (hashtable, print_index_row, 0);
}

/*
* Fonction auxiliaire qui affiche une ligne de la table de hachage.
*/
void print_index_row (gpointer key, gpointer value, gpointer user_data) {
  printf("%s", (char *)key);
  char *token;
  char *cpyValue = strdup(value);
  token = strtok(cpyValue, "{");
  while (token != NULL) {
    printf(" ; %s", token);
    token = strtok(NULL, "{");
  }
  printf("\n");
}

/*
int main(int argc, char const *argv[]) {

  return 0;
}
*/
