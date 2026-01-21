#ifndef BUFFER_H
#define BUFFER_H

#define BUFFER_SIZE 2049

typedef struct {
  char text[BUFFER_SIZE];
  int letterCount;
  float timer;
  int cursorIndex;
} TextBuffer;

#endif
