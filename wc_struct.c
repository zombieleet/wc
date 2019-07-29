#include <stdio.h>
#include <stdarg.h>
#include "wc_struct.h"

void no_argument(FILE * stream) {
  while ( ! feof(stream) ) {
    char c = fgetc(stream);
    if ( c == ( '\0' | EOF ) ) break;
    if ( c == '\n' ) {
      wc.line_number++;
    }
    wc.character_number++;
  }
  wc.print_details(3, wc.line_number, wc.words_number, wc.character_number);
}

void print_details(int len , ...) {

  va_list params;
  va_start(params, len);

  for ( int i = 0 ; i < len ; i++ ) {
    int d = va_arg(params, int);
    fprintf(stdout, "\t%d", d);
  }
  return ;
}

void character_argument(char * data) {

  if ( data == NULL ) {
    while ( !feof(stdin) ) {
      char c = fgetc(stdin);
      if ( c == ( '\0' | '\n' | EOF ) ) break;
      wc.character_number++;
    }
    wc.print_details(1, wc.character_number);
    return;
  }

  for ( int i = 0 ; i < strlen(data); i++ ) {
    if ( data[i] == '\0' ) continue;
    wc.character_number++;
  }

  wc.print_details(1, wc.character_number);
  return;

}

void line_argument(char * data) {
  if ( data == NULL ) {
    while (!feof(stdin)) {
      char c = fgetc(stdin);
      if ( c == EOF ) break;
      if ( c == '\n' ) wc.line_number++;
    }
    wc.print_details(1, wc.line_number);
    return;
  }

  for ( int i = 0; i < strlen(data); i++ ) {
    if ( data[i] == '\n' ) wc.line_number++;
  }
  wc.print_details(1,wc.line_number);
  return;
}

void word_argument(char * data) {
  //puts("word");
}
