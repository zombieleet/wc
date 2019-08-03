#include <stdio.h>
#include <stdarg.h>
#include "wc_struct.h"


#define SWITCH(arg)                             \
  call_site = 1;                                \
  call_site_value = arg
#define CASE(arg,ops)                                       {      \
    if ( call_site != 1 ) {                                        \
      fprintf(stderr, "no SWITCH macro called\n");                 \
      exit(1);                                                  \
    }                                                           \
    if ( strcmp(arg,call_site_value) != 0) ops;      \
  }


int call_site  = 0;
int call_out   = 0;

void * call_site_value = NULL;

void selective_output_for_options(struct file_data * stream_ds , FILE * stream, struct operation_argument * ops_argument) {

  if ( ops_argument->CHARACTER_ARGUMENT == 1 && ops_argument->WORDS_COUNT_ARGUMENT == 1 ) {
    wc.character_argument(stream_ds,stream).word_argument(stream_ds,stream);
    wc.print_details(2, stream_ds->character_number, stream_ds->words_count, stream_ds->filename);
    return;
  }

  if ( ops_argument->CHARACTER_ARGUMENT == 1 && ops_argument->LINE_ARGUMENT == 1  ) {
    wc.line_argument(stream_ds,stream).character_argument(stream_ds,stream);
    wc.print_details(2, stream_ds->line_number, stream_ds->character_number, stream_ds->filename);
    return;
  }

  if ( ops_argument->LINE_ARGUMENT == 1 && ops_argument->WORDS_COUNT_ARGUMENT == 1 ) {
    wc.line_argument(stream_ds,stream).word_argument(stream_ds,stream);
    wc.print_details(2, stream_ds->line_number, stream_ds->words_count, stream_ds->filename);
    return;
  }

  // one of this below conditionals will be true
  // if the above fails

  if ( ops_argument->CHARACTER_ARGUMENT == 1 ) {
    wc.character_argument(stream_ds,stream);
    wc.print_details(1, stream_ds->character_number, stream_ds->filename);
    return;
  }

  if ( ops_argument->LINE_ARGUMENT == 1 ) {
    wc.line_argument(stream_ds,stream);
    wc.print_details(1, stream_ds->line_number, stream_ds->filename);
    return;
  }

  if ( ops_argument->WORDS_COUNT_ARGUMENT == 1 ) {
    wc.word_argument(stream_ds,stream);
    wc.print_details(1, stream_ds->words_count, stream_ds->filename);
    return;
  }

  return;
}

void read_start_operation_data(struct file_data * stream_ds, struct operation_argument * ops_argument) {

  FILE * stream = stream_ds->stream != NULL ? stream_ds->stream : stdin;

  if ( ops_argument == NULL )
    while ( ! feof(stream) ) {
      char c = fgetc(stream);
      if ( c == ( '\0' | EOF ) ) break;
      if ( c == '\n' ) {
        stream_ds->line_number++;
      }
      stream_ds->character_number++;
    }

  if ( wc.length > 1 ) {
    wc.total.line_number_total      += stream_ds->line_number;
    wc.total.words_count_total      += stream_ds->words_count;
    wc.total.character_number_total += stream_ds->character_number;
  }

  if ( ops_argument == NULL ) {
    wc.print_details(3, stream_ds->line_number, stream_ds->words_count, stream_ds->character_number , stream_ds->filename);
    return;
  }

  return selective_output_for_options(stream_ds, stream , ops_argument);
}

void start_operation(struct file_data * stream_ds,  struct operation_argument * ops_argument ) {
  // if we are reading from stdin
  // then stream_ds->stream should be null
  if ( stream_ds->stream == NULL ) return read_start_operation_data(stream_ds,NULL);

  // when we are reading from a file when there is no argument
  for ( stream_ds ; stream_ds->next != NULL ;  ) {
    read_start_operation_data(stream_ds,ops_argument);
    stream_ds = stream_ds->next;
    if ( stream_ds->previous ) {
      free(stream_ds->previous);
      stream_ds->previous = NULL;
    }
  }
}

void print_details(int len , ...) {

  va_list params;
  va_start(params, len);

  for ( int i = 0 ; i < len ; i++ ) {
    int d = va_arg(params, int);
    fprintf(stdout, "\t%d", d);
  }

  char * fname = va_arg(params, char *);

  if ( fname != NULL ) fprintf(stdout, "\t%s\n", fname);

  return ;
}
struct word_count_ops character_argument(struct file_data * stream_ds, FILE * stream) {
  while ( !feof(stream) ) {
    char c = fgetc(stream);
    if ( c == ( '\0' | '\n' | EOF ) ) break;
    stream_ds->character_number++;
  }
  return wc;
}

struct word_count_ops line_argument(struct file_data * stream_ds, FILE * stream) {
  while (!feof(stream)) {
    char c = fgetc(stream);
    if ( c == EOF ) break;
    if ( c == '\n' ) stream_ds->line_number++;
  }
  return wc;
}

struct word_count_ops word_argument(struct file_data * stream_ds, FILE * stream) {
  //puts("word");
  return wc;
}
