#include <stdio.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <string.h>
#include <pcre.h>

struct values values;
struct node node;


struct values* parseMemory(char *toParse); //Parse the char* in memory(HTML doc) and return a values struct with the word_list of the char* to parse and the URL_list
void navigateTree(xmlNode *a_node, struct values *toModify);
struct node* get_words_from_text(char* str, struct values *toModify); //Uses PCRE to get all the words from a String and add them to the word_list of toModify.
void addToList(char **list, char *toAdd); //Add a word or a URL to a list. Check if the list contains the word before adding it.
