#include <stdio.h>
#include <stdarg.h>
#include "wc_struct.h"

void read_no_argument_data(struct file_data * stream_ds) {

  FILE * stream = stream_ds->stream != NULL ? stream_ds->stream : stdin;

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

  wc.print_details(3, stream_ds->line_number, stream_ds->words_count, stream_ds->character_number , stream_ds->filename);
}

void no_argument(struct file_data * stream_ds) {
  // if we are reading from stdin
  // then stream_ds->stream should be null
  if ( stream_ds->stream == NULL ) return read_no_argument_data(stream_ds);

  // when we are reading from a file when there is no argument

  for ( stream_ds ; stream_ds->next != NULL ;  ) {
    read_no_argument_data(stream_ds);
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

  if ( fname != NULL ) fprintf(stdout, "\t%s", fname);

  fprintf(stdout, "\n");

  return ;
}

/* void character_argument(char * data) { */

/*   if ( data == NULL ) { */
/*     while ( !feof(stdin) ) { */
/*       char c = fgetc(stdin); */
/*       if ( c == ( '\0' | '\n' | EOF ) ) break; */
/*       wc.character_number++; */
/*     } */
/*     wc.print_details(1, wc.character_number); */
/*     return; */
/*   } */

/*   for ( int i = 0 ; i < strlen(data); i++ ) { */
/*     if ( data[i] == '\0' ) continue; */
/*     wc.character_number++; */
/*   } */

/*   wc.print_details(1, wc.character_number); */
/*   return; */

/* } */

/* void line_argument(char * data) { */
/*   if ( data == NULL ) { */
/*     while (!feof(stdin)) { */
/*       char c = fgetc(stdin); */
/*       if ( c == EOF ) break; */
/*       if ( c == '\n' ) wc.line_number++; */
/*     } */
/*     wc.print_details(1, wc.line_number); */
/*     return; */
/*   } */

/*   for ( int i = 0; i < strlen(data); i++ ) { */
/*     if ( data[i] == '\n' ) wc.line_number++; */
/*   } */
/*   wc.print_details(1,wc.line_number); */
/*   return; */
/* } */

/* void word_argument(char * data) { */
/*   //puts("word"); */
/* } */
