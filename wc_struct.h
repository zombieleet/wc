#ifndef _WORD_COUNT_STRUCT
#define _WORD_COUNT_STRUCT
#include <stdio.h>
#include <stdarg.h>


struct word_count {
  int  line_number;
  int  character_number;
  int  words_number;
  void (*no_argument)(FILE * stream, char * filename);
  void (*print_details)(int len, ...);
  void (*character_argument)(char * data);
  void (*word_argument)(char * data);
  void (*line_argument)(char * data);
};

extern void no_argument(FILE * stream, char * filename);
extern void print_details(int len, ...);
extern void character_argument(char * data);
extern void word_argument(char * data);
extern void line_argument(char * data);

struct word_count wc = {
  .no_argument = no_argument,
  .print_details = print_details,
  .character_argument = character_argument,
  .word_argument = word_argument,
  .line_argument = line_argument
};

#endif
