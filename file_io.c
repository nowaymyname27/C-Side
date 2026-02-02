#include "buffer.h"
#include <stddef.h>
#include <stdio.h>

void write_to_file(TextBuffer *buffer) {
  FILE *f;
  f = fopen("test.txt", "w");
  fprintf(f, "%s", buffer->text);
  printf("File saved!\n");
  fclose(f);
}

void open_file(TextBuffer *buffer) {
  FILE *f;
  f = fopen("test.txt", "r");
  if (f != NULL) {
    fseek(f, 0, SEEK_END);
    buffer->letterCount = ftell(f);
    rewind(f);
    fread(buffer->text, 1, buffer->letterCount, f);
    buffer->text[buffer->letterCount] = '\0';
    buffer->cursorIndex = buffer->letterCount;
    printf("File opened!\n");
    fclose(f);
  }
}
