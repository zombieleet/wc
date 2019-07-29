#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "wc_struct.c"

#define DEFAULT_BUFFER_SIZE 3

void single_option(char opt, char * data);
void multiple_option(char * opt);
void process_multiple_short_option(char * opt);

int main(int argc , char ** argv ) {

  // from stdin print all argument
  if ( argc == 1 ) {
    wc.no_argument(stdin);
    return EXIT_SUCCESS;
  }

  for ( int i = 1; i < argc ; i++ ) {

    if ( argv[i][0] == '-' && argv[i][1] != '-' ) {
      if ( argv[i][2] == ' ' ) single_option(argv[i][1], NULL); //short option e.g -o
      else process_multiple_short_option(argv[i]);
      continue;
    }

    //long option e.g --output
    if ( argv[i][1] == '-' ) multiple_option(argv[i]);
    else  {
      FILE * fp = fopen(argv[i],"r");
      if ( ! fp ) {
        perror(strerror(errno));
        exit(1);
      }
      wc.no_argument(fp); //operate on file
    }

  }

  puts("\n");

  return EXIT_SUCCESS;
}

void single_option(char opt, char * data) {
  switch(opt) {
  case 'c':
    wc.character_argument(data);
    break;
  case 'w':
    wc.word_argument(data);
    break;
  case 'l':
    wc.line_argument(data);
    break;
  default:
    fprintf(stderr, "Invalid argument\n");
    exit(1);
  }
}

void multiple_option(char * opt) {
  if      (strcmp(opt,"--word") == 0) wc.word_argument(NULL);
  else if (strcmp(opt,"--line") == 0) wc.line_argument(NULL);
  else if (strcmp(opt,"--char") == 0) wc.character_argument(NULL);
  else {
    fprintf(stderr, "Invalid argument\n");
    exit(1);
  }
}

void process_multiple_short_option(char * opt) {
  
  char ** buf       = calloc(DEFAULT_BUFFER_SIZE, sizeof(char));
  char *  container = calloc(DEFAULT_BUFFER_SIZE, sizeof(char));
  size_t bufSize = DEFAULT_BUFFER_SIZE;

  int reallocBufSize = DEFAULT_BUFFER_SIZE;

  while ( ! feof(stdin) ) {

    int f = getline(buf, &bufSize , stdin);

    if ( f == -1 ) break;

    if ( ( strlen(container) + strlen(*buf)) >= reallocBufSize ) {
      reallocBufSize = strlen(container) + strlen(*buf);
      container = reallocarray(container, reallocBufSize + DEFAULT_BUFFER_SIZE, sizeof(char));
      if ( ! container ) {
        fprintf(stderr, "Error allocating memory\n");
        exit(1);
      }
    }
    strncat( container, *buf , strlen(*buf));
  }


  for ( int i = 1 ; i < strlen(opt) ; i++ ) {
    single_option(opt[i],container);
  }

  free(container);
  container = NULL;
}
