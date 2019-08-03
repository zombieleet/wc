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


struct operation_argument {
  int LINE_ARGUMENT;
  int CHARACTER_ARGUMENT;
  int WORDS_COUNT_ARGUMENT;
};


struct word_count_ops {
  void (*start_operation)(struct file_data * stream_ds, struct operation_argument * ops_argument);
  void (*print_details)(int len, ...);
  struct word_count_ops (*character_argument)(struct file_data * stream_ds, FILE * stream);
  struct word_count_ops (*word_argument)(struct file_data * stream_ds, FILE * stream);
  struct word_count_ops (*line_argument)(struct file_data * stream_ds, FILE * stream);
  int  length;
  struct total_struct total;
};

extern void start_operation(struct file_data * stream_ds,  struct operation_argument * ops_argument);
extern void print_details(int len, ...);
extern struct word_count_ops character_argument(struct file_data * stream_ds, FILE * stream);
extern struct word_count_ops word_argument(struct file_data * stream_ds, FILE * stream);
extern struct word_count_ops line_argument(struct file_data * stream_ds, FILE * stream);

struct word_count_ops wc = {
  .start_operation = start_operation,
  .print_details = print_details,
  .character_argument = character_argument,
  .word_argument = word_argument,
  .line_argument = line_argument
};

#endif
