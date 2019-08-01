#ifndef _WORD_COUNT_STRUCT
#define _WORD_COUNT_STRUCT
#include <stdio.h>
#include <stdarg.h>


struct file_data {
  int line_number;
  int character_number;
  int words_count;
  char * filename;
  FILE * stream;
  struct file_data * next;
  struct file_data * previous;
};

struct total_struct {
  int line_number_total;
  int character_number_total;
  int words_count_total;
};


struct word_count_ops {
  void (*no_argument)(struct file_data * stream_ds);
  void (*print_details)(int len, ...);
  void (*character_argument)(char * data);
  void (*word_argument)(char * data);
  void (*line_argument)(char * data);
  int  length;
  struct total_struct total;
};

struct operation_argument {
  int LINE_ARGUMENT;
  int CHARACTER_ARGUMENT;
  int WORDS_COUNT_ARGUMENT;
};

extern void no_argument(struct file_data * stream_ds);
extern void print_details(int len, ...);
extern void character_argument(char * data);
extern void word_argument(char * data);
extern void line_argument(char * data);

struct word_count_ops wc = {
  .no_argument = no_argument,
  .print_details = print_details
  //  .character_argument = character_argument,
  //  .word_argument = word_argument,
  // .line_argument = line_argument
};

#endif
