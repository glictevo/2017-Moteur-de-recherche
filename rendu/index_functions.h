#ifndef INDEX_PATHNAME
# define INDEX_PATHNAME "index.csv"
# endif
#ifndef SENDURLS_ERROR
# define SENDURLS_ERROR "error no word"
# endif
# ifndef BLOCKSIZE
# define BLOCKSIZE 1024
# endif
# include <stdio.h>
# include <stdlib.h>
# include <glib.h>
# include <assert.h>
# include <string.h>
# include <unistd.h>

GHashTable *extract_hash_table ();
int add_word (char *word, GHashTable *hashtable);
int add_url (char *word, char *url, GHashTable *hashtable);
int remove_word (char *word, GHashTable *hashtable);
int remove_url (char *word, char *url, GHashTable *hashtable);
char *send_urls (char *word, GHashTable *hashtable);
