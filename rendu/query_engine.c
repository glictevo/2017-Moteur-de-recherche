# include "index_functions.h"
# include <ctype.h>
# include <string.h>
# ifndef ERR_WORD_FORM
# define ERR_WORD_FORM "error word format"
# endif
# ifndef ERR_WORD_READ
# define ERR_WORD_READ "error bad reading"
# endif
# ifndef QUIT_QE
# define QUIT_QE ".quit"
# endif

/*
* Cette fonction vérifie que la requête entrée par l'utilisateur ne contient
* qu'un seul mot, composé uniquement de lettres. Si la requête contient plusieurs
* mots, la fonction ne renvoie que le premier mot.
* Si certains caractères ne sont pas des lettres, la fonction renvoie un
* message d'erreur.
*/
char *verify_request (char *word) {
  word = strtok(word, " ");
  /*Dans le cas où l'utilisateur veut fermer le programme.*/
  if (strcmp(word, QUIT_QE)==0) {
    return word;
  }
  int onlyletters = 0;
  int i = 0;
  while(word[i] != '\0') {
    if (!isalpha(word[i])) {
      onlyletters = 1;
    }
    i++;
  }
  if (onlyletters == 1) {
    return ERR_WORD_FORM;
  } else {
    return (char *)word;
  }
}

/*
* Cette fonction permet à l'utilisateur d'entrer une requête.
* Elle renvoie le mot entré par l'utilisateur, ou un message d'erreur.
*/
char *get_user_request () {
  char *request = malloc(sizeof(request)*50);
  char *fgets_result = fgets(request, 50, stdin);
  if (fgets_result == NULL) {
    return ERR_WORD_READ;
  } else {
    request = strtok(request, "\n");
    return verify_request(request);
  }
}

/*
* Cette fonction affiche les urls renvoyées par l'index.
* Si l'index a renvoyé une erreur, celle-ci sera affichée au lieu des
* urls attendues.
*/
void print_sent_urls (char *answer) {
  if (strcmp(answer, SENDURLS_ERROR)==0) {
    printf("Desolé, il n'y a pas de résultat pour votre requête.\n");
  } else {
    char *url = strtok(answer, "{");
    while (url != NULL) {
      printf("%s\n", url);
      url = strtok(NULL, "{");
    }
  }
}


int main(int argc, char const *argv[]) {
  printf("Bienvenue sur votre moteur de recherche. Veuillez entrer un mot (et une seul) pour lancer la recherche. La première recherche peut être plus longue que les autres.\n");
  printf("Attention, votre mot de peut pas faire plus de 50 caractères.\n");
  printf("Pour quitter l'application, tapez .quit \n");
  char *request = get_user_request();
  char *answer;
  GHashTable *hashtable = extract_hash_table();
  while (strcmp(request, QUIT_QE) != 0) {
    while (strcmp(request, ERR_WORD_READ)==0||strcmp(request, ERR_WORD_FORM)==0) {
      printf("Erreur : vous devez entrer un mot de moins de 50 caractères qui ne contient que des lettres.\n");
      request = get_user_request();
    }
    answer = send_urls(request, hashtable);
    print_sent_urls(answer);
    request = get_user_request();
  }
  printf("A bientôt !\n");
  free(request);
  return 0;
}
