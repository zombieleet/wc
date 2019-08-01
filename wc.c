#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "wc_struct.c"

#define DEFAULT_BUFFER_SIZE 3

void single_option(char opt, char * data);
void multiple_option(char * opt, struct operation_argument * ops_argument);
void process_multiple_short_option(char * opt , struct operation_argument * ops_argument);
void process_argument_for_files();

int main(int argc , char ** argv ) {

  struct operation_argument * ops_argument = malloc(sizeof(struct operation_argument));
  struct file_data          * stream_ds    = malloc(sizeof(struct file_data));

  if ( ! stream_ds || ! ops_argument ) {
    perror(strerror(errno));
    return EXIT_FAILURE;
  }

  // from stdin print all argument
  if ( argc == 1 ) {
    wc.no_argument(stream_ds);
    return EXIT_SUCCESS;
  }

  struct file_data * HEAD = stream_ds;

  for ( int i = 1; i < argc ; i++ ) {

    if ( argv[i][0] == '-' && argv[i][1] != '-' ) {
      if ( argv[i][2] == ' ' ) single_option(argv[i][1], NULL); //short option e.g -o
      else process_multiple_short_option(argv[i], ops_argument);
      continue;
    }

    //long option e.g --output
    if ( argv[i][1] == '-' ) {
      multiple_option(argv[i],ops_argument);
    } else  {
      FILE * fp = fopen(argv[i],"r");
      if ( ! fp ) {
        perror(strerror(errno));
        exit(1);
      }

      // when we get a file name as an argument we add it to the data structure
      // a file name is any argument that does not start with a single or double - (dash)

      stream_ds->filename       = argv[i];
      stream_ds->stream         = fp;

      stream_ds->next           = malloc(sizeof(struct file_data));
      stream_ds->next->previous = stream_ds;
      // replace stream_ds with the next pointer
      stream_ds                 = stream_ds->next;

      wc.length += 1;
      //wc.no_argument(fp,argv[i]); //operate on file
    }

  }

  /**
     This conditional statement handles a situation were
     no option was specified except file names
   **/

  if (
      ops_argument->CHARACTER_ARGUMENT   == 0  &&
      ops_argument->LINE_ARGUMENT        == 0  &&
      ops_argument->WORDS_COUNT_ARGUMENT == 0
      ) wc.no_argument(HEAD);


  // if more than one file was specified as a parameter
  // this will show the total of computed lines,words and character of the whole file
  if ( wc.length > 1 )
    wc.print_details(3,wc.total.line_number_total,wc.total.words_count_total,wc.total.character_number_total, "total");

  free(stream_ds);
  free(ops_argument);

  stream_ds    = NULL;
  ops_argument = NULL;

  return EXIT_SUCCESS;
}

void single_option(char opt, char * data) {
  switch(opt) {
  case 'c':
    //wc.character_argument(data);
    break;
  case 'w':
    //wc.word_argument(data);
    break;
  case 'l':
    //wc.line_argument(data);
    break;
  default:
    fprintf(stderr, "Invalid argument\n");
    exit(1);
  }
}

void multiple_option(char * opt, struct operation_argument * ops_argument ) {

  if      (strcmp(opt,"--word") == 0) ops_argument->WORDS_COUNT_ARGUMENT = 1;
  else if (strcmp(opt,"--line") == 0) ops_argument->LINE_ARGUMENT = 1;
  else if (strcmp(opt,"--char") == 0) ops_argument->CHARACTER_ARGUMENT = 1;
  else {
    fprintf(stderr, "Invalid argument\n");
    exit(1);
  }
}

// arguments like -lwc where l is --line , w is --word and c is --chars
void process_multiple_short_option(char * opt , struct operation_argument * ops_argument) {

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
