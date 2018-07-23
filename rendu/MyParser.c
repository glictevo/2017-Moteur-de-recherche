#include "MyParser.h"

struct node {
  char *data;
  struct node *next;
};

struct values {
  struct node *word_list;
  struct node *URL_list;
};

  /*
  * Creates a new Node, with a next poiting to NULL.
  * The value of the new node created will be the char* passed in arguments.
  */

struct node* newNode(char *data){
  struct node *new = (struct node*) malloc (sizeof(node));
  new->data = data;
  new->next = NULL;
  return new;
}

  /*
  * Creates a new Node, and add them at the start of the chain.
  * The value of the new node created will be the char* passed in arguments.
  * The value of next will be passed in argument in the form of a node.
  */

struct node* insertFirst(char *data, struct node *next){
  struct node *newFirst = (struct node*) malloc (sizeof(node));
  newFirst->data = strdup(data);
  newFirst->next = next;
  return newFirst;
}

  /*
  * Creates a new values containing a word_list and a URL_list being new node of char* data passed in argument.
  */

struct values* newValues(char* data){
  struct values *new = (struct values*) malloc (sizeof(values));
  new->word_list = newNode(data);
  new->URL_list = newNode(data);
  return new;
}

  /*
  * Extracts all the words in the string str passed in argument and add them to the word_list of toModify.
  * A word is composed solely of letters, capitalized or not, accentuated or not.
  * This function is utf-8 compatible if pcre has been built with --utf-enabled.
  */

struct node* get_words_from_text(char* str, struct values *toModify){
  const char *error;
  int erroffset;
  pcre *re;
  int rc;
  int ovector[100];

  char *regex = "[A-zÀ-ú]+";

  re = pcre_compile (regex,          /* the pattern */
                     PCRE_UTF8,
                     &error,         /* for error message */
                     &erroffset,     /* for error offset */
                     0);             /* use default character tables */
  if (!re)
  {
      printf("pcre_compile failed (offset: %d), %s\n", erroffset, error);
      return NULL;
  }

  int offset = 0;
  int len    = strlen(str);
  while (offset < len && (rc = pcre_exec(re, 0, str, len, offset, 0, ovector, sizeof(ovector))) >= 0)
  {
      for(int i = 0; i < rc; i++)
      {
        char* substr = strndup(str+ovector[2*i], ovector[2*i+1] - ovector[2*i]); /* Extract the word from str, starting at char ovector[2*i], and of length ovector[2*i+1] - ovector[2*i] */
        toModify->word_list = insertFirst(substr, toModify->word_list);  /* Add the word found to the word_list */
        free(substr);
      }
      offset = ovector[1];
  }
  return toModify->word_list;
}

 /*
 * Gets and returns the word_list and URL_list of the HTML document stored in memory in the form of a char*.
 */

struct values* parseMemory(char *toParse){
  struct values *inThisDoc = newValues("End of List\n");

  xmlDoc *doc = NULL;
  xmlNode *root_element = NULL;

  doc = htmlReadMemory(toParse, strlen(toParse), NULL, NULL, 0); /* Builds the tree corresponding to the HTML document. */
  if(doc == NULL){
    printf("Error in parsing the document\n");
  }
  root_element = xmlDocGetRootElement(doc); /* Retrieves the root_element of the document. */
  if(root_element == NULL){
    printf("Error in retrieving the root element\n");
  }

  navigateTree(root_element, inThisDoc); /* Fills the inThisDoc values with the word_list and URL_list found in the document. */

  xmlFreeDoc(doc);

  return inThisDoc;
}

  /*
  * Recursively navigates the tree starting at the node passed in arguments.
  * Adds the words found in the document to the word_list of the toModify values.
  * Add the urls found in the document to the URL_list of the toModify values.
  */

void navigateTree(xmlNode *a_node, struct values *toModify){
  xmlNode *cur_node = NULL; /* The current xmlNode we will retrieve info on */
  xmlAttr *cur_attr = NULL;
  xmlChar *attr_value = NULL;

  for(cur_node = a_node; cur_node; cur_node=cur_node->next){
    if(cur_node->type == XML_TEXT_NODE){ /* Checks if cur_node is a text node */
      if(cur_node->content != NULL && xmlStrcmp(cur_node->content, (const xmlChar *)" ")){
        char *sentence = cur_node->content;
        toModify->word_list = get_words_from_text(sentence, toModify);
      }
    }
    for (cur_attr = cur_node->properties; cur_attr; cur_attr = cur_attr->next) {
      if(!xmlStrcmp(cur_attr->name, (const xmlChar *)"href")){
        attr_value = xmlGetProp(cur_node, "href");
        toModify->URL_list = insertFirst(attr_value, toModify->URL_list);
      }
    }
    navigateTree(cur_node->children, toModify); /* Recursively calls the function on the children of cur_node */
  }
}

/*int main(int argc, char const *argv[]) {
  struct values *inThisDoc = malloc(sizeof(values));
  char *buffer = malloc(10000*sizeof(char));
  FILE *fd = fopen(argv[1],"r");
  fread(buffer, 10000, 1, fd);
  inThisDoc = parseMemory(buffer);
  struct node *cur_node;
  for(cur_node = inThisDoc->word_list; cur_node; cur_node=cur_node->next){
    printf("%s\n", cur_node->data);
  }
  for(cur_node = inThisDoc->URL_list; cur_node; cur_node=cur_node->next){
    printf("%s\n", cur_node->data);
  }
  return 0;
}
*/
