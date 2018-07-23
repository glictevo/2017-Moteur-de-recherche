CRAWLER = crawler
QUERY_ENGINE = query_engine

CC = gcc
CFLAGS =  -I../parser
LIBCURL_FLAGS = $$(pkg-config --libs --cflags libcurl)
LIBXML2_FLAGS = -I/usr/include/libxml2 -lxml2
GLIB2_FLAGS = -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include
GLIB2_FLAGS2 = -lglib-2.0
PCRE_FLAGS = -I/usr/include/pcre -lpcre
SRC = list.c crawler.c fetch_page.c MyParser.c index.c
SRC_QE = query_engine.c index.c
OBJ = $(SRC:.c=.o)
OBJ_QE = $(SRC_QE:.c=.o)

all: $(CRAWLER) $(QUERY_ENGINE)

$(CRAWLER):
	$(CC) -c $(CFLAGS) $(LIBXML2_FLAGS) $(GLIB2_FLAGS) $(SRC)
	$(CC) $(OBJ) $(GLIB2_FLAGS2) $(PCRE_FLAGS) $(LIBXML2_FLAGS) $(LIBCURL_FLAGS) -o $(CRAWLER)
	#$(CC) $(CFLAGS) $(SRC) $(GLIB2_FLAGS) $(LIBXML2_FLAGS) $(LIBCURL_FLAGS) -o $(NAME)

$(QUERY_ENGINE):
	$(CC) -c $(GLIB2_FLAGS) $(GLIB2_FLAGS2) $(SRC_QE)
	$(CC) $(OBJ_QE) $(GLIB2_FLAGS2) $(GLIB2_FLAGS) -o $(QUERY_ENGINE)

clean:
	rm -rf $(OBJ) $(OBJ_QE)

fclean: clean
	rm -rf $(CRAWLER) $(QUERY_ENGINE)

re: fclean all
